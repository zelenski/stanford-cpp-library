/*
 * File: call_stack_gcc.cpp
 * ------------------------
 * Linux/gcc implementation of the call_stack class.
 *
 * @author Marty Stepp, based on code from Fredrik Orderud
 * @version 2019/04/16
 * - bug fix for stack trace line numbers on some Mac systems (thanks to Julie Zelenski)
 * @version 2018/10/22
 * - bug fix for STL vector vs Stanford Vector
 * @version 2018/10/18
 * - added addr2line_functionName to resolve some function names not in backtrace
 * - improved calculation of function offsets for better stack trace resolving
 * @version 2017/10/18
 * - small bug fix for pointer comparison
 * @version 2017/09/02
 * - small bug fix for new clang warning about ordered comparison between pointer and zero
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

#define INTERNAL_INCLUDE 1
#ifdef __GNUC__
#include <stdio.h>
#include <cxxabi.h>
#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#undef MOUSE_EVENT
#undef KEY_EVENT
#undef MOUSE_MOVED
#undef HELP_KEY
#else // _WIN32
#include <execinfo.h>
#include <dlfcn.h>
#include <cstring>
#endif // _WIN32
#endif // __GNUC__
#if defined(__APPLE__)
#include <mach-o/dyld.h>   // for _dyld_get_image_header
#endif // __APPLE__
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#define INTERNAL_INCLUDE 1
#include "call_stack.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

namespace platform {
std::string os_getLastError() {
#ifdef _WIN32
    // Windows error-reporting code
    DWORD lastErrorCode = ::GetLastError();
    char* errorMsg = nullptr;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    ::FormatMessageA(
                   /* dwFlags */ FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   /* lpSource */ nullptr,
                   /* dwMessageId */ lastErrorCode,
                   /* dwLanguageId */ LANG_NEUTRAL,
                   /* lpBuffer */ reinterpret_cast<LPSTR>(&errorMsg),
                   /* dwSize */ 0,
                   /* arguments */ nullptr);
    if (errorMsg) {
        return std::string(errorMsg);
    } else {
        return "";
    }
#else
    // Linux/Mac error-reporting code
    return std::string(strerror(errno));
#endif // _WIN32
}
} // namespace platform

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
            const_cast<char*>(cmd.c_str()),    // command line
            nullptr,                           // process security attributes
            nullptr,                           // primary thread security attributes
            TRUE,                              // handles are inherited
            CREATE_NO_WINDOW,                  // creation flags
            nullptr,                           // use parent's environment
            nullptr,                           // use parent's current directory
            &siStartInfo,                      // STARTUPINFO pointer
            &piProcInfo)) {                    // receives PROCESS_INFORMATION
        std::cerr << "CREATE PROCESS FAIL: " << platform::os_getLastError() << std::endl;
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
    for (int i = 0; i < static_cast<int>(dwRead); i++) {
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

std::string addr2line_functionName(std::string line) {
#if defined(_WIN32)
    // TODO: implement on Windows
    // "ZN10stacktrace25print_stack_trace_windowsEv at C:\Users\stepp\Documents\StanfordCPPLib\build\stanfordcpplib-windows-Desktop_Qt_5_3_MinGW_32bit-Debug/../../StanfordCPPLib/stacktrace/call_stack_windows.cpp:126"
#elif defined(__APPLE__)
    // Mac OS X version (atos)
    // "Vector<int>::checkIndex(int) const (in Autograder_QtCreatorProject) (vector.h:764)"
    if (line.find(" (") != std::string::npos) {
        line = line.substr(0, line.rfind(" (") - 1);
    }
    if (line.find("(in ") != std::string::npos) {
        line = line.substr(0, line.rfind("(in "));
    }
    line = trim(line);
#elif defined(__GNUC__)
    // Linux version (addr2line)
    // "_Z4Mainv at /home/stepp/.../FooProject/src/mainfunc.cpp:131"
    // "std::_Function_handler<void (), stanfordcpplib::autograder::GuiAutograder::runTest(stanfordcpplib::autograder::AutograderTest*)::{lambda()#1}>::_M_invoke(std::_Any_data const&) at std_function.h:318"
    if (line.find(" at ") != std::string::npos) {
        line = line.substr(0, line.rfind(" at "));
    }
    line = trim(line);
#endif
    return line;
}

int addr2line_all(Vector<void*> addrsVector, std::string& output) {
    int length = static_cast<int>(addrsVector.size());
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

    // have addr2line map the address to the relevant line in the code
#if defined(__APPLE__)
    // Mac OS X
    // JL : change "atos" to "xcrun atos"?
    void *base = (void *) _dyld_get_image_header(0);
    if (base) {
        out << "atos -l " << base << " -o " << exceptions::getProgramNameForStackTrace() << addrsStr;
    } else {
        out << "atos -o " << exceptions::getProgramNameForStackTrace() << addrsStr;
    }
#elif defined(_WIN64)
    // Windows
    if (fileExists("addr2line64.exe")) {
        out << "addr2line64.exe -f -i -C -s -p -e \"" << exceptions::getProgramNameForStackTrace() << "\"" << addrsStr;
    } else {
        out << "(addr2line64.exe unavailable; no stack trace produced)";
    }
#elif defined(_WIN32)
    // Windows
    if (fileExists("addr2line.exe")) {
        out << "addr2line.exe -f -i -C -s -p -e \"" << exceptions::getProgramNameForStackTrace() << "\"" << addrsStr;
    } else {
        out << "(addr2line.exe unavailable; no stack trace produced)";
    }
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
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_MAX, 50)

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
        // const char *dli_fname;   // pathname of shared object (file) that contains address
        // void       *dli_fbase;   // address at which shared object is loaded in system memory
        // const char *dli_sname;   // name of nearest symbol with address lower than addr
        // void       *dli_saddr;   // exact address of symbol named in dli_sname

        Dl_info dlinfo;
        if (!dladdr(trace[i], &dlinfo)) {
            continue;
        }

        const char* symname = dlinfo.dli_sname;

        int   status;
        char* demangled = abi::__cxa_demangle(symname, /* buffer */ nullptr,
                                              /* length pointer */ nullptr, &status);
        if (status == 0 && demangled) {
            symname = demangled;
        }

        // debug code left in because we occasionally need to debug stack traces
//        std::cout << "call_stack: I am thread " << GThread::getCurrentThread()->objectName().toStdString() << std::endl;
//        std::cout << "info for " << trace[i] << ":" << std::endl;
//        std::cout << "dlinfo " << i << ":"
//                  << " fbase=" << dlinfo.dli_fbase
//                  << " fname=" << (dlinfo.dli_fname ? dlinfo.dli_fname : "NULL")
//                  << " sname=" << (dlinfo.dli_sname ? dlinfo.dli_sname : "NULL")
//                  << " saddr=" << dlinfo.dli_saddr
//                  << std::endl;
//        if (demangled) {
//            std::cout << "demangled name " << i << ": " << std::string(demangled) << " (status " << status << ")" << std::endl;
//        } else {
//            std::cout << "demangled name " << i << ": NULL" << " (status " << status << ")" << std::endl;
//        }
//        std::cout << std::endl;
        
        // store entry to stack
        if (dlinfo.dli_fname) {
            entry e;
            e.file     = dlinfo.dli_fname;
            e.line     = 0;   // unsupported; use lineStr instead (later)
            e.function = symname ? symname : "(unknown)";
            e.address  = trace[i];

            // The dli_fbase gives an overall offset into the file itself;
            // the dli_saddr is the offset of that symbol/function/line.
            // by subtracting them we get the offset of the function within the file
            // which addr2line can use to look up function line numbers.

            if (dlinfo.dli_fbase) {
                // subtract smaller address from larger one to get offset
                if (trace[i] >= dlinfo.dli_fbase) {
                    e.address2 = (void*) ((long) trace[i] - (long) dlinfo.dli_fbase);
                } else {
                    e.address2 = (void*) ((long) dlinfo.dli_fbase - (long) trace[i]);
                }
            } else {
                e.address2 = dlinfo.dli_saddr;
            }
            stack.push_back(e);
        }

        if (demangled) {
            free(demangled);
        }
    }

    if (stack_depth == 0 || stack.isEmpty()) {
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

    Vector<void*> addrsToLookup;
    for (const entry& e : stack) {
        addrsToLookup.add(e.address);
        addrsToLookup.add(e.address2);
    }

    std::string addr2lineOutput;
    addr2line_all(addrsToLookup, addr2lineOutput);

    Vector<std::string> addr2lineLines = stringSplit(addr2lineOutput, "\n");
    int numAddrLines = (int) addr2lineLines.size();
    for (int i = 0, size = (int) stack.size(); i < size; i++) {
        std::string opt1 = (2 * i < numAddrLines ? addr2lineLines[2 * i] : std::string());
        std::string opt2 = (2 * i + 1 < numAddrLines ? addr2lineLines[2 * i + 1] : std::string());
        std::string best = opt1.length() > opt2.length() ? opt1 : opt2;
        stack[i].lineStr = addr2line_clean(best);
        if (stack[i].function.empty() || stack[i].function == "(unknown)") {
            stack[i].function = addr2line_functionName(best);
        }
    }
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#endif // _WIN32
