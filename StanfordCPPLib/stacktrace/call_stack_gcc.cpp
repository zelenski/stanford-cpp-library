/*
 * File: base64.h
 * --------------
 * Linux/gcc implementation of the call_stack class.
 *
 * @author Marty Stepp, based on code from Fredrik Orderud
 * @version 2015/05/28
 */

/* Copyright (c) 2009, Fredrik Orderud
   License: BSD licence (http://www.opensource.org/licenses/bsd-license.php)
   Based on: http://stupefydeveloper.blogspot.com/2008/10/cc-call-stack.html */

#ifdef __GNUC__
#include <stdio.h>
#include <cxxabi.h>
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#else
#include <execinfo.h>
#include <dlfcn.h>
#endif // _WIN32
#endif // __GNUC__
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "call_stack.h"
#include "exceptions.h"
#include "strlib.h"
#include "platform.h"

namespace stacktrace {
static void* fakeCallStackPointer = NULL;

/*
 * Run a sub-process and capture its output.
 */
int execAndCapture(std::string cmd, std::string& output) {
#ifdef _WIN32
    // Windows code for external process (ugly)
    HANDLE g_hChildStd_IN_Rd = NULL;
    HANDLE g_hChildStd_IN_Wr = NULL;
    HANDLE g_hChildStd_OUT_Rd = NULL;
    HANDLE g_hChildStd_OUT_Wr = NULL;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0)) {
        return 1;   // fail
    }
    if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) {
        return 1;   // fail
    }
    if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0)) {
        return 1;   // fail
    }
    if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) ) {
        return 1;   // fail
    }

    // CreateChildProcess();
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory( &siStartInfo, sizeof(STARTUPINFOA) );
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = g_hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
    siStartInfo.hStdInput = g_hChildStd_IN_Rd;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    if (!CreateProcessA(
            NULL,
            (char*) cmd.c_str(),   // command line
            NULL,                  // process security attributes
            NULL,                  // primary thread security attributes
            TRUE,                  // handles are inherited
            CREATE_NO_WINDOW,      // creation flags
            NULL,                  // use parent's environment
            NULL,                  // use parent's current directory
            &siStartInfo,          // STARTUPINFO pointer
            &piProcInfo)) {        // receives PROCESS_INFORMATION
        std::cerr << "CREATE PROCESS FAIL: " << getPlatform()->os_getLastError() << std::endl;
        std::cerr << cmd << std::endl;
        return 1;   // fail
    }

    // close the subprocess's handles (waits for it to finish)
    WaitForSingleObject(piProcInfo.hProcess, INFINITE);
    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);

    // ReadFromPipe();
    DWORD dwRead;
    const int BUFSIZE = 65536;
    CHAR chBuf[BUFSIZE] = {0};
    if (!ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, NULL) || dwRead == 0) {
        return 1;
    }
    std::ostringstream out;
    for (int i = 0; i < (int) dwRead; i++) {
        out.put(chBuf[i]);
    }

    output = out.str();
    return 0;
#else
    // Linux / Mac code for external process
    cmd += " 2>&1";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        return -1;
    }
    char buffer[65536] = {0};
    output = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 65536, pipe) != NULL) {
            output += buffer;
        }
    }
    return pclose(pipe);
#endif // _WIN32
}

std::string addr2line_clean(std::string line) {
#if defined(_WIN32)
    // TODO: implement on Windows
    // "ZN10stacktrace25print_stack_trace_windowsEv at C:\Users\stepp\Documents\StanfordCPPLib\build\stanfordcpplib-windows-Desktop_Qt_5_3_MinGW_32bit-Debug/../../StanfordCPPLib/stacktrace/call_stack_windows.cpp:126"
#elif defined(__APPLE__)
    // Mac OS X version (atos)
    // "Vector<int>::checkIndex(int) const (in Autograder_QtCreatorProject) (vector.h:764)"
    if (line.find(" (") != std::string::npos) {
        line = line.substr(line.rfind(" (") + 2);
    }
    if (line.find(')') != std::string::npos) {
        line = line.substr(0, line.rfind(')'));
    }
    line = trim(line);
#elif defined(__GNUC__)
    // Linux version (addr2line)
    // "_Z4Mainv at /home/stepp/.../FooProject/src/mainfunc.cpp:131"
    if (line.find(" at ") != std::string::npos) {
        line = line.substr(line.rfind(" at ") + 4);
    }
    if (line.find('/') != std::string::npos) {
        line = line.substr(line.rfind('/') + 1);
    }

    // strip extra parenthesized info from the end
    if (line.find(" (") != std::string::npos) {
        line = line.substr(0, line.rfind(" ("));
    }
    line = trim(line);
#endif
    return line;
}

int addr2line_all(std::vector<void*> addrsVector, std::string& output) {
    int length = (int) addrsVector.size();
    void* addrs[length];
    for (int i = 0; i < length; i++) {
        addrs[i] = addrsVector[i];
    }
    return addr2line_all(addrs, length, output);
}

int addr2line_all(void** addrs, int length, std::string& output) {
    // turn the addresses into a space-separated string
    std::ostringstream out;
    for (int i = 0; i < length; i++) {
        out << " " << std::hex << std::setfill('0') << addrs[i];
    }
    std::string addrsStr = out.str();
    out.str("");

    // have addr2line map the address to the relent line in the code
#if defined(__APPLE__)
    // Mac OS X
    out << "atos -o " << exceptions::getProgramNameForStackTrace() << addrsStr;
#elif defined(_WIN32)
    // Windows
    out << "addr2line.exe -f -i -C -s -p -e \"" << exceptions::getProgramNameForStackTrace() << "\"" << addrsStr;
#else
    // Linux
    out << "addr2line -f -i -C -s -p -e " << exceptions::getProgramNameForStackTrace() << addrsStr;
#endif
    std::string command = out.str();
    int result = execAndCapture(command, output);
    return result;
}

/*
 * Resolve symbol name and source location given the path to the executable
 * and an address
 */
int addr2line(void* addr, std::string& line) {
    void* addrs[1] = {addr};
    return addr2line_all(addrs, 1, line);
}

void* getFakeCallStackPointer() {
    return fakeCallStackPointer;
}

void setFakeCallStackPointer(void* ptr) {
    fakeCallStackPointer = ptr;
}
} // namespace stacktrace


/*
 * Below is the Linux/Mac-specific stack trace code.
 */
#ifndef _WIN32

namespace stacktrace {
const int WIN_STACK_FRAMES_TO_SKIP = 0;
const int WIN_STACK_FRAMES_MAX = 20;

call_stack::call_stack(const size_t /*num_discard = 0*/) {
    using namespace abi;

    // retrieve call-stack
    void* trace[WIN_STACK_FRAMES_MAX];
    int stack_depth = backtrace(trace, WIN_STACK_FRAMES_MAX);

    // let's also try to get the line numbers via an external process
    std::string addr2lineOutput;
    std::vector<std::string> addr2lineLines;
    if (stack_depth > 0) {
        addr2line_all(trace, stack_depth, addr2lineOutput);
        addr2lineLines = stringSplit(addr2lineOutput, "\n");
    }
    
    for (int i = WIN_STACK_FRAMES_TO_SKIP; i < stack_depth; i++) {
        Dl_info dlinfo;
        if (!dladdr(trace[i], &dlinfo)) {
            continue;
        }

        const char* symname = dlinfo.dli_sname;

        int   status;
        char* demangled = abi::__cxa_demangle(symname, NULL, 0, &status);
        if (status == 0 && demangled) {
            symname = demangled;
        }
        
        // store entry to stack
        if (dlinfo.dli_fname && symname) {
            entry e;
            e.file     = dlinfo.dli_fname;
            e.line     = 0; // unsupported
            e.function = symname;
            
            // let's try to get the line number
            // addr2line(trace[i], e.lineStr);
            if (i < (int) addr2lineLines.size()) {
                e.lineStr = addr2line_clean(addr2lineLines[i]);
            }
            
            stack.push_back(e);
        } else {
            continue; // skip last entries below main
        }

        if (demangled) {
            free(demangled);
        }
    }
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#endif // _WIN32
