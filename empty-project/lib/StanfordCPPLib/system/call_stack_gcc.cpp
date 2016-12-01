/*
 * File: call_stack_gcc.cpp
 * ------------------------
 * Linux/gcc implementation of the call_stack class.
 *
 * @author Marty Stepp, based on code from Fredrik Orderud
 * @version 2016/12/01
 * - bug fixes for call stack line number retrieval
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
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
#include "private/platform.h"
#include "private/static.h"

namespace stacktrace {

/*
 * Run a sub-process and capture its output.
 */
int execAndCapture(std::string cmd, std::string& output) {
#ifdef _WIN32
    // Windows code for external process (ugly)
    HANDLE g_hChildStd_IN_Rd = nullptr;
    HANDLE g_hChildStd_IN_Wr = nullptr;
    HANDLE g_hChildStd_OUT_Rd = nullptr;
    HANDLE g_hChildStd_OUT_Wr = nullptr;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = nullptr;
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
            nullptr,
            (char*) cmd.c_str(),   // command line
            nullptr,               // process security attributes
            nullptr,               // primary thread security attributes
            TRUE,                  // handles are inherited
            CREATE_NO_WINDOW,      // creation flags
            nullptr,               // use parent's environment
            nullptr,               // use parent's current directory
            &siStartInfo,          // STARTUPINFO pointer
            &piProcInfo)) {        // receives PROCESS_INFORMATION
        std::cerr << "CREATE PROCESS FAIL: " << stanfordcpplib::getPlatform()->os_getLastError() << std::endl;
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
    if (!ReadFile(g_hChildStd_OUT_Rd, chBuf, BUFSIZE, &dwRead, nullptr) || dwRead == 0) {
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
        if (fgets(buffer, 65536, pipe) != nullptr) {
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
    // JL : change "atos" to "xcrun atos"?
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

void*& fakeCallStackPointer() {
    static void* fakeCallStackPointer = nullptr;
    return fakeCallStackPointer;
}
} // namespace stacktrace


/*
 * Below is the Linux/Mac-specific stack trace code.
 */
#ifndef _WIN32

namespace stacktrace {
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_TO_SKIP, 0)
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_MAX, 20)

std::ostream& operator <<(std::ostream& out, const entry& ent) {
    return out << ent.toString();
}

call_stack::call_stack(const size_t /*num_discard = 0*/) {
    // retrieve call-stack as an array of void* pointers to each function on stack
    void* trace[STATIC_VARIABLE(STACK_FRAMES_MAX)];
    for (int i = 0; i < STATIC_VARIABLE(STACK_FRAMES_MAX); i++) {
        trace[i] = nullptr;
    }
    int stack_depth = backtrace(trace, STATIC_VARIABLE(STACK_FRAMES_MAX));

    // First pass: read linker symbol info and get address offsets.
    for (int i = STATIC_VARIABLE(STACK_FRAMES_TO_SKIP); i < stack_depth; i++) {
        // DL* = programmer API to dynamic linking loader

        // https://linux.die.net/man/3/dladdr
        // const char *dli_fname;   // pathname of shared object that contains address
        // void       *dli_fbase;   // address at which shared object is loaded
        // const char *dli_sname;   // name of nearest symbol with address lower than addr
        // void       *dli_saddr;   // exact address of symbol named in dli_sname

        Dl_info dlinfo;
        if (!dladdr(trace[i], &dlinfo)) {
            continue;
        }

        // debug code left in because we occasionally need to debug stack traces
        // std::cout << i << "  ptr=" << trace[i] << "  dlinfo: "
        //           << " fname=" << (dlinfo.dli_fname ? dlinfo.dli_fname : "null")
        //           << " fbase=" << dlinfo.dli_fbase
        //           << " sname=" << (dlinfo.dli_sname ? dlinfo.dli_sname : "null")
        //           << " saddr=" << dlinfo.dli_saddr << std::endl;

        const char* symname = dlinfo.dli_sname;

        int   status;
        char* demangled = abi::__cxa_demangle(symname, /* buffer */ nullptr,
                                              /* length pointer */ nullptr, &status);
        if (status == 0 && demangled) {
            symname = demangled;
        }
        
        // store entry to stack
        if (dlinfo.dli_fname && symname) {
            entry e;
            e.file     = dlinfo.dli_fname;
            e.line     = 0;   // unsupported; use lineStr instead (later)
            e.function = symname;
            e.address  = trace[i];

            // The dli_fbase gives an overall offset into the file itself;
            // the dli_saddr is the offset of that symbol/function/line.
            // by subtracting them we get the offset of the function within the file
            // which addr2line can use to look up function line numbers.

            if (dlinfo.dli_fbase > 0 && trace[i] >= dlinfo.dli_fbase) {
                e.address2 = (void*) ((long) trace[i] - (long) dlinfo.dli_fbase);
            } else {
                e.address2 = dlinfo.dli_saddr;
            }
            stack.push_back(e);
        }

        if (demangled) {
            free(demangled);
        }
    }

    if (stack_depth == 0 || stack.empty()) {
        return;
    }

    // Second pass: try to look up line numbers.
    //
    // let's also try to get the line numbers via an 'addr2line' external process
    // (for max compatibility with GCC and Clang, we look up the addresses 2 ways:
    // 1) by the raw void* given to us from backtrace(), and
    // 2) by the offsetted pointer where we subtract the addr of the exe file.
    // Option 1 used to work for all compilers, but sometime around summer 2016
    // GCC started failing unless we use option 2.
    // Clang and other compilers still need option 1 and fail with option 2,
    // and to avoid running external addr2line process twice, we just look it up
    // both ways and then figure out which one is best by string length.
    // The failing one will emit a lot of short "??:?? 0" lines.

    std::vector<void*> addrsToLookup;
    for (const entry& e : stack) {
        addrsToLookup.push_back(e.address);
        addrsToLookup.push_back(e.address2);
    }

    std::string addr2lineOutput;
    addr2line_all(addrsToLookup, addr2lineOutput);
    std::vector<std::string> addr2lineLines = stringSplit(addr2lineOutput, "\n");
    int numAddrLines = (int) addr2lineLines.size();
    for (int i = 0, size = (int) stack.size(); i < size; i++) {
        std::string opt1 = (2 * i < numAddrLines ? addr2lineLines[2 * i] : std::string());
        std::string opt2 = (2 * i + 1 < numAddrLines ? addr2lineLines[2 * i + 1] : std::string());
        std::string best = opt1.length() > opt2.length() ? opt1 : opt2;
        stack[i].lineStr = addr2line_clean(best);
    }
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#endif // _WIN32
