
/*
 * File: os.cpp
 * ------------
 * This file implements the OS class declared in os.h.
 * 
 * @author Marty Stepp
 * @version 2018/11/22
 * - added headless (non-Qt) mode support
 * @version 2018/09/23
 * - bug fix for isMac
 * @version 2018/09/17
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "os.h"
#ifndef SPL_HEADLESS_MODE
#include <QString>
#include <QSysInfo>
#endif // SPL_HEADLESS_MODE
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
/*static*/ std::string OS::getName() {
    std::string productName = QSysInfo::prettyProductName().toStdString();
    return productName;
}

/*static*/ std::string OS::getVersion() {
    std::string productVersion = QSysInfo::productVersion().toStdString();
    return productVersion;
}

bool OS::isLinux() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return kernelType == "linux" || productType == "linux";
}

bool OS::isMac() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return stringContains(kernelType, "apple")
            || stringContains(kernelType, "darwin")
            || stringContains(kernelType, "mac")
            || stringContains(kernelType, "ios")
            || stringContains(productType, "apple")
            || stringContains(productType, "ios")
            || stringContains(productType, "mac")
            || stringContains(productType, "osx");
}

bool OS::isWindows() {
    std::string kernelType = toLowerCase(QSysInfo::kernelType().toStdString());
    std::string productType = toLowerCase(QSysInfo::productType().toStdString());
    return stringContains(kernelType, "windows")
            || stringContains(productType, "windows");
}
#else
/*static*/ std::string OS::getName() {
    return "unknown";
}

/*static*/ std::string OS::getVersion() {
    return "unknown";
}

bool OS::isLinux() {
    return false;
}

bool OS::isMac() {
    return false;
}

bool OS::isWindows() {
    return true;   // most old versions of Qt occur on old Windows installs
}
#endif // QT_VERSION

OS::OS() {
    // empty
}

/*
 * File: error.cpp
 * ---------------
 * Implementation of the error function.
 * 
 * @version 2016/11/23
 * - added operator << to print ErrorExceptions
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#include <sstream>
#undef INTERNAL_INCLUDE

/* Definitions for the ErrorException class */

ErrorException::ErrorException(std::string msg)
        : _kind("error") {
    _msg = msg;

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    std::ostringstream out;
    exceptions::printStackTrace(out);
    _stackTrace = out.str();
#endif // SPL_CONSOLE_PRINT_EXCEPTIONS
}

ErrorException::~ErrorException() throw () {
    /* Empty */
}

void ErrorException::dump() const {
    dump(std::cerr);
}

void ErrorException::dump(std::ostream& out) const {
    out << std::endl;
    out << "***" << std::endl;
    out << "*** STANFORD C++ LIBRARY" << std::endl;
    out << "*** An ErrorException occurred during program execution:" << std::endl;
    if (!_msg.empty()) {
        out << "*** " << _msg << std::endl;
    }
    out << insertStarsBeforeEachLine(getStackTrace()) << std::endl;
    out.flush();
}

std::string ErrorException::getKind() const {
    return _kind;
}

std::string ErrorException::getMessage() const {
    return _msg;
}

bool ErrorException::hasStackTrace() const {
    return !_stackTrace.empty();
}

std::string ErrorException::getStackTrace() const {
    return _stackTrace;
}

std::string ErrorException::insertStarsBeforeEachLine(const std::string& s) {
    std::string result;
    for (std::string line : stringSplit(s, "\n")) {
        if (!result.empty()) {
            if (!startsWith(line, "***")) {
                line = "*** " + line;
            }
            result += "\n";
        }
        result += line;
    }
    return result;
}

void ErrorException::setKind(const std::string& kind) {
    _kind = kind;
}

void ErrorException::setStackTrace(const std::string& stackTrace) {
    _stackTrace = stackTrace;
}

const char* ErrorException::what() const throw () {
    // stepp : The original "Error: " prefix is commented out here,
    // because in many error cases, the attempt to do the string concatenation
    // ends up garbling the string and leading to garbage exception text
    // return ("Error: " + msg).c_str();
    return _msg.c_str();
}

std::ostream& operator <<(std::ostream& out, const ErrorException& ex) {
    out << "ErrorException: " << ex.what();
    std::string stack = ex.getStackTrace();
    if (!stack.empty()) {
        out << "Stack trace:" << std::endl;
        out << stack;
        out.flush();
    }
    return out;
}

/*
 * Implementation notes: error
 * ---------------------------
 * Earlier implementations of error made it possible, at least on the
 * Macintosh, to help the debugger generate a backtrace at the point
 * of the error.  Unfortunately, doing so is no longer possible if
 * the errors are catchable.
 */

/* [[noreturn]] */ void error(const std::string& msg) {
    throw ErrorException(msg);
}

/*
 * File: call_stack_gcc.cpp
 * ------------------------
 * Linux/gcc implementation of the call_stack class.
 *
 * @author Marty Stepp, based on code from Fredrik Orderud
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
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#define INTERNAL_INCLUDE 1
#include "call_stack.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
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

/*
 * File: exceptions.cpp
 * --------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * 
 * @author Marty Stepp
 * @version 2018/10/18
 * - added set_unexpected handler (used by autograders when errors are thrown)
 * - added some new function names to filter from stack traces
 * @version 2018/09/27
 * - bug fixes to print better stack traces when used with threads
 * @version 2018/09/25
 * - modify setTopLevelExceptionHandlerEnabled to work better with threads
 * @version 2016/12/23
 * - added more function names for stack trace filtering (mainly thread stuff)
 * @version 2016/12/09
 * - added insertStarsBeforeEachLine
 * @version 2016/11/07
 * - added cleanupFunctionNameForStackTrace
 * - slight refactor of shouldFilterOutFromStackTrace
 * @version 2016/10/30
 * - moved recursion functions to recursion.h/cpp
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/08/02
 * - added some new cxx11 filters to stack traces
 * - fixed spacing on *** messages from exception handlers
 * @version 2015/10/13
 * - bug fix in terminate handler to turn off signal handler at end
 * @version 2015/05/28
 * - tiny bug fix to exception stack trace printing format on Windows
 * @version 2014/11/19
 * - disabled SetThreadErrorMode to avoid compiler errors on Windows systems
 * @version 2014/11/18
 * - fixed minor bug with filtering out nested <> template args from stack traces
 * @version 2014/11/14
 * - fixed bug with SIGABRT handling in autograder mode (was muffling unit test failures)
 * @version 2014/11/12
 * - made printStackTrace function publicly available
 * - added top-level signal handler (for null-pointer derefs etc.)
 * @since 2014/11/05
 */

#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#include <csignal>
#include <iomanip>
#include <iostream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "call_stack.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#ifdef _WIN32
#include <windows.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#endif
#undef INTERNAL_INCLUDE

// uncomment the definition below to use an alternative 'signal stack'
// which helps in handling stack overflow errors
// (disabled because it currently breaks stack traces for other errors)
// #define SHOULD_USE_SIGNAL_STACK

namespace exceptions {
// just some value that is not any existing signal
#define SIGSTACK (static_cast<int>(0xdeadbeef))
#define SIGUNKNOWN (static_cast<int>(0xcafebabe))
#define SIGTIMEOUT (static_cast<int>(0xf00df00d))

// static 'variables' (as functions to avoid initialization ordering bugs)
STATIC_CONST_VARIABLE_DECLARE(bool, STACK_TRACE_SHOULD_FILTER, true)
STATIC_CONST_VARIABLE_DECLARE(bool, STACK_TRACE_SHOW_TOP_BOTTOM_BARS, false)

STATIC_VARIABLE_DECLARE(std::string, programNameForStackTrace, "")
STATIC_VARIABLE_DECLARE(bool, topLevelExceptionHandlerEnabled, false)

// handle SIGABRT in normal mode, but not autograder mode
// (Google Test uses SIGABRT internally so we can't catch it)
#ifdef SPL_AUTOGRADER_MODE
STATIC_CONST_VARIABLE_DECLARE_COLLECTION(Vector<int>, SIGNALS_HANDLED, SIGSEGV, SIGILL, SIGFPE, SIGINT)
#else
STATIC_CONST_VARIABLE_DECLARE_COLLECTION(Vector<int>, SIGNALS_HANDLED, SIGSEGV, SIGILL, SIGFPE, SIGABRT)
#endif // SPL_AUTOGRADER_MODE

static void signalHandlerDisable();
static void signalHandlerEnable();
static void stanfordCppLibSignalHandler(int sig);
static void stanfordCppLibTerminateHandler();
static void stanfordCppLibUnexpectedHandler();

std::string cleanupFunctionNameForStackTrace(std::string function) {
    // remove references to std:: namespace
    stringReplaceInPlace(function, "std::", "");
    stringReplaceInPlace(function, "__cxx11::", "");
    stringReplaceInPlace(function, "__cxxabi::", "");
    stringReplaceInPlace(function, "__cxxabiv1::", "");
    stringReplaceInPlace(function, "[abi:cxx11]", "");
    stringReplaceInPlace(function, "__1::", "");   // on Mac

    // a few substitutions related to predefined types for simplicity
    stringReplaceInPlace(function, "basic_ostream", "ostream");
    stringReplaceInPlace(function, "basic_istream", "istream");
    stringReplaceInPlace(function, "basic_ofstream", "ofstream");
    stringReplaceInPlace(function, "basic_ifstream", "ifstream");
    stringReplaceInPlace(function, "basic_string", "string");

    // remove empty/unknown function names
    stringReplaceInPlace(function, "?? ??:0", "");

    // cleanup autograder test case names
    stringReplaceInPlace(function, "_Test::TestRealBody", "");
    stringReplaceInPlace(function, "_Test::TestBody", "");

    // remove template arguments
    // TODO: does not work well for nested templates
    int lessThan = stringIndexOf(function, "<");
    while (lessThan >= 0) {
        // see if there is a matching > for this <
        int greaterThan = lessThan + 1;
        int count = 1;
        while (greaterThan < (int) function.length()) {
            if (function[greaterThan] == '<') {
                count++;
            } else if (function[greaterThan] == '>') {
                count--;
                if (count == 0) {
                    break;
                }
            }
            greaterThan++;
        }
        if (count == 0 && lessThan >= 0 && greaterThan > lessThan) {
            function.erase(lessThan, greaterThan - lessThan + 1);
        } else {
            // look for the next < in the string, if any, to see if it has a matching >
            lessThan = stringIndexOf(function, "<", lessThan + 1);
        }
    }

    // addr2line oddly writes "const Foo&" as "Foo const&"
    stringReplaceInPlace(function, "string const&", "const string&");

    // small patch for renamed main function
    if (function == "_main_") {
        function = "main";
    }

    return function;
}

std::string& getProgramNameForStackTrace() {
    return STATIC_VARIABLE(programNameForStackTrace);
}

bool getTopLevelExceptionHandlerEnabled() {
    return STATIC_VARIABLE(topLevelExceptionHandlerEnabled);
}

void setProgramNameForStackTrace(char* programName) {
    STATIC_VARIABLE(programNameForStackTrace) = programName;
}

#ifdef _WIN32
void myInvalidParameterHandler(const wchar_t* expression,
        const wchar_t* function,
        const wchar_t* file,
        unsigned int line,
        uintptr_t /*pReserved*/) {
    wprintf(L"Invalid parameter detected in function %s."
            L" File: %s Line: %d\n", function, file, line);
    wprintf(L"Expression: %s\n", expression);
}

LONG WINAPI UnhandledException(LPEXCEPTION_POINTERS exceptionInfo) {
    // dear student: if you get a compiler error about 'Eip' not being found here,
    // it means you're using a 64-bit compiler like the MS Visual C++ compiler,
    // and not the 32-bit MinGW compiler we instructed you to install.
    // Please re-install Qt Creator with the proper compiler (MinGW 32-bit) enabled.
    if (exceptionInfo && exceptionInfo->ContextRecord && exceptionInfo->ContextRecord->Eip) {
        // stacktrace::fakeCallStackPointer() = (void*) exceptionInfo->ContextRecord->Eip;
        stacktrace::fakeCallStackPointer() = (void*) exceptionInfo;
    }
    DWORD code = exceptionInfo->ExceptionRecord->ExceptionCode;
    if (code == EXCEPTION_STACK_OVERFLOW || code == EXCEPTION_FLT_STACK_CHECK) {
        stanfordCppLibSignalHandler(SIGSTACK);
    } else if (code == EXCEPTION_IN_PAGE_ERROR || code == EXCEPTION_ACCESS_VIOLATION) {
        stanfordCppLibSignalHandler(SIGSEGV);
    } else if (code == EXCEPTION_FLT_DENORMAL_OPERAND || code == EXCEPTION_FLT_DIVIDE_BY_ZERO
               || code == EXCEPTION_FLT_INEXACT_RESULT || code == EXCEPTION_FLT_INVALID_OPERATION
               || code == EXCEPTION_FLT_OVERFLOW || code == EXCEPTION_FLT_UNDERFLOW
               || code == EXCEPTION_INT_DIVIDE_BY_ZERO || code == EXCEPTION_INT_OVERFLOW) {
        stanfordCppLibSignalHandler(SIGFPE);
    } else if (code == EXCEPTION_ILLEGAL_INSTRUCTION || code == EXCEPTION_PRIV_INSTRUCTION) {
        stanfordCppLibSignalHandler(SIGILL);
    } else {
        stanfordCppLibSignalHandler(SIGUNKNOWN);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif // _WIN32

void setTopLevelExceptionHandlerEnabled(bool enabled, bool force) {
    static void (* old_terminate)() = nullptr;
    static void (* old_unexpected)() = nullptr;

    if ((!STATIC_VARIABLE(topLevelExceptionHandlerEnabled) || force) && enabled) {
        if (!old_terminate) {
            old_terminate = std::set_terminate(stanfordCppLibTerminateHandler);
            old_unexpected = std::set_unexpected(stanfordCppLibUnexpectedHandler);
        } else {
            std::set_terminate(stanfordCppLibTerminateHandler);
            std::set_unexpected(stanfordCppLibUnexpectedHandler);
        }
#ifdef _WIN32
        // disabling this code for now because it messes with the
        // newly added uncaught signal handler
        // SetErrorMode(SEM_NOGPFAULTERRORBOX);
        SetErrorMode(SEM_FAILCRITICALERRORS);
        // SetThreadErrorMode(SEM_FAILCRITICALERRORS, nullptr);
        SetUnhandledExceptionFilter(UnhandledException);
        // _invalid_parameter_handler newHandler;
        // newHandler = myInvalidParameterHandler;
        // _set_invalid_parameter_handler(newHandler);
        //_set_error_mode(_OUT_TO_STDERR);
#endif // _WIN32
        
        // also set up a signal handler for things like segfaults / null-pointer-dereferences
        signalHandlerEnable();
    } else if ((STATIC_VARIABLE(topLevelExceptionHandlerEnabled) || force) && !enabled) {
        std::set_terminate(old_terminate);
        std::set_unexpected(old_unexpected);
    }
    STATIC_VARIABLE(topLevelExceptionHandlerEnabled) = enabled;
}

/*
 * Some lines from the stack trace are filtered out because they come from
 * private library code or OS code and would confuse the student.
 */
bool shouldFilterOutFromStackTrace(const std::string& function) {
    // exact names that should be matched and filtered
    static const Vector<std::string> FORBIDDEN_NAMES {
        "",
        "??",
        "call_stack",
        "_clone",
        "clone",
        "error",
        "error(const string&)",
        "error(string)",
        "ErrorException",
        "__libc_start_main",
        "_start",
        "startupMain(int, char**)",
        "(unknown)",
        "_Unwind_Resume"
    };

    // substrings to filter (don't show any func whose name contains these)
    static const Vector<std::string> FORBIDDEN_SUBSTRINGS {
        " error(",
        "__cxa_rethrow",
        "__cxa_call_terminate",
        "__cxa_call_unexpected",
        "_endthreadex",
        "_Function_handler",
        "_Internal_",
        "_M_invoke",
        "_sigtramp",
        "autograderMain",
        "BaseThreadInitThunk",
        "call_stack_gcc.cpp",
        "call_stack_windows.cpp",
        "crtexe.c",
        "ErrorException::ErrorException",
        "exceptions.cpp",
        "function::operator",
        "GetModuleFileName",
        "GetProfileString",
        // "GStudentThread::run",
        "InitializeExceptionChain",
        "KnownExceptionFilter",
        "M_invoke",
        "multimain.cpp",
        // "operator",
        "printStackTrace",
        // "QAbstractItemModel::",
        // "QAbstractProxyModel::",
        "QApplicationPrivate::",
        "QCoreApplication::",
        "QGuiApplicationPrivate::",
        "QMetaMethod::",
        "QMetaObject::",
        "QObjectPrivate::",
        // "QWidget::",
        "QWidgetBackingStore::",
        "QWindowSystemInterface::",
        "shouldFilterOutFromStackTrace",
        "stacktrace::",
        "stanfordCppLibPosixSignalHandler",
        "stanfordCppLibSignalHandler",
        "stanfordCppLibTerminateHandler",
        "stanfordCppLibUnexpectedHandler",
        "testing::",
        "UnhandledException"
    };

    // prefixes to filter (don't show any func whose name starts with these)
    static const Vector<std::string> FORBIDDEN_PREFIXES {
        // "__"
    };

    for (const std::string& name : FORBIDDEN_NAMES) {
        if (function == name) {
            return true;
        }
    }

    for (const std::string& name : FORBIDDEN_SUBSTRINGS) {
        if (function.find(name) != std::string::npos) {
            return true;
        }
    }

    for (const std::string& name : FORBIDDEN_PREFIXES) {
        if (function.find(name) == 0) {
            return true;
        }
    }

    return false;
}

void printStackTrace() {
    printStackTrace(std::cerr);
}

void printStackTrace(std::ostream& out) {
    // constructing the following object jumps into fancy code in call_stack_gcc/windows.cpp
    // to rebuild the stack trace; implementation differs for each operating system
    stacktrace::call_stack trace;
    Vector<stacktrace::entry> entries = trace.stack;
    
    // get longest line string length to line up stack traces
    void* fakeStackPtr = stacktrace::fakeCallStackPointer();
    int entriesToShowCount = 0;
    int funcNameLength = 0;
    int lineStrLength = 0;
    for (int i = 0; i < entries.size(); ++i) {
        entries[i].function = cleanupFunctionNameForStackTrace(entries[i].function);
        if (!STATIC_VARIABLE(STACK_TRACE_SHOULD_FILTER)
                || (!shouldFilterOutFromStackTrace(entries[i].function)
                    && !shouldFilterOutFromStackTrace(entries[i].file)
                    && !shouldFilterOutFromStackTrace(entries[i].lineStr))) {
            lineStrLength = std::max(lineStrLength, (int) entries[i].lineStr.length());
            funcNameLength = std::max(funcNameLength, (int) entries[i].function.length());
            entriesToShowCount++;
        }
    }
    
    if (entries.isEmpty() || entriesToShowCount == 0) {
        return;   // couldn't get a stack trace, or had no useful data  :-(
    }
    
    if (lineStrLength > 0) {
        out << "*** Stack trace (line numbers are approximate):" << std::endl;
        if (STATIC_VARIABLE(STACK_TRACE_SHOW_TOP_BOTTOM_BARS)) {
            out << "*** "
                      << std::setw(lineStrLength) << std::left
                      << "file:line" << "  " << "function" << std::endl;
            out << "*** "
                      << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
        }
    } else {
        out << "*** Stack trace:" << std::endl;
    }
    
    for (int i = 0; i < entries.size(); ++i) {
        stacktrace::entry entry = entries[i];
        entry.file = getTail(entry.file);
        
        // skip certain entries for clarity
        if (STATIC_VARIABLE(STACK_TRACE_SHOULD_FILTER)
                && (shouldFilterOutFromStackTrace(entry.function)
                    || shouldFilterOutFromStackTrace(entry.file)
                    || shouldFilterOutFromStackTrace(entry.lineStr))) {
            continue;
        }
        
        // show Main() as main() to hide hidden case-change by Stanford C++ lib internals
        if (startsWith(entry.function, "Main(")) {
            entry.function.replace(0, 5, "main(");
        }

        // for some reason, some functions don't show () parens after; add them
        if (!entry.function.empty() && !stringContains(entry.function, "(")) {
            entry.function += "()";
        }

        // fix main to hide int/char**
        if (entry.function == "main(int, char**)") {
            entry.function = "main()";
        }

        // fix qMain => main to hide Qt main renaming
        if (entry.function == "qMain()") {
            entry.function = "main()";
        }

        std::string lineStr = "";
        if (!entry.lineStr.empty()) {
            lineStr = trimEnd(entry.lineStr);
            if (lineStr == "?? ??:0") {
                lineStr = "(unknown)";
            }

            if (entry.line == 0 && stringContains(lineStr, ":")) {
                Vector<std::string> tokens = stringSplit(lineStr, ":");
                if (stringIsInteger(tokens[tokens.size() - 1])) {
                    entry.line = stringToInteger(tokens[tokens.size() - 1]);
                }
            }
        }
        if (entry.lineStr.empty() && entry.line > 0) {
            lineStr = "line " + std::to_string(entry.line);
        }
        
        out << "*** " << std::left << std::setw(lineStrLength) << lineStr
                  << "  " << entry.function << std::endl;
        
        // don't show entries beneath the student's main() function, for simplicity
        if (entry.function == "main"
                || entry.function == "main()"
                || entry.function == "main(int, char**)"
                || entry.function == "qMain"
                || entry.function == "qMain()") {
            break;
        }
    }
    if (entries.size() == 1 && fakeStackPtr && entries[0].address == fakeStackPtr) {
        out << "*** (partial stack due to crash)" << std::endl;
    }

    if (STATIC_VARIABLE(STACK_TRACE_SHOW_TOP_BOTTOM_BARS) && lineStrLength > 0) {
        out << "*** "
                  << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
    }
    
//    out << "***" << std::endl;
//    out << "*** NOTE:" << std::endl;
//    out << "*** Any line numbers listed above are approximate." << std::endl;
//    out << "*** To learn more about why the program crashed, we" << std::endl;
//    out << "*** suggest running your program under the debugger." << std::endl;
    
    out << "***" << std::endl;
}

// macro to avoid lots of redundancy in catch statements below
#ifdef _WIN32
#define THROW_NOT_ON_WINDOWS(ex)
#else
#define THROW_NOT_ON_WINDOWS(ex) throw(ex)
#endif // _WIN32

#define FILL_IN_EXCEPTION_TRACE(ex, kind, desc) \
    {\
    std::string __kind = (kind); \
    std::string __desc = (desc); \
    if ((!__kind.empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_KIND, __kind); } \
    if ((!__desc.empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_DETAILS, __desc); } \
    std::cout.flush(); \
    out << msg; \
    printStackTrace(out); \
    THROW_NOT_ON_WINDOWS(ex); \
    }

#define FILL_IN_EXCEPTION_TRACE_AND_THROW_ERROREXCEPTION(ex, kind, desc) \
    {\
    std::string __kind = (kind); \
    std::string __desc = (desc); \
    if ((!__kind.empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_KIND, __kind); } \
    if ((!__desc.empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_DETAILS, __desc); } \
    std::cout.flush(); \
    out << msg; \
    printStackTrace(out); \
    ErrorException errorEx(out.str()); \
    errorEx.setKind(kind); \
    THROW_NOT_ON_WINDOWS(errorEx); \
    }

static void signalHandlerDisable() {
    for (int sig : STATIC_VARIABLE(SIGNALS_HANDLED)) {
        signal(sig, SIG_DFL);
    }
}

#ifndef _WIN32
void stanfordCppLibPosixSignalHandler(int sig, siginfo_t* /*siginfo*/, void* /*context*/) {
    stanfordCppLibSignalHandler(sig);
}
#endif

static void signalHandlerEnable() {
    bool handled = false;
#ifdef SHOULD_USE_SIGNAL_STACK
#if !defined(_WIN32)
    // alternate stack on Linux for stack overflows
    static uint8_t alternate_stack[SIGSTKSZ];
    stack_t ss = {};
    ss.ss_sp = (void*) alternate_stack;
    ss.ss_size = SIGSTKSZ;
    ss.ss_flags = 0;
    sigaltstack(&ss, nullptr);
    
    struct sigaction sig_action = {};
    sig_action.sa_sigaction = stanfordCppLibPosixSignalHandler;
    sigemptyset(&sig_action.sa_mask);
#ifdef __APPLE__
    // backtrace() doesn't work on OS X when we use an alternate stack
    sig_action.sa_flags = SA_SIGINFO;
#else
    sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif // __APPLE__
    sigaction(SIGSEGV, &sig_action, nullptr);
    sigaction(SIGFPE,  &sig_action, nullptr);
    sigaction(SIGILL,  &sig_action, nullptr);
    sigaction(SIGTERM, &sig_action, nullptr);
    sigaction(SIGUSR1, &sig_action, nullptr);
#ifdef SPL_AUTOGRADER_MODE
    sigaction(SIGINT,  &sig_action, nullptr);
#else // not SPL_AUTOGRADER_MODE
    sigaction(SIGABRT, &sig_action, nullptr);
#endif // SPL_AUTOGRADER_MODE
    handled = true;
#endif
#endif // SHOULD_USE_SIGNAL_STACK

    if (!handled) {
        for (int sig : STATIC_VARIABLE(SIGNALS_HANDLED)) {
            signal(sig, stanfordCppLibSignalHandler);
        }
    }
}

/*
 * A general handler for process signals.
 * Prints details about the signal and then tries to print a stack trace.
 */
static void stanfordCppLibSignalHandler(int sig) {
#ifndef SPL_AUTOGRADER_MODE
    // turn the signal handler off (should run only once; avoid infinite cycle)
    signalHandlerDisable();
#endif // SPL_AUTOGRADER_MODE

    // tailor the error message to the kind of signal that occurred
    std::string SIGNAL_KIND = "A fatal error";
    std::string SIGNAL_DETAILS = "No details were provided about the error.";
    if (sig == SIGSEGV) {
        SIGNAL_KIND = "A segmentation fault (SIGSEGV)";
        SIGNAL_DETAILS = "This typically happens when you try to dereference a pointer\n*** that is NULL or invalid.";
    } else if (sig == SIGABRT) {
        SIGNAL_KIND = "An abort error (SIGABRT)";
        SIGNAL_DETAILS = "This error is thrown by system functions that detect corrupt state.";
    } else if (sig == SIGILL) {
        SIGNAL_KIND = "An illegal instruction error (SIGILL)";
        SIGNAL_DETAILS = "This typically happens when you have corrupted your program's memory.";
    } else if (sig == SIGFPE) {
        SIGNAL_KIND = "An arithmetic error (SIGFPE)";
        SIGNAL_DETAILS = "This typically happens when you divide by 0 or produce an overflow.";
    } else if (sig == SIGINT) {
        SIGNAL_KIND = "An interrupt error (SIGINT)";
        SIGNAL_DETAILS = "This typically happens when your code timed out because it was stuck in an infinite loop.";
    } else if (sig == SIGSTACK) {
        SIGNAL_KIND = "A stack overflow";
        SIGNAL_DETAILS = "This can happen when you have a function that calls itself infinitely.";
    } else if (sig == SIGUSR1) {
        SIGNAL_KIND = "Custom signal 1";
        SIGNAL_DETAILS = "This can happen when you produce infinite output in your code.";
    }
    
#ifndef SPL_AUTOGRADER_MODE
    std::cerr << std::endl;
    std::cerr << "***" << std::endl;
    std::cerr << "*** STANFORD C++ LIBRARY" << std::endl;
    std::cerr << "*** " << SIGNAL_KIND << " occurred during program execution." << std::endl;
    std::cerr << "*** " << SIGNAL_DETAILS << std::endl;;
    std::cerr << "***" << std::endl;
    
//    if (sig != SIGSTACK) {
        exceptions::printStackTrace();
//    } else {
//        std::string line;
//        stacktrace::addr2line(stacktrace::getFakeCallStackPointer(), line);
//        std::cerr << "*** (unable to print stack trace because of stack memory corruption.)" << std::endl;
//        std::cerr << "*** " << line << std::endl;
//    }
    std::cerr.flush();

    // if in autograder mode, swallow the signal;
    // if in student code, let it bubble out to crash the app
    raise(sig == SIGSTACK ? SIGABRT : sig);
#else // SPL_AUTOGRADER_MODE
    // throw an exception
    std::ostringstream out;
    out << SIGNAL_KIND << "." << std::endl;
    out << SIGNAL_DETAILS << std::endl;
    error(out.str());
#endif // SPL_AUTOGRADER_MODE
}

// puts "*** " before each line for multi-line error messages
static std::string insertStarsBeforeEachLine(const std::string& s) {
    std::string result;
    for (std::string line : stringSplit(s, "\n")) {
        if (!result.empty()) {
            if (!startsWith(line, "***")) {
                line = "*** " + line;
            }
            result += "\n";
        }
        result += line;
    }
    return result;
}

/*
 * A general handler for any uncaught exception.
 * Prints details about the exception and then tries to print a stack trace.
 */
static void stanfordCppLibTerminateHandler() {
    std::string DEFAULT_EXCEPTION_KIND = "An exception";
    std::string DEFAULT_EXCEPTION_DETAILS = "(unknown exception details)";
    
    std::string msg;
    msg += "\n";
    msg += "***\n";
    msg += "*** STANFORD C++ LIBRARY \n";
    msg += "*** " + DEFAULT_EXCEPTION_KIND + " occurred during program execution: \n";
    msg += "*** " + DEFAULT_EXCEPTION_DETAILS + "\n";
    msg += "***\n";
    
    std::ostream& out = std::cerr;   // used by FILL_IN_EXCEPTION_TRACE macro
    try {
        signalHandlerDisable();   // don't want both a signal AND a terminate() call
        throw;   // re-throws the exception that already occurred
    } catch (const ErrorException& ex) {
        if (ex.hasStackTrace()) {
            ex.dump();
        } else {
            FILL_IN_EXCEPTION_TRACE(ex, "An ErrorException", insertStarsBeforeEachLine(ex.what()));
        }
    } catch (const std::exception& ex) {
        FILL_IN_EXCEPTION_TRACE(ex, "A C++ exception", insertStarsBeforeEachLine(ex.what()));
    } catch (std::string str) {
        FILL_IN_EXCEPTION_TRACE(str, "A string exception", insertStarsBeforeEachLine(str));
    } catch (char const* str) {
        FILL_IN_EXCEPTION_TRACE(str, "A string exception", insertStarsBeforeEachLine(str));
    } catch (int n) {
        FILL_IN_EXCEPTION_TRACE(n, "An int exception", std::to_string(n));
    } catch (long l) {
        FILL_IN_EXCEPTION_TRACE(l, "A long exception", std::to_string(l));
    } catch (char c) {
        FILL_IN_EXCEPTION_TRACE(c, "A char exception", charToString(c));
    } catch (bool b) {
        FILL_IN_EXCEPTION_TRACE(b, "A bool exception", boolToString(b));
    } catch (double d) {
        FILL_IN_EXCEPTION_TRACE(d, "A double exception", std::to_string(d));
    } catch (...) {
        std::string ex = "Unknown";
        FILL_IN_EXCEPTION_TRACE(ex, "An exception", std::string());
    }
}

/*
 * A general handler for any exception thrown that is missing from the throw()
 * clause of a function header.
 * Prints details about the exception and then tries to print a stack trace.
 */
static void stanfordCppLibUnexpectedHandler() {
    std::string DEFAULT_EXCEPTION_KIND = "An exception";
    std::string DEFAULT_EXCEPTION_DETAILS = "(unknown exception details)";

    std::string msg;
    msg += "\n";
    msg += "***\n";
    msg += "*** STANFORD C++ LIBRARY \n";
    msg += "*** " + DEFAULT_EXCEPTION_KIND + " occurred during program execution: \n";
    msg += "*** " + DEFAULT_EXCEPTION_DETAILS + "\n";
    msg += "***\n";

    std::string kind = "error";
    std::string message = "";
    try {
        throw;   // re-throws the exception that already occurred
    } catch (bool b) {
        kind = "bool";
        message = boolToString(b);
    } catch (char c) {
        kind = "char";
        message = charToString(c);
    } catch (char const* str) {
        kind = "string";
        message = str;
    } catch (double d) {
        kind = "double";
        message = std::to_string(d);
    } catch (const ErrorException& ex) {
        kind = "error";
        message = ex.what();
    } catch (const std::exception& ex) {
        kind = "exception";
        message = ex.what();
    } catch (int n) {
        kind = "int";
        message = std::to_string(n);
    } catch (long l) {
        kind = "long";
        message = std::to_string(l);
    } catch (std::string str) {
        kind = "string";
        message = str;
    } catch (...) {
        kind = "unknown";
    }

    ErrorException errorEx(message);
    errorEx.setKind(kind);
    throw errorEx;
}

} // namespace exceptions


/*
 * File: call_stack_windows.cpp
 * ----------------------------
 * Windows implementation of the call_stack class.
 *
 * @author Marty Stepp
 * @version 2018/10/22
 * - bug fix for STL vector vs Stanford Vector
 * @version 2018/09/12
 * - fixed compiler errors with os_getLastError and other misc warnings
 * @version 2017/10/24
 * - removed SYMOPT_DEBUG from SymSetOptions to avoid spurious console output
 * @version 2017/10/20
 * - changed null/0s to nullptr to remove compiler warnings
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2015/05/28
 */

#ifdef _WIN32
#define INTERNAL_INCLUDE 1
#include "call_stack.h"
#include <windows.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#include <tchar.h>
#include <assert.h>
#include <errno.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string>
#include <windows.h>
#include <tchar.h>
#define _NO_CVCONST_H
#include <dbghelp.h>
#include <imagehlp.h>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#include <cxxabi.h>
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

namespace stacktrace {
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_TO_SKIP, 0)
STATIC_CONST_VARIABLE_DECLARE(int, STACK_FRAMES_MAX, 20)

// line = "ZNK6VectorIiE3getEi at vector.h:587"
//         <FUNCTION> at <LINESTR>
void injectAddr2lineInfo(entry& ent, const std::string& line) {
    ent.line     = 0;
    int atIndex = stringIndexOf(line, " at ");
    if (atIndex >= 0) {
        if (ent.function.empty()) {
            ent.function = line.substr(0, atIndex);
        }
        ent.lineStr  = line.substr(atIndex + 4);

        int colonIndex = stringIndexOf(ent.lineStr, ":");
        if (colonIndex >= 0) {
            ent.file = ent.lineStr.substr(0, colonIndex);
            std::string rest = ent.lineStr.substr(colonIndex + 1);
            if (stringIsInteger(rest)) {
                ent.line = stringToInteger(rest);
            }
        }
    } else {
        if (ent.function.empty()) {
            ent.function = line;
        }
    }

    // demangle function name if necessary
    if (startsWith(ent.function, "Z")) {
        ent.function = "_" + ent.function;
    }

    if (startsWith(ent.function, "_Z")) {
        int status;
        char* demangled = abi::__cxa_demangle(ent.function.c_str(), nullptr, nullptr, &status);
        if (status == 0 && demangled) {
            ent.function = demangled;
        }
    }
}

std::ostream& operator <<(std::ostream& out, const entry& ent) {
    return out << ent.toString();
}

call_stack::call_stack(const size_t /*num_discard = 0*/) {
    // getting a stack trace on Windows / MinGW is loads of fun (not)
    Vector<void*> traceVector;
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    void* fakeStackPtr = stacktrace::fakeCallStackPointer();
    if (fakeStackPtr) {
        // set up fake stack for partial trace
        LPEXCEPTION_POINTERS exceptionInfo = (LPEXCEPTION_POINTERS) fakeStackPtr;
        if (exceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_STACK_OVERFLOW) {
            // can't do stack walking in Windows when a stack overflow happens :-/
            traceVector.push_back((void*) exceptionInfo->ContextRecord->Eip);
        } else {
            SymInitialize(GetCurrentProcess(), nullptr, TRUE);
            STACKFRAME frame = {0};
            frame.AddrPC.Offset    = exceptionInfo->ContextRecord->Eip;
            frame.AddrPC.Mode      = AddrModeFlat;
            frame.AddrStack.Offset = exceptionInfo->ContextRecord->Esp;
            frame.AddrStack.Mode   = AddrModeFlat;
            frame.AddrFrame.Offset = exceptionInfo->ContextRecord->Ebp;
            frame.AddrFrame.Mode   = AddrModeFlat;
            while ((int) traceVector.size() < STATIC_VARIABLE(STACK_FRAMES_MAX) &&
                   StackWalk(IMAGE_FILE_MACHINE_I386,
                             process,
                             thread,
                             &frame,
                             exceptionInfo->ContextRecord,
                             nullptr,
                             SymFunctionTableAccess,
                             SymGetModuleBase,
                             nullptr)) {
                traceVector.push_back((void*) frame.AddrPC.Offset);
            }
        }
    } else {
        if (!::SymSetOptions(
                             // ::SymGetOptions()
                             // SYMOPT_DEBUG
                               SYMOPT_DEFERRED_LOADS
                             | SYMOPT_INCLUDE_32BIT_MODULES
                             // | SYMOPT_UNDNAME
                             | SYMOPT_CASE_INSENSITIVE
                             | SYMOPT_LOAD_LINES)) {
            // std::cout << "SymSetOptions failed!" << std::endl;
            // return;
        }
        if (!::SymInitialize(
                /* process */ process,
                /* user-defined search path */ nullptr,
                /* include current process */ TRUE)) {
            // std::cout << "SymInitialize failed!" << std::endl;
            // return;
        }

        void* trace[STATIC_VARIABLE(STACK_FRAMES_MAX)];
        USHORT frameCount = ::CaptureStackBackTrace(
                    /* framesToSkip */ STATIC_VARIABLE(STACK_FRAMES_TO_SKIP),
                    /* framesToCapture; must be < 63 */ STATIC_VARIABLE(STACK_FRAMES_MAX),
                    trace,
                    /* hash */ nullptr
                    );
        for (int i = 0; i < frameCount; i++) {
            traceVector.push_back(trace[i]);
        }

        // try to load module symbol information; this always fails for me  :-/
        DWORD BaseAddr = 0;
        DWORD FileSize = 0;
        const char* progFileC = exceptions::getProgramNameForStackTrace().c_str();
        char* progFile = const_cast<char*>(progFileC);
        if (!::SymLoadModule(
                process,      // Process handle of the current process
                nullptr,      // Handle to the module's image file (not needed)
                progFile,     // Path/name of the file
                nullptr,      // User-defined short name of the module (it can be null)
                BaseAddr,     // Base address of the module (cannot be null if .PDB file is used, otherwise it can be null)
                FileSize)) {      // Size of the file (cannot be null if .PDB file is used, otherwise it can be null)
            // std::cerr << "Error: SymLoadModule() failed: " << platform::os_getLastError() << std::endl;
            // return;
        }
    }

    // let's also try to get the line numbers via an external command-line process 'addr2line'
    // (ought to be able to get this information through C function 'backtrace', but for some
    // reason, Qt Creator's shipped version of MinGW does not include this functionality, argh)
    std::string addr2lineOutput;
    Vector<std::string> addr2lineLines;
    if (!traceVector.isEmpty()) {
        int result = addr2line_all(traceVector, addr2lineOutput);
        if (result == 0) {
            addr2lineLines = stringSplit(addr2lineOutput, "\n");
        }
    }

    SYMBOL_INFO* symbol = (SYMBOL_INFO*) calloc(sizeof(SYMBOL_INFO) + 1024 * sizeof(char), 1);
    symbol->MaxNameLen   = 1020;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    for (int i = 0; i < (int) traceVector.size(); ++i) {
        entry ent;
        ent.address = traceVector[i];
        if (process && ::SymFromAddr(process, (DWORD64) traceVector[i], nullptr, symbol)) {
            ent.function = symbol->Name;
        }
        // internal stuff failed, so load from external process
        if (i < addr2lineLines.size()) {
            injectAddr2lineInfo(ent, addr2line_clean(addr2lineLines[i]));
        } else {
            injectAddr2lineInfo(ent, "");
        }

        if (!ent.function.empty() || ent.line > 0) {
            stack.push_back(ent);
        }
    }
    free(symbol);
}

call_stack::~call_stack() throw() {
    // automatic cleanup
}

} // namespace stacktrace

#endif // _WIN32

/*
 * File: timer.cpp
 * ---------------
 * Implementation of the Timer class as declared in timer.h.
 */

#define INTERNAL_INCLUDE 1
#include "timer.h"
#include <sys/time.h>
#define INTERNAL_INCLUDE 1
#include "error.h"
#undef INTERNAL_INCLUDE

Timer::Timer(bool autostart) {
    _startMS = 0;
    _stopMS = 0;
    _isStarted = false;
    if (autostart) {
        start();
    }
}

long Timer::elapsed() const {
    return _stopMS - _startMS;
}

bool Timer::isStarted() const {
    return _isStarted;
}

void Timer::start() {
    _startMS = currentTimeMS();
    _isStarted = true;
}

long Timer::stop() {
    _stopMS = currentTimeMS();
    if (!_isStarted) {
        // error("Timer is not started");
        _startMS = _stopMS;
    }
    _isStarted = false;
    return elapsed();
}

long Timer::currentTimeMS() {
    timeval time;
    gettimeofday(&time, nullptr);
    return (time.tv_sec * 1000000 + time.tv_usec) / 1000;
}

/*
 * File: note.cpp
 * --------------
 * This file implements the body of each member of the Note class.
 *
 * @author Marty Stepp
 * @version 2017/09/29
 * - updated to use composite hashCode function
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 * @version 2016/09/26
 * - initial version
 * @since 2016/09/26
 */

#define INTERNAL_INCLUDE 1
#include "note.h"
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gmath.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

// line e.g. "1.5 G 5 NATURAL false"
Note::Note(std::string line) {
    std::istringstream input(line);

    // parse duration
    double dur;
    if (!(input >> dur)) {
        error("illegal/missing duration");
    }
    setDuration(dur);

    // parse pitch
    std::string pit;
    if (!(input >> pit)) {
        error("illegal/missing pitch");
    }
    if (pit == "A") {
        setPitch(A);
    } else if (pit == "B") {
        setPitch(B);
    } else if (pit == "C") {
        setPitch(C);
    } else if (pit == "D") {
        setPitch(D);
    } else if (pit == "E") {
        setPitch(E);
    } else if (pit == "F") {
        setPitch(F);
    } else if (pit == "G") {
        setPitch(G);
    } else if (pit == "R") {
        setPitch(R);
    } else {
        error("illegal pitch");
    }

    if (isRest()) {
        // rests always have fixed octave and accidental values
        setOctave(OCTAVE_MIN + 1);
        setAccidental(NATURAL);
    } else {
        // parse octave
        int oct;
        if (!(input >> oct)) {
            error("illegal/missing octave");
        }
        setOctave(oct);

        // parse accidental
        std::string acc;
        if (!(input >> acc)) {
            error("illegal/missing accidental");
        }
        if (acc == "SHARP") {
            setAccidental(SHARP);
        } else if (acc == "NATURAL") {
            setAccidental(NATURAL);
        } else if (acc == "FLAT") {
            setAccidental(FLAT);
        } else {
            error("illegal accidental");
        }
    }

    // parse repeat
    std::string repStr;
    if (!(input >> repStr)) {
        error("illegal/missing repeat");
    }
    if (repStr == "true") {
        setRepeat(true);
    } else if (repStr == "false") {
        setRepeat(false);
    } else {
        error("illegal repeat");
    }
}

Note::Note(double duration, Pitch pitch, int octave, Accidental accidental, bool repeat) {
    setDuration(duration);
    setPitch(pitch);
    setOctave(octave);
    setAccidental(accidental);
    setRepeat(repeat);
}

bool Note::equals(const Note& note2) const {
    return floatingPointEqual(duration, note2.duration)
            && pitch == note2.pitch
            && octave == note2.octave
            && accidental == note2.accidental
            && repeat == note2.repeat;
}

Note::Accidental Note::getAccidental() const {
    return accidental;
}

double Note::getDuration() const {
    return duration;
}

int Note::getOctave() const {
    return octave;
}

Note::Pitch Note::getPitch() const {
    return pitch;
}

bool Note::isRepeat() const {
    return repeat;
}

bool Note::isRest() const {
    return pitch == R;
}

void Note::play() const {
#ifdef NOTE_DEBUG
    if (NOTE_DEBUG) {
        printf("%s\n", toString().c_str());
    }
#endif // NOTE_DEBUG
    // TODO
    // stanfordcpplib::getPlatform()->note_play(toString());
}

void Note::setAccidental(Note::Accidental accidental) {
    if (accidental != NATURAL && accidental != SHARP && accidental != FLAT) {
        error("ilegal accidental value");
    }
    if (isRest()) {
        this->accidental = NATURAL;
    } else {
        this->accidental = accidental;
    }
}

void Note::setDuration(double duration) {
    if (duration < 0.0) {
        error("ilegal negative duration");
    }
    this->duration = duration;
}

void Note::setOctave(int octave) {
    if (octave < OCTAVE_MIN || octave > OCTAVE_MAX) {
        error("illegal octave");
    }
    if (isRest()) {
        this->octave = OCTAVE_MIN + 1;
    } else {
        this->octave = octave;
    }
}

void Note::setPitch(Note::Pitch pitch) {
    if (pitch != A && pitch != B && pitch != C && pitch != D && pitch != E
            && pitch != F && pitch != G && pitch != R) {
        error("illegal pitch");
    }
    this->pitch = pitch;
}

void Note::setRepeat(bool repeat) {
    this->repeat = repeat;
}

std::string Note::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

bool operator ==(const Note& note1, const Note& note2) {
    return note1.equals(note2);
}

bool operator !=(const Note& note1, const Note& note2) {
    return !note1.equals(note2);
}

std::ostream& operator <<(std::ostream& out, const Note& note) {
    static std::string PITCH_STR[] {"A", "B", "C", "D", "E", "F", "G", "R"};
    static std::string ACCIDENTAL_STR[] {"SHARP", "NATURAL", "FLAT"};

    out << note.getDuration() << " " << PITCH_STR[note.getPitch()] << " ";
    if (!note.isRest()) {
        out << note.getOctave() << " " << ACCIDENTAL_STR[note.getAccidental()] << " ";
    }
    return out << std::boolalpha << note.isRepeat();
}

int hashCode(const Note& note) {
    return hashCode(
            note.getDuration(),
            note.getPitch(),
            note.getOctave(),
            note.getAccidental(),
            note.isRepeat());
}

/*
 * File: sound.cpp
 * ---------------
 * Implementation of the Sound class.
 * 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unused include statements
 */

#define INTERNAL_INCLUDE 1
#include "sound.h"
#include <QUrl>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

/*static*/ QMediaPlayer* Sound::_qmediaPlayer = nullptr;

/*static*/ long Sound::getDuration() {
    initialize();
    return _qmediaPlayer->duration();
}

/*static*/ int Sound::getVolume() {
    initialize();
    return _qmediaPlayer->volume();
}

/*static*/ void Sound::initialize() {
    if (!_qmediaPlayer) {
        GThread::runOnQtGuiThread([]() {
            if (!_qmediaPlayer) {
                _qmediaPlayer = new QMediaPlayer;
            }
        });
    }
}

/*static*/ void Sound::pause() {
    initialize();
    _qmediaPlayer->pause();
}

/*static*/ void Sound::playSound(const std::string& filename) {
    initialize();
    std::string absPath = getAbsolutePath(filename);
    if (!fileExists(absPath)) {
        error("Sound::playSound: file not found: " + filename);
    }

    GThread::runOnQtGuiThreadAsync([absPath]() {
        _qmediaPlayer->setMedia(QUrl::fromLocalFile(QString::fromStdString(absPath)));
        _qmediaPlayer->play();
    });
}

/*static*/ void Sound::stop() {
    initialize();
    _qmediaPlayer->stop();
}

/*static*/ void Sound::setVolume(int volume) {
    initialize();
    require::inRange(volume, 0, 100, "Sound::setVolume", "volume");
    _qmediaPlayer->setVolume(volume);
}

Sound::Sound(std::string filename)
        : _filename(filename) {
    initialize();
}

Sound::~Sound() {
    // TODO
}

void Sound::play() {
    playSound(_filename);
}

/*
 * memory.cpp
 * ----------
 * This file defines utility functions dealing with memory regions and
 * allocation.
 *
 * @author Marty Stepp
 * @version 2018/12/04
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "memory.h"
#include <cstddef>
#undef INTERNAL_INCLUDE

namespace stanfordcpplib {
namespace memory {

void computeMemoryDistances(
        void* const p,
        unsigned long int& stackDist,
        unsigned long int& heapDist,
        unsigned long int& staticDist) {
    static int si = 0;
    int i = 0;
    int* pi = new int(0);

    // don't use abs/subtraction here because it can over/underflow with very large integers
    stackDist = p > &i ?
            reinterpret_cast<unsigned long int>(p) - reinterpret_cast<unsigned long int>(&i) :
            reinterpret_cast<unsigned long int>(&i) - reinterpret_cast<unsigned long int>(p);
    heapDist = p > pi ?
            reinterpret_cast<unsigned long int>(p) - reinterpret_cast<unsigned long int>(pi) :
            reinterpret_cast<unsigned long int>(pi) - reinterpret_cast<unsigned long int>(p);
    staticDist = p > &si ?
            reinterpret_cast<unsigned long int>(p) - reinterpret_cast<unsigned long int>(&si) :
            reinterpret_cast<unsigned long int>(&si) - reinterpret_cast<unsigned long int>(p);
    delete pi;
}

bool isOnHeap(void* const p) {
    unsigned long int stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return heapDist < stackDist && heapDist < staticDist;
}

bool isOnStack(void* const p) {
    unsigned long int stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return stackDist < heapDist && stackDist < staticDist;
}

bool isOnStatic(void* const p) {
    unsigned long int stackDist, heapDist, staticDist;
    computeMemoryDistances(p, stackDist, heapDist, staticDist);
    return staticDist < stackDist && staticDist < heapDist;
}

} // namespace memory
} // namespace stanfordcpplib

/*
 * File: random.cpp
 * ----------------
 * This file implements the random.h interface.
 * 
 * @version 2017/10/05
 * - added randomFeedClear
 * @version 2017/09/28
 * - moved random 'feed' functions into autograder namespace
 * - ensure that randomly fed integers are within the specified range
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/08/02
 * - added randomColor, randomColorString
 * @version 2014/10/19
 * - alphabetized functions
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "random.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <queue>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

/* Private function prototype */

static void initRandomSeed();

/* internal buffer of fixed random numbers to return; used by autograders */
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(std::queue<bool>, fixedBools)
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(std::queue<int>, fixedInts)
STATIC_VARIABLE_DECLARE_COLLECTION_EMPTY(std::queue<double>, fixedReals)

namespace autograder {
void randomFeedBool(bool value) {
    STATIC_VARIABLE(fixedBools).push(value);
}

void randomFeedClear() {
    STATIC_VARIABLE(fixedBools) = std::queue<bool>();
    STATIC_VARIABLE(fixedInts) = std::queue<int>();
    STATIC_VARIABLE(fixedReals) = std::queue<double>();
}

void randomFeedInteger(int value) {
    STATIC_VARIABLE(fixedInts).push(value);
}

void randomFeedReal(double value) {
    STATIC_VARIABLE(fixedReals).push(value);
}
}
/* end 'fixed' internal stuff */

bool randomBool() {
    return randomChance(0.5);
}

/*
 * Implementation notes: randomChance
 * ----------------------------------
 * The code for randomChance calls randomReal(0, 1) and then checks
 * whether the result is less than the requested probability.
 */
bool randomChance(double p) {
    if (!STATIC_VARIABLE(fixedBools).empty()) {
        bool top = STATIC_VARIABLE(fixedBools).front();
        STATIC_VARIABLE(fixedBools).pop();
        return top;
    }
    initRandomSeed();
    return randomReal(0, 1) < p;
}

int randomColor() {
    if (!STATIC_VARIABLE(fixedInts).empty()) {
        int top = STATIC_VARIABLE(fixedInts).front();
        STATIC_VARIABLE(fixedInts).pop();
        return top & 0x00ffffff;
    }
    initRandomSeed();
    return rand() & 0x00ffffff;
}

// see convertRGBToColor in gcolor.h (repeated here to avoid Qt dependency)
std::string randomColorString() {
    int rgb = randomColor();
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (rgb >> 16 & 0xFF);
    os << std::setw(2) << (rgb >> 8 & 0xFF);
    os << std::setw(2) << (rgb & 0xFF);
    return os.str();
}

/*
 * Implementation notes: randomInteger
 * -----------------------------------
 * The code for randomInteger produces the number in four steps:
 *
 * 1. Generate a random real number d in the range [0 .. 1).
 * 2. Scale the number to the range [0 .. N) where N is the number of values.
 * 3. Translate the number so that the range starts at the appropriate value.
 * 4. Convert the result to the next lower integer.
 *
 * The implementation is complicated by the fact that both the expression
 *
 *     RAND_MAX + 1
 *
 * and the expression for the number of values
 *
 *     high - low + 1
 *
 * can overflow the integer range.  These calculations must therefore be
 * performed using doubles instead of ints.
 */
int randomInteger(int low, int high) {
    if (!STATIC_VARIABLE(fixedInts).empty()) {
        int top = STATIC_VARIABLE(fixedInts).front();
        STATIC_VARIABLE(fixedInts).pop();
        if (top < low || top > high) {
            // make sure the value is in the given range
            // (assumes that low/high don't overflow int range)
            int range = high - low + 1;
            top = low + std::abs(top) % range;
        }
        return top;
    }
    initRandomSeed();
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (double(high) - low + 1);
    return int(floor(low + s));
}

/*
 * Implementation notes: randomReal
 * --------------------------------
 * The code for randomReal is similar to that for randomInteger,
 * without the final conversion step.
 */
double randomReal(double low, double high) {
    if (!STATIC_VARIABLE(fixedReals).empty()) {
        double top = STATIC_VARIABLE(fixedReals).front();
        STATIC_VARIABLE(fixedReals).pop();
        return top;
    }
    initRandomSeed();
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (high - low);
    return low + s;
}

/*
 * Implementation notes: setRandomSeed
 * -----------------------------------
 * The setRandomSeed function simply forwards its argument to srand.
 * The call to initRandomSeed is required to set the initialized flag.
 */
void setRandomSeed(int seed) {
    initRandomSeed();
    srand(seed);
}

/*
 * Implementation notes: initRandomSeed
 * ------------------------------------
 * The initRandomSeed function declares a static variable that keeps track
 * of whether the seed has been initialized.  The first time initRandomSeed
 * is called, initialized is false, so the seed is set to the current time.
 */
static void initRandomSeed() {
    static bool _initialized = false;
    if (!_initialized) {
        srand(int(time(nullptr)));
        rand();   // BUGFIX: throwaway call to get randomness going
        _initialized = true;
    }
}

/*
 * File: recursion.cpp
 * -------------------
 * This file implements the recursion.h interface.
 */

#define INTERNAL_INCLUDE 1
#include "recursion.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "call_stack.h"
#undef INTERNAL_INCLUDE

int getRecursionIndentLevel() {
    // constructing the following object jumps into fancy code in call_stack_gcc/windows.cpp
    // to rebuild the stack trace; implementation differs for each operating system
    stacktrace::call_stack trace;
    Vector<stacktrace::entry> entries = trace.stack;

    std::string currentFunction = "";
    int currentFunctionCount = 0;
    for (int i = 0; i < entries.size(); ++i) {
        // remove references to std:: namespace
        if (exceptions::shouldFilterOutFromStackTrace(entries[i].function)
                || entries[i].function.find("recursionIndent(") != std::string::npos
                || entries[i].function.find("getRecursionIndentLevel(") != std::string::npos) {
            continue;
        } else if (currentFunction.empty()) {
            currentFunction = entries[i].function;
            currentFunctionCount = 1;
        } else if (entries[i].function == currentFunction) {
            currentFunctionCount++;
        } else {
            break;
        }
    }
    return currentFunctionCount;
}

std::string recursionIndent(const std::string& indenter) {
    int indent = getRecursionIndentLevel();
    std::string result = "";
    for (int i = 0; i < indent - 1; i++) {
        result += indenter;
    }
    return result;
}

/*
 * File: direction.cpp
 * -------------------
 * This file implements the direction.h interface.
 * 
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "direction.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "tokenscanner.h"
#undef INTERNAL_INCLUDE

/*
 * Implementation notes: leftFrom, rightFrom, opposite
 * ---------------------------------------------------
 * These functions use the remainder operator to cycle through the
 * internal values of the enumeration type.  Note that the leftFrom
 * function cannot subtract 1 from the direction because the result
 * might then be negative; adding 3 achieves the same effect but
 * ensures that the values remain positive.
 */

Direction leftFrom(Direction dir) {
    return Direction((dir + 3) % 4);
}

Direction rightFrom(Direction dir) {
    return Direction((dir + 1) % 4);
}

Direction opposite(Direction dir) {
    return Direction((dir + 2) % 4);
}

/*
 * Implementation notes: directionToString
 * ---------------------------------------
 * The C++ compiler requires the default clause to ensure that this
 * function always returns a string, even if the direction is not one
 * of the legal values.
 */

std::string directionToString(Direction dir) {
    switch (dir) {
    case NORTH:
        return "NORTH";
    case EAST:
        return "EAST";
    case SOUTH:
        return "SOUTH";
    default:
    // case WEST:
        return "WEST";   // BUGFIX 2014/07/09: removed unreachable 'default' case
    }
}

/*
 * Implementation notes: <<
 * ------------------------
 * This operator must return the stream by reference after printing
 * the value.  The operator << returns this stream, so the function
 * can be implemented as a single line.
 */

std::ostream& operator <<(std::ostream& os, const Direction& dir) {
    return os << directionToString(dir);
}

/*
 * Implementation notes: >>
 * ------------------------
 * This implementation uses the TokenScanner to read tokens from the
 * stream.
 */

std::istream& operator >>(std::istream& is, Direction& dir) {
    TokenScanner scanner(is);
    scanner.ignoreWhitespace();
    std::string token = toUpperCase(scanner.nextToken());
    if (token == "") {
        dir = Direction(-1);
    } else if (startsWith("NORTH", token)) {
        dir = NORTH;
    } else if (startsWith("EAST", token)) {
        dir = EAST;
    } else if (startsWith("SOUTH", token)) {
        dir = SOUTH;
    } else if (startsWith("WEST", token)) {
        dir = WEST;
    } else {
#ifdef SPL_ERROR_ON_COLLECTION_PARSE
        error("Direction::operator >>: Unrecognized direction \"" + token + "\"");
#endif
        is.setstate(std::ios_base::failbit);
        return is;
    }
    return is;
}

/*
 * Implementation notes: ++
 * ------------------------
 * The int parameter in the signature for this operator is a marker used
 * by the C++ compiler to identify the suffix form of the operator.  Note
 * that the value after incrementing a variable containing WEST will be
 * out of the Direction range.  That fact will not cause a problem if
 * this operator is used only in the for loop idiom for which it is defined.
 */

Direction operator ++(Direction& dir, int) {
    Direction old = dir;
    dir = Direction(dir + 1);
    return old;
}

/*
 * File: stringutils.cpp
 * ---------------------
 * This file contains implementations of utility functions related to strings.
 * See stringutils.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2017/10/20
 * - changed string to const string& in all functions
 * @version 2016/11/09
 * - added trimToSize function
 * @version 2014/10/31
 * - fixed bug in string trimToHeight function
 * @since 2014/03/01
 */

#define INTERNAL_INCLUDE 1
#include "stringutils.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>
#undef INTERNAL_INCLUDE

namespace stringutils {
int charsDifferent(const std::string& s1, const std::string& s2) {
    int length = std::min(s1.length(), s2.length());
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (s1[i] != s2[i]) {
            count++;
        }
    }
    return count;
}

std::string collapseSpaces(const std::string& s) {
    std::ostringstream result;
    char last = '\0';
    for (int i = 0; i < (int) s.length(); i++) {
        if (s[i] == ' ' && last == ' ') {
            continue;
        }
        last = s[i];
        result << last;
    }
    return result.str();
}

Vector<std::string> explodeLines(const std::string& s) {
    Vector<std::string> result;
    std::string line;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == '\n') {
            // end of line
            result.push_back(trimR(line));
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }
    if ((int) line.length() > 0 || s.empty()) {
        result.push_back(line);
    }
    return result;
}

int height(const std::string& s) {
    std::string line;
    int height = 0;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == '\n') {
            // end of line
            height++;
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }
    if ((int) line.length() > 0) {
        height++;
    }
    return height;
}

std::string implode(const Vector<std::string>& v, const std::string& delimiter) {
    if (v.isEmpty()) {
        return "";
    } else {
        std::string result = v[0];
        for (int i = 1; i < v.size(); i++) {
            result += delimiter;
            result += v[i];
        }
        return result;
    }
}

std::string indent(const std::string& s, int spaces) {
    Vector<std::string> lines = explodeLines(s);
    Vector<std::string> newLines;
    std::string indentStr = "";
    for (int i = 0; i < spaces; i++) {
        indentStr += " ";
    }
    for (const std::string& line : lines) {
        newLines.add(indentStr + line);
    }
    return implode(newLines);
}

std::string makeSloppy(const std::string& s) {
    return collapseSpaces(removeBlankLines(s));
}

std::string removeBlankLines(const std::string& s) {
    Vector<std::string> lines = explodeLines(s);
    Vector<std::string> newLines;
    for (std::string line : lines) {
        line = trimR(line);
        if (line.length() > 0) {
            newLines.add(line);
        }
    }
    return implode(newLines, "\n");
}

std::string stripWhitespace(const std::string& s) {
    std::ostringstream result;
    for (size_t i = 0; i < s.length(); i++) {
        if (!isspace(s[i])) {
            result << tolower(s[i]);
        }
    }
    return result.str();
}

std::string toLowerCase(const std::string& s) {
    std::string result;
    for (size_t i = 0; i < s.length(); i++) {
        result += tolower(s[i]);
    }
    return result;
}

std::string toPrintable(int ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == ' ') {
        return "' '";
    } else if (!isgraph(ch)) {
        return "???";
    } else {
        return std::string("'") + (char) ch + std::string("'");
    }
}

std::string trimR(const std::string& s) {
    std::string trimmed = s;
    while (!trimmed.empty() && isspace(trimmed[trimmed.length() - 1])) {
        trimmed.erase(trimmed.length() - 1, 1);
    }
    return trimmed;
}

std::string trimToHeight(const std::string& s, int height, const std::string& suffix) {
    Vector<std::string> lines = explodeLines(s);
    int lineCount = lines.size();
    bool wasTooTall = lineCount > height;
    while (lines.size() > height) {
        lines.remove(lines.size() - 1);
    }
    if (wasTooTall && !suffix.empty()) {
        lines.add(suffix + " (" + std::to_string(lineCount - height) + " line(s) truncated)");
    }
    return implode(lines);
}

std::string trimToSize(const std::string& s, int width, int height, const std::string& suffix) {
    std::string trimmed = trimToHeight(s, height, suffix);
    trimmed = trimToWidth(trimmed, width, suffix);
    return trimmed;
}

std::string trimToWidth(const std::string& s, int width, const std::string& suffix) {
    Vector<std::string> lines = explodeLines(s);
    for (int i = 0; i < lines.size(); i++) {
        if ((int) lines[i].length() > width) {
            lines[i] = lines[i].substr(0, width);
            if (!suffix.empty()) {
                lines[i] += suffix;
            }
        }
    }
    return implode(lines);
}

// truncate string with ... between
std::string truncate(const std::string& s, int length, const std::string& suffix) {
    int slength = s.length();
    if (slength > length) {
        return s.substr(0, length) + suffix;
    } else {
        return s;
    }
}

int width(const std::string& s) {
    std::string line;
    size_t width = 0;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == '\n') {
            // end of line
            if (line.length() > width) {
                width = line.length();
            }
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }
    if ((int) line.length() > 0) {
        if (line.length() > width) {
            width = line.length();
        }
    }
    return width;
}
} // namespace stringutils

/*
 * File: diff.cpp
 * --------------
 * This file contains implementations of functions that perform a text 'diff'
 * operation to compare two strings and output the differences.
 * See diff.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2016/10/30
 * - fixed diff flags; added punctuation flag
 * @version 2016/10/22
 * - added diffPass (for autograder assertDiff)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#define INTERNAL_INCLUDE 1
#include "diff.h"
#include <algorithm>
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "regexpr.h"
#define INTERNAL_INCLUDE 1
#include "set.h"
#define INTERNAL_INCLUDE 1
#include "stringutils.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

namespace diff {
std::string diff(std::string s1, std::string s2, int flags) {
    Vector<std::string> lines1 = stringutils::explodeLines(s1);
    Vector<std::string> lines2 = stringutils::explodeLines(s2);
    Vector<std::string> lines1Original = lines1;
    Vector<std::string> lines2Original = lines2;

    if (flags & IGNORE_NUMBERS) {
        s1 = regexReplace(s1, "[0-9]+", "###");
        s2 = regexReplace(s2, "[0-9]+", "###");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_NONNUMBERS) {
        s1 = regexReplace(s1, "[^0-9\n]+", " ");
        s2 = regexReplace(s2, "[^0-9\n]+", " ");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_PUNCTUATION) {
        std::string punct = "[.,?!'\"()\\/#$%@^&*_\\[\\]{}|<>:;-]+";
        s1 = regexReplace(s1, punct, "");
        s2 = regexReplace(s2, punct, "");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_AFTERDECIMAL) {
        s1 = regexReplace(s1, "\\.[0-9]+", ".#");
        s2 = regexReplace(s2, "\\.[0-9]+", ".#");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_CASE) {
        s1 = stringutils::toLowerCase(s1);
        s2 = stringutils::toLowerCase(s2);
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_CHARORDER) {
        Vector<std::string> lines1Sorted;
        for (std::string line : lines1) {
            sort(line.begin(), line.end());
            lines1Sorted.push_back(line);
        }
        lines1 = lines1Sorted;
        s1 = stringutils::implode(lines1);

        Vector<std::string> lines2Sorted;
        for (std::string line : lines2) {
            sort(line.begin(), line.end());
            lines2Sorted.push_back(line);
        }
        lines2 = lines2Sorted;
        s2 = stringutils::implode(lines2);
    }
    if (flags & IGNORE_LINEORDER) {
        sort(lines1.begin(), lines1.end());
        sort(lines2.begin(), lines2.end());
        s1 = stringutils::implode(lines1);
        s2 = stringutils::implode(lines2);
    }
    if (flags & IGNORE_WHITESPACE) {
        for (int i = 0; i < lines1.size(); i++) {
            lines1[i] = stringutils::stripWhitespace(lines1[i]);
        }
        for (int i = 0; i < lines2.size(); i++) {
            lines2[i] = stringutils::stripWhitespace(lines2[i]);
        }
    }

    if (stringutils::trimR(s1) == stringutils::trimR(s2)) {
        return NO_DIFFS_MESSAGE;
    }

    // build a reverse-index array using the line as key and line number as value
    // don't store blank lines, so they won't be targets of the shortest distance
    // search
    Map<std::string, Set<int> > reverse1;
    Map<std::string, Set<int> > reverse2;
    for (int i = 0; i < lines1.size(); i++) {
        std::string line = lines1[i];
        if ((int) line.length() > 0) {
            reverse1[line].add(i);
        }
    }
    for (int i = 0; i < lines2.size(); i++) {
        std::string line = lines2[i];
        if ((int) line.length() > 0) {
            reverse2[line].add(i);
        }
    }

    // indexes of current lines within each file
    // (start at beginning of each list)
    int index1 = 0;
    int index2 = 0;
    Vector<int> actions;

    // walk this loop until we reach the end of one of the lists of lines
    while (index1 < lines1.size() && index2 < lines2.size()) {
        // if we have a common line, save it and go to the next
        if (lines1[index1] == lines2[index2]) {
            actions += 4;
            index1++;
            index2++;
            continue;
        }
        // otherwise, find the shortest move (Manhattan-distance) from the
        // current location
        int best1 = lines1.size();
        int best2 = lines2.size();
        int s1 = index1;
        int s2 = index2;
        while ((s1 + s2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
            int d = -1;
            if (lines2.size() > s2 && reverse1.containsKey(lines2[s2])) {
                for (int lineNumber : reverse1[lines2[s2]]) {
                    if (lineNumber >= s1) {
                        d = lineNumber;
                        break;
                    }
                }
            }
            if (d >= s1 && (d + s2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
                best1 = d;
                best2 = s2;
            }

            d = -1;
            if (lines1.size() > s1 && reverse2.containsKey(lines1[s1])) {
                for (int lineNumber : reverse2[lines1[s1]]) {
                    if (lineNumber >= s2) {
                        d = lineNumber;
                        break;
                    }
                }
            }
            if (d >= s2 && (d + s1 - index1 - index2) < (best1 + best2 - index1 - index2)) {
                best1 = s1;
                best2 = d;
            }

            s1++;
            s2++;
        }

        while (index1 < best1) {
            actions += 1;
            index1++;
        }  // deleted elements

        while (index2 < best2) {
            actions += 2;
            index2++;
        }  // added elements
    }

    // we've reached the end of one list, now walk to the end of the other
    while (index1 < lines1.size()) {
        actions += 1;
        index1++;
    }  // deleted elements

    if (flags & IGNORE_TRAILING) {
        while (index2 < lines2.size()) {
            actions += 2;
            index2++;
        }  // added elements
    }

    // and this marks our ending point
    actions += 8;

    int op = 0;
    int x0 = 0;
    int x1 = 0;
    int y0 = 0;
    int y1 = 0;
    Vector<std::string> out;

    for (int action : actions) {
        if (action == 1) {
            op |= action;
            x1++;
            continue;
        } else if (action == 2) {
            op |= action;
            y1++;
            continue;
        }

        if (op > 0) {
            bool multipleLines = (x1 != x0 + 1);
            std::string xstr = std::string("") + (multipleLines ? (std::to_string(x0 + 1) + "-" + std::to_string(x1)) : std::to_string(x1));
            std::string ystr = std::string("") + ((y1 != y0 + 1) ? (std::to_string(y0 + 1) + "-" + std::to_string(y1)) : std::to_string(y1));
            std::string linesStr = std::string("\nLine") + (multipleLines ? "s " : " ");
            std::string doStr = std::string("do") + (multipleLines ? "" : "es");
            if (op == 1) {
                out += linesStr + xstr + " deleted near student line " + std::to_string(y1);
            } else if (op == 3) {
                if (xstr == ystr) {
                    out += linesStr + xstr + " " + doStr + " not match";
                } else {
                    out += linesStr + xstr + " changed to student line " + ystr;
                }
            }

            while (x0 < x1) {
                out += "EXPECTED < " + lines1Original[x0];
                x0++;
            }   // deleted elems

            if (op == 2) {
                if (!(flags & IGNORE_LEADING) || x1 > 0) {
                    out += linesStr + std::to_string(x1) + " added at student line " + ystr;
                }
            } else if (op == 3) {
                // out += "---";
            }

            while (y0 < y1) {
                if (!(flags & IGNORE_LEADING) || op != 2 || x1 > 0) {
                    out += "STUDENT  > " + lines2Original[y0];
                }
                y0++;
            }   // added elems
        }
        x1++;
        x0 = x1;
        y1++;
        y0 = y1;
        op = 0;
    }

    if (out.size() > 0) {
        out += "";
        return trim(stringutils::implode(out, "\n"));
    } else {
        return NO_DIFFS_MESSAGE;
    }
}

bool diffPass(const std::string& s1, const std::string& s2, int flags) {
    std::string diffs = diff(s1, s2, flags);
    bool result = trim(diffs) == NO_DIFFS_MESSAGE;
    return result;
}

bool isDiffMatch(const std::string& diffs) {
    return trim(diffs) == NO_DIFFS_MESSAGE;
}
} // namespace diff

/*
 * File: regexpr.cpp
 * -----------------
 * Implementation of the functions in regexpr.h.
 * See regexpr.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2018/12/16
 * - added CodeStepByStep disabling of regexes
 * @version 2018/11/22
 * - added headless (non-Qt) mode support
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/03/01
 */

#define INTERNAL_INCLUDE 1
#include "regexpr.h"
#ifndef SPL_HEADLESS_MODE
#include <QtGlobal>
#endif // SPL_HEADLESS_MODE
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "stringutils.h"
#undef INTERNAL_INCLUDE

#if defined(SPL_CODESTEPBYSTEP) || QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
bool regexMatch(const std::string& /*s*/, const std::string& /*regexp*/) {
    return false;   // not supported
}

int regexMatchCount(const std::string& /*s*/, const std::string& /*regexp*/) {
    return 0;   // not supported
}

void regexMatchCountWithLines(const std::string& /*s*/, const std::string& /*regexp*/,
                             Vector<int>& /*linesOut*/) {
    // empty; not supported
}

std::string regexReplace(const std::string& s, const std::string& /*regexp*/, const std::string& /*replacement*/, int /*limit*/) {
    return s;   // not supported
}

#else // QT_VERSION

// C++ regex support
#include <iterator>
#include <regex>

bool regexMatch(const std::string& s, const std::string& regexp) {
    std::regex reg(regexp);
    std::smatch match;
    return std::regex_search(s, match, reg);
}

int regexMatchCount(const std::string& s, const std::string& regexp) {
    std::regex reg(regexp);
    auto it1 = std::sregex_iterator(s.begin(), s.end(), reg);
    auto it2 = std::sregex_iterator();
    return std::distance(it1, it2);
}

void regexMatchCountWithLines(const std::string& s, const std::string& regexp,
                             Vector<int>& linesOut) {
    linesOut.clear();

    // keep a running index and line#, and each time we find a regex match,
    // "walk" forward to its index.  when we see a \n, increment line number
    int currentIndex = 0;
    int currentLine = 1;

    // get all regex matches by character position/index
    std::regex reg(regexp);
    for (std::sregex_iterator itr = std::sregex_iterator(s.begin(), s.end(), reg),
            end = std::sregex_iterator();
            itr != end;
            ++itr) {
        std::smatch match = *itr;
        int matchIndex = (int) match.position();
        while (currentIndex < (int) s.length() && currentIndex < matchIndex) {
            if (s[currentIndex] == '\n') {
                currentLine++;
            }
            currentIndex++;
        }
        linesOut.add(currentLine);
    }
}

std::string regexReplace(const std::string& s, const std::string& regexp, const std::string& replacement, int limit) {
    std::regex reg(regexp);
    std::string result;
    if (limit == 1) {
        // replace single occurrence
        result = std::regex_replace(s, reg, replacement,
                                    std::regex_constants::format_first_only);
    } else if (limit <= 0) {
        // replace all
        result = std::regex_replace(s, reg, replacement);
    } else {
        error("regexReplace: given limit not supported.");
    }
    return result;
}
#endif // QT_VERSION

// this function can be implemented the same way whether regexes are available or not
int regexMatchCountWithLines(const std::string& s, const std::string& regexp, std::string& linesOut) {
    Vector<int> linesOutVec;
    regexMatchCountWithLines(s, regexp, linesOutVec);

    // concatenate the vector into a string like "1, 4, 7, 7, 19"
    linesOut = "";
    if (!linesOutVec.isEmpty()) {
        linesOut += std::to_string(linesOutVec[0]);
        for (int i = 1; i < linesOutVec.size(); i++) {
            linesOut += ", " + std::to_string(linesOutVec[i]);
        }
    }
    return linesOutVec.size();
}

//#define INTERNAL_INCLUDE 1
//#include "bigfloat.h"
//#undef INTERNAL_INCLUDE
//#include <limits>

//bool BigFloatCache::_isInitialized = false;
//BigFloat BigFloatCache::_e;
//BigFloat BigFloatCache::_zero;
//BigFloat* BigFloatCache::_ePower;
//s8 BigFloatCache::_ePowerLen;
//BigFloat* BigFloatCache::_eInvPower;
//BigFloat BigFloatCache::_pi;
//BigFloat BigFloatCache::_twoPi;
//BigFloat BigFloatCache::_overTwoPi;
//BigFloat BigFloatCache::_piOverTwo;
//BigFloat BigFloatCache::_threePiOverTwo;
//BigFloat BigFloatCache::_piOverFour;
//s8 BigFloatCache::_overFactLen;
//BigFloat* BigFloatCache::_overFact;

//void BigFloatCache::Init()
//{
//    s8 i;
//    const int c_maxIter = 1000;

//    // fill _pi using Brent-Salamin algorithm
//    BigFloat a(1);

//    BigFloat b(2);
//    b.Sqrt();
//    b.Invert();

//    BigFloat t(4);
//    t.Invert();

//    BigFloat p(1);

//    BigFloat oldPi(3);
//    BigFloat newPi;

//    // for 32 32-bit digits, this stops after 8 iterations
//    for (i=0; i<c_maxIter; ++i)
//    {
//        BigFloat a2(a);
//        a2.Add(b);
//        a2.Div(2);

//        b.Mult(a);
//        b.Sqrt();

//        BigFloat t2(a);
//        t2.Sub(a2);
//        t2.Mult(t2);
//        t2.Mult(p);
//        t.Sub(t2);
        
//        p.Mult(2);

//        a.Copy(a2);

//        newPi.Copy(a);
//        newPi.Add(b);
//        newPi.Mult(newPi);
//        newPi.Div(t);
//        newPi.Div(4);

//        if (newPi.Compare(oldPi) == 0)
//            break;

//        oldPi.Copy(newPi);
//    }
//    ASSERT(i < c_maxIter);

//    oldPi.Copy(a);
//    oldPi.Add(b);
//    oldPi.Mult(oldPi);
//    BigFloat piDivisor(t);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _twoPi.Copy(newPi);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _pi.Copy(newPi);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _piOverTwo.Copy(newPi);

//    piDivisor.Mult(2);
//    newPi.Copy(oldPi);
//    newPi.Div(piDivisor);
//    _piOverFour.Copy(newPi);  // higher precision than pi/2 or pi or 2pi

//    _threePiOverTwo.Copy(_pi);
//    _threePiOverTwo.Mult(3);
//    _threePiOverTwo.Div(2);
//    _overTwoPi.Copy(_twoPi);
//    _overTwoPi.Invert();

//    // fill _e using Brother's formula (7)
//    BigFloat e(0);
//    BigFloat oldE(-1);
//    BigFloat denom(1);

//    // for 32 32-bit digits, this takes 42 iterations
//    for (i=0; i<c_maxIter; ++i)
//    {
//        BigFloat term((8*i*i+1) * (8*i-4) + 5);
//        term.Div(denom);
//        e.Add(term);
//        if (e.Compare(oldE) == 0)
//        {
//            break;
//        }
//        oldE.Copy(e);
//        denom.Mult((4*i+1) * (4*i+2) * (4*i+3) * (4*i+4));
//    }
//    ASSERT(i < c_maxIter);
//    _e.Copy(e);
    
//    // precompute 1/n!, to reduce the cost of sin cos tan exp, etc.
//    _overFactLen = 4*i+32;
//    _overFact = new BigFloat[static_cast<unsigned int>(_overFactLen)];
//    BigFloat fact(1);
//    _overFact[0].Copy(1);
//    for (i = 1; i < _overFactLen; ++i)
//    {
//        fact.Mult(i);
//        _overFact[i].Copy(fact);
//        _overFact[i].Invert();
//    }

//    BigFloat power(_e);
//    for (i = 1; !power.IsSpecial(); ++i)
//    {
//        power.Mult(power);
//    }
//    _ePowerLen = i;
//    BigFloat* ePowerBuf = new BigFloat[static_cast<unsigned int>(_ePowerLen-_ePowerNeg)];
//    _ePower = &ePowerBuf[-_ePowerNeg];
//    _ePower[0].Copy(_e);
//    BigFloat* eInvPowerBuf = new BigFloat[static_cast<unsigned int>(_ePowerLen-_ePowerNeg)];
//    _eInvPower = &eInvPowerBuf[-_ePowerNeg];
//    _eInvPower[0].Copy(_e);
//    _eInvPower[0].Invert();

//    for (int i=1; i<_ePowerLen; ++i)
//    {
//        _ePower[i].Copy(_ePower[i-1]);
//        _ePower[i].Mult(_ePower[i]);
//        _eInvPower[i].Copy(_eInvPower[i-1]);
//        _eInvPower[i].Mult(_eInvPower[i]);
//    }

//    for (int i=-1; i>_ePowerNeg; --i)
//    {
//        _ePower[i].Copy(_ePower[1+i]);
//        _ePower[i].Sqrt();
//        _eInvPower[i].Copy(_eInvPower[1+i]);
//        _eInvPower[i].Sqrt();
//    }

//    _isInitialized = true;
//}


//// Do Gaussian elimination on m[rows][cols+1] (destructively).
//// Put the results in m[*][cols].
//void BigFloat::GaussianElimination(BigFloat** m, s8 rows, s8 cols)
//{
//    s8 i;  // currently working on ith column
//    s8 k;  // eliminating m[i] from m[k]
//    s8 j;  // eliminating m[i][j] from m[k][j]

//    // build a triangular matrix
//    for (i=0; i<cols; ++i)
//    {
//        // swap a row to i such that m[i][i] is nonzero
//        for (k=i; k<rows; ++k)
//        {
//            if (!m[k][i].IsZero())
//            {
//                break;
//            }
//        }
//        ASSERT(k < rows);
//        if (k != i)
//        {
//            BigFloat* temp = m[i];
//            m[i] = m[k];
//            m[k] = temp;
//        }

//        // do the elimination
//        for (k=i+1; k<rows; ++k)
//        {
//            BigFloat c(m[k][i]);
//            c.Div(m[i][i]);
//            for (j=i; j<cols+1; ++j)
//            {
//                BigFloat temp(m[i][j]);
//                temp.Mult(c);
//                m[k][j].Sub(temp);
//            }
//        }
//    }

//    // use the triangular matrix to find the results
//    for (i=cols; i--; )
//    {
//        for (j=i+1; j<cols; ++j)
//        {
//            BigFloat temp(m[i][j]);
//            temp.Mult(m[j][cols]);
//            m[i][cols].Sub(temp);
//        }
//        m[i][cols].Div(m[i][i]);
//    }
//}


//BigFloat& BigFloat::FromInteger(s8 origNum, s8 exponent)
//{
//    if (origNum == 0)
//    {
//        return Zero();
//    }

//    _isNegative = (origNum < 0);
//    u8 num = (u8)(_isNegative ? -origNum : origNum);

//    u8 shift = 64 - c_log;
//    _exponent = shift / c_log;
//    u8 mask = (c_range-1) << shift;
//    while (!(num & mask))
//    {
//        --_exponent;
//        num <<= c_log;
//    }

//    _length = 0;
//    _exponent += exponent;
//    while (num && _length < c_digits)
//    {
//        _d[_length++] = static_cast<u4>((num & mask) >> shift);
//        num <<= c_log;
//    }

//    // this shows up in test mode: allow truncation if too much precision
//    // ASSERT(!num, "integer had too much precision\n");

//    return *this;
//}


//// truncate if there is a fraction
//// throw an exception if it is bigger than an 8-byte signed integer
//// ignore the sign
//s8 BigFloat::ToInteger() const
//{
//    if (IsZero())
//        return 0;
    
//    ASSERT((_exponent < ((64-c_log)/c_log)) ||
//           (_exponent == (64-c_log)/c_log && _d[0] < c_range/2),
//           "exponent unsuitable for integer conversion");
    
//    s8 result = 0;
//    u8 pos = 0;
//    while (pos < _length)
//    {
//        result <<= c_log;
//        result += _d[pos++];
//    }
//    s8 shift = c_log * (_exponent + 1 - _length);
//    if (shift > 0)
//        result <<= shift;
//    else if (shift < 0)
//        result >>= -shift;
//    if (_isNegative)
//        result = -result;
//    return result;
//}


//// Ignore sign and exponent.
//// Stick the digits in a u8.
//// The highest digit is multiplied by (((u8)1) << (64-c_log))
//u8 BigFloat::ToDigits() const
//{
//    u8 result = 0;
//    for (int pos=_length; pos--; )
//    {
//        result >>= c_log;
//        result += (u8)_d[pos] << (64 - c_log);
//    }
//    return result;
//}


//// convert bigfloat to double
//double BigFloat::ToDouble() const
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return _isNegative ? -0.0 : 0.0;
//        else if (IsInf())
//            return _isNegative ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
//        else
//            return std::numeric_limits<double>::quiet_NaN();
//    }

//    double x = 0.0;

//    // handle the digits
//    for (int i=_length; i--; )
//    {
//        x /= (((u8)1) << c_log);
//        x += _d[i];
//    }

//    // handle the exponent
//    for (int i=0; i<_exponent; ++i)
//        x *= (((u8)1) << c_log);
//    for (int i=0; i>_exponent; --i)
//        x /= (((u8)1) << c_log);

//    // sign
//    if (_isNegative)
//        x = -x;
    
//    return x;
//}


//// print an accurate initializer for a double: (n / d) * 2^^x, where 1 <= n/d < 2
//void BigFloat::PrintContinuedFraction() const
//{
//    BigFloat n1; // numerator #1
//    BigFloat n2; // numerator #2
//    BigFloat d1; // denominator #1
//    BigFloat d2; // denominator #2
//    BigFloat r;  // remainder

//    if (IsZero())
//    {
//        printf("0.0");
//        return;
//    }
    
//    // find the power of 2 to put *this in [1.0, 2.0)
//    n1.Copy(*this);
//    s8 x = 0;
//    char sign[2] = {0, 0};
//    if (n1.IsNegative())
//    {
//        sign[0] = '-';
//        n1.Negate();
//    }

//    while (n1.Compare(2) >= 0)
//    {
//        n1.Div(2);
//        x += 1;
//    }
//    while (n1.Compare(1) < 0)
//    {
//        n1.Mult(2);
//        x -= 1;
//    }

//    if (x < -400)
//    {
//        printf("0.0");
//        return;
//    }
    
//    printf("(");
    
//    // fill n1/d1 with the first approximation
//    r.Copy(n1);
//    n1.Trunc();
//    d1.Copy(1);

//    // fill n2/d2 with the second approximation
//    r.Sub(n1);
//    if (r.IsZero())
//    {
//        printf("0x%llx)", n1.ToInteger());
//        return;
//    }
//    r.Invert();
//    d2.Copy(r).Trunc();
//    r.Sub(d2);
//    n2.Copy(n1).Mult(d2).Add(1);

//    // recurse until we get too precise: 1/r2 = c + r3, and n3/d3 = (c*n2 + n1)/(c*d2 + d1)
//    for (;;)
//    {
//        r.Invert();
//        BigFloat c(r);
//        c.Trunc();
//        r.Sub(c);

//        BigFloat temp(n2);
//        temp.Mult(c);
//        n1.Add(temp);

//        temp.Copy(d2);
//        temp.Mult(c);
//        d1.Add(temp);

//        if (r.IsZero() || n1._exponent*c_log >= 32)
//        {
//            printf("%sstatic_cast<double>(%llu) / %llu", sign, n2.ToInteger(), d2.ToInteger());
//            break;
//        }

//        r.Invert();
//        c.Copy(r).Trunc();
//        r.Sub(c);

//        temp.Copy(n1);
//        temp.Mult(c);
//        n2.Add(temp);

//        temp.Copy(d1);
//        temp.Mult(c);
//        d2.Add(temp);

//        if (r.IsZero() || n2._exponent*c_log >= 32)
//        {
//            printf("%sstatic_cast<double>(%llu) / %llu", sign, n1.ToInteger(), d1.ToInteger());
//            break;
//        }
//    }

//    if (0 < x && x < 32)
//    {
//        printf(" * 0x%llx", ((u8)1)<<x);
//    }
//    else if (0 > x && x > -32)
//    {
//        printf(" / 0x%llx", ((u8)1)<<(-x));
//    }
//    else
//    {
//        printf(" * PowerOfTwo(%d)", x);
//    }

//    printf(")");

//}


//void BigFloat::PrintDouble() const
//{
//    double x = ToDouble();
//    printf("%.19g", x);
//}


//// use continued fractions to convert this number to a ratio
//void BigFloat::ToFraction(BigFloat& num, BigFloat& denom, int iter) const
//{
//    BigFloat remainder(*this);
//    remainder._isNegative = false;
//    num.Copy(*this);
//    num.Trunc();
//    remainder.Sub(num);
//    if (remainder.IsZero() ||
//        remainder._exponent < -c_digits/2 ||
//        iter>=0)
//    {
//        denom.FromInteger(1);
//    }
//    else
//    {
//        remainder.Invert();
//        BigFloat subDenom;
//        remainder.ToFraction(denom, subDenom, iter-1);
//        num.Mult(denom);
//        num.Add(subDenom);
//    }
//    num._isNegative = this->_isNegative;
//}


//void BigFloat::Print() const
//{
//    printf("exp=%lld, %c ", _exponent, _isNegative ? '-' : '+');
//    for (int iPos=0; iPos < _length; ++iPos)
//        printf("%x ", _d[iPos]);
//    printf("\n");
//}


//void BigFloat::PrintHex() const
//{
//    if (_isNegative)
//        printf("-");
//    if (IsSpecial())
//    {
//        if (IsZero())
//            printf("0");
//        else if (IsInf())
//            printf("inf");
//        else if (IsNaN())
//            printf("NaN");
//    }
//    else if (_exponent+1 > _length || _exponent < 0)
//    {
//        printf("0x");
//        for (int i=0; i<_length; ++i)
//        {
//            if (i==0)
//            {
//                printf("%lx", _d[0]);
//            }
//            else
//            {
//                if (i==1)
//                    printf(".");
//                printf("%.8lx", _d[i]);
//            }
//        }
//        printf(":e%lld", _exponent);
//    }
//    else
//    {
//        printf("0x");
//        for (int i=0; i<_length; ++i)
//        {
//            if (i==0)
//            {
//                printf("%lx", _d[0]);
//            }
//            else
//            {
//                if (i == _exponent+1)
//                    printf(".");
//                printf("%.8lx", _d[i]);
//            }
//        }
//    }
//}


//BigFloat& BigFloat::Copy(const BigFloat& n)
//{
//    _exponent = n._exponent;
//    _isNegative = n._isNegative;
//    _length = n._length;
//    for (int iPos=_length; iPos--; )
//        _d[iPos] = n._d[iPos];
//    return *this;
//}


//BigFloat& BigFloat::Negate()
//{
//    _isNegative = !_isNegative;
//    return *this;
//}


//// -1 if |this|<|n|, 0 if |this|==|n|, 1 if |this|>|n|
//int BigFloat::CompareAbsolute(const BigFloat& n) const
//{
//    if (IsSpecial() || n.IsSpecial())
//    {
//        if (IsInf())
//            return n.IsInf() ? 0 : 1;
//        else if (n.IsInf())
//            return -1;
//        else if (IsZero())
//            return n.IsZero() ? 0 : -1;
//        else if (n.IsZero())
//            return 1;
//        else
//            return -1;
//    }
//    else if (_exponent > n._exponent)
//        return 1;
//    else if (_exponent < n._exponent)
//        return -1;
//    else // n._exponent == _exponent
//    {
//        u2 length = _length;
//        if (n._length < length)
//            length = n._length;
//        for (int iPos = 0;  iPos < length;  ++iPos)
//        {
//            u4 bd = n._d[iPos];
//            u4 ad = _d[iPos];
//            if (ad > bd)
//                return 1;
//            else if (ad < bd)
//                return -1;
//        }
//        if (_length > n._length)
//            return 1;
//        else if (_length < n._length)
//            return -1;
//    }
//    return 0;
//}

//// -1 if this<n, 0 if this==n, 1 if this>n
//int BigFloat::Compare(const BigFloat& n) const
//{
//    if (_isNegative != n._isNegative)
//        return _isNegative ? -1 : 1;
//    else if (_isNegative)
//        return -CompareAbsolute(n);
//    else
//        return CompareAbsolute(n);
//}


//BigFloat& BigFloat::Trunc()
//{
//    if (_exponent + 1 - _length >= 0)
//        ;
//    else if (_exponent < 0)
//        Zero();
//    else
//        _length = static_cast<u2>(_exponent + 1);

//    return *this;
//}

//// lots of operations want to round to c_digits digits of precision
//BigFloat& BigFloat::Round(bool carry, s8 previousDigit)
//{
//    s8 iPos;
//    bool round;

//    ASSERT(previousDigit == 0 || _length == c_digits);

//    if (-previousDigit > (s8)c_range/2)
//    {
//        iPos = c_digits-1;
//        round = true;
//        previousDigit += c_range;
//        while (round && iPos >= 0)
//        {
//            s8 sum = (u8)_d[iPos] - round;
//            round = (sum < 0);
//            if (round)
//                sum += c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
//        if (round)
//        {
//            ASSERT(carry);
//            carry = false;
//        }
//    }

//    if (carry)
//    {
//        bool round = false;
//        // need to shift all digits over by one and add a top digit
//        if (_length == c_digits)
//        {
//            // First, round bottom digit up if needed.
//            // Since the top digit carried already, it won't
//            // carry again.
//            iPos = c_digits-1;
//            round = (_d[iPos] > c_range/2 ||
//                     (_d[iPos] == c_range/2 && previousDigit >= 0));
//            iPos--;
//            while (round && iPos >= 0)
//            {
//                u8 sum = (u8)_d[iPos] + round;
//                round = (sum >= c_range);
//                if (round)
//                    sum -= c_range;
//                _d[iPos] = (u4)sum;
//                iPos--;
//            }
//        }
//        else
//        {
//            ++_length;
//        }
        
//        // now shift everyone over by one.
//        for (iPos = _length;  iPos-- > 1; )
//            _d[iPos] = _d[iPos-1];
        
//        // add the carry as the top new digit.
//        _d[0] = (u8)carry + (u8)round;
        
//        // increase the exponent by one.
//        if (_exponent == c_maxExponent)
//            return _isNegative ? NInf() : Inf();
//        ++_exponent;
//    }
//    else if (previousDigit >= (s8)c_range/2)
//    {
//        ASSERT(_length == c_digits);
        
//        // post-round only: 950+95 = 1000 not 1100 to 2 digits
//        round = true;
//        iPos = c_digits-1;
//        while (round && iPos >= 0)
//        {
//            u8 sum = (u8)_d[iPos] + round;
//            round = (sum >= c_range);
//            if (round)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
//        if (round)
//        {
//            _length = 1;
//            _d[0] = 1;
//            if (_exponent == c_maxExponent)
//            {
//                return _isNegative ? NInf() : Inf();
//            }
//            ++_exponent;
//        }
//    }
    
//    // remove trailing zeros
//    if (_length)
//    {
//        while (_d[_length-1] == 0)
//        {
//            if (--_length == 0)
//            {
//                Zero(_isNegative);
//            }
//        }
//    }
    
//    // final postchecks
//    if (_exponent < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (_exponent > c_maxExponent)
//    {
//        return Inf(_isNegative);
//    }

//    return *this;
//}


//// round up, no carry, 0 or positive previous digit
//BigFloat& BigFloat::Round(u8 previousDigit)
//{
//    s8 iPos;
//    bool round;

//    ASSERT(previousDigit == 0 || _length == c_digits);

//    if (previousDigit >= c_range/2)
//    {
//        ASSERT(_length == c_digits);
        
//        // post-round only: 950+95 = 1000 not 1100 to 2 digits
//        round = true;
//        iPos = c_digits-1;
//        while (round && iPos >= 0)
//        {
//            u8 sum = (u8)_d[iPos] + round;
//            round = (sum >= c_range);
//            if (round)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
//        if (round)
//        {
//            _length = 1;
//            _d[0] = 1;
//            if (_exponent == c_maxExponent)
//            {
//                return _isNegative ? NInf() : Inf();
//            }
//            ++_exponent;
//        }
//    }
    
//    // remove trailing zeros
//    while (_length && _d[_length-1] == 0)
//    {
//        if (--_length == 0)
//		{
//			Zero(_isNegative);
//        }
//    }

//    // final postchecks
//    if (_exponent < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (_exponent > c_maxExponent)
//    {
//        return Inf(_isNegative);
//    }

//    return *this;
//}


//// Ye gods, how could addition possibly be so complicated?
//BigFloat& BigFloat::AddOrSubtract(const BigFloat& n, bool minus)
//{
//    BigFloat m(*this);  // cache of this

//    if (IsSpecial() || n.IsSpecial())
//    {
//        BigFloat n2(n);
//        if (minus)
//            n2.Negate();

//        if (n2.IsZero())
//            return *this;
//        else if (IsZero())
//            return Copy(n2);
//        else if (IsNaN() || n.IsNaN())
//            return NaN();
//        else if (IsPInf())
//            return n2.IsNInf() ? NaN() : *this;
//        else if (IsNInf())
//            return n2.IsPInf() ? NaN() : *this;
//        else if (n2.IsPInf())
//            return PInf();
//        else if (n2.IsNInf())
//            return NInf();
//        else
//            ASSERT(false);
//    }
    
//    // make sure the absolute largest number of (a,b) is in a
//    const BigFloat* a = &m;
//    const BigFloat* b = &n;
//    bool signA = a->_isNegative;
//    bool signB = (b->_isNegative != minus);

//    // make sure a has the bigger absolute value
//    if (CompareAbsolute(n) < 0)
//    {
//        const BigFloat* temp = b;
//        b = a;
//        a = temp;
//        bool tempSign = signB;
//        signB = signA;
//        signA = tempSign;
//    }
//    bool add = (signA == signB);
//    _isNegative = signA;

//    // actually do the addition now
//    s8 delta = a->_exponent - b->_exponent;
//    if (delta > c_digits+1)
//    {
//        // b is too small to matter, just copy a
//        if (this != a)
//            Copy(*a);
//        _isNegative = signA;
//        return *this;
//    }
        
//    s8 previousDigit = 0;
    
//    _exponent = a->_exponent;
//    // do we need to carry one to the next digit?
//    bool carry = false;
    
//    // assume subtraction shifts the set of significant digits down by one?
//    bool shift = false;
    
//    // these digit positions are relative to the result assuming no carry
//    s8 iPos; // which digit position we are looking at right now
//    s8 leastA = a->_length; // least significant A position + 1
//    s8 leastB = b->_length + delta; // least signficant B position + 1
        
//    if (leastA > leastB)
//    {
//        iPos = leastA-1;
//        _length = static_cast<u2>(leastA);
        
//        // copy low-order digits from a
//        while (iPos >= leastB)
//        {
//            _d[iPos] = a->_d[iPos];
//            iPos--;
//        }
//    }
//    else
//    {
//        // assume we will have digits 1..digits not 0..digits-1.
//        if (!add && leastB > c_digits && a->_d[0]==1)
//        {
//            shift = true;
//            _exponent--;
//            leastB--;
//            leastA--;
//            delta--;
//        }
        
//        // copy low-order digits from b
//        s8 leastBUsed = leastB;
//        if (leastB > c_digits)
//        {
//            leastBUsed = c_digits;
//        }
        
//        iPos = leastBUsed-1;
//        _length = static_cast<u2>(leastBUsed);
        
//        if (iPos >= leastA) // either > or ==, we know not <
//        {
//            s8 stopB = delta > leastA ? delta : leastA;
//            if (add)
//            {
//                while (iPos >= stopB)
//                {
//                    _d[iPos] = b->_d[iPos-delta] + carry;
//                    if (carry && _d[iPos] != 0)
//                        carry = false;
//                    iPos--;
//                }
//            }
//            else
//            {
//                while (iPos >= stopB)
//                {
//					// the low-order digit is nonzero, and carry==false, so it is going to be in range.
//					// all the other digits might be zero, but they have carry==true, so they will also be in range.
//                    _d[iPos] = static_cast<u4>(c_range - b->_d[iPos-delta] - carry);
//                    carry = true;
//                    iPos--;
//                }
//            }
            
//            if (stopB > leastA)
//            {
//                // the biggest b is smaller than the smallest a
//                // fill in the digits in the gap
//                if (carry)
//                {
//                    if (add)
//                    {
//                        while (iPos >= leastA)
//                        {
//                            _d[iPos] = carry;
//                            carry = false;
//                            iPos--;
//                        }
//                    }
//                    else
//                    {
//                        while (iPos >= leastA)
//                        {
//                            _d[iPos] = c_range-1;
//                            iPos--;
//                        }
//                    }
//                }
//                else
//                {
//                    while (iPos >= leastA)
//                    {
//                        _d[iPos] = 0;
//                        iPos--;
//                    }
//                }
//            }
//        }
        
//        ASSERT(iPos == leastA-1);
//    }
        
    
//    if (add)
//    {
//        // do the sum
//        while (iPos-delta >= 0)
//        {
//            u8 sum = (u8)a->_d[iPos] + (u8)b->_d[iPos-delta] + carry;
//            carry = (sum >= c_range);
//            if (carry)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
        
//        // ran out of digits, finish copying the rest of the digits
//        while (iPos >= 0)
//        {
//            u8 sum = (u8)a->_d[iPos] + carry;
//            carry = (sum >= c_range);
//            if (carry)
//                sum -= c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }

//        if (leastB >= c_digits+1)
//        {
//            previousDigit = b->_d[c_digits-delta];
//        }
//    }
//    else  // subtract, not add
//    {
//        // do the subtraction
//        while (iPos-delta >= 0)
//        {
//            s8 sum = (u8)a->_d[iPos+shift] - (u8)b->_d[iPos-delta] - carry;
//            carry = (sum < 0);
//            if (carry)
//                sum += c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
        
//        // ran out of digits in b, finish the addition
//        while (iPos >= 0)
//        {
//            s8 sum = (u8)a->_d[iPos+shift] - carry;
//            carry = (sum < 0);
//            if (carry)
//                sum += c_range;
//            _d[iPos] = (u4)sum;
//            iPos--;
//        }
        
//        // remember what the first dropped digit would have been
//        if (leastB >= c_digits+1)
//        {
//            previousDigit = -(s8)b->_d[c_digits-delta];
//            if (leastB > c_digits+1)
//                --previousDigit;
//        }
        
//        if (shift)
//        {
//            // we pre-carried, convert lack of negative carry to
//            // a positive carry
//            carry = !carry;
//        }
//        else
//        {
//            // |a|>|b|, so there should be no way to get a final carry
//            ASSERT(!carry);
//        }
        
//        if (!carry)
//        {
//			u2 pos;
//            // find out how many leading digits are zero
//            for (pos = 0;  pos < _length; ++pos)
//                if (_d[pos] > 0)
//                    break;
//            u2 adjust = pos;
            
//            // adjust the exponent and promote remaining nonzero digits
//            if (adjust == _length || _exponent - adjust < -c_maxExponent)
//            {
//                return Zero(_isNegative);
//            }
//            else if (adjust > 0)
//            {
//                _exponent -= adjust;
//                _length -= adjust;
//                for (pos = 0;  pos < _length;  pos++)
//                    _d[pos] = _d[pos+adjust];
//            }
//        }
//    }

//    return Round(carry, previousDigit);
//}



//// Either do the multiplication, or raise an exception.
//BigFloat& BigFloat::Mult(const BigFloat& n)
//{
//    // actually exponent will be exponent or exponent+1
//    s8 exponent = _exponent + n._exponent + 1;
//    _isNegative ^= n._isNegative;

//    // do rough prechecks
//    if (IsSpecial() || n.IsSpecial())
//    {
//        if (IsNaN() || n.IsNaN())
//            return NaN();
//        else if (IsZero() || n.IsZero())
//            return Zero(_isNegative);
//        else
//            return Inf(_isNegative);
//    }
//    else if (exponent < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (exponent > c_maxExponent+1)
//    {
//        return Inf(_isNegative);
//    }
    
//    u8 length = _length + n._length;
//    if (length > c_digits)
//    {
//        length = c_digits+3;
//    }

//    u8 temp[2*c_digits];
//    for (int i=0; i<length; ++i)
//        temp[i] = 0;

//    // multiplications
//    for (int i=_length; i--; )
//    {
//        u8 result;
//        u8 start = n._length;
//        if (n._length > length - i)
//        {
//            start = length - i;
//                }
//        for (int j=start; j--; )
//        {
//            result = (u8)_d[i] * (u8)n._d[j];
//            temp[i + j + 1] += result & (c_range-1);
//            temp[i + j] += result >> c_log;
//        }
//    }

//    // carries
//    for (u8 i=length; i-- > 1; )
//    {
//        temp[i-1] += (temp[i] >> c_log);
//        temp[i] &= (c_range-1);
//    }
//    ASSERT(length > 0 && temp[0] < c_range);

//    u8* result = temp;
//    if (temp[0] == 0)
//    {
//        result = &temp[1];
//        --exponent;
//        --length;
//    }
//    u8 previousDigit = 0;
//    if (length > c_digits)
//    {
//        length = c_digits;
//        previousDigit = result[length];
//    }

//    // record the result
//    for (int i=0; i<length; ++i)
//        _d[i] = static_cast<u4>(result[i]);
//    _length = static_cast<u2>(length);
//    _exponent = exponent;
    
//    return Round(previousDigit);
//}


//BigFloat& BigFloat::Div(const BigFloat& n)
//{
//    // actually exponent will be exponent or exponent+1
//    s8 exponent = _exponent - n._exponent;

//    _isNegative = _isNegative ^ n._isNegative;

//    // do rough prechecks
//    if (IsSpecial() || n.IsSpecial())
//    {
//        if (IsNaN() || n.IsNaN() || n.IsZero())
//            return NaN();
//        else if (IsZero() || n.IsInf())
//            return Zero(_isNegative);
//        else
//            return Inf(_isNegative);
//    }
//    else if (exponent+1 < c_minExponent)
//    {
//        return Zero(_isNegative);
//    }
//    else if (exponent > c_maxExponent+1)
//    {
//        return Inf(_isNegative);
//    }

//    ASSERT((c_log/2)*2 == c_log);
//	ASSERT(_length > 0);

//    // approximate result to only to this many half-digits, then round
//	// need at least one full previous digit, and one full digit of zeros in front
//    static const u8 limit = 2*(c_digits+2);

//    // numerator
//	static const u8 numerLimit = 2 * limit + 2;
//    s8 t[numerLimit];
//    for (int i=0; i<_length; ++i)
//    {
//        t[2*i] = _d[i]>>(c_log/2);
//        t[2*i+1] = _d[i]&((((s8)1)<<(c_log/2))-1);
//    }
//    for (int i=2*_length; i<numerLimit; ++i)
//        t[i] = 0;

//    // denominator (overallocate; only 2*n._length of this will be used)
//    s8 d[limit];
//    s8 ad = n._d[0];
//    s8 length = 2*n._length;
//    s8 shift = (ad < (1<<(c_log/2))) ? 1 : 0;
//    if (shift)
//    {
//        --length;
//        d[0] = ad;
//        ad <<= c_log/2;
//        for (int i=2; i<2*n._length; i+=2)
//        {
//            d[i-1] = n._d[i/2] >> (c_log/2);
//            d[i] = n._d[i/2] & ((1<<(c_log/2))-1);
//        }
//        if (n._length > 1)
//        {
//            ad += d[1];
//        }
//    }
//    else
//    {
//        for (int i=0; i<2*n._length; i+=2)
//        {
//            d[i] = n._d[i/2] >> (c_log/2);
//            d[i+1] = n._d[i/2] & ((1<<(c_log/2))-1);
//        }
//    }

//    // divide
//    s8 r[limit];
//    s8 an = t[0];
//    for (int i=0; i<limit; ++i)
//    {
//        an <<= c_log/2;
//        an += t[i+1];
//        s8 q = an/ad;
//        if (q)
//        {
//            // first 2 digits of d are already accounted for by ad
//            if (2 < length)
//            {
//                t[i+2] -= q*d[2];
//                for (int j=length; j-- > 3; )
//                {
//                    s8 p = t[i+j] - q*d[j];
//                    ASSERT(p < (((s8)1)<<(3*c_log/2)) && p > -(((s8)1)<<(3*c_log/2)));
//                    t[i+j-1] += p >> (c_log/2);
//                    t[i+j] = p & ((1<<(c_log/2))-1);
//                }
//            }
//        }
//        ASSERT(ad*q + (an%ad) == an);
//        an %= ad;
        
//        r[i] = q;
//    }

//    // carry
//    for (int i=limit; i-- > 1;)
//    {
//        s8 carry = r[i] >> (c_log/2);
//        if (carry != 0)
//        {
//            r[i] &= (1<<(c_log/2)) - 1;
//            r[i-1] += carry;
//        }
//    }
//    ASSERT((r[0] >> (c_log/2)) == 0);

//    s8* pr = r;
//    while (pr[0] == 0)
//    {
//        ++pr;
//        --shift;
//    }

//    // combine the half-digits and round
//    s8 previousDigit = 0;
//    if (!(shift & 1))
//    {
//        _d[0] = static_cast<u4>(pr[0]);
//        for (int i=1; i<c_digits; ++i)
//        {
//            _d[i] = static_cast<u4>((pr[2*i-1] << (c_log/2)) + pr[2*i]);
//        }
//        previousDigit = (pr[2*c_digits-1] << (c_log/2)) + pr[2*c_digits];
//    }
//    else
//    {
//        for (int i=0; i<c_digits; ++i)
//        {
//            _d[i] = static_cast<u4>((pr[2*i] << (c_log/2)) + pr[2*i+1]);
//        }
//        previousDigit = (pr[2*c_digits] << (c_log/2)) + pr[2*c_digits+1];
//    }
//    _length = c_digits;
//    _exponent = exponent + (shift>>1);
//    return Round(previousDigit);
//}


//int BigFloat::Compare(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Compare(b);
//}


//BigFloat& BigFloat::Add(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Add(b);
//}


//BigFloat& BigFloat::Sub(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Sub(b);
//}


//BigFloat& BigFloat::Mult(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Mult(b);
//}


//BigFloat& BigFloat::Div(s8 n, s8 exponent)
//{
//    BigFloat b(n, exponent);
//    return Div(b);
//}


//BigFloat& BigFloat::Invert()
//{
//    BigFloat i(1);
//    i.Div(*this);
//    return Copy(i);
//}


//BigFloat& BigFloat::Sqrt()
//{
//    if (_isNegative)
//    {
//        return NaN();
//    }
//    else if (IsSpecial())
//    {
//        return *this;
//    }

//    static const int c_intermediate = 2*c_digits+2;
//    static const int c_remainder = 2*c_intermediate;

//    // remainder stores the original value in half-digits
//    s8 r[c_remainder];

//    // answer is also in half-digits
//    s8 abuf[c_intermediate+1];
//    bool odd = (_exponent & 1);

//    s8* a = abuf;
//    if (!odd)
//    {
//        abuf[0] = 0;
//        a = &abuf[1];
//    }
    
//    for (int i=0; i<_length; ++i)
//    {
//        r[2*i] = _d[i]>>(c_log/2);
//        r[2*i+1] = _d[i] & ((1<<(c_log/2))-1);
//    }
//    for (int i=_length*2; i<c_remainder; ++i)
//        r[i] = 0;
//    for (int i=0; i<c_intermediate; ++i)
//        a[i] = 0;

//    // Find the first two half-digits of the root using integer arithmetic.
//    //
//    // This method sometimes gives an answer one too high, which is not
//    // important usually, but would cause the answer to not fit in two
//    // half-digits in the case of 1<<(2*c_log)-1.  I chose the starting
//    // root so that that doesn't happen.
//    u8 first =
//        (((u8)r[0])<<(3*c_log/2)) +
//        (((u8)r[1])<<(2*c_log/2)) +
//        (((u8)r[2])<<(1*c_log/2)) +
//        ((u8)r[3]);
//    r[0] = 0;
//    r[1] = 0;
//    r[2] = 0;
//    r[3] = 0;
//    u8 root = ((u8)1) << c_log;
//    for (;;)
//    {
//        u8 oldroot = root;
//        root = (root + (first/root)) / 2;
//        if (oldroot == root || oldroot == root+1)
//            break;
//    }
//    a[0] = root >> (c_log/2);
//    ASSERT(a[0] >= 1);
//    ASSERT(a[0] < (1<<(c_log/2)));
//    a[1] = root & ((1<<(c_log/2))-1);

//    u8 square = root*root;
//    s8 remainder = (s8)(first - root*root);
//    ASSERT(remainder < ((s8)2)<<c_log);
//    ASSERT(remainder > ((s8)-2)<<c_log);
//    s8 denominator = -2*(s8)root;
//    for (int i=2; i<c_intermediate; i++)
//    {
//        // new approximate remainder
//        remainder <<= c_log/2;
//        remainder += r[i+2];
//        r[i+2] = 0;

//        // new approximate delta
//        s8 delta = remainder/denominator;

//        // adjust the remainder
//        remainder -= denominator * delta;
//        ASSERT(remainder < ((s8)2)<<c_log);
//        ASSERT(remainder > ((s8)-2)<<c_log);
//        ASSERT(r[i] == 0);

//        if (2*i+1 < c_intermediate)
//        {
//            square = delta*delta;
//            r[2*i+1] -= square;
//        }
//        int j = (2*i+1 >= c_intermediate) ? c_intermediate-i : i;
//        if (j > 2)
//        {
//            while (j-- > 3)
//            {
//                s8 p = r[i+j+1] + 2 * a[j] * delta;
//                r[i+j+1] = p & ((1<<(c_log/2))-1);
//                r[i+j] += p >> (c_log/2);
//            }
//            r[i+j+1] += 2 * a[j] * delta;
//        }

//        a[i] = -delta;
//    }

//    // carries, make all the digits between 0 and 1<<(c_log/2)
//    for (int i=c_intermediate; i-- > 1; )
//    {
//        s8 top = a[i] >> c_log/2;
//        a[i-1] += top;
//        a[i] -= top << c_log/2;
//    }

//    // combine the half digits into the final answer
//    _exponent >>= 1;
//    _length = c_digits;
//    s8 previousDigit = (a[2*c_digits] << (c_log/2)) + a[2*c_digits+1];
//	s8 x = (abuf[0] << (c_log/2)) + abuf[1];
//    ASSERT(x >= 1);
//    ASSERT(x < (((s8)1)<<c_log));
//	_d[0] = static_cast<u4>(x);
//    for (int i=1; i<c_digits; ++i)
//    {
//        x = (abuf[2*i] << c_log/2) + abuf[2*i+1];
//        ASSERT(x >= 0);
//        ASSERT(x < (((s8)1)<<c_log));
//		_d[i] = static_cast<u4>(x);
//    }
//    return Round(previousDigit);
//}


//const BigFloat& BigFloat::Pi()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    return BigFloatCache::_pi;
//}


//const BigFloat& BigFloat::E()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    return BigFloatCache::_e;
//}


//const BigFloat& BigFloat::ConstZero()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    return BigFloatCache::_zero;
//}


//// given x in [-pi/4, pi/4], replace it with sin(x)
//BigFloat& BigFloat::PartialSin()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);

//    BigFloat x6(x4);
//    x6.Mult(x2);

//    BigFloat x8(x6);
//    x8.Mult(x2);

//    BigFloat sin(0);
//    BigFloat oldSin(0);
//    BigFloat power(*this);
//    s8 i = 7;
//    ASSERT(BigFloatCache::_overFactLen < (1<<(63/6)));
//    for (; i<BigFloatCache::_overFactLen; i+=8)
//    {
//        BigFloat sum(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5));

//        BigFloat term(x2);
//        term.Mult(i*(i-1)*(i-2)*(i-3));
//        sum.Sub(term);

//        term.Copy(x4);
//        term.Mult(i*(i-1));
//        sum.Add(term);

//        sum.Sub(x6);
            

//        sum.Mult(BigFloatCache::_overFact[i]);
//        sum.Mult(power);
//        sin.Add(sum);

//        if (sin.Compare(oldSin) == 0)
//            break;

//        oldSin.Copy(sin);
//        power.Mult(x8);
//    }

//    ASSERT(i < BigFloatCache::_overFactLen);
//    return Copy(sin);
//}



//// given x in [-pi/4, pi/4], replace it with cos(x)
//BigFloat& BigFloat::PartialCos()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);

//    BigFloat x6(x4);
//    x6.Mult(x2);

//    BigFloat x8(x6);
//    x8.Mult(x2);

//    BigFloat cos(0);
//    BigFloat oldCos(0);
//    BigFloat power(1);
//    s8 i = 6;
//    ASSERT(BigFloatCache::_overFactLen < (1<<(63/6)));
//    for (; i<BigFloatCache::_overFactLen; i+=8)
//    {
//        BigFloat sum(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5));

//        BigFloat term(x2);
//        term.Mult(i*(i-1)*(i-2)*(i-3));
//        sum.Sub(term);

//        term.Copy(x4);
//        term.Mult(i*(i-1));
//        sum.Add(term);

//        sum.Sub(x6);
            
//        sum.Mult(BigFloatCache::_overFact[i]);
//        sum.Mult(power);
//        cos.Add(sum);

//        if (cos.Compare(oldCos) == 0)
//            break;

//        oldCos.Copy(cos);
//        power.Mult(x8);
//    }

//    ASSERT(i < BigFloatCache::_overFactLen);
//    return Copy(cos);
//}


//// Lots of functions would prefer inputs only in [-pi/4, pi/4).
//s8 BigFloat::Quadrant()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    BigFloat div2pi(*this);

//    // modulo 2pi, don't care about the rest
//    div2pi.Mult(BigFloatCache::_overTwoPi);
//    BigFloat extra(div2pi);
//    extra.Trunc();
//    div2pi.Sub(extra);
//    extra.Mult(BigFloatCache::_twoPi);
//    Sub(extra);

//    div2pi.Mult(8);
//    div2pi.Trunc();
//    s8 quadrant = div2pi.ToInteger();
//    switch (quadrant)
//    {
//    case 7:
//        Sub(BigFloatCache::_twoPi);
//        break;
//    case 6:
//    case 5:
//        Sub(BigFloatCache::_threePiOverTwo);
//        break;
//    case 4:
//    case 3:
//        Sub(BigFloatCache::_pi);
//        break;
//    case 2:
//    case 1:
//        Sub(BigFloatCache::_piOverTwo);
//        break;
//    case 0:
//        if (_isNegative)
//            quadrant += 7;
//        break;
//    case -1:
//    case -2:
//        quadrant += 7;
//        Add(BigFloatCache::_piOverTwo);
//        break;
//    case -3:
//    case -4:
//        quadrant += 7;
//        Add(BigFloatCache::_pi);
//        break;
//    case -5:
//    case -6:
//        quadrant += 7;
//        Add(BigFloatCache::_threePiOverTwo);
//        break;
//    case -7:
//        quadrant += 7;
//        Add(BigFloatCache::_twoPi);
//        break;
//    default:
//        ASSERT(false, "bad quadrant %d", quadrant);
//    }

//    BigFloat limit(3217);
//    limit.Div(4096);    // limit is a little bigger than pi/4, there are sometimes rounding issues
//    ASSERT(CompareAbsolute(limit) <= 0);
//    return quadrant;
//}


//BigFloat& BigFloat::Sin()
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return Copy(0);
//        else
//            return NaN();
//    }

//    s8 quadrant = Quadrant();
//    switch (quadrant)
//    {
//    case 7:
//    case 0:
//        PartialSin();
//        break;
//    case 1:
//    case 2:
//        PartialCos();
//        break;
//    case 3:
//    case 4:
//        PartialSin();
//        Negate();
//        break;
//    case 5:
//    case 6:
//        PartialCos();
//        Negate();
//        break;
//    }
//    return *this;
//}


//BigFloat& BigFloat::Csc()
//{
//    Sin();
//    Invert();
//    return *this;
//}


//BigFloat& BigFloat::Cos()
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return Copy(1);
//        else
//            return NaN();
//    }
    
//    s8 quadrant = Quadrant();
//    switch (quadrant)
//    {
//    case 7:
//    case 0:
//        PartialCos();
//        break;
//    case 1:
//    case 2:
//        PartialSin();
//        Negate();
//        break;
//    case 3:
//    case 4:
//        PartialCos();
//        Negate();
//        break;
//    case 5:
//    case 6:
//        PartialSin();
//        break;
//    }
//    return *this;
//}


//BigFloat& BigFloat::Sec()
//{
//    Cos();
//    Invert();
//    return *this;
//}


//BigFloat& BigFloat::Tan()
//{
//    if (IsSpecial())
//    {
//        if (IsZero())
//            return Copy(0);
//        else
//            return NaN();
//    }

//    s8 quadrant = Quadrant();
//    BigFloat sin2;
//    BigFloat cos;
//    BigFloat cos2;
//    BigFloat sin;
//    bool negate = false;
//    switch (quadrant)
//    {
//    case 7:
//    case 0:
//    case 3:
//    case 4:
//        // sine is more accurate, derive cosine
//        PartialSin();
//        sin2.Copy(*this);
//        sin2.Mult(sin2);
//        cos.Copy(1);
//        cos.Sub(sin2);
//        cos.Sqrt();
//        Div(cos);
//        break;
//    case 2:
//    case 6:
//        negate = true;
//    case 1:
//    case 5:
//        // cosine is more accurate, derive sine
//        PartialCos();
//        cos2.Copy(*this);
//        cos2.Mult(cos2);
//        sin.Copy(1);
//        sin.Sub(cos2);
//        sin.Sqrt();
//        if (negate)
//            sin._isNegative = !sin._isNegative;
//        Div(sin);
//        break;
//    }
//    return *this;
//}


//BigFloat& BigFloat::Exp()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
//    if (IsSpecial())
//    {
//        if (IsNaN())
//            return *this;
//        else if (IsZero())
//            return Copy(1);
//        else if (IsPInf())
//            return *this;
//        else if (IsNInf())
//            return Zero();
//    }

//    // only calculate e on [-1/16, 1/16].
//    // For the rest use multiples of e^^-16.
//    BigFloat whole(*this);
//    whole.Mult(1<<(-BigFloatCache::_ePowerNeg-1));
//    whole.Trunc();
//    whole.Div(1<<(-BigFloatCache::_ePowerNeg-1));
//    Sub(whole);

//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);

//    BigFloat x8(x4);
//    x8.Mult(x4);

//    // sum the series (8 terms at a time, to reduce bignum multiplications)
//    // calculating only the fractional part mod 1/16
//    BigFloat exp(1);
//    BigFloat oldExp(1);
//    BigFloat power(*this);
//    for (s8 i=8; i<BigFloatCache::_overFactLen; i+=8)
//    {
//        BigFloat even(i*(i-1)*(i-2)*(i-3)*(i-4));

//        BigFloat term(i);
//        term.Mult(x4);
//        even.Add(term);
//        even.Mult(x2);

//        even.Add(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5)*(i-6));

//        term.Copy(i*(i-1)*(i-2));
//        term.Mult(x4);
//        even.Add(term);

//        BigFloat odd(i*(i-1)*(i-2)*(i-3));

//        term.Copy(1);
//        term.Mult(x4);
//        odd.Add(term);
//        odd.Mult(x2);
        
//        odd.Add(i*(i-1)*(i-2)*(i-3)*(i-4)*(i-5));

//        term.Copy(i*(i-1));
//        term.Mult(x4);
//        odd.Add(term);

//        odd.Mult(*this);

//        odd.Add(even);
//        odd.Mult(BigFloatCache::_overFact[i]);
//        odd.Mult(power);

//        exp.Add(odd);

//        if (exp.Compare(oldExp) == 0)
//            break;

//        oldExp.Copy(exp);
//        power.Mult(x8);
        
//    }
 
//    // now handle the whole number multiple of 1/16
//    if (!whole._isNegative)
//    {
//        for (s8 i=0; i<whole._length; ++i)
//        {
//            for (s8 j=0; j<c_log; ++j)
//            {
//                if (whole._d[i] & (1<<j))
//                {
//                    s8 index = c_log*(whole._exponent-i)+j;
//                    if (index < BigFloatCache::_ePowerLen)
//                    {
//                        ASSERT(index > BigFloatCache::_ePowerNeg);
//                        exp.Mult(BigFloatCache::_ePower[index]);
//                    }
//                    else
//                        exp.Inf();
//                }
//            }
//        }
//    }
//    else if (whole._isNegative)
//    {
//        for (s8 i=0; i<whole._length; ++i)
//        {
//            for (s8 j=0; j<c_log; ++j)
//            {
//                if (whole._d[i] & (1<<j))
//                {
//                    s8 index = c_log*(whole._exponent-i)+j;
//                    if (index < BigFloatCache::_ePowerLen)
//                    {
//                        ASSERT(index > BigFloatCache::_ePowerNeg);
//                        exp.Mult(BigFloatCache::_eInvPower[index]);
//                    }
//                    else
//                        exp.Zero();
//                }
//            }
//        }
//    }
    
//    return Copy(exp);
//}


//BigFloat& BigFloat::ASin()
//{
//    BigFloat denom(*this);
//    denom.Mult(denom);
//    denom.Sub(1);
//    denom.Negate();
//    denom.Sqrt();
//    denom.Add(1);
//    Div(denom);
//    ATan();
//    return Mult(2);
//}


//BigFloat& BigFloat::ACos()
//{
//    BigFloat num(*this);
//    num.Mult(num);
//    num.Sub(1);
//    num.Negate();
//    num.Sqrt();
//    BigFloat denom(*this);
//    denom.Add(1);
//    num.Div(denom);
//    num.ATan();
//    num.Mult(2);
//    return Copy(num);
//}


//BigFloat& BigFloat::ATan()
//{
//    if (IsNaN())
//        return *this;

//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();

//    bool flip = (CompareAbsolute(1) > 0);
//    if (flip)
//    {
//        Invert();
//    }

//    bool reduce = false;

//    // tan(x) = 2x/(1+sqrt(1+xx))
//    static const int c_iter = 2;
//    for (int i=0; i<c_iter; ++i)
//    {
//        BigFloat denom(*this);
//        denom.Mult(denom);
//        denom.Add(1);
//        denom.Sqrt();
//        denom.Add(1);
//        Div(denom);
//    }

//    BigFloat x2(*this);
//    x2.Mult(*this);

//    BigFloat x4(x2);
//    x4.Mult(x2);
    
//    BigFloat aTan(0);
//    BigFloat oldATan(0);
//    BigFloat power(*this);
//    for (s8 i=1; ; i+=4)
//    {
//        BigFloat term(i);
//        term.Mult(x2);
//        term.Sub(i+2);
//        term.Mult(power);
//        term.Div(i*(i+2));
//        aTan.Sub(term);

//        if (aTan.Compare(oldATan) == 0)
//            break;

//        oldATan.Copy(aTan);
//        power.Mult(x4);
//    }
//    Copy(aTan);

//    // multiply the result by 1<<c_iter to make up for all those half-angle reductions earlier
//    Mult(1<<c_iter);
//    if (flip)
//    {
//        Negate();
//        if (_isNegative)
//        {
//            Sub(BigFloatCache::_piOverTwo);
//        }
//        else
//        {
//            Add(BigFloatCache::_piOverTwo);
//        }
//    }

//    return *this;
//}


//BigFloat& BigFloat::Ln()
//{
//    if (!BigFloatCache::_isInitialized)
//        BigFloatCache::Init();
    
//    if (_isNegative)
//        return NaN();
//    else if (IsSpecial())
//    {
//        if (IsZero())
//            return NInf();
//        else if (IsInf())
//            return Inf();
//        else
//            return NaN();
//    }

//    // get a remainder in [1-delta, delta] for some small delta
//    BigFloat whole;
//    static const s8 minDigit = BigFloatCache::_ePowerNeg/c_log - 2;
//    static const s8 offset = minDigit * c_log;  // offset to keep i positive
//    if (_exponent >= 0)
//    {
//        s8 i=BigFloatCache::_ePowerNeg+1;
//        while (i < BigFloatCache::_ePowerLen && Compare(BigFloatCache::_ePower[i]) >= 0)
//            ++i;
//        whole._exponent = minDigit + (i-1-offset)/c_log;
//        whole._length = (u2)(whole._exponent - minDigit);
//        if (whole._length > c_digits)
//            whole._length = c_digits;
//        for (int j=0; j<whole._length; ++j)
//            whole._d[j] = 0;
//        while (i-- > BigFloatCache::_ePowerNeg+1)
//        {
//            if (Compare(BigFloatCache::_ePower[i]) >= 0)
//            {
//                Mult(BigFloatCache::_eInvPower[i]);
//                whole._d[whole._exponent - (i-offset)/c_log - minDigit] += (1 << ((i+offset)%c_log));
//            }
//        }
//    }
//    else
//    {
//        whole._isNegative = true;
//        s8 i=BigFloatCache::_ePowerNeg+1;
//        while (i < BigFloatCache::_ePowerLen && Compare(BigFloatCache::_eInvPower[i]) <= 0)
//            ++i;
//        whole._exponent = minDigit + (i-1-offset)/c_log;
//		s8 len = whole._exponent - minDigit;
//		ASSERT(len >= 0);
//		ASSERT(len < (u2)~0);
//        whole._length = static_cast<u2>(len);
//        if (whole._length > c_digits)
//            whole._length = c_digits;
//        for (int j=0; j<whole._length; ++j)
//            whole._d[j] = 0;
        
//        while (i-- > BigFloatCache::_ePowerNeg+1)
//        {
//            if (Compare(BigFloatCache::_eInvPower[i]) <= 0)
//            {
//                Mult(BigFloatCache::_ePower[i]);
//                whole._d[whole._exponent - (i-offset)/c_log - minDigit] += (1 << ((i+offset)%c_log));
//            }
//        }
//    }

//    // ok here goes
//    BigFloat x(*this);
//    x.Sub(1);
//    BigFloat x2(x);
//    x2.Mult(x);
//    BigFloat power(x);
//    BigFloat oldLn(1);
//    BigFloat ln(0);
//    for (s8 i=1; ; i+= 2)
//    {
//        BigFloat term(i);
//        term.Mult(x);
//        term.Sub(i+1);
//        term.Div(i*(i+1));
//        term.Mult(power);
//        ln.Sub(term);
//        if (oldLn.Compare(ln) == 0)
//            break;

//        oldLn.Copy(ln);
//        power.Mult(x2);
//    }
    
//    // add in whole
//    ln.Add(whole);
    
//    return Copy(ln);
//}


//BigFloat& BigFloat::Power(const BigFloat& n)
//{
//    Ln();
//    Mult(n);
//    return Exp();
//}


//BigFloat& BigFloat::Log(const BigFloat& n)
//{
//    BigFloat lnn(n);
//    lnn.Ln();

//    Ln();
//    Invert();
    
//    return Mult(lnn);
//}


//s8 BigFloat::RoundInteger(s8 value)
//{
//    u8 x;

//    // find if it is negative
//    bool negative = (value < 0);

//    x = (u8)(negative ? -value : value);

//    // find the exponent
//    int exp=0;
//    for (; exp<64; exp+=c_log)
//    {
//        if (x < ((u8)c_range)<<exp)
//            break;
//    }

//    // round to the nearest legal int
//    u8 precision = c_log*(c_digits-1);
//    if (exp > precision)
//    {
//        // add 1/2 so it rounds right
//        x += ((u8)1)<<(exp - precision - 1);

//        // truncate x to the legal precision
//        x >>= (exp - precision);
//        x <<= (exp - precision);
//    }

//    return negative ? -(s8)x : (s8)x;
//}


//void BigFloat::TestInteger(const BigFloat& n, s8 value)
//{
//    s8 x = RoundInteger(value);
//    ASSERT(n.ToInteger() == x, "n=%d, x=%d", (s4)n.ToInteger(), (s4)x);
//}


//void BigFloat::TestAdd(s8 x, s8 y)
//{
//    BigFloat bx(x);
//    BigFloat by(y);
//    BigFloat bz(bx);

//    TestInteger(bz.Add(by), x+y);
//    TestInteger(bz.Copy(bx).Sub(by), x-y);
//}


//void BigFloat::TestMult(s8 x, s8 ex, s8 y, s8 ey)
//{
//    s8 px = x<0 ? -x : x;
//    s8 py = y<0 ? -y : y;
//    ASSERT(px < (((s8)1)<<31));
//    ASSERT(py < (((s8)1)<<31));
//    s8 pz = RoundInteger(px*py);

//    BigFloat bx(x, ex);
//    BigFloat by(y, ey);
//    BigFloat bz(bx);

//    if (pz == 0)
//    {
//        bz.Mult(by);
//        ASSERT(bz.ToInteger() == 0);
//        return;
//    }

//    int i;
//    for (i=0; pz < (((u8)1)<<(64-c_log)) && i<64; ++i)
//    {
//        pz <<= c_log;
//    }
//    s8 exponent = ex+ey+(64/c_log - i - 1);

//    if (exponent > c_maxExponent)
//        return;  // doing the multiplication would have raised an exception

//    bz.Mult(by);
//    if (exponent < c_minExponent)
//    {
//        ASSERT(bz.CompareAbsolute(0) == 0);
//    }
//    else
//    {
//        ASSERT(bz.ToDigits() == pz);
//        ASSERT(bz.ToExponent() == exponent);
//        ASSERT(bz.IsNegative() == ((x<0) != (y<0)));
//    }
//}

//void BigFloat::TestInverse(s8 x, s8 ex)
//{
//    BigFloat bx(x, ex);

//    BigFloat bi(bx);
//    bi.Invert();

//    BigFloat bq(bi);
//    bq.Mult(bx);
//    if (bx.ToExponent() >= -c_maxExponent && bi.IsZero())
//        return;

//    bq.Sub(1);
//    ASSERT(bq.Compare(0) == 0 ||
//           bq.ToExponent() <= 1-c_digits);
//}


//void BigFloat::TestSqrt(s8 x, s8 ex)
//{
//    BigFloat bx(x, ex);

//    BigFloat br(bx);  // root
//    br.Sqrt();

//    // check (bx-br*br)/bx
//    BigFloat bd(br);  // delta
//    bd.Mult(bd);
//    bd.Sub(bx);
//    BigFloat bo(bx);  // ratio of delta to original
//    bo.Invert();
//    bo.Mult(bd);
//    ASSERT(bo.Compare(0) == 0 ||
//           bo.ToExponent() <= 1-c_digits);


//    // check (br-bx/br)/br
//    // this tests division in interesting ways too
//    bd.Copy(bx);  // delta
//    bd.Div(br);
//    bd.Sub(br);
//    bo.Copy(br);  // ratio of delta to original
//    bo.Invert();
//    bo.Mult(bd);
//    ASSERT(bo.Compare(0) == 0 ||
//           bo.ToExponent() <= 1-c_digits);
//}


//#ifdef BIGFLOAT_TEST
//void BigFloat::UnitTest()
//{
//    BigFloat b;
//    BigFloat zero;
//    ASSERT(b.Compare(zero) == 0);

//    // b is 1
//    b.FromInteger(1);
//    ASSERT(b.Compare(zero) == 1);
//    ASSERT(b.Compare(b) == 0);

//    // b is -1
//    b.FromInteger(-1);
//    ASSERT(b.Compare(zero) == -1);
//    ASSERT(b.Compare(b) == 0);
//    BigFloat c;

//    // c is -c_range
//    c.FromInteger(-(s8)c_range);
//    TestInteger(c, -(s8)c_range);

//    TestSqrt(2, 0);

//    static const u8 c_maxInt = ((u8)1) << (c_log * c_digits);

//    // check if inverse works
//    for (s8 ex=(s8)(c_minExponent-c_digits+1);
//         ex<=(s8)(c_maxExponent-c_digits+1);
//         ++ex)
//    {
//        for (s8 x=(((s8)1)<<(c_log*(c_digits-1))); x<c_maxInt; x++)
//        {
//            TestInverse(x, ex);
//            TestInverse(-x, ex);
//        }
//    }
//    printf("Inverse works\n");


//    // check if square root works
//    for (s8 ex = (s8)(c_minExponent-c_digits+1);
//         ex < (s8)(c_maxExponent-c_digits+1);
//         ++ex)
//    {
//        for (s8 x=(((s8)1)<<(c_log*(c_digits-1))); x<c_maxInt; x++)
//        {
//            TestSqrt(x, ex);
//        }
//    }
//    printf("Square root works\n");

//    // check if addition and subtraction work
//    u4 coef = 1;
//    for (int q=0; q<2*c_digits; ++q)
//    {
//        for (s8 x=0; x<coef*c_maxInt; x += coef)
//        {
//            for (s8 y=0; y<c_maxInt; ++y)
//            {
//                TestAdd(x,y);
//                TestAdd(x,-y);
//                TestAdd(-x,y);
//                TestAdd(-x,-y);
//            }
//        }
//        coef *= c_range;
//    }
//    printf("Addition and Subtraction work\n");

//    // check if multiplication works
//    TestMult(0, 0, 1, 0);
//    TestMult(-1, 0, 0, 0);
//    for (s8 ex=(s8)(c_minExponent-c_digits+1);
//         ex<=(s8)(c_maxExponent-c_digits+1);
//         ++ex)
//    {
//        for (s8 x=(((s8)1)<<(c_log*(c_digits-1))); x<c_maxInt; x++)
//        {
//            for (s8 ey=(s8)(c_minExponent-c_digits+1);
//                 ey<=(s8)(c_maxExponent-c_digits+1);
//                 ++ey)
//            {
//                for (s8 y=(((s8)1)<<(c_log*(c_digits-1))); y<c_maxInt; y++)
//                {
//                    TestMult(x, ex, y, ey);
//                    TestMult(x, ex, -y, ey);
//                    TestMult(-x, ex, y, ey);
//                    TestMult(-x, ex, -y, ey);
//                }
//            }
//        }
//    }
//    printf("Multiplication works\n");
//}

//int main(int argc, const char **argv)
//{
//    BigFloat* m[2];
//    m[0] = new BigFloat[3];
//    m[1] = new BigFloat[3];
//    m[0][1].Copy(1);
//    m[1][0].Copy(1);
//    m[1][1].Copy(1);
//    m[0][2].Copy(1);
//    m[1][2].Copy(2);
//    BigFloat::GaussianElimination(m, 2, 2);
//    printf("%d, %d\n", (s4)m[0][2].ToInteger(), (s4)m[1][2].ToInteger());


//    printf("%f\n", BigFloat::Pi().ToDouble());
    
//    try
//    {
//        BigFloat::UnitTest();
//    }
//    catch( const std::exception & ex )
//    {
//        fprintf(stderr, "%s\n", ex.what());
//    }

//    return 0;
//}
//#endif

/*
 * File: biginteger.cpp
 * --------------------
 * This file implements members of a class for arbitrary-size integer arithmetic.
 * It is meant to help get around the max/min value limit for types
 * such as int and long.
 * See biginteger.h for declarations and documentation of each member.
 *
 * @version 2017/11/05
 * - fixed compiler error on some older clang versions about string insert call
 * @version 2017/10/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "biginteger.h"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

static int STRING_SIZE_MAX = 10000;   // for strings

const BigInteger BigInteger::NEGATIVE_ONE("-1");
const BigInteger BigInteger::ZERO("0");
const BigInteger BigInteger::ONE("1");
const BigInteger BigInteger::TWO("2");
const BigInteger BigInteger::TEN("10");
const BigInteger BigInteger::MAX_INT("2147483647");
const BigInteger BigInteger::MIN_INT("-2147483648");
const BigInteger BigInteger::MAX_UINT("4294967295");
const BigInteger BigInteger::MAX_LONG("9223372036854775807");
const BigInteger BigInteger::MIN_LONG("-9223372036854775808");
const BigInteger BigInteger::MAX_ULONG("18446744073709551615");
const BigInteger BigInteger::MAX_SHORT("32767");
const BigInteger BigInteger::MIN_SHORT("-32768");
const BigInteger BigInteger::MAX_USHORT("65535");

BigInteger::BigInteger()
    : number("0"),
      sign(false) {
    // empty
}

BigInteger::BigInteger(const BigInteger& other)
    : number(other.number),
      sign(other.sign) {
    // empty
}

BigInteger::BigInteger(const std::string& s, int radix) {
    setValue(s, radix);
}

BigInteger::BigInteger(const std::string& s, bool sin) {
    setValue(s);
    setSign(sin);
}

BigInteger::BigInteger(long n) {
    setValue(toString(n));
}

BigInteger BigInteger::abs() const {
    return BigInteger(getNumber());
}

std::string BigInteger::add(const std::string& n1, const std::string& n2) {
    // sum variable to store result;
    // left-pad numbers with 0s to be the same length as needed
    int digits = (int) (n1.length() > n2.length() ? n1.length() : n2.length());
    std::string sum = padLeft("", digits);
    std::string n1copy = padLeft(n1, digits);
    std::string n2copy = padLeft(n2, digits);

    // add each digit pair
    int carry = 0;
    for (int i = n1copy.length() - 1; i >= 0; --i) {
        sum[i] = (carry + (n1copy[i] - '0') + (n2copy[i] - '0')) + '0';
        if (i != 0) {
            if (sum[i] > '9') {
                sum[i] -= 10;
                carry = 1;
            } else {
                carry = 0;
            }
        }
    }

    if (sum[0] > '9') {
        // top digit carried over; make an additional digit at front
        sum[0] -= 10;
        sum.insert(0, (size_t) 1, '1');
    }

    return sum;
}

void BigInteger::checkStringIsNumeric(const std::string& s, int radix) {
    std::string scopy = stripNumberPrefix(s, radix);

    // first char can optionally be + or - sign
    if (scopy.empty()) {
        error("Empty string cannot be converted into an integer");
    }

    int start = 0;
    if (scopy[0] == '+' || scopy[0] == '-') {
        start++;
    }
    for (int i = start, len = (int) scopy.length(); i < len; i++) {
        char ch = tolower(scopy[i]);
        bool good = false;
        if (radix == 1) {
            good = ch == '1';
        } else if (radix <= 10) {
            good = ch >= '0' && ch < '0' + radix;
        } else if (radix <= 16) {
            good = isdigit(ch) || (ch >= 'a' && ch < radix - 10 + 'a');
        } else {
            // don't know how to verify other bases very well
            good = isalnum(ch);
        }

        if (!good) {
            error("Non-numeric string passed: \"" + scopy + "\"");
        }
    }
}

std::pair<std::string, long> BigInteger::divide(const std::string& n, long den) {
    if (den == 0) {
        error("Cannot divide by zero");
    }

    long rem = 0;
    std::string result;
    result.resize(STRING_SIZE_MAX);

    for (int indx = 0, len = (int) n.length(); indx < len; ++indx) {
        rem = (rem * 10) + (n[indx] - '0');
        result[indx] = rem / den + '0';
        rem %= den;
    }

    result.resize(n.length());
    result = removeLeadingZeros(result);

    return make_pair(result, rem);
}

// Returns (quotient, remainder) as a pair 2-tuple.
// Implementation note: This is a stupid implementation.
// It is quite slow and there are much better algorithms.  But it works.
// see also:
// https://en.wikipedia.org/wiki/Division_algorithm#Division_by_repeated_subtraction
// pre: b2 != 0
std::pair<BigInteger, BigInteger> BigInteger::divideBig(const BigInteger& numerator, const BigInteger& denominator) {
    if (denominator == ZERO) {
        error("Cannot divide by zero");
    } else if (denominator.isNegative()) {
        std::pair<BigInteger, BigInteger> result = divideBig(numerator, -denominator);
        result.second = -result.second;
        return result;
    } else if (numerator.isNegative()) {
        std::pair<BigInteger, BigInteger> result = divideBig(-numerator, denominator);
        if (result.second == ZERO) {
            result.first = -result.first;
        } else {
            result.first = -result.first - 1;
            result.second = denominator - result.second;
        }
        return result;
    } else {
        // TODO: implement a proper division algorithm
        error("Denominator too large to divide: " + denominator.toString());
    }

    // now b1 >= 0 && b2 > 0
    BigInteger quotient(ZERO);
    BigInteger remainder(numerator);
    while (remainder >= denominator) {
        ++quotient;
        remainder -= denominator;
    }
    std::pair<BigInteger, BigInteger> result {quotient, remainder};
    return result;
}

bool BigInteger::equals(const BigInteger& n1, const BigInteger& n2) {
    return n1.getNumber() == n2.getNumber()
        && n1.getSign() == n2.getSign();
}

void BigInteger::fixNegativeZero() {
    if (number == "0") {
        // avoid (-0) problem
        sign = false;
    }
}

BigInteger BigInteger::gcd(const BigInteger& other) const {
    BigInteger a(*this);
    BigInteger b(other);
    while (b != ZERO) {
        BigInteger temp(b);
        b = a % b;
        a = temp;
    }
    return a;
}

const std::string& BigInteger::getNumber() const {
    return number;
}

bool BigInteger::getSign() const {
    return sign;
}

bool BigInteger::greater(const BigInteger& n1, const BigInteger& n2) {
    return less(n2, n1);
}

bool BigInteger::isInt() const {
    return stringIsInteger(toString());
}

bool BigInteger::isLong() const {
    return stringIsLong(toString());
}

bool BigInteger::isNegative() const {
    return sign;   // true means negative
}

bool BigInteger::isNonNegative() const {
    return !sign;   // true means negative
}

bool BigInteger::isPositive() const {
    return !sign && number[0] != '0';
}

bool BigInteger::less(const BigInteger& n1, const BigInteger& n2) {
    bool sign1 = n1.getSign();
    bool sign2 = n2.getSign();

    if (sign1 && ! sign2) {
        // n1 is -ve and n2 is +ve
        return true;
    } else if (!sign1 && sign2) {
        // n1 is +ve and n2 is -ve
        return false;
    } else if (!sign1) {
        // both +ve
        if (n1.getNumber().length() < n2.getNumber().length()) {
            return true;
        } else if (n1.getNumber().length() > n2.getNumber().length()) {
            return false;
        } else {
            return n1.getNumber() < n2.getNumber();
        }
    } else {
        // both -ve
        if (n1.getNumber().length() > n2.getNumber().length()) {
            return true;
        } else if (n1.getNumber().length() < n2.getNumber().length()) {
            return false;
        } else {
            // greater with -ve sign is LESS
            return n1.getNumber().compare(n2.getNumber()) > 0;
        }
    }
}

const BigInteger& BigInteger::max(const BigInteger& other) const {
    if (*this < other) {
        return other;
    } else {
        return *this;
    }
}

const BigInteger& BigInteger::min(const BigInteger& other) const {
    if (*this > other) {
        return other;
    } else {
        return *this;
    }
}

BigInteger BigInteger::modPow(const BigInteger& exp, const BigInteger& m) const {
    BigInteger result = ONE;
    for (BigInteger i = ZERO; i < exp; i++) {
        result = (result * *this) % m;
    }
    return result;
}

std::string BigInteger::multiply(const std::string& n1, const std::string& n2) {
    std::string n1copy = n1;
    std::string n2copy = n2;

    if (n1copy.length() > n2copy.length()) {
        n1copy.swap(n2copy);
    }

    std::string product = "0";
    for (int i = n1copy.length() - 1; i >= 0; --i) {
        std::string temp = n2copy;
        int currentDigit = n1copy[i] - '0';
        int carry = 0;
        for (int j = temp.length() - 1; j >= 0; --j) {
            temp[j] = ((temp[j] - '0') * currentDigit) + carry;
            if (temp[j] > 9) {
                carry = (temp[j] / 10);
                temp[j] -= (carry * 10);
            } else {
                carry = 0;
            }

            temp[j] += '0';   // back to string mode
        }

        if (carry > 0) {
            temp.insert(0, (size_t) 1, (char) (carry + '0'));
        }

        // as like mult by 10, 100, 1000, 10000 and so on
        temp.append((n1copy.length() - i - 1), '0');

        product = add(product, temp);   // O(n)
    }

    // erase leading zeros
    product = removeLeadingZeros(product);
    return product;
}

BigInteger BigInteger::pow(long exp) const {
    if (exp < 0) {
        error("negative exponent: " + std::to_string(exp));
    } else if (exp == 0) {
        return ONE;
    } else if (exp == 1) {
        return *this;
    }

    // optimization for even exponents
    BigInteger thisCopy(*this);
    BigInteger result(ONE);
    while (exp > 0) {
        if (exp % 2 == 0) {
            thisCopy *= thisCopy;
            exp /= 2;
        } else {
            result *= thisCopy;
            exp--;
        }
    }
    return result;
}

BigInteger BigInteger::pow(const BigInteger& exp) const {
    if (exp < ZERO) {
        error("negative exponent: " + exp.toString());
    } else if (exp == ZERO) {
        return ONE;
    } else if (exp == ONE) {
        return *this;
    }

    // optimization for even exponents
    BigInteger thisCopy(*this);
    BigInteger expCopy(exp);
    BigInteger result(ONE);
    while (expCopy > ZERO) {
        if (expCopy % TWO == ZERO) {
            thisCopy *= thisCopy;
            expCopy /= TWO;
        } else {
            result *= thisCopy;
            expCopy--;
        }
    }
    return result;
}

std::string BigInteger::removeLeadingZeros(const std::string& s) {
    std::string result = s;
    while (result[0] == '0' && (int) result.length() > 1) {
        result.erase(0, 1);
    }
    if ((int) result.length() == 0) {
        result = "0";
    }
    return result;
}

void BigInteger::setValue(const std::string& s, int radix) {
    std::string scopy = stripNumberPrefix(s, radix);
    checkStringIsNumeric(scopy, radix);
    if (scopy[0] == '+' || scopy[0] == '-') {
        // signed value; separate sign from number
        setNumber(scopy.substr(1), radix);
        sign = (scopy[0] == '-');
    } else {
        // not signed; positive value
        setNumber(scopy, radix);
        sign = false;
    }
    fixNegativeZero();
}

// TODO: throw error on negative/too-large radixes

void BigInteger::setNumber(const std::string& s, int radix) {
    // accept hex as 0x???, bin as 0b???, oct as 0o???
    std::string scopy = stripNumberPrefix(s, radix);
    checkStringIsNumeric(scopy, radix);
    if (radix == 10) {
        number = scopy;
    } else if (scopy == "0") {
        number = scopy;   // 0 is 0 in any base
    } else if (radix == 1) {
        std::ostringstream out;
        for (int i = 0; i < radix; i++) {
            out << '1';
        }
        *this = BigInteger(out.str());
    } else {
        // convert to base-10 by plucking off a "digit" char,
        // and adding it to a cumulative sum BigInteger variable
        BigInteger result(ZERO);
        BigInteger power(ONE);
        BigInteger biRadix(std::to_string(radix));
        for (int i = (int) scopy.length() - 1; i >= 0; i--) {
            std::string ch = scopy.substr(i, 1);
            int val = stringToInteger(ch, radix);
            result += power * BigInteger(std::to_string(val));
            power *= biRadix;
        }
        *this = result;
    }
    fixNegativeZero();
}

void BigInteger::setSign(bool s) {
    sign = s;
    fixNegativeZero();
}

std::string BigInteger::stripNumberPrefix(const std::string& num, int radix) {
    std::string result;
    if (radix == 2 && (int) num.length() >= 2 && num[0] == '0' && tolower(num[1]) == 'b') {
        result = num.substr(2);
    } else if (radix == 8 && (int) num.length() >= 2 && num[0] == '0' && tolower(num[1]) == 'o') {
        result = num.substr(2);
    } else if (radix == 16 && (int) num.length() >= 2 && num[0] == '0' && tolower(num[1]) == 'x') {
        result = num.substr(2);
    } else {
        result = num;
    }
    result = stringReplace(result, ",", "");
    result = stringReplace(result, "_", "");
    result = stringReplace(result, " ", "");
    return result;
}

std::string BigInteger::subtract(const std::string& n1, const std::string& n2) {
    // sub variable to store result;
    // left-pad numbers with 0s to be the same length as needed
    int digits = (int) (n1.length() > n2.length() ? n1.length() : n2.length());
    std::string sub = padLeft("", digits);
    std::string n1copy = padLeft(n1, digits);
    std::string n2copy = padLeft(n2, digits);

    for (int i = (int) n1copy.length() - 1; i >= 0; --i) {
        if (n1copy[i] < n2copy[i]) {
            n1copy[i] += 10;
            n1copy[i - 1]--;
        }
        sub[i] = ((n1copy[i] - '0') - (n2copy[i] - '0')) + '0';
    }

    // erase leading zeros
    sub = removeLeadingZeros(sub);

    return sub;
}

long BigInteger::toLong(const std::string& s) {
    long sum = 0;
    long prevSum = 0;
    for (int i = 0; i < (int) s.length(); i++) {
        prevSum = sum;
        sum = (sum * 10) + (s[i] - '0');
        if (sum < prevSum) {
            error("numeric overflow when converting string to long: \"" + s + "\"");
        }
    }
    return sum;
}

int BigInteger::toInt() const {
    return (int) toLong(number);
}

long BigInteger::toLong() const {
    return toLong(number);
}

std::string BigInteger::toString(int radix) const {
    if (radix == 10) {
        return std::string(*this);
    } else if (radix <= 0) {
        error("Illegal radix value: " + std::to_string(radix));
    } else if (number == "0") {
        return "0";
    } else if (radix == 1) {
        std::ostringstream out;
        for (BigInteger i = ZERO; i < *this; i++) {
            out << "1";
        }
        return out.str();
    } else {
        // write string (reversed) into an output stream, digit by digit
        std::ostringstream out;
        out << std::setbase(radix);
        BigInteger copy(this->abs());
        BigInteger biRadix(std::to_string(radix));
        while (copy.isPositive()) {
            int lastDigit = (copy % biRadix).toInt();
            out << lastDigit;
            copy /= biRadix;
        }
        if (isNegative()) {
            out << '-';
        }

        // reverse and return the string
        std::string str = out.str();
        std::reverse(str.begin(), str.end());
        return str;
    }
    return "0";
}

std::string BigInteger::toString(long n) {
    std::stringstream ss;
    std::string temp;
    ss << n;
    ss >> temp;
    return temp;
}

BigInteger& BigInteger::operator =(const BigInteger& b) {
    setNumber(b.getNumber());
    setSign(b.getSign());
    return *this;
}

BigInteger& BigInteger::operator ++() {
    (*this) = (*this) + ONE;
    return (*this);
}

BigInteger BigInteger::operator ++(int) {
    BigInteger before = (*this);
    (*this) = (*this) + ONE;
    return before;
}

BigInteger& BigInteger::operator --() {
    (*this) = (*this) - ONE;
    return (*this);
}

BigInteger BigInteger::operator --(int) {
    BigInteger before = (*this);
    (*this) = (*this) - ONE;
    return before;
}

BigInteger& BigInteger::operator +=(const BigInteger& b) {
    (*this) = (*this) + b;
    return (*this);
}

BigInteger& BigInteger::operator -=(const BigInteger& b) {
    (*this) = (*this) - b;
    return (*this);
}

BigInteger& BigInteger::operator *=(const BigInteger& b) {
    (*this) = (*this) * b;
    return (*this);
}

BigInteger& BigInteger::operator /=(const BigInteger& b) {
    (*this) = (*this) / b;
    return (*this);
}

BigInteger& BigInteger::operator %=(const BigInteger& b) {
    (*this) = (*this) % b;
    return (*this);
}

BigInteger& BigInteger::operator &=(const BigInteger& b) {
    *this = (*this & b);
    return *this;
}

BigInteger& BigInteger::operator |=(const BigInteger& b) {
    *this = (*this | b);
    return *this;
}

BigInteger& BigInteger::operator ^=(const BigInteger& b) {
    *this = (*this ^ b);
    return *this;
}

BigInteger BigInteger::operator ~() const {
    std::string s1 = this->toString(2);

    // loop over each bit, performing ~ operation
    for (int i = 0, len = (int) s1.length(); i < len; i++) {
        bool b1 = s1[i] == '1';
        s1[i] = b1 ? '0' : '1';
    }
    s1 = removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

BigInteger BigInteger::operator !() const {
    if (*this == ZERO) {
        return ONE;
    } else {
        return ZERO;
    }
}

BigInteger& BigInteger::operator [](int n) {
    return *(this + (n * sizeof(BigInteger)));
}

BigInteger BigInteger::operator -() const {
    return BigInteger(getNumber(), !getSign());
}

//BigInteger& BigInteger::operator =(const BigInteger& other) {
//    this->number = other.number;
//    this->sign = other.sign;
//}

BigInteger BigInteger::operator <<(unsigned int shift) const {
    BigInteger result(*this);
    for (unsigned int i = 0; i < shift; i++) {
        result *= TWO;
    }
    return result;
}

BigInteger& BigInteger::operator <<=(unsigned int shift) {
    (*this) = (*this) << shift;
    return *this;
}

BigInteger BigInteger::operator >>(unsigned int shift) const {
    BigInteger result(*this);
    for (unsigned int i = 0; i < shift; i++) {
        result /= TWO;
    }
    return result;
}

BigInteger& BigInteger::operator >>=(unsigned int shift) {
    (*this) = (*this) >> shift;
    return *this;
}

BigInteger::operator bool() const {
    return *this != ZERO;
}

//BigInteger::operator double() const {
//    return (double) toLong();
//}

//BigInteger::operator float() const {
//    return (float) toLong();
//}

BigInteger::operator int() const {
    return toInt();
}

BigInteger::operator long() const {
    return toLong();
}

BigInteger::operator std::string() const {
    // if positive, don't print + sign
    std::string signedString = (getSign()) ? "-" : "";
    signedString += number;
    return signedString;
}

std::string bigIntegerToString(const BigInteger& bi, int radix) {
    return bi.toString(radix);
}

int hashCode(const BigInteger& b) {
    return hashCode(b.getNumber(), b.getSign());
}

BigInteger operator +(const BigInteger& b1, const BigInteger& b2) {
    BigInteger sum;
    if (b1.getSign() == b2.getSign()) {
        // both +ve or -ve
        sum.setNumber(BigInteger::add(b1.getNumber(), b2.getNumber()));
        sum.setSign(b1.getSign());
    } else {
        // sign different
        if (b1.abs() > b2.abs()) {
            sum.setNumber(BigInteger::subtract(b1.getNumber(), b2.getNumber()));
            sum.setSign(b1.getSign());
        } else {
            sum.setNumber(BigInteger::subtract(b2.getNumber(), b1.getNumber()));
            sum.setSign(b2.getSign());
        }
    }
    if (sum.getNumber() == "0") {
        // avoid (-0) problem
        sum.setSign(false);
    }
    return sum;
}

BigInteger operator -(const BigInteger& b1, const BigInteger& b2) {
    // x - y = x + (-y)
    BigInteger bcopy = b2;
    bcopy.setSign(!b2.getSign());
    return b1 + bcopy;
}

BigInteger operator *(const BigInteger& b1, const BigInteger& b2) {
    BigInteger product;
    product.setNumber(BigInteger::multiply(b1.getNumber(), b2.getNumber()));
    product.setSign(b1.getSign() != b2.getSign());
    return product;
}

BigInteger operator /(const BigInteger& b1, const BigInteger& b2) {
    if (b2 == BigInteger::ZERO) {
        error("Division by zero");
    } else if (BigInteger::MIN_LONG <= b2 && b2 <= BigInteger::MAX_LONG) {
        long den = BigInteger::toLong(b2.getNumber());
        BigInteger div;
        div.setNumber(BigInteger::divide(b1.getNumber(), den).first);
        div.setSign(b1.getSign() != b2.getSign());
        return div;
    }

    // else large denominator; compute through naive repeated subtraction
    // (this is not a smart way to do large integer division, but this
    // library is not meant to be particularly clever or performant)
    std::pair<BigInteger, BigInteger> result = BigInteger::divideBig(b1, b2);
    return result.first;
}

BigInteger operator %(const BigInteger& b1, const BigInteger& b2) {
    if (b2 == BigInteger::ZERO) {
        error("Division by zero");
    } else if (BigInteger::MIN_LONG <= b2 && b2 <= BigInteger::MAX_LONG) {
        long den = BigInteger::toLong(b2.getNumber());
        BigInteger div;
        div.setNumber(BigInteger::divide(b1.getNumber(), den).first);
        div.setSign(b1.getSign() != b2.getSign());
        return div;
    }

    // else large denominator; compute through naive repeated subtraction
    // (this is not a smart way to do large integer division, but this
    // library is not meant to be particularly clever or performant)
    std::pair<BigInteger, BigInteger> result = BigInteger::divideBig(b1, b2);
    return result.second;
}

BigInteger operator &(const BigInteger& b1, const BigInteger& b2) {
    // build same-length binary strings for both numbers
    std::string s1 = b1.toString(2);
    std::string s2 = b2.toString(2);
    int len = (int) (s1.length() > s2.length() ? s1.length() : s2.length());
    s1 = padLeft(s1, len);
    s2 = padLeft(s2, len);

    // loop over each bit, performing & operation; store into s1
    for (int i = 0; i < len; i++) {
        bool b1 = s1[i] == '1';
        bool b2 = s2[i] == '1';
        s1[i] = (b1 && b2) ? '1' : '0';
    }
    s1 = BigInteger::removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

BigInteger operator |(const BigInteger& b1, const BigInteger& b2) {
    // build same-length binary strings for both numbers
    std::string s1 = b1.toString(2);
    std::string s2 = b2.toString(2);
    int len = (int) (s1.length() > s2.length() ? s1.length() : s2.length());
    s1 = padLeft(s1, len);
    s2 = padLeft(s2, len);

    // loop over each bit, performing | operation; store into s1
    for (int i = 0; i < len; i++) {
        bool b1 = s1[i] == '1';
        bool b2 = s2[i] == '1';
        s1[i] = (b1 || b2) ? '1' : '0';
    }
    // don't need to check for leading 0s because | can only make more 1s than before
    // s1 = removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

BigInteger operator ^(const BigInteger& b1, const BigInteger& b2) {
    // build same-length binary strings for both numbers
    std::string s1 = b1.toString(2);
    std::string s2 = b2.toString(2);
    int len = (int) (s1.length() > s2.length() ? s1.length() : s2.length());
    s1 = padLeft(s1, len);
    s2 = padLeft(s2, len);

    // loop over each bit, performing ^ operation; store into s1
    for (int i = 0; i < len; i++) {
        bool b1 = s1[i] == '1';
        bool b2 = s2[i] == '1';
        s1[i] = (b1 ^ b2) ? '1' : '0';
    }
    s1 = BigInteger::removeLeadingZeros(s1);

    return BigInteger(s1, 2);
}

bool operator ==(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::equals(b1, b2);
}

bool operator !=(const BigInteger& b1, const BigInteger& b2) {
    return !BigInteger::equals(b1, b2);
}

bool operator >(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::greater(b1, b2);
}

bool operator <(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::less(b1, b2);
}

bool operator >=(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::equals(b1, b2)
            || BigInteger::greater(b1, b2);
}

bool operator <=(const BigInteger& b1, const BigInteger& b2) {
    return BigInteger::equals(b1, b2)
            || BigInteger::less(b1, b2);
}

std::istream& operator >>(std::istream& input, BigInteger& b) {
    std::string s;
    input >> s;
    b.setValue(s);
    return input;
}

std::ostream& operator <<(std::ostream& out, const BigInteger& b) {
    return out << std::string(b);
}

//BigInteger operator +(int n, const BigInteger& b) {
//    return BigInteger(n) + b;
//}

//BigInteger operator -(int n, const BigInteger& b) {
//    return BigInteger(n) - b;
//}

//BigInteger operator *(int n, const BigInteger& b) {
//    return BigInteger(n) * b;
//}

//BigInteger operator /(int n, const BigInteger& b) {
//    return BigInteger(n) / b;
//}

//BigInteger operator %(int n, const BigInteger& b) {
//    return BigInteger(n) % b;
//}

/*
 * File: require.cpp
 * -----------------
 * This file implements the require.h interface.
 *
 * @version 2018/09/05
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "require.h"
#include <string>
#define INTERNAL_INCLUDE 1
#include "error.h"
#undef INTERNAL_INCLUDE

namespace require {

static void _errorMessage(const std::string& caller, const std::string& valueName, const std::string& details) {
    std::string overallMessage = "";
    if (!caller.empty()) {
        overallMessage += caller;
    }
    if (!valueName.empty() || !details.empty()) {
        if (!overallMessage.empty()) {
            overallMessage += ": ";
        }
        if (!valueName.empty()) {
            if (!overallMessage.empty()) {
                overallMessage += " ";
            }
            overallMessage += valueName;
            if (!details.empty()) {
                overallMessage + " ";
            }
        }
        overallMessage += details;
    }
    error(overallMessage);
}

#define _spl_assert(test, caller, valueName, details) \
    if (!(test)) { \
        _errorMessage((caller), _default((valueName), "value"), (details)); \
    }

#define _default(value, defaultValue) ((value) == std::string("") ? (defaultValue) : (value))

void inRange(double value, double min, double max, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(min <= value && value <= max, caller, _default(valueName, "value"), _default(details, "must be between " + std::to_string(min) + " and " + std::to_string(max) + " inclusive but was " + std::to_string(value)));
}

void inRange(int value, int min, int max, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(min <= value && value <= max, caller, _default(valueName, "value"), _default(details, "must be between " + std::to_string(min) + " and " + std::to_string(max) + " inclusive but was " + std::to_string(value)));
}

void inRange2D(double x, double y, double maxX, double maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange2D(x, y, /* minX */ 0.0, /* minY */ 0.0, maxX, maxY, caller, xValueName, yValueName, details);
}

void inRange2D(double x, double y, double minX, double minY, double maxX, double maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange(x, minX, maxX, caller, xValueName, _default(details, "must be between (" + std::to_string(minX) + "," + std::to_string(minY) + ")-" + std::to_string(maxX) + "," + std::to_string(maxY) + ") inclusive but was (" + std::to_string(x) + "," + std::to_string(y) + ")"));
    inRange(y, minY, maxY, caller, yValueName, _default(details, "must be between (" + std::to_string(minX) + "," + std::to_string(minY) + ")-" + std::to_string(maxX) + "," + std::to_string(maxY) + ") inclusive but was (" + std::to_string(x) + "," + std::to_string(y) + ")"));
}

void inRange2D(int x, int y, int maxX, int maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange2D(x, y, /* minX */ 0, /* minY */ 0, maxX, maxY, caller, xValueName, yValueName, details);
}

void inRange2D(int x, int y, int minX, int minY, int maxX, int maxY, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    inRange(x, minX, maxX, caller, xValueName, _default(details, "must be between (" + std::to_string(minX) + "," + std::to_string(minY) + ")-" + std::to_string(maxX) + "," + std::to_string(maxY) + ") inclusive but was (" + std::to_string(x) + "," + std::to_string(y) + ")"));
    inRange(y, minY, maxY, caller, yValueName, _default(details, "must be between (" + std::to_string(minX) + "," + std::to_string(minY) + ")-" + std::to_string(maxX) + "," + std::to_string(maxY) + ") inclusive but was (" + std::to_string(x) + "," + std::to_string(y) + ")"));
}

void nonEmpty(const std::string& str, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(!str.empty(), caller, _default(valueName, "string"), _default(details, "must not be empty"));
}

void nonNegative(double value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value >= 0.0, caller, _default(valueName, "value"), _default(details, "must be non-negative but was " + std::to_string(value)));
}

void nonNegative(int value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value >= 0, caller, _default(valueName, "value"), _default(details, "must be non-negative but was " + std::to_string(value)));
}

void nonNegative2D(double x, double y, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    nonNegative(x, caller, xValueName, details);
    nonNegative(y, caller, yValueName, details);
}

void nonNegative2D(int x, int y, const std::string& caller, const std::string& xValueName, const std::string& yValueName, const std::string& details) {
    nonNegative(x, caller, xValueName, details);
    nonNegative(y, caller, yValueName, details);
}

void nonNull(const void* ptr, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(ptr != nullptr, caller, _default(valueName, "value"), _default(details, "must be non-null"));
}

void positive(double value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value > 0.0, caller, _default(valueName, "value"), _default(details, "must be positive but was " + std::to_string(value)));
}

void positive(int value, const std::string& caller, const std::string& valueName, const std::string& details) {
    _spl_assert(value > 0, caller, _default(valueName, "value"), _default(details, "must be positive but was " + std::to_string(value)));
}

void require(bool test, const std::string& caller, const std::string& details) {
    _spl_assert(test, caller, "", details);
}

} // namespace require

/*
 * File: gmath.cpp
 * ---------------
 * This file implements the gmath.h interface.  In all cases, the
 * implementation for each function requires only one line of code,
 * which makes detailed documentation unnecessary.
 *
 * @version 2018/11/22
 * - added headless mode support
 * - alphabetized methods
 * @version 2016/10/14
 * - modified floating-point equality tests to use floatingPointEqual function
 */

#define INTERNAL_INCLUDE 1
#include "gmath.h"
#include <cmath>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#undef INTERNAL_INCLUDE

extern const double PI = 3.14159265358979323846;
extern const double E  = 2.71828182845904523536;

double cosDegrees(double angle) {
    return cos(toRadians(angle));
}

int countDigits(int n, int base) {
    if (base <= 0) {
        error("countDigits: base must be 1 or greater");
    }
    if (n == 0) {
        return 0;
    } else if (n < 0) {
        n = -n;
    }

    int digits = 0;
    for (int temp = n; temp > 0 && digits < 65; temp /= base) {
        digits++;
    }
    return digits;
}

double sinDegrees(double angle) {
    return sin(toRadians(angle));
}

double tanDegrees(double angle) {
    return tan(toRadians(angle));
}

double toDegrees(double radians) {
    return radians * 180 / PI;
}

double toRadians(double degrees) {
    return degrees * PI / 180;
}

double vectorAngle(double x, double y) {
    return floatingPointEqual(x, 0) && floatingPointEqual(y, 0)
            ? 0 : toDegrees(atan2(-y, x));
}

#ifndef SPL_HEADLESS_MODE
double vectorAngle(const GPoint& pt) {
    return vectorAngle(pt.getX(), pt.getY());
}
#endif // SPL_HEADLESS_MODE

double vectorAngle(const Point& pt) {
    return vectorAngle(pt.getX(), pt.getY());
}

double vectorDistance(double x, double y) {
    return sqrt(x * x + y * y);
}

#ifndef SPL_HEADLESS_MODE
double vectorDistance(const GPoint& pt) {
    return vectorDistance(pt.getX(), pt.getY());
}
#endif // SPL_HEADLESS_MODE

double vectorDistance(const Point& pt) {
    return vectorDistance(pt.getX(), pt.getY());
}

/*
 * File: complex.cpp
 * -----------------
 * This file defines a type representing complex numbers.
 * See complex.h for declarations and documentation of class and members.
 *
 * @version 2017/10/18
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "complex.h"
#include <cctype>
#include <cmath>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "gmath.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

Complex::Complex(double a, double b) {
    this->a = a;
    this->b = b;
}

double Complex::abs() const {
   return sqrt(a*a + b*b);
}

double Complex::imag() const {
    return b;
}

double Complex::real() const {
    return a;
}

std::string Complex::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const Complex& c) {
    return hashCode(c.real(), c.imag());
}

Complex operator *(const Complex& m, const Complex& n) {
   double realP = m.a * n.a - m.b * n.b;
   double imagP = m.b * n.a + m.a * n.b;
   return Complex(realP, imagP);
}

Complex operator +(const Complex& m, const Complex& n) {
   double realP = m.a + n.a;
   double imagP = m.b + n.b;
   return Complex(realP, imagP);
}

Complex operator -(const Complex& m, const Complex& n) {
   double realP = m.a - n.a;
   double imagP = m.b - n.b;
   return Complex(realP, imagP);
}

bool operator <(const Complex& c1, const Complex& c2) {
    return c1.real() < c2.real() ||
            (floatingPointEqual(c1.real(), c2.real()) && c1.imag() < c2.imag());
}

bool operator <=(const Complex& c1, const Complex& c2) {
    return c1 < c2 || c1 == c2;
}

bool operator >(const Complex& c1, const Complex& c2) {
    return c2 < c1;
}

bool operator >=(const Complex& c1, const Complex& c2) {
    return c2 < c1 || c2 == c1;
}

bool operator ==(const Complex& c1, const Complex& c2) {
    return floatingPointEqual(c1.real(), c2.real())
            && floatingPointEqual(c1.imag(), c2.imag());
}

bool operator !=(const Complex& c1, const Complex& c2) {
    return !(c1 == c2);
}

std::ostream& operator <<(std::ostream& out, const Complex& c) {
    out << c.real();
    if (!floatingPointEqual(c.imag(), 0.0)) {
        out << (c.imag() > 0 ? "+" : "") << c.imag() << "i";
    }
    return out;
}

// This code is pretty ugly.
// I don't remember some of the ins and outs of the istream API.
// Oh, well.
std::istream& operator >>(std::istream& input, Complex& c) {
    input >> std::ws;   // skip whitespace

    // optional '('
    bool needClosingParen = false;
    if (input.peek() == '(') {
        input.get();
        needClosingParen = true;
    }

    // read real part
    input >> std::ws;
    double real = 0.0;
    bool readReal = false;

    std::streampos pos = input.tellg();
    if (input >> real) {
        readReal = true;
    } else {
        input.clear();
        input.seekg(pos);
    }

    // read imag part
    input >> std::ws;
    double imag = 0.0;
    bool readImag = false;

    pos = input.tellg();
    if (input >> imag) {
        readImag = true;
    } else {
        input.clear();
        input.seekg(pos);
        if (input.peek() == 'i') {
            imag = 1.0;
            readImag = true;
        } else if (input.peek() == '-' || input.peek() == '+') {
            bool isNegative = input.peek() == '-';
            input.get();
            input >> std::ws;
            pos = input.tellg();

            if (input >> imag) {
                if (isNegative) {
                    imag = -imag;
                }
                readImag = true;
            } else {
                input.clear();
                input.seekg(pos);
                if (input.peek() == 'i') {
                    imag = isNegative ? -1.0 : 1.0;
                    readImag = true;
                } else {
                    input.clear();
                    input.unget();
                }
            }
        }
    }

    // read 'i'
    input >> std::ws;
    if (input.peek() == 'i') {
        input.get();
    }

    // optional ')'
    input >> std::ws;
    if (input.peek() == ')') {
        input.get();
    } else if (needClosingParen) {
        // invalid input; saw opening ( but no closing )
        input.setstate(std::ios_base::failbit);
        return input;
    }

    if (readReal || readImag) {
        input.clear();   // unset fail state
        c.a = real;
        c.b = imag;
    } else {
        input.setstate(std::ios_base::failbit);
    }

    return input;
}

/*
 * File: strlib.cpp
 * ----------------
 * This file implements the strlib.h interface.
 * 
 * @version 2018/11/14
 * - added std::to_string for bool, char, pointer, and generic template type T
 * - bug fix for pointerToString (was putting two "0x" prefixes)
 * @version 2018/09/02
 * - added padLeft, padRight
 * @version 2017/10/24
 * - print nullptr instead of null in uppercase
 * @version 2016/11/07
 * - bug fix: urlDecode throws error on invalid encodings (courtesy GitHub @scinart)
 * @version 2016/10/30
 * - alphabetized functions
 * - added overloads that take type char instead of string:
 *   stringContains, stringIndexOf, stringJoin, stringLastIndexOf, stringReplace,
 *   stringSplit, toLowerCase, toUpperCase
 * @version 2016/10/13
 * - modified writeQuotedString to return ostream
 * @version 2016/08/03
 * - modified readQuotedString not to throw error() on parse failures
 *   (needed to support idiomatic silent-failing >> operators)
 * @version 2015/11/07
 * - fixed bugs in urlDecode (wasn't decoding % sequences properly, oops)
 * @version 2015/10/26
 * - added charToInteger/integerToChar functions
 * @version 2015/06/19
 * - slight bug fix to make stringToInteger functions compile with int radix
 * @version 2015/05/22
 * - slight bug fix in stringToBool function
 * @version 2014/10/31
 * - fixed infinite loop bug in stringReplace function
 * @version 2014/10/19
 * - alphabetized functions
 * - added several 'inPlace' variants of existing functions that return strings
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "strlib.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

/* Function prototypes */

std::string boolToString(bool b) {
    return (b ? "true" : "false");
}

std::string boolToString(int b) {
    return (b ? "true" : "false");
}

int charToInteger(char c) {
    if (c < '0' || c > '9') {
        std::ostringstream out;
        out << "charToInteger: character is not numeric: '" << c
            << "' (ASCII value " << (int) c << ")";
        error(out.str());
    }
    return c - '0';
}

std::string charToString(char c) {
    std::string s;
    s += c;
    return s;
}

std::string doubleToString(double d) {
    return realToString(d);
}

bool endsWith(const std::string& str, char suffix) {
    return str.length() > 0 && str[str.length() - 1] == suffix;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    int nChars = suffix.length();
    int start = str.length() - nChars;
    if (start < 0) return false;
    for (int i = 0; i < nChars; i++) {
        if (str[start + i] != suffix[i]) return false;
    }
    return true;
}

/*
 * Implementation notes: equalsIgnoreCase
 * --------------------------------------
 * This implementation uses a for loop to cycle through the characters in
 * each string.  Converting each string to uppercase and then comparing
 * the results makes for a shorter but less efficient implementation.
 */
bool equalsIgnoreCase(const std::string& s1, const std::string& s2) {
    if (s1.length() != s2.length()) return false;
    int nChars = s1.length();
    for (int i = 0; i < nChars; i++) {
        if (tolower(s1[i]) != tolower(s2[i])) return false;
    }
    return true;
}

std::string htmlDecode(const std::string& s) {
    std::string result = s;
    stringReplaceInPlace(result, "&lt;", "<");
    stringReplaceInPlace(result, "&gt;", ">");
    stringReplaceInPlace(result, "&quot;", "\"");
    stringReplaceInPlace(result, "&amp;", "&");
    return result;
}

std::string htmlEncode(const std::string& s) {
    std::string result = s;
    stringReplaceInPlace(result, "&", "&amp;");
    stringReplaceInPlace(result, "<", "&lt;");
    stringReplaceInPlace(result, ">", "&gt;");
    stringReplaceInPlace(result, "\"", "&quot;");
    return result;
}

char integerToChar(int n) {
    if (n < 0 || n > 9) {
        std::ostringstream out;
        out << "integerToChar: number must be between 0-9: " << n;
        error(out.str());
    }
    return (char) (n + '0');
}

/*
 * Implementation notes: numeric conversion
 * ----------------------------------------
 * These functions use the <sstream> library to perform the conversion.
 */
std::string integerToString(int n, int radix) {
    if (radix <= 0) {
        error("integerToString: Illegal radix: " + std::to_string(radix));
    }
    std::ostringstream stream;
    if (radix != 10) {
        stream << std::setbase(radix);
    }
    stream << n;
    return stream.str();
}

std::string longToString(long n, int radix) {
    if (radix <= 0) {
        error("longToString: Illegal radix: " + std::to_string(radix));
    }
    std::ostringstream stream;
    if (radix != 10) {
        stream << std::setbase(radix);
    }
    stream << n;
    return stream.str();
}

std::string padLeft(const std::string& s, int length, char fill) {
    if ((int) s.length() >= length) {
        return s;
    } else {
        std::ostringstream out;
        for (int i = 0, count = length - (int) s.length(); i < count; i++) {
            out << fill;
        }
        out << s;
        return out.str();
    }
}

std::string padRight(const std::string& s, int length, char fill) {
    if ((int) s.length() >= length) {
        return s;
    } else {
        std::ostringstream out;
        out << s;
        for (int i = 0, count = length - (int) s.length(); i < count; i++) {
            out << fill;
        }
        return out.str();
    }
}

std::string pointerToString(void* p) {
    if (p) {
        std::ostringstream stream;
        stream << std::hex;
        stream << p;
        return stream.str();
    } else {
        return "nullptr";
    }
}

std::string realToString(double d) {
    std::ostringstream stream;
    stream << std::uppercase << d;
    return stream.str();
}

bool startsWith(const std::string& str, char prefix) {
    return str.length() > 0 && str[0] == prefix;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) return false;
    int nChars = prefix.length();
    for (int i = 0; i < nChars; i++) {
        if (str[i] != prefix[i]) return false;
    }
    return true;
}

bool stringIsBool(const std::string& str) {
    return str == "true" || str == "false";
}

bool stringIsDouble(const std::string& str) {
    return stringIsReal(str);
}

bool stringIsInteger(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringIsInteger: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    int value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringIsLong(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringIsLong: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    long value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringIsReal(const std::string& str) {
    std::istringstream stream(trim(str));
    double value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringContains(const std::string& s, char ch) {
    return s.find(ch) != std::string::npos;
}

bool stringContains(const std::string& s, const std::string& substring) {
    return s.find(substring) != std::string::npos;
}

int stringIndexOf(const std::string& s, char ch, int startIndex) {
    size_t index = s.find(ch, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

int stringIndexOf(const std::string& s, const std::string& substring, int startIndex) {
    size_t index = s.find(substring, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

std::string stringJoin(const Vector<std::string>& v, char delimiter) {
    std::string delim = charToString(delimiter);
    return stringJoin(v, delim);
}

std::string stringJoin(const Vector<std::string>& v, const std::string& delimiter) {
    if (v.isEmpty()) {
        return "";
    } else {
        std::ostringstream out;
        out << v[0];
        for (int i = 1; i < (int) v.size(); i++) {
            out << delimiter;
            out << v[i];
        }
        return out.str();
    }
}

int stringLastIndexOf(const std::string& s, char ch, int startIndex) {
    size_t index = s.rfind(ch, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

int stringLastIndexOf(const std::string& s, const std::string& substring, int startIndex) {
    size_t index = s.rfind(substring, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

std::string stringReplace(const std::string& str, char old, char replacement, int limit) {
    std::string str2 = str;
    stringReplaceInPlace(str2, old, replacement, limit);
    return str2;
}

std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit) {
    std::string str2 = str;
    stringReplaceInPlace(str2, old, replacement, limit);
    return str2;
}

int stringReplaceInPlace(std::string& str, char old, char replacement, int limit) {
    int count = 0;
    for (size_t i = 0, len = str.length(); i < len; i++) {
        if (str[i] == old) {
            str[i] = replacement;
            count++;
            if (limit > 0 && count >= limit) {
                break;
            }
        }
    }
    return count;
}

int stringReplaceInPlace(std::string& str, const std::string& old, const std::string& replacement, int limit) {
    int count = 0;
    size_t startIndex = 0;
    size_t rlen = replacement.length();
    while (limit <= 0 || count < limit) {
        size_t index = str.find(old, startIndex);
        if (index == std::string::npos) {
            break;
        }
        str.replace(index, old.length(), replacement);
        startIndex = index + rlen;
        count++;
    }
    return count;
}

Vector<std::string> stringSplit(const std::string& str, char delimiter, int limit) {
    std::string delim = charToString(delimiter);
    return stringSplit(str, delim, limit);
}

Vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit) {
    std::string str2 = str;
    Vector<std::string> result;
    int count = 0;
    size_t index = 0;
    while (limit < 0 || count < limit) {
        index = str2.find(delimiter);
        if (index == std::string::npos) {
            break;
        }
        result.add(str2.substr(0, index));
        str2.erase(str2.begin(), str2.begin() + index + delimiter.length());
        count++;
    }
    if ((int) str2.length() > 0) {
        result.add(str2);
    }

    return result;
}

bool stringToBool(const std::string& str) {
    if (str == "true" || str == "1") {
        return true;
    } else if (str == "false" || str == "0") {
        return false;
    }
    std::istringstream stream(trim(str));
    bool value;
    stream >> std::boolalpha >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToBool: Illegal bool format (" + str + ")");
    }
    return value;
}

char stringToChar(const std::string& str) {
    std::string str2 = trim(str);
    if ((int) str2.length() != 1) {
        error("stringToChar: string must contain exactly 1 non-whitespace character");
    }
    return str2[0];
}

double stringToDouble(const std::string& str) {
    return stringToReal(str);
}

int stringToInteger(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringToInteger: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    int value;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToInteger: Illegal integer format: \"" + str + "\"");
    }
    return value;
}

long stringToLong(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringToLong: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    long value;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToLong: Illegal long format \"" + str + "\"");
    }
    return value;
}

double stringToReal(const std::string& str) {
    std::istringstream stream(trim(str));
    double value;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToReal: Illegal floating-point format (" + str + ")");
    }
    return value;
}

char toLowerCase(char ch) {
    return (char) tolower(ch);
}

std::string toLowerCase(const std::string& str) {
    std::string str2 = str;
    toLowerCaseInPlace(str2);
    return str2;
}

void toLowerCaseInPlace(std::string& str) {
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        str[i] = tolower(str[i]);
    }
}

char toUpperCase(char ch) {
    return (char) toupper(ch);
}

std::string toUpperCase(const std::string& str) {
    std::string str2 = str;
    toUpperCaseInPlace(str2);
    return str2;
}

void toUpperCaseInPlace(std::string& str) {
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        str[i] = toupper(str[i]);
    }
}

std::string trim(const std::string& str) {
    std::string str2 = str;
    trimInPlace(str2);
    return str2;
}

void trimInPlace(std::string& str) {
    trimEndInPlace(str);
    trimStartInPlace(str);
}

std::string trimEnd(const std::string& str) {
    std::string str2 = str;
    trimEndInPlace(str2);
    return str2;
}

void trimEndInPlace(std::string& str) {
    int end = (int) str.length();
    int finish = end;
    while (finish > 0 && isspace(str[finish - 1])) {
        finish--;
    }
    if (finish < end) {
        str.erase(finish, end - finish);
    }
}

std::string trimStart(const std::string& str) {
    std::string str2 = str;
    trimStartInPlace(str2);
    return str2;
}

void trimStartInPlace(std::string& str) {
    int start = 0;
    int finish = (int) str.length() - 1;
    while (start <= finish && isspace(str[start])) {
        start++;
    }
    if (start > 0) {
        str.erase(0, start);
    }
}

std::string urlDecode(const std::string& str) {
    std::ostringstream unescaped;
    for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            unescaped << c;
        } else if (c == '+')  {
            unescaped << ' ';
        } else if (c == '%') {
            // throw error if string is invalid and doesn't have 2 char after,
            // or if it has non-hex chars here (courtesy GitHub @scinart)
            if (i + 2 >= n || !isxdigit(*(i + 1)) || !isxdigit(*(i + 2))) {
                error("urlDecode: Invalid percent-encoding");
            }

            // decode a URL-encoded ASCII character, e.g. %40 => &
            char ch1 = *(i + 1);
            char ch2 = *(i + 2);
            int hex1 = (isdigit(ch1) ? (ch1 - '0') : (toupper(ch1) - 'A' + 10));
            int hex2 = (isdigit(ch2) ? (ch2 - '0') : (toupper(ch2) - 'A' + 10));
            int decodedChar = (hex1 << 4) + hex2;
            unescaped << (char) decodedChar;
            i += 2;
        } else {
            std::ostringstream msg;
            msg << "urlDecode: Unexpected character in string: "
                << (int) c << " (" << c << ")";
            error(msg.str());
        }
    }

    return unescaped.str();
}

void urlDecodeInPlace(std::string& str) {
    str = urlDecode(str);   // no real efficiency gain here
}

std::string urlEncode(const std::string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex << std::uppercase;

    for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            escaped << c;
        } else if (c == ' ')  {
            escaped << '+';
        } else {
            escaped << '%' << std::setw(2) << ((int) c) << std::setw(0);
        }
    }

    return escaped.str();
}

void urlEncodeInPlace(std::string& str) {
    str = urlEncode(str);   // no real efficiency gain here
}

namespace std {
bool stob(const std::string& str) {
    return ::stringToBool(str);
}

char stoc(const std::string& str) {
    return ::stringToChar(str);
}

std::string to_string(bool b) {
    return ::boolToString(b);
}

std::string to_string(char c) {
    return ::charToString(c);
}

std::string to_string(void* p) {
    return ::pointerToString(p);
}
} // namespace std

/*
 * File: point.cpp
 * ---------------
 * This file implements the point.h interface.
 * 
 * @version 2018/11/22
 * - added headless mode support
 * @version 2017/09/29
 * - updated to use composite hashCode function
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "point.h"
#include <string>
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

#ifndef SPL_HEADLESS_MODE
Point::Point(const GPoint& point) {
    this->x = (int) point.getX();
    this->y = (int) point.getY();
}
#endif // SPL_HEADLESS_MODE

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

std::string Point::toString() const {
    return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
}

bool Point::operator ==(const Point& p2) const {
    return (x == p2.x) && (y == p2.y);
}

bool Point::operator !=(const Point& p2) const {
    return (x != p2.x) || (y != p2.y);
}

std::ostream& operator <<(std::ostream& os, const Point& pt) {
    return os << pt.toString();
}

int hashCode(const Point& pt) {
    return hashCode(pt.getX(), pt.getY());
}

/*
 * File: intrange.cpp
 * ------------------
 * This file implements the <code>IntRange</code> class.
 * See intrange.h for the declarations of each member.
 *
 * @version 2018/08/25
 * - renamed from range.cpp to intrange.cpp
 * @version 2018/03/12
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "intrange.h"
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

IntRange::IntRange(int length)
    : _min(0),
      _max(length - 1) {
    // empty
}

IntRange::IntRange(int minValue, int maxValue)
    : _min(minValue),
      _max(maxValue) {
    // empty
}

IntRange::IntRangeIterator IntRange::begin() const {
    return IntRangeIterator(this, /* end */ false);
}

bool IntRange::contains(int n) const {
    return _min <= n && n <= _max;
}

bool IntRange::contains(const IntRange& r) const {
    return _min <= r._min && _max >= r._max;
}

bool IntRange::empty() const {
    return _max < _min;
}

IntRange::IntRangeIterator IntRange::end() const {
    return IntRangeIterator(this, /* end */ true);
}

//bool IntRange::intersects(const IntRange& r) const {
//    return (_min <= r._min && r._min <= _max)
//            || (_min <= r._max && r._max <= _max);
//}

bool IntRange::isEmpty() const {
    return _max < _min;
}

int IntRange::length() const {
    if (isEmpty()) {
        return 0;
    } else {
        return _max - _min + 1;
    }
}

int IntRange::max() const {
    return _max;
}

int IntRange::min() const {
    return _min;
}

int IntRange::size() const {
    return length();
}

std::string IntRange::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const IntRange& r) {
    return hashCode(r.min(), r.max());
}

IntRange range(int length) {
    return IntRange(length);
}

IntRange range(int min, int max) {
    return IntRange(min, max);
}

bool operator <(const IntRange& r1, const IntRange& r2) {
    return r1.min() < r2.min() ||
            (r1.min() == r2.min() && r1.max() < r2.max());
}

bool operator <=(const IntRange& r1, const IntRange& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator ==(const IntRange& r1, const IntRange& r2) {
    return r1.min() == r2.min() && r1.max() == r2.max();
}

bool operator !=(const IntRange& r1, const IntRange& r2) {
    return !(r1 == r2);
}

bool operator >(const IntRange& r1, const IntRange& r2) {
    return r2 < r1;
}

bool operator >=(const IntRange& r1, const IntRange& r2) {
    return !(r1 < r2);
}

std::ostream& operator <<(std::ostream& out, const IntRange& r) {
    return out << "[" << r.min() << " .. " << r.max() << "]";
}

std::istream& operator >>(std::istream& input, IntRange& r) {
    // read '['
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read min
    int min;
    if (!(input >> min)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ' .. '
    input.get();
    input.get();
    input.get();
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read max
    int max;
    if (!(input >> max)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ']'
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // success!
    r._min = min;
    r._max = max;
    return input;
}

/////////////////////////////////////////////////////////////////////
// IntRange2D
/////////////////////////////////////////////////////////////////////

IntRange2D::IntRange2D(int width, int height, bool yMajor)
    : _minX(0),
      _minY(0),
      _maxX(width - 1),
      _maxY(height - 1),
      _yMajor(yMajor) {
    // empty
}

IntRange2D::IntRange2D(int minX, int minY, int maxX, int maxY, bool yMajor)
    : _minX(minX),
      _minY(minY),
      _maxX(maxX),
      _maxY(maxY),
      _yMajor(yMajor) {
    // empty
}

IntRange2D::IntRange2DIterator IntRange2D::begin() const {
    return IntRange2DIterator(this, /* end */ false);
}

bool IntRange2D::contains(int x, int y) const {
    return _minX <= x && x <= _maxX
            && _minY <= y && y <= _maxY;
}

bool IntRange2D::contains(const IntRange2D& r) const {
    return contains(r._minX, r._minY) && contains(r._maxX, r._maxY);
}

bool IntRange2D::empty() const {
    return _maxX < _minX || _maxY < _minY;
}

IntRange2D::IntRange2DIterator IntRange2D::end() const {
    return IntRange2DIterator(this, /* end */ true);
}

int IntRange2D::height() const {
    if (_maxY < _minY) {
        return 0;
    } else {
        return _maxY - _minY + 1;
    }
}

//bool IntRange2D::intersects(const IntRange2D& r) const {
//    // TODO
//}

bool IntRange2D::isEmpty() const {
    return _maxX < _minX || _maxY < _minY;
}

bool IntRange2D::isYMajor() const {
    return _yMajor;
}

int IntRange2D::maxX() const {
    return _maxX;
}

int IntRange2D::maxY() const {
    return _maxY;
}

int IntRange2D::minX() const {
    return _minX;
}

int IntRange2D::minY() const {
    return _minY;
}

int IntRange2D::size() const {
    return width() * height();
}

std::string IntRange2D::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int IntRange2D::width() const {
    if (_maxX < _minX) {
        return 0;
    } else {
        return _maxX - _minX + 1;
    }
}

int hashCode(const IntRange2D& r) {
    return hashCode(r.minX(), r.minY(), r.maxX(), r.maxY());
}

IntRange2D range2d(int width, int height, bool yMajor) {
    return IntRange2D(width, height, yMajor);
}

IntRange2D range2d(int minX, int minY, int maxX, int maxY, bool yMajor) {
    return IntRange2D(minX, minY, maxX, maxY, yMajor);
}

bool operator <(const IntRange2D& r1, const IntRange2D& r2) {
    if (r1.minX() != r2.minX()) {
        return r1.minX() < r2.minX();
    } else if (r1.minY() != r2.minY()) {
        return r1.minY() < r2.minY();
    } else if (r1.maxX() != r2.maxX()) {
        return r1.maxX() < r2.maxX();
    } else if (r1.maxY() != r2.maxY()) {
        return r1.maxY() < r2.maxY();
    } else if (r1.isYMajor() != r2.isYMajor()) {
        return (int) r1.isYMajor() < (int) r2.isYMajor();
    } else {
        return false;
    }
}

bool operator <=(const IntRange2D& r1, const IntRange2D& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator ==(const IntRange2D& r1, const IntRange2D& r2) {
    return r1.minX() == r2.minX()
            && r1.minY() == r2.minY()
            && r1.maxX() == r2.maxX()
            && r1.maxY() == r2.maxY()
            && r1.isYMajor() == r2.isYMajor();
}

bool operator !=(const IntRange2D& r1, const IntRange2D& r2) {
    return !(r1 == r2);
}

bool operator >(const IntRange2D& r1, const IntRange2D& r2) {
    return r2 < r1;
}

bool operator >=(const IntRange2D& r1, const IntRange2D& r2) {
    return !(r1 < r2);
}

std::ostream& operator <<(std::ostream& out, const IntRange2D& r) {
    return out << "["
               << r.minX() << "," << r.minY()
               << " .. "
               << r.maxX() << "," << r.maxY()
               << "]";
}

std::istream& operator >>(std::istream& input, IntRange2D& r) {
    // read '['
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read minX
    int minX;
    if (!(input >> minX)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ','
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read minY
    int minY;
    if (!(input >> minY)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ' .. '
    input.get();
    input.get();
    input.get();
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read maxX
    int maxX;
    if (!(input >> maxX)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ','
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read maxY
    int maxY;
    if (!(input >> maxY)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read ']'
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // success!
    r._minX = minX;
    r._minY = minY;
    r._maxX = maxX;
    r._maxY = maxY;
    return input;
}

/*
 * File: gchooser.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gchooser.cpp to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gchooser.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GChooser::GChooser(QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqcomboBox = new _Internal_QComboBox(this, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GChooser::GChooser(const std::initializer_list<std::string>& items, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqcomboBox = new _Internal_QComboBox(this, getInternalParent(parent));
    });
    addItems(items);
    setVisible(false);   // all widgets are not shown until added to a window
}

GChooser::GChooser(const Vector<std::string>& items, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqcomboBox = new _Internal_QComboBox(this, getInternalParent(parent));
    });
    addItems(items);
    setVisible(false);   // all widgets are not shown until added to a window
}

GChooser::~GChooser() {
    // TODO: delete _iqcomboBox;
    _iqcomboBox = nullptr;
}

void GChooser::addItem(const std::string& item) {
    require::nonEmpty(item, "GChooser::addItem", "item");
    GThread::runOnQtGuiThread([this, item]() {
        _iqcomboBox->addItem(QString::fromStdString(item));
    });
}

void GChooser::addItems(const std::initializer_list<std::string>& items) {
    GThread::runOnQtGuiThread([this, &items]() {
        for (const std::string& item : items) {
            if (!item.empty()) {
                _iqcomboBox->addItem(QString::fromStdString(item));
            }
        }
    });
}

void GChooser::addItems(const Vector<std::string>& items) {
    GThread::runOnQtGuiThread([this, &items]() {
        for (const std::string& item : items) {
            if (!item.empty()) {
                _iqcomboBox->addItem(QString::fromStdString(item));
            }
        }
    });
}

void GChooser::checkIndex(const std::string& member, int index, int min, int max) const {
    if (max < 0) {
        max = size() - 1;
    }
    require::inRange(index, min, max, member, "index");
}

void GChooser::clearItems() {
    GThread::runOnQtGuiThread([this]() {
        _iqcomboBox->clear();
    });
}

std::string GChooser::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getSelectedItem();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GChooser::getInternalWidget() const {
    return _iqcomboBox;
}

std::string GChooser::getItem(int index) const {
    checkIndex("GChooser::getItem", index);
    return _iqcomboBox->itemText(index).toStdString();
}

int GChooser::getItemCount() const {
    return _iqcomboBox->count();
}

int GChooser::getSelectedIndex() const {
    return _iqcomboBox->currentIndex();
}

std::string GChooser::getSelectedItem() const {
    return _iqcomboBox->currentText().toStdString();
}

std::string GChooser::getType() const {
    return "GChooser";
}

QWidget* GChooser::getWidget() const {
    return static_cast<QWidget*>(_iqcomboBox);
}

bool GChooser::isEditable() const {
    return _iqcomboBox->isEditable();
}

bool GChooser::isEmpty() const {
    return getItemCount() == 0;
}

void GChooser::removeActionListener() {
    removeEventListener("change");
}

void GChooser::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GChooser::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GChooser::setItem(int index, const std::string& item) {
    checkIndex("GChooser::setItem", index);
    GThread::runOnQtGuiThread([this, index, item]() {
        _iqcomboBox->setItemText(index, QString::fromStdString(item));
    });
}

void GChooser::setSelectedIndex(int index) {
    checkIndex("GChooser::setSelectedIndex", index);
    GThread::runOnQtGuiThread([this, index]() {
        _iqcomboBox->setCurrentIndex(index);
    });
}

void GChooser::setEditable(bool editable) {
    GThread::runOnQtGuiThread([this, editable]() {
        _iqcomboBox->setEditable(editable);
    });
}

void GChooser::setSelectedItem(const std::string& item) {
    for (int i = 0, len = getItemCount(); i < len; i++) {
        std::string thisItem = _iqcomboBox->itemText(i).toStdString();
        if (thisItem == item) {
            setSelectedIndex(i);
            break;
        }
    }
}

int GChooser::size() const {
    return _iqcomboBox->count();
}


_Internal_QComboBox::_Internal_QComboBox(GChooser* gchooser, QWidget* parent)
        : QComboBox(parent),
          _gchooser(gchooser) {
    require::nonNull(gchooser, "_Internal_QComboBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QComboBox_" + std::to_string(gchooser->getID())));
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(handleChange()));
}

void _Internal_QComboBox::handleChange() {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gchooser);
    changeEvent.setActionCommand(_gchooser->getActionCommand());
    _gchooser->fireEvent(changeEvent);
}

QSize _Internal_QComboBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QComboBox::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gchooser.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gtextfield.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gtextfield.cpp to replace Java version
 * @version 2018/06/29
 * - added textChange event
 * - added autocompletion
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gtextfield.h"
#include <QCompleter>
#include <QFontMetrics>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GTextField::GTextField(const std::string& text, int charsWide, QWidget* parent)
        : _iqlineedit(nullptr),
          _iqspinbox(nullptr),
          _iqdoublespinbox(nullptr) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
        _inputType = GTextField::INPUT_TYPE_TEXT;
    });
    if (!text.empty()) {
        setText(text);
    }
    if (charsWide > 0) {
        setCharsWide(charsWide);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::GTextField(int charsWide, QWidget* parent)
        : _iqlineedit(nullptr),
          _iqspinbox(nullptr),
          _iqdoublespinbox(nullptr) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlineedit = new _Internal_QLineEdit(this, getInternalParent(parent));
        _inputType = GTextField::INPUT_TYPE_TEXT;
    });
    if (charsWide > 0) {
        setCharsWide(charsWide);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::GTextField(int value, int min, int max, int step, QWidget* parent) {
    require::require(min <= max, "GTextField::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GTextField::constructor", "value");
    GThread::runOnQtGuiThread([this, value, min, max, step, parent]() {
        _iqspinbox = new _Internal_QSpinBox(this, min, max, step, getInternalParent(parent));
        _iqspinbox->setValue(value);
        _inputType = GTextField::INPUT_TYPE_INTEGER;
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::GTextField(double value, double min, double max, double step, QWidget* parent) {
    require::require(min <= max, "GTextField::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GTextField::constructor", "value");
    GThread::runOnQtGuiThread([this, value, min, max, step, parent]() {
        _iqdoublespinbox = new _Internal_QDoubleSpinBox(this, min, max, step, getInternalParent(parent));
        _iqdoublespinbox->setValue(value);
        _inputType = GTextField::INPUT_TYPE_REAL;
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextField::~GTextField() {
    // TODO: delete _iqlineedit;
    _iqlineedit = nullptr;
    _iqspinbox = nullptr;
    _iqdoublespinbox = nullptr;
}

int GTextField::getCharsWide() const {
    QFontMetrics fm(getWidget()->font());
    int mWidth = fm.width(QString::fromStdString("m"));
    return (int) (getWidth() / mWidth);
}

GTextField::InputType GTextField::getInputType() const {
    return _inputType;
}

_Internal_QWidget* GTextField::getInternalWidget() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit;
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox;
    } else {
        return _iqdoublespinbox;
    }
}

int GTextField::getMaxLength() const {
    int maxLength = _iqlineedit->maxLength();

    // Qt has default max text length of 32767
    maxLength = maxLength == 32767 ? 0 : maxLength;

    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        // empty
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        std::string maxStr = std::to_string(_iqspinbox->maximum());
        maxLength = std::max(maxLength, (int) maxStr.length());
    } else {
        std::string maxStr = std::to_string(_iqdoublespinbox->maximum());
        maxLength = std::max(maxLength, (int) maxStr.length());   // TODO: may be incorrect w/ decimal value
    }

    return maxLength;
}

std::string GTextField::getPlaceholder() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->placeholderText().toStdString();
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->placeholderText().toStdString();
    } else {
        return _iqdoublespinbox->lineEdit()->placeholderText().toStdString();
    }
}

std::string GTextField::getText() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->text().toStdString();
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->text().toStdString();
    } else {
        return _iqdoublespinbox->lineEdit()->text().toStdString();
    }
}

std::string GTextField::getType() const {
    return "GTextField";
}

std::string GTextField::getValue() const {
    return getText();
}

bool GTextField::getValueAsBool() const {
    std::string text = trim(getText());
    return stringToBool(text);
}

char GTextField::getValueAsChar() const {
    std::string text = getText();
    if (text.empty()) {
        return '\0';
    } else {
        return text[0];
    }
}

double GTextField::getValueAsDouble() const {
    std::string text = trim(getText());
    return stringToDouble(text);
}

int GTextField::getValueAsInt() const {
    return getValueAsInteger();
}

int GTextField::getValueAsInteger() const {
    std::string text = trim(getText());
    return stringToInteger(text);
}

QWidget* GTextField::getWidget() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return static_cast<QWidget*>(_iqlineedit);
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return static_cast<QWidget*>(_iqspinbox);
    } else {
        return static_cast<QWidget*>(_iqdoublespinbox);
    }
}

bool GTextField::isAutocompleteEnabled() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return _iqlineedit->completer() != nullptr;
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return _iqspinbox->lineEdit()->completer() != nullptr;
    } else {
        return _iqdoublespinbox->lineEdit()->completer() != nullptr;
    }
}

bool GTextField::isEditable() const {
    if (_inputType == GTextField::INPUT_TYPE_TEXT) {
        return !_iqlineedit->isReadOnly();
    } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
        return !_iqspinbox->lineEdit()->isReadOnly();
    } else {
        return !_iqdoublespinbox->lineEdit()->isReadOnly();
    }
}

void GTextField::removeActionListener() {
    removeEventListener("action");
}

void GTextField::removeTextChangeListener() {
    removeEventListener("textchange");
}

void GTextField::setActionListener(GEventListener func) {
    setEventListener("action", func);
}

void GTextField::setActionListener(GEventListenerVoid func) {
    setEventListener("action", func);
}

void GTextField::setAutocompleteList(std::initializer_list<std::string> strings) {
    Vector<std::string> v(strings);
    setAutocompleteList(v);
}

void GTextField::setAutocompleteList(const Vector<std::string>& strings) {
    GThread::runOnQtGuiThread([this, &strings]() {
        QStringList stringList;
        for (std::string s : strings) {
            if (!s.empty()) {
                stringList.push_back(QString::fromStdString(s));
            }
        }
        QStringListModel* model = new QStringListModel(stringList, _iqlineedit);   // TODO: MEMORY LEAK
        QCompleter* completer = new QCompleter(model, _iqlineedit);                // TODO: MEMORY LEAK
        completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setCompletionMode(QCompleter::PopupCompletion);

        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setCompleter(completer);
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setCompleter(completer);
        } else {
            _iqdoublespinbox->lineEdit()->setCompleter(completer);
        }
    });
}

void GTextField::setAutocompleteEnabled(bool enabled) {
    GThread::runOnQtGuiThread([this, enabled]() {
        if (!enabled) {
            if (_inputType == GTextField::INPUT_TYPE_TEXT) {
                _iqlineedit->setCompleter(nullptr);
            } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
                _iqspinbox->lineEdit()->setCompleter(nullptr);
            } else {
                _iqdoublespinbox->lineEdit()->setCompleter(nullptr);
            }
        }
    });
    // TODO: ability to set back to false
}

void GTextField::setCharsWide(int charsWide) {
    require::nonNegative(charsWide, "GTextField::setCharsWide");
    GThread::runOnQtGuiThread([this, charsWide]() {
        QFontMetrics fm(getWidget()->font());
        int mWidth = fm.width(QString::fromStdString("m"));
        getWidget()->setFixedWidth(mWidth * charsWide);
        getWidget()->updateGeometry();
    });
}

void GTextField::setEditable(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setReadOnly(!value);
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->setReadOnly(!value);
        } else {
            _iqdoublespinbox->setReadOnly(!value);
        }
    });
}

void GTextField::setMaxLength(int maxLength) {
    GThread::runOnQtGuiThread([this, maxLength]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setMaxLength(maxLength);
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setMaxLength(maxLength);
        } else {
            _iqdoublespinbox->lineEdit()->setMaxLength(maxLength);
        }
    });
}

void GTextField::setPlaceholder(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setPlaceholderText(QString::fromStdString(text));
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setPlaceholderText(QString::fromStdString(text));
        } else {
            _iqdoublespinbox->lineEdit()->setPlaceholderText(QString::fromStdString(text));
        }
    });
}

void GTextField::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        if (_inputType == GTextField::INPUT_TYPE_TEXT) {
            _iqlineedit->setText(QString::fromStdString(text));
        } else if (_inputType == GTextField::INPUT_TYPE_INTEGER) {
            _iqspinbox->lineEdit()->setText(QString::fromStdString(text));
        } else {
            _iqdoublespinbox->lineEdit()->setText(QString::fromStdString(text));
        }
    });
}

void GTextField::setTextChangeListener(GEventListener func) {
    setEventListener("textchange", func);
}

void GTextField::setTextChangeListener(GEventListenerVoid func) {
    setEventListener("textchange", func);
}

void GTextField::setValue(bool value) {
    setText(boolToString(value));
}

void GTextField::setValue(char value) {
    setText(charToString(value));
}

void GTextField::setValue(double value) {
    setText(std::to_string(value));
}

void GTextField::setValue(int value) {
    setText(std::to_string(value));
}

void GTextField::setValue(const std::string& value) {
    setText(value);
}

bool GTextField::valueIsBool() const {
    return stringIsBool(trim(getText()));
}

bool GTextField::valueIsChar() const {
    return (int) getText().length() == 1;
}

bool GTextField::valueIsDouble() const {
    return stringIsDouble(trim(getText()));
}

bool GTextField::valueIsInt() const {
    return stringIsInteger(trim(getText()));
}

bool GTextField::valueIsInteger() const {
    return stringIsInteger(trim(getText()));
}

bool GTextField::valueIsReal() const {
    return stringIsReal(trim(getText()));
}


_Internal_QLineEdit::_Internal_QLineEdit(GTextField* gtextField, QWidget* parent)
        : QLineEdit(parent),
          _gtextfield(gtextField) {
    require::nonNull(gtextField, "_Internal_QLineEdit::constructor");
    setObjectName(QString::fromStdString("_Internal_QLineEdit_" + std::to_string(gtextField->getID())));
    connect(this, SIGNAL(textChanged(QString)), this, SLOT(handleTextChange(const QString&)));
}

void _Internal_QLineEdit::handleTextChange(const QString&) {
    GEvent textChangeEvent(
                /* class  */ KEY_EVENT,
                /* type   */ KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _gtextfield);
    textChangeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(textChangeEvent);
}

void _Internal_QLineEdit::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QLineEdit::keyPressEvent", "event");
    QLineEdit::keyPressEvent(event);   // call super
    if (!_gtextfield->isAcceptingEvent("action")) return;
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        GEvent actionEvent(
                    /* class  */ ACTION_EVENT,
                    /* type   */ ACTION_PERFORMED,
                    /* name   */ "action",
                    /* source */ _gtextfield);
        actionEvent.setActionCommand(_gtextfield->getActionCommand());
        _gtextfield->fireEvent(actionEvent);
    }
}

QSize _Internal_QLineEdit::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QLineEdit::sizeHint();
    }
}

_Internal_QSpinBox::_Internal_QSpinBox(GTextField* gtextField, int min, int max, int step, QWidget* parent)
        : QSpinBox(parent),
          _gtextfield(gtextField) {
    require::nonNull(gtextField, "_Internal_QSpinBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QSpinBox_" + std::to_string(gtextField->getID())));
    setRange(min, max);
    setSingleStep(step);
}

void _Internal_QSpinBox::handleTextChange(const QString&) {
    GEvent textChangeEvent(
                /* class  */ KEY_EVENT,
                /* type   */ KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _gtextfield);
    textChangeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(textChangeEvent);
}

QLineEdit* _Internal_QSpinBox::lineEdit() const {
    return QSpinBox::lineEdit();
}

QSize _Internal_QSpinBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSpinBox::sizeHint();
    }
}

_Internal_QDoubleSpinBox::_Internal_QDoubleSpinBox(GTextField* gtextField, double min, double max, double step, QWidget* parent)
        : QDoubleSpinBox(parent),
          _gtextfield(gtextField) {
    require::nonNull(gtextField, "_Internal_QDoubleSpinBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QDoubleSpinBox_" + std::to_string(gtextField->getID())));
    setRange(min, max);
    setSingleStep(step);
}

void _Internal_QDoubleSpinBox::handleTextChange(const QString&) {
    GEvent textChangeEvent(
                /* class  */ KEY_EVENT,
                /* type   */ KEY_TYPED,
                /* name   */ "textchange",
                /* source */ _gtextfield);
    textChangeEvent.setActionCommand(_gtextfield->getActionCommand());
    _gtextfield->fireEvent(textChangeEvent);
}

QLineEdit* _Internal_QDoubleSpinBox::lineEdit() const {
    return QDoubleSpinBox::lineEdit();
}

QSize _Internal_QDoubleSpinBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QDoubleSpinBox::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gtextfield.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: geventqueue.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to geventqueue.cpp
 * @version 2018/07/03
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#include <QEvent>
#include <QThread>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32

GEventQueue* GEventQueue::_instance = nullptr;

GEventQueue::GEventQueue()
        : _eventMask(0) {
    // empty
}

GThunk GEventQueue::dequeue() {
    _functionQueueMutex.lockForWrite();
    GThunk thunk = _functionQueue.dequeue();
    _functionQueueMutex.unlock();
    return thunk;
}

void GEventQueue::enqueueEvent(const GEvent& event) {
    if (isAcceptingEvent(event.getEventClass())) {
        _eventQueueMutex.lockForWrite();
        _eventQueue.enqueue(event);
        _eventQueueMutex.unlock();
    }
}

int GEventQueue::getEventMask() const {
    return _eventMask;
}

GEvent GEventQueue::getNextEvent(int mask) {
    setEventMask(mask);

    // check if any events have arrived
    _eventQueueMutex.lockForRead();
    bool empty = _eventQueue.isEmpty();
    _eventQueueMutex.unlock();

    if (!empty) {
        // grab the event and return it
        _eventQueueMutex.lockForWrite();
        while (!_eventQueue.isEmpty()) {
            GEvent event = _eventQueue.dequeue();
            if (isAcceptingEvent(event)) {
                _eventQueueMutex.unlock();
                return event;
            }
        }
        _eventQueueMutex.unlock();
    }

    GEvent bogusEvent;
    return bogusEvent;
}

GEventQueue* GEventQueue::instance() {
    if (!_instance) {
        _instance = new GEventQueue();
    }
    return _instance;
}

bool GEventQueue::isAcceptingEvent(const GEvent& event) const {
    return isAcceptingEvent(event.getEventClass());
}

bool GEventQueue::isAcceptingEvent(int eventClass) const {
    return (_eventMask & eventClass) != 0;
}

bool GEventQueue::isEmpty() const {
    return _functionQueue.isEmpty();
}

GThunk GEventQueue::peek() {
    _functionQueueMutex.lockForRead();
    GThunk thunk = _functionQueue.peek();
    _functionQueueMutex.unlock();
    return thunk;
}

void GEventQueue::runOnQtGuiThreadAsync(GThunk thunk) {
    _functionQueueMutex.lockForWrite();
    _functionQueue.add(thunk);
    _functionQueueMutex.unlock();
    emit eventReady();
}

void GEventQueue::runOnQtGuiThreadSync(GThunk thunk) {
    _functionQueueMutex.lockForWrite();
    _functionQueue.add(thunk);
    _functionQueueMutex.unlock();
    emit eventReady();

    // TODO: "empty" is not quite right condition
    while (true) {
        _functionQueueMutex.lockForRead();
        bool empty = _functionQueue.isEmpty();
        _functionQueueMutex.unlock();
        if (empty) {
            break;
        } else {
            GThread::sleep(1);
        }
    }
}

void GEventQueue::setEventMask(int mask) {
    _eventMask = mask;
}

GEvent GEventQueue::waitForEvent(int mask) {
    setEventMask(mask);
    while (true) {
        // check if any events have arrived
        _eventQueueMutex.lockForRead();
        bool empty = _eventQueue.isEmpty();
        _eventQueueMutex.unlock();

        if (!empty) {
            // grab the event and return it
            _eventQueueMutex.lockForWrite();
            while (!_eventQueue.isEmpty()) {
                GEvent event = _eventQueue.dequeue();
                if (isAcceptingEvent(event)) {
                    _eventQueueMutex.unlock();
                    return event;
                }
            }
            _eventQueueMutex.unlock();
        }

        GThread::sleep(1);
    }
}

GEvent getNextEvent(int mask) {
    return GEventQueue::instance()->getNextEvent(mask);
}

GMouseEvent waitForClick() {
    return GEventQueue::instance()->waitForEvent(CLICK_EVENT);
}

GEvent waitForEvent(int mask) {
    return GEventQueue::instance()->waitForEvent(mask);
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_geventqueue.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gbrowserpane.cpp
 * ----------------------
 * This file contains the implementation of the <code>GBrowserPane</code> class
 * as declared in gbrowserpane.h.
 *
 * @version 2018/12/28
 * - added methods for text selection, scrolling, cursor position, key/mouse listeners
 * @version 2018/09/17
 * - fixed thread safety bugs
 * - added link listener events
 * @version 2018/08/23
 * - renamed to gbrowserpane.h to replace Java version
 * @version 2018/07/15
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gbrowserpane.h"
#include <QScrollBar>
#include <QTextCursor>
#include <fstream>
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "server.h"
#undef INTERNAL_INCLUDE

GBrowserPane::GBrowserPane(const std::string& url, QWidget* parent) {
    GThread::runOnQtGuiThread([this, url, parent]() {
        _iqtextbrowser = new _Internal_QTextBrowser(this, getInternalParent(parent));
    });
    if (!url.empty()) {
        readTextFromUrl(url);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GBrowserPane::~GBrowserPane() {
    // TODO: delete _iqtextbrowser;
    _iqtextbrowser = nullptr;
}

void GBrowserPane::clearSelection() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.clearSelection();
        _iqtextbrowser->setTextCursor(cursor);
    });
}

void GBrowserPane::clearText() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextbrowser->clear();
    });
}

std::string GBrowserPane::getContentType() const {
    return _contentType;
}

int GBrowserPane::getCursorPosition() const {
    return _iqtextbrowser->textCursor().position();
}

_Internal_QWidget* GBrowserPane::getInternalWidget() const {
    return _iqtextbrowser;
}

std::string GBrowserPane::getPageUrl() const {
    return _pageUrl;
}

std::string GBrowserPane::getSelectedText() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return getText().substr(start, end - start);
    } else {
        return "";
    }
}

int GBrowserPane::getSelectionEnd() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return end;
    } else {
        // no selection; cursor sets selection start/end to be equal
        return -1;
    }
}

int GBrowserPane::getSelectionLength() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    return end - start;
}

int GBrowserPane::getSelectionStart() const {
    QTextCursor cursor = _iqtextbrowser->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return start;
    } else {
        return -1;
    }
}

std::string GBrowserPane::getText() const {
    return _iqtextbrowser->toHtml().toStdString();
}

std::string GBrowserPane::getType() const {
    return "GBrowserPane";
}

QWidget* GBrowserPane::getWidget() const {
    return static_cast<QWidget*>(_iqtextbrowser);
}

bool GBrowserPane::isEditable() const {
    return !_iqtextbrowser->isReadOnly();
}

bool GBrowserPane::isLineWrap() const {
    return _iqtextbrowser->lineWrapMode() != QTextEdit::NoWrap;
}

void GBrowserPane::moveCursorToEnd() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        _iqtextbrowser->setTextCursor(cursor);
        _iqtextbrowser->ensureCursorVisible();
    });
}

void GBrowserPane::moveCursorToStart() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        _iqtextbrowser->setTextCursor(cursor);
        _iqtextbrowser->ensureCursorVisible();
    });
}

void GBrowserPane::readTextFromFile(std::istream& file) {
    std::string fileText = readEntireStream(file);
    setText(fileText);
}

void GBrowserPane::readTextFromFile(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str());
    if (!input.fail()) {
        _pageUrl = filename;
        std::string extension = getExtension(filename);
        std::string contentType = HttpServer::getContentType(extension);
        if (!contentType.empty()) {
            setContentType(contentType);
        }
        readTextFromFile(input);
    }
}

void GBrowserPane::readTextFromUrl(const std::string& url) {
    this->_pageUrl = url;
    GThread::runOnQtGuiThread([this, url]() {
        QUrl qurl(QString::fromStdString(url));
        _iqtextbrowser->setSource(qurl);
    });
}

void GBrowserPane::removeKeyListener() {
    removeEventListeners({"keypress",
                          "keyrelease",
                          "keytype"});
}

void GBrowserPane::removeLinkListener() {
    removeEventListener("linkclick");
}

void GBrowserPane::removeMouseListener() {
    removeEventListeners({"mousepress",
                          "mouserelease"});
}

void GBrowserPane::removeTextChangeListener() {
    removeEventListener("textchange");
}

void GBrowserPane::scrollToBottom() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextbrowser->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
        scrollbar->setSliderPosition(scrollbar->maximum());
    });
}

void GBrowserPane::scrollToTop() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextbrowser->verticalScrollBar();
        scrollbar->setValue(0);
        scrollbar->setSliderPosition(0);
    });
}

void GBrowserPane::select(int startIndex, int length) {
    require::nonNegative(startIndex, 0, "GBrowserPane::select", "startIndex");
    require::nonNegative(length, 0, "GBrowserPane::select", "length");
    GThread::runOnQtGuiThread([this, startIndex, length]() {
        QTextCursor cursor = _iqtextbrowser->textCursor();
        cursor.setPosition(startIndex);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
        _iqtextbrowser->setTextCursor(cursor);
    });
}

void GBrowserPane::selectAll() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextbrowser->selectAll();
    });
}

void GBrowserPane::setContentType(const std::string& contentType) {
    _contentType = contentType;
}

void GBrowserPane::setCursorPosition(int index, bool keepAnchor) {
    require::nonNegative(index, "TextArea::setCursorPosition", "index");
    GThread::runOnQtGuiThread([this, index, keepAnchor]() {
        QTextCursor cursor(_iqtextbrowser->textCursor());
        cursor.setPosition(index, keepAnchor ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
        _iqtextbrowser->setTextCursor(cursor);
        _iqtextbrowser->ensureCursorVisible();
    });
}

void GBrowserPane::setEditable(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqtextbrowser->setReadOnly(!value);
    });
}

void GBrowserPane::setKeyListener(GEventListener func) {
    GThread::runOnQtGuiThread([this]() {
        _iqtextbrowser->setFocusPolicy(Qt::StrongFocus);
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GBrowserPane::setKeyListener(GEventListenerVoid func) {
    GThread::runOnQtGuiThread([this]() {
        _iqtextbrowser->setFocusPolicy(Qt::StrongFocus);
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GBrowserPane::setMouseListener(GEventListener func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GBrowserPane::setMouseListener(GEventListenerVoid func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GBrowserPane::setLineWrap(bool wrap) {
    GThread::runOnQtGuiThread([this, wrap]() {
        _iqtextbrowser->setLineWrapMode(wrap ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    });
}

void GBrowserPane::setLinkListener(GEventListener func) {
    setEventListener("linkclick", func);
}

void GBrowserPane::setLinkListener(GEventListenerVoid func) {
    setEventListener("linkclick", func);
}

void GBrowserPane::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqtextbrowser->setText(QString::fromStdString(text));
    });
}

void GBrowserPane::setTextChangeListener(GEventListener func) {
    setEventListener("textchange", func);
}

void GBrowserPane::setTextChangeListener(GEventListenerVoid func) {
    setEventListener("textchange", func);
}


_Internal_QTextBrowser::_Internal_QTextBrowser(GBrowserPane* gbrowserpane, QWidget* parent)
        : QTextBrowser(parent),
          _gbrowserpane(gbrowserpane) {
    require::nonNull(gbrowserpane, "_Internal_QTextBrowser::constructor");
    setObjectName(QString::fromStdString("_Internal_QTextBrowser_" + std::to_string(gbrowserpane->getID())));
}

QVariant _Internal_QTextBrowser::loadResource(int type, const QUrl &url) {
    // patch to work properly for data:... URLs
    if (type == QTextDocument::ImageResource && url.scheme() == QLatin1String("data")) {
        QRegExp regex("^image/[^;]+;base64,(.+)={0,2}$");
        if (regex.indexIn(url.path()) >= 0) {
            QImage img;
            if (img.loadFromData(QByteArray::fromBase64(regex.cap(1).toLatin1()))) {
                return QVariant::fromValue(img);
            }
        }
    }
    return QTextBrowser::loadResource(type, url);
}

void _Internal_QTextBrowser::mousePressEvent(QMouseEvent* event) {
    QTextBrowser::mousePressEvent(event);
    if (!_gbrowserpane->isAcceptingEvent("linkclick")) return;
    if (!(event->button() & Qt::LeftButton)) {
        return;
    }
    QString clickedAnchor = anchorAt(event->pos());
    if (clickedAnchor.isEmpty()) {
        return;
    }
    _clickedLink = clickedAnchor;
}

void _Internal_QTextBrowser::mouseReleaseEvent(QMouseEvent* event) {
    if (!_gbrowserpane->isAcceptingEvent("linkclick")) {
        QTextBrowser::mouseReleaseEvent(event);   // call super
        return;
    }
    if (!(event->button() & Qt::LeftButton)) {
        QTextBrowser::mouseReleaseEvent(event);   // call super
        return;
    }
    QString clickedAnchor = anchorAt(event->pos());
    if (clickedAnchor.isEmpty() || _clickedLink.isEmpty()
            || clickedAnchor != _clickedLink) {
        QTextBrowser::mouseReleaseEvent(event);   // call super
        return;
    }

    _clickedLink = QString::fromStdString("");   // clear

    GEvent linkEvent(
                /* class  */ HYPERLINK_EVENT,
                /* type   */ HYPERLINK_CLICKED,
                /* name   */ "linkclick",
                /* source */ _gbrowserpane);
    linkEvent.setButton(static_cast<int>(event->button()));
    linkEvent.setX(event->x());
    linkEvent.setY(event->y());
    linkEvent.setModifiers(event->modifiers());
    linkEvent.setRequestURL(clickedAnchor.toStdString());
    linkEvent.setActionCommand(_gbrowserpane->getActionCommand());
    linkEvent.setInternalEvent(event);
    _gbrowserpane->fireEvent(linkEvent);
}

QSize _Internal_QTextBrowser::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextBrowser::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gbrowserpane.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: ginputpanel.cpp
 * ---------------------
 * This file contains declarations of functions that can pop up an
 * "input panel" of easy clickable buttons on the screen.
 * See ginputpanel.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2018/08/28
 * - refactored from free functions to GInputPanel class
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#define INTERNAL_INCLUDE 1
#include "ginputpanel.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "xmlutils.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

GInputPanel* GInputPanel::_instance = nullptr;

GInputPanel* GInputPanel::instance() {
    if (!_instance) {
        _instance = new GInputPanel();
    }
    return _instance;
}

GInputPanel::GInputPanel()
        : _loaded(false) {
    // empty
}

void GInputPanel::addButton(const std::string& /*text*/, const std::string& /*input*/) {
    // TODO
}

void GInputPanel::addCategory(const std::string& /*name*/) {
    // TODO
}

void GInputPanel::removeCategory(const std::string& /*name*/) {
    // TODO
}

void GInputPanel::removeButton(const std::string& /*text*/) {
    // TODO
}

void GInputPanel::setVisible(bool /*value*/) {
    // TODO
}

bool GInputPanel::isLoaded() const {
    return _loaded;
}

void GInputPanel::load(const std::string& xmlFilename) {
    if (isLoaded() || !fileExists(xmlFilename)) {
        return;
    }

    _loaded = true;
    rapidxml::xml_node<>* doc = xmlutils::openXmlDocument(xmlFilename, "inputpanel");
    for (rapidxml::xml_node<>* category : xmlutils::getChildNodes(doc, "category")) {
        std::string categoryName = xmlutils::getAttribute(category, "name");
        addCategory(categoryName);
        for (rapidxml::xml_node<>* button : xmlutils::getChildNodes(category, "button")) {
            std::string text = xmlutils::getAttribute(button, "text");
            std::string input = text;
            if (xmlutils::hasAttribute(button, "input")) {
                input = xmlutils::getAttribute(button, "input");
                
                // preprocess text to allow stuff like \n, \\, &quot; from XML to C++
                input = stringReplace(input, "\\n", "\n");
                input = stringReplace(input, "\\t", "\t");
                input = stringReplace(input, "\\r", "\r");
                input = stringReplace(input, "\\f", "\f");
                input = stringReplace(input, "\\\\", "\\");
                input = stringReplace(input, "&nbsp;", " ");
                input = stringReplace(input, "&lt;", "<");
                input = stringReplace(input, "&gt;", ">");
                input = stringReplace(input, "&amp;", "&");
                input = stringReplace(input, "&quot;", "\"");
            }
            addButton(text, input);
        }
    }
}

/*
 * File: gfont.cpp
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/09/23
 * - added macro checks to improve compatibility with old Qt versions
 * @version 2018/09/14
 * - added boldFont, italicFont
 * @version 2018/08/23
 * - renamed to gfont.cpp to replace Java version
 * @version 2018/07/05
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gfont.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <QtGlobal>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

GFont::GFont() {
    // empty
}

void GFont::boldFont(GInteractor* interactor) {
    require::nonNull(interactor, "GFont::boldFont", "interactor");
    QFont newFont = deriveQFont(toQFont(interactor->getFont()), /* weight */ QFont::Bold);
    interactor->setFont(newFont);
}

void GFont::changeFontSize(GInteractor* interactor, int dsize) {
    require::nonNull(interactor, "GFont::changeFontSize", "interactor");
    QFont newFont = changeFontSize(toQFont(interactor->getFont()), dsize);
    interactor->setFont(newFont);
}

QFont GFont::changeFontSize(const QFont& font, int dsize) {
    int newSize = std::max(1, font.pointSize() + dsize);
    return QFont(font.family(), newSize, font.weight(), font.italic());
}

QFont GFont::deriveQFont(const QFont& font, QFont::Weight weight, int size) {
    return QFont(font.family(), size, weight, /* italic */ font.italic());
}

QFont GFont::deriveQFont(const QFont& font, const std::string& fontFamily, QFont::Weight weight, int size) {
    return QFont(QString::fromStdString(fontFamily), size, weight, /* italic */ font.italic());
}

QFont GFont::deriveQFont(const std::string& font, QFont::Weight weight, int size) {
    return deriveQFont(toQFont(font), weight, size);
}

QFont GFont::deriveQFont(const std::string& font, const std::string& fontFamily, QFont::Weight weight, int size) {
    return deriveQFont(toQFont(font), fontFamily, weight, size);
}

// http://doc.qt.io/qt-5/qfont.html#StyleHint-enum
QFont::StyleHint GFont::getStyleHint(const std::string& fontFamily) {
    std::string fontFamilyStr = toLowerCase(trim(fontFamily));
    if (fontFamilyStr == "arial"
            || fontFamilyStr == "helvetica"
            || fontFamilyStr == "sansserif"
            || fontFamilyStr == "sans-serif") {
        return QFont::Helvetica;
    } else if (fontFamilyStr == "serif"
            || fontFamilyStr == "times"
            || fontFamilyStr == "times roman"
            || fontFamilyStr == "times new roman") {
        return QFont::Times;
    } else if (fontFamilyStr == "courier"
            || fontFamilyStr == "courier new"
            || fontFamilyStr == "consolas"
            || fontFamilyStr == "monospace"
            || fontFamilyStr == "monospaced"
            || endsWith(fontFamilyStr, " mono")) {
        return QFont::Monospace;
    } else if (fontFamilyStr == "comic sans"
            || fontFamilyStr == "comic sans ms"
            || fontFamilyStr == "fantasy") {
        return QFont::Fantasy;
    } else if (fontFamilyStr == "cursive") {
        return QFont::Cursive;
    } else if (fontFamilyStr == "system") {
        return QFont::System;
    } else {
        return QFont::AnyStyle;
    }
}

void GFont::italicFont(GInteractor* interactor) {
    require::nonNull(interactor, "GFont::boldFont", "interactor");
    QFont oldFont = toQFont(interactor->getFont());
    QFont newFont(oldFont.family(), oldFont.pointSize(), oldFont.weight(), /* italic */ true);
    interactor->setFont(newFont);
}

// example font string:
// "Courier New-Bold-12"
std::string GFont::toFontString(const QFont& font) {
    std::ostringstream out;
    out << font.family().toStdString();

    // http://doc.qt.io/qt-5/qfont.html#Weight-enum
    bool dashPrinted = false;
    switch (font.weight()) {
        case QFont::DemiBold:
        case QFont::Bold:
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
        case QFont::ExtraBold:
#endif // QT_VERSION
        case QFont::Black:
            out << "-bold";
            dashPrinted = true;
            break;
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
        case QFont::Thin:
        case QFont::ExtraLight:
#endif // QT_VERSION
        case QFont::Light:
        case QFont::Normal:
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
        case QFont::Medium:
#endif // QT_VERSION
        default:
            // not bold
            break;
    }
    if (font.italic()) {
        if (!dashPrinted) {
            // dashPrinted = true;
            out << "-";
        }
        out << "italic";
    }

    out << "-" << font.pointSize();
    return out.str();
}

// example font string:
// "Courier New-Bold-12"
QFont GFont::toQFont(const std::string& fontString) {
    Vector<std::string> tokens = stringSplit(trim(fontString), "-");
    if (tokens.isEmpty()) {
        return QFont();
    }

    std::string fontFamily = "";
    bool fontBold = false;
    bool fontItalic = false;
    int fontSize = 12;   // 12pt standard font size

    if (!tokens.isEmpty()) {
        fontFamily = trim(tokens.removeFront());
        if (fontFamily == "*") {
            fontFamily = "SansSerif";
        }
    }
    for (int i = 0; i < 2; i++) {
        if (!tokens.isEmpty()) {
            // tokens 2-3 can be size-style or style-size
            std::string fontWeightStr = toLowerCase(trim(tokens.removeFront()));
            if (stringIsInteger(fontWeightStr)) {
                fontSize = stringToInteger(fontWeightStr);
            }
            if (stringContains(fontWeightStr, "bold")) {
                fontBold = true;
            }
            if (stringContains(fontWeightStr, "italic")) {
                fontItalic = true;
            }
        }
    }

    QFont font = QFont(QString::fromStdString(fontFamily),
                       fontSize,
                       fontBold ? QFont::Bold : QFont::Normal,
                       fontItalic);
    font.setStyleHint(getStyleHint(fontFamily));
    return font;
}

QFont GFont::toQFont(const QFont& basisFont, const std::string& fontString) {
    Vector<std::string> tokens = stringSplit(trim(fontString), "-");
    if (tokens.isEmpty()) {
        return QFont();
    }

    std::string fontFamily = "";
    bool fontBold = basisFont.bold();
    bool fontItalic = basisFont.italic();
    int fontSize = basisFont.pointSize();   // 12pt standard font size

    if (!tokens.isEmpty()) {
        fontFamily = trim(tokens.removeFront());
        if (fontFamily == "*") {
            fontFamily = basisFont.family().toStdString();
        }
    }
    for (int i = 0; i < 2; i++) {
        if (!tokens.isEmpty()) {
            // tokens 2-3 can be size-style or style-size
            std::string fontWeightStr = toLowerCase(trim(tokens.removeFront()));
            if (stringIsInteger(fontWeightStr)) {
                fontSize = stringToInteger(fontWeightStr);
            } else if (stringContains(fontWeightStr, "bold")) {
                fontBold = true;
            } else if (stringContains(fontWeightStr, "italic")) {
                fontItalic = true;
            }
        }
    }

    QFont font = QFont(QString::fromStdString(fontFamily),
                       fontSize,
                       fontBold ? QFont::Bold : QFont::Normal,
                       fontItalic);
    font.setStyleHint(getStyleHint(fontFamily));
    return font;
}

/*
 * File: gtypes.cpp
 * ----------------
 * This file implements the classes in the gtypes.h interface.
 *
 * @version 2018/07/14
 * - initial version, based on gtypes.cpp
 */

#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#include <cmath>
#include <sstream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gmath.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "point.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

/*
 * Implementation notes: GDimension class
 * --------------------------------------
 * The GDimension class itself is entirely straightforward.  The
 * relational operators compare the width first, followed by the height.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GDimension::GDimension(double width, double height)
        : _width(width),
          _height(height) {
    // empty
}

double GDimension::getWidth() const {
    return _width;
}

double GDimension::getHeight() const {
    return _height;
}

std::string GDimension::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GDimension& dim) {
    return os << "(" << dim._width << ", " << dim._height << ")";
}

bool operator ==(const GDimension& d1, const GDimension& d2) {
    return floatingPointEqual(d1._width, d2._width)
            && floatingPointEqual(d1._height, d2._height);
}

bool operator !=(const GDimension& d1, const GDimension& d2) {
    return !(d1 == d2);
}

bool operator <(const GDimension& d1, const GDimension& d2) {
    return d1._width < d2._width
            || (floatingPointEqual(d1._width, d2._width) && d1._height < d2._height);
}

bool operator <=(const GDimension& d1, const GDimension& d2) {
    return d1 < d2 || d1 == d2;
}

bool operator >(const GDimension& d1, const GDimension& d2) {
    return d2 < d1;
}

bool operator >=(const GDimension& d1, const GDimension& d2) {
    return d1 > d2 || d1 == d2;
}

GDimension operator *(const GDimension& d, double scale) {
    return GDimension(d._width * scale, d._height * scale);
}

int hashCode(const GDimension& dim) {
    return hashCode(dim._width, dim._height);
}

std::string toString(HorizontalAlignment alignment) {
    if (alignment == ALIGN_CENTER) {
        return "Center";
    } else if (alignment == ALIGN_LEFT) {
        return "Left";
    } else {   // if (alignment == ALIGN_RIGHT)
        return "Right";
    }
}

std::string toString(VerticalAlignment alignment) {
    if (alignment == ALIGN_MIDDLE) {
        return "Middle";
    } else if (alignment == ALIGN_TOP) {
        return "Top";
    } else {   // if (alignment == ALIGN_BOTTOM)
        return "Bottom";
    }
}

HorizontalAlignment toHorizontalAlignment(const std::string& alignmentStr) {
    std::string alignLC = toLowerCase(trim(alignmentStr));
    if (stringContains(alignLC, "left") || stringContains(alignLC, "west")) {
        return ALIGN_LEFT;
    } else if (stringContains(alignLC, "right") || stringContains(alignLC, "east")) {
        return ALIGN_RIGHT;
    } else if (stringContains(alignLC, "stretch") || stringContains(alignLC, "justify")) {
        return ALIGN_HORIZONTAL_STRETCH;
    } else {
        return ALIGN_CENTER;
    }
}

Qt::Alignment toQtAlignment(HorizontalAlignment alignment) {
    return alignment == ALIGN_LEFT ? Qt::AlignLeft
            : alignment == ALIGN_CENTER ? Qt::AlignHCenter
            : alignment == ALIGN_HORIZONTAL_STRETCH ? Qt::AlignJustify
            : Qt::AlignRight;
}

Qt::Alignment toQtAlignment(VerticalAlignment alignment) {
    return alignment == ALIGN_TOP ? Qt::AlignTop
            : alignment == ALIGN_MIDDLE ? Qt::AlignVCenter
            : alignment == ALIGN_VERTICAL_STRETCH ? Qt::AlignTop
            : Qt::AlignBottom;
}

VerticalAlignment toVerticalAlignment(const std::string& alignmentStr) {
    std::string alignLC = toLowerCase(trim(alignmentStr));
    if (stringContains(alignLC, "top") || stringContains(alignLC, "north")) {
        return ALIGN_TOP;
    } else if (stringContains(alignLC, "bottom") || stringContains(alignLC, "south")) {
        return ALIGN_BOTTOM;
    } else if (stringContains(alignLC, "stretch") || stringContains(alignLC, "justify")) {
        return ALIGN_VERTICAL_STRETCH;
    } else {
        return ALIGN_MIDDLE;
    }
}

/*
 * Implementation notes: GPoint class
 * ----------------------------------
 * The GPoint class itself is entirely straightforward.  The relational
 * operators compare the x components first, followed by the y component.
 * The hashCode function computes the exclusive-or of the individual words.
 */

GPoint::GPoint(double x, double y)
        : _x(x),
          _y(y) {
    // empty
}

GPoint::GPoint(const Point& point) {
    _x = point.getX();
    _y = point.getY();
}

double GPoint::getX() const {
    return _x;
}

double GPoint::getY() const {
    return _y;
}

std::string GPoint::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GPoint& pt) {
    return os << "(" << pt._x << ", " << pt._y << ")";
}

bool operator ==(const GPoint& p1, const GPoint& p2) {
    return floatingPointEqual(p1._x, p2._x)
            && floatingPointEqual(p1._y, p2._y);
}

bool operator !=(const GPoint& p1, const GPoint& p2) {
    return !(p1 == p2);
}

bool operator <(const GPoint& p1, const GPoint& p2) {
    return p1._x < p2._x || (floatingPointEqual(p1._x, p2._x) && p1._y < p2._y);
}

bool operator <=(const GPoint& p1, const GPoint& p2) {
    return p1 < p2 || p1 == p2;
}

bool operator >(const GPoint& p1, const GPoint& p2) {
    return p2 < p1;
}

bool operator >=(const GPoint& p1, const GPoint& p2) {
    return p1 > p2 || p1 == p2;
}

GPoint operator *(const GPoint& p, double scale) {
    return GPoint(p._x * scale, p._y * scale);
}

int hashCode(const GPoint& pt) {
    return hashCode(pt._x, pt._y);
}

/*
 * Implementation notes: GRectangle class
 * --------------------------------------
 * The GRectangle class itself is entirely straightforward.  The
 * relational operators compare the components in the following order:
 * x, y, width, height.  The hashCode function computes the exclusive-or
 * of the individual words.
 */

GRectangle::GRectangle(double x, double y, double width, double height)
        : _x(x),
          _y(y),
          _width(width),
          _height(height) {
    // empty
}

GRectangle::GRectangle(double x, double y, const GDimension& size)
        : _x(x),
          _y(y),
          _width(size.getWidth()),
          _height(size.getHeight()) {
    // empty
}

GRectangle::GRectangle(const GPoint& p, double width, double height)
        : _x(p.getX()),
          _y(p.getY()),
          _width(width),
          _height(height) {
    // empty
}

GRectangle::GRectangle(const GPoint& p, const GDimension& size)
        : _x(p.getX()),
          _y(p.getY()),
          _width(size.getWidth()),
          _height(size.getHeight()) {
    // empty
}

GRectangle GRectangle::enlargedBy(double amount) {
    return GRectangle(_x - amount, _y - amount, _width + 2 * amount, _height + 2 * amount);
}

double GRectangle::getX() const {
    return _x;
}

double GRectangle::getY() const {
    return _y;
}

double GRectangle::getWidth() const {
    return _width;
}

double GRectangle::getHeight() const {
    return _height;
}

bool GRectangle::isEmpty() const {
    return _width <= 0 || _height <= 0;
}

bool GRectangle::contains(double x, double y) const {
    return x >= this->_x && y >= this->_y
            && x < this->_x + _width
            && y < this->_y + _height;
}

bool GRectangle::contains(const GPoint& pt) const {
    return contains(pt.getX(), pt.getY());
}

std::string GRectangle::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& os, const GRectangle& rect) {
    return os << "(" << rect.getX() << ", " << rect.getY() << ", "
              << rect.getWidth() << ", " << rect.getHeight() << ")";
}

bool operator ==(const GRectangle& r1, const GRectangle& r2) {
    return floatingPointEqual(r1._x, r2._x)
            && floatingPointEqual(r1._y, r2._y)
            && floatingPointEqual(r1._width, r2._width)
            && floatingPointEqual(r1._height, r2._height);
}

bool operator !=(const GRectangle& r1, const GRectangle& r2) {
    return !(r1 == r2);
}

bool operator <(const GRectangle& r1, const GRectangle& r2) {
    return stanfordcpplib::collections::compareTo4(
            r1._x, r2._x, r1._y, r2._y, r1._width, r2._width, r1._height, r2._height) < 0;
}

bool operator <=(const GRectangle& r1, const GRectangle& r2) {
    return r1 < r2 || r1 == r2;
}

bool operator >(const GRectangle& r1, const GRectangle& r2) {
    return r2 < r1;
}

bool operator >=(const GRectangle& r1, const GRectangle& r2) {
    return r1 > r2 || r1 == r2;
}

int hashCode(const GRectangle& r) {
    return hashCode(r._x, r._y, r._width, r._height);
}

/*
 * File: gconsolewindow.cpp
 * ------------------------
 * This file implements the gconsolewindow.h interface.
 *
 * @author Marty Stepp
 * @version 2018/12/27
 * - bug fix for endless waitForEvent queued events caused by printing text
 *   to console (bug reported by Keith Schwarz)
 * @version 2018/10/11
 * - bug fixes for shutdown flag, input script hotkeys (e.g. Ctrl+1)
 * @version 2018/10/04
 * - changed default line wrap to true
 * @version 2018/09/27
 * - bug fix for printing strings with line breaks (remove \r, favor \n)
 * @version 2018/09/23
 * - added getFont
 * - bug fix for loading input scripts
 * - bug fix for default font on Mac
 * @version 2018/09/18
 * - window size/location fixes
 * @version 2018/09/17
 * - fixes for monospaced font on Mac OS X
 * @version 2018/08/23
 * - initial version, separated out from console.cpp
 */

#define INTERNAL_INCLUDE 1
#include "gconsolewindow.h"
#include <cstdio>
#include <QAction>
#include <QTextDocumentFragment>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gclipboard.h"
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gcolorchooser.h"
#define INTERNAL_INCLUDE 1
#include "gdiffgui.h"
#define INTERNAL_INCLUDE 1
#include "gdownloader.h"
#define INTERNAL_INCLUDE 1
#include "gfilechooser.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gfontchooser.h"
#define INTERNAL_INCLUDE 1
#include "goptionpane.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "os.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#define INTERNAL_INCLUDE 1
#include "private/version.h"
#undef INTERNAL_INCLUDE

void setConsolePropertiesQt();

const bool GConsoleWindow::GConsoleWindow::ALLOW_RICH_INPUT_EDITING = true;
const double GConsoleWindow::DEFAULT_WIDTH = 800;
const double GConsoleWindow::DEFAULT_HEIGHT = 500;
const double GConsoleWindow::DEFAULT_X = 10;
const double GConsoleWindow::DEFAULT_Y = 40;
const std::string GConsoleWindow::CONFIG_FILE_NAME = "spl-jar-settings.txt";
const std::string GConsoleWindow::DEFAULT_WINDOW_TITLE = "Console";
const std::string GConsoleWindow::DEFAULT_FONT_FAMILY = "Monospace";
const std::string GConsoleWindow::DEFAULT_FONT_WEIGHT = "";
const int GConsoleWindow::DEFAULT_FONT_SIZE = 12;
const int GConsoleWindow::MIN_FONT_SIZE = 4;
const int GConsoleWindow::MAX_FONT_SIZE = 255;
const std::string GConsoleWindow::DEFAULT_BACKGROUND_COLOR = "white";
const std::string GConsoleWindow::DEFAULT_ERROR_COLOR = "red";
const std::string GConsoleWindow::DEFAULT_OUTPUT_COLOR = "black";
const std::string GConsoleWindow::USER_INPUT_COLOR = "blue";
GConsoleWindow* GConsoleWindow::_instance = nullptr;
bool GConsoleWindow::_consoleEnabled = false;

/* static */ bool GConsoleWindow::consoleEnabled() {
    return _consoleEnabled;
}

/* static */ std::string GConsoleWindow::getDefaultFont() {
    if (OS::isMac()) {
        // for some reason, using "Monospace" doesn't work for me on Mac testing
        return "Menlo-"
                + std::to_string(DEFAULT_FONT_SIZE + 1)
                + (DEFAULT_FONT_WEIGHT.empty() ? "" : ("-" + DEFAULT_FONT_WEIGHT));
    } else {
        return DEFAULT_FONT_FAMILY
                + "-" + std::to_string(DEFAULT_FONT_SIZE)
                + (DEFAULT_FONT_WEIGHT.empty() ? "" : ("-" + DEFAULT_FONT_WEIGHT));
    }
}

/* static */ GConsoleWindow* GConsoleWindow::instance() {
    if (!_instance) {
        // initialize Qt system and Qt Console window
        GThread::runOnQtGuiThread([]() {
            if (!_instance) {
                QtGui::instance()->initializeQt();
                _instance = new GConsoleWindow();
                setConsolePropertiesQt();
            }
        });
    }
    return _instance;
}

/* static */ bool GConsoleWindow::isInitialized() {
    return _instance != nullptr;
}

/* static */ void GConsoleWindow::setConsoleEnabled(bool enabled) {
    _consoleEnabled = enabled;
}

GConsoleWindow::GConsoleWindow()
        : GWindow(/* visible */ false),
          _textArea(nullptr),
          _clearEnabled(true),
          _echo(false),
          _locationSaved(false),
          _locked(false),
          _promptActive(false),
          _shutdown(false),
          _commandHistoryIndex(-1),
          _errorColor(""),
          _outputColor(""),
          _inputBuffer(""),
          _lastSaveFileName(""),
          _cinout_new_buf(nullptr),
          _cerr_new_buf(nullptr),
          _cin_old_buf(nullptr),
          _cout_old_buf(nullptr),
          _cerr_old_buf(nullptr) {
    _initMenuBar();
    _initWidgets();
    _initStreams();
    loadConfiguration();
}

void GConsoleWindow::_initMenuBar() {
    const std::string ICON_FOLDER = "icons/";

    // File menu
    addMenu("&File");
    addMenuItem("File", "&Save", ICON_FOLDER + "save.gif",
                [this]() { this->save(); })
                ->setShortcut(QKeySequence::Save);

    addMenuItem("File", "Save &As...", ICON_FOLDER + "save_as.gif",
                [this]() { this->saveAs(); })
                ->setShortcut(QKeySequence::SaveAs);
    addMenuSeparator("File");

    addMenuItem("File", "&Print", ICON_FOLDER + "print.gif",
                [this]() { this->showPrintDialog(); })
                ->setShortcut(QKeySequence::Print);
    setMenuItemEnabled("File", "Print", false);
    addMenuSeparator("File");

    addMenuItem("File", "&Load Input Script...", ICON_FOLDER + "script.gif",
                [this]() { this->showInputScriptDialog(); });

    addMenuItem("File", "&Compare Output...", ICON_FOLDER + "compare_output.gif",
                [this]() { this->showCompareOutputDialog(); });

    addMenuItem("File", "&Quit", ICON_FOLDER + "quit.gif",
                [this]() { this->close(); /* TODO: exit app */ })
                ->setShortcut(QKeySequence::Quit);

    // Edit menu
    addMenu("&Edit");
    addMenuItem("Edit", "Cu&t", ICON_FOLDER + "cut.gif",
                [this]() { this->clipboardCut(); })
                ->setShortcut(QKeySequence::Cut);

    addMenuItem("Edit", "&Copy", ICON_FOLDER + "copy.gif",
                [this]() { this->clipboardCopy(); })
                ->setShortcut(QKeySequence::Copy);

    addMenuItem("Edit", "&Paste", ICON_FOLDER + "paste.gif",
                [this]() { this->clipboardPaste(); })
                ->setShortcut(QKeySequence::Paste);

    addMenuItem("Edit", "Select &All", ICON_FOLDER + "select_all.gif",
                [this]() { this->selectAll(); })
                ->setShortcut(QKeySequence::SelectAll);

    addMenuItem("Edit", "C&lear Console", ICON_FOLDER + "clear_console.gif",
                [this]() { this->clearConsole(); })
                ->setShortcut(QKeySequence(QString::fromStdString("Ctrl+L")));

    // Options menu
    addMenu("&Options");
    addMenuItem("Options", "&Font...", ICON_FOLDER + "font.gif",
                [this]() { this->showFontDialog(); });

    addMenuItem("Options", "&Background Color...", ICON_FOLDER + "background_color.gif",
                [this]() { this->showColorDialog(/* background */ true); });

    addMenuItem("Options", "&Text Color...", ICON_FOLDER + "text_color.gif",
                [this]() { this->showColorDialog(/* background */ false); });

    // Help menu
    addMenu("&Help");
    addMenuItem("Help", "&About...", ICON_FOLDER + "about.gif",
                [this]() { this->showAboutDialog(); })
                ->setShortcut(QKeySequence::HelpContents);

    addMenuItem("Help", "&Check for Updates", ICON_FOLDER + "check_for_updates.gif",
                [this]() { this->checkForUpdates(); });
}

void GConsoleWindow::_initStreams() {
    // buffer C-style stderr
    static char stderrBuf[BUFSIZ + 10] = {'\0'};
    std::ios::sync_with_stdio(false);
    setbuf(stderr, stderrBuf);

    // redirect cin/cout/cerr
    _cinout_new_buf = new stanfordcpplib::qtgui::ConsoleStreambufQt();
    _cerr_new_buf = new stanfordcpplib::qtgui::ConsoleStreambufQt(/* isStderr */ true);
    _cin_old_buf = std::cin.rdbuf(_cinout_new_buf);
    _cout_old_buf = std::cout.rdbuf(_cinout_new_buf);
    _cerr_old_buf = std::cerr.rdbuf(_cerr_new_buf);
}

void GConsoleWindow::_initWidgets() {
    _textArea = new GTextArea();
    _textArea->setColor("black");
    _textArea->setContextMenuEnabled(false);
    // _textArea->setEditable(false);
    _textArea->setLineWrap(true);
    _textArea->setFont(getDefaultFont());
    // _textArea->setRowsColumns(25, 70);
    QTextEdit* rawTextEdit = static_cast<QTextEdit*>(_textArea->getWidget());
    rawTextEdit->setTabChangesFocus(false);
    _textArea->setKeyListener([this](GEvent event) {
        if (event.getEventType() == KEY_PRESSED) {
            this->processKeyPress(event);
        } else if (event.getEventType() == KEY_RELEASED
                   || event.getEventType() == KEY_TYPED) {
            event.ignore();
        }
    });
    _textArea->setMouseListener([](GEvent event) {
        // snuff out mouse-based operations:
        // - popping up context menu by right-clicking
        // - Linux-style copy/paste operations using selection plus middle-click
        if (event.getButton() > 1
                || event.getEventType() == MOUSE_RELEASED) {
            event.ignore();
        }
    });
    addToRegion(_textArea, "Center");

    // tell window to shut down when it is closed
    setWindowListener([this](GEvent event) {
        if (event.getEventType() == WINDOW_CLOSING) {
            shutdown();
        }
    });

    setTitle(DEFAULT_WINDOW_TITLE);
    setCloseOperation(GWindow::CLOSE_HIDE);
    setLocation(DEFAULT_X, DEFAULT_Y);
    setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setVisible(true);
}


GConsoleWindow::~GConsoleWindow() {
    // TODO: delete?
    _cinout_new_buf = nullptr;
    _cerr_new_buf = nullptr;
    _cin_old_buf = nullptr;
    _cout_old_buf = nullptr;
    _cerr_old_buf = nullptr;
}

void GConsoleWindow::checkForUpdates() {
    GThread::runInNewThreadAsync([this]() {
        static const std::string CPP_ZIP_VERSION_URL = version::getCppLibraryDocsUrl() + "CURRENTVERSION_CPPLIB.txt";
        std::string currentVersion = version::getCppLibraryVersion();

        GDownloader downloader;
        std::string latestVersion = trim(downloader.downloadAsString(CPP_ZIP_VERSION_URL));

        if (latestVersion.empty()) {
            GOptionPane::showMessageDialog(
                    /* parent  */ getWidget(),
                    /* message */ "Unable to look up latest library version from web.",
                    /* title   */ "Network error",
                    /* type    */ GOptionPane::MESSAGE_ERROR);
            return;
        }

        std::string message;
        if (currentVersion >= latestVersion) {
                message = "This project already has the latest version \nof the Stanford libraries (" + currentVersion + ").";
        } else {
                message = "<html>There is an updated version of the Stanford libraries available.\n\n"
                   "This project's library version: " + currentVersion + "\n"
                   "Current newest library version: " + latestVersion + "\n\n"
                   "Go to <a href=\"" + version::getCppLibraryDocsUrl() + "\">"
                   + version::getCppLibraryDocsUrl() + "</a> to get the new version.</html>";
        }
        GOptionPane::showMessageDialog(
                    /* parent  */ getWidget(),
                    /* message */ message);
    }, "Check for Updates");
}

void GConsoleWindow::clearConsole() {
    std::string msg = "==================== (console cleared) ====================";
    if (_clearEnabled) {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());

        // clear the graphical console window
        _coutMutex.lock();
        _textArea->clearText();
        _coutMutex.unlock();
    } else {
        // don't actually clear the window, just display 'cleared' message on it
        println(msg);
    }
}

void GConsoleWindow::clipboardCopy() {
    std::string selectedText = _textArea->getSelectedText();
    if (!selectedText.empty()) {
        GClipboard::set(selectedText);
    }
}

void GConsoleWindow::clipboardCut() {
    if (_shutdown || !_promptActive || !ALLOW_RICH_INPUT_EDITING) {
        return;
    }

    // if selection is entirely within the user input area, cut out of user input area
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    int selectionStart = _textArea->getSelectionStart();
    int selectionEnd = _textArea->getSelectionEnd();
    if (selectionEnd > selectionStart
            && selectionStart >= userInputStart
            && selectionEnd <= userInputEnd) {
        // selection is entirely user input! cut it!
        QTextFragment frag = getUserInputFragment();
        if (frag.isValid()) {
            std::string selectedText = _textArea->getSelectedText();
            QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
            QTextCursor cursor(textArea->textCursor());

            int indexStart = selectionStart - userInputStart;
            int selectionLength = _textArea->getSelectionLength();
            _cinMutex.lockForWrite();
            _inputBuffer.erase(indexStart, selectionLength);
            cursor.beginEditBlock();
            cursor.removeSelectedText();
            cursor.endEditBlock();
            textArea->setTextCursor(cursor);
            _cinMutex.unlock();
            GClipboard::set(selectedText);
        }
    }
}

void GConsoleWindow::clipboardPaste() {
    if (_shutdown) {
        return;
    }

    _textArea->clearSelection();
    if (!isCursorInUserInputArea()) {
        _textArea->moveCursorToEnd();
    }

    std::string clipboardText = GClipboard::get();
    for (int i = 0; i < (int) clipboardText.length(); i++) {
        if (clipboardText[i] == '\r') {
            continue;
        } else if (clipboardText[i] == '\n') {
            processUserInputEnterKey();
        } else {
            processUserInputKey(clipboardText[i]);
        }
    }
}

void GConsoleWindow::close() {
    shutdown();
    GWindow::close();   // call super
}

void GConsoleWindow::compareOutput(const std::string& filename) {
    std::string expectedOutput;
    if (!filename.empty() && fileExists(filename)) {
        expectedOutput = readEntireFile(filename);
    } else {
        expectedOutput = "File not found: " + filename;
    }

    std::string studentOutput = getAllOutput();

    GDiffGui::showDialog("Expected Output", expectedOutput,
                         "Your Output", studentOutput,
                         /* showCheckBoxes */ false);
}

std::string GConsoleWindow::getAllOutput() const {
    GConsoleWindow* thisHack = const_cast<GConsoleWindow*>(this);
    thisHack->_coutMutex.lock();
    std::string allOutput = thisHack->_allOutputBuffer.str();
    thisHack->_coutMutex.unlock();
    return allOutput;
}

std::string GConsoleWindow::getBackground() const {
    return _textArea->getBackground();
}

int GConsoleWindow::getBackgroundInt() const {
    return _textArea->getBackgroundInt();
}

std::string GConsoleWindow::getColor() const {
    return getOutputColor();
}

int GConsoleWindow::getColorInt() const {
    return GColor::convertColorToRGB(getOutputColor());
}

std::string GConsoleWindow::getErrorColor() const {
    return _errorColor.empty() ? DEFAULT_ERROR_COLOR : _errorColor;
}

std::string GConsoleWindow::getFont() const {
    return _textArea->getFont();
}

std::string GConsoleWindow::getForeground() const {
    return getOutputColor();
}

int GConsoleWindow::getForegroundInt() const {
    return GColor::convertColorToRGB(getOutputColor());
}

std::string GConsoleWindow::getOutputColor() const {
    return _outputColor.empty() ? DEFAULT_OUTPUT_COLOR : _outputColor;
}

QTextFragment GConsoleWindow::getUserInputFragment() const {
    if (!_inputBuffer.empty()) {
        QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
        QTextBlock block = textArea->document()->end().previous();
        while (block.isValid()) {
            QTextBlock::iterator it;
            for (it = block.begin(); !(it.atEnd()); ++it) {
                QTextFragment frag = it.fragment();
                if (frag.isValid()) {
                    std::string fragText = frag.text().toStdString();

                    // see if it is the given user input
                    if (fragText == _inputBuffer) {
                        return frag;
                    }
                }
            }
            block = block.previous();
        }
    }

    // didn't find the fragment; this will return an 'invalid' fragment
    QTextFragment notFound;
    return notFound;
}

int GConsoleWindow::getUserInputStart() const {
    QTextFragment frag = getUserInputFragment();
    if (frag.isValid()) {
        return frag.position();
    } else if (_promptActive) {
        // at end of text
        return (int) _textArea->getText().length();
    } else {
        return -1;
    }
}

int GConsoleWindow::getUserInputEnd() const {
    QTextFragment frag = getUserInputFragment();
    if (frag.isValid()) {
        return frag.position() + frag.length();
    } else if (_promptActive) {
        // at end of text
        return (int) _textArea->getText().length();
    } else {
        return -1;
    }
}

bool GConsoleWindow::isClearEnabled() const {
    return _clearEnabled;
}

bool GConsoleWindow::isCursorInUserInputArea() const {
    int cursorPosition = _textArea->getCursorPosition();
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    return _promptActive
            && userInputStart <= cursorPosition
            && cursorPosition <= userInputEnd;
}

bool GConsoleWindow::isEcho() const {
    return _echo;
}

bool GConsoleWindow::isLocationSaved() const {
    return _locationSaved;
}

bool GConsoleWindow::isLocked() const {
    return _locked;
}

bool GConsoleWindow::isSelectionInUserInputArea() const {
    int userInputStart = getUserInputStart();
    int userInputEnd   = getUserInputEnd();
    int selectionStart = _textArea->getSelectionStart();
    int selectionEnd = _textArea->getSelectionEnd();
    return userInputStart >= 0 && userInputEnd >= 0
            && selectionStart >= userInputStart
            && selectionEnd <= userInputEnd;
}

void GConsoleWindow::loadConfiguration() {
    std::string configFile = getTempDirectory() + "/" + CONFIG_FILE_NAME;
    if (fileExists(configFile)) {
        std::ifstream infile;
        infile.open(configFile.c_str());
        if (!infile) {
                return;
        }
        std::string line;
        while (getline(infile, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }
            Vector<std::string> tokens = stringSplit(line, "=");
            if (tokens.size() < 2) {
                continue;
            }
            std::string key   = toLowerCase(tokens[0]);
            std::string value = tokens[1];
            if (key == "font") {
                setFont(value);
            } else if (key == "background") {
                setBackground(value);
            } else if (key == "foreground") {
                setForeground(value);
            }
        }
    }
}

void GConsoleWindow::loadInputScript(int number) {
    std::string sep = getDirectoryPathSeparator();
    static std::initializer_list<std::string> directoriesToCheck {
            ".",
            "." + sep + "input",
            "." + sep + "output"
    };
    std::string inputFile;
    std::string expectedOutputFile;
    for (std::string dir : directoriesToCheck) {
        if (!isDirectory(dir)) {
            continue;
        }

        for (std::string filename : listDirectory(dir)) {
            filename = dir + sep + filename;
            if (inputFile.empty()
                    && stringContains(filename, "input-" + std::to_string(number))
                    && endsWith(filename, ".txt")) {
                inputFile = filename;
            } else if (expectedOutputFile.empty()
                       && stringContains(filename, "expected-output-" + std::to_string(number))
                       && endsWith(filename, ".txt")) {
                expectedOutputFile = filename;
            }
        }
    }

    if (!_shutdown && !inputFile.empty()) {
        loadInputScript(inputFile);
        pause(50);
    }
    if (!expectedOutputFile.empty()) {
        GThread::runInNewThreadAsync([this, expectedOutputFile]() {
            pause(1000);
            compareOutput(expectedOutputFile);
        }, "Compare Output");
    }
}

void GConsoleWindow::loadInputScript(const std::string& filename) {
    if (_shutdown) {
        return;
    }
    if (!filename.empty() && fileExists(filename)) {
        std::ifstream infile;
        infile.open(filename.c_str());
        Vector<std::string> lines;
        readEntireFile(infile, lines);

        _cinQueueMutex.lockForWrite();
        _inputScript.clear();
        for (std::string line : lines) {
            _inputScript.enqueue(line);
        }
        _cinQueueMutex.unlock();
    }
}

void GConsoleWindow::print(const std::string& str, bool isStdErr) {
    if (_echo) {
        fflush(isStdErr ? stdout : stderr);
        fflush(isStdErr ? stderr : stdout);
        fprintf(isStdErr ? stderr : stdout, "%s", str.c_str());
        if (str.find("\n") != std::string::npos) {
            fflush(isStdErr ? stderr : stdout);
            fflush(isStdErr ? stdout : stderr);
        }
    }

    // clean up line breaks (remove \r)
    std::string strToPrint = str;
    stringReplaceInPlace(strToPrint, "\r\n", "\n");
    stringReplaceInPlace(strToPrint, "\r", "\n");

    GThread::runOnQtGuiThread([this, strToPrint, isStdErr]() {
        _coutMutex.lock();
        _allOutputBuffer << strToPrint;
        if (!this->_textArea) {
            return;
        }
        this->_textArea->setEventsEnabled(false);
        this->_textArea->appendFormattedText(strToPrint, isStdErr ? getErrorColor() : getOutputColor());
        this->_textArea->moveCursorToEnd();
        this->_textArea->scrollToBottom();
        this->_textArea->setEventsEnabled(true);
        _coutMutex.unlock();
    });
}

void GConsoleWindow::println(bool isStdErr) {
    print("\n", isStdErr);
}

void GConsoleWindow::println(const std::string& str, bool isStdErr) {
    print(str + "\n", isStdErr);
}

void GConsoleWindow::processKeyPress(GEvent event) {
    char key = event.getKeyChar();
    int keyCode = event.getKeyCode();

    if (event.isCtrlOrCommandKeyDown()) {
        if (keyCode == Qt::Key_Plus || keyCode == Qt::Key_Equal) {
            // increase font size
            event.ignore();
            QFont font = GFont::toQFont(_textArea->getFont());
            if (font.pointSize() + 1 <= MAX_FONT_SIZE) {
                font.setPointSize(font.pointSize() + 1);
                setFont(GFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Minus) {
            // decrease font size
            event.ignore();
            QFont font = GFont::toQFont(_textArea->getFont());
            if (font.pointSize() - 1 >= MIN_FONT_SIZE) {
                font.setPointSize(font.pointSize() - 1);
                setFont(GFont::toFontString(font));
            }
        } else if (keyCode == Qt::Key_Insert) {
            // Ctrl+Ins => Copy
            event.ignore();
            clipboardCopy();
        } else if (keyCode == Qt::Key_0) {
            // normalize font size
            event.ignore();
            setFont(DEFAULT_FONT_FAMILY + "-" + std::to_string(DEFAULT_FONT_SIZE));
        } else if (keyCode >= Qt::Key_1 && keyCode <= Qt::Key_9) {
            // load input script 1-9
            loadInputScript(keyCode - Qt::Key_0);
        } else if (keyCode == Qt::Key_C) {
            event.ignore();
            clipboardCopy();
        } else if (event.isCtrlKeyDown() && keyCode == Qt::Key_D) {
            event.ignore();
            processEof();
        } else if (keyCode == Qt::Key_L) {
            event.ignore();
            clearConsole();
        } else if (keyCode == Qt::Key_Q || keyCode == Qt::Key_W) {
            event.ignore();
            close();
        } else if (keyCode == Qt::Key_S) {
            event.ignore();
            if (event.isShiftKeyDown()) {
                saveAs();
            } else {
                save();
            }
        } else if (keyCode == Qt::Key_V) {
            event.ignore();
            clipboardPaste();
        } else if (keyCode == Qt::Key_X) {
            event.ignore();
            clipboardCut();
        }
    }

    if (_shutdown) {
        return;
    }

    if (event.isCtrlOrCommandKeyDown() || event.isAltKeyDown()) {
        // system hotkey; let the normal keyboard handler process this event
        event.ignore();
        return;
    }

    switch (keyCode) {
        case GEvent::PAGE_UP_KEY:
        case GEvent::PAGE_DOWN_KEY:
            // don't ignore event
            break;
        case GEvent::BACKSPACE_KEY: {
            event.ignore();
            processBackspace(keyCode);
            break;
        }
        case GEvent::DELETE_KEY: {
            event.ignore();
            if (event.isShiftKeyDown()) {
                clipboardCut();   // Shift+Del => Cut
            } else {
                processBackspace(keyCode);
            }
            break;
        }
        case GEvent::INSERT_KEY: {
            event.ignore();
            if (event.isShiftKeyDown()) {
                clipboardPaste();   // Shift+Ins => Paste
            }
            break;
        }
        case GEvent::HOME_KEY:
            if (ALLOW_RICH_INPUT_EDITING) {
                // move to start of input buffer
                if (_promptActive) {
                    event.ignore();
                    int start = getUserInputStart();
                    if (start >= 0) {
                        _textArea->setCursorPosition(
                                start,
                                /* keepAnchor */ event.isShiftKeyDown() && isCursorInUserInputArea());
                    } else {
                        _textArea->moveCursorToEnd();
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case GEvent::END_KEY:
            if (ALLOW_RICH_INPUT_EDITING) {
                // move to end of input buffer
                if (_promptActive) {
                    event.ignore();
                    int end = getUserInputEnd();
                    if (end >= 0) {
                        _textArea->setCursorPosition(
                                end,
                                /* keepAnchor */ event.isShiftKeyDown() && isCursorInUserInputArea());
                    } else {
                        _textArea->moveCursorToEnd();
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case GEvent::LEFT_ARROW_KEY: {
            // bound within user input area if a prompt is active
            if (ALLOW_RICH_INPUT_EDITING) {
                if (isCursorInUserInputArea()) {
                    int cursorPosition = _textArea->getCursorPosition();
                    int userInputStart = getUserInputStart();
                    if (cursorPosition <= userInputStart) {
                        event.ignore();
                        if (!event.isShiftKeyDown()) {
                            _textArea->clearSelection();
                        }
                    }
                }
            } else {
                event.ignore();
            }
            break;
        }
        case GEvent::RIGHT_ARROW_KEY:
            // bound within user input area if a prompt is active
            if (ALLOW_RICH_INPUT_EDITING) {
                if (isCursorInUserInputArea()) {
                    int cursorPosition = _textArea->getCursorPosition();
                    int userInputEnd   = getUserInputEnd();
                    if (cursorPosition >= userInputEnd) {
                        event.ignore();
                        if (!event.isShiftKeyDown()) {
                            _textArea->clearSelection();
                        }
                    }
                }
            } else {
                event.ignore();
            }
            break;
        case GEvent::UP_ARROW_KEY:
            if (isCursorInUserInputArea()) {
                event.ignore();
                processCommandHistory(/* delta */ -1);
            }
            break;
        case GEvent::DOWN_ARROW_KEY:
            if (isCursorInUserInputArea()) {
                event.ignore();
                processCommandHistory(/* delta */ 1);
            }
            break;
        case GEvent::TAB_KEY:
            // TODO: tab completion?
        case GEvent::CLEAR_KEY:
            break;
        case GEvent::F1_KEY: {
            event.ignore();
            showAboutDialog();
            break;
        }
        case GEvent::F2_KEY:
        case GEvent::F3_KEY:
        case GEvent::F4_KEY:
        case GEvent::F5_KEY:
        case GEvent::F6_KEY:
        case GEvent::F7_KEY:
        case GEvent::F8_KEY:
        case GEvent::F9_KEY:
        case GEvent::F10_KEY:
        case GEvent::F11_KEY:
        case GEvent::F12_KEY:
        case GEvent::HELP_KEY: {
            // various control/modifier keys: do nothing / consume event
            event.ignore();
            break;
        }
        case GEvent::SHIFT_KEY:
        case GEvent::CTRL_KEY:
        case GEvent::ALT_KEY:
        case GEvent::PAUSE_KEY:
        case GEvent::CAPS_LOCK_KEY:
        case GEvent::ESCAPE_KEY:
        case GEvent::NUM_LOCK_KEY:
        case GEvent::SCROLL_LOCK_KEY:
        case GEvent::PRINT_SCREEN_KEY:
        case GEvent::META_KEY:
        case GEvent::WINDOWS_KEY:
        case GEvent::MENU_KEY: {
            // various other control/modifier keys: let OS have the event (don't call ignore())
            break;
        }
        case GEvent::RETURN_KEY:
        case GEvent::ENTER_KEY: {
            // \n end line
            event.ignore();
            processUserInputEnterKey();
            break;
        }
        default: {
            event.ignore();
            processUserInputKey(key);
            break;
        }
    }
}

void GConsoleWindow::processBackspace(int key) {
    if (_shutdown || !_promptActive) {
        return;
    }

    // check whether it is a backspace or a delete
    bool isBackspace = key == GEvent::BACKSPACE_KEY /* TODO: or computer is Mac */;

    _cinMutex.lockForWrite();
    if (!_inputBuffer.empty()) {
        // remove last char from onscreen text editor:
        // - find last blue area
        QTextFragment frag = getUserInputFragment();
        if (frag.isValid()) {
            // remove last char from onscreen document fragment
            QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
            QTextCursor cursor(textArea->textCursor());

            int oldCursorPosition = cursor.position();
            int indexToDelete = (int) _inputBuffer.length() - 1;
            int userInputIndexMin = frag.position();
            int userInputIndexMax = frag.position() + frag.length() - (isBackspace ? 0 : 1);

            if (oldCursorPosition >= userInputIndexMin && oldCursorPosition < userInputIndexMax) {
                // cursor is inside the user input fragment;
                // figure out which character it's on so we can delete it
                indexToDelete = oldCursorPosition - frag.position() - (isBackspace ? 1 : 0);
            } else {
                // cursor is outside of the user input fragment; move it there
                cursor.setPosition(frag.position() + frag.length());
            }

            if (indexToDelete >= 0 && indexToDelete < (int) _inputBuffer.length()) {
                if (isBackspace || indexToDelete == (int) _inputBuffer.length() - 1) {
                    cursor.deletePreviousChar();
                } else {
                    cursor.deleteChar();   // Delete
                }

                // remove last char from internal input buffer
                _inputBuffer.erase(indexToDelete, 1);
            }
        }
    }
    _cinMutex.unlock();
}

void GConsoleWindow::processCommandHistory(int delta) {
    if (_shutdown) {
        return;
    }
    _cinMutex.lockForRead();
    std::string oldCommand = "";
    _commandHistoryIndex += delta;
    _commandHistoryIndex = std::max(-1, _commandHistoryIndex);
    _commandHistoryIndex = std::min(_commandHistoryIndex, _inputCommandHistory.size());
    if (0 <= _commandHistoryIndex && _commandHistoryIndex < _inputCommandHistory.size()) {
        oldCommand = _inputCommandHistory[_commandHistoryIndex];
    }
    _cinMutex.unlock();
    setUserInput(oldCommand);
}

void GConsoleWindow::processEof() {
    if (_shutdown) {
        return;
    }
    // only set EOF if input buffer is empty; this is the behavior on most *nix consoles
    if (_inputBuffer.empty()) {
        std::cin.setstate(std::ios_base::eofbit);
    }
}

void GConsoleWindow::processUserInputEnterKey() {
    if (_shutdown) {
        return;
    }
    _cinMutex.lockForWrite();
    _cinQueueMutex.lockForWrite();
    _inputLines.enqueue(_inputBuffer);
    _inputCommandHistory.add(_inputBuffer);
    _commandHistoryIndex = _inputCommandHistory.size();
    _cinQueueMutex.unlock();
    _allOutputBuffer << _inputBuffer << std::endl;
    _inputBuffer = "";   // clear input buffer
    this->_textArea->appendFormattedText("\n", USER_INPUT_COLOR);
    _cinMutex.unlock();
}

void GConsoleWindow::processUserInputKey(int key) {
    if (_shutdown) {
        return;
    }
    if (key != '\0' && isprint(key)) {
        // normal key: append to user input buffer
        _cinMutex.lockForWrite();

        std::string keyStr = charToString((char) key);

        bool inserted = false;
        if (ALLOW_RICH_INPUT_EDITING && isCursorInUserInputArea()) {
            QTextFragment frag = getUserInputFragment();
            if (frag.isValid()) {
                QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
                QTextCursor cursor(textArea->textCursor());

                // BUGFIX: if there is any selected text, remove it first
                int fragStart = frag.position();
                int selectionStart = cursor.selectionStart() - fragStart;
                int selectionEnd = cursor.selectionEnd() - fragStart;
                if (selectionEnd > selectionStart
                        && selectionStart >= 0
                        && selectionEnd <= (int) _inputBuffer.length()) {
                    cursor.removeSelectedText();
                    _inputBuffer.erase(selectionStart, selectionEnd - selectionStart);
                }

                int cursorPosition = cursor.position();
                int indexToInsert = cursorPosition - frag.position();
                if (indexToInsert == 0) {
                    // special case for inserting at start of fragment.
                    // example: fragment is "abcde", cursor at start, user types "x".
                    // if we just insert the "x" in the document, it won't be part of
                    // the same fragment and won't have the blue bold format.
                    // So what we do is temporarily insert it after the first character,
                    // then delete the first character, so that everything is inside
                    // the formatted span.
                    // "abcde"
                    //  ^
                    //   ^          move right by 1
                    // "axabcde"    insert "xa" at index 1
                    //     ^
                    //   ^          move left by 2
                    // "xabcde"     delete previous character "a" from index 0
                    //  ^
                    //   ^          move right by 1
                    cursor.beginEditBlock();

                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);             // move to index 1
                    cursor.insertText(QString::fromStdString(keyStr + _inputBuffer.substr(0, 1)));   // insert new char + old first char
                    cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 2);              // delete old copy of first char
                    cursor.deletePreviousChar();
                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 1);             // move to index 1
                    cursor.endEditBlock();
                    textArea->setTextCursor(cursor);
                } else {
                    cursor.beginEditBlock();
                    cursor.insertText(QString::fromStdString(keyStr));
                    cursor.endEditBlock();
                    textArea->setTextCursor(cursor);
                }
                _inputBuffer.insert(indexToInsert, keyStr);
                inserted = true;
            }
        }

        if (!inserted) {
            // append to end of buffer/fragment
            _inputBuffer += keyStr;
            // display in blue highlighted text
            this->_textArea->appendFormattedText(keyStr, USER_INPUT_COLOR, "*-*-Bold");
        }

        _cinMutex.unlock();
    }
}

std::string GConsoleWindow::readLine() {
    // TODO: threads/locking
    // wait for a line to be available in queue
    std::string line;
    if (_shutdown) {
        return line;
    }

    this->_textArea->moveCursorToEnd();
    this->_textArea->scrollToBottom();
    this->toFront();   // move window to front on prompt for input
    this->_textArea->requestFocus();

    _cinMutex.lockForWrite();
    _promptActive = true;
    _cinMutex.unlock();

    while (!_shutdown && !std::cin.eof()) {
        bool lineRead = false;
        if (!_inputScript.isEmpty()) {
            _cinQueueMutex.lockForWrite();
            line = _inputScript.dequeue();
            lineRead = true;
            _cinQueueMutex.unlock();

            // echo user input, as if the user had just typed it
            GThread::runOnQtGuiThreadAsync([this, line]() {
                _coutMutex.lock();
                _allOutputBuffer << line << std::endl;
                _textArea->appendFormattedText(line + "\n", USER_INPUT_COLOR, "*-*-Bold");
                _coutMutex.unlock();
            });
        }

        if (!_inputLines.isEmpty()) {
            _cinQueueMutex.lockForWrite();
            if (!_inputLines.isEmpty()) {
                line = _inputLines.dequeue();
                lineRead = true;
            }

            _cinQueueMutex.unlock();
        }

        if (lineRead) {
            break;
        } else {
            sleep(20);
        }
    }

    _cinMutex.lockForWrite();
    _promptActive = false;
    _cinMutex.unlock();
    this->_textArea->scrollToBottom();

    if (_echo) {
        fprintf(stdout, "%s\n", line.c_str());
    }
    return line;
}

void GConsoleWindow::save() {
    saveAs(_lastSaveFileName);
}

void GConsoleWindow::saveAs(const std::string& filename) {
    std::string filenameToUse;
    if (filename.empty()) {
        filenameToUse = GFileChooser::showSaveDialog(
                /* parent */ this->getWidget(),
                /* title */ "",
                getHead(_lastSaveFileName));
    } else {
        filenameToUse = filename;
    }
    if (filenameToUse.empty()) {
        return;
    }

    std::string consoleText = _textArea->getText();
    writeEntireFile(filenameToUse, consoleText);
    _lastSaveFileName = filenameToUse;
}

void GConsoleWindow::saveConfiguration(bool prompt) {
    if (prompt && !GOptionPane::showConfirmDialog(
            /* parent  */  getWidget(),
            /* message */  "Make this the default for future console windows?",
            /* title   */  "Save configuration?")) {
        return;
    }
    std::string configFile = getTempDirectory() + "/" + CONFIG_FILE_NAME;
    std::string configText = "# Stanford C++ library configuration file\n"
            "background=" + _textArea->getBackground() + "\n"
            "foreground=" + getOutputColor() + "\n"
            "font=" + _textArea->getFont() + "\n";
    writeEntireFile(configFile, configText);
}

void GConsoleWindow::selectAll() {
    _textArea->selectAll();
}

void GConsoleWindow::setBackground(int color) {
    GWindow::setBackground(color);   // call super
    _textArea->setBackground(color);
}

void GConsoleWindow::setBackground(const std::string& color) {
    GWindow::setBackground(color);   // call super
    _textArea->setBackground(color);
}

void GConsoleWindow::setClearEnabled(bool clearEnabled) {
    if (_locked || _shutdown) {
        return;
    }
    _clearEnabled = clearEnabled;
}

void GConsoleWindow::setConsoleSize(double width, double height) {
    // TODO: base on text area's preferred size / packing window
    // _textArea->setPreferredSize(width, height);
    // pack();
    setSize(width, height);
}

void GConsoleWindow::setColor(int color) {
    setOutputColor(color);
}

void GConsoleWindow::setColor(const std::string& color) {
    setOutputColor(color);
}

void GConsoleWindow::setEcho(bool echo) {
    if (_locked || _shutdown) {
        return;
    }
    _echo = echo;
}

void GConsoleWindow::setFont(const QFont& font) {
    GWindow::setFont(font);   // call super
    _textArea->setFont(font);
}

void GConsoleWindow::setFont(const std::string& font) {
    GWindow::setFont(font);   // call super
    _textArea->setFont(font);
}

void GConsoleWindow::setForeground(int color) {
    setOutputColor(color);
}

void GConsoleWindow::setForeground(const std::string& color) {
    setOutputColor(color);
}

void GConsoleWindow::setLocationSaved(bool locationSaved) {
    _locationSaved = locationSaved;
}

void GConsoleWindow::setLocked(bool locked) {
    _locked = locked;
}

void GConsoleWindow::setErrorColor(const std::string& errorColor) {
    _errorColor = errorColor;
}

void GConsoleWindow::setOutputColor(int rgb) {
    setOutputColor(GColor::convertRGBToColor(rgb));
}

void GConsoleWindow::setOutputColor(const std::string& outputColor) {
    _outputColor = outputColor;
    _textArea->setForeground(outputColor);
    if (_shutdown) {
        return;
    }

    // go through any past fragments and recolor them to this color

    // select all previous text and change its color
    // (BUG?: also changes user input text to be that color; desired?)
    QTextEdit* textArea = static_cast<QTextEdit*>(this->_textArea->getWidget());
    QTextCursor cursor = textArea->textCursor();
    cursor.beginEditBlock();
    cursor.setPosition(0);
    QTextCharFormat format = cursor.charFormat();
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    format.setForeground(QBrush(GColor::toQColor(outputColor)));
    textArea->setTextCursor(cursor);
    cursor.setCharFormat(format);
    cursor.endEditBlock();
    _textArea->moveCursorToEnd();
}

void GConsoleWindow::setSize(double width, double height) {
    if (isHighDpiScalingEnabled() && isHighDensityScreen()) {
        double ratio = getScreenDpiScaleRatio();
        width = std::min(getScreenWidth(), width * ratio);
        height = std::min(getScreenHeight(), height * ratio);;
    }

    // call super
    GWindow::setSize(width, height);
}

void GConsoleWindow::setUserInput(const std::string& userInput) {
    if (_shutdown) {
        return;
    }
    _cinMutex.lockForWrite();
    QTextEdit* textArea = static_cast<QTextEdit*>(_textArea->getWidget());

    // delete any current user input
    QTextFragment frag = getUserInputFragment();
    if (frag.isValid()) {
        QTextCursor cursor = textArea->textCursor();
        cursor.beginEditBlock();
        cursor.setPosition(frag.position(), QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, frag.length());
        cursor.removeSelectedText();
        cursor.endEditBlock();
        textArea->setTextCursor(cursor);
    }
    _inputBuffer.clear();
    _cinMutex.unlock();

    // insert the given user input
    for (int i = 0; i < (int) userInput.length(); i++) {
        processUserInputKey(userInput[i]);
    }
}

void GConsoleWindow::showAboutDialog() {
    // this text roughly matches that from old spl.jar message
    static const std::string ABOUT_MESSAGE =
            "<html><p>"
            "Stanford C++ Library version <b>" + version::getCppLibraryVersion() + "</b><br>\n"
            "<br>\n"
            "Libraries originally written by <b>Eric Roberts</b>,<br>\n"
            "with assistance from Julie Zelenski, Keith Schwarz, et al.<br>\n"
            "This version of the library is unofficially maintained by <b>Marty Stepp</b>.<br>\n"
            "<br>\n"
            "See <a href=\"" + version::getCppLibraryDocsUrl() + "\">" + version::getCppLibraryDocsUrl() + "</a> for documentation."
            "</p></html>";
    GOptionPane::showMessageDialog(
                /* parent */   getWidget(),
                /* message */  ABOUT_MESSAGE,
                /* title */    "About Stanford C++ Library",
                /* type */     GOptionPane::MESSAGE_ABOUT);
}

void GConsoleWindow::showColorDialog(bool background) {
    std::string color = GColorChooser::showDialog(
                /* parent */   getWidget(),
                /* title */    "",
                /* initial */  background ? _textArea->getBackground() : _textArea->getForeground());
    if (!color.empty()) {
        if (background) {
            setBackground(color);
        } else {
            setOutputColor(color);
        }
        saveConfiguration();   // prompt to save configuration
    }
}

void GConsoleWindow::showCompareOutputDialog() {
    std::string filename = GFileChooser::showOpenDialog(
                /* parent */ getWidget(),
                /* title  */ "Select an expected output file");
    if (!filename.empty() && fileExists(filename)) {
        compareOutput(filename);
    }
}

void GConsoleWindow::showFontDialog() {
    std::string font = GFontChooser::showDialog(
                /* parent */ getWidget(),
                /* title  */ "",
                /* initialFont */ _textArea->getFont());
    if (!font.empty()) {
        _textArea->setFont(font);
        saveConfiguration();   // prompt to save configuration
    }
}

void GConsoleWindow::showInputScriptDialog() {
    if (_shutdown) {
        return;
    }
    std::string filename = GFileChooser::showOpenDialog(
                /* parent */ getWidget(),
                /* title  */ "Select an input script file");
    if (!filename.empty() && fileExists(filename)) {
        loadInputScript(filename);
    }
}

void GConsoleWindow::showPrintDialog() {
    // TODO
}

void GConsoleWindow::shutdown() {
    const std::string PROGRAM_COMPLETED_TITLE_SUFFIX = " [completed]";
    _shutdown = true;
    std::cout.flush();
    std::cerr.flush();

    // restore old cin, cout, cerr
    if (_cin_old_buf) {
        _coutMutex.lock();
        std::cin.rdbuf(_cin_old_buf);
        std::cout.rdbuf(_cout_old_buf);
        std::cerr.rdbuf(_cerr_old_buf);
        _cin_old_buf = nullptr;
        _cout_old_buf = nullptr;
        _cerr_old_buf = nullptr;
        std::cout.flush();
        std::cerr.flush();
        _coutMutex.unlock();
    }

    _textArea->setEditable(false);
    std::string title = getTitle();
    if (title.find(PROGRAM_COMPLETED_TITLE_SUFFIX) == std::string::npos) {
        setTitle(title + PROGRAM_COMPLETED_TITLE_SUFFIX);
    }

    // TODO: disable some menu items
}

// global functions used by ConsoleStreambufQt

namespace stanfordcpplib {
namespace qtgui {

void endLineConsoleQt(bool isStderr) {
    GConsoleWindow::instance()->println(isStderr);
}

std::string getLineConsoleQt() {
    return GConsoleWindow::instance()->readLine();
}

void putConsoleQt(const std::string& str, bool isStderr) {
    GConsoleWindow::instance()->print(str, isStderr);
}

} // namespace qtgui
} // namespace stanfordcpplib

/*
 * File: gevent.cpp
 * ----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gevent.cpp to replace Java version
 * @version 2018/07/06
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gevent.h"
#include <iostream>
#include <sys/time.h>
#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

static void __emptyEventListener(GEvent) {
    // empty
}

static void __logEventListener(GEvent event) {
    std::cout << "log event: " << event << std::endl;
}

GEventListener GEvent::EMPTY_EVENT_LISTENER = __emptyEventListener;
GEventListener GEvent::LOG_EVENT = __logEventListener;


GEvent::GEvent(EventClass eventClass,
                 EventType eventType,
                 const std::string& eventName,
                 GObservable* source)
        :
          _actionCommand(""),
          _class(eventClass),
          _keyChar('\0'),
          _keyCode(0),
          _modifiers(0),
          _name(eventName),
          _source(source),
          _time(getCurrentTimeMS()),
          _type(eventType),
          _x(0),
          _y(0),
          _row(0),
          _col(0),
          _internalQtEvent(nullptr) {
    // empty
}

GEvent::~GEvent() {
    _source = nullptr;
}

std::string GEvent::classToString(EventClass eventClass) {
    switch (eventClass) {
    case NULL_EVENT:      return "NULL_EVENT";
    case ACTION_EVENT:    return "ACTION_EVENT";
    case KEY_EVENT:       return "KEY_EVENT";
    case TIMER_EVENT:     return "TIMER_EVENT";
    case WINDOW_EVENT:    return "WINDOW_EVENT";
    case MOUSE_EVENT:     return "MOUSE_EVENT";
    case CLICK_EVENT:     return "CLICK_EVENT";
    case TABLE_EVENT:     return "TABLE_EVENT";
    case SERVER_EVENT:    return "SERVER_EVENT";
    case CHANGE_EVENT:    return "CHANGE_EVENT";
    case HYPERLINK_EVENT: return "HYPERLINK_EVENT";
    case SCROLL_EVENT:    return "SCROLL_EVENT";
    default:              return "UNKNOWN";
    }
}

std::string GEvent::typeToString(EventType eventType) {
    switch (eventType) {
    case NULL_TYPE:            return "NULL_TYPE";
    case WINDOW_CLOSED:        return "WINDOW_CLOSED";
    case WINDOW_RESIZED:       return "WINDOW_RESIZED";
    case CONSOLE_CLOSED:       return "CONSOLE_CLOSED";
    case WINDOW_CLOSING:       return "WINDOW_CLOSING";
    case WINDOW_MINIMIZED:     return "WINDOW_MINIMIZED";
    case WINDOW_RESTORED:      return "WINDOW_RESTORED";
    case WINDOW_MAXIMIZED:     return "WINDOW_MAXIMIZED";
    case ACTION_PERFORMED:     return "ACTION_PERFORMED";
    case ACTION_MENU:          return "ACTION_MENU";
    case MOUSE_CLICKED:        return "MOUSE_CLICKED";
    case MOUSE_PRESSED:        return "MOUSE_PRESSED";
    case MOUSE_RELEASED:       return "MOUSE_RELEASED";
    case MOUSE_MOVED:          return "MOUSE_MOVED";
    case MOUSE_DRAGGED:        return "MOUSE_DRAGGED";
    case MOUSE_ENTERED:        return "MOUSE_ENTERED";
    case MOUSE_EXITED:         return "MOUSE_EXITED";
    case MOUSE_WHEEL_DOWN:     return "MOUSE_WHEEL_DOWN";
    case MOUSE_WHEEL_UP:       return "MOUSE_WHEEL_UP";
    case MOUSE_DOUBLE_CLICKED: return "MOUSE_DOUBLE_CLICKED";
    case KEY_PRESSED:          return "KEY_PRESSED";
    case KEY_RELEASED:         return "KEY_RELEASED";
    case KEY_TYPED:            return "KEY_TYPED";
    case TIMER_TICKED:         return "TIMER_TICKED";
    case TABLE_UPDATED:        return "TABLE_UPDATED";
    case TABLE_SELECTED:       return "TABLE_SELECTED";
    case TABLE_EDIT_BEGIN:     return "TABLE_EDIT_BEGIN";
    case TABLE_REPLACE_BEGIN:  return "TABLE_REPLACE_BEGIN";
    case TABLE_EDIT_CANCEL:    return "TABLE_EDIT_CANCEL";
    case TABLE_CUT:            return "TABLE_CUT";
    case TABLE_COPY:           return "TABLE_COPY";
    case TABLE_PASTE:          return "TABLE_PASTE";
    case SERVER_REQUEST:       return "SERVER_REQUEST";
    case STATE_CHANGED:        return "STATE_CHANGED";
    case HYPERLINK_CLICKED:    return "HYPERLINK_CLICKED";
    case SCROLL_SCROLLED:      return "SCROLL_SCROLLED";
    default:                   return "UNKNOWN";
    }
}


std::string GEvent::getActionCommand() const {
    return _actionCommand;
}

int GEvent::getButton() const {
    return _button;
}

EventClass GEvent::getClass() const {
    return _class;
}

int GEvent::getColumn() const {
    return _col;
}

long GEvent::getCurrentTimeMS() {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

EventClass GEvent::getEventClass() const {
    return _class;
}

EventType GEvent::getEventType() const {
    return _type;
}

char GEvent::getKeyChar() const {
    return _keyChar;
}

int GEvent::getKeyCode() const {
    return _keyCode;
}

int GEvent::getModifiers() const {
    return _modifiers;
}

std::string GEvent::getName() const {
    return _name;
}

std::string GEvent::getRequestURL() const {
    return _requestUrl;
}

GInteractor* GEvent::getInteractor() const {
    return static_cast<GInteractor*>(_source);
}

QEvent* GEvent::getInternalEvent() const {
    return _internalQtEvent;
}

GPoint GEvent::getLocation() const {
    return GPoint(getX(), getY());
}

int GEvent::getRow() const {
    return _row;
}

GObservable* GEvent::getSource() const {
    return _source;
}

long GEvent::getTime() const {
    return _time;
}

EventType GEvent::getType() const {
    return _type;
}

double GEvent::getX() const {
    return _x;
}

double GEvent::getY() const {
    return _y;
}

void GEvent::ignore() {
    if (_internalQtEvent) {
        _internalQtEvent->ignore();
    }
}

bool GEvent::isAltKeyDown() const {
    return (_modifiers & ALT_DOWN) != 0;
}

bool GEvent::isCtrlKeyDown() const {
    return (_modifiers & CTRL_DOWN) != 0;
}

bool GEvent::isCtrlOrCommandKeyDown() const {
    return isCtrlKeyDown() || isMetaKeyDown();
}

bool GEvent::isDoubleClick() const {
    return _type == MOUSE_DOUBLE_CLICKED || _name == "doubleclick";
}

bool GEvent::isLeftClick() const {
    return !isMiddleClick() && !isRightClick();
}

bool GEvent::isMiddleClick() const {
    return _button == 4 || ((_modifiers & BUTTON3_DOWN) != 0);
}

bool GEvent::isRightClick() const {
    return _button == 2 || ((_modifiers & BUTTON2_DOWN) != 0);
}

bool GEvent::isMetaKeyDown() const {
    return (_modifiers & META_DOWN) != 0;
}

bool GEvent::isShiftKeyDown() const {
    return (_modifiers & SHIFT_DOWN) != 0;
}

std::string GEvent::keyCodeToString(int keyCode) {
    switch (keyCode) {
    case ALT_KEY: return "Alt";
    case BACKSPACE_KEY: return "Bksp";
    case CAPS_LOCK_KEY: return "CapsLock";
    case CLEAR_KEY: return "Clear";
    case CTRL_KEY: return "Ctrl";
    case DELETE_KEY: return "Del";
    case DOWN_ARROW_KEY: return "Down";
    case END_KEY: return "End";
    case ENTER_KEY: return "Enter";
    case ESCAPE_KEY: return "Esc";
    case F10_KEY: return "F10";
    case F11_KEY: return "F11";
    case F12_KEY: return "F12";
    case F1_KEY: return "F1";
    case F2_KEY: return "F2";
    case F3_KEY: return "F3";
    case F4_KEY: return "F4";
    case F5_KEY: return "F5";
    case F6_KEY: return "F6";
    case F7_KEY: return "F7";
    case F8_KEY: return "F8";
    case F9_KEY: return "F9";
    case HELP_KEY: return "Help";
    case HOME_KEY: return "Home";
    case INSERT_KEY: return "Ins";
    case LEFT_ARROW_KEY: return "Left";
    case MENU_KEY: return "Menu";
    case META_KEY: return "Meta";
    case NUM_LOCK_KEY: return "NumLock";
    case PAGE_DOWN_KEY: return "PgDn";
    case PAGE_UP_KEY: return "PgUp";
    case PAUSE_KEY: return "Pause";
    case PRINT_SCREEN_KEY: return "PrtSc";
    case RIGHT_ARROW_KEY: return "Right";
    case SCROLL_LOCK_KEY: return "ScrollLock";
    case SHIFT_KEY: return "Shift";
    case TAB_KEY: return "Tab";
    case UP_ARROW_KEY: return "Up";
    case WINDOWS_KEY: return "Win";
    case '\r': return "Enter";
    case '\\': return "\\";
    default: return charToString((char) keyCode);
    }
}

void GEvent::setActionCommand(const std::string& actionCommand) {
    _actionCommand = actionCommand;
}

void GEvent::setButton(int button) {
    _button = button;
}

void GEvent::setInternalEvent(QEvent* event) {
    _internalQtEvent = event;
}

void GEvent::setKeyChar(char keyChar) {
    if (keyChar == '\r') {
        keyChar = '\n';
    }
    _keyChar = keyChar;
}

void GEvent::setKeyChar(const std::string& keyCharString) {
    if (!keyCharString.empty()) {
        setKeyChar(keyCharString[0]);
    }
}

void GEvent::setKeyCode(int keyCode) {
    if (keyCode == (int) '\r') {
        keyCode = (int) '\n';
    }
    _keyCode = keyCode;
    if (keyCode > 0 && keyCode < 255) {
        _keyChar = (char) keyCode;
    } else {
        switch (keyCode) {
        case Qt::Key_Alt:
        case Qt::Key_AltGr:
            _keyChar = _keyCode = ALT_KEY;
            break;
        case Qt::Key_Backspace:
            _keyChar = _keyCode = BACKSPACE_KEY;
            break;
        case Qt::Key_CapsLock:
            _keyChar = _keyCode = CAPS_LOCK_KEY;
            break;
        case Qt::Key_Control:
            _keyChar = _keyCode = CTRL_KEY;
            break;
        case Qt::Key_Delete:
            _keyChar = _keyCode = DELETE_KEY;
            break;
        case Qt::Key_Down:
            _keyChar = _keyCode = DOWN_ARROW_KEY;
            break;
        case Qt::Key_End:
            _keyChar = _keyCode = END_KEY;
            break;
        case Qt::Key_Enter:
            _keyChar = _keyCode = ENTER_KEY;
            break;
        case Qt::Key_Escape:
            _keyChar = _keyCode = ESCAPE_KEY;
            break;
        case Qt::Key_F1:
            _keyChar = _keyCode = F1_KEY;
            break;
        case Qt::Key_F2:
            _keyChar = _keyCode = F2_KEY;
            break;
        case Qt::Key_F3:
            _keyChar = _keyCode = F3_KEY;
            break;
        case Qt::Key_F4:
            _keyChar = _keyCode = F4_KEY;
            break;
        case Qt::Key_F5:
            _keyChar = _keyCode = F5_KEY;
            break;
        case Qt::Key_F6:
            _keyChar = _keyCode = F6_KEY;
            break;
        case Qt::Key_F7:
            _keyChar = _keyCode = F7_KEY;
            break;
        case Qt::Key_F8:
            _keyChar = _keyCode = F8_KEY;
            break;
        case Qt::Key_F9:
            _keyChar = _keyCode = F9_KEY;
            break;
        case Qt::Key_F10:
            _keyChar = _keyCode = F10_KEY;
            break;
        case Qt::Key_F11:
            _keyChar = _keyCode = F11_KEY;
            break;
        case Qt::Key_F12:
            _keyChar = _keyCode = F12_KEY;
            break;
        case Qt::Key_Home:
            _keyChar = _keyCode = HOME_KEY;
            break;
        case Qt::Key_Insert:
            _keyChar = _keyCode = INSERT_KEY;
            break;
        case Qt::Key_Left:
            _keyChar = _keyCode = LEFT_ARROW_KEY;
            break;
        case Qt::Key_Menu:
            _keyChar = _keyCode = MENU_KEY;
            break;
        case Qt::Key_Meta:
            _keyChar = _keyCode = META_KEY;
            break;
        case Qt::Key_NumLock:
            _keyChar = _keyCode = NUM_LOCK_KEY;
            break;
        case Qt::Key_PageDown:
            _keyChar = _keyCode = PAGE_DOWN_KEY;
            break;
        case Qt::Key_PageUp:
            _keyChar = _keyCode = PAGE_UP_KEY;
            break;
        case Qt::Key_Pause:
            _keyChar = _keyCode = PAUSE_KEY;
            break;
        case Qt::Key_Print:
            _keyChar = _keyCode = PRINT_SCREEN_KEY;
            break;
        case Qt::Key_Return:
            _keyChar = _keyCode = ENTER_KEY;
            break;
        case Qt::Key_Right:
            _keyChar = _keyCode = RIGHT_ARROW_KEY;
            break;
        case Qt::Key_ScrollLock:
            _keyChar = _keyCode = SCROLL_LOCK_KEY;
            break;
        case Qt::Key_Shift:
            _keyChar = _keyCode = SHIFT_KEY;
            break;
        case Qt::Key_Super_L:
        case Qt::Key_Super_R:
            _keyChar = _keyCode = WINDOWS_KEY;
            break;
        case Qt::Key_Tab:
            _keyChar = _keyCode = TAB_KEY;
            break;
        case Qt::Key_Up:
            _keyChar = _keyCode = UP_ARROW_KEY;
            break;
        }
    }
}

void GEvent::setModifiers(Qt::KeyboardModifiers modifiers) {
    _modifiers = 0;
    if (modifiers & Qt::AltModifier) {
        _modifiers |= ALT_DOWN;
    }
    if (modifiers & Qt::ControlModifier) {
        _modifiers |= CTRL_DOWN;
    }
    if (modifiers & Qt::MetaModifier) {
        _modifiers |= META_DOWN;
    }
    if (modifiers & Qt::ShiftModifier) {
        _modifiers |= SHIFT_DOWN;
    }
}

void GEvent::setRequestURL(const std::string& requestUrl) {
    _requestUrl = requestUrl;
}

void GEvent::setRowAndColumn(int row, int col) {
    _row = row;
    _col = col;
}

void GEvent::setSource(GObservable* source) {
    _source = source;
}

void GEvent::setX(double x) {
    _x = x;
}

void GEvent::setY(double y) {
    _y = y;
}

std::string GEvent::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& out, const GEvent& event) {
    out << "GEvent(";
    out << "class=" << GEvent::classToString(event.getClass());
    out << ",type=" << GEvent::typeToString(event.getType());
    out << ",name=" << event.getName();
    out << ",time=" << event.getTime();
    if (event.getSource()) {
        out << ",source=" << event.getSource()->toString();
    }
    if (!event.getActionCommand().empty()) {
        out << ",actionCommand=\"" << event.getActionCommand() << "\"";
    }
    if (event.getEventClass() == KEY_EVENT) {
        out << ",key=" << event.getKeyCode()
            << "(" << GEvent::keyCodeToString(event.getKeyCode()) << ")";
    } else if (event.getEventClass() == MOUSE_EVENT) {
        out << ",x=" << event.getX() << ",y=" << event.getY() << ",button=" << event.getButton();
    } else if (event.getEventClass() == TABLE_EVENT) {
        out << ",row=" << event.getRow() << ",col=" << event.getColumn();
    }
    out << ")";
    return out;
}

/*
 * File: glayout.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to glayout.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "glayout.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

GLayout::GLayout() {
    // empty
}

void GLayout::clearLayout(QLayout* layout) {
    if (!layout) {
        return;
    }

    // http://doc.qt.io/qt-5/qlayout.html#takeAt
    QLayoutItem* child;
    while ((child = layout->takeAt(0))) {
        // TODO: delete child;
    }
    layout->update();
}

bool GLayout::contains(QLayout* layout, QWidget* widget) {
    if (!layout || !widget) {
        return false;
    }

    for (int i = 0; i < layout->count(); i++) {
        QLayoutItem* child = layout->itemAt(i);
        QWidget* childWidget = child->widget();
        if (childWidget == widget) {
            return true;
        }
    }

    return false;
}

void GLayout::forceUpdate(GInteractor* interactor) {
    if (interactor) {
        forceUpdate(interactor->getWidget());
    }
}

/**
 * Forces the given widget to update, even if it's hidden.
 * from https://stackoverflow.com/questions/2427103/qt-how-to-force-a-hidden-widget-to-calculate-its-layout
 */
void GLayout::forceUpdate(QWidget* widget) {
    if (!widget) {
        return;
    }

    // Update all child widgets.
    for (int i = 0; i < widget->children().size(); i++) {
        QObject* child = widget->children()[i];
        if (child->isWidgetType()) {
            forceUpdate((QWidget*) child);
        }
    }

    widget->updateGeometry();
    widget->update();

    // Invalidate the layout of the widget.
    if (widget->layout()) {
        invalidateLayout(widget->layout());
    }

    widget->setAttribute(Qt::WA_DontShowOnScreen, true);   // TODO: remove?
    widget->setAttribute(Qt::WA_DontShowOnScreen, false);
    widget->show();
}

QSize GLayout::getPreferredSize(QWidget* widget) {
    require::nonNull(widget, "GLayout::getPreferredSize", "widget");
    QRect geom = widget->geometry();
    QSize hint = widget->sizeHint();
    int height = hint.height() > 0 ? hint.height() : geom.height();
    int width = hint.width() > 0 ? hint.width() : geom.width();
    return QSize(width, height);
}

QSize GLayout::getProperSize(QLayout* layout) {
    require::nonNull(layout, "GLayout::getProperSize", "layout");
    QRect geom = layout->geometry();
    QSize hint = layout->sizeHint();
    int height = geom.height() > 0 ? geom.height() : hint.height();
    int width = geom.width() > 0 ? geom.width() : hint.width();
    return QSize(width, height);
}

QSize GLayout::getProperSize(QWidget* widget) {
    require::nonNull(widget, "GLayout::getProperSize", "widget");
    QRect geom = widget->geometry();
    QSize hint = widget->sizeHint();
    int height = geom.height() > 0 ? geom.height() : hint.height();
    int width = geom.width() > 0 ? geom.width() : hint.width();
    return QSize(width, height);
}

/**
 * Helper function for forceUpdate(). Not self-sufficient!
 * from https://stackoverflow.com/questions/2427103/qt-how-to-force-a-hidden-widget-to-calculate-its-layout
 */
void GLayout::invalidateLayout(QLayout* layout) {
    if (!layout) {
        return;
    }

    // Recompute the given layout and all its child layouts.
    for (int i = 0; i < layout->count(); i++) {
        QLayoutItem* item = layout->itemAt(i);
        if (item->layout()) {
            invalidateLayout(item->layout());
        } else {
            item->invalidate();
        }
    }
    layout->invalidate();
    layout->activate();
}

GLayout::Position GLayout::toPosition(const std::string& positionName) {
    std::string regionU = toUpperCase(positionName);
    if (regionU == "NORTH" || regionU == "TOP") {
        return GLayout::North;
    } else if (regionU == "SOUTH" || regionU == "BOTTOM") {
        return GLayout::South;
    } else if (regionU == "EAST" || regionU == "RIGHT") {
        return GLayout::East;
    } else if (regionU == "WEST" || regionU == "LEFT") {
        return GLayout::West;
    } else {
        // "CENTER" or "MIDDLE" or "NONE"
        return GLayout::Center;
    }
}

GBorderLayout::GBorderLayout(QWidget* parent, int margin, int spacing)
        : QLayout(parent) {
    setMargin(margin);
    setSpacing(spacing);
}

GBorderLayout::GBorderLayout(int spacing) {
    setSpacing(spacing);
}

GBorderLayout::~GBorderLayout() {
    QLayoutItem* l;
    while ((l = takeAt(0))) {
        delete l;
    }
}

void GBorderLayout::addItem(QLayoutItem* item) {
    require::nonNull(item, "GBorderLayout::addItem");
    add(item, GLayout::North);
}

void GBorderLayout::addWidget(QWidget* widget) {
    addWidget(widget, GLayout::Center);
}

void GBorderLayout::addWidget(QWidget* widget, GLayout::Position position) {
    require::nonNull(widget, "GBorderLayout::addWidget", "widget");
    add(new QWidgetItem(widget), position);
}

Qt::Orientations GBorderLayout::expandingDirections() const {
    return Qt::Horizontal | Qt::Vertical;
}

bool GBorderLayout::hasHeightForWidth() const {
    return false;
}

int GBorderLayout::count() const {
    return list.size();
}

QLayoutItem* GBorderLayout::itemAt(int index) const {
    ItemWrapper* wrapper = list.value(index);
    if (wrapper) {
        return wrapper->item;
    } else {
        return nullptr;
    }
}

QSize GBorderLayout::minimumSize() const {
    return calculateSize(MinimumSize);
}

void GBorderLayout::setGeometry(const QRect& rect) {
    ItemWrapper* center = nullptr;
    int eastWidth = 0;
    int westWidth = 0;
    int northHeight = 0;
    int southHeight = 0;
    int centerHeight = 0;

    QLayout::setGeometry(rect);

    // size/place north/south widgets first
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        QLayoutItem* item = wrapper->item;
        GLayout::Position position = wrapper->position;

        if (position == GLayout::North) {
            QRect geom(rect.x(),
                       northHeight,
                       rect.width(),
                       item->sizeHint().height());
            item->setGeometry(geom);
            northHeight += item->geometry().height() + spacing();
        } else if (position == GLayout::South) {
            QRect geom(item->geometry().x(),
                       item->geometry().y(),
                       rect.width(),
                       item->sizeHint().height());
            item->setGeometry(geom);

            southHeight += item->geometry().height() + spacing();
            geom = QRect(rect.x(),
                         rect.y() + rect.height() - southHeight + spacing(),
                         item->geometry().width(),
                         item->geometry().height());
            item->setGeometry(geom);
        } else if (position == GLayout::Center) {
            center = wrapper;
        }
    }

    centerHeight = rect.height() - northHeight - southHeight;

    // size/place west/east widgets second
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        QLayoutItem* item = wrapper->item;
        GLayout::Position position = wrapper->position;

        if (position == GLayout::West) {
            QRect geom(rect.x() + westWidth,
                       northHeight,
                       item->sizeHint().width(),
                       centerHeight);
            item->setGeometry(geom);
            westWidth += item->geometry().width() + spacing();
        } else if (position == GLayout::East) {
            QRect geom(item->geometry().x(),
                       item->geometry().y(),
                       item->sizeHint().width(),
                       centerHeight);
            item->setGeometry(geom);

            eastWidth += item->geometry().width() + spacing();
            geom = QRect(rect.x() + rect.width() - eastWidth + spacing(),
                       northHeight,
                       item->geometry().width(),
                       item->geometry().height());
            item->setGeometry(geom);
        }
    }

    // size/place central widget last
    if (center) {
        QRect geom(westWidth,
                   northHeight,
                   rect.width() - eastWidth - westWidth,
                   centerHeight);
        center->item->setGeometry(geom);
    }
}

QSize GBorderLayout::sizeHint() const {
    return calculateSize(SizeHint);
}

QLayoutItem* GBorderLayout::takeAt(int index) {
    if (index >= 0 && index < list.size()) {
        ItemWrapper* layoutStruct = list.takeAt(index);
        return layoutStruct->item;
    } else {
        return nullptr;
    }
}

void GBorderLayout::add(QLayoutItem* item, GLayout::Position position) {
    require::nonNull(item, "GBorderLayout::add");
    list.append(new ItemWrapper(item, position));
}

QSize GBorderLayout::calculateSize(SizeType sizeType) const {
    QSize totalSize;
    for (int i = 0; i < list.size(); ++i) {
        ItemWrapper* wrapper = list.at(i);
        GLayout::Position position = wrapper->position;
        QSize itemSize;

        if (sizeType == MinimumSize) {
            itemSize = wrapper->item->minimumSize();
        } else { // (sizeType == SizeHint)
            itemSize = wrapper->item->sizeHint();
        }

        if (position == GLayout::North || position == GLayout::South || position == GLayout::Center) {
            totalSize.rheight() += itemSize.height();
        }

        if (position == GLayout::West || position == GLayout::East || position == GLayout::Center) {
            totalSize.rwidth() += itemSize.width();
        }
    }
    return totalSize;
}

/*
 * File: gcanvas.cpp
 * -----------------
 *
 * @version 2018/09/20
 * - added read/write lock for canvas contents to avoid race conditions
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gcanvas.cpp to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gcanvas.h"
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

#define CHAR_TO_HEX(ch) ((ch >= '0' && ch <= '9') ? (ch - '0') : (ch - 'a' + 10))

const int GCanvas::WIDTH_HEIGHT_MAX = 65535;

int GCanvas::createRgbPixel(int red, int green, int blue) {
    if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0 || blue > 255) {
        error("RGB values must be between 0-255");
    }
    return ((red << 16 & 0xff0000) | (green << 8 & 0x00ff00) | (blue & 0x0000ff)) & 0xffffff;
}

int GCanvas::getAlpha(int argb) {
    // have to & a second time because of sign-extension on >> shift
    return ((static_cast<unsigned int>(argb) & 0xff000000) >> 24) & 0x000000ff;
}

int GCanvas::getBlue(int rgb) {
    return rgb & 0x0000ff;
}

int GCanvas::getGreen(int rgb) {
    return (rgb & 0x00ff00) >> 8;
}

int GCanvas::getRed(int rgb) {
    return (rgb & 0xff0000) >> 16;
}

void GCanvas::getRedGreenBlue(int rgb, int& red, int& green, int& blue) {
    red = getRed(rgb);
    green = getGreen(rgb);
    blue = getBlue(rgb);
}


GCanvas::GCanvas(QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    init(/* width */ -1, /* height */ -1, /* background */ 0xffffff, parent);
}

GCanvas::GCanvas(const std::string& filename, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename(filename) {
    init(/* width */ -1, /* height */ -1, /* background */ 0xffffff, parent);
    load(filename);
}

GCanvas::GCanvas(double width, double height, int rgbBackground, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    init(width, height, rgbBackground, parent);
}

GCanvas::GCanvas(double width, double height, const std::string& rgbBackground, QWidget* parent)
        : _backgroundImage(nullptr),
          _filename("") {
    _backgroundColor = rgbBackground;
    init(width, height,
         GColor::hasAlpha(rgbBackground)
                ? GColor::convertColorToARGB(rgbBackground)
                : GColor::convertColorToRGB(rgbBackground),
         parent);
}

void GCanvas::init(double width, double height, int rgbBackground, QWidget* parent) {
    checkSize("GCanvas::constructor", width, height);
    checkColor("GCanvas::constructor", rgbBackground);

    GThread::runOnQtGuiThread([this, rgbBackground, parent]() {
        _iqcanvas = new _Internal_QCanvas(this, getInternalParent(parent));
        _gcompound.setWidget(_iqcanvas);
        int alpha = getAlpha(rgbBackground);
        if (GColor::hasAlpha(_backgroundColor)) {
            // empty
        } else if (alpha > 0 && alpha < 255) {
            _backgroundColor = GColor::convertARGBToColor(rgbBackground);
        } else {
            _backgroundColor = GColor::convertRGBToColor(rgbBackground);
        }
        _backgroundColorInt = rgbBackground;
    });

    if (width > 0 && height > 0) {
        setSize(width, height);
        if (rgbBackground != 0) {
            fill(rgbBackground);
        }
    }

    setVisible(false);   // all widgets are not shown until added to a window
}

GCanvas::~GCanvas() {
    // TODO: delete _GCanvas;
    _iqcanvas = nullptr;
}

void GCanvas::add(GObject* gobj) {
    GThread::runOnQtGuiThread([this, gobj]() {
        lockForWrite();
        _gcompound.add(gobj);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::add(GObject* gobj, double x, double y) {
    GThread::runOnQtGuiThread([this, gobj, x, y]() {
        lockForWrite();
        _gcompound.add(gobj, x, y);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::add(GObject& gobj) {
    GThread::runOnQtGuiThread([this, &gobj]() {
        lockForWrite();
        _gcompound.add(gobj);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::add(GObject& gobj, double x, double y) {
    GThread::runOnQtGuiThread([this, &gobj, x, y]() {
        lockForWrite();
        _gcompound.add(gobj, x, y);   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::clear() {
    clearObjects();
    clearPixels();   // calls conditionalRepaint
}

void GCanvas::clearObjects() {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _gcompound.clear();   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::clearPixels() {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        if (_backgroundImage) {
            // delete _backgroundImage;
            // _backgroundImage = nullptr;
            // keep background image buffer but fill with background color instead
            GThread::runOnQtGuiThread([this]() {
                if (GColor::hasAlpha(_backgroundColor)) {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt));
                } else {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt) | 0xff000000);
                }
            });
        }
        unlock();
    });
    conditionalRepaint();
}

bool GCanvas::contains(double x, double y) const {
    lockForReadConst();
    bool result = _gcompound.contains(x, y);
    unlockConst();
    return result;
}

int GCanvas::countDiffPixels(const GCanvas& image) const {
    lockForReadConst();
    int w1 = static_cast<int>(getWidth());
    int h1 = static_cast<int>(getHeight());
    int w2 = static_cast<int>(image.getWidth());
    int h2 = static_cast<int>(image.getHeight());

    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);

    int overlap = std::min(w1, w2) * std::min(h1, h2);
    int diffPxCount = (w1 * h1 - overlap) + (w2 * h2 - overlap);

    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = _backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            int px2 = image._backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    unlockConst();
    return diffPxCount;
}

int GCanvas::countDiffPixels(const GCanvas& image, int xmin, int ymin, int xmax, int ymax) const {
    lockForReadConst();
    int w1 = static_cast<int>(getWidth());
    int h1 = static_cast<int>(getHeight());
    int w2 = static_cast<int>(image.getWidth());
    int h2 = static_cast<int>(image.getHeight());
    int diffPxCount = 0;

    for (int y = ymin; y < ymax; y++) {
        for (int x = xmin; x < xmax; x++) {
            int px1 = x < w1 && y < h1 ? (_backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff) : -1;
            int px2 = x < w2 && y < h2 ? (image._backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff) : -1;
            if (px1 != px2) {
                diffPxCount++;
            }
        }
    }

    unlockConst();
    return diffPxCount;
}

int GCanvas::countDiffPixels(const GCanvas* image) const {
    require::nonNull(image, "GCanvas::countDiffPixels");
    return countDiffPixels(*image);
}

int GCanvas::countDiffPixels(const GCanvas* image, int xmin, int ymin, int xmax, int ymax) const {
    require::nonNull(image, "GCanvas::countDiffPixels");
    return countDiffPixels(*image, xmin, ymin, xmax, ymax);
}

GCanvas* GCanvas::diff(const GCanvas& image, int diffPixelColor) const {
    lockForReadConst();
    int w1 = static_cast<int>(getWidth());
    int h1 = static_cast<int>(getHeight());
    int w2 = static_cast<int>(image.getWidth());
    int h2 = static_cast<int>(image.getHeight());
    int wmin = std::min(w1, w2);
    int hmin = std::min(h1, h2);
    int wmax = std::max(w1, w2);
    int hmax = std::max(h1, h2);

    Grid<int> resultGrid;
    resultGrid.resize(hmax, wmax);
    resultGrid.fill(diffPixelColor);
    for (int r = 0; r < h1; r++) {
        for (int c = 0; c < w1; c++) {
            resultGrid[r][c] = _backgroundColorInt;
        }
    }
    for (int y = 0; y < hmin; y++) {
        for (int x = 0; x < wmin; x++) {
            int px1 = _backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            int px2 = image._backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
            if (px1 != px2) {
                resultGrid[y][x] = diffPixelColor;
            }
        }
    }
    GCanvas* result = new GCanvas(wmax, hmax);
    result->fromGrid(resultGrid);
    unlockConst();
    return result;
}

GCanvas* GCanvas::diff(const GCanvas* image, int diffPixelColor) const {
    require::nonNull(image, "GCanvas::diff");
    return diff(*image, diffPixelColor);
}

void GCanvas::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    // lockForRead();
    if (_backgroundImage) {
        painter->drawImage(/* x */ 0, /* y */ 0, *_backgroundImage);
    }
    _gcompound.draw(painter);   // calls conditionalRepaint
    // unlock();
}

void GCanvas::draw(GObject* gobj) {
    require::nonNull(gobj, "GCanvas::draw");
    ensureBackgroundImage();
    if (_backgroundImage && _backgroundImage->paintEngine()) {
        GThread::runOnQtGuiThread([this, gobj]() {
            lockForWrite();
            if (gobj->isVisible()) {
                QPainter painter(_backgroundImage);
                painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
                painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
                gobj->draw(&painter);
                painter.end();
            }
            unlock();
        });
        conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
    }
}

void GCanvas::ensureBackgroundImage() {
    if (!_backgroundImage) {
        GThread::runOnQtGuiThread([this]() {
            lockForWrite();
            _backgroundImage = new QImage(
                        static_cast<int>(getWidth()),
                        static_cast<int>(getHeight()),
                        QImage::Format_ARGB32);
            if (!_backgroundColor.empty()) {
                if (GColor::hasAlpha(_backgroundColor)) {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt));
                } else {
                    _backgroundImage->fill(static_cast<unsigned int>(_backgroundColorInt) | 0xff000000);
                }
            }
            unlock();
        });
    }
}

void GCanvas::ensureBackgroundImageConstHack() const {
    if (!_backgroundImage) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        GCanvas* hack = const_cast<GCanvas*>(this);
        hack->ensureBackgroundImage();
    }
}

bool GCanvas::equals(const GCanvas& other) const {
    if (getSize() != other.getSize()) {
        return false;
    }
    // TODO: compare pixels
    return true;
}

void GCanvas::fill(int rgb) {
    checkColor("GCanvas::fill", rgb);
    fillRegion(/* x */ 0,
               /* y */ 0,
               /* width */ getWidth(),
               /* height */ getHeight(),
               rgb);
}

void GCanvas::fill(const std::string& rgb) {
    fill(GColor::convertColorToRGB(rgb));
}

void GCanvas::fillRegion(double x, double y, double width, double height, int rgb) {
    checkBounds("GCanvas::fillRegion", x, y, getWidth(), getHeight());
    checkBounds("GCanvas::fillRegion", x + width - 1, y + height - 1, getWidth(), getHeight());
    checkColor("GCanvas::fillRegion", rgb);
    bool wasAutoRepaint = isAutoRepaint();
    setAutoRepaint(false);
    GThread::runOnQtGuiThread([this, x, y, width, height, rgb]() {
        ensureBackgroundImage();
        lockForWrite();

        // fix alpha for 0-alpha non-0-rgb ints
        int argb = rgb;
        if (rgb != 0 && getAlpha(rgb) == 0) {
            argb = rgb | 0xff000000;
        }

        for (int yy = static_cast<int>(y); yy < y + height; yy++) {
            for (int xx = static_cast<int>(x); xx < x + width; xx++) {
                _backgroundImage->setPixel(xx, yy, static_cast<unsigned int>(argb));
            }
        }
        unlock();
    });
    setAutoRepaint(wasAutoRepaint);
    conditionalRepaint();
}

void GCanvas::fillRegion(double x, double y, double width, double height, const std::string& rgb) {
    fillRegion(x, y, width, height, GColor::convertColorToRGB(rgb));
}

void GCanvas::flatten() {
    GThread::runOnQtGuiThread([this]() {
        ensureBackgroundImage();
        lockForWrite();
        QPainter painter(_backgroundImage);
        painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
        painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
        _gcompound.draw(&painter);
        painter.end();
        _gcompound.clear();   // calls conditionalRepaint
        unlock();
    });
}

void GCanvas::fromGrid(const Grid<int>& grid) {
    checkSize("GCanvas::fromGrid", grid.width(), grid.height());
    setSize(grid.width(), grid.height());

    bool wasAutoRepaint = isAutoRepaint();
    setAutoRepaint(false);

    GThread::runOnQtGuiThread([this, &grid]() {
        ensureBackgroundImage();
        lockForWrite();
        for (int row = 0, width = grid.width(), height = grid.height(); row < height; row++) {
            for (int col = 0; col < width; col++) {
                // setPixel(col, row, grid[row][col]);
                _backgroundImage->setPixel(col, row, static_cast<unsigned int>(grid[row][col]) | 0xff000000);
            }
        }
        unlock();
    });

    if (wasAutoRepaint) {
        setAutoRepaint(wasAutoRepaint);
        conditionalRepaint();
    }
}

std::string GCanvas::getBackground() const {
    return GDrawingSurface::getBackground();
}

int GCanvas::getBackgroundInt() const {
    return GDrawingSurface::getBackgroundInt();
}

GObject* GCanvas::getElement(int index) const {
    lockForReadConst();
    GObject* result = _gcompound.getElement(index);
    unlockConst();
    return result;
}

GObject* GCanvas::getElementAt(double x, double y) const {
    lockForReadConst();
    GObject* result = _gcompound.getElementAt(x, y);
    unlockConst();
    return result;
}

int GCanvas::getElementCount() const {
    lockForReadConst();
    int result = _gcompound.getElementCount();
    unlockConst();
    return result;
}

std::string GCanvas::getFilename() const {
    return _filename;
}

std::string GCanvas::getFont() const {
    return GDrawingSurface::getFont();
}

_Internal_QWidget* GCanvas::getInternalWidget() const {
    return _iqcanvas;
}

int GCanvas::getPixel(double x, double y) const {
    checkBounds("GCanvas::getPixel", x, y, getWidth(), getHeight());
    ensureBackgroundImageConstHack();
    lockForReadConst();
    int pixel = _backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)) & 0x00ffffff;
    unlockConst();
    return pixel;
}

int GCanvas::getPixelARGB(double x, double y) const {
    checkBounds("GCanvas::getPixelARGB", x, y, getWidth(), getHeight());
    ensureBackgroundImageConstHack();
    lockForReadConst();
    int pixel = static_cast<int>(_backgroundImage->pixel(static_cast<int>(x), static_cast<int>(y)));
    unlockConst();
    return pixel;
}

Grid<int> GCanvas::getPixels() const {
    ensureBackgroundImageConstHack();
    lockForReadConst();
    Grid<int> grid(static_cast<int>(getHeight()), static_cast<int>(getWidth()));
    for (int y = 0; y < static_cast<int>(getHeight()); y++) {
        for (int x = 0; x < static_cast<int>(getWidth()); x++) {
            grid[y][x] = _backgroundImage->pixel(x, y) & 0x00ffffff;
        }
    }
    unlockConst();
    return grid;
}

Grid<int> GCanvas::getPixelsARGB() const {
    ensureBackgroundImageConstHack();
    lockForReadConst();
    Grid<int> grid(static_cast<int>(getHeight()), static_cast<int>(getWidth()));
    for (int y = 0; y < static_cast<int>(getHeight()); y++) {
        for (int x = 0; x < static_cast<int>(getWidth()); x++) {
            grid[y][x] = static_cast<int>(_backgroundImage->pixel(x, y));
        }
    }
    unlockConst();
    return grid;
}

std::string GCanvas::getType() const {
    return "GCanvas";
}

QWidget* GCanvas::getWidget() const {
    return static_cast<QWidget*>(_iqcanvas);
}

bool GCanvas::isAutoRepaint() const {
    return _gcompound.isAutoRepaint();
}

void GCanvas::load(const std::string& filename) {
    // for efficiency, let's at least check whether the file exists
    // and throw error immediately rather than contacting the back-end
    if (!fileExists(filename)) {
        error("GCanvas::load: file not found: " + filename);
    }

    bool hasError = false;
    GThread::runOnQtGuiThread([this, filename, &hasError]() {
        ensureBackgroundImage();
        lockForWrite();
        if (!_backgroundImage->load(QString::fromStdString(filename))) {
            hasError = true;
            return;
        }

        _filename = filename;
        GInteractor::setSize(_backgroundImage->width(), _backgroundImage->height());
        // setSize(_qimage->width(), _qimage->height());
        unlock();
        conditionalRepaint();
    });

    if (hasError) {
        error("GCanvas::load: failed to load from " + filename);
    }
}

void GCanvas::notifyOfResize(double width, double height) {
    if (_backgroundImage) {
        GThread::runOnQtGuiThread([this, width, height]() {
            // make new image buffer of the new size
            lockForWrite();
            QImage* newImage = new QImage(
                        static_cast<int>(width),
                        static_cast<int>(height), QImage::Format_ARGB32);
            if (!_backgroundColor.empty()) {
                if (GColor::hasAlpha(_backgroundColor)) {
                    newImage->fill(static_cast<unsigned int>(_backgroundColorInt));
                } else {
                    newImage->fill(static_cast<unsigned int>(_backgroundColorInt) | 0xff000000);
                }
            }

            // draw any previous contents onto it
            if (newImage->paintEngine()) {
                QPainter painter(newImage);
                painter.drawImage(0, 0, *_backgroundImage);
            }

            // TODO: delete _backgroundImage;
            _backgroundImage = newImage;
            unlock();
            conditionalRepaint();
        });
    }
}

void GCanvas::remove(GObject* gobj) {
    GThread::runOnQtGuiThread([this, gobj]() {
        lockForWrite();
        _gcompound.remove(gobj);
        unlock();
    });
}

void GCanvas::remove(GObject& gobj) {
    GThread::runOnQtGuiThread([this, &gobj]() {
        lockForWrite();
        _gcompound.remove(gobj);
        unlock();
    });
}

void GCanvas::removeAll() {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _gcompound.removeAll();
        unlock();
    });
}

void GCanvas::removeClickListener() {
    removeEventListener("click");
}

void GCanvas::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GCanvas::removeKeyListener() {
    removeEventListeners({"keypress",
                         "keyrelease",
                         "keytype"});
}

void GCanvas::removeMouseListener() {
    removeEventListeners({"click",
                         "mousedrag",
                         "mouseenter",
                         "mouseexit",
                         "mousemove",
                         "mousepress",
                         "mouserelease",
                         "mousewheeldown",
                         "mousewheelup"});
}

void GCanvas::repaint() {
    GThread::runOnQtGuiThreadAsync([this]() {
        lockForRead();
        getWidget()->repaint();
        unlock();
        // _gcompound.repaint();   // runs on Qt GUI thread
    });
}

void GCanvas::repaintRegion(int x, int y, int width, int height) {
    GThread::runOnQtGuiThreadAsync([this, x, y, width, height]() {
        lockForRead();
        getWidget()->repaint(x, y, width, height);
        unlock();
        // _gcompound.repaintRegion(x, y, width, height);   // runs on Qt GUI thread
    });
}

void GCanvas::resize(double width, double height, bool /* retain */) {
    checkSize("GCanvas::resize", width, height);

    // TODO
    setSize(width, height);

    conditionalRepaint();
}

void GCanvas::save(const std::string& filename) {
    GThread::runOnQtGuiThread([this, filename]() {
        ensureBackgroundImage();
        lockForRead();
        if (!_gcompound.isEmpty()) {
            // flatten image in a copy object, then save
            QImage imageCopy = this->_backgroundImage->copy(
                        0, 0,
                        static_cast<int>(getWidth()), static_cast<int>(getHeight()));
            QPainter painter(&imageCopy);
            painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
            painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
            _gcompound.draw(&painter);
            painter.end();
            bool result = imageCopy.save(QString::fromStdString(filename));
            unlock();
            if (!result) {
                error("GCanvas::save: failed to save to " + filename);
            }
        } else {
            // save it myself
            bool result = _backgroundImage->save(QString::fromStdString(filename));
            unlock();
            if (!result) {
                error("GCanvas::save: failed to save to " + filename);
            }
        }
    });
    _filename = filename;
}

void GCanvas::setAutoRepaint(bool autoRepaint) {
    GThread::runOnQtGuiThread([this, autoRepaint]() {
        lockForWrite();
        _gcompound.setAutoRepaint(autoRepaint);
        unlock();
    });
}

void GCanvas::setBackground(int color) {
    GDrawingSurface::setBackground(color);
    GInteractor::setBackground(color);
    if (_backgroundImage) {
        // Slightly confusing semantics here;
        // if you draw some shapes on a canvas (with draw() variants) and then set its background,
        // the shapes will get wiped out.
        // The lesson is, set the background first before drawing stuff.
        // Or add your shapes using add() rather than draw() so they sit atop the background.
        GThread::runOnQtGuiThread([this, color]() {
            lockForWrite();
            _backgroundImage->fill(static_cast<unsigned int>(color));
            unlock();
        });
        conditionalRepaint();
    }
}

void GCanvas::setBackground(const std::string& color) {
    setBackground(GColor::convertColorToRGB(color));
}

void GCanvas::setClickListener(GEventListener func) {
    setEventListener("click", func);
}

void GCanvas::setClickListener(GEventListenerVoid func) {
    setEventListener("click", func);
}

void GCanvas::setColor(int color) {
    GDrawingSurface::setColor(color);
}

void GCanvas::setColor(const std::string& color) {
    setColor(GColor::convertColorToRGB(color));
}

void GCanvas::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GCanvas::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GCanvas::setFont(const QFont& font) {
    GDrawingSurface::setFont(font);
}

void GCanvas::setFont(const std::string& font) {
    GDrawingSurface::setFont(font);
}

void GCanvas::setForeground(int color) {
    GDrawingSurface::setForeground(color);
}

void GCanvas::setForeground(const std::string& color) {
    setForeground(GColor::convertColorToRGB(color));
}

void GCanvas::setKeyListener(GEventListener func) {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _iqcanvas->setFocusPolicy(Qt::StrongFocus);
        unlock();
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GCanvas::setKeyListener(GEventListenerVoid func) {
    GThread::runOnQtGuiThread([this]() {
        lockForWrite();
        _iqcanvas->setFocusPolicy(Qt::StrongFocus);
        unlock();
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GCanvas::setMouseListener(GEventListener func) {
    setEventListeners({"click",
                       "mousedrag",
                       "mouseenter",
                       "mouseexit",
                       "mousemove",
                       "mousepress",
                       "mouserelease",
                       "mousewheeldown",
                       "mousewheelup"}, func);
}

void GCanvas::setMouseListener(GEventListenerVoid func) {
    setEventListeners({"click",
                       "mousedrag",
                       "mouseenter",
                       "mouseexit",
                       "mousemove",
                       "mousepress",
                       "mouserelease",
                       "mousewheeldown",
                       "mousewheelup"}, func);
}

void GCanvas::setPixel(double x, double y, int rgb) {
    require::inRange2D(x, y, getWidth(), getHeight(), "GCanvas::setPixel", "x", "y");
    checkColor("GCanvas::setPixel", rgb);
    GThread::runOnQtGuiThread([this, x, y, rgb]() {
        ensureBackgroundImage();
        lockForWrite();
        _backgroundImage->setPixel(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<unsigned int>(rgb) | 0xff000000);
        unlock();
        conditionalRepaintRegion(
                static_cast<int>(x),
                static_cast<int>(y),
                /* width */ 1,
                /* height */ 1);
    });
}

void GCanvas::setPixel(double x, double y, int r, int g, int b) {
    setPixel(x, y, GColor::convertRGBToRGB(r, g, b) | 0xff000000);
}

void GCanvas::setPixelARGB(double x, double y, int argb) {
    require::inRange2D(x, y, getWidth(), getHeight(), "GCanvas::setPixelARGB", "x", "y");
    checkColor("GCanvas::setPixel", argb);
    GThread::runOnQtGuiThread([this, x, y, argb]() {
        ensureBackgroundImage();
        lockForWrite();
        _backgroundImage->setPixel((int) x, (int) y, argb);
        unlock();
        conditionalRepaintRegion((int) x, (int) y, /* width */ 1, /* height */ 1);
    });
}

void GCanvas::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    setPixelARGB(x, y, GColor::convertARGBToARGB(a, r, g, b));
}

void GCanvas::setPixels(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.width() != (int) getWidth() || pixels.height() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("GCanvas::setPixels: wrong size");
    }
    GThread::runOnQtGuiThread([this, &pixels]() {
        lockForWrite();
        for (int y = 0; y < pixels.height(); y++) {
            for (int x = 0; x < pixels.width(); x++) {
                _backgroundImage->setPixel(x, y, pixels[y][x]);
            }
        }
        unlock();
        conditionalRepaint();
    });
}

void GCanvas::setPixelsARGB(const Grid<int>& pixels) {
    ensureBackgroundImage();
    if (pixels.width() != (int) getWidth() || pixels.height() != (int) getHeight()) {
        // TODO
        // resize(pixels.width(), pixels.height());
        error("GCanvas::setPixels: wrong size");
    }

    GThread::runOnQtGuiThread([this, &pixels]() {
        lockForWrite();
        for (int y = 0; y < pixels.height(); y++) {
            for (int x = 0; x < pixels.width(); x++) {
                _backgroundImage->setPixel(x, y, pixels[y][x]);
            }
        }
        unlock();
        conditionalRepaint();
    });
}

GImage* GCanvas::toGImage() const {
    ensureBackgroundImageConstHack();
    lockForReadConst();
    GImage* image = new GImage(_backgroundImage);
    unlockConst();
    return image;

//    GCanvas* that = const_cast<GCanvas*>(this);
//    QImage* backgroundImage = _backgroundImage;
//    GImage* gimage = nullptr;
//    GThread::runOnQtGuiThread([that, backgroundImage, &gimage]() {
//        QImage* copy = new QImage((int) that->getWidth(), (int) that->getHeight(), QImage::Format_ARGB32);
//        *copy = backgroundImage->copy();
//        QPainter painter(copy);
//        painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
//        painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
//        that->_gcompound.draw(&painter);
//        painter.end();

//        gimage = new GImage(copy);
//    });
//    return gimage;
}

Grid<int> GCanvas::toGrid() const {
    Grid<int> grid;
    toGrid(grid);
    return grid;
}

void GCanvas::toGrid(Grid<int>& grid) const {
    grid.resize(getHeight(), getWidth());
    lockForReadConst();
    for (int row = 0, width = (int) getWidth(), height = (int) getHeight(); row < height; row++) {
        for (int col = 0; col < width; col++) {
            grid[row][col] = _backgroundImage->pixel(col, row);
        }
    }
    unlockConst();
}


_Internal_QCanvas::_Internal_QCanvas(GCanvas* gcanvas, QWidget* parent)
        : QWidget(parent),
          _gcanvas(gcanvas) {
    require::nonNull(gcanvas, "_Internal_QCanvas::constructor");
    setObjectName(QString::fromStdString("_Internal_QCanvas_" + std::to_string(gcanvas->getID())));

    // set default white background color
//    QPalette pal = palette();
//    pal.setColor(QPalette::Background, Qt::white);
//    setAutoFillBackground(true);
//    setPalette(pal);
    setMouseTracking(true);   // causes mouse move events to occur
}

void _Internal_QCanvas::enterEvent(QEvent* event) {
    QWidget::enterEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mouseenter")) return;
    _gcanvas->fireGEvent(event, MOUSE_ENTERED, "mouseenter");
}

void _Internal_QCanvas::keyPressEvent(QKeyEvent* event) {
    QWidget::keyPressEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("keypress")) return;
    _gcanvas->fireGEvent(event, KEY_PRESSED, "keypress");
}

void _Internal_QCanvas::keyReleaseEvent(QKeyEvent* event) {
    QWidget::keyReleaseEvent(event);   // call super
    if (_gcanvas->isAcceptingEvent("keyrelease")) {
        _gcanvas->fireGEvent(event, KEY_RELEASED, "keyrelease");
    }
    if (_gcanvas->isAcceptingEvent("keytype")) {
        _gcanvas->fireGEvent(event, KEY_TYPED, "keytype");
    }
}

void _Internal_QCanvas::leaveEvent(QEvent* event) {
    QWidget::leaveEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mouseexit")) return;
    _gcanvas->fireGEvent(event, MOUSE_EXITED, "mouseexit");
}

void _Internal_QCanvas::mouseDoubleClickEvent(QMouseEvent* event) {
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gcanvas->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gcanvas);
    mouseEvent.setActionCommand(_gcanvas->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gcanvas->fireEvent(mouseEvent);
}

void _Internal_QCanvas::mouseMoveEvent(QMouseEvent* event) {
    QWidget::mouseMoveEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mousemove")
            && !_gcanvas->isAcceptingEvent("mousedrag")) return;
    _gcanvas->fireGEvent(event, MOUSE_MOVED, "mousemove");
    if (event->buttons() != 0) {
        // mouse drag
        _gcanvas->fireGEvent(event, MOUSE_DRAGGED, "mousedrag");
    }
}

void _Internal_QCanvas::mousePressEvent(QMouseEvent* event) {
    QWidget::mousePressEvent(event);   // call super
    if (!_gcanvas->isAcceptingEvent("mousepress")) return;
    _gcanvas->fireGEvent(event, MOUSE_PRESSED, "mousepress");
}

void _Internal_QCanvas::mouseReleaseEvent(QMouseEvent* event) {
    QWidget::mouseReleaseEvent(event);   // call super
    if (_gcanvas->isAcceptingEvent("mouserelease")) {
        _gcanvas->fireGEvent(event, MOUSE_RELEASED, "mouserelease");
    }

    if (_gcanvas->isAcceptingEvent("click")) {
        _gcanvas->fireGEvent(event, MOUSE_CLICKED, "click");
    }
}

void _Internal_QCanvas::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);   // call super

    QPainter painter(this);
    // g.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    // g.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
    painter.setRenderHint(QPainter::TextAntialiasing, GObject::isAntiAliasing());
    _gcanvas->draw(&painter);
    painter.end();
}

void _Internal_QCanvas::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);   // call super
    QSize size = event->size();
    _gcanvas->notifyOfResize(size.width(), size.height());
}

void _Internal_QCanvas::setCanvasSize(double width, double height) {
    QWidget::setMinimumSize(QSize((int) width, (int) height));
}

QSize _Internal_QCanvas::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}

void _Internal_QCanvas::wheelEvent(QWheelEvent* event) {
    QWidget::wheelEvent(event);   // call super
    if (event->pixelDelta().y() < 0) {
        // scroll down
        if (_gcanvas->isAcceptingEvent("mousewheeldown")) {
            _gcanvas->fireGEvent(event, MOUSE_WHEEL_DOWN, "mousewheeldown");
        }
    } else if (event->pixelDelta().y() > 0) {
        // scroll up
        if (_gcanvas->isAcceptingEvent("mousewheelup")) {
            _gcanvas->fireGEvent(event, MOUSE_WHEEL_UP, "mousewheelup");
        }
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gcanvas.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: ginteractor.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/09/20
 * - bug fix for requestFocus threading
 * @version 2018/09/04
 * - added get/setName, getID
 * @version 2018/08/23
 * - renamed to ginteractor.cpp to replace Java version
 * @version 2018/06/29
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#include <iostream>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

int GInteractor::_interactorCount = 0;

GInteractor::GInteractor()
        : _actionCommand(""),
          _icon(""),
          _name(""),
          _id(-1),
          _container(nullptr),
          _lock(QReadWriteLock::Recursive) {
    QtGui::instance()->initializeQt();   // make sure Qt system is initialized
    _id = ++_interactorCount;            // set ID to number of interactors + 1
}

GInteractor::~GInteractor() {
    // empty
}

void GInteractor::addActionListener() {
    // empty
}

bool GInteractor::eventsEnabled() const {
    return GObservable::eventsEnabled() && getWidget() != nullptr && isVisible();
}

std::string GInteractor::getAccelerator() const {
    // override in subclasses
    return "";
}

std::string GInteractor::getActionCommand() const {
    return _actionCommand;
}

std::string GInteractor::getBackground() const {
    int rgb = getBackgroundInt();
    return GColor::convertRGBToColor(rgb);
}

int GInteractor::getBackgroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->backgroundRole());
    return GColor::convertQColorToRGB(color);
}

GRectangle GInteractor::getBounds() const {
    return GRectangle(getX(), getY(), getWidth(), getHeight());
}

std::string GInteractor::getColor() const {
    int rgb = getColorInt();
    return GColor::convertRGBToColor(rgb);
}

int GInteractor::getColorInt() const {
    QColor color = getWidget()->palette().color(getWidget()->foregroundRole());
    return GColor::convertQColorToRGB(color);
}

GContainer* GInteractor::getContainer() const {
    return _container;
}

std::string GInteractor::getDefaultInteractorName() const {
    return getType() + "_" + std::to_string(getID());
}

std::string GInteractor::getFont() const {
    return GFont::toFontString(getWidget()->font());
}

std::string GInteractor::getForeground() const {
    int rgb = getForegroundInt();
    return GColor::convertRGBToColor(rgb);
}

int GInteractor::getForegroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->foregroundRole());
    return GColor::convertQColorToRGB(color);
}

double GInteractor::getHeight() const {
    return getWidget()->height();
}

int GInteractor::getID() const {
    return _id;
}

std::string GInteractor::getIcon() const {
    return _icon;
}

GPoint GInteractor::getLocation() const {
    return GPoint(getX(), getY());
}

/* static */ QWidget* GInteractor::getInternalParent(QWidget* parent) {
    return parent ? parent : (QWidget*) GWindow::getLastWindow();
}

double GInteractor::getMinimumHeight() const {
    return getMinimumSize().getHeight();
}

GDimension GInteractor::getMinimumSize() const {
    QSize size = getInternalWidget()->getMinimumSize();
    return GDimension(size.width(), size.height());
}

double GInteractor::getMinimumWidth() const {
    return getMinimumSize().getWidth();
}

char GInteractor::getMnemonic() const {
    // TODO
    return '?';
}

std::string GInteractor::getName() const {
    if (_name.empty()) {
        return getDefaultInteractorName();
    } else {
        return _name;
    }
}

double GInteractor::getPreferredHeight() const {
    return getPreferredSize().getHeight();
}

GDimension GInteractor::getPreferredSize() const {
    QSize size;
    if (getInternalWidget()->hasPreferredSize()) {
        size = getInternalWidget()->getPreferredSize();
    } else {
        size = getWidget()->sizeHint();
    }
    return GDimension(size.width(), size.height());
}

double GInteractor::getPreferredWidth() const {
    return getPreferredSize().getWidth();
}

GDimension GInteractor::getSize() const {
    return GDimension(getWidth(), getHeight());
}

double GInteractor::getWidth() const {
    return getWidget()->width();
}

double GInteractor::getX() const {
    return getWidget()->x();
}

double GInteractor::getY() const {
    return getWidget()->y();
}

bool GInteractor::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool GInteractor::inBounds(int x, int y) const {
    return 0 <= x && x < (int) getWidth() && 0 <= y && y < (int) getHeight();
}

bool GInteractor::isEnabled() const {
    return getWidget()->isEnabled();
}

bool GInteractor::isVisible() const {
    return getWidget()->isVisible();
}

void GInteractor::lockForRead() {
    _lock.lockForRead();
}

void GInteractor::lockForReadConst() const {
    GInteractor* that = const_cast<GInteractor*>(this);
    that->lockForRead();
}

void GInteractor::lockForWrite() {
    _lock.lockForWrite();
}

void GInteractor::lockForWriteConst() const {
    GInteractor* that = const_cast<GInteractor*>(this);
    that->lockForWrite();
}

void GInteractor::unlock() {
    _lock.unlock();
}

void GInteractor::unlockConst() const {
    GInteractor* that = const_cast<GInteractor*>(this);
    that->unlock();
}

std::string GInteractor::normalizeAccelerator(const std::string& accelerator) {
    std::string acceleratorStr = stringReplace(accelerator, "Alt-", "Alt+");
    acceleratorStr = stringReplace(acceleratorStr, "Command-", "Command+");
    acceleratorStr = stringReplace(acceleratorStr, "Ctrl-", "Ctrl+");
    acceleratorStr = stringReplace(acceleratorStr, "Meta-", "Meta+");
    acceleratorStr = stringReplace(acceleratorStr, "Shift-", "Shift+");
    return acceleratorStr;
}

void GInteractor::requestFocus() {
    GThread::runOnQtGuiThread([this]() {
        getWidget()->setFocus();
    });
}

void GInteractor::setActionCommand(const std::string& actionCommand) {
    _actionCommand = actionCommand;
}

void GInteractor::setAccelerator(const std::string& /* accelerator */) {
    // override in subclasses
}

void GInteractor::setBackground(int rgb) {
    GThread::runOnQtGuiThread([this, rgb]() {
        QPalette palette(getWidget()->palette());
        palette.setColor(getWidget()->backgroundRole(), QColor(rgb));

        // additional palette color settings for GChooser and other widgets
        // TODO: does not totally work for some widgets, e.g. GChooser popup menu
        if (getType() == "GChooser") {
            palette.setColor(QPalette::Base, QColor(rgb));
            palette.setColor(QPalette::Active, QPalette::Button, QColor(rgb));
            palette.setColor(QPalette::Inactive, QPalette::Button, QColor(rgb));
        }

        getWidget()->setAutoFillBackground(true);
        getWidget()->setPalette(palette);
    });
}

void GInteractor::setBackground(const std::string& color) {
    if (GColor::hasAlpha(color)) {
        int argb = GColor::convertColorToARGB(color);
        GThread::runOnQtGuiThread([this, argb]() {
            QColor qcolor = GColor::toQColorARGB(argb);
            QPalette palette(getWidget()->palette());
            palette.setColor(getWidget()->backgroundRole(), qcolor);

            // additional palette color settings for GChooser and other widgets
            // TODO: does not totally work for some widgets, e.g. GChooser popup menu
            if (getType() == "GChooser") {
                palette.setColor(QPalette::Base, qcolor);
                palette.setColor(QPalette::Active, QPalette::Button, qcolor);
                palette.setColor(QPalette::Inactive, QPalette::Button, qcolor);
            }

            getWidget()->setAutoFillBackground(true);
            getWidget()->setPalette(palette);
        });
    } else {
        int rgb = GColor::convertColorToRGB(color);
        setBackground(rgb);
    }
}

void GInteractor::setBounds(double x, double y, double width, double height) {
    GThread::runOnQtGuiThread([this, x, y, width, height]() {
        getWidget()->setGeometry((int) x, (int) y, (int) width, (int) height);
        getWidget()->setFixedSize((int) width, (int) height);
    });
}

void GInteractor::setBounds(const GRectangle& size) {
    setBounds(size.getX(), size.getY(), size.getWidth(), size.getHeight());
}

void GInteractor::setColor(int rgb) {
    setForeground(rgb);
}

void GInteractor::setColor(const std::string& color) {
    setForeground(color);
}

void GInteractor::setContainer(GContainer* container) {
    _container = container;
    if (!container) {
        // widgets that are not in any container should not be shown on screen
        // (they will awkwardly hover at (0, 0) if they are shown)
        QWidget* widget = getWidget();
        GThread::runOnQtGuiThread([widget]() {
            widget->setParent(nullptr);
        });
        setVisible(false);
    }
}

void GInteractor::setEnabled(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        getWidget()->setEnabled(value);
    });
}

void GInteractor::setForeground(int rgb) {
    GThread::runOnQtGuiThread([this, rgb]() {
        QPalette palette(getWidget()->palette());
        palette.setColor(getWidget()->foregroundRole(), QColor(rgb));
        // TODO: does not totally work for some widgets, e.g. GChooser popup menu
        getWidget()->setPalette(palette);
    });
}

void GInteractor::setForeground(const std::string& color) {
    if (GColor::hasAlpha(color)) {
        int argb = GColor::convertColorToARGB(color);
        GThread::runOnQtGuiThread([this, argb]() {
            QPalette palette(getWidget()->palette());
            palette.setColor(getWidget()->foregroundRole(), GColor::toQColorARGB(argb));
            // TODO: does not totally work for some widgets, e.g. GChooser popup menu
            getWidget()->setPalette(palette);
        });
    } else {
        int rgb = GColor::convertColorToRGB(color);
        setForeground(rgb);
    }
}

void GInteractor::setFont(const QFont& font) {
    GThread::runOnQtGuiThread([this, font]() {
        getWidget()->setFont(font);
    });
}

void GInteractor::setFont(const std::string& font) {
    setFont(GFont::toQFont(font));
}

void GInteractor::setHeight(double height) {
    require::nonNegative(height, "GInteractor::setHeight", "height");
    GThread::runOnQtGuiThread([this, height]() {
        getWidget()->setFixedHeight((int) height);
    });
}

void GInteractor::setIcon(const std::string& filename, bool /* retainIconSize */) {
    _icon = filename;

    // override in subclasses as appropriate; make sure to call super
}

void GInteractor::setLocation(double x, double y) {
    GThread::runOnQtGuiThread([this, x, y]() {
        getWidget()->setGeometry(x, y, getWidth(), getHeight());
    });
}

void GInteractor::setMinimumSize(double width, double height) {
    require::nonNegative(width, "GInteractor::setMinimumSize", "width");
    require::nonNegative(height, "GInteractor::setMinimumSize", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        getInternalWidget()->setMinimumSize(width, height);
    });
}

void GInteractor::setMinimumSize(const GDimension& size) {
    setMinimumSize(size.getWidth(), size.getHeight());
}

void GInteractor::setMnemonic(char /* mnemonic */) {
    // empty; use an & before mnemonic character in interactor's text instead
}

void GInteractor::setName(const std::string& name) {
    _name = name;
    // TODO: getWidget()->setObjectName() ?
}

void GInteractor::setPreferredHeight(double height) {
    setPreferredSize(getPreferredWidth(), height);
}

void GInteractor::setPreferredSize(double width, double height) {
    require::nonNegative(width, "GInteractor::setPreferredSize", "width");
    require::nonNegative(height, "GInteractor::setPreferredSize", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        getInternalWidget()->setPreferredSize(width, height);
    });
}

void GInteractor::setPreferredSize(const GDimension& size) {
    GThread::runOnQtGuiThread([this, size]() {
        getInternalWidget()->setPreferredSize(size.getWidth(), size.getHeight());
    });
}

void GInteractor::setPreferredWidth(double width) {
    setPreferredSize(width, getPreferredHeight());
}

void GInteractor::setSize(double width, double height) {
    require::nonNegative(width, "GInteractor::setSize", "width");
    require::nonNegative(height, "GInteractor::setSize", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        // setBounds(GRectangle(getX(), getY(), width, height));
        getWidget()->setGeometry((int) getX(), (int) getY(), (int) width, (int) height);
        getWidget()->setFixedSize((int) width, (int) height);
        getWidget()->setMinimumSize((int) width, (int) height);
    });
}

void GInteractor::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GInteractor::setTooltip(const std::string& tooltipText) {
    GThread::runOnQtGuiThread([this, tooltipText]() {
        getWidget()->setToolTip(QString::fromStdString(tooltipText));
    });
}

void GInteractor::setVisible(bool visible) {
    // don't allow setting visible to true unless widget is in a container
    if (!visible || _container) {
        GThread::runOnQtGuiThread([this, visible]() {
            getWidget()->setVisible(visible);
        });
    }
}

void GInteractor::setWidth(double width) {
    GThread::runOnQtGuiThread([this, width]() {
        getWidget()->setFixedWidth((int) width);
    });
}

void GInteractor::setX(double x) {
    setLocation(x, getY());
}

void GInteractor::setY(double y) {
    setLocation(getX(), y);
}


_Internal_QWidget::_Internal_QWidget()
        : _minimumSize(-1, -1),
          _preferredSize(-1, -1) {
    // empty
}

_Internal_QWidget::~_Internal_QWidget() {
    // empty
}

QSize _Internal_QWidget::getMinimumSize() const {
    return QSize((int) _minimumSize.getWidth(), (int) _minimumSize.getHeight());
}

bool _Internal_QWidget::hasMinimumSize() const {
    return _minimumSize.getWidth() >= 0 && _minimumSize.getHeight() >= 0;
}

QSize _Internal_QWidget::getPreferredSize() const {
    return QSize((int) _preferredSize.getWidth(), (int) _preferredSize.getHeight());
}

bool _Internal_QWidget::hasPreferredSize() const {
    return _preferredSize.getWidth() >= 0 && _preferredSize.getHeight() >= 0;
}

void _Internal_QWidget::setMinimumSize(double width, double height) {
    _minimumSize = GDimension(width, height);
}

void _Internal_QWidget::setMinimumSize(const QSize& size) {
    setMinimumSize(size.width(), size.height());
}

void _Internal_QWidget::setPreferredSize(double width, double height) {
    _preferredSize = GDimension(width, height);
}

void _Internal_QWidget::setPreferredSize(const QSize& size) {
    setPreferredSize(size.width(), size.height());
}

/*
 * File: gwindow.cpp
 * -----------------
 *
 * @version 2018/10/20
 * - added high-density screen features
 * @version 2018/10/11
 * - bug fix for compareToImage function
 * @version 2018/09/23
 * - bug fixes for Windows/MinGW compatibility
 * @version 2018/09/19
 * - bug fix for clear() method
 * @version 2018/09/13
 * - updated cast syntax to remove warnings in new compiler versions
 * - updated window parent semantics
 * @version 2018/09/05
 * - refactored to use a border layout GContainer "content pane" for storing all interactors
 * @version 2018/08/23
 * - renamed to gwindow.h to replace Java version
 * @version 2018/07/03
 * - thread safety code
 * @version 2018/07/01
 * - 2D graphics/painting functions
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#include <QDesktopWidget>
#include <QMenu>
#include <QMenuBar>
#include <QSizePolicy>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gdiffgui.h"
#define INTERNAL_INCLUDE 1
#include "gdiffimage.h"
#define INTERNAL_INCLUDE 1
#include "glabel.h"
#define INTERNAL_INCLUDE 1
#include "glayout.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

_Internal_QMainWindow* GWindow::_lastWindow = nullptr;
/*static*/ const int GWindow::DEFAULT_WIDTH = 500;
/*static*/ const int GWindow::DEFAULT_HEIGHT = 300;
/*static*/ const int GWindow::HIGH_DPI_SCREEN_THRESHOLD = 200;
/*static*/ const int GWindow::STANDARD_SCREEN_DPI = 96;
/*static*/ const std::string GWindow::DEFAULT_ICON_FILENAME = "splicon-large.png";

GWindow::GWindow(bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE) {
    _init(DEFAULT_WIDTH, DEFAULT_HEIGHT, visible);
}

GWindow::GWindow(double width, double height, bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE) {
    _init(width, height, visible);
}

GWindow::GWindow(double x, double y, double width, double height, bool visible)
        : _iqmainwindow(nullptr),
          _contentPane(nullptr),
          _canvas(nullptr),
          _resizable(true),
          _closeOperation(GWindow::CLOSE_DISPOSE) {
    _init(width, height, visible);
    setLocation(x, y);
}

void GWindow::_init(double width, double height, bool visible) {
    require::nonNegative2D(width, height, "GWindow::constructor", "width", "height");
    if (static_cast<int>(width) == 0) {
        width = DEFAULT_WIDTH;
    }
    if (static_cast<int>(height) == 0) {
        height = DEFAULT_HEIGHT;
    }

    GThread::runOnQtGuiThread([this]() {
        QtGui::instance()->initializeQt();
        _iqmainwindow = new _Internal_QMainWindow(this);
        _iqmainwindow->setAttribute(Qt::WA_QuitOnClose, false);
        _lastWindow = _iqmainwindow;
        _contentPane = new GContainer(GContainer::LAYOUT_BORDER);
        _iqmainwindow->setCentralWidget(_contentPane->getWidget());
    });

    ensureForwardTarget();
    setCanvasSize(width, height);
    setWindowIcon(DEFAULT_ICON_FILENAME);
    setVisible(visible);
}

GWindow::~GWindow() {
    if (_lastWindow == _iqmainwindow) {
        _lastWindow = nullptr;
    }
    // TODO: delete _iqmainwindow;
    _iqmainwindow = nullptr;
}

void GWindow::_autograder_setIsAutograderWindow(bool /*isAutograderWindow*/) {
    // TODO
}

void GWindow::_autograder_setExitGraphicsEnabled(bool /*enabled*/) {
    // TODO
}

void GWindow::_autograder_setPauseEnabled(bool /*enabled*/) {
    // TODO
}

void GWindow::add(GInteractor* interactor) {
    require::nonNull(interactor, "GWindow::add");
    addToRegion(interactor, REGION_CENTER);
}

void GWindow::add(GInteractor* interactor, double x, double y) {
    require::nonNull(interactor, "GWindow::add");
    interactor->setLocation(x, y);
    addToRegion(interactor, REGION_CENTER);
}

void GWindow::add(GInteractor& interactor) {
    addToRegion(&interactor, REGION_CENTER);
}

void GWindow::add(GInteractor& interactor, double x, double y) {
    interactor.setLocation(x, y);
    addToRegion(&interactor, REGION_CENTER);
}

void GWindow::add(GObject* obj) {
    require::nonNull(obj, "GWindow::add");
    ensureForwardTarget();
    _canvas->add(obj);
}

void GWindow::add(GObject* obj, double x, double y) {
    require::nonNull(obj, "GWindow::add");
    ensureForwardTarget();
    _canvas->add(obj, x, y);
}

void GWindow::add(GObject& obj) {
    ensureForwardTarget();
    _canvas->add(obj);
}

void GWindow::add(GObject& obj, double x, double y) {
    ensureForwardTarget();
    _canvas->add(obj, x, y);
}

QMenu* GWindow::addMenu(const std::string& menu) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (_menuMap.containsKey(menuKey)) {
        // duplicate; do not create again
        return _menuMap[menuKey];
    }

    QMenu* qmenu = nullptr;
    GThread::runOnQtGuiThread([this, menu, &qmenu]() {
        qmenu = _iqmainwindow->menuBar()->addMenu(QString::fromStdString(menu));
        std::string menuKey = toLowerCase(stringReplace(stringReplace(menu, "/", ""), "&", ""));
        _menuMap[menuKey] = qmenu;
    });
    return qmenu;
}

QAction* GWindow::addMenuItem(const std::string& menu, const std::string& item, const std::string& icon) {
    GEventListenerVoid func = [this, menu, item]() {
        this->_iqmainwindow->handleMenuAction(menu, item);
    };
    return addMenuItem(menu, item, icon, func);
}

QAction* GWindow::addMenuItem(const std::string& menu, const std::string& item, const std::string& icon, GEventListenerVoid func) {
    QAction* action = nullptr;
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = menuKey + "/" + itemKey;
    if (_menuActionMap.containsKey(menuItemKey)) {
        // duplicate; do not create again
        return _menuActionMap[menuItemKey];
    }

    GThread::runOnQtGuiThread([this, menu, item, icon, func, menuKey, menuItemKey, &action]() {
        QMenu* qmenu = _menuMap[menuKey];
        action = qmenu->addAction(QString::fromStdString(item));
        if (!icon.empty() && fileExists(icon)) {
            QIcon qicon(QString::fromStdString(icon));
            action->setIcon(qicon);
        }

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });
        _menuActionMap[menuItemKey] = action;
    });
    return action;
}

QAction* GWindow::addMenuItemCheckBox(const std::string& menu,
                                      const std::string& item,
                                      bool checked,
                                      const std::string& icon) {
    GEventListenerVoid func = [this, menu, item]() {
        this->_iqmainwindow->handleMenuAction(menu, item);
    };
    return addMenuItemCheckBox(menu, item, checked, icon, func);
}

QAction* GWindow::addMenuItemCheckBox(const std::string& menu,
                                      const std::string& item,
                                      bool checked,
                                      const std::string& icon,
                                      GEventListenerVoid func) {
    QAction* action = nullptr;
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    GThread::runOnQtGuiThread([this, menu, item, icon, checked, func, menuKey, &action]() {
        QMenu* qmenu = _menuMap[menuKey];
        action = qmenu->addAction(QString::fromStdString(item));
        action->setCheckable(true);
        action->setChecked(checked);
        if (!icon.empty() && fileExists(icon)) {
            QIcon qicon(QString::fromStdString(icon));
            action->setIcon(qicon);
        }

        // when menu item is clicked, call the function the user gave us
        _iqmainwindow->connect(action, &QAction::triggered, _iqmainwindow, [func]() {
            func();
        });

        std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
        _menuActionMap[menuKey + "/" + itemKey] = action;
    });
    return action;
}


QAction* GWindow::addMenuSeparator(const std::string& menu) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    QAction* separator = nullptr;
    GThread::runOnQtGuiThread([this, menuKey, &separator]() {
        QMenu* qmenu = _menuMap[menuKey];
        separator = qmenu->addSeparator();
    });
    return separator;
}

QMenu* GWindow::addSubMenu(const std::string& menu, const std::string& submenu) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::addMenuItem: menu \"" + menu + "\" does not exist");
        return nullptr;
    }

    QMenu* qsubmenu = nullptr;
    GThread::runOnQtGuiThread([this, menu, menuKey, submenu, &qsubmenu]() {
        QMenu* qmenu = _menuMap[menuKey];
        qsubmenu = qmenu->addMenu(QString::fromStdString(submenu));
        std::string subMenuKey = menuKey + "/"
                + toLowerCase(stringReplace(submenu, "&", ""));
        _menuMap[subMenuKey] = qsubmenu;
    });
    return qsubmenu;
}

void GWindow::addToRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GWindow::addToRegion");
    if (region == REGION_CENTER) {
        // labels in "GText mode" are added as GText objects to canvas
        if (interactor->getType() == "GLabel") {
            GLabel* label = static_cast<GLabel*>(interactor);
            if (label->hasGText()) {
                add(label->getGText());
                return;
            }
        }
    }
    _contentPane->addToRegion(interactor, static_cast<GContainer::Region>(region));
}

void GWindow::addToRegion(GInteractor* interactor, const std::string& region) {
    addToRegion(interactor, stringToRegion(region));
}

void GWindow::addToRegion(GInteractor& interactor, Region region) {
    addToRegion(&interactor, region);
}

void GWindow::addToRegion(GInteractor& interactor, const std::string& region) {
    addToRegion(&interactor, region);
}

void GWindow::clear() {
    // TODO: reimplement to clear out widgets rather than just canvas
    clearCanvas();

//    bool hasCanvas = _canvas && _contentPane->regionContains(_canvas, GContainer::REGION_CENTER);
//    _contentPane->clearRegion(GContainer::REGION_NORTH);
//    _contentPane->clearRegion(GContainer::REGION_SOUTH);
//    _contentPane->clearRegion(GContainer::REGION_WEST);
//    _contentPane->clearRegion(GContainer::REGION_EAST);
//    if (hasCanvas) {
//        clearCanvas();
//    } else {
//        // don't remove canvas, but do remove any other widgets in center
//        _contentPane->clearRegion(GContainer::REGION_CENTER);
//        ensureForwardTarget();
//    }
}

void GWindow::clearCanvas() {
    if (_canvas) {
        _canvas->clear();
    }
}

void GWindow::clearCanvasObjects() {
    if (_canvas) {
        _canvas->clearObjects();   // runs on Qt GUI thread
    }
}

void GWindow::clearCanvasPixels() {
    if (_canvas) {
        _canvas->clearPixels();   // runs on Qt GUI thread
    }
}

void GWindow::clearRegion(Region region) {
    _contentPane->clearRegion(static_cast<GContainer::Region>(region));
}

void GWindow::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

void GWindow::center() {
    GDimension screenSize = getScreenSize();
    GDimension windowSize = getSize();
    setLocation(screenSize.getWidth()  / 2 - windowSize.getWidth()  / 2,
                screenSize.getHeight() / 2 - windowSize.getHeight() / 2);
}

void GWindow::close() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->close();
    });
}

void GWindow::compareToImage(const std::string& filename, bool /*ignoreWindowSize*/) const {
    ensureForwardTargetConstHack();
    GCanvas* fileCanvas = new GCanvas(filename);
    GDiffImage::showDialog("expected output", fileCanvas,
                           "your output", _canvas);

    // TODO
    // delete fileCanvas;
}

void GWindow::ensureForwardTarget() {
    if (!_canvas) {
        // tell canvas to take any unclaimed space in the window
        GThread::runOnQtGuiThread([this]() {
            _canvas = new GCanvas(_iqmainwindow);
            _canvas->setBackground(GColor::WHITE);
            setDrawingForwardTarget(_canvas);
            addToRegion(_canvas, REGION_CENTER);
        });
    }
//    else if (!_canvas->isVisible()) {
//        // put canvas back in center region
//        GThread::runOnQtGuiThread([this]() {
//            QLayout* centerLayout = layoutForRegion(REGION_CENTER);
//            if (!GLayout::contains(centerLayout, _canvas->getWidget())) {
//                GLayout::clearLayout(centerLayout);
//                addToRegion(_canvas, "Center");
//                _canvas->setVisible(true);
//            }
//        });
//    }
}

bool GWindow::eventsEnabled() const {
    return getWidget() != nullptr && isVisible();
}

GCanvas* GWindow::getCanvas() const {
    ensureForwardTargetConstHack();
    return _canvas;
}

double GWindow::getCanvasHeight() const {
    ensureForwardTargetConstHack();
    return _canvas->getHeight();
}

GDimension GWindow::getCanvasSize() const {
    ensureForwardTargetConstHack();
    return GDimension(_canvas->getWidth(), _canvas->getHeight());
}

double GWindow::getCanvasWidth() const {
    ensureForwardTargetConstHack();
    return _canvas->getWidth();
}

GWindow::CloseOperation GWindow::getCloseOperation() const {
    return _closeOperation;
}

GObject* GWindow::getGObject(int index) const {
    if (_canvas) {
        return _canvas->getElement(index);
    } else {
        return nullptr;
    }
}

GObject* GWindow::getGObjectAt(double x, double y) const {
    if (_canvas) {
        return _canvas->getElementAt(x, y);
    } else {
        return nullptr;
    }
}

int GWindow::getGObjectCount() const {
    if (_canvas) {
        return _canvas->getElementCount();
    } else {
        return 0;
    }
}

/* static */ QMainWindow* GWindow::getLastWindow() {
    return _lastWindow;
}

GPoint GWindow::getLocation() const {
    QRect geom = _iqmainwindow->geometry();
    return GPoint(geom.x(), geom.y());
}

double GWindow::getHeight() const {
    return _iqmainwindow->geometry().height();
}

GDimension GWindow::getPreferredSize() const {
    return _contentPane->getPreferredSize();
}

double GWindow::getRegionHeight(Region region) const {
    return _contentPane->getRegionHeight(static_cast<GContainer::Region>(region));
}

double GWindow::getRegionHeight(const std::string& region) const {
    return _contentPane->getRegionHeight(region);
}

GDimension GWindow::getRegionSize(Region region) const {
    return _contentPane->getRegionSize(static_cast<GContainer::Region>(region));
}

GDimension GWindow::getRegionSize(const std::string& region) const {
    return _contentPane->getRegionSize(region);
}

double GWindow::getRegionWidth(Region region) const {
    return _contentPane->getRegionWidth(static_cast<GContainer::Region>(region));
}

double GWindow::getRegionWidth(const std::string& region) const {
    return _contentPane->getRegionWidth(region);
}

/*static*/ int GWindow::getScreenDpi() {
    return QtGui::instance()->getApplication()->desktop()->logicalDpiX();
}

/*static*/ double GWindow::getScreenDpiScaleRatio() {
    double ratio = (double) getScreenDpi() / STANDARD_SCREEN_DPI;
    return (ratio >= 1.0) ? ratio : 1.0;
}

/*static*/ double GWindow::getScreenHeight() {
    return getScreenSize().getHeight();
}

/*static*/ GDimension GWindow::getScreenSize() {
    QRect rec;
    GThread::runOnQtGuiThread([&rec]() {
        rec = QApplication::desktop()->availableGeometry();
    });
    return GDimension(rec.width(), rec.height());
}

/*static*/ double GWindow::getScreenWidth() {
    return getScreenSize().getWidth();
}

GDimension GWindow::getSize() const {
    QRect geom = _iqmainwindow->geometry();
    return GDimension(geom.width(), geom.height());
}

std::string GWindow::getTitle() const {
    return _iqmainwindow->windowTitle().toStdString();
}

std::string GWindow::getType() const {
    return "GWindow";
}

QWidget* GWindow::getWidget() const {
    return static_cast<QWidget*>(_iqmainwindow);
}

double GWindow::getWidth() const {
    return _iqmainwindow->geometry().width();
}

double GWindow::getX() const {
    return _iqmainwindow->geometry().x();
}

double GWindow::getY() const {
    return _iqmainwindow->geometry().y();
}

void GWindow::hide() {
    setVisible(false);
}

bool GWindow::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool GWindow::inCanvasBounds(double x, double y) const {
    return 0 <= x && x < getCanvasWidth() && 0 <= y && y < getCanvasHeight();
}

/*static*/ bool GWindow::isHighDensityScreen() {
    return getScreenDpi() >= HIGH_DPI_SCREEN_THRESHOLD;
}

/*static*/ bool GWindow::isHighDpiScalingEnabled() {
#ifdef SPL_SCALE_HIGH_DPI_SCREEN
    return true;
#else
    return false;
#endif // SPL_SCALE_HIGH_DPI_SCREEN
}

bool GWindow::isMaximized() const {
    return (_iqmainwindow->windowState() & Qt::WindowMaximized) != 0
            || (_iqmainwindow->windowState() & Qt::WindowFullScreen) != 0;
}

bool GWindow::isMinimized() const {
    return (_iqmainwindow->windowState() & Qt::WindowMinimized) != 0;
}

bool GWindow::isOpen() const {
    return isVisible();
}

bool GWindow::isRepaintImmediately() const {
    return _canvas && _canvas->isRepaintImmediately();
}

bool GWindow::isResizable() const {
    return _resizable;
}

bool GWindow::isVisible() const {
    return _iqmainwindow->isVisible();
}

void GWindow::loadCanvasPixels(const std::string& filename) {
    ensureForwardTarget();
    _canvas->load(filename);   // runs on Qt GUI thread
}

void GWindow::maximize() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setWindowState(Qt::WindowMaximized);
    });
}

void GWindow::minimize() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setWindowState(Qt::WindowMinimized);
    });
}

void GWindow::pack() {
    setSize(getPreferredSize());
}

void GWindow::pause(double ms) {
    require::nonNegative(ms, "GWindow::pause", "milliseconds");
    GThread::sleep(ms);
}

void GWindow::processKeyPressEventInternal(QKeyEvent* /* event */) {
    // empty; override me
}

void GWindow::rememberPosition() {
    // TODO
}

void GWindow::remove(GObject* obj) {
    require::nonNull(obj, "GWindow::remove");
    if (_canvas) {
        _canvas->remove(obj);
    }
}

void GWindow::remove(GObject& obj) {
    if (_canvas) {
        _canvas->remove(&obj);   // runs on Qt GUI thread
    }
}

void GWindow::remove(GInteractor* interactor) {
    require::nonNull(interactor, "GWindow::remove");
    _contentPane->remove(interactor);
}

void GWindow::remove(GInteractor& interactor) {
    remove(&interactor);
}

void GWindow::removeClickListener() {
    if (_canvas) {
        _canvas->removeClickListener();   // runs on Qt GUI thread
    }
}

void GWindow::removeFromRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GWindow::removeFromRegion");

    // special case: labels in "GText mode" are added to canvas
    if (region == REGION_CENTER && interactor->getType() == "GLabel") {
        GLabel* label = static_cast<GLabel*>(interactor);
        if (label->hasGText()) {
            remove(label->getGText());
            return;
        }
    }

    _contentPane->removeFromRegion(interactor, static_cast<GContainer::Region>(region));
}

void GWindow::removeFromRegion(GInteractor* interactor, const std::string& region) {
    removeFromRegion(interactor, stringToRegion(region));
}

void GWindow::removeFromRegion(GInteractor& interactor, Region region) {
    removeFromRegion(&interactor, region);
}

void GWindow::removeFromRegion(GInteractor& interactor, const std::string& region) {
    removeFromRegion(&interactor, region);
}

void GWindow::removeKeyListener() {
    if (_canvas) {
        _canvas->removeKeyListener();   // runs on Qt GUI thread
    }
}

void GWindow::removeMenuListener() {
    removeEventListener("actionMenu");
}

void GWindow::removeMouseListener() {
    if (_canvas) {
        _canvas->removeMouseListener();   // runs on Qt GUI thread
    }
}

void GWindow::removeTimerListener() {
    removeEventListener("timer");
}

void GWindow::removeWindowListener() {
    removeEventListeners({"close",
                         "closing",
                         "maximize",
                         "minimize",
                         "open",
                         "resize",
                         "restore"});
}

void GWindow::requestFocus() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setFocus();
    });
}

void GWindow::restore() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->setWindowState(Qt::WindowActive);
    });
}

void GWindow::saveCanvasPixels(const std::string& filename) {
    ensureForwardTarget();
    _canvas->save(filename);   // runs on Qt GUI thread
}

void GWindow::setBackground(int color) {
    _contentPane->setBackground(color);
    GThread::runOnQtGuiThread([this, color]() {
        GForwardDrawingSurface::setBackground(color);
    });
}

void GWindow::setBackground(const std::string& color) {
    _contentPane->setBackground(color);
    GThread::runOnQtGuiThread([this, color]() {
        GForwardDrawingSurface::setBackground(color);
        // TODO: set background of N/S/E/W regions and central region?
    });
}

void GWindow::setCanvasHeight(double height) {
    ensureForwardTarget();
    setCanvasSize(getCanvasWidth(), height);
}

void GWindow::setCanvasSize(double width, double height) {
    require::nonNegative2D(width, height, "GWindow::setCanvasSize", "width", "height");
    ensureForwardTarget();
    _canvas->setMinimumSize(width, height);    // runs on Qt GUI thread
    _canvas->setPreferredSize(width, height);
    pack();
}

void GWindow::setCanvasSize(const GDimension& size) {
    setCanvasSize(size.getWidth(), size.getHeight());
}

void GWindow::setCanvasWidth(double width) {
    ensureForwardTarget();
    setCanvasSize(width, getCanvasHeight());
}

void GWindow::setCloseOperation(CloseOperation op) {
    GThread::runOnQtGuiThread([this, op]() {
        _closeOperation = op;
        _iqmainwindow->setAttribute(Qt::WA_QuitOnClose, op == GWindow::CLOSE_EXIT);
    });
}

void GWindow::setExitOnClose(bool exitOnClose) {
    if (exitOnClose) {
        setCloseOperation(GWindow::CLOSE_EXIT);
    } else {
        setCloseOperation(_closeOperation == GWindow::CLOSE_EXIT ? GWindow::CLOSE_HIDE : _closeOperation);
    }
}

void GWindow::setHeight(double height) {
    setSize(getWidth(), height);
}

void GWindow::setLocation(double x, double y) {
    GThread::runOnQtGuiThread([this, x, y]() {
        _iqmainwindow->setGeometry(static_cast<int>(x),
                                   static_cast<int>(y),
                                   static_cast<int>(getWidth()),
                                   static_cast<int>(getHeight()));
    });
}

void GWindow::setLocation(const GPoint& p) {
    setLocation(p.getX(), p.getY());
}

void GWindow::setLocation(const Point& p) {
    setLocation(p.getX(), p.getY());
}

void GWindow::setMenuItemEnabled(const std::string& menu, const std::string& item, bool enabled) {
    std::string menuKey = toLowerCase(stringReplace(menu, "&", ""));
    std::string itemKey = toLowerCase(stringReplace(item, "&", ""));
    std::string menuItemKey = menuKey + "/" + itemKey;
    if (!_menuMap.containsKey(menuKey)) {
        error("GWindow::setMenuItemEnabled: menu \"" + menu + "\" does not exist");
    } else if (!_menuActionMap.containsKey(menuItemKey)) {
        error("GWindow::setMenuItemEnabled: menu item \"" + item + "\" does not exist");
    }

    QAction* action = _menuActionMap[menuItemKey];
    GThread::runOnQtGuiThread([action, enabled]() {
        action->setEnabled(enabled);
    });
}

void GWindow::setClickListener(GEventListener func) {
    _canvas->setClickListener(func);   // runs on Qt GUI thread
}

void GWindow::setClickListener(GEventListenerVoid func) {
    _canvas->setClickListener(func);   // runs on Qt GUI thread
}

void GWindow::setKeyListener(GEventListener func) {
    _canvas->setKeyListener(func);   // runs on Qt GUI thread
}

void GWindow::setKeyListener(GEventListenerVoid func) {
    _canvas->setKeyListener(func);   // runs on Qt GUI thread
}

void GWindow::setMenuListener(GEventListener func) {
    setEventListener("actionMenu", func);   // runs on Qt GUI thread
}

void GWindow::setMenuListener(GEventListenerVoid func) {
    setEventListener("actionMenu", func);   // runs on Qt GUI thread
}

void GWindow::setMouseListener(GEventListener func) {
    _canvas->setMouseListener(func);   // runs on Qt GUI thread
}

void GWindow::setMouseListener(GEventListenerVoid func) {
    _canvas->setMouseListener(func);   // runs on Qt GUI thread
}

void GWindow::setRegionAlignment(Region region, HorizontalAlignment halign) {
    _contentPane->setRegionAlignment(static_cast<GContainer::Region>(region), halign);
}

void GWindow::setRegionAlignment(Region region, VerticalAlignment valign) {
    _contentPane->setRegionAlignment(static_cast<GContainer::Region>(region), valign);
}

void GWindow::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    _contentPane->setRegionAlignment(static_cast<GContainer::Region>(region), halign, valign);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& align) {
    _contentPane->setRegionAlignment(region, align);
}

void GWindow::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    _contentPane->setRegionAlignment(region, halign, valign);
}

void GWindow::setRegionHorizontalAlignment(Region region, HorizontalAlignment halign) {
    _contentPane->setRegionHorizontalAlignment(static_cast<GContainer::Region>(region), halign);
}

void GWindow::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    _contentPane->setRegionHorizontalAlignment(region, halign);
}

void GWindow::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    _contentPane->setRegionVerticalAlignment(static_cast<GContainer::Region>(region), valign);
}

void GWindow::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    _contentPane->setRegionVerticalAlignment(region, valign);
}

void GWindow::setResizable(bool resizable) {
    GThread::runOnQtGuiThread([this, resizable]() {
        if (resizable) {
            if (!_resizable) {
                _iqmainwindow->resize(static_cast<int>(getWidth()),
                                      static_cast<int>(getHeight()));
                _iqmainwindow->setMinimumSize(_iqmainwindow->minimumSizeHint());
                GDimension screenSize = getScreenSize();
                _iqmainwindow->setMaximumSize(static_cast<int>(screenSize.getWidth()),
                                              static_cast<int>(screenSize.getHeight()));
                _iqmainwindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            }
        } else {
            if (_resizable) {
                _iqmainwindow->setFixedSize(_iqmainwindow->size());
                _iqmainwindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            }
        }
        _resizable = resizable;
    });
}

void GWindow::setSize(double width, double height) {
    require::nonNegative2D(width, height, "GWindow::setSize", "width", "height");
    GThread::runOnQtGuiThread([this, width, height]() {
        if (isResizable()) {
            _iqmainwindow->resize(static_cast<int>(width),
                                  static_cast<int>(height));
        } else {
            _iqmainwindow->setFixedSize(static_cast<int>(width),
                                        static_cast<int>(height));
        }
    });
}

void GWindow::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void GWindow::setTimerListener(double ms, GEventListener func) {
    require::nonNegative(ms, "GWindow::setTimerListener", "delay (ms)");
    setEventListener("timer", func);
    GThread::runOnQtGuiThread([this, ms]() {
        _iqmainwindow->timerStart(ms);
    });
}

void GWindow::setTimerListener(double ms, GEventListenerVoid func) {
    require::nonNegative(ms, "GWindow::setTimerListener", "delay (ms)");
    setEventListener("timer", func);
    GThread::runOnQtGuiThread([this, ms]() {
        _iqmainwindow->timerStart(ms);
    });
}

void GWindow::setTitle(const std::string& title) {
    GThread::runOnQtGuiThread([this, title]() {
        _iqmainwindow->setWindowTitle(QString::fromStdString(title));
    });
}

void GWindow::setVisible(bool visible) {
    GThread::runOnQtGuiThread([this, visible]() {
        _iqmainwindow->setVisible(visible);
    });
}

void GWindow::setWidth(double width) {
    setSize(width, getHeight());
}

void GWindow::setWindowIcon(const std::string& iconFile) {
    if (fileExists(iconFile)) {
        GThread::runOnQtGuiThread([this, iconFile]() {
            QIcon qicon(QString::fromStdString(iconFile));
            _iqmainwindow->setWindowIcon(qicon);
        });
    }
}

void GWindow::setWindowListener(GEventListener func) {
    setEventListeners({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
}

void GWindow::setWindowListener(GEventListenerVoid func) {
    setEventListeners({"close",
                      "closing",
                      "maximize",
                      "minimize",
                      "open",
                      "resize",
                      "restore"}, func);
}

void GWindow::setWindowTitle(const std::string& title) {
    setTitle(title);
}

void GWindow::setX(double x) {
    setLocation(x, getY());
}

void GWindow::setY(double y) {
    setLocation(getX(), y);
}

void GWindow::show() {
    setVisible(true);
}

void GWindow::sleep(double ms) {
    require::nonNegative(ms, "GWindow::sleep", "delay (ms)");
    GThread::sleep(ms);
}

GWindow::Region GWindow::stringToRegion(const std::string& regionStr) {
    std::string regionLC = toLowerCase(trim(regionStr));
    if (stringContains(regionLC, "north") || stringContains(regionLC, "top")) {
        return GWindow::REGION_NORTH;
    } else if (stringContains(regionLC, "south") || stringContains(regionLC, "bottom")) {
        return GWindow::REGION_SOUTH;
    } else if (stringContains(regionLC, "west") || stringContains(regionLC, "left")) {
        return GWindow::REGION_WEST;
    } else if (stringContains(regionLC, "east") || stringContains(regionLC, "right")) {
        return GWindow::REGION_EAST;
    } else {
        return GWindow::REGION_CENTER;
    }
}

void GWindow::toBack() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->lower();
    });
}

void GWindow::toFront() {
    GThread::runOnQtGuiThread([this]() {
        _iqmainwindow->raise();
        _iqmainwindow->setFocus();
    });
}


// global functions for compatibility

int convertColorToRGB(const std::string& colorName) {
    return GColor::convertColorToRGB(colorName);
}

std::string convertRGBToColor(int rgb) {
    return GColor::convertRGBToColor(rgb);
}

void exitGraphics() {
    QtGui::instance()->exitGraphics();
}

double getScreenHeight() {
    return GWindow::getScreenHeight();
}

GDimension getScreenSize() {
    return GWindow::getScreenSize();
}

double getScreenWidth() {
    return GWindow::getScreenWidth();
}

void pause(double milliseconds) {
    GThread::sleep(milliseconds);
}

void repaint() {
    QMainWindow* lastWindow = GWindow::getLastWindow();
    if (lastWindow) {
        lastWindow->repaint();
    }
    // TODO: other windows?
}


_Internal_QMainWindow::_Internal_QMainWindow(GWindow* gwindow, QWidget* parent)
        : QMainWindow(parent),
          _gwindow(gwindow) {
    require::nonNull(gwindow, "_Internal_QMainWindow::constructor");
    GThread::ensureThatThisIsTheQtGuiThread("GWindow internal initialization");
    setObjectName(QString::fromStdString("_Internal_QMainWindow"));
}

void _Internal_QMainWindow::changeEvent(QEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::changeEvent", "event");
    QMainWindow::changeEvent(event);   // call super
    if (event->type() != QEvent::WindowStateChange) {
        return;
    }

    // https://doc.qt.io/Qt-5/qt.html#WindowState-enum
    QWindowStateChangeEvent* stateChangeEvent = static_cast<QWindowStateChangeEvent*>(event);
    Qt::WindowStates state = windowState();
    bool wasMaximized = (stateChangeEvent->oldState() & Qt::WindowMaximized) != 0;
    bool wasMinimized = (stateChangeEvent->oldState() & Qt::WindowMinimized) != 0;
    bool isMaximized = (state & Qt::WindowMaximized) != 0;
    bool isMinimized = (state & Qt::WindowMinimized) != 0;
    if (!wasMaximized && isMaximized) {
        _gwindow->fireGEvent(stateChangeEvent, WINDOW_MAXIMIZED, "maximize");
    } else if (!wasMinimized && isMinimized) {
        _gwindow->fireGEvent(stateChangeEvent, WINDOW_MINIMIZED, "minimize");
    } else if ((wasMinimized || wasMaximized) && !isMinimized && !isMaximized) {
        _gwindow->fireGEvent(stateChangeEvent, WINDOW_RESTORED, "restore");
    }
}

void _Internal_QMainWindow::closeEvent(QCloseEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::closeEvent", "event");
    // send "closing" event before window closes
    _gwindow->fireGEvent(event, WINDOW_CLOSING, "closing");

    GWindow::CloseOperation closeOp = _gwindow->getCloseOperation();
    if (closeOp == GWindow::CLOSE_DO_NOTHING) {
        event->ignore();
        return;
    }

    // send "close" event after window closes
    event->accept();
    QMainWindow::closeEvent(event);   // call super
    _gwindow->fireGEvent(event, WINDOW_CLOSED, "close");

    if (closeOp == GWindow::CLOSE_EXIT) {
        // exit app
        QtGui::instance()->exitGraphics();
    }
}

void _Internal_QMainWindow::handleMenuAction(const std::string& menu, const std::string& item) {
    GEvent actionEvent(
                /* class  */ ACTION_EVENT,
                /* type   */ ACTION_MENU,
                /* name   */ "actionMenu",
                /* source */ _gwindow);
    actionEvent.setActionCommand(menu + "/" + item);
    _gwindow->fireEvent(actionEvent);
}

void _Internal_QMainWindow::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::keyPressEvent", "event");
    QMainWindow::keyPressEvent(event);   // call super
    _gwindow->processKeyPressEventInternal(event);
}

void _Internal_QMainWindow::resizeEvent(QResizeEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::resizeEvent", "event");
    QMainWindow::resizeEvent(event);   // call super
    _gwindow->fireGEvent(event, WINDOW_RESIZED, "resize");
}

void _Internal_QMainWindow::timerEvent(QTimerEvent* event) {
    require::nonNull(event, "_Internal_QMainWindow::timerEvent", "event");
    QMainWindow::timerEvent(event);   // call super
    _gwindow->fireGEvent(event, TIMER_TICKED, "timer");
}

bool _Internal_QMainWindow::timerExists(int id) {
    if (id >= 0) {
        return _timerIDs.contains(id);
    } else {
        return !_timerIDs.isEmpty();
    }
}

int _Internal_QMainWindow::timerStart(double ms) {
    require::nonNegative(ms, "_Internal_QMainWindow::timerStart", "delay (ms)");
    int timerID = startTimer(static_cast<int>(ms));
    _timerIDs.add(timerID);
    return timerID;
}

void _Internal_QMainWindow::timerStop(int id) {
    if (id < 0 && timerExists()) {
        id = _timerIDs.first();
    }

    if (timerExists(id)) {
        killTimer(id);
        _timerIDs.remove(id);
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gwindow.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gobservable.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gobservable.cpp to replace Java version
 * @version 2018/07/11
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gobservable.h"
#include <iostream>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "geventqueue.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

GObservable::GObservable()
        : _eventsEnabled(true) {
    // empty
}

GObservable::~GObservable() {
    // empty
}

void GObservable::clearEventListeners() {
    _eventMap.clear();
}

void GObservable::ensureThreadSafety(const std::string& memberName) {
    GThread::ensureThatThisIsTheQtGuiThread(memberName);
}

bool GObservable::eventsEnabled() const {
    return _eventsEnabled;
}

void GObservable::fireEvent(GEvent& event) {
    if (eventsEnabled()) {
        event.setSource(this);
        if (hasEventListener(event.getName())) {
            _eventMap[event.getName()].fireEvent(event);
        } else {
            // put into global queue for waitForEvent calls
            GEventQueue::instance()->enqueueEvent(event);
        }
    }
}

void GObservable::fireGEvent(QEvent* event,
                             EventType eventType,
                             const std::string& eventName) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent generalEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    generalEvent.setInternalEvent(event);
    fireEvent(generalEvent);
}

void GObservable::fireGEvent(QCloseEvent* event,
                             EventType eventType,
                             const std::string& eventName) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent windowEvent(
                /* class  */ WINDOW_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    windowEvent.setInternalEvent(event);
    fireEvent(windowEvent);
}

void GObservable::fireGEvent(QKeyEvent* event,
                             EventType eventType,
                             const std::string& eventName) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent keyEvent(
                /* class  */ KEY_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    keyEvent.setKeyCode(event->key());
    std::string text = event->text().toStdString();
    if (!text.empty()) {
        keyEvent.setKeyChar(text);
    } else {
        // int keyChar = event->key() & 0x0000ffff & ~Qt::KeyboardModifierMask;
        // keyEvent.setKeyChar((char) keyChar);
    }
    keyEvent.setModifiers(event->modifiers());
    keyEvent.setInternalEvent(event);
    fireEvent(keyEvent);
}

void GObservable::fireGEvent(QMouseEvent* event,
                             EventType eventType,
                             const std::string& eventName,
                             const std::string& actionCommand) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    mouseEvent.setModifiers(event->modifiers());
    mouseEvent.setActionCommand(actionCommand);
    mouseEvent.setInternalEvent(event);
    fireEvent(mouseEvent);
}

void GObservable::fireGEvent(QResizeEvent* event,
                             EventType /* eventType */,
                             const std::string& eventName) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent windowEvent(
                /* class  */ WINDOW_EVENT,
                /* type   */ WINDOW_RESIZED,
                /* name   */ eventName,
                /* source */ this);
    windowEvent.setInternalEvent(event);
    fireEvent(windowEvent);
}

void GObservable::fireGEvent(QTimerEvent* event,
                             EventType /* eventType */,
                             const std::string& /* eventName */) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent timerEvent(
                /* class  */ TIMER_EVENT,
                /* type   */ TIMER_TICKED,
                /* name   */ "timer",
                /* source */ this);
    timerEvent.setInternalEvent(event);
    fireEvent(timerEvent);
}

void GObservable::fireGEvent(QWheelEvent* event,
                             EventType eventType,
                             const std::string& eventName) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent wheelEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    wheelEvent.setButton((int) event->buttons());
    wheelEvent.setX(event->x());
    wheelEvent.setY(event->y());
    wheelEvent.setModifiers(event->modifiers());
    wheelEvent.setInternalEvent(event);
    fireEvent(wheelEvent);
}

void GObservable::fireGEvent(QWindowStateChangeEvent* event,
                             EventType eventType,
                             const std::string& eventName) {
    require::nonNull(event, "GObservable::fireGEvent", "event");
    GEvent windowEvent(
                /* class  */ WINDOW_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    windowEvent.setInternalEvent(event);
    fireEvent(windowEvent);
}

bool GObservable::isAcceptingEvent(const std::string& eventType) const {
    if (hasEventListener(eventType)) {
        return true;
    }
    int eventMask = ANY_EVENT;
    if (startsWith(eventType, "action")) {
        eventMask = ACTION_EVENT;
    } else if (startsWith(eventType, "change")) {
        eventMask = CHANGE_EVENT;
    } else if (startsWith(eventType, "key")) {
        eventMask = KEY_EVENT;
    } else if (startsWith(eventType, "mouse")) {
        eventMask = MOUSE_EVENT;
    } else if (startsWith(eventType, "server")) {
        eventMask = SERVER_EVENT;
    } else if (startsWith(eventType, "table")) {
        eventMask = TABLE_EVENT;
    } else if (startsWith(eventType, "timer")) {
        eventMask = TIMER_EVENT;
    } else if (startsWith(eventType, "window")) {
        eventMask = WINDOW_EVENT;
    }
    return isAcceptingEvent(eventMask);
}

bool GObservable::isAcceptingEvent(const GEvent& event) const {
    return isAcceptingEvent(event.getName());
}

bool GObservable::hasEventListener(const std::string& eventName) const {
    return _eventMap.containsKey(eventName);
}

bool GObservable::isAcceptingEvent(int eventMask) const {
    return GEventQueue::instance()->isAcceptingEvent(eventMask);
}

void GObservable::removeEventListener(const std::string& eventName) {
    _eventMap.remove(eventName);
}

void GObservable::removeEventListeners(std::initializer_list<std::string> eventNames) {
    for (std::string eventName : eventNames) {
        removeEventListener(eventName);
    }
}

void GObservable::setEventListener(const std::string& eventName, GEventListener func) {
    GEvent::EventListenerWrapper wrapper;
    wrapper.type = GEvent::HANDLER_EVENT;
    wrapper.handler = func;
    _eventMap[eventName] = wrapper;
}

void GObservable::setEventListener(const std::string& eventName, GEventListenerVoid func) {
    GEvent::EventListenerWrapper wrapper;
    wrapper.type = GEvent::HANDLER_VOID;
    wrapper.handlerVoid = func;
    _eventMap[eventName] = wrapper;
}

void GObservable::setEventListeners(std::initializer_list<std::string> eventNames, GEventListener func) {
    for (std::string eventName : eventNames) {
        setEventListener(eventName, func);
    }
}

void GObservable::setEventListeners(std::initializer_list<std::string> eventNames, GEventListenerVoid func) {
    for (std::string eventName : eventNames) {
        setEventListener(eventName, func);
    }
}

void GObservable::setEventsEnabled(bool eventsEnabled) {
    _eventsEnabled = eventsEnabled;
}

std::string GObservable::toString() const {
    std::ostringstream out;
    out << getType() << "@" << this;
    return out.str();
}

/*
 * File: gclipboard.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gclipboard.cpp to replace Java version
 * @version 2018/07/19
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gclipboard.h"
#include <QApplication>
#include <QClipboard>
#include <QString>
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GClipboard::GClipboard() {
    // empty
}

void GClipboard::set(const std::string& text) {
    QClipboard* clipboard = QApplication::clipboard();
    require::nonNull(clipboard, "GClipboard::set", "clipboard");
    clipboard->setText(QString::fromStdString(text));
}

std::string GClipboard::get() {
    QClipboard* clipboard = QApplication::clipboard();
    require::nonNull(clipboard, "GClipboard::get", "clipboard");
    return clipboard->text().toStdString();
}

bool GClipboard::isCopy(QKeyEvent* event) {
    require::nonNull(event, "GClipboard::isCopy", "event");
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return ctrlOrMeta && (event->key() == Qt::Key_C || event->key() == Qt::Key_Insert);
}

bool GClipboard::isCut(QKeyEvent* event) {
    require::nonNull(event, "GClipboard::isCopy", "event");
    bool shift = (event->modifiers() & Qt::ShiftModifier) != 0;
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return (ctrlOrMeta && event->key() == Qt::Key_X)
            || (shift && event->key() == Qt::Key_Delete);
}

bool GClipboard::isPaste(QKeyEvent* event) {
    require::nonNull(event, "GClipboard::isCopy", "event");
    bool shift = (event->modifiers() & Qt::ShiftModifier) != 0;
    bool ctrlOrMeta = (event->modifiers() & Qt::ControlModifier) != 0
            || (event->modifiers() & Qt::MetaModifier) != 0;
    return (ctrlOrMeta && event->key() == Qt::Key_V)
            || (shift && event->key() == Qt::Key_Insert);
}

/*
 * File: gtimer.cpp
 * ----------------
 * This file implements the gtimer.h interface.
 * 
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * - removed unneeded include statements
 */

#define INTERNAL_INCLUDE 1
#include "gtimer.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

/* Implementation of the GTimer class */

GTimer::GTimer(double milliseconds)
        : _ms(0),
          _id(-1) {
    setDelay(milliseconds);
}

double GTimer::getDelay() const {
    return _ms;
}

bool GTimer::isStarted() const {
    return _id >= 0;
}

void GTimer::restart() {
    stop();
    start();
}

void GTimer::setDelay(double ms) {
    require::positive(ms, "GTimer::setDelay", "delay (ms)");
    _ms = ms;
    if (isStarted()) {
        restart();
    }
}

void GTimer::start() {
    _Internal_QMainWindow* lastWindow = static_cast<_Internal_QMainWindow*>(GWindow::getLastWindow());
    if (!lastWindow) {
        error("GTimer::start: You must create at least one GWindow before starting a GTimer.");
        return;
    }
    GThread::runOnQtGuiThreadAsync([this, lastWindow]() {
        _id = lastWindow->timerStart(_ms);
    });
}

void GTimer::stop() {
    if (isStarted()) {
        _Internal_QMainWindow* lastWindow = static_cast<_Internal_QMainWindow*>(GWindow::getLastWindow());
        if (!lastWindow) {
            error("GTimer::constructor: You must create at least one GWindow before stopping a GTimer.");
            return;
        }
        GThread::runOnQtGuiThreadAsync([this, lastWindow]() {
            lastWindow->timerStop(_id);
            _id = -1;
        });
    }
}

/*
 * File: gcontainer.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/11/27
 * - added code to set a widget visible after adding/inserting it to flow container
 *   (needed to see widgets added to container after window is showing on screen)
 * @version 2018/09/19
 * - added contains, regionContains methods
 * - added argument checking with require.h
 * @version 2018/09/05
 * - thread safety improvements
 * - added setContainer logic to child interactors
 * @version 2018/08/29
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gcontainer.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "glabel.h"
#define INTERNAL_INCLUDE 1
#include "glayout.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

// margin  - around container, but outside of its background color area (like CSS)
// padding - around container, but within its background color area (like CSS)
// spacing - between neighboring widgets in container

const int GContainer::MARGIN_DEFAULT = 5;
const int GContainer::SPACING_DEFAULT = 8;


GContainer::GContainer(Layout layout, QWidget* parent)
        : _iqcontainer(nullptr),
          _layout(layout) {
    GThread::runOnQtGuiThread([this, layout, parent]() {
        _iqcontainer = new _Internal_QContainer(this, layout, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GContainer::GContainer(Layout /*layout*/, int rows, int cols, QWidget* parent)
        : _iqcontainer(nullptr),
          _layout(LAYOUT_GRID) {
    GThread::runOnQtGuiThread([this, rows, cols, parent]() {
        _iqcontainer = new _Internal_QContainer(this, rows, cols, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GContainer::~GContainer() {
    // TODO: delete _iqcontainer;
    _iqcontainer = nullptr;
}

void GContainer::add(GInteractor* interactor) {
    require::nonNull(interactor, "GContainer::add");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(this);
    _interactors.add(interactor);

    GThread::runOnQtGuiThread([this, widget]() {
        widget->setParent(_iqcontainer);
        _iqcontainer->add(widget);
    });
}

void GContainer::add(GInteractor& interactor) {
    add(&interactor);
}

void GContainer::addToGrid(GInteractor* interactor, int row, int col, int rowspan, int colspan) {
    require::nonNull(interactor, "GContainer::addToGrid");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    GThread::runOnQtGuiThread([this, widget, row, col, rowspan, colspan]() {
        _iqcontainer->addToGrid(widget, row, col, rowspan, colspan);
    });
}

void GContainer::addToGrid(GInteractor& interactor, int row, int col, int rowspan, int colspan) {
    addToGrid(&interactor, row, col, rowspan, colspan);
}

void GContainer::addToRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GContainer::addToRegion");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    // special case: if center, remove all other widgets in that region first
    if (region == REGION_CENTER) {
        clearRegion(region);
    }

    interactor->setContainer(this);
    _interactors.add(interactor);
    _interactorsByRegion[region].add(interactor);

    GThread::runOnQtGuiThread([this, widget, region]() {
        _iqcontainer->addToRegion(widget, region);
    });
}

void GContainer::addToRegion(GInteractor* interactor, const std::string& region) {
    addToRegion(interactor, stringToRegion(region));
}

void GContainer::addToRegion(GInteractor& interactor, Region region) {
    addToRegion(&interactor, region);
}

void GContainer::addToRegion(GInteractor& interactor, const std::string& region) {
    addToRegion(&interactor, region);
}

void GContainer::clear() {
    for (GInteractor* interactor : _interactors) {
        interactor->setContainer(nullptr);
    }
    _interactors.clear();
    _interactorsByRegion.clear();

    GThread::runOnQtGuiThread([this]() {
        _iqcontainer->clear();
    });
}

void GContainer::clearRegion(Region region) {
    for (GInteractor* interactor : _interactorsByRegion[region]) {
        interactor->setContainer(nullptr);
        interactor->setVisible(false);
        _interactors.removeValue(interactor);
    }
    _interactorsByRegion.remove(region);

    GThread::runOnQtGuiThread([this, region]() {
        _iqcontainer->clearRegion(region);
    });
}

void GContainer::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

bool GContainer::contains(GInteractor* interactor) const {
    if (!interactor) {
        return false;
    } else {
        return _iqcontainer->contains(interactor->getWidget());
    }
}

bool GContainer::contains(GInteractor& interactor) const {
    return contains(&interactor);
}

Vector<GInteractor*> GContainer::getDescendents(const std::string& type) const {
    Vector<GInteractor*> result;
    for (GInteractor* interactor : _interactors) {
        // pre-order traversal; add parent and then visit children
        if (type.empty() || type == "*" || equalsIgnoreCase(type, interactor->getType())) {
            result.add(interactor);
        }

        if (equalsIgnoreCase(interactor->getType(), "GContainer")) {
            // recursively get all descendents of this child
            GContainer* subcontainer = static_cast<GContainer*>(interactor);
            Vector<GInteractor*> descendents = subcontainer->getDescendents(type);
            result.addAll(descendents);
        }
    }
    return result;
}

GInteractor* GContainer::getInteractor(int i) const {
    return _interactors[i];
}

const Vector<GInteractor*>& GContainer::getInteractors() const {
    return _interactors;
}

int GContainer::getInteractorCount() const {
    return _interactors.size();
}

GInteractor* GContainer::getInteractorByRegion(int i, Region region) const {
    return _interactorsByRegion[region][i];
}

GInteractor* GContainer::getInteractorByRegion(int i, const std::string& region) const {
    return getInteractorByRegion(i, stringToRegion(region));
}

int GContainer::getInteractorCountByRegion(Region region) const {
    return _interactorsByRegion[region].size();
}

int GContainer::getInteractorCountByRegion(const std::string& region) const {
    return getInteractorCountByRegion(stringToRegion(region));
}


_Internal_QWidget* GContainer::getInternalWidget() const {
    return _iqcontainer;
}

GContainer::Layout GContainer::getLayout() const {
    return _layout;
}

double GContainer::getMargin() const {
    return _iqcontainer->getMargin();
}

double GContainer::getPadding() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return (left + top + right + bottom) / 4.0;
}

double GContainer::getPaddingBottom() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return bottom;
}

double GContainer::getPaddingLeft() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return left;
}

double GContainer::getPaddingRight() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return right;
}

double GContainer::getPaddingTop() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return top;
}

GDimension GContainer::getPreferredSize() const {
    QSize qsize = _iqcontainer->getPreferredSize();
    return GDimension(qsize.width(), qsize.height());
}

double GContainer::getRegionHeight(Region region) const {
    return getRegionSize(region).getHeight();
}

double GContainer::getRegionHeight(const std::string& region) const {
    return getRegionHeight(stringToRegion(region));
}

GDimension GContainer::getRegionSize(Region region) const {
    QLayout* layout = _iqcontainer->layoutForRegion(region);
    if (!layout) {
        return GDimension();
    } else {
        QRect geom = layout->geometry();
        return GDimension(geom.width(), geom.height());
    }
}

GDimension GContainer::getRegionSize(const std::string& region) const {
    return getRegionSize(stringToRegion(region));
}

double GContainer::getRegionWidth(Region region) const {
    return getRegionSize(region).getWidth();
}

double GContainer::getRegionWidth(const std::string& region) const {
    return getRegionWidth(stringToRegion(region));
}

double GContainer::getSpacing() const {
    return _iqcontainer->getSpacing();
}

std::string GContainer::getType() const {
    return "GContainer";
}

QWidget* GContainer::getWidget() const {
    return static_cast<QWidget*>(_iqcontainer);
}

void GContainer::insert(int index, GInteractor* interactor) {
    require::nonNull(interactor, "GContainer::insert");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(this);
    _interactors.insert(index, interactor);

    GThread::runOnQtGuiThread([this, index, widget]() {
        _iqcontainer->insert(index, widget);
    });
}

void GContainer::insert(int index, GInteractor& interactor) {
    insert(index, &interactor);
}

void GContainer::insertToRegion(int index, GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GContainer::insertToRegion");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(this);
    _interactors.add(interactor);
    _interactorsByRegion[region].insert(index, interactor);

    GThread::runOnQtGuiThread([this, index, widget, region]() {
        _iqcontainer->insertToRegion(index, widget, region);
    });
}

void GContainer::insertToRegion(int index, GInteractor* interactor, const std::string& region) {
    insertToRegion(index, interactor, stringToRegion(region));
}

void GContainer::insertToRegion(int index, GInteractor& interactor, Region region) {
    insertToRegion(index, &interactor, region);
}

void GContainer::insertToRegion(int index, GInteractor& interactor, const std::string& region) {
    insertToRegion(index, &interactor, stringToRegion(region));
}

bool GContainer::isEmpty() const {
    return getInteractorCount() == 0;
}

bool GContainer::regionContains(GInteractor* interactor, GContainer::Region region) const {
    if (!interactor) {
        return false;
    } else {
        return _iqcontainer->regionContains(interactor->getWidget(), region);
    }
}

bool GContainer::regionContains(GInteractor* interactor, const std::string& region) const {
    return regionContains(interactor, stringToRegion(region));
}

bool GContainer::regionContains(GInteractor& interactor, GContainer::Region region) const {
    return regionContains(&interactor, region);
}

bool GContainer::regionContains(GInteractor& interactor, const std::string& region) const {
    return regionContains(interactor, stringToRegion(region));
}


void GContainer::remove(GInteractor* interactor) {
    require::nonNull(interactor, "GContainer::remove");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(nullptr);
    _interactors.removeValue(interactor);

    GThread::runOnQtGuiThread([this, widget]() {
        _iqcontainer->remove(widget);
    });
}

void GContainer::remove(GInteractor& interactor) {
    remove(&interactor);
}

void GContainer::remove(int index) {
    GInteractor* interactor = _interactors[index];
    interactor->setContainer(nullptr);
    _interactors.remove(index);

    GThread::runOnQtGuiThread([this, index]() {
        _iqcontainer->remove(index);
    });
}

void GContainer::removeFromRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GContainer::removeFromRegion");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(nullptr);
    _interactors.removeValue(interactor);
    _interactorsByRegion[region].removeValue(interactor);

    GThread::runOnQtGuiThread([this, widget, region]() {
        _iqcontainer->removeFromRegion(widget, region);
    });
}

void GContainer::removeFromRegion(GInteractor* interactor, const std::string& region) {
    removeFromRegion(interactor, stringToRegion(region));
}

void GContainer::removeFromRegion(GInteractor& interactor, Region region) {
    removeFromRegion(&interactor, region);
}

void GContainer::removeFromRegion(GInteractor& interactor, const std::string& region) {
    removeFromRegion(&interactor, region);
}

void GContainer::removeFromRegion(int index, Region region) {
    GInteractor* interactor = _interactorsByRegion[region][index];
    interactor->setContainer(nullptr);
    _interactors.removeValue(interactor);
    _interactorsByRegion[region].remove(index);

    GThread::runOnQtGuiThread([this, index, region]() {
        _iqcontainer->removeFromRegion(index, region);
    });
}

void GContainer::removeFromRegion(int index, const std::string& region) {
    removeFromRegion(index, stringToRegion(region));
}

void GContainer::setAlignment(HorizontalAlignment halign, VerticalAlignment valign) {
    setHorizontalAlignment(halign);
    setVerticalAlignment(valign);
}

void GContainer::setHorizontalAlignment(HorizontalAlignment halign) {
    GThread::runOnQtGuiThread([this, halign]() {
        _iqcontainer->setHorizontalAlignment(halign);
    });
}

void GContainer::setMargin(double px) {
    GThread::runOnQtGuiThread([this, px]() {
        _iqcontainer->setMargin((int) px);
    });
}

void GContainer::setPadding(double px) {
    setPadding(px, px, px, px);
}

void GContainer::setPadding(double topBottom, double leftRight) {
    setPadding(topBottom, leftRight, topBottom, leftRight);
}

void GContainer::setPadding(double top, double right, double bottom, double left) {
    GThread::runOnQtGuiThread([this, top, right, bottom, left]() {
        _iqcontainer->setPadding((int) top, (int) right, (int) bottom, (int) left);
    });
}

void GContainer::setRegionAlignment(Region region, HorizontalAlignment halign) {
    setRegionHorizontalAlignment(region, halign);
}

void GContainer::setRegionAlignment(Region region, VerticalAlignment valign) {
    setRegionVerticalAlignment(region, valign);
}

void GContainer::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, region, halign, valign]() {
        _iqcontainer->setRegionAlignment(region, halign, valign);
    });
}

void GContainer::setRegionAlignment(const std::string& region, const std::string& align) {
    HorizontalAlignment halignment = toHorizontalAlignment(align);
    VerticalAlignment valignment = toVerticalAlignment(align);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void GContainer::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    HorizontalAlignment halignment = toHorizontalAlignment(halign);
    VerticalAlignment valignment = toVerticalAlignment(valign);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void GContainer::setRegionHorizontalAlignment(Region region, HorizontalAlignment halign) {
    GThread::runOnQtGuiThread([this, region, halign]() {
        _iqcontainer->setRegionHorizontalAlignment(region, halign);
    });
}

void GContainer::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    setRegionHorizontalAlignment(stringToRegion(region), toHorizontalAlignment(halign));
}

void GContainer::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, region, valign]() {
        _iqcontainer->setRegionVerticalAlignment(region, valign);
    });
}

void GContainer::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    setRegionVerticalAlignment(stringToRegion(region), toVerticalAlignment(valign));
}

void GContainer::setSpacing(double px) {
    GThread::runOnQtGuiThread([this, px]() {
        _iqcontainer->setSpacing((int) px);
    });
}

void GContainer::setVerticalAlignment(VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, valign]() {
        _iqcontainer->setVerticalAlignment(valign);
    });
}

GContainer::Region GContainer::stringToRegion(const std::string& regionStr) {
    std::string regionLC = toLowerCase(trim(regionStr));
    if (stringContains(regionLC, "north") || stringContains(regionLC, "top")) {
        return REGION_NORTH;
    } else if (stringContains(regionLC, "south") || stringContains(regionLC, "bottom")) {
        return REGION_SOUTH;
    } else if (stringContains(regionLC, "west") || stringContains(regionLC, "left")) {
        return REGION_WEST;
    } else if (stringContains(regionLC, "east") || stringContains(regionLC, "right")) {
        return REGION_EAST;
    } else {
        return REGION_CENTER;
    }
}


_Internal_QContainer::_Internal_QContainer(GContainer* gcontainer, GContainer::Layout layoutType, QWidget* parent)
        : QWidget(parent),
          _gcontainer(gcontainer),
          _layoutType(GContainer::LAYOUT_NONE),
          _halign(ALIGN_CENTER),
          _valign(ALIGN_MIDDLE),
          _margin(GContainer::MARGIN_DEFAULT),
          _spacing(GContainer::SPACING_DEFAULT),
          // _rows(0),
          _cols(0),
          _currentIndex(-1),
          _overallLayout(nullptr),
          _northLayout(nullptr),
          _southLayout(nullptr),
          _westLayout(nullptr),
          _eastLayout(nullptr),
          _centerLayout(nullptr),
          _middleLayout(nullptr) {
    require::nonNull(gcontainer, "_Internal_QContainer::constructor");
    setLayoutType(layoutType);
    if (layout()) {
        setSpacing(GContainer::SPACING_DEFAULT);
    }
}

_Internal_QContainer::_Internal_QContainer(GContainer* gcontainer, int /*rows*/, int cols, QWidget* parent)
        : QWidget(parent),
          _gcontainer(gcontainer),
          _layoutType(GContainer::LAYOUT_NONE),
          _halign(ALIGN_CENTER),
          _valign(ALIGN_MIDDLE),
          _margin(GContainer::MARGIN_DEFAULT),
          _spacing(GContainer::SPACING_DEFAULT),
          // _rows(rows),
          _cols(cols),
          _currentIndex(-1),
          _overallLayout(nullptr),
          _northLayout(nullptr),
          _southLayout(nullptr),
          _westLayout(nullptr),
          _eastLayout(nullptr),
          _centerLayout(nullptr),
          _middleLayout(nullptr) {
    setObjectName(QString::fromStdString("_Internal_QContainer_" + std::to_string(gcontainer->getID())));
    setLayoutType(GContainer::LAYOUT_GRID);
    if (layout()) {
        setMargin(GContainer::MARGIN_DEFAULT);
        setSpacing(GContainer::SPACING_DEFAULT);
    }
}

void _Internal_QContainer::add(QWidget* widget) {
    switch (_layoutType) {
        case GContainer::LAYOUT_BORDER: {
            addToRegion(widget, GContainer::REGION_CENTER);
            break;
        }
        case GContainer::LAYOUT_GRID: {
            _currentIndex++;
            int row = _cols <= 0 ? 0 : _currentIndex / _cols;
            int col = _cols <= 0 ? 0 : _currentIndex % _cols;
            QGridLayout* gridLayout = (QGridLayout*) getQLayout();
            gridLayout->addWidget(widget, row, col);
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // add to end of the list of widgets in this region
            QBoxLayout* boxLayout = (QBoxLayout*) getQLayout();
            boxLayout->insertWidget(/* index */ boxLayout->count() - 1, widget);
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
        default: {
            error("GContainer::add: no layout has been set");
            break;
        }
    }
}

void _Internal_QContainer::addToGrid(QWidget* widget, int row, int col, int rowspan, int colspan) {
    if (_layoutType == GContainer::LAYOUT_GRID) {
        QGridLayout* gridLayout = (QGridLayout*) getQLayout();
        gridLayout->addWidget(widget, row, col, rowspan, colspan);
        _currentIndex = row * _cols + col;   // approximate
        widget->setVisible(true);
        GLayout::forceUpdate(this);
    } else {
        add(widget);
    }
}

void _Internal_QContainer::addToRegion(QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QBoxLayout* layout = (QBoxLayout*) layoutForRegion(region);
        if (region == GContainer::REGION_CENTER) {
            // center holds at most one widget
            GLayout::clearLayout(layout);

            // http://doc.qt.io/qt-5/qsizepolicy.html
            QSizePolicy sizePolicy;
            sizePolicy.setHorizontalPolicy(QSizePolicy::Ignored);
            sizePolicy.setVerticalPolicy(QSizePolicy::Ignored);
            sizePolicy.setHorizontalStretch(999);
            sizePolicy.setVerticalStretch(999);
            widget->setSizePolicy(sizePolicy);

            layout->addWidget(widget, /* stretch */ 999);
        } else {
            // add to end of the list of widgets in this region
            widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            layout->insertWidget(/* index */ layout->count() - 1, widget, /* stretch */ 0);
        }
        widget->setVisible(true);

        // set alignment of widget
        fixAlignment(widget, region);
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
        GLayout::forceUpdate(this);
    } else {
        add(widget);
    }
}

void _Internal_QContainer::clear() {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        clearRegion(GContainer::REGION_NORTH);
        clearRegion(GContainer::REGION_SOUTH);
        clearRegion(GContainer::REGION_WEST);
        clearRegion(GContainer::REGION_EAST);
        clearRegion(GContainer::REGION_CENTER);
    } else {
        GLayout::clearLayout(layout());
        _currentIndex = -1;
        setHorizontalAlignment(getHorizontalAlignment());
        setVerticalAlignment(getVerticalAlignment());
    }
}

void _Internal_QContainer::clearRegion(GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        GLayout::clearLayout(layout);
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
        GLayout::forceUpdate(this);
    } else {
        clear();
    }
}

bool _Internal_QContainer::contains(QWidget* widget) const {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        return GLayout::contains(_northLayout, widget)
                || GLayout::contains(_southLayout, widget)
                || GLayout::contains(_westLayout, widget)
                || GLayout::contains(_eastLayout, widget)
                || GLayout::contains(_centerLayout, widget);
    } else {
        return GLayout::contains(getQLayout(), widget);
    }
}

void _Internal_QContainer::fixAlignment(QWidget* widget, GContainer::Region region) {
    // needs to be run on GUI thread
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        if (_halignMap.containsKey(region) && _valignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_halignMap[region]) | toQtAlignment(_valignMap[region]));
        } else if (_halignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_halignMap[region]));
        } else if (_valignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_valignMap[region]));
        }
    } else {
        layout()->setAlignment(widget, toQtAlignment(_halign) | toQtAlignment(_valign));
    }
}

void _Internal_QContainer::fixMargin(QLayout* layout, bool hasStretch) {
    if (!hasStretch) {
        return;
    }
    int emptyCount = hasStretch ? 2 : 0;
    if (layout->count() <= emptyCount) {
        layout->setMargin(0);
    } else {
        layout->setMargin(getMargin());
    }
    GLayout::invalidateLayout(layout);
}

HorizontalAlignment _Internal_QContainer::getHorizontalAlignment() const {
    return _halign;
}

GContainer::Layout _Internal_QContainer::getLayoutType() const {
    return _layoutType;
}

int _Internal_QContainer::getMargin() const {
    return _margin;
}

QSize _Internal_QContainer::getPreferredSize() const {
    // make sure the layout has calculated everybody's position/size
    // GLayout::forceUpdate((QWidget*) this);

    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QSize north = GLayout::getProperSize(_northLayout);
        QSize south = GLayout::getProperSize(_southLayout);
        QSize west = GLayout::getProperSize(_westLayout);
        QSize east = GLayout::getProperSize(_eastLayout);

        QSize center(0, 0);
        if (!_centerLayout->isEmpty()) {
            QWidget* centerWidget = _centerLayout->itemAt(0)->widget();
            center = GLayout::getPreferredSize(centerWidget);
        }

        int preferredWidth = center.width() + west.width() + east.width();
        int preferredHeight = center.height() + north.height() + south.height();
        return QSize(preferredWidth, preferredHeight);
    } else {
        return layout()->sizeHint();
    }
}

QLayout* _Internal_QContainer::getQLayout() const {
    return layout();
}

int _Internal_QContainer::getSpacing() const {
    return _spacing;
}

VerticalAlignment _Internal_QContainer::getVerticalAlignment() const {
    return _valign;
}

void _Internal_QContainer::insert(int i, QWidget* widget) {
    switch (_layoutType) {
        case GContainer::LAYOUT_BORDER: {
            insertToRegion(i, widget, GContainer::REGION_CENTER);
            break;
        }
        case GContainer::LAYOUT_GRID: {
            int row = _cols <= 0 ? 0 : i / _cols;
            int col = _cols <= 0 ? 0 : i % _cols;
            QGridLayout* gridLayout = (QGridLayout*) getQLayout();
            gridLayout->addWidget(widget, row, col);
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        case GContainer::LAYOUT_FLOW_VERTICAL:
        default: {
            // index is off by 1 because of 'stretch' widgets at start/end
            QBoxLayout* boxLayout = (QBoxLayout*) getQLayout();
            boxLayout->insertWidget(/* index */ i - 1, widget);
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
    }
}

void _Internal_QContainer::insertToRegion(int i, QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        if (region == GContainer::REGION_CENTER) {
            // center holds at most one widget
            GLayout::clearLayout(layout);
            layout->addWidget(widget);
            widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        } else {
            // index is off by 1 because of 'stretch' widgets at start/end
            ((QBoxLayout*) layout)->insertWidget(/* index */ i + 1, widget);
            widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        }
        widget->setVisible(true);

        // set alignment of widget
        fixAlignment(widget, region);
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);

        GLayout::forceUpdate(this);
    } else {
        insert(i, widget);
    }
}

bool _Internal_QContainer::isRegionStretch(GContainer::Region region) const {
    return _regionStretchMap[region];
}

QLayout* _Internal_QContainer::layoutForRegion(GContainer::Region region) const {
    if (region == GContainer::REGION_NORTH) {
        return _northLayout;
    } else if (region == GContainer::REGION_SOUTH) {
        return _southLayout;
    } else if (region == GContainer::REGION_WEST) {
        return _westLayout;
    } else if (region == GContainer::REGION_EAST) {
        return _eastLayout;
    } else {
        return _centerLayout;
    }
}

bool _Internal_QContainer::regionContains(QWidget* widget, GContainer::Region region) const {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        return GLayout::contains(layout, widget);
    } else {
        return contains(widget);
    }
}

void _Internal_QContainer::remove(QWidget* widget) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        removeFromRegion(widget, GContainer::REGION_CENTER);
        removeFromRegion(widget, GContainer::REGION_NORTH);
        removeFromRegion(widget, GContainer::REGION_SOUTH);
        removeFromRegion(widget, GContainer::REGION_WEST);
        removeFromRegion(widget, GContainer::REGION_EAST);
    } else if (layout()) {
        layout()->removeWidget(widget);
        GLayout::forceUpdate(this);
    }
}

void _Internal_QContainer::removeFromRegion(QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        if (GLayout::contains(layout, widget)) {
            widget->setVisible(false);
            layout->removeWidget(widget);
            layout->update();
            fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
            GLayout::forceUpdate(this);
        }
    } else {
        remove(widget);
    }
}

void _Internal_QContainer::remove(int i) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        removeFromRegion(i, GContainer::REGION_CENTER);
    } else {
        QWidget* widget = layout()->itemAt(i)->widget();
        if (widget) {
            widget->setVisible(false);
        }
        // add +1 to the index to account for the 'stretch' at start
        layout()->removeItem(layout()->itemAt(i + 1));
        layout()->update();
        GLayout::forceUpdate(this);
    }
}

void _Internal_QContainer::removeFromRegion(int i, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        QWidget* widget = layout->itemAt(i)->widget();
        widget->setVisible(false);
        if (layout == _centerLayout) {
            layout->removeItem(layout->itemAt(i));
        } else {
            // add +1 to the index to account for the 'stretch' at start
            layout->removeItem(layout->itemAt(i + 1));
        }
        layout->update();
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
        GLayout::forceUpdate(this);
    } else {
        remove(i);
    }
}

void _Internal_QContainer::setPadding(int padding) {
    setPadding(padding, padding, padding, padding);
}

void _Internal_QContainer::setPadding(int top, int right, int bottom, int left) {
    QWidget::setContentsMargins(left, top, right, bottom);   // call super
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        _northLayout->setContentsMargins(left, top, right, bottom);
        _southLayout->setContentsMargins(left, top, right, bottom);
        _westLayout->setContentsMargins(left, top, right, bottom);
        _eastLayout->setContentsMargins(left, top, right, bottom);
        // _centerLayout->setContentsMargins(left, top, right, bottom);
        // _overallLayout->setContentsMargins(left, top, right, bottom);
        // _middleLayout->setContentsMargins(left, top, right, bottom);
    }
    GLayout::forceUpdate(this);
}

void _Internal_QContainer::setHorizontalAlignment(HorizontalAlignment halign) {
    switch (_layoutType) {
        case GContainer::LAYOUT_FLOW_HORIZONTAL: {
            // to align "left", limit first stretch;
            // to align "right", limit last stretch
            QHBoxLayout* layout = (QHBoxLayout*) getQLayout();
            if (layout->count() >= 2) {
                layout->removeItem(layout->itemAt(0));
                layout->removeItem(layout->itemAt(layout->count() - 1));
            }
            if (halign == ALIGN_LEFT) {
                layout->insertStretch(0, /* stretch */ 0);
                layout->addStretch(/* stretch */ 99);
            } else if (halign == ALIGN_RIGHT) {
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 0);
            } else {   // halign == ALIGN_CENTER
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 99);
            }
            break;
        }
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // set each widget's horizontal alignment individually
            QVBoxLayout* layout = (QVBoxLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(halign);
            for (int i = 1; i < layout->count() - 1; i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (widget) {
                    layout->setAlignment(widget, qtAlign);
                }
            }
            break;
        }
        case GContainer::LAYOUT_GRID: {
            // set each widget's horizontal alignment individually
            QGridLayout* layout = (QGridLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(halign);
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (widget) {
                    layout->setAlignment(widget, qtAlign);
                }
            }
            break;
        }
        case GContainer::LAYOUT_BORDER: {
            // - set align of ALL regions in align map
            setRegionHorizontalAlignment(GContainer::REGION_CENTER, halign);
            setRegionHorizontalAlignment(GContainer::REGION_NORTH, halign);
            setRegionHorizontalAlignment(GContainer::REGION_SOUTH, halign);
            setRegionHorizontalAlignment(GContainer::REGION_EAST, halign);
            setRegionHorizontalAlignment(GContainer::REGION_WEST, halign);
            break;
        }
        default: {
            break;
        }
    }
}

void _Internal_QContainer::setLayoutType(GContainer::Layout layoutType) {
    if (layoutType == _layoutType) {
        return;   // don't do work if it's the same layout
    }

    if (_layoutType == GContainer::LAYOUT_BORDER
            && layoutType != GContainer::LAYOUT_BORDER) {
        // get rid of the now-unneeded inner layouts
        _overallLayout = nullptr;
        _northLayout   = nullptr;
        _southLayout   = nullptr;
        _westLayout    = nullptr;
        _eastLayout    = nullptr;
        _centerLayout  = nullptr;
        _middleLayout  = nullptr;
    }

    _layoutType = layoutType;
    switch (layoutType) {
        case GContainer::LAYOUT_BORDER: {
            // set up border regions
            _overallLayout = new QVBoxLayout;
            _overallLayout->setObjectName(QString::fromStdString("_overallLayout_" + std::to_string(_gcontainer->getID())));
            _northLayout   = new QHBoxLayout;
            _northLayout->setObjectName(QString::fromStdString("_northLayout_" + std::to_string(_gcontainer->getID())));
            _southLayout   = new QHBoxLayout;
            _southLayout->setObjectName(QString::fromStdString("_southLayout_" + std::to_string(_gcontainer->getID())));
            _westLayout    = new QVBoxLayout;
            _westLayout->setObjectName(QString::fromStdString("_westLayout_" + std::to_string(_gcontainer->getID())));
            _eastLayout    = new QVBoxLayout;
            _eastLayout->setObjectName(QString::fromStdString("_eastLayout_" + std::to_string(_gcontainer->getID())));
            _centerLayout  = new QHBoxLayout;
            _centerLayout->setObjectName(QString::fromStdString("_centerLayout_" + std::to_string(_gcontainer->getID())));
            _middleLayout  = new QHBoxLayout;
            _middleLayout->setObjectName(QString::fromStdString("_middleLayout_" + std::to_string(_gcontainer->getID())));

            // squish margins/padding
            _overallLayout->setSpacing(0);
            _northLayout->setSpacing(getSpacing());
            _southLayout->setSpacing(getSpacing());
            _westLayout->setSpacing(getSpacing());
            _eastLayout->setSpacing(getSpacing());
            _centerLayout->setSpacing(0);
            _middleLayout->setSpacing(0);

            _overallLayout->setMargin(0);
            _overallLayout->setContentsMargins(0, 0, 0, 0);
//            _northLayout->setMargin(getMargin());
//            _southLayout->setMargin(getMargin());
//            _westLayout->setMargin(getMargin());
//            _eastLayout->setMargin(getMargin());
            _northLayout->setMargin(0);
            _northLayout->setContentsMargins(0, 0, 0, 0);
            _southLayout->setMargin(0);
            _southLayout->setContentsMargins(0, 0, 0, 0);
            _westLayout->setMargin(0);
            _westLayout->setContentsMargins(0, 0, 0, 0);
            _eastLayout->setMargin(0);
            _eastLayout->setContentsMargins(0, 0, 0, 0);
            _centerLayout->setMargin(0);
            _centerLayout->setContentsMargins(0, 0, 0, 0);
            _middleLayout->setMargin(0);
            _middleLayout->setContentsMargins(0, 0, 0, 0);

            // add "stretches" at start and end of N/S/W/E regions
            // to center and un-space the actual widgets in each
            _northLayout->addStretch(99);
            _northLayout->addStretch(99);
            _southLayout->addStretch(99);
            _southLayout->addStretch(99);
            _westLayout->addStretch(99);
            _westLayout->addStretch(99);
            _eastLayout->addStretch(99);
            _eastLayout->addStretch(99);

            // fake empty central widget as placeholder until center widget added
            _centerLayout->addStretch(999);

            _overallLayout->addLayout(_northLayout, /* stretch */ 0);
            _middleLayout->addLayout(_westLayout, /* stretch */ 0);
            _middleLayout->addLayout(_centerLayout, /* stretch */ 99);
            _middleLayout->addLayout(_eastLayout, /* stretch */ 0);
            _overallLayout->addLayout(_middleLayout, /* stretch */ 99);
            _overallLayout->addLayout(_southLayout, /* stretch */ 0);
            setLayout(_overallLayout);
            if (layout()) {
                layout()->setSpacing(0);
                layout()->setMargin(0);
            }
            setRegionAlignment(GContainer::REGION_NORTH, ALIGN_CENTER, ALIGN_MIDDLE);
            setRegionAlignment(GContainer::REGION_SOUTH, ALIGN_CENTER, ALIGN_MIDDLE);
            setRegionAlignment(GContainer::REGION_WEST, ALIGN_LEFT, ALIGN_MIDDLE);
            setRegionAlignment(GContainer::REGION_EAST, ALIGN_LEFT, ALIGN_MIDDLE);
            break;
        }
        case GContainer::LAYOUT_GRID: {
            QGridLayout* qlayout = new QGridLayout;
            if (layout()) {
                // transfer over from previous layout
                for (int i = 1; i < layout()->count() - 1; i++) {
//                    QWidget* widget = layout()->itemAt(i)->widget();
//                    qlayout->addWidget(widget);
                }
            }
            setLayout(qlayout);
            break;
        }
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            QVBoxLayout* qlayout = new QVBoxLayout;
            qlayout->setSpacing(getSpacing());
            qlayout->setMargin(getMargin());
            qlayout->addStretch(99);   // top side stretch
            if (layout()) {
                // transfer over from previous layout
                for (int i = 1; i < layout()->count() - 1; i++) {
                    QWidget* widget = layout()->itemAt(i)->widget();
                    if (widget) {
                        qlayout->addWidget(widget);
                    }
                }
            }
            qlayout->addStretch(99);   // bottom side stretch
            setLayout(qlayout);
            setHorizontalAlignment(getHorizontalAlignment());
            setVerticalAlignment(getVerticalAlignment());
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        default: {
            QHBoxLayout* qlayout = new QHBoxLayout;
            qlayout->setSpacing(getSpacing());
            qlayout->setMargin(getMargin());
            qlayout->addStretch(99);   // left side stretch
            if (layout()) {
                // transfer over from previous layout
                for (int i = 1; i < layout()->count() - 1; i++) {
                    QWidget* widget = layout()->itemAt(i)->widget();
                    qlayout->addWidget(widget);
                }
            }
            qlayout->addStretch(99);   // right side stretch
            setLayout(qlayout);
            setHorizontalAlignment(getHorizontalAlignment());
            setVerticalAlignment(getVerticalAlignment());
            break;
        }
    }
}

void _Internal_QContainer::setMargin(int margin) {
    _margin = margin;
    if (layout()) {
        if (_layoutType == GContainer::LAYOUT_BORDER) {
            fixMargin(_northLayout, /* hasStretch */ true);
            fixMargin(_southLayout, /* hasStretch */ true);
            fixMargin(_westLayout, /* hasStretch */ true);
            fixMargin(_eastLayout, /* hasStretch */ true);
            // fixMargin(_centerLayout, /* hasStretch */ false);
            // _centerLayout->setMargin(margin);
        } else {
            layout()->setMargin(margin);
        }
    }
}

void _Internal_QContainer::setRegionAlignment(GContainer::Region region,
                                              HorizontalAlignment halign,
                                              VerticalAlignment valign) {
    setRegionHorizontalAlignment(region, halign);
    setRegionVerticalAlignment(region, valign);
}

void _Internal_QContainer::setRegionHorizontalAlignment(GContainer::Region region,
                                                        HorizontalAlignment halign) {
    _halignMap[region] = halign;
    if (_layoutType != GContainer::LAYOUT_BORDER) {
        setHorizontalAlignment(halign);
        return;
    }

    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    if (layout == _westLayout || layout == _eastLayout || layout == _centerLayout) {
        // set each widget's horizontal alignment individually
        Qt::Alignment qtAlign = toQtAlignment(halign);
        QSizePolicy::Policy hSizePolicy = ((qtAlign & Qt::AlignJustify) != 0) ? QSizePolicy::Expanding : QSizePolicy::Preferred;
        for (int i = 0; i < layout->count(); i++) {
            QWidget* widget = layout->itemAt(i)->widget();
            if (!widget) {
                continue;
            }
            layout->setAlignment(widget, qtAlign);
            widget->setSizePolicy(
                    /* horizontal */ hSizePolicy,
                    /* vertical */   widget->sizePolicy().verticalPolicy());
        }
    } else if (layout == _northLayout || layout == _southLayout) {
        // to align "left", limit first stretch;
        // to align "right", limit last stretch
        QHBoxLayout* boxLayout = (QHBoxLayout*) layout;
        boxLayout->removeItem(layout->itemAt(0));
        boxLayout->removeItem(layout->itemAt(layout->count() - 1));
        int beforeStretch = (halign == ALIGN_CENTER || halign == ALIGN_RIGHT)  ? 99 : 0;
        int afterStretch  = (halign == ALIGN_LEFT   || halign == ALIGN_CENTER) ? 99 : 0;
        boxLayout->insertStretch(0, beforeStretch);
        boxLayout->addStretch(afterStretch);
    }

    layout->update();
    GLayout::forceUpdate(this);
}

void _Internal_QContainer::setRegionStretch(GContainer::Region region, bool stretch) {
    _regionStretchMap[region] = stretch;

    // stretch / de-stretch any widgets in that region

}

void _Internal_QContainer::setRegionVerticalAlignment(GContainer::Region region,
                                                      VerticalAlignment valign) {
    _valignMap[region] = valign;
    if (_layoutType != GContainer::LAYOUT_BORDER) {
        setVerticalAlignment(valign);
        return;
    }

    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

    if (layout == _westLayout || layout == _eastLayout) {
        // to align "top", limit first stretch;
        // to align "bottom", limit last stretch
        QVBoxLayout* boxLayout = (QVBoxLayout*) layout;
        boxLayout->removeItem(layout->itemAt(0));
        boxLayout->removeItem(layout->itemAt(layout->count() - 1));
        int beforeStretch = (valign == ALIGN_MIDDLE || valign == ALIGN_BOTTOM) ? 99 : 0;
        int afterStretch  = (valign == ALIGN_TOP    || valign == ALIGN_MIDDLE) ? 99 : 0;
        boxLayout->insertStretch(0, beforeStretch);
        boxLayout->addStretch(afterStretch);
    } else if (layout == _northLayout || layout == _southLayout || layout == _centerLayout) {
        // set each widget's vertical alignment individually
        Qt::Alignment qtAlign = toQtAlignment(valign);
        QSizePolicy::Policy vSizePolicy = ((qtAlign & Qt::AlignJustify) != 0) ? QSizePolicy::Expanding : QSizePolicy::Preferred;
        for (int i = 0; i < layout->count(); i++) {
            QWidget* widget = layout->itemAt(i)->widget();
            if (!widget) {
                continue;
            }
            layout->setAlignment(widget, qtAlign);
            widget->setSizePolicy(
                    /* horizontal */ widget->sizePolicy().horizontalPolicy(),
                    /* vertical */   vSizePolicy);
        }
    }

    layout->update();
    GLayout::forceUpdate(this);
}

void _Internal_QContainer::setSpacing(int spacing) {
    _spacing = spacing;
    if (layout()) {
        if (_layoutType == GContainer::LAYOUT_BORDER) {
            _northLayout->setSpacing(spacing);
            _southLayout->setSpacing(spacing);
            _westLayout->setSpacing(spacing);
            _eastLayout->setSpacing(spacing);
            _centerLayout->setSpacing(spacing);
        } else {
            layout()->setSpacing(spacing);
        }
    }
}

void _Internal_QContainer::setVerticalAlignment(VerticalAlignment valign) {
    switch (_layoutType) {
        case GContainer::LAYOUT_FLOW_HORIZONTAL: {
            // set each widget's vertical alignment individually
            QVBoxLayout* layout = (QVBoxLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(valign);
            for (int i = 1; i < layout->count() - 1; i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (!widget) {
                    continue;
                }
                layout->setAlignment(widget, qtAlign);
            }
            break;
        }
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // to align "left", limit first stretch;
            // to align "right", limit last stretch
            QVBoxLayout* layout = (QVBoxLayout*) getQLayout();
            if (layout->count() >= 2) {
                layout->removeItem(layout->itemAt(0));
                layout->removeItem(layout->itemAt(layout->count() - 1));
            }
            if (valign == ALIGN_TOP) {
                layout->insertStretch(0, /* stretch */ 0);
                layout->addStretch(/* stretch */ 99);
            } else if (valign == ALIGN_BOTTOM) {
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 0);
            } else {   // halign == ALIGN_MIDDLE
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 99);
            }
            break;
        }
        case GContainer::LAYOUT_GRID: {
            // set each widget's vertical alignment individually
            QGridLayout* layout = (QGridLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(valign);
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (!widget) {
                    continue;
                }
                layout->setAlignment(widget, qtAlign);
            }
            break;
        }
        case GContainer::LAYOUT_BORDER: {
            // - set align of ALL regions in align map
            setRegionVerticalAlignment(GContainer::REGION_CENTER, valign);
            setRegionVerticalAlignment(GContainer::REGION_NORTH, valign);
            setRegionVerticalAlignment(GContainer::REGION_SOUTH, valign);
            setRegionVerticalAlignment(GContainer::REGION_EAST, valign);
            setRegionVerticalAlignment(GContainer::REGION_WEST, valign);
            break;
        }
        default: {
            break;
        }
    }
}

QSize _Internal_QContainer::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gcontainer.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gscrollbar.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gscrollbar.cpp to replace Java version
 * @version 2018/07/16
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gscrollbar.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GScrollBar::GScrollBar(GScrollBar::Orientation orientation,
                         int value,
                         int extent,
                         int min,
                         int max,
                         QWidget* parent) {
    GThread::runOnQtGuiThread([this, orientation, parent]() {
        _iqscrollbar = new _Internal_QScrollBar(this, orientation == VERTICAL ? Qt::Vertical : Qt::Horizontal, getInternalParent(parent));
    });
    setState(value, extent, min, max);
    setVisible(false);   // all widgets are not shown until added to a window
}

GScrollBar::~GScrollBar() {
    // TODO: delete _iqscrollbar;
    _iqscrollbar = nullptr;
}

int GScrollBar::getExtent() const {
    return _iqscrollbar->pageStep();
}

_Internal_QWidget* GScrollBar::getInternalWidget() const {
    return _iqscrollbar;
}

int GScrollBar::getMax() const {
    return _iqscrollbar->maximum();
}

int GScrollBar::getMin() const {
    return _iqscrollbar->minimum();
}

GScrollBar::Orientation GScrollBar::getOrientation() const {
    return _iqscrollbar->orientation() == Qt::Vertical ? VERTICAL : HORIZONTAL;
}

std::string GScrollBar::getType() const {
    return "QGScrollBar";
}

int GScrollBar::getValue() const {
    return _iqscrollbar->value();
}

QWidget* GScrollBar::getWidget() const {
    return static_cast<QWidget*>(_iqscrollbar);
}

void GScrollBar::removeActionListener() {
    removeEventListener("change");
}

void GScrollBar::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GScrollBar::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GScrollBar::setExtent(int extent) {
    GThread::runOnQtGuiThread([this, extent]() {
        _iqscrollbar->setPageStep(extent);
    });
}

void GScrollBar::setMax(int max) {
    int min = getMin();
    require::require(min <= max, "GScrollBar::setMax", "max (" + std::to_string(max) + ") cannot be less than min (" + std::to_string(min) + ")");
    GThread::runOnQtGuiThread([this, max]() {
        _iqscrollbar->setMaximum(max);
    });
    updateSize();
}

void GScrollBar::setMin(int min) {
    int max = getMax();
    require::require(min <= max, "GScrollBar::setMin", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min]() {
        _iqscrollbar->setMinimum(min);
    });
    updateSize();
}

void GScrollBar::setState(int value, int extent, int min, int max) {
    require::require(min <= max, "GScrollBar::setState", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GScrollBar::setState", "value");
    GThread::runOnQtGuiThread([this, value, extent, min, max]() {
        _iqscrollbar->setRange(min, max);
        _iqscrollbar->setValue(value);
        _iqscrollbar->setPageStep(extent);
    });
    updateSize();
}

void GScrollBar::setValue(int value) {
    require::inRange(value, getMin(), getMax(), "GScrollBar::setValue", "value");
    GThread::runOnQtGuiThread([this, value]() {
        _iqscrollbar->setValue(value);
    });
}

void GScrollBar::updateSize() {
    GThread::runOnQtGuiThread([this]() {
        _iqscrollbar->update();
        _iqscrollbar->updateGeometry();
        if (getOrientation() == VERTICAL) {
            _iqscrollbar->setPreferredSize(_iqscrollbar->sizeHint().width(), getMax() - getMin() + 1);
        } else {
            _iqscrollbar->setPreferredSize(getMax() - getMin() + 1, _iqscrollbar->sizeHint().height());
        }
    });
}


_Internal_QScrollBar::_Internal_QScrollBar(GScrollBar* gscrollbar, Qt::Orientation orientation, QWidget* parent)
        : QScrollBar(orientation, parent),
          _gscrollbar(gscrollbar) {
    require::nonNull(gscrollbar, "_Internal_QScrollBar::constructor");
    setObjectName(QString::fromStdString("_Internal_QScrollBar_" + std::to_string(gscrollbar->getID())));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleValueChange(int)));
}

void _Internal_QScrollBar::handleValueChange(int /* value */) {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gscrollbar);
    changeEvent.setActionCommand(_gscrollbar->getActionCommand());
    _gscrollbar->fireEvent(changeEvent);
}

QSize _Internal_QScrollBar::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QScrollBar::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gscrollbar.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: glabel.cpp
 * ----------------
 *
 * @author Marty Stepp
 * @version 2018/10/04
 * - added get/setWordWrap
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/09/03
 * - added addActionListener methods for clickable labels
 * @version 2018/08/23
 * - renamed to glabel.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "glabel.h"
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "glayout.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GLabel::GLabel(const std::string& text, const std::string& iconFileName, QWidget* parent)
        : _gtext(nullptr) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlabel = new _Internal_QLabel(this, getInternalParent(parent));
    });
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GLabel::~GLabel() {
    // TODO: if (_gtext) { delete _gtext; }
    // TODO: delete _iqlabel;
    _iqlabel = nullptr;
}

void GLabel::ensureGText() {
    _gtext = new GText(getText());
    _gtext->setColor(getColor());
    _gtext->setFont(getFont());
}

GText* GLabel::getGText() const {
    return _gtext;
}

_Internal_QWidget* GLabel::getInternalWidget() const {
    return _iqlabel;
}

std::string GLabel::getLabel() const {
    return getText();
}

std::string GLabel::getText() const {
    return _iqlabel->text().toStdString();
}

GInteractor::TextPosition GLabel::getTextPosition() const {
    // TODO
//    switch (_iqlabel->toolButtonStyle()) {
//    case Qt::ToolButtonTextBesideIcon:
//        return GInteractor::TEXT_BESIDE_ICON;
//    case Qt::ToolButtonTextUnderIcon:
//        return GInteractor::TEXT_UNDER_ICON;
//    case Qt::ToolButtonTextOnly:
//    default:
//        return GInteractor::TEXT_ONLY;
//    }

    return GInteractor::TEXT_BESIDE_ICON;
}

std::string GLabel::getType() const {
    return "GLabel";
}

QWidget* GLabel::getWidget() const {
    return static_cast<QWidget*>(_iqlabel);
}

bool GLabel::hasGText() const {
    return _gtext != nullptr;
}

bool GLabel::isWordWrap() const {
    return _iqlabel->wordWrap();
}

void GLabel::removeActionListener() {
    removeEventListener("click");
}

void GLabel::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GLabel::setActionListener(GEventListener func) {
    setEventListener("click", func);
}

void GLabel::setActionListener(GEventListenerVoid func) {
    setEventListener("click", func);
}

void GLabel::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GLabel::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GLabel::setBounds(double x, double y, double width, double height) {
    if (_gtext) {
        _gtext->setBounds(x, y, width, height);
    }
    GInteractor::setBounds(x, y, width, height);
}

void GLabel::setBounds(const GRectangle& size) {
    if (_gtext) {
        _gtext->setBounds(size);
    }
    GInteractor::setBounds(size);
}

void GLabel::setColor(int rgb) {
    if (_gtext) {
        _gtext->setColor(rgb);
    }
    GInteractor::setColor(rgb);   // call super
}

void GLabel::setColor(const std::string& color) {
    if (_gtext) {
        _gtext->setColor(color);
    }
    GInteractor::setColor(color);   // call super
}

void GLabel::setFont(const QFont& font) {
    if (_gtext) {
        _gtext->setFont(font);
    }
    GInteractor::setFont(font);   // call super
}

void GLabel::setFont(const std::string& font) {
    if (_gtext) {
        _gtext->setFont(font);
    }
    GInteractor::setFont(font);   // call super
}

void GLabel::setForeground(int rgb) {
    if (_gtext) {
        _gtext->setForeground(rgb);
    }
    GInteractor::setForeground(rgb);   // call super
}

void GLabel::setForeground(const std::string& color) {
    if (_gtext) {
        _gtext->setForeground(color);
    }
    GInteractor::setForeground(color);   // call super
}

void GLabel::setHeight(double height) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setHeight(height);
    GInteractor::setHeight(height);
}

void GLabel::setIcon(const std::string& filename, bool retainIconSize) {
    GInteractor::setIcon(filename, retainIconSize);
    if (!filename.empty() && fileExists(filename)) {
        GThread::runOnQtGuiThread([this, filename, retainIconSize]() {
            if (filename.empty()) {
                _iqlabel->setPixmap(QPixmap());
            } else {
                QPixmap pixmap(QString::fromStdString(filename));
                _iqlabel->setPixmap(pixmap);
                if (retainIconSize) {
                    // TODO
                    // _iqlabel->setIconSize(pixmap.size());
                    _iqlabel->updateGeometry();
                    _iqlabel->update();
                }

                // TODO: loses text; how to have both icon and text in same label?
                if (!getText().empty()) {
                    std::cerr << "Warning: a GLabel cannot currently have both text and icon." << std::endl;
                }
            }
        });
    }
}

void GLabel::setLabel(const std::string& text) {
    setText(text);
}

void GLabel::setLocation(double x, double y) {
    ensureGText();   // setting location triggers GText mode
    _gtext->setLocation(x, y);
    GInteractor::setLocation(x, y);
}

void GLabel::setSize(double width, double height) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setSize(width, height);
    GInteractor::setSize(width, height);
}

void GLabel::setSize(const GDimension& size) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setSize(size);
    GInteractor::setSize(size);
}

void GLabel::setText(const std::string& text) {
    if (_gtext) {
        _gtext->setText(text);
    }
    GThread::runOnQtGuiThread([this, text]() {
        _iqlabel->setText(QString::fromStdString(text));
        GLayout::forceUpdate(_iqlabel);
    });
}

void GLabel::setTextPosition(GInteractor::TextPosition position) {
    // TODO: doesn't really work because a label can't have both text and icon in Qt
    if (position == GInteractor::TEXT_UNDER_ICON) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    } else if (position == GInteractor::TEXT_BESIDE_ICON) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else if (position == GInteractor::TEXT_ONLY) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}

void GLabel::setVisible(bool visible) {
    if (_gtext) {
        _gtext->setVisible(visible);
    }
    GInteractor::setVisible(visible);   // call super
}

void GLabel::setWidth(double width) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setWidth(width);
    GInteractor::setWidth(width);
}

void GLabel::setWordWrap(bool wrap) {
    GThread::runOnQtGuiThread([this, wrap]() {
        _iqlabel->setWordWrap(wrap);
    });
}

void GLabel::setX(double x) {
    ensureGText();   // setting location triggers GText mode
    _gtext->setX(x);
    GInteractor::setX(x);
}

void GLabel::setY(double y) {
    ensureGText();   // setting location triggers GText mode
    _gtext->setY(y);
    GInteractor::setY(y);
}


_Internal_QLabel::_Internal_QLabel(GLabel* glabel, QWidget* parent)
        : QLabel(parent),
          _glabel(glabel) {
    require::nonNull(glabel, "_Internal_QLabel::constructor");
    setObjectName(QString::fromStdString("_Internal_QLabel_" + std::to_string(glabel->getID())));
}

void _Internal_QLabel::mouseDoubleClickEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QLabel::mouseDoubleClickEvent", "event");
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_glabel->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _glabel);
    mouseEvent.setActionCommand(_glabel->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _glabel->fireEvent(mouseEvent);
}

void _Internal_QLabel::mousePressEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QLabel::mousePressEvent", "event");
    QWidget::mousePressEvent(event);   // call super

    // fire the signal/event only for left-clicks
    if (!(event->button() & Qt::LeftButton)) {
        return;
    }

    emit clicked();

    if (!_glabel->isAcceptingEvent("click")) return;

    GEvent actionEvent(
                /* class  */ ACTION_EVENT,
                /* type   */ ACTION_PERFORMED,
                /* name   */ "click",
                /* source */ _glabel);
    actionEvent.setActionCommand(_glabel->getActionCommand());
    actionEvent.setButton((int) event->button());
    actionEvent.setX(event->x());
    actionEvent.setY(event->y());
    _glabel->fireEvent(actionEvent);
}

QSize _Internal_QLabel::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QLabel::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_glabel.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gobjects.cpp
 * ------------------
 * This file implements the gobjects.h interface.
 *
 * @author Marty Stepp
 * @version 2018/09/14
 * - added opacity support
 * - added GCanvas-to-GImage conversion support
 * @version 2018/08/23
 * - renamed to gobjects.cpp to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gobjects.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <QBrush>
#include <QFont>
#include <QPointF>
#include <QPolygon>
#include <QVector>
#include <sstream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gmath.h"
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

const double GRoundRect::DEFAULT_CORNER = 10.0;
const std::string GText::DEFAULT_FONT = "Dialog-13";

// static constants
STATIC_CONST_VARIABLE_DECLARE(double, LINE_TOLERANCE, 1.5)
STATIC_CONST_VARIABLE_DECLARE(double, ARC_TOLERANCE, 2.5)
STATIC_VARIABLE_DECLARE_BLANK(QBrush, DEFAULT_BRUSH)
STATIC_VARIABLE_DECLARE_BLANK(QFont, DEFAULT_QFONT)
STATIC_VARIABLE_DECLARE(bool, DEFAULT_QFONT_SET, false)

/**
 * Returns the square of the distance between two points.
 * Used when checking to see if a line touches a given point.
 * @private
 */
static double dsq(double x0, double y0, double x1, double y1);


/* GObject class */

bool GObject::_sAntiAliasing = true;

GObject::GObject(double x, double y, double width, double height)
        : _x(x),
          _y(y),
          _width(width),
          _height(height),
          _lineWidth(1),
          _opacity(1.0),
          _lineStyle(GObject::LINE_SOLID),
          _color(""),
          _colorInt(0),
          _fillColor(""),
          _fillColorInt(0),
          _font(""),
          _fillFlag(false),
          _visible(true),
          _transformed(false),
          _parent(nullptr) {
    // http://doc.qt.io/qt-5/qpen.html#cap-style
    _pen.setJoinStyle(Qt::MiterJoin);   // don't round corners of line edges
    _pen.setMiterLimit(99.0);
    _pen.setCapStyle(Qt::FlatCap);      // don't overextend line endpoint
    _brush.setStyle(Qt::SolidPattern);
}

GObject::~GObject() {
    // empty
}

bool GObject::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        return getBounds().contains(x, y);
    } else {
        return getBounds().contains(x, y);
    }
}

bool GObject::contains(const GPoint& pt) const {
    return contains(pt.getX(), pt.getY());
}

GPoint GObject::getBottomRightLocation() const {
    return GPoint(getRightX(), getBottomY());
}

double GObject::getBottomY() const {
    return getY() + getHeight();
}

GRectangle GObject::getBounds() const {
    if (isTransformed()) {
        // TODO
        return GRectangle(getX(), getY(), getWidth(), getHeight());
    } else {
        return GRectangle(getX(), getY(), getWidth(), getHeight());
    }
}

GPoint GObject::getCenterLocation() const {
    return GPoint(getCenterX(), getCenterY());
}

double GObject::getCenterX() const {
    return getX() + getWidth() / 2;
}

double GObject::getCenterY() const {
    return getY() + getHeight() / 2;
}

std::string GObject::getColor() const {
    return _color;
}

std::string GObject::getFillColor() const {
    return _fillColor;
}

double GObject::getHeight() const {
    return _height;
}

GObject::LineStyle GObject::getLineStyle() const {
    return _lineStyle;
}

double GObject::getLineWidth() const {
    return _lineWidth;
}

GPoint GObject::getLocation() const {
    return GPoint(getX(), getY());
}

double GObject::getOpacity() const {
    return _opacity;
}

GCompound* GObject::getParent() const {
    return _parent;
}

double GObject::getRightX() const {
    return getX() + getWidth();
}

GDimension GObject::getSize() const {
    GRectangle bounds = getBounds();
    return GDimension(bounds.getWidth(), bounds.getHeight());
}

double GObject::getWidth() const {
    return _width;
}

double GObject::getX() const {
    return _x;
}

double GObject::getY() const {
    return _y;
}

void GObject::initializeBrushAndPen(QPainter* painter) {
    if (!painter) {
        return;
    }
    if (GColor::hasAlpha(_color)) {
        _pen.setColor(GColor::toQColorARGB(_colorInt));   // allow alpha
    } else {
        _pen.setColor(QColor(_colorInt));
    }
    _pen.setWidth((int) _lineWidth);
    _pen.setStyle(toQtPenStyle(_lineStyle));

    // http://doc.qt.io/qt-5/qpen.html#join-style
    painter->setPen(_pen);

    // font
    if (!STATIC_VARIABLE(DEFAULT_QFONT_SET)) {
        STATIC_VARIABLE(DEFAULT_QFONT) = painter->font();
        STATIC_VARIABLE(DEFAULT_BRUSH).setColor(QColor(0x00ffffff));
    }
    if (_font.empty()) {
        painter->setFont(STATIC_VARIABLE(DEFAULT_QFONT));
    } else {
        painter->setFont(GFont::toQFont(_font));
    }


    // fill color
    if (_fillFlag) {
        if (GColor::hasAlpha(_fillColor)) {
            _brush.setColor(GColor::toQColorARGB(_fillColorInt));   // allow alpha
        } else {
            _brush.setColor(QColor(_fillColorInt));
        }
        painter->setBrush(_brush);
    } else {
        painter->setBrush(STATIC_VARIABLE(DEFAULT_BRUSH));
    }

    // opacity
    painter->setOpacity(_opacity);

    // transform
    painter->setTransform(_transform, /* combine */ false);
}

bool GObject::isAntiAliasing() {
    return _sAntiAliasing;
}

bool GObject::isFilled() const {
    return _fillFlag;
}

bool GObject::isTransformed() const {
    return _transformed;
}

bool GObject::isVisible() const {
    return _visible;
}

void GObject::move(double dx, double dy) {
    setLocation(getX() + dx, getY() + dy);   // calls repaint
}

void GObject::repaint() {
    // really instructs the GCompound parent to redraw itself
    GCompound* parent = getParent();
    while (parent && parent->getParent()) {
        parent = parent->getParent();
    }
    if (parent) {
        parent->conditionalRepaint();
    }
}

void GObject::resetTransform() {
    _transform = QTransform();
    _transformed = false;
    repaint();
}

void GObject::rotate(double theta) {
    _transformed = true;
    _transform = _transform.rotate(theta);
    repaint();
}

void GObject::scale(double sf) {
    scale(sf, sf);   // calls repaint
}

void GObject::scale(double sx, double sy) {
    _transformed = true;
    _transform = _transform.scale(sx, sy);
    repaint();
}

void GObject::sendBackward() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendBackward(this);
    }
}

void GObject::sendForward() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendForward(this);
    }
}

void GObject::sendToBack() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendToBack(this);
    }
}

void GObject::sendToFront() {
    GCompound* parent = getParent();
    if (parent) {
        parent->sendToFront(this);
    }
}

void GObject::setAntiAliasing(bool value) {
    _sAntiAliasing = value;
}

void GObject::setBottomY(double y) {
    setBottomRightLocation(getRightX(), y);   // calls repaint
}

void GObject::setRightX(double x) {
    setBottomRightLocation(x, getBottomY());   // calls repaint
}

void GObject::setBottomRightLocation(double x, double y) {
    setLocation(x - getWidth(), y - getHeight());   // calls repaint
}

void GObject::setBottomRightLocation(const GPoint& pt) {
    setBottomRightLocation(pt.getX(), pt.getY());   // calls repaint
}

void GObject::setBounds(double x, double y, double width, double height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    repaint();
}

void GObject::setBounds(const GRectangle& bounds) {
    setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void GObject::setCenterX(double x) {
    setCenterLocation(x, getCenterY());   // calls repaint
}

void GObject::setCenterY(double y) {
    setCenterLocation(getCenterX(), y);   // calls repaint
}

void GObject::setCenterLocation(double x, double y) {
    setLocation(x - getWidth() / 2, y - getHeight() / 2);   // calls repaint
}

void GObject::setCenterLocation(const GPoint& pt) {
    setCenterLocation(pt.getX(), pt.getY());   // calls repaint
}

void GObject::setColor(int r, int g, int b) {
    _color = GColor::convertRGBToColor(r, g, b);
    _colorInt = GColor::convertRGBToRGB(r, g, b);
    repaint();
}

void GObject::setColor(int rgb) {
    _color = GColor::convertRGBToColor(rgb);
    _colorInt = rgb;
    repaint();
}

void GObject::setColor(const std::string& color) {
    if (GColor::hasAlpha(color)) {
        _color = color;
        _colorInt = GColor::convertColorToRGB(color);
        repaint();
    } else {
        setColor(GColor::convertColorToRGB(color));
    }
}

void GObject::setFillColor(int r, int g, int b) {
    _fillColor = GColor::convertRGBToColor(r, g, b);
    _fillColorInt = GColor::convertRGBToRGB(r, g, b);
    repaint();
}

void GObject::setFillColor(int rgb) {
    _fillColor = GColor::convertRGBToColor(rgb);
    _fillColorInt = rgb;
    repaint();
}

void GObject::setFillColor(const std::string& color) {
    _fillColor = color;
    _fillColorInt = GColor::convertColorToRGB(color);
    if (_fillColor == "") {
        _fillFlag = false;
    } else {
        if (!GColor::hasAlpha(color)) {
            _fillColor = GColor::convertRGBToColor(_fillColorInt);
        }
        _fillFlag = true;
    }
    repaint();
}

void GObject::setFilled(bool flag) {
    _fillFlag = flag;
    repaint();
}

void GObject::setFont(const QFont& font) {
    setFont(GFont::toFontString(font));
}

void GObject::setFont(const std::string& font) {
    _font = font;
    repaint();
}

void GObject::setForeground(int r, int g, int b) {
    setColor(r, g, b);
}

void GObject::setForeground(int rgb) {
    setColor(rgb);
}

void GObject::setForeground(const std::string& color) {
    setColor(color);
}

void GObject::setHeight(double height) {
    setSize(getWidth(), height);
}

void GObject::setLineStyle(GObject::LineStyle lineStyle) {
    _lineStyle = lineStyle;
    repaint();
}

void GObject::setLineWidth(double lineWidth) {
    _lineWidth = lineWidth;
    repaint();
}

void GObject::setLocation(double x, double y) {
    _x = x;
    _y = y;
    repaint();
}

void GObject::setLocation(const GPoint& pt) {
    setLocation(pt.getX(), pt.getY());   // calls repaint
}

void GObject::setOpacity(double opacity) {
    require::inRange(opacity, 0.0, 1.0, "GObject::setOpacity");
    _opacity = opacity;
    repaint();
}

void GObject::setSize(double width, double height) {
    if (isTransformed()) {
        error("GObject::setSize: Object has been transformed");
    }
    _width = width;
    _height = height;
    repaint();
}

void GObject::setSize(const GDimension& size) {
    setSize(size.getWidth(), size.getHeight());   // calls repaint
}

void GObject::setVisible(bool flag) {
    _visible = flag;
    repaint();
}

void GObject::setWidth(double width) {
    setSize(width, getHeight());
}

void GObject::setX(double x) {
    setLocation(x, getY());   // calls repaint
}

void GObject::setY(double y) {
    setLocation(getX(), y);   // calls repaint
}

std::string GObject::toString() const {
    std::string extra = toStringExtra();
    return getType()
            + "("
            + "x=" + std::to_string(_x)
            + ",y=" + std::to_string(_y)
            + ",w=" + std::to_string(_width)
            + ",h=" + std::to_string(_height)
            + (_lineWidth <= 1 ? "" : (",lineWidth=" + std::to_string(_lineWidth)))
            + (_color.empty() ? "" : (",color=" + _color))
            + (_fillColor.empty() ? "" : (",fillColor=" + _fillColor))
            + (_font.empty() ? "" : (",font=" + _font))
            + (_visible ? "" : (",visible=" + boolToString(_visible)))
            + (extra.empty() ? "" : ("," + extra))
            + ")";
}

Qt::PenStyle GObject::toQtPenStyle(GObject::LineStyle lineStyle) {
    switch (lineStyle) {
    case GObject::LINE_DASH:
        return Qt::DashLine;
    case GObject::LINE_DASH_DOT:
        return Qt::DashDotLine;
    case GObject::LINE_DASH_DOT_DOT:
        return Qt::DashDotDotLine;
    case GObject::LINE_DOT:
        return Qt::DotLine;
    case GObject::LINE_NONE:
        return Qt::NoPen;
    case GObject::LINE_SOLID:
    default:
        return Qt::SolidLine;
    }
}

std::string GObject::toStringExtra() const {
    return "";
}


GArc::GArc(double width, double height, double start, double sweep)
        : GObject(/* x */ 0, /* y */ 0, width, height),
          _start(start),
          _sweep(sweep) {
    // empty
}

GArc::GArc(double x, double y, double width, double height, double start, double sweep)
        : GObject(x, y, width, height),
          _start(start),
          _sweep(sweep) {
    // empty
}

bool GArc::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (getX() + rx);
    double dy = y - (getY() + ry);
    double r = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);
    if (isFilled()) {
        if (r > 1.0) {
            return false;
        }
    } else {
        double t = STATIC_VARIABLE(ARC_TOLERANCE) / ((rx + ry) / 2);
        if (std::fabs(1.0 - r) > t) {
            return false;
        }
    }

    // JL BUGFIX: must scale by ry, rx.
    return containsAngle(atan2(-dy/ry, dx/rx) * 180 / PI);
}

bool GArc::containsAngle(double theta) const {
    double start = std::min(_start, _start + _sweep);
    double sweep = std::abs(_sweep);
    if (sweep >= 360) {
        return true;
    }
    theta = (theta < 0) ? 360 - fmod(-theta, 360) : fmod(theta, 360);
    start = (start < 0) ? 360 - fmod(-start, 360) : fmod(start, 360);
    if (start + sweep > 360) {
        return theta >= start || theta <= start + sweep - 360;
    } else {
        return theta >= start && theta <= start + sweep;
    }
}

void GArc::draw(QPainter* painter) {
    // for some reason, Qt's arc-drawing functionality asks for angles in
    // 1/16ths of a degree. okay sure whatever
    static const int QT_ANGLE_SCALE_FACTOR = 16;
    initializeBrushAndPen(painter);
    painter->drawChord((int) getX(), (int) getY(),
                       (int) getWidth(), (int) getHeight(),
                       (int) (_start * QT_ANGLE_SCALE_FACTOR),
                       (int) (_sweep * QT_ANGLE_SCALE_FACTOR));
}

GPoint GArc::getArcPoint(double theta) const {
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    double cx = getX() + rx;
    double cy = getY() + ry;
    double radians = theta * PI / 180;
    return GPoint(cx + rx * cos(radians), cy - ry * sin(radians));
}

GRectangle GArc::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    double cx = getX() + rx;
    double cy = getY() + ry;
    double startRadians = _start * PI / 180;
    double sweepRadians = _sweep * PI / 180;
    double p1x = cx + cos(startRadians) * rx;
    double p1y = cy - sin(startRadians) * ry;
    double p2x = cx + cos(startRadians + sweepRadians) * rx;
    double p2y = cy - sin(startRadians + sweepRadians) * ry;
    double xMin = std::min(p1x, p2x);
    double xMax = std::max(p1x, p2x);
    double yMin = std::min(p1y, p2y);
    double yMax = std::max(p1y, p2y);
    if (containsAngle(0)) xMax = cx + rx;
    if (containsAngle(90)) yMin = cy - ry;
    if (containsAngle(180)) xMin = cx - rx;
    if (containsAngle(270)) yMax = cy + ry;
    if (isFilled()) {
        xMin = std::min(xMin, cx);
        yMin = std::min(yMin, cy);
        xMax = std::max(xMax, cx);
        yMax = std::max(yMax, cy);
    }
    return GRectangle(xMin, yMin, xMax - xMin, yMax - yMin);
}

GPoint GArc::getEndPoint() const {
    return getArcPoint(_start + _sweep);
}

GRectangle GArc::getFrameRectangle() const {
    return getBounds();
}

double GArc::getStartAngle() const {
    return _start;
}

GPoint GArc::getStartPoint() const {
    return getArcPoint(_start);
}

double GArc::getSweepAngle() const {
    return _sweep;
}

std::string GArc::getType() const {
    return "GArc";
}

void GArc::setFrameRectangle(double x, double y, double width, double height) {
    setBounds(x, y, width, height);   // calls repaint
}

void GArc::setFrameRectangle(const GRectangle& rect) {
    setFrameRectangle(rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());   // calls repaint
}

void GArc::setStartAngle(double start) {
    _start = start;
    repaint();
}

void GArc::setSweepAngle(double sweep) {
    _sweep = sweep;
    repaint();
}

std::string GArc::toStringExtra() const {
    std::ostringstream oss;
    oss << " start=" << _start << " sweep=" << _sweep;
    return oss.str();
}


GCompound::GCompound()
        : _autoRepaint(true) {
    // empty
}

void GCompound::add(GObject* gobj) {
    require::nonNull(gobj, "GCompound::add");
    for (int i = _contents.size() - 1; i >= 0; i--) {   // avoid duplicates
        if (_contents[i] == gobj) {
            return;
        }
    }
    _contents.add(gobj);
    gobj->_parent = this;
    if (gobj->isTransformed()) {
        conditionalRepaint();
    } else {
        conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
    }
}

void GCompound::add(GObject* gobj, double x, double y) {
    require::nonNull(gobj, "GCompound::add");
    gobj->setLocation(x, y);
    add(gobj);   // calls conditionalRepaint
}

void GCompound::add(GObject& gobj) {
    add(&gobj);
}

void GCompound::add(GObject& gobj, double x, double y) {
    add(&gobj, x, y);
}

void GCompound::clear() {
    removeAll();   // calls conditionalRepaint
}

void GCompound::conditionalRepaint() {
    if (_autoRepaint) {
        repaint();
    }
}

void GCompound::conditionalRepaintRegion(int x, int y, int width, int height) {
    if (_autoRepaint) {
        repaintRegion(x, y, width, height);
    }
}

void GCompound::conditionalRepaintRegion(const GRectangle& bounds) {
    if (_autoRepaint) {
        repaintRegion(bounds);
    }
}

bool GCompound::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    for (int i = 0, sz = _contents.size(); i < sz; i++) {
        if (_contents[i]->contains(x, y)) {
            return true;
        }
    }
    return false;
}

void GCompound::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    // TODO: uncomment this? need settings to apply to every shape
    // initializeBrushAndPen(painter);   //
    for (GObject* obj : _contents) {
        if (obj->isVisible()) {
            obj->draw(painter);
        }
    }
}

int GCompound::findGObject(GObject* gobj) const {
    int n = _contents.size();
    for (int i = 0; i < n; i++) {
        if (_contents.get(i) == gobj) {
            return i;
        }
    }
    return -1;
}

GRectangle GCompound::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double xMin = +1E20;
    double yMin = +1E20;
    double xMax = -1E20;
    double yMax = -1E20;
    for (int i = 0; i < _contents.size(); i++) {
        GRectangle bounds = _contents.get(i)->getBounds();
        xMin = std::min(xMin, bounds.getX());
        yMin = std::min(yMin, bounds.getY());
        xMax = std::max(xMax, bounds.getX());
        yMax = std::max(yMax, bounds.getY());   // JL BUGFIX 2016/10/11
    }
    // JL BUGFIX: shifted anchor point
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

GObject* GCompound::getElement(int index) const {
    return _contents.get(index);
}

GObject* GCompound::getElementAt(double x, double y) const {
    for (GObject* gobj : _contents) {
        if (gobj && gobj->contains(x, y)) {
            return gobj;
        }
    }
    return nullptr;
}

int GCompound::getElementCount() const {
    return _contents.size();
}

std::string GCompound::getType() const {
    return "GCompound";
}

QWidget* GCompound::getWidget() const {
    return _widget;
}

bool GCompound::isAutoRepaint() const {
    return _autoRepaint;
}

bool GCompound::isEmpty() const {
    return _contents.size() == 0;
}

void GCompound::remove(GObject* gobj) {
    require::nonNull(gobj, "GCompound::remove");
    int index = findGObject(gobj);
    if (index != -1) {
        removeAt(index);   // calls conditionalRepaint
    }
}

void GCompound::remove(GObject& gobj) {
    remove(&gobj);
}

void GCompound::removeAll() {
    bool wasEmpty = _contents.isEmpty();
    Vector<GObject*> contentsCopy = _contents;
    _contents.clear();
    for (GObject* obj : contentsCopy) {
        obj->_parent = nullptr;
        // TODO: delete obj;
    }
    if (!wasEmpty) {
        conditionalRepaint();
    }
}

void GCompound::removeAt(int index) {
    GObject* gobj = _contents[index];
    _contents.remove(index);
    gobj->_parent = nullptr;
    if (gobj->isTransformed()) {
        conditionalRepaint();
    } else {
        conditionalRepaintRegion(gobj->getBounds().enlargedBy((gobj->getLineWidth() + 1) / 2));
    }
}

void GCompound::repaint() {
    if (!_widget) {
        return;
    }

    // actual repainting must be done in the Qt GUI thread
    if (GThread::iAmRunningOnTheQtGuiThread()) {
        _widget->repaint();   // TODO: change to update()?
    } else {
        GThread::runOnQtGuiThread([this]() {
            _widget->repaint();   // TODO: change to update()?
        });
    }
}

void GCompound::repaintRegion(int x, int y, int width, int height) {
    if (!_widget) {
        return;
    }

    // actual repainting must be done in the Qt GUI thread
    if (GThread::iAmRunningOnTheQtGuiThread()) {
        _widget->repaint(x, y, width, height);
    } else {
        GThread::runOnQtGuiThread([this, x, y, width, height]() {
            _widget->repaint(x, y, width, height);
        });
    }
}

void GCompound::repaintRegion(const GRectangle& bounds) {
    repaintRegion((int) bounds.getX(), (int) bounds.getY(),
                  (int) bounds.getWidth(), (int) bounds.getHeight());
}

void GCompound::sendBackward(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendBackward");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != 0) {
        _contents.remove(index);
        _contents.insert(index - 1, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendBackward(gobj);
        conditionalRepaint();
    }
}

void GCompound::sendForward(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendForward");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != _contents.size() - 1) {
        _contents.remove(index);
        _contents.insert(index + 1, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendForward(gobj);
        conditionalRepaint();
    }
}

void GCompound::sendToBack(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendToBack");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != 0) {
        _contents.remove(index);
        _contents.insert(0, gobj);
        // stanfordcpplib::getPlatform()->gobject_sendToBack(gobj);
        conditionalRepaint();
    }
}

void GCompound::sendToFront(GObject* gobj) {
    require::nonNull(gobj, "GCompound::sendToFront");
    int index = findGObject(gobj);
    if (index == -1) {
        return;
    }
    if (index != _contents.size() - 1) {
        _contents.remove(index);
        _contents.add(gobj);
        conditionalRepaint();
    }
}

void GCompound::setAutoRepaint(bool autoRepaint) {
    _autoRepaint = autoRepaint;
}

void GCompound::setWidget(QWidget* widget) {
    _widget = widget;
}

std::string GCompound::toString() const {
    return "GCompound(...)";
}


GImage::GImage(const std::string& filename, double x, double y)
        : GObject(x, y),
          _filename(filename),
          _qimage(nullptr) {
    if (!_filename.empty()) {
        if (!fileExists(_filename)) {
            error("GImage: file not found: \"" + filename + "\"");
        }
        // load image
        bool hasError = false;
        GThread::runOnQtGuiThread([this, filename, &hasError]() {
            _qimage = new QImage;
            if (_qimage->load(QString::fromStdString(_filename))) {
                _width = _qimage->width();
                _height = _qimage->height();
            } else {
                hasError = true;
            }
        });

        if (hasError) {
            error("GImage: unable to load image from: \"" + filename + "\"");
        }
    }
}

GImage::GImage(double width, double height) {
    require::nonNegative2D(width, height, "GImage::constructor", "width", "height");
    _width = width;
    _height = height;
    GThread::runOnQtGuiThread([this]() {
        _qimage = new QImage(static_cast<int>(_width), static_cast<int>(_height), QImage::Format_ARGB32);
    });
}

GImage::GImage(QImage* qimage) {
    require::nonNull(qimage, "GImage::constructor");
    _qimage = qimage;
    _width = _qimage->width();
    _height = _qimage->height();
}

GImage::~GImage() {
    // TODO: delete _image;
    _qimage = nullptr;
}

void GImage::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    double myX = getX();
    double myY = getY();
    double myWidth = getWidth();
    double myHeight = getHeight();
    double imgWidth = _qimage->width();
    double imgHeight = _qimage->height();
    painter->setOpacity(_opacity);
    painter->setTransform(_transform, /* combine */ false);
    if (floatingPointEqual(myWidth, imgWidth) && floatingPointEqual(myHeight, imgHeight)) {
        // draw unscaled
        painter->drawImage((int) myX, (int) myY, *_qimage);
    } else {
        // draw scaled
        QRectF rect(myX, myY, myWidth, myHeight);
        painter->drawImage(rect, *_qimage);
    }
}

std::string GImage::getFileName() const {
    return _filename;
}

int GImage::getPixel(int x, int y) const {
    require::inRange2D(x, y, (int) getWidth() - 1, (int) getHeight() - 1, "GImage::getPixel", "x", "y");
    return (int) _qimage->pixel(x, y);
}

QImage* GImage::getQImage() const {
    return _qimage;
}

std::string GImage::getType() const {
    return "GImage";
}

void GImage::setPixel(int x, int y, int rgb) {
    _qimage->setPixel(x, y, rgb);
}

std::string GImage::toStringExtra() const {
    return "filename=\"" + _filename + "\"";
}


GLine::GLine(double x0, double y0, double x1, double y1, GObject::LineStyle lineStyle)
        : GObject(x0, y0),
          _dx(x1 - x0),
          _dy(y1 - y0) {
    setLineStyle(lineStyle);
}

GLine::GLine(const GPoint& p0, const GPoint& p1)
        : GObject(p0.getX(), p0.getY()),
          _dx(p1.getX() - p0.getX()),
          _dy(p1.getY() - p0.getY()) {
    // empty
}

bool GLine::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double x0 = getX();
    double y0 = getY();
    double x1 = x0 + _dx;
    double y1 = y0 + _dy;
    double tSquared = STATIC_VARIABLE(LINE_TOLERANCE) * STATIC_VARIABLE(LINE_TOLERANCE);
    if (dsq(x, y, x0, y0) < tSquared) {
        return true;
    }
    if (dsq(x, y, x1, y1) < tSquared) {
        return true;
    }
    if (x < std::min(x0, x1) - STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (x > std::max(x0, x1) + STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (y < std::min(y0, y1) - STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (y > std::max(y0, y1) + STATIC_VARIABLE(LINE_TOLERANCE)) {
        return false;
    }
    if (floatingPointEqual(x0 - x1, 0) && floatingPointEqual(y0 - y1, 0)) {
        return false;
    }
    double u = ((x - x0) * (x1 - x0) + (y - y0) * (y1 - y0))
            / dsq(x0, y0, x1, y1);
    return dsq(x, y, x0 + u * (x1 - x0), y0 + u * (y1 - y0)) < tSquared;
}

void GLine::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawLine((int) getX(), (int) getY(), (int) (getX() + _dx), (int) getY() + _dy);
}

GRectangle GLine::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double x0 = (_dx < 0) ? getX() + _dx : getX();
    double y0 = (_dy < 0) ? getY() + _dy : getY();
    return GRectangle(x0, y0, getWidth(), getHeight());
}

GPoint GLine::getEndPoint() const {
    return GPoint(getX() + _dx, getY() + _dy);
}

double GLine::getEndX() const {
    return getX() + _dx;
}

double GLine::getEndY() const {
    return getY() + _dy;
}

double GLine::getHeight() const {
    return std::fabs(_dy);
}

GPoint GLine::getStartPoint() const {
    return getLocation();
}

double GLine::getStartX() const {
    return getX();
}

double GLine::getStartY() const {
    return getY();
}

std::string GLine::getType() const {
    return "GLine";
}

double GLine::getWidth() const {
    return std::fabs(_dx);
}

void GLine::setEndPoint(double x, double y) {
    _dx = x - this->getX();
    _dy = y - this->getY();
    repaint();
}

void GLine::setStartPoint(double x, double y) {
    _dx += getX() - x;
    _dy += getY() - y;
    setLocation(x, y);   // calls repaint
}

std::string GLine::toStringExtra() const {
    std::ostringstream oss;
    oss << "x2=" << (_x + _dx) << " y2=" << (_y + _dy);
    return oss.str();
}


GOval::GOval(double x, double y, double width, double height)
        : GObject(x, y, width, height) {
    // empty
}

bool GOval::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    double rx = getWidth() / 2;
    double ry = getHeight() / 2;
    if (floatingPointEqual(rx, 0) || floatingPointEqual(ry, 0)) {
        return false;
    }
    double dx = x - (getX() + rx);
    double dy = y - (getY() + ry);
    return (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry) <= 1.0;
}

void GOval::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawEllipse((int) getX(), (int) getY(), (int) getWidth(), (int) getHeight());
}

std::string GOval::getType() const {
    return "GOval";
}


GPolygon::GPolygon() {
    // empty
}

GPolygon::GPolygon(std::initializer_list<double> coords) {
    addVertexes(coords);
}

GPolygon::GPolygon(std::initializer_list<GPoint> points) {
    addVertexes(points);
}

void GPolygon::addEdge(double dx, double dy) {
    addVertex(_cx + dx, _cy + dy);
}

void GPolygon::addEdge(const GPoint& pt) {
    addEdge(pt.getX(), pt.getY());
}

void GPolygon::addEdges(std::initializer_list<double> coords) {
    int i = 0;
    double dx = 0;
    double dy = 0;
    for (double d : coords) {
        if (i % 2 == 0) {
            dx = d;
        } else {
            dy = d;
            addEdge(dx, dy);
        }
        i++;
    }
}

void GPolygon::addEdges(std::initializer_list<GPoint> points) {
    for (GPoint pt : points) {
        addEdge(pt);
    }
}

void GPolygon::addPolarEdge(double r, double theta) {
    addEdge(r * cos(theta * PI / 180), -r * sin(theta * PI / 180));
}

void GPolygon::addVertex(double x, double y) {
    _cx = x;
    _cy = y;
    _vertices.append(QPointF(_cx, _cy));
    repaint();
}

void GPolygon::addVertex(const GPoint& pt) {
    addVertex(pt.getX(), pt.getY());
}

void GPolygon::addVertexes(std::initializer_list<double> coords) {
    int i = 0;
    double x = 0;
    double y = 0;
    for (double d : coords) {
        if (i % 2 == 0) {
            x = d;
        } else {
            y = d;
            addVertex(x, y);
        }
        i++;
    }
}

void GPolygon::addVertexes(std::initializer_list<GPoint> points) {
    for (GPoint pt : points) {
        addVertex(pt);
    }
}

void GPolygon::clear() {
    _vertices.clear();
    repaint();
}

bool GPolygon::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }
    int crossings = 0;
    int n = _vertices.size();
    if (n < 2) {
        return false;
    }
    if (_vertices[0] == _vertices[n - 1]) {
        n--;
    }
    double x0 = _vertices[0].x();
    double y0 = _vertices[0].y();
    for (int i = 1; i <= n; i++) {
        double x1 = _vertices[i % n].x();
        double y1 = _vertices[i % n].y();
        if ((y0 > y) != (y1 > y) && x - x0 < (x1 - x0) * (y - y0) / (y1 - y0)) {
            crossings++;
        }
        x0 = x1;
        y0 = y1;
    }
    return (crossings % 2 == 1);
}

void GPolygon::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawPolygon(QPolygonF(_vertices));
}

GRectangle GPolygon::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    double xMin = 0;
    double yMin = 0;
    double xMax = 0;
    double yMax = 0;
    for (int i = 0; i < _vertices.size(); i++) {
        double x = _vertices[i].x();
        double y = _vertices[i].y();
        if (i == 0 || x < xMin) xMin = x;
        if (i == 0 || y < yMin) yMin = y;
        if (i == 0 || x > xMax) xMax = x;
        if (i == 0 || y > yMax) yMax = y;
    }
    // JL BUGFIX: add getX, getY
    return GRectangle(xMin + getX(), yMin + getY(), xMax - xMin, yMax - yMin);
}

double GPolygon::getHeight() const {
    return getBounds().getHeight();
}

std::string GPolygon::getType() const {
    return "GPolygon";
}

GPoint GPolygon::getVertex(int i) const {
    return GPoint(_vertices[i].x(), _vertices[i].y());
}

int GPolygon::getVertexCount() const {
    return _vertices.size();
}

Vector<GPoint> GPolygon::getVertices() const {
    Vector<GPoint> vec;
    for (const QPointF& point : _vertices) {
        vec.add(GPoint(point.x(), point.y()));
    }
    return vec;
}

double GPolygon::getWidth() const {
    return getBounds().getHeight();
}

void GPolygon::setVertex(int i, GPoint point) {
    _vertices[i].setX(point.getX());
    _vertices[i].setY(point.getY());
    repaint();
}

std::string GPolygon::toStringExtra() const {
    std::ostringstream oss;
    oss << "vertices=" << _vertices.size();
    return oss.str();
}


GRect::GRect(double x, double y, double width, double height)
        : GObject(x, y, width, height) {
    // empty
}

void GRect::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawRect((int) getX(), (int) getY(), (int) getWidth(), (int) getHeight());
    painter->setRenderHint(QPainter::Antialiasing, GObject::isAntiAliasing());
}

std::string GRect::getType() const {
    return "GRect";
}


/*
 * Implementation notes: GRoundRect class
 * ---------------------------------------
 * Most of the GRoundRect class is inherited from the GRect class.
 */

GRoundRect::GRoundRect(double width, double height, double corner)
        : GRect(/* x */ 0, /* y */ 0, width, height),
          _corner(corner) {
    require::nonNegative(corner, "GRoundRect::constructor", "corner");
}

GRoundRect::GRoundRect(double x, double y, double width, double height, double corner)
        : GRect(x, y, width, height),
          _corner(corner) {
    require::nonNegative(corner, "GRoundRect::constructor", "corner");
}

bool GRoundRect::contains(double x, double y) const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_contains(this, x, y);
    }

    // JL BUGFIX: The rest of this is code to return correct result in non-transformed case
    // (accounting for corners)
    if (!getBounds().contains(x, y)) {
        return false;
    }

    // If corner diameter is too big, the largest sensible value is used by Java back end.
    double a = std::min(_corner, getWidth()) / 2;
    double b = std::min(_corner, getHeight()) / 2;

    // Get distances from nearest edges of bounding rectangle
    double dx = std::min(std::abs(x - getX()), std::abs(x - getRightX()));
    double dy = std::min(std::abs(y - getY()), std::abs(y - getBottomY()));

    if (dx > a || dy > b) {
        return true;   // in "central cross" of rounded rect
    }

    return (dx - a) * (dx - a) / (a * a) + (dy - b) * (dy - b) / (b * b) <= 1;
}

void GRoundRect::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawRoundRect((int) getX(), (int) getY(),
                           (int) getWidth(), (int) getHeight(),
                           (int) _corner, (int) _corner);
}

double GRoundRect::getCorner() const {
    return _corner;
}

std::string GRoundRect::getType() const {
    return "GRoundRect";
}

void GRoundRect::setCorner(double corner) {
    require::nonNegative(corner, "GRoundRect::setCorner", "corner");
    _corner = corner;
    repaint();
}

std::string GRoundRect::toStringExtra() const {
    return "corner=" + std::to_string(_corner);
}


GText::GText(const std::string& str, double x, double y)
        : GObject(x, y),
          _text(str) {
    _font = DEFAULT_FONT;
    updateSize();
}

void GText::draw(QPainter* painter) {
    if (!painter) {
        return;
    }
    initializeBrushAndPen(painter);
    painter->drawText((int) getX(), (int) getY(), QString::fromStdString(_text));
}

GRectangle GText::getBounds() const {
    if (isTransformed()) {
        // TODO
        // return stanfordcpplib::getPlatform()->gobject_getBounds(this);
    }
    return GRectangle(getX(), getY() - getFontAscent(), getWidth(), getHeight());
}

std::string GText::getFont() const {
    return _font;
}

double GText::getFontAscent() const {
    QFontMetrics metrics(GFont::toQFont(_font));
    return metrics.ascent();
}

double GText::getFontDescent() const {
    QFontMetrics metrics(GFont::toQFont(_font));
    return metrics.descent();
}

std::string GText::getLabel() const {
    return _text;
}

std::string GText::getText() const {
    return _text;
}

std::string GText::getType() const {
    return "GString";
}

void GText::setFont(const QFont& font) {
    setFont(GFont::toFontString(font));
}

void GText::setFont(const std::string& font) {
    _font = font;
    updateSize();
    repaint();
}

void GText::setLabel(const std::string& str) {
    _text = str;
    updateSize();
    repaint();
}

void GText::setText(const std::string& str) {
    setLabel(str);
}

std::string GText::toStringExtra() const {
    return "text=\"" + _text + "\"";
}

void GText::updateSize() {
    QFontMetrics metrics(GFont::toQFont(_font));
    _width = metrics.width(QString::fromStdString(_text));
    _height = metrics.height();
}

std::ostream& operator <<(std::ostream& out, const GObject& obj) {
    return out << obj.toString();
}

static double dsq(double x0, double y0, double x1, double y1) {
    return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

/*
 * File: gbutton.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gbutton.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gbutton.h"
#include <QKeySequence>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GButton::GButton(const std::string& text, const std::string& iconFileName, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    });
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GButton::~GButton() {
    // TODO: delete _button;
    _iqpushbutton = nullptr;
}

std::string GButton::getAccelerator() const {
    return _iqpushbutton->shortcut().toString().toStdString();
}

std::string GButton::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GButton::getInternalWidget() const {
    return _iqpushbutton;
}

std::string GButton::getText() const {
    return _iqpushbutton->text().toStdString();
}

GInteractor::TextPosition GButton::getTextPosition() const {
    switch (_iqpushbutton->toolButtonStyle()) {
    case Qt::ToolButtonTextBesideIcon:
        return GInteractor::TEXT_BESIDE_ICON;
    case Qt::ToolButtonTextUnderIcon:
        return GInteractor::TEXT_UNDER_ICON;
    case Qt::ToolButtonTextOnly:
    default:
        return GInteractor::TEXT_ONLY;
    }
}

std::string GButton::getType() const {
    return "GButton";
}

QWidget* GButton::getWidget() const {
    return static_cast<QWidget*>(_iqpushbutton);
}

void GButton::removeActionListener() {
    removeEventListener("click");
}

void GButton::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GButton::setAccelerator(const std::string& accelerator) {
    GThread::runOnQtGuiThread([this, accelerator]() {
        QKeySequence keySeq(QString::fromStdString(normalizeAccelerator(accelerator)));
        _iqpushbutton->setShortcut(keySeq);
    });
}

void GButton::setActionListener(GEventListener func) {
    setEventListener("click", func);
}

void GButton::setActionListener(GEventListenerVoid func) {
    setEventListener("click", func);
}

void GButton::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GButton::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GButton::setIcon(const std::string& filename, bool retainIconSize) {
    GInteractor::setIcon(filename, retainIconSize);
    if (!filename.empty() && fileExists(filename)) {
        GThread::runOnQtGuiThread([this, filename, retainIconSize]() {
            if (filename.empty()) {
                _iqpushbutton->setIcon(QIcon());
            } else {
                QPixmap pixmap(QString::fromStdString(filename));
                QIcon icon(pixmap);
                _iqpushbutton->setIcon(icon);
                _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                if (retainIconSize) {
                    _iqpushbutton->setIconSize(pixmap.size());
                    _iqpushbutton->updateGeometry();
                    _iqpushbutton->update();
                }
            }
        });
    }
}

void GButton::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqpushbutton->setText(QString::fromStdString(text));
    });
    setActionCommand(text);
}

void GButton::setTextPosition(GInteractor::TextPosition position) {
    GThread::runOnQtGuiThread([this, position]() {
        if (position == GInteractor::TEXT_UNDER_ICON) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        } else if (position == GInteractor::TEXT_BESIDE_ICON) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        } else if (position == GInteractor::TEXT_ONLY) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        }
    });
}

void GButton::setTextPosition(SwingConstants /*horizontal*/, SwingConstants /*vertical*/) {
    // TODO: not really supported
    setTextPosition(GInteractor::TEXT_UNDER_ICON);
}


_Internal_QPushButton::_Internal_QPushButton(GButton* button, QWidget* parent)
        : QToolButton(parent),
          _gbutton(button) {
    require::nonNull(button, "_Internal_QPushButton::constructor");
    setObjectName(QString::fromStdString("_Internal_QPushButton_" + std::to_string(button->getID())));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QPushButton::handleClick() {
    if (!_gbutton->isAcceptingEvent("click")) return;
    GEvent actionEvent(
                /* class  */ ACTION_EVENT,
                /* type   */ ACTION_PERFORMED,
                /* name   */ "click",
                /* source */ _gbutton);
    actionEvent.setActionCommand(_gbutton->getActionCommand());
    _gbutton->fireEvent(actionEvent);
}

void _Internal_QPushButton::mouseDoubleClickEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QPushButton::mouseDoubleClickEvent", "event");
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gbutton->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gbutton);
    mouseEvent.setActionCommand(_gbutton->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gbutton->fireEvent(mouseEvent);
}

QSize _Internal_QPushButton::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QToolButton::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gbutton.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gcolorchooser.cpp
 * -----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gfontchooser.cpp to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gfontchooser.h"
#include <QFontDialog>
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#undef INTERNAL_INCLUDE

GFontChooser::GFontChooser() {
    // empty
}

std::string GFontChooser::showDialog(const std::string& title, const std::string& initialFont) {
    return showDialog(/* parent */ (QWidget*) nullptr, title, initialFont);
}

std::string GFontChooser::showDialog(GWindow* parent, const std::string& title, const std::string& initialFont) {
    return showDialog(parent ? parent->getWidget() : nullptr, title, initialFont);
}

std::string GFontChooser::showDialog(QWidget* parent, const std::string& title, const std::string& initialFont) {
    QFont initialQFont = GFont::toQFont(initialFont);
    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, initialQFont, &result]() {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, initialQFont, parent, QString::fromStdString(title));
        if (ok) {
            result = GFont::toFontString(font);
        }
    });
    return result;
}

/*
 * File: gfilechooser.cpp
 * ----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gfilechooser.cpp to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gfilechooser.h"
#include <QFileDialog>
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

GFileChooser::GFileChooser() {
    // empty
}

std::string GFileChooser::showOpenDialog(const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showOpenDialog(/* parent */ (QWidget*) nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showOpenDialog(GWindow* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showOpenDialog(parent ? parent->getWidget() : nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showOpenDialog(QWidget* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    // Qt filter spec:
    // If you want multiple filters, separate them with ';;', for example:
    // "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"

    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, currentDir, fileFilter, &result]() {
        result = QFileDialog::getOpenFileName(parent,
                QString::fromStdString(title),
                QString::fromStdString(currentDir),
                QString::fromStdString(normalizeFileFilter(fileFilter))).toStdString();
    });
    return result;
}

std::string GFileChooser::showSaveDialog(const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showSaveDialog(/* parent */ (QWidget*) nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showSaveDialog(GWindow* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showSaveDialog(parent ? parent->getWidget() : nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showSaveDialog(QWidget* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, currentDir, fileFilter, &result]() {
        result = QFileDialog::getSaveFileName(parent,
                QString::fromStdString(title),
                QString::fromStdString(currentDir),
                QString::fromStdString(normalizeFileFilter(fileFilter))).toStdString();
    });
    return result;
}

std::string GFileChooser::normalizeFileFilter(const std::string& fileFilter) {
    Vector<std::string> tokens = stringSplit(fileFilter, ",");
    for (int i = 0; i < tokens.size(); i++) {
        std::string token = tokens[i];
        token = trim(token);
        // TODO: more processing
        tokens[i] = token;
    }
    return stringJoin(tokens, ";;");
}

/*
 * File: gdiffgui.cpp
 * ------------------
 * 
 * @author Marty Stepp
 * @version 2018/10/06
 * - allow passing diff flags
 * @version 2018/09/27
 * - bug fix for allocating on heap rather than stack
 * @version 2018/08/23
 * - renamed to gdiffgui.cpp to replace Java version
 * @version 2018/07/31
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gdiffgui.h"
#include <iostream>
#include <QScrollBar>
#include <string>
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#undef INTERNAL_INCLUDE

/*static*/ const std::string GDiffGui::COLOR_EXPECTED = "#009900";
/*static*/ const std::string GDiffGui::COLOR_LINE_NUMBERS = "#888888";
/*static*/ const std::string GDiffGui::COLOR_STUDENT  = "#bb0000";
/*static*/ const bool GDiffGui::LINE_NUMBERS = true;

void GDiffGui::showDialog(const std::string& name1,
                          const std::string& text1,
                          const std::string& name2,
                          const std::string& text2,
                          int diffFlags,
                          bool showCheckBoxes) {
    /* GDiffGui* diffGui = */ new GDiffGui(name1, text1, name2, text2, diffFlags, showCheckBoxes);
    // TODO: delete diffGui;
    // but don't do it too quickly or the lambda functions won't be able to
    // work on the window
}

GDiffGui::GDiffGui(const std::string& name1,
                   const std::string& text1,
                   const std::string& name2,
                   const std::string& text2,
                   int diffFlags,
                   bool /*showCheckBoxes*/) {
    GThread::runOnQtGuiThread([this, name1, text1, name2, text2, diffFlags]() {
        std::string diffs = diff::diff(text1, text2, diffFlags);

        _window = new GWindow(800, 600);
        _window->setTitle("Compare Output");
        _hsplitter = new QSplitter(/* orientation */ Qt::Horizontal, /* parent */ _window->getWidget());
        _window->setCloseOperation(GWindow::CLOSE_HIDE);

        // function to close the window when Escape is pressed
        auto windowCloseLambda = [this](GEvent event) {
            if (event.getType() == KEY_PRESSED && event.getKeyChar() == GEvent::ESCAPE_KEY) {
                _window->close();
            }
        };

        std::string font = getConsoleFont();
        _textAreaLeft  = new GTextArea();
        setupLeftRightText(_textAreaLeft, name1 + ":\n" + text1);
        _textAreaLeft->setEditable(false);
        _textAreaLeft->setFont(font);
        _textAreaLeft->setLineWrap(false);
        _textAreaLeft->setKeyListener(windowCloseLambda);
        _textAreaLeft->setEventListener("scroll", [this]() {
            syncScrollBars(/* left */ true);
        });

        _textAreaRight = new GTextArea();
        setupLeftRightText(_textAreaRight, name2 + ":\n" + text2);
        _textAreaRight->setEditable(false);
        _textAreaRight->setFont(font);
        _textAreaRight->setLineWrap(false);
        _textAreaRight->setKeyListener(windowCloseLambda);
        _textAreaRight->setEventListener("scroll", [this]() {
            syncScrollBars(/* left */ false);
        });

        _textAreaBottom = new GTextArea("Differences:\n");
        _textAreaBottom->setEditable(false);
        _textAreaBottom->setFont(font);
        _textAreaBottom->setLineWrap(false);
        _textAreaBottom->setKeyListener(windowCloseLambda);
        setupDiffText(diffs);
        _hsplitter->addWidget(_textAreaLeft->getWidget());
        _hsplitter->addWidget(_textAreaRight->getWidget());
        _hsplitter->setStretchFactor(0, 1);
        _hsplitter->setStretchFactor(1, 1);
        _hsplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));   // evenly size the two halves
        _hsplitterInteractor = new GGenericInteractor<QSplitter>(_hsplitter);

        _vsplitter = new QSplitter(/* orientation */ Qt::Vertical, /* parent */ _window->getWidget());
        _vsplitter->addWidget(_hsplitter);
        _vsplitter->addWidget(_textAreaBottom->getWidget());
        _vsplitter->setStretchFactor(0, 1);
        _vsplitter->setStretchFactor(1, 1);
        _vsplitter->setSizes(QList<int>({INT_MAX, INT_MAX}));   // evenly size the two halves
        _vsplitterInteractor = new GGenericInteractor<QSplitter>(_vsplitter);

        _window->addToRegion(_vsplitterInteractor, "Center");
        _window->setKeyListener(windowCloseLambda);
        _window->center();
        _window->show();
    });
}

GDiffGui::~GDiffGui() {
    // TODO: delete
    _window = nullptr;
    _hsplitter = nullptr;
    _vsplitter = nullptr;
    _textAreaLeft = nullptr;
    _textAreaRight = nullptr;
    _textAreaBottom = nullptr;
    _hsplitterInteractor = nullptr;
    _vsplitterInteractor = nullptr;
}

void GDiffGui::setupDiffText(const std::string& diffs) {
    Vector<std::string> lines = stringSplit(diffs, "\n");
    const std::string COLOR_NORMAL = _textAreaBottom->getColor();
    for (std::string line : lines) {
        std::string color;
        if (startsWith(line, "EXPECTED <")) {
            color = COLOR_EXPECTED;
        } else if (startsWith(line, "STUDENT  >")) {
            color = COLOR_STUDENT;
        } else {
            color = COLOR_NORMAL;
        }

        _textAreaBottom->appendFormattedText(line + "\n", color);
    }
}

void GDiffGui::setupLeftRightText(GTextArea* textArea, const std::string& text) {
    if (!LINE_NUMBERS) {
        textArea->setText(text);
        return;
    }

    Vector<std::string> lines = stringSplit(text, "\n");

    const std::string COLOR_NORMAL = textArea->getColor();
    for (int i = 0; i < lines.size(); i++) {
        std::string line = lines[i];

        // insert a gray line number at start of each line
        int digits = static_cast<int>(std::to_string(lines.size()).length());
        std::string lineNumberString =
                padLeft(i == 0 ? std::string("") : std::to_string(i), digits) + "  ";
        textArea->appendFormattedText(lineNumberString, COLOR_LINE_NUMBERS);
        textArea->appendFormattedText(line + "\n", COLOR_NORMAL);
    }
}

void GDiffGui::syncScrollBars(bool left) {
    QTextEdit* qleft  = static_cast<QTextEdit*>(_textAreaLeft->getWidget());
    QTextEdit* qright = static_cast<QTextEdit*>(_textAreaRight->getWidget());
    if (left) {
        // adjust the right scrollbar to match the left one
        GThread::runOnQtGuiThreadAsync([qleft, qright]() {
            int lval = qleft->verticalScrollBar()->value();
            int rmax = qright->verticalScrollBar()->maximum();
            int rval = std::max(0, std::min(lval, rmax));
            qright->verticalScrollBar()->setValue(rval);
        });
    } else {
        // adjust the left scrollbar to match the right one
        GThread::runOnQtGuiThreadAsync([qleft, qright]() {
            int rval = qright->verticalScrollBar()->value();
            int lmax = qleft->verticalScrollBar()->maximum();
            int lval = std::max(0, std::min(rval, lmax));
            qleft->verticalScrollBar()->setValue(lval);
        });
    }
}

/*
 * File: goptionpane.cpp
 * ---------------------
 *
 * This code is largely copied from goptionpane.cpp and modified to use
 * Qt's QMessageBox and QInputDialog classes.
 *
 * @author Marty Stepp
 * @version 2018/12/28
 * - bug fix for auto mnemonics/hotkeys in showOptionDialog
 * @version 2018/11/14
 * - added mnemonics/hotkey to showOptionDialog window option buttons
 * - added Cancel logic to Escape out of showOptionDialog window
 * @version 2018/10/18
 * - bug fix for showOptionDialog to run on Qt GUI thread
 * @version 2018/08/23
 * - renamed to goptionpane.cpp to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "goptionpane.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QWidget>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gbutton.h"
#define INTERNAL_INCLUDE 1
#include "gtextarea.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "set.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

GOptionPane::GOptionPane() {
    // empty
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(const std::string& message,
                                                          const std::string& title,
                                                          ConfirmType type) {
    return showConfirmDialog(/* parent */ static_cast<QWidget*>(nullptr), message, title, type);
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(GWindow* parent,
                                                          const std::string& message,
                                                          const std::string& title,
                                                          ConfirmType type) {
    return showConfirmDialog(parent ? parent->getWidget() : nullptr, message, title, type);
}

GOptionPane::ConfirmResult GOptionPane::showConfirmDialog(QWidget* parent,
                                                          const std::string& message,
                                                          const std::string& title,
                                                          ConfirmType type) {
    if (type != GOptionPane::ConfirmType::CONFIRM_YES_NO
            && type != GOptionPane::ConfirmType::CONFIRM_YES_NO_CANCEL
            && type != GOptionPane::ConfirmType::CONFIRM_OK_CANCEL) {
        error("GOptionPane::showConfirmDialog: Illegal dialog type");
    }
    std::string titleToUse = title.empty() ? std::string("Select an option") : title;

    // convert our enum types to Qt's button enum type
    QMessageBox::StandardButtons buttons;
    QMessageBox::StandardButton defaultButton = QMessageBox::Cancel;
    if (type == GOptionPane::ConfirmType::CONFIRM_YES_NO) {
        buttons = QMessageBox::Yes | QMessageBox::No;
        defaultButton = QMessageBox::No;
    } else if (type == GOptionPane::ConfirmType::CONFIRM_YES_NO_CANCEL) {
        buttons = QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel;
    } else if (type == GOptionPane::ConfirmType::CONFIRM_OK_CANCEL) {
        buttons = QMessageBox::Ok | QMessageBox::Cancel;
    }

    GOptionPane::ConfirmResult confirmResult = GOptionPane::CONFIRM_CANCEL;
    GThread::runOnQtGuiThread([parent, titleToUse, message, buttons, defaultButton, &confirmResult]() {
        int dialogResult = QMessageBox::question(parent,
                QString::fromStdString(titleToUse),
                QString::fromStdString(message),
                buttons,
                defaultButton);
        switch (dialogResult) {
            case QMessageBox::Yes:
                confirmResult = GOptionPane::ConfirmResult::CONFIRM_YES;
                break;
            case QMessageBox::No:
                confirmResult = GOptionPane::ConfirmResult::CONFIRM_NO;
                break;
            case QMessageBox::Cancel:
            default:
                confirmResult = GOptionPane::ConfirmResult::CONFIRM_CANCEL;
                break;
        }
    });
    return confirmResult;
}

std::string GOptionPane::showInputDialog(const std::string& message,
                                         const std::string& title,
                                         const std::string& initialValue) {
    return showInputDialog(/* parent */ static_cast<QWidget*>(nullptr), message, title, initialValue);
}

std::string GOptionPane::showInputDialog(GWindow* parent,
                                         const std::string& message,
                                         const std::string& title,
                                         const std::string& initialValue) {
    return showInputDialog(parent ? parent->getWidget() : nullptr, message, title, initialValue);
}

std::string GOptionPane::showInputDialog(QWidget* parent,
                                         const std::string& message,
                                         const std::string& title,
                                         const std::string& initialValue) {
    std::string titleToUse = title.empty() ? std::string("Type a value") : title;
    std::string result = "";
    GThread::runOnQtGuiThread([parent, titleToUse, message, initialValue, &result]() {
        result = QInputDialog::getText(parent,
                QString::fromStdString(titleToUse),
                QString::fromStdString(message),
                QLineEdit::Normal,
                QString::fromStdString(initialValue)).toStdString();
    });
    return result;
}

void GOptionPane::showMessageDialog(const std::string& message,
                                    const std::string& title,
                                    MessageType type) {
    showMessageDialog(/* parent */ static_cast<QWidget*>(nullptr), message, title, type);
}

void GOptionPane::showMessageDialog(GWindow* parent,
                                    const std::string& message,
                                    const std::string& title,
                                    MessageType type) {
    showMessageDialog(parent ? parent->getWidget() : nullptr, message, title, type);
}

void GOptionPane::showMessageDialog(QWidget* parent,
                                    const std::string& message,
                                    const std::string& title,
                                    MessageType type) {
    if (type != GOptionPane::MessageType::MESSAGE_PLAIN
            && type != GOptionPane::MessageType::MESSAGE_INFORMATION
            && type != GOptionPane::MessageType::MESSAGE_ERROR
            && type != GOptionPane::MessageType::MESSAGE_WARNING
            && type != GOptionPane::MessageType::MESSAGE_QUESTION
            && type != GOptionPane::MessageType::MESSAGE_ABOUT) {
        error("GOptionPane::showMessageDialog: Illegal dialog type");
    }
    std::string titleToUse = title.empty() ? std::string("Message") : title;

    GThread::runOnQtGuiThread([parent, message, titleToUse, type]() {
        if (type == GOptionPane::MessageType::MESSAGE_PLAIN
                || type == GOptionPane::MessageType::MESSAGE_INFORMATION
                || type == GOptionPane::MessageType::MESSAGE_QUESTION) {
            QMessageBox::information(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        } else if (type == GOptionPane::MessageType::MESSAGE_WARNING) {
            QMessageBox::warning(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        } else if (type == GOptionPane::MessageType::MESSAGE_ERROR) {
            QMessageBox::critical(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        } else if (type == GOptionPane::MessageType::MESSAGE_ABOUT) {
            QMessageBox::about(parent, QString::fromStdString(titleToUse), QString::fromStdString(message));
        }
    });
}

std::string GOptionPane::showOptionDialog(const std::string& message,
                                          const Vector<std::string>& options,
                                          const std::string& title,
                                          const std::string& initiallySelected) {
    return showOptionDialog(/* parent */ static_cast<QWidget*>(nullptr), message, options, title, initiallySelected);
}

std::string GOptionPane::showOptionDialog(GWindow* parent,
                                          const std::string& message,
                                          const Vector<std::string>& options,
                                          const std::string& title,
                                          const std::string& initiallySelected) {
    return showOptionDialog(parent ? parent->getWidget() : nullptr, message, options, title, initiallySelected);
}

std::string GOptionPane::showOptionDialog(QWidget* parent,
                                          const std::string& message,
                                          const Vector<std::string>& options,
                                          const std::string& title,
                                          const std::string& initiallySelected) {
    std::string titleToUse = title.empty() ? std::string("Select an option") : title;
    std::string result = "";
    GThread::runOnQtGuiThread([parent, message, &options, titleToUse, initiallySelected, &result]() {
        QMessageBox box;
        if (parent) {
            box.setParent(parent);
        }
        box.setText(QString::fromStdString(message));
        box.setWindowTitle(QString::fromStdString(titleToUse));
        box.setAttribute(Qt::WA_QuitOnClose, false);

        for (std::string option : options) {
            box.addButton(QString::fromStdString(option), QMessageBox::ActionRole);
        }

        if (!initiallySelected.empty()) {
            // TODO: dunno how to set initially selected button properly
            // box.setDefaultButton(QString::fromStdString(initiallySelected));
        }

        // give each button a unique hotkey; listen to key presses on buttons
        // (try to set char at index 0, 1, 2 as the mnemonic)
        Set<QAbstractButton*> buttonsUsed;
        Set<std::string> charsUsed;
        QAbstractButton* escapeButton = nullptr;
        int escapeButtonIndex = -1;

        for (int i = 0; i <= 2; i++) {
            int buttonIndex = 0;
            for (QAbstractButton* button : box.buttons()) {
                if (buttonsUsed.contains(button)) {
                    buttonIndex++;
                    continue;
                }

                std::string text = button->text().toStdString();
                if (!escapeButton && text == "Cancel") {
                    escapeButton = button;
                    escapeButtonIndex = buttonIndex;
                }
                if (static_cast<int>(text.length()) <= i) {
                    buttonIndex++;
                    continue;
                }
                std::string letter = text.substr(i, 1);
                if (charsUsed.contains(letter)) {
                    buttonIndex++;
                    continue;
                }

                buttonsUsed.add(button);
                charsUsed.add(letter);
                button->setText(QString::fromStdString(text.substr(0, i) + "&" + text.substr(i)));
                button->setShortcut(QKeySequence::fromString(QString::fromStdString(letter)));
                buttonIndex++;
            }
        }

        // set listener to close window when Esc is pressed
        if (escapeButton) {
            box.setEscapeButton(escapeButton);
        }

        int index = box.exec();
        if (index == GOptionPane::InternalResult::INTERNAL_CLOSED_OPTION
                || index < 0 || index >= options.size()
                || (escapeButtonIndex >= 0 && index == escapeButtonIndex)) {
            result = "";
        } else {
            result = options[index];
        }
    });
    return result;
}

void GOptionPane::showTextFileDialog(const std::string& fileText,
                                     const std::string& title,
                                     int rows,
                                     int cols) {
    showTextFileDialog(static_cast<QWidget*>(nullptr), fileText, title, rows, cols);
}

void GOptionPane::showTextFileDialog(GWindow* parent,
                                     const std::string& fileText,
                                     const std::string& title,
                                     int rows,
                                     int cols) {
    showTextFileDialog(parent ? parent->getWidget() : nullptr, fileText, title, rows, cols);
}

void GOptionPane::showTextFileDialog(QWidget* /*parent*/,
                                     const std::string& fileText,
                                     const std::string& title,
                                     int rows,
                                     int cols) {
    static const std::string DEFAULT_FONT = "Monospaced-*-*";
    static const int DEFAULT_ROWS    = 20;
    static const int DEFAULT_COLUMNS = 80;
    if (rows <= 0) {
        rows = DEFAULT_ROWS;
    }
    if (cols <= 0) {
        cols = DEFAULT_COLUMNS;
    }

    std::string titleToUse = title.empty() ? std::string("Text file contents") : title;
    GWindow* window = new GWindow;
    window->setTitle(title);

    GTextArea* textArea = new GTextArea(fileText);
    textArea->setFont(DEFAULT_FONT);
    textArea->setRowsColumns(DEFAULT_ROWS, DEFAULT_COLUMNS);
    window->addToRegion(textArea, GWindow::REGION_CENTER);

    GButton* button = new GButton("&OK");
    button->setActionListener([window]() {
        window->close();
    });
    window->addToRegion(button, GWindow::REGION_SOUTH);

    window->pack();
    window->center();
    window->show();
}

/*
 * File: qtgui.cpp
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to qtgui.cpp
 * @version 2018/07/03
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#include <QEvent>
#include <QtGlobal>
#include <QThread>
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32

// QSPLApplication members
QSPLApplication::QSPLApplication(int& argc, char *argv[])
        : QApplication(argc, argv) {
    // empty
}

bool QSPLApplication::notify(QObject* receiver, QEvent* e) {
#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    try {
        return QApplication::notify(receiver, e);
    } catch (ErrorException& ex) {
        ex.dump();
        return false;
    }
#else
    return QApplication::notify(receiver, e);   // call super
#endif
}


// QtGui members
QSPLApplication* QtGui::_app = nullptr;
QtGui* QtGui::_instance = nullptr;

QtGui::QtGui()
        : _initialized(false) {
    connect(GEventQueue::instance(), SIGNAL(eventReady()), this, SLOT(processEventFromQueue()));
}

void QtGui::exitGraphics(int exitCode) {
    if (_app) {
// need to temporarily turn off C++ lib exit macro to call QApplication's exit method
// (NOTE: must keep in sync with exit definition in init.h)
#undef exit
        _app->quit();
        _app = nullptr;
        std::exit(exitCode);
#define exit __stanfordcpplib__exitLibrary
    } else {
        std::exit(exitCode);
    }
}

QSPLApplication* QtGui::getApplication() {
    return _app;
}

int QtGui::getArgc() const {
    return _argc;
}

char** QtGui::getArgv() const {
    return _argv;
}

void QtGui::initializeQt() {
    if (_app) return;

    GThread::runOnQtGuiThread([this]() {
        if (!_app) {
            qSetMessagePattern(
                    "Qt internal warning: %{message}\n"
                    "  - pid: %{pid}\n"
                    "  - thread: %{threadid}\n"

                    // backtrace doesn't work on windows and some other builds
#ifndef _WIN32
                    "  - stack:\n"
                    "      %{backtrace depth=20 separator=\"\n      \"}"
#endif // _WIN32
            );
            _app = new QSPLApplication(_argc, _argv);
            _initialized = true;
        }
    });
}

QtGui* QtGui::instance() {
    if (!_instance) {
        _instance = new QtGui();
        GEventQueue::instance();   // create event queue on Qt GUI main thread
    }
    return _instance;
}

void QtGui::processEventFromQueue() {
    if (!GEventQueue::instance()->isEmpty()) {
        GThunk thunk = GEventQueue::instance()->peek();
        thunk();
        GEventQueue::instance()->dequeue();
    }
}

void QtGui::setArgs(int argc, char** argv) {
    _argc = argc;
    _argv = argv;
}

// this should be called by the Qt main thread
void QtGui::startBackgroundEventLoop(GThunkInt mainFunc, bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!GThread::studentThreadExists()) {
        GStudentThread::startStudentThread(mainFunc);
        startEventLoop(exitAfter);   // begin Qt event loop on main thread
    }
}

// this should be called by the Qt main thread
void QtGui::startBackgroundEventLoopVoid(GThunk mainFunc, bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!GThread::studentThreadExists()) {
        GStudentThread::startStudentThreadVoid(mainFunc);
        startEventLoop(exitAfter);   // begin Qt event loop on main thread
    }
}

// this should be called by the Qt main thread
void QtGui::startEventLoop(bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startEventLoop");
    if (!_app) {
        error("QtGui::startEventLoop: need to initialize Qt first");
    }

    // start Qt event loop on main thread;
    // Qt GUI main thread blocks here until student main() is done
    int exitCode = _app->exec();

    // if I get here, it means an "exit on close" window was just closed;
    // it's time to shut down the Qt system and exit the C++ program
    if (exitAfter) {
        exitGraphics(exitCode);
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_qtgui.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: console.cpp
 * -----------------
 * This file implements the console .h interface.
 *
 * @author Marty Stepp
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/10/01
 * - bug fix for graphical console popping up even if not included
 * @version 2018/08/23
 * - renamed to console .cpp/h to replace Java version
 * - separated out gconsolewindow.h/cpp
 * @version 2018/07/15
 * - initial version, based on io/console .cpp
 */

#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#include <cstdio>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gconsolewindow.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#define INTERNAL_INCLUDE 1
#include "private/version.h"
#undef INTERNAL_INCLUDE

#ifdef SPL_HEADLESS_MODE

void clearConsole() {
    // empty
}

bool getConsoleClearEnabled() {
    return true;
}

/* GWindow::CloseOperation */ int getConsoleCloseOperation() {
    return 0;
}

bool getConsoleEcho() {
    return true;
}

bool getConsoleEnabled() {
    return true;
}

bool getConsoleEventOnClose() {
    return true;
}

bool getConsoleExitProgramOnClose() {
    return true;
}

std::string getConsoleFont() {
    return "";
}

double getConsoleHeight() {
    return 0;
}

bool getConsoleLocationSaved() {
    return false;
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return false;
}

double getConsoleWidth() {
    return 0;
}

std::string getConsoleWindowTitle() {
    return "";
}

void pause(double /*milliseconds*/) {
    // empty
}

void setConsoleClearEnabled(bool /*value*/) {
    // empty
}

void setConsoleCloseOperation(int /*op*/) {
    // empty
}

void setConsoleEcho(bool /*echo*/) {
    // empty
}

void setConsoleEnabled(bool /*enabled*/) {
    // empty
}

void setConsoleErrorColor(const std::string& /*color*/) {
    // empty
}

void setConsoleEventOnClose(bool /*eventOnClose*/) {
    // empty
}

void setConsoleExitProgramOnClose(bool /*exitOnClose*/) {
    // empty
}

void setConsoleFont(const std::string& /*font*/) {
    // empty
}

void setConsoleLocation(double /*x*/, double /*y*/) {
    // empty
}

void setConsoleLocationSaved(bool /*value*/) {
    // empty
}

void setConsoleOutputColor(const std::string& /*color*/) {
    // empty
}

void setConsolePrintExceptions(bool printExceptions, bool force) {
    if (getConsoleSettingsLocked()) { return; }
    exceptions::setTopLevelExceptionHandlerEnabled(printExceptions, force);
}

void setConsoleSettingsLocked(bool /*value*/) {
    // empty
}

void setConsoleSize(double /*width*/, double /*height*/) {
    // empty
}

void setConsoleWindowTitle(const std::string& /*title*/) {
    // empty
}

void shutdownConsole() {
    // empty
}

#else // SPL_HEADLESS_MODE

void clearConsole() {
    GConsoleWindow::instance()->clearConsole();
}

bool getConsoleClearEnabled() {
    return GConsoleWindow::instance()->isClearEnabled();
}

/* GWindow::CloseOperation */ int getConsoleCloseOperation() {
    return GConsoleWindow::instance()->getCloseOperation();
}

bool getConsoleEcho() {
    return GConsoleWindow::instance()->isEcho();
}

bool getConsoleEnabled() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return false;
#else
    return GConsoleWindow::consoleEnabled();
#endif
}

bool getConsoleEventOnClose() {
    return true;
}

bool getConsoleExitProgramOnClose() {
    return GConsoleWindow::instance()->getCloseOperation() == GWindow::CLOSE_EXIT;
}

std::string getConsoleFont() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return GConsoleWindow::getDefaultFont();
#else
    return GConsoleWindow::instance()->getFont();
#endif
}

double getConsoleHeight() {
    return GConsoleWindow::instance()->getHeight();
}

GPoint getConsoleLocation() {
    return GConsoleWindow::instance()->getLocation();
}

bool getConsoleLocationSaved() {
    return GConsoleWindow::instance()->isLocationSaved();
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return GConsoleWindow::isInitialized()
            && GConsoleWindow::instance()->isLocked();
}

GDimension getConsoleSize() {
    return GConsoleWindow::instance()->getSize();
}

double getConsoleWidth() {
    return GConsoleWindow::instance()->getWidth();
}

GConsoleWindow* getConsoleWindow() {
    return GConsoleWindow::instance();
}

std::string getConsoleWindowTitle() {
    return GConsoleWindow::instance()->getTitle();
}

void setConsoleClearEnabled(bool value) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setClearEnabled(value);
}

void setConsoleCloseOperation(/*GWindow::CloseOperation*/ int op) {
    GWindow::CloseOperation gwcop = static_cast<GWindow::CloseOperation>(op);
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setCloseOperation(gwcop);
}

void setConsoleEcho(bool echo) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setEcho(echo);
}

void setConsoleEnabled(bool enabled) {
    GConsoleWindow::setConsoleEnabled(enabled);
}

void setConsoleErrorColor(const std::string& color) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setErrorColor(color);
}

void setConsoleEventOnClose(bool /*eventOnClose*/) {
    // empty
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setExitOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setFont(font);
}

void setConsoleLocation(double x, double y) {
    if (getConsoleSettingsLocked()) { return; }
    if (floatingPointEqual(x, -1) && floatingPointEqual(y, -1)) {
        GConsoleWindow::instance()->center();
    } else {
        GConsoleWindow::instance()->setLocation(x, y);
    }
}

void setConsoleLocationSaved(bool value) {
    GConsoleWindow::instance()->setLocationSaved(value);
}

void setConsoleOutputColor(const std::string& color) {
    GConsoleWindow::instance()->setOutputColor(color);
}

void setConsolePrintExceptions(bool printExceptions, bool force) {
    if (getConsoleSettingsLocked()) { return; }
    exceptions::setTopLevelExceptionHandlerEnabled(printExceptions, force);
}

void setConsoleSettingsLocked(bool value) {
    GConsoleWindow::instance()->setLocked(value);
}

void setConsoleSize(double width, double height) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setConsoleSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setTitle(title);
}

void shutdownConsole() {
    if (getConsoleEnabled() && !GConsoleWindow::instance()->isLocked()) {
        GConsoleWindow::instance()->shutdown();
    }
}

#endif // SPL_HEADLESS_MODE

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * based on compiler options set in the .pro file.
 */
void setConsolePropertiesQt() {
#if defined(SPL_CONSOLE_FONTSIZE)
    std::string fontStr = std::string("Monospaced-Bold-") + to_string(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

#if defined(SPL_CONSOLE_WIDTH) && defined(SPL_CONSOLE_HEIGHT)
    setConsoleSize(SPL_CONSOLE_WIDTH, SPL_CONSOLE_HEIGHT);
#endif

#if defined(SPL_CONSOLE_X) && defined(SPL_CONSOLE_Y)
    setConsoleLocation(SPL_CONSOLE_X, SPL_CONSOLE_Y);
#endif

#if defined(SPL_CONSOLE_ECHO)
    setConsoleEcho(true);
#endif

#if defined(SPL_CONSOLE_EXIT_ON_CLOSE)
    setConsoleExitProgramOnClose(true);
#endif

#if defined(SPL_CONSOLE_LOCATION_SAVED)
    setConsoleLocationSaved(true);
#endif

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true);
#endif

#if defined(SPL_VERIFY_PROJECT_VERSION)
    version::ensureProjectVersion();
#endif
}

void initializeQtGraphicalConsole() {
#ifndef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    // ensure that console is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    static std::ios_base::Init ios_base_init;

#ifndef SPL_HEADLESS_MODE
    if (GConsoleWindow::consoleEnabled()) {
        GConsoleWindow::instance();   // ensure that console window is ready
        setConsolePropertiesQt();
    }
#endif // SPL_HEADLESS_MODE

#endif // __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
}

/*
 * File: gdrawingsurface.cpp
 * -------------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gdrawingsurface.cpp to replace Java version
 * @version 2018/07/11
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gdrawingsurface.h"
#include <QPainter>
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GDrawingSurface::GDrawingSurface()
        : _forwardTarget(nullptr),
          _backgroundColor("0xffffffff"),
          _color(""),
          _fillColor(""),
          _font(""),
          _backgroundColorInt(0xffffffff),
          _colorInt(0),
          _fillColorInt(0),
          _lineStyle(GObject::LINE_SOLID),
          _lineWidth(1),
          _autoRepaint(true) {
    // empty
}

GDrawingSurface::~GDrawingSurface() {
    _forwardTarget = nullptr;
}

void GDrawingSurface::checkBounds(const std::string& member, double x, double y, double width, double height) const {
    require::inRange2D(x, y, width - 1, height - 1, member);
}

void GDrawingSurface::checkColor(const std::string& /* member */, int /* rgb */) const {
    // I think this code is wrong; it ignores the possibility of alpha values
    // or of the top bits being set to 255 (all 1) by default by Qt libraries
//    if (rgb < 0x0 || rgb > 0xffffff) {
//        error(member + ": color is outside of range 0x000000 through 0xffffff");
//    }
}

void GDrawingSurface::checkSize(const std::string& /* member */, double /* width */, double /* height */) const {
//    if (width < 0 || height < 0) {
//        error(member + ": width/height cannot be negative");
//    }
//    if (width > GCanvas::WIDTH_HEIGHT_MAX
//            || height > GCanvas::WIDTH_HEIGHT_MAX) {
//        error(getType() + "::" + member + ": width/height too large (cannot exceed "
//              + std::to_string(GCanvas::WIDTH_HEIGHT_MAX));
//    }
}

void GDrawingSurface::conditionalRepaint() {
    if (_forwardTarget) {
        _forwardTarget->conditionalRepaint();
    } else {
        if (isAutoRepaint()) {
            repaint();
        }
    }
}

void GDrawingSurface::conditionalRepaintRegion(int x, int y, int width, int height) {
    if (isAutoRepaint()) {
        repaintRegion(x, y, width, height);
    }
}

void GDrawingSurface::conditionalRepaintRegion(const GRectangle& bounds) {
    if (isAutoRepaint()) {
        repaintRegion(bounds);
    }
}

void GDrawingSurface::draw(GObject* gobj, double x, double y) {
    require::nonNull(gobj, "GDrawingSurface::draw");
    gobj->setLocation(x, y);
    draw(gobj);
}

void GDrawingSurface::draw(GObject& gobj) {
    draw(&gobj);
}

void GDrawingSurface::draw(GObject& gobj, double x, double y) {
    gobj.setLocation(x, y);
    draw(&gobj);
}

void GDrawingSurface::drawArc(double x, double y, double width, double height, double start, double sweep) {
    GArc arc(x, y, width, height, start, sweep);
    initializeGObject(arc);
    draw(arc);
}

void GDrawingSurface::drawImage(const std::string& filename, double x, double y) {
    GImage image(filename, x, y);
    draw(image);
}

void GDrawingSurface::drawLine(const GPoint& p0, const GPoint& p1) {
    drawLine(p0.getX(), p0.getY(), p1.getX(), p1.getY());
}

void GDrawingSurface::drawLine(double x0, double y0, double x1, double y1) {
    GLine line(x0, y0, x1, y1);
    initializeGObject(line);
    draw(line);
}

void GDrawingSurface::drawOval(const GRectangle& bounds) {
    drawOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void GDrawingSurface::drawOval(double x, double y, double width, double height) {
    GOval oval(x, y, width, height);
    initializeGObject(oval);
    draw(oval);
}

GPoint GDrawingSurface::drawPolarLine(const GPoint& p0, double r, double theta) {
    return drawPolarLine(p0.getX(), p0.getY(), r, theta);
}

GPoint GDrawingSurface::drawPolarLine(double x0, double y0, double r, double theta) {
    double x1 = x0 + r * cosDegrees(theta);
    double y1 = y0 - r * sinDegrees(theta);
    drawLine(x0, y0, x1, y1);
    return GPoint(x1, y1);
}

void GDrawingSurface::drawPixel(double x, double y) {
    setPixel(x, y, _colorInt);
}

void GDrawingSurface::drawPixel(double x, double y, int color) {
    setPixel(x, y, color);
}

void GDrawingSurface::drawPixel(double x, double y, const std::string& color) {
    setPixel(x, y, color);
}

void GDrawingSurface::drawPolygon(std::initializer_list<double> coords) {
    GPolygon polygon(coords);
    initializeGObject(polygon);
    draw(polygon);
}

void GDrawingSurface::drawPolygon(std::initializer_list<GPoint> points) {
    GPolygon polygon(points);
    initializeGObject(polygon);
    draw(polygon);
}

void GDrawingSurface::drawRect(const GRectangle& bounds) {
    drawRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void GDrawingSurface::drawRect(double x, double y, double width, double height) {
    GRect rect(x, y, width, height);
    initializeGObject(rect);
    draw(rect);
}

void GDrawingSurface::drawString(const std::string& text, double x, double y) {
    GText str(text, x, y);
    initializeGObject(str);
    draw(str);
}

void GDrawingSurface::fillArc(double x, double y, double width, double height, double start, double sweep) {
    GArc arc(x, y, width, height, start, sweep);
    initializeGObject(arc, /* filled */ true);
    draw(arc);
}

void GDrawingSurface::fillOval(const GRectangle& bounds) {
    fillOval(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void GDrawingSurface::fillOval(double x, double y, double width, double height) {
    GOval oval(x, y, width, height);
    initializeGObject(oval, /* filled */ true);
    draw(oval);
}

void GDrawingSurface::fillPolygon(std::initializer_list<double> coords) {
    GPolygon polygon(coords);
    initializeGObject(polygon, /* filled */ true);
    draw(polygon);
}

void GDrawingSurface::fillRect(const GRectangle& bounds) {
    fillRect(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
}

void GDrawingSurface::fillRect(double x, double y, double width, double height) {
    GRect rect(x, y, width, height);
    initializeGObject(rect, /* filled */ true);
    draw(rect);
}

int GDrawingSurface::getARGB(double x, double y) const {
    return getPixelARGB(x, y);
}

std::string GDrawingSurface::getBackground() const {
    if (_forwardTarget) {
        return _forwardTarget->getBackground();
    } else {
        return _backgroundColor;
    }
}

int GDrawingSurface::getBackgroundInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getBackgroundInt();
    } else {
        return _backgroundColorInt;
    }
}

std::string GDrawingSurface::getColor() const {
    if (_forwardTarget) {
        return _forwardTarget->getColor();
    } else {
        return _color;
    }
}

int GDrawingSurface::getColorInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getColorInt();
    } else {
        return _colorInt;
    }
}

std::string GDrawingSurface::getFillColor() const {
    if (_forwardTarget) {
        return _forwardTarget->getFillColor();
    } else {
        return _fillColor;
    }
}

int GDrawingSurface::getFillColorInt() const {
    if (_forwardTarget) {
        return _forwardTarget->getFillColorInt();
    } else {
        return _fillColorInt;
    }
}

std::string GDrawingSurface::getFont() const {
    if (_forwardTarget) {
        return _forwardTarget->getFont();
    } else {
        return _font;
    }
}

std::string GDrawingSurface::getForeground() const {
    return getColor();
}

int GDrawingSurface::getForegroundInt() const {
    return getColorInt();
}

GObject::LineStyle GDrawingSurface::getLineStyle() const {
    if (_forwardTarget) {
        return _forwardTarget->getLineStyle();
    } else {
        return _lineStyle;
    }
}

double GDrawingSurface::getLineWidth() const {
    if (_forwardTarget) {
        return _forwardTarget->getLineWidth();
    } else {
        return _lineWidth;
    }
}

std::string GDrawingSurface::getPixelString(double x, double y) const {
    return GColor::convertRGBToColor(getPixel(x, y));
}

int GDrawingSurface::getRGB(double x, double y) const {
    return getPixel(x, y);
}

std::string GDrawingSurface::getRGBString(double x, double y) const {
    return GColor::convertRGBToColor(getPixel(x, y));
}

void GDrawingSurface::initializeGObject(GObject& obj, bool fill) {
    initializeGObject(&obj, fill);
}

void GDrawingSurface::initializeGObject(GObject* obj, bool fill) {
    if (!obj) {
        return;
    }
    obj->setColor(getColor());
    if (fill) {
        obj->setFilled(true);
        obj->setFillColor(getFillColor());
    }
    obj->setFont(getFont());
    obj->setLineStyle(getLineStyle());
    obj->setLineWidth(getLineWidth());
}

bool GDrawingSurface::isAutoRepaint() const {
    if (_forwardTarget) {
        return _forwardTarget->isAutoRepaint();
    } else {
        return _autoRepaint;
    }
}

bool GDrawingSurface::isRepaintImmediately() const {
    return isAutoRepaint();
}

void GDrawingSurface::repaintRegion(const GRectangle& bounds) {
    repaintRegion((int) bounds.getX(), (int) bounds.getY(),
                  (int) bounds.getWidth(), (int) bounds.getHeight());
}

void GDrawingSurface::setAutoRepaint(bool autoRepaint) {
    if (_forwardTarget) {
        _forwardTarget->setAutoRepaint(autoRepaint);
    } else {
        _autoRepaint = autoRepaint;
    }
}

void GDrawingSurface::setBackground(int color) {
    if (_forwardTarget) {
        _forwardTarget->setBackground(color);
    } else {
        _backgroundColorInt = color;
        _backgroundColor = GColor::convertRGBToColor(color);
    }
}

void GDrawingSurface::setBackground(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setBackground(color);
    } else {
        _backgroundColor = color;
        _backgroundColorInt = GColor::convertColorToRGB(color);
    }
}

void GDrawingSurface::setColor(int color) {
    // sets both color and fillColor
    if (_forwardTarget) {
        _forwardTarget->setColor(color);
        _forwardTarget->setFillColor(color);
    } else {
        _colorInt = color;
        _color = GColor::convertRGBToColor(color);
        _fillColorInt = color;
        _fillColor = _color;
    }
}

void GDrawingSurface::setColor(const std::string& color) {
    // sets both color and fillColor
    if (_forwardTarget) {
        _forwardTarget->setColor(color);
        _forwardTarget->setFillColor(color);
    } else {
        _color = color;
        _colorInt = GColor::convertColorToRGB(color);
        _fillColor = color;
        _fillColorInt = _colorInt;
    }
}

void GDrawingSurface::setDrawingForwardTarget(GDrawingSurface* forwardTarget) {
    _forwardTarget = forwardTarget;
}

void GDrawingSurface::setFillColor(int color) {
    if (_forwardTarget) {
        _forwardTarget->setFillColor(color);
    } else {
        _fillColorInt = color;
        _fillColor = GColor::convertRGBToColor(color);
    }
}

void GDrawingSurface::setFillColor(const std::string& color) {
    if (_forwardTarget) {
        _forwardTarget->setFillColor(color);
    } else {
        _fillColor = color;
        _fillColorInt = GColor::convertColorToRGB(color);
    }
}

void GDrawingSurface::setFont(const QFont& font) {
    if (_forwardTarget) {
        _forwardTarget->setFont(font);
    } else {
        _font = GFont::toFontString(font);
    }
}

void GDrawingSurface::setFont(const std::string& font) {
    if (_forwardTarget) {
        _forwardTarget->setFont(font);
    } else {
        _font = font;
    }
}

void GDrawingSurface::setForeground(int color) {
    setColor(color);
}

void GDrawingSurface::setForeground(const std::string& color) {
    setColor(color);
}

void GDrawingSurface::setLineStyle(GObject::LineStyle lineStyle) {
    if (_forwardTarget) {
        _forwardTarget->setLineStyle(lineStyle);
    } else {
        _lineStyle = lineStyle;
    }
}

void GDrawingSurface::setLineWidth(double lineWidth) {
    if (_forwardTarget) {
        _forwardTarget->setLineWidth(lineWidth);
    } else {
        _lineWidth = lineWidth;
    }
}

void GDrawingSurface::setPixel(double x, double y, int r, int g, int b) {
    setPixel(x, y, GColor::convertRGBToRGB(r, g, b));
}

void GDrawingSurface::setPixel(double x, double y, const std::string& color) {
    setPixel(x, y, GColor::convertColorToRGB(color));
}

void GDrawingSurface::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    setPixelARGB(x, y, GColor::convertARGBToARGB(a, r, g, b));
}

void GDrawingSurface::setRepaintImmediately(bool autoRepaint) {
    setAutoRepaint(autoRepaint);
}

void GDrawingSurface::setRGB(double x, double y, int rgb) {
    setPixel(x, y, rgb);
}

void GDrawingSurface::setRGB(double x, double y, int r, int g, int b) {
    this->setPixel(x, y, r, g, b);
}

void GDrawingSurface::setRGB(double x, double y, const std::string& color) {
    setPixel(x, y, GColor::convertColorToRGB(color));
}


void GForwardDrawingSurface::clear() {
    if (_forwardTarget) {
        _forwardTarget->clear();
    }
}

void GForwardDrawingSurface::draw(GObject* obj) {
    ensureForwardTarget();
    _forwardTarget->draw(obj);
}

void GForwardDrawingSurface::draw(GObject* obj, double x, double y) {
    ensureForwardTarget();
    _forwardTarget->draw(obj, x, y);
}

void GForwardDrawingSurface::draw(GObject& obj) {
    ensureForwardTarget();
    _forwardTarget->draw(obj);
}

void GForwardDrawingSurface::draw(GObject& obj, double x, double y) {
    ensureForwardTarget();
    _forwardTarget->draw(obj, x, y);
}

void GForwardDrawingSurface::draw(QPainter* painter) {
    ensureForwardTarget();
    _forwardTarget->draw(painter);
}

void GForwardDrawingSurface::ensureForwardTargetConstHack() const {
    if (!_forwardTarget) {
        // Your whole life has been a lie.
        // Also, this code is bad and I should feel bad.
        GForwardDrawingSurface* hack = (GForwardDrawingSurface*) this;
        hack->ensureForwardTarget();
    }
}

int GForwardDrawingSurface::getPixel(double x, double y) const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixel(x, y);
}

int GForwardDrawingSurface::getPixelARGB(double x, double y) const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixelARGB(x, y);
}

Grid<int> GForwardDrawingSurface::getPixels() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixels();
}

Grid<int> GForwardDrawingSurface::getPixelsARGB() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->getPixelsARGB();
}

bool GForwardDrawingSurface::isAutoRepaint() const {
    ensureForwardTargetConstHack();
    return _forwardTarget->isAutoRepaint();
}

void GForwardDrawingSurface::repaint() {
    if (_forwardTarget) {
        _forwardTarget->repaint();
    }
}

void GForwardDrawingSurface::repaintRegion(int x, int y, int width, int height) {
    if (_forwardTarget) {
        _forwardTarget->repaintRegion(x, y, width, height);
    }
}

void GForwardDrawingSurface::setAutoRepaint(bool autoRepaint) {
    ensureForwardTarget();
    _forwardTarget->setAutoRepaint(autoRepaint);
}

void GForwardDrawingSurface::setBackground(int color) {
    ensureForwardTarget();
    _forwardTarget->setBackground(color);
}

void GForwardDrawingSurface::setBackground(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setBackground(color);
}

void GForwardDrawingSurface::setColor(int color) {
    ensureForwardTarget();
    _forwardTarget->setColor(color);
}

void GForwardDrawingSurface::setColor(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setColor(color);
}

void GForwardDrawingSurface::setFillColor(int color) {
    ensureForwardTarget();
    _forwardTarget->setFillColor(color);
}

void GForwardDrawingSurface::setFillColor(const std::string& color) {
    ensureForwardTarget();
    _forwardTarget->setFillColor(color);
}

void GForwardDrawingSurface::setFont(const QFont& font) {
    ensureForwardTarget();
    _forwardTarget->setFont(font);
}

void GForwardDrawingSurface::setFont(const std::string& font) {
    ensureForwardTarget();
    _forwardTarget->setFont(font);
}

void GForwardDrawingSurface::setLineWidth(double lineWidth) {
    ensureForwardTarget();
    _forwardTarget->setLineWidth(lineWidth);
}

void GForwardDrawingSurface::setPixel(double x, double y, int rgb) {
    ensureForwardTarget();
    _forwardTarget->setPixel(x, y, rgb);
}

void GForwardDrawingSurface::setPixel(double x, double y, int r, int g, int b) {
    ensureForwardTarget();
    _forwardTarget->setPixel(x, y, r, g, b);
}

void GForwardDrawingSurface::setPixelARGB(double x, double y, int argb) {
    ensureForwardTarget();
    _forwardTarget->setPixelARGB(x, y, argb);
}

void GForwardDrawingSurface::setPixelARGB(double x, double y, int a, int r, int g, int b) {
    ensureForwardTarget();
    _forwardTarget->setPixelARGB(x, y, a, r, g, b);
}

void GForwardDrawingSurface::setPixels(const Grid<int>& pixels) {
    ensureForwardTarget();
    _forwardTarget->setPixels(pixels);
}

void GForwardDrawingSurface::setPixelsARGB(const Grid<int>& pixelsARGB) {
    ensureForwardTarget();
    _forwardTarget->setPixelsARGB(pixelsARGB);
}

void GForwardDrawingSurface::setRepaintImmediately(bool repaintImmediately) {
    ensureForwardTarget();
    _forwardTarget->setAutoRepaint(repaintImmediately);
}

/*
 * File: gthread.cpp
 * -----------------
 *
 * This file implements the members declared in gthread.h.
 *
 * @version 2018/10/18
 * - improved thread names
 * @version 2018/10/01
 * - bug fix where output wasn't showing up on the console if main ended too soon
 * @version 2018/09/23
 * - bug fix to shut down console at end of program
 * @version 2018/08/23
 * - renamed to gthread.h to replace Java version
 * @version 2018/07/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "gconsolewindow.h"
#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "geventqueue.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GFunctionThread::GFunctionThread(GThunk func, const std::string& threadName)
        : _func(func) {
    if (!threadName.empty()) {
        setObjectName(QString::fromStdString(threadName));
    }
}

void GFunctionThread::run() {
    _func();
}


/*static*/ QThread* GThread::_qtMainThread = nullptr;
/*static*/ QThread* GThread::_studentThread = nullptr;

GThread::GThread() {
    // empty
}

/*static*/ void GThread::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

/*static*/ QThread* GThread::getCurrentThread() {
    return QThread::currentThread();
}

/*static*/ QThread* GThread::getQtMainThread() {
    return _qtMainThread;
}

/*static*/ QThread* GThread::getStudentThread() {
    return _studentThread;
}

/*static*/ bool GThread::iAmRunningOnTheQtGuiThread() {
    return _qtMainThread && _qtMainThread == QThread::currentThread();
}

/*static*/ bool GThread::iAmRunningOnTheStudentThread() {
    return _studentThread && _studentThread == QThread::currentThread();
}

/*static*/ bool GThread::qtGuiThreadExists() {
    return _qtMainThread != nullptr;
}

/*static*/ void GThread::runInNewThread(GThunk func, const std::string& threadName) {
    GFunctionThread* thread = new GFunctionThread(func, threadName);
    thread->start();
    while (!thread->isFinished()) {
        sleep(10);
    }
    delete thread;
}

/*static*/ QThread* GThread::runInNewThreadAsync(GThunk func, const std::string& threadName) {
    GFunctionThread* thread = new GFunctionThread(func, threadName);
    thread->start();
    return thread;
}

/*static*/ void GThread::runOnQtGuiThread(GThunk func) {
    // send timer "event" telling GUI thread what to do
    // TODO: enable
//    if (!_initialized) {
//        error("GThread::runOnQtGuiThread: Qt GUI system has not been initialized.\n"
//              "You must #include one of the \"q*.h\" files in your main program file.");
//    }
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        GEventQueue::instance()->runOnQtGuiThreadSync(func);
    } else {
        error("GThread::runOnQtGuiThread: Qt GUI thread has not been initialized properly. \n"
              "Make sure that the file containing your main() function #includes at least \n"
              "one .h header from the Stanford C++ library.");
    }
}

/*static*/ void GThread::runOnQtGuiThreadAsync(GThunk func) {
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        GEventQueue::instance()->runOnQtGuiThreadAsync(func);
    } else {
        error("GThread::runOnQtGuiThreadAsync: Qt GUI thread has not been initialized properly. \n"
              "Make sure that the file containing your main() function #includes at least \n"
              "one .h header from the Stanford C++ library.");
    }
}

/*static*/ void GThread::setMainThread() {
    if (!_qtMainThread) {
        _qtMainThread = QThread::currentThread();
        _qtMainThread->setObjectName("Qt GUI Thread");
    }
}

/*static*/ void GThread::sleep(double ms) {
    require::nonNegative(ms, "GThread::sleep", "delay (ms)");
    getCurrentThread()->msleep(static_cast<unsigned long>(ms));
}

/*static*/ bool GThread::studentThreadExists() {
    return _studentThread != nullptr;
}

/*static*/ bool GThread::wait(QThread* thread, long ms) {
    QThread* currentThread = getCurrentThread();
    if (currentThread == thread) {
        error("GThread::wait: a thread cannot wait for itself");
    }

    long startTime = GEvent::getCurrentTimeMS();
    unsigned long msToSleep = static_cast<unsigned long>(ms > 10 ? 10 : ms);
    while (thread && thread->isRunning()) {
        currentThread->msleep(msToSleep);

        // stop if we have waited at least the given amount of time
        if (ms > 0 && GEvent::getCurrentTimeMS() - startTime >= ms) {
            break;
        }
    }
    return thread->isRunning();
}

void GThread::yield() {
    QThread::yieldCurrentThread();
}


GStudentThread::GStudentThread(GThunkInt mainFunc)
        : _mainFunc(mainFunc),
          _mainFuncVoid(nullptr),
          _result(0) {
    this->setObjectName(QString::fromStdString("Main (student)"));
}

GStudentThread::GStudentThread(GThunk mainFunc)
        : _mainFunc(nullptr),
          _mainFuncVoid(mainFunc) {
    this->setObjectName(QString::fromStdString("Main (student)"));
}

int GStudentThread::getResult() const {
    return _result;
}

void GStudentThread::run() {
    yield();

    // perform any thread-specific initialization
    stanfordcpplib::initializeLibraryStudentThread();

    if (_mainFunc) {
        _result = _mainFunc();
    } else {
        _mainFuncVoid();
    }

    // briefly wait for the console to finish printing any/all output
    yield();
    pause(1);

    // if I get here, student's main() has finished running;
    // indicate this by showing a completed title on the graphical console
    if (getConsoleEnabled()) {
        GConsoleWindow* console = getConsoleWindow();
        if (console) {
            console->shutdown();
        }
    }
//    else {
//        // need to exit here else program will not terminate
//        QtGui::instance()->exitGraphics(_result);
//    }
}

/*static*/ void GStudentThread::startStudentThread(GThunkInt mainFunc) {
    if (!_studentThread) {
        _studentThread = new GStudentThread(mainFunc);
        _studentThread->start();
    }
}

/*static*/ void GStudentThread::startStudentThreadVoid(GThunk mainFunc) {
    if (!_studentThread) {
        _studentThread = new GStudentThread(mainFunc);
        _studentThread->start();
    }
}

/*static*/ bool GStudentThread::studentThreadExists() {
    return _studentThread != nullptr;
}

/*
 * File: gdiffimage.cpp
 * --------------------
 * 
 * @author Marty Stepp
 * @version 2018/10/12
 * - added "highlight diffs in color" checkbox and functionality
 * @version 2018/09/15
 * - initial version, converted from Java back-end DiffImage class
 */

// TODO: add checkbox for highlighting diffs in color
// TODO: free memory

#define INTERNAL_INCLUDE 1
#include "gdiffimage.h"
#include <iostream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gcolorchooser.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gspacer.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

/*static*/ const std::string GDiffImage::HIGHLIGHT_COLOR_DEFAULT = "#e000e0";   // 224, 0, 224

/*static*/ void GDiffImage::showDialog(
        const std::string& name1,
        GCanvas* image1,
        const std::string& name2,
        GCanvas* image2) {
    // TODO: free memory on close
    new GDiffImage(name1, image1, name2, image2);
}

/*static*/ void GDiffImage::showDialog(
        const std::string& name1,
        const std::string& imageFile1,
        const std::string& name2,
        const std::string& imageFile2) {
    GCanvas* image1 = new GCanvas(imageFile1);
    GCanvas* image2 = new GCanvas(imageFile2);
    showDialog(name1, image1, name2, image2);
}

GDiffImage::GDiffImage(
        const std::string& name1,
        GCanvas* image1,
        const std::string& name2,
        GCanvas* image2) {
    require::nonNull(image1, "GDiffImage::constructor", "image1");
    require::nonNull(image2, "GDiffImage::constructor", "image2");

    _image1 = image1->toGImage();
    _image2 = image2->toGImage();
    _imageDiffs = nullptr;
    int diffPixelsCount = image1->countDiffPixels(image2);
    _image2->setOpacity(0.5);

    // set up window and widgets
    _window = new GWindow(800, 600);
    _window->setTitle("Compare Graphical Output");
    // _window->setResizable(false);
    _window->setAutoRepaint(false);

    _slider = new GSlider();
    _slider->setActionListener([this]() {
        _image2->setOpacity(_slider->getValue() / 100.0);
        drawImages();
    });

    _highlightDiffsBox = new GCheckBox("&Highlight diffs in color: ");
    _highlightDiffsBox->setActionListener([this]() {
        _slider->setEnabled(!_highlightDiffsBox->isChecked());
        drawImages();
    });

    _highlightColor = HIGHLIGHT_COLOR_DEFAULT;
    _colorButton = new GButton("&X");
    _colorButton->setBackground(_highlightColor);
    _colorButton->setForeground(_highlightColor);
    _colorButton->setActionListener([this]() {
        chooseHighlightColor();
    });

    _diffPixelsLabel = new GLabel("(" + std::to_string(diffPixelsCount) + " pixels differ)");
    GFont::boldFont(_diffPixelsLabel);

    _imageLabel1 = new GLabel(name1);
    _imageLabel2 = new GLabel(name2);
    _southPixelLabel = new GLabel("");

    // TODO?
    // setupMenuBar();

    // do layout
    GContainer* south = new GContainer(GContainer::LAYOUT_FLOW_VERTICAL);
    GContainer* south1 = new GContainer();
    south1->add(_imageLabel1);
    south1->add(_slider);
    south1->add(_imageLabel2);
    south->add(south1);
    // 20px strut?

    GContainer* south2 = new GContainer();
    south2->add(_diffPixelsLabel);
    south2->add(new GSpacer(20, 1));
    south2->add(_highlightDiffsBox);
    south2->add(_colorButton);
    south->add(south2);

    GContainer* south3 = new GContainer();
    south3->add(_southPixelLabel);
    south->add(south3);

    _window->addToRegion(south, GWindow::REGION_SOUTH);

    // poke the canvas
    double canvasWidth  = std::max(_image1->getWidth(),  _image2->getWidth());
    double canvasHeight = std::max(_image1->getHeight(), _image2->getHeight());
    // _window->add(_image1);
    // _window->add(_image2);

    _window->getCanvas()->setMouseListener([this](GEvent event) {
        if (event.getType() != MOUSE_MOVED) {
            return;
        }
        int x = static_cast<int>(event.getX());
        int y = static_cast<int>(event.getY());
        _southPixelLabel->setText(
                "(x=" + std::to_string(x)
                + ", y=" + std::to_string(y)
                + ") expected: " + getPixelString(_image1, x, y)
                + " actual: " + getPixelString(_image2, x, y));
    });

    // set up events
    _window->setCanvasSize(canvasWidth, canvasHeight);
    _window->drawPixel(0, 0, 0xffffff);
    drawImages();

    _window->center();
    _window->show();
}

GDiffImage::~GDiffImage() {
    // TODO: delete
    _window = nullptr;
    _slider = nullptr;
    _highlightDiffsBox = nullptr;
    _colorButton = nullptr;
}

void GDiffImage::chooseHighlightColor() {
    std::string color = GColorChooser::showDialog(
            /* parent */ _window,
            /* title */ "Choose a highlight color",
            /* initial color */ _highlightColor);
    if (color.empty()) {
        return;
    }
    _highlightColor = color;
    _colorButton->setBackground(color);
    _colorButton->setForeground(color);
    drawImages();
}

void GDiffImage::drawImages() {
    _window->clearCanvas();
    if (_highlightDiffsBox->isChecked()) {
        GThread::runOnQtGuiThreadAsync([this]() {
            // draw the highlighted diffs (if so desired)
            int w1 = _image1->getWidth();
            int h1 = _image1->getHeight();
            int w2 = _image2->getWidth();
            int h2 = _image2->getHeight();

            int wmax = std::max(w1, w2);
            int hmax = std::max(h1, h2);

            if (!_imageDiffs) {
                _imageDiffs = new GImage(wmax, hmax);
            }
            QImage* imgDiff = _imageDiffs->getQImage();

            // check each pair of pixels
            // (access raw QImages for speed)
            int highlightColor = GColor::convertColorToRGB(_highlightColor) | 0xff000000;
            QImage* img1 = _image1->getQImage();
            QImage* img2 = _image2->getQImage();
            for (int y = 0; y < hmax; y++) {
                for (int x = 0; x < wmax; x++) {
                    int pixel1 = (x < w1 && y < h1) ? (img1->pixel(x, y) & 0xffffffff) : 0;
                    int pixel2 = (x < w2 && y < h2) ? (img2->pixel(x, y) & 0xffffffff) : 0;
                    imgDiff->setPixel(x, y, (pixel1 == pixel2) ? pixel1 : highlightColor);
                }
            }
            _window->draw(_imageDiffs);
        });
    } else {
        _window->draw(_image1);
        _window->draw(_image2);   // possibly at sub-1 opacity
    }

    _window->repaint();
}

std::string GDiffImage::getPixelString(GImage* image, int x, int y) const {
    if (x >= 0 && x < image->getWidth()
            && y >= 0 && y < image->getHeight()) {
        int rgb = image->getPixel(x, y);
        return GColor::convertRGBToColor(rgb);
    } else {
        return "(none)";
    }
}

/*
 * File: gradiobutton.cpp
 * ----------------------
 *
 * @author Marty Stepp
 * @version 2018/10/06
 * - added toggle()
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gradiobutton.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gradiobutton.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

Map<std::string, QButtonGroup*> GRadioButton::_buttonGroups;

GRadioButton::GRadioButton(const std::string& text, const std::string& group, bool checked, QWidget* parent) {
    GThread::runOnQtGuiThread([this, text, group, checked, parent]() {
        _iqradioButton = new _Internal_QRadioButton(this, checked, getInternalParent(parent));
        QButtonGroup* buttonGroup = getButtonGroup(group);
        buttonGroup->addButton(_iqradioButton);
    });
    setText(text);
    setVisible(false);   // all widgets are not shown until added to a window
}

GRadioButton::~GRadioButton() {
    // TODO: delete _iqradioButton;
    _iqradioButton = nullptr;
}

std::string GRadioButton::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GRadioButton::getInternalWidget() const {
    return _iqradioButton;
}

std::string GRadioButton::getText() const {
    return _iqradioButton->text().toStdString();
}

std::string GRadioButton::getType() const {
    return "GRadioButton";
}

QWidget* GRadioButton::getWidget() const {
    return static_cast<QWidget*>(_iqradioButton);
}

bool GRadioButton::isChecked() const {
    return _iqradioButton->isChecked();
}

bool GRadioButton::isSelected() const {
    return _iqradioButton->isChecked();
}

void GRadioButton::removeActionListener() {
    removeEventListener("change");
}

void GRadioButton::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GRadioButton::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GRadioButton::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GRadioButton::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GRadioButton::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GRadioButton::setChecked(bool checked) {
    GThread::runOnQtGuiThread([this, checked]() {
        _iqradioButton->setChecked(checked);
    });
}

void GRadioButton::setSelected(bool selected) {
    setChecked(selected);
}

void GRadioButton::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqradioButton->setText(QString::fromStdString(text));
    });
}

void GRadioButton::toggle() {
    setChecked(!isChecked());
}

/* static */ QButtonGroup* GRadioButton::getButtonGroup(const std::string& group) {
    if (!_buttonGroups.containsKey(group)) {
        GThread::runOnQtGuiThread([group]() {
            _buttonGroups.put(group, new QButtonGroup());
        });
    }
    return _buttonGroups[group];
}


_Internal_QRadioButton::_Internal_QRadioButton(GRadioButton* gradioButton, bool checked, QWidget* parent)
        : QRadioButton(parent),
          _gradioButton(gradioButton) {
    require::nonNull(gradioButton, "_Internal_QRadioButton::constructor");
    setObjectName(QString::fromStdString("_Internal_QRadioButton_" + std::to_string(gradioButton->getID())));
    setChecked(checked);
    // We handle the clicked signal rather than toggled because, in a radio button group,
    // the toggled signal will fire twice: once for the radio button clicked, and once
    // for the other button that was unchecked.
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QRadioButton::handleClick() {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gradioButton);
    changeEvent.setActionCommand(_gradioButton->getActionCommand());
    _gradioButton->fireEvent(changeEvent);
}

void _Internal_QRadioButton::mouseDoubleClickEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QRadioButton::mouseDoubleClickEvent");
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gradioButton->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gradioButton);
    mouseEvent.setActionCommand(_gradioButton->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gradioButton->fireEvent(mouseEvent);
}

QSize _Internal_QRadioButton::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QRadioButton::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gradiobutton.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gcheckbox.cpp
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/10/06
 * - added toggle()
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gcheckbox.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gcheckbox.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GCheckBox::GCheckBox(const std::string& text, bool checked, QWidget* parent) {
    GThread::runOnQtGuiThread([this, checked, parent]() {
        _iqcheckBox = new _Internal_QCheckBox(this, checked, getInternalParent(parent));
    });
    setText(text);
    setVisible(false);   // all widgets are not shown until added to a window
}

GCheckBox::~GCheckBox() {
    // TODO: delete _iqcheckBox;
    _iqcheckBox = nullptr;
}

std::string GCheckBox::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GCheckBox::getInternalWidget() const {
    return _iqcheckBox;
}

std::string GCheckBox::getText() const {
    return _iqcheckBox->text().toStdString();
}

std::string GCheckBox::getType() const {
    return "GCheckBox";
}

QWidget* GCheckBox::getWidget() const {
    return static_cast<QWidget*>(_iqcheckBox);
}

bool GCheckBox::isChecked() const {
    return _iqcheckBox->isChecked();
}

bool GCheckBox::isSelected() const {
    return _iqcheckBox->isChecked();
}

void GCheckBox::removeActionListener() {
    removeEventListener("change");
}

void GCheckBox::removeDoubleClickListener() {
    removeEventListener("doubleclick");
}

void GCheckBox::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GCheckBox::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GCheckBox::setDoubleClickListener(GEventListener func) {
    setEventListener("doubleclick", func);
}

void GCheckBox::setDoubleClickListener(GEventListenerVoid func) {
    setEventListener("doubleclick", func);
}

void GCheckBox::setChecked(bool checked) {
    GThread::runOnQtGuiThread([this, checked]() {
        _iqcheckBox->setChecked(checked);
    });
}

void GCheckBox::setSelected(bool selected) {
    setChecked(selected);
}

void GCheckBox::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqcheckBox->setText(QString::fromStdString(text));
    });
}

void GCheckBox::toggle() {
    setChecked(!isChecked());
}


_Internal_QCheckBox::_Internal_QCheckBox(GCheckBox* gcheckBox, bool checked, QWidget* parent)
        : QCheckBox(parent),
          _gcheckBox(gcheckBox) {
    require::nonNull(gcheckBox, "_Internal_QCheckBox::constructor");
    setObjectName(QString::fromStdString("_Internal_QCheckBox_" + std::to_string(gcheckBox->getID())));
    setChecked(checked);
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(handleStateChange(int)));
}

void _Internal_QCheckBox::handleStateChange(int /* state */) {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gcheckBox);
    changeEvent.setActionCommand(_gcheckBox->getActionCommand());
    _gcheckBox->fireEvent(changeEvent);
}

void _Internal_QCheckBox::mouseDoubleClickEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QCheckBox::mouseDoubleClickEvent");
    QWidget::mouseDoubleClickEvent(event);   // call super
    emit doubleClicked();
    if (!_gcheckBox->isAcceptingEvent("doubleclick")) return;
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gcheckBox);
    mouseEvent.setActionCommand(_gcheckBox->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    _gcheckBox->fireEvent(mouseEvent);
}

QSize _Internal_QCheckBox::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QCheckBox::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gcheckbox.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gtable.cpp
 * ----------------
 * This file implements the GTable.h interface.
 * See that file for documentation of each member.
 *
 * @author Marty Stepp
 * @version 2018/09/06
 * - added bounds-checking and require() calls
 * @version 2018/08/23
 * - renamed to gtable.cpp to replace Java version
 * @version 2018/07/21
 * - rich formatting on cell, row, column, table
 * - improved event handling
 * @version 2018/07/17
 * - initial version, based on gtable.h
 * @since 2018/07/17
 */

#define INTERNAL_INCLUDE 1
#include "gtable.h"
#include <cstring>
#include <iostream>
#include <iomanip>
#include <QBrush>
#include <QColor>
#include <QHeaderView>
#include <QLineEdit>
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "gclipboard.h"
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GTable::TableStyle GTable::_defaultCellStyle = GTable::TableStyle::unset();

GTable::GTable(int rows, int columns, double width, double height, QWidget* parent)
        : _iqtableview(nullptr),
          _columnHeaderStyle(GTable::COLUMN_HEADER_NONE) {
    GThread::runOnQtGuiThread([this, rows, columns, parent]() {
        _iqtableview = new _Internal_QTableWidget(this, rows, columns, getInternalParent(parent));
        _iqtableview->setSelectionMode(QAbstractItemView::SingleSelection);
        _globalCellStyle = TableStyle::unset();
    });
    require::nonNegative2D(rows, columns, "GTable::constructor", "rows", "columns");
    require::nonNegative2D(width, height, "GTable::constructor", "width", "height");
    setVisible(false);   // all widgets are not shown until added to a window
}

GTable::~GTable() {
    // TODO: delete
    _iqtableview = nullptr;
}

void GTable::applyStyleToCell(int row, int column, const TableStyle& style) {
    setCellAlignmentInternal(row, column, style.alignment);
    setCellBackgroundInternal(row, column, style.background);
    setCellFontInternal(row, column, style.font);
    setCellForegroundInternal(row, column, style.foreground);
}

void GTable::autofitColumnWidths() {
    GThread::runOnQtGuiThread([this]() {
        _iqtableview->resizeColumnsToContents();
    });
}

void GTable::checkColumn(const std::string& member, int column) const {
    require::inRange(column, 0, numCols(), "GTable::" + member, "column");
}

void GTable::checkIndex(const std::string& member, int row, int column) const {
    require::inRange2D(row, column, 0, 0, numRows(), numCols(), "GTable::" + member, "row", "column");
}

void GTable::checkRow(const std::string& member, int row) const {
    require::inRange(row, 0, numRows(), "GTable::" + member, "row");
}

void GTable::clear() {
    GThread::runOnQtGuiThread([this]() {
        _iqtableview->clear();
        // for some reason, clearing a table also wipes the Excel-style column headers
        updateColumnHeaders();
    });
}

void GTable::clearCell(int row, int column) {
    checkIndex("clearCell", row, column);
    set(row, column, /* text */ "");
}

void GTable::clearFormatting() {
    GThread::runOnQtGuiThread([this]() {
        // clear out all records of row, column, and global table styles
        _columnStyles.clear();
        _rowStyles.clear();
        _globalCellStyle = TableStyle::unset();

        // set the formatting on each cell
        for (int row = 0, nr = numRows(), nc = numCols(); row < nr; row++) {
            for (int col = 0; col < nc; col++) {
                clearCellFormatting(row, col);
            }
        }
    });
}

void GTable::clearCellFormatting(int row, int column) {
    checkIndex("clearCellFormatting", row, column);
    GThread::runOnQtGuiThread([this, row, column]() {
        ensureDefaultFormatting();
        TableStyle style = _defaultCellStyle;
        if (style.background >= 0) {
            setCellBackgroundInternal(row, column, style.background);
        }
        if (!style.font.empty()) {
            setCellFontInternal(row, column, style.font);
        }
        if (style.foreground >= 0) {
            setCellForegroundInternal(row, column, style.foreground);
        }
        if (style.alignment >= 0) {
            setCellAlignmentInternal(row, column, style.alignment);
        }
    });
}

void GTable::clearSelection() {
    GThread::runOnQtGuiThread([this]() {
        _iqtableview->clearSelection();
    });
}

void GTable::ensureColumnStyle(int column) {
    ensureDefaultFormatting();
    if (!_columnStyles.containsKey(column)) {
        _columnStyles[column] = TableStyle::unset();
    }
}

void GTable::ensureDefaultFormatting() const {
    GTable* thisHack = (GTable*) this;
    if (!_defaultCellStyle.isSet()) {
        QPalette palette = thisHack->_iqtableview->palette();
        _defaultCellStyle.background = palette.base().color().rgb() & 0x00ffffff;
        _defaultCellStyle.foreground = palette.text().color().rgb() & 0x00ffffff;
        _defaultCellStyle.font       = GFont::toFontString(thisHack->_iqtableview->font());
        _defaultCellStyle.alignment  = ALIGN_LEFT;   // TODO: ask cell for its alignment
    }
}

void GTable::ensureRowStyle(int row) {
    ensureDefaultFormatting();
    if (!_rowStyles.containsKey(row)) {
        _rowStyles[row] = TableStyle::unset();
    }
}

void GTable::fill(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        int nr = numRows();
        int nc = numCols();
        for (int r = 0; r < nr; r++) {
            for (int c = 0; c < nc; c++) {
                set(r, c, text);
            }
        }
    });
}

std::string GTable::get(int row, int column) const {
    checkIndex("get", row, column);
    return _iqtableview->model()->data(_iqtableview->model()->index(row, column)).toString().toStdString();
}

GTable::ColumnHeaderStyle GTable::getColumnHeaderStyle() const {
    return _columnHeaderStyle;
}

double GTable::getColumnWidth(int column) const {
    checkColumn("getColumnWidth", column);
    return _iqtableview->columnWidth(column);
}

_Internal_QWidget* GTable::getInternalWidget() const {
    return _iqtableview;
}

GTable::TableStyle GTable::getMergedStyleForCell(int row, int column) {
    // style precedence: cell > column > row > global
    ensureDefaultFormatting();
    TableStyle style = _defaultCellStyle;
    style.mergeWith(_globalCellStyle);
    if (_rowStyles.containsKey(row)) {
        style.mergeWith(_rowStyles[row]);
    }
    if (_columnStyles.containsKey(column)) {
        style.mergeWith(_columnStyles[column]);
    }
    return style;
}

double GTable::getRowHeight(int row) const {
    checkRow("getRowHeight", row);
    return _iqtableview->rowHeight(row);
}

GridLocation GTable::getSelectedCell() const {
    QModelIndexList list = _iqtableview->selectionModel()->selectedIndexes();
    if (list.empty()) {
        return GridLocation(-1, -1);
    } else {
        QModelIndex index = list.at(0);
        return GridLocation(index.row(), index.column());
    }
}

void GTable::getSelectedCell(int& row, int& column) const {
    GridLocation loc = getSelectedCell();
    row = loc.row;
    column = loc.col;
}

std::string GTable::getSelectedCellValue() const {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        return get(loc.row, loc.col);
    } else {
        return "";
    }
}

int GTable::getSelectedColumn() const {
    return getSelectedCell().col;
}

int GTable::getSelectedRow() const {
    return getSelectedCell().row;
}

std::string GTable::getType() const {
    return "GTable";
}

QWidget* GTable::getWidget() const {
    return static_cast<QWidget*>(_iqtableview);
}

bool GTable::hasSelectedCell() const {
    GridLocation loc = getSelectedCell();
    return loc.row >= 0 && loc.col >= 0;
}

int GTable::height() const {
    return numRows();
}

bool GTable::inBounds(int row, int column) const {
    return 0 <= row && row < height() && 0 <= column && column < width();
}

bool GTable::isEditable() const {
    return (_iqtableview->editTriggers() & QAbstractItemView::NoEditTriggers) != 0;
}

int GTable::numCols() const {
    return _iqtableview->model()->columnCount();
}

int GTable::numRows() const {
    return _iqtableview->model()->rowCount();
}

void GTable::removeTableListener() {
    removeEventListeners({"table",
                          "tableupdate",
                          "tableselect",
                          "tableeditbegin",
                          "tablereplacebegin",
                          "tablecut",
                          "tablecopy",
                          "tablepaste"});
}

void GTable::requestFocus() {
    bool wasEditing = _iqtableview->isEditing();
    GInteractor::requestFocus();
    if (!wasEditing && hasSelectedCell()) {
        GThread::runOnQtGuiThread([this]() {
            GridLocation loc = getSelectedCell();
            _iqtableview->closePersistentEditor(_iqtableview->item(loc.row, loc.col));
        });
    }
}

void GTable::resize(int newNumRows, int newNumCols) {
    require::nonNegative2D(newNumRows, newNumCols, "GTable::resize", "rows", "columns");
    GThread::runOnQtGuiThread([this, newNumRows, newNumCols]() {
        int oldNumRows = numRows();
        int oldNumCols = numCols();
        _iqtableview->setRowCount(newNumRows);
        _iqtableview->setColumnCount(newNumCols);

        // make sure proper headers showing on each column
        if (newNumCols > oldNumCols) {
            updateColumnHeaders();
        }

        // apply appropriate styles to newly added cells
        if (newNumRows > oldNumRows || newNumCols > oldNumCols) {
            for (int row = 0; row < newNumRows; row++) {
                for (int col = 0; col < newNumCols; col++) {
                    if (row >= oldNumRows || col >= oldNumCols) {
                        // figure out appropriate style (row, col, global, etc.) and apply it
                        TableStyle style = getMergedStyleForCell(row, col);
                        applyStyleToCell(row, col, style);
                    }
                }
            }
        }
    });
}

bool GTable::rowColumnHeadersVisible() const {
    return _iqtableview->horizontalHeader()->isVisible()
            && _iqtableview->verticalHeader()->isVisible();
}

void GTable::select(int row, int column) {
    checkIndex("select", row, column);
    GThread::runOnQtGuiThread([this, row, column]() {
        QModelIndex index = _iqtableview->model()->index(row, column);
        _iqtableview->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
    });
}

void GTable::set(int row, int column, const std::string& text) {
    checkIndex("set", row, column);
    GThread::runOnQtGuiThread([this, row, column, text]() {
        QModelIndex index = _iqtableview->model()->index(row, column);
        _iqtableview->model()->setData(index, QVariant(text.c_str()));
    });
}

void GTable::setBackground(int rgb) {
    // GInteractor::setBackground(rgb);
    // (don't call super; that will set the BG of the headers and everything)

    GThread::runOnQtGuiThread([this, rgb]() {
        // save this background color in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();
        _globalCellStyle.background = rgb;

        // remove background colors from any row/column styles because they are
        // now overridden by this global background color style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].background = unset.background;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].background = unset.background;
        }

        // set each cell's background color
        for (int row = 0, nr = numRows(); row < nr; row++) {
            for (int col = 0, nc = numCols(); col < nc; col++) {
                setCellBackgroundInternal(row, col, rgb);
            }
        }
    });
}

void GTable::setBackground(const std::string& color) {
    setBackground(GColor::convertColorToRGB(color));
}

void GTable::setCellAlignment(int row, int column, HorizontalAlignment alignment) {
    checkIndex("setCellAlignment", row, column);
    GThread::runOnQtGuiThread([this, row, column, alignment]() {
        setCellAlignmentInternal(row, column, alignment);   // do the actual work
    });
}

void GTable::setCellAlignmentInternal(int row, int column, HorizontalAlignment alignment) {
    Qt::Alignment align = Qt::AlignVCenter | toQtAlignment(alignment);
    _iqtableview->model()->setData(_iqtableview->model()->index(row, column),
                                   QVariant(align), Qt::TextAlignmentRole);
}

void GTable::setCellBackground(int row, int column, int rgb) {
    checkIndex("setCellBackground", row, column);
    GThread::runOnQtGuiThread([this, row, column, rgb]() {
        setCellBackgroundInternal(row, column, rgb);   // do the actual work
    });
}

void GTable::setCellBackground(int row, int column, const std::string& color) {
    setCellBackground(row, column, GColor::convertColorToRGB(color));
}

void GTable::setCellBackgroundInternal(int row, int column, int rgb) {
    _iqtableview->item(row, column)->setBackground(QBrush(QColor(rgb)));
}

void GTable::setCellFont(int row, int column, const std::string& font) {
    checkIndex("setCellFont", row, column);
    GThread::runOnQtGuiThread([this, row, column, font]() {
        setCellFontInternal(row, column, font);   // do the actual work
    });
}

void GTable::setCellFontInternal(int row, int column, const std::string& font) {
    _iqtableview->item(row, column)->setFont(GFont::toQFont(_iqtableview->font(), font));
}

void GTable::setCellForeground(int row, int column, int rgb) {
    checkIndex("setCellForeground", row, column);
    GThread::runOnQtGuiThread([this, row, column, rgb]() {
        setCellForegroundInternal(row, column, rgb);   // do the actual work
    });
}

void GTable::setCellForegroundInternal(int row, int column, int rgb) {
    ensureDefaultFormatting();
    _iqtableview->item(row, column)->setForeground(QBrush(QColor(rgb)));
}

void GTable::setCellForeground(int row, int column, const std::string& color) {
    checkIndex("setCellForeground", row, column);
    setCellForeground(row, column, GColor::convertColorToRGB(color));
}

void GTable::setColor(int rgb) {
    setForeground(rgb);
}

void GTable::setColor(const std::string& color) {
    setForeground(color);
}

void GTable::setColumnAlignment(int column, HorizontalAlignment alignment) {
    checkColumn("setColumnAlignment", column);

    GThread::runOnQtGuiThread([this, column, alignment]() {
        // save this alignment in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].alignment = alignment;

        // set each cell's alignment in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellAlignmentInternal(row, column, alignment);
        }
    });
}

void GTable::setColumnBackground(int column, int rgb) {
    checkColumn("setColumnBackground", column);

    GThread::runOnQtGuiThread([this, column, rgb]() {
        // save this background color in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].background = rgb;

        // set each cell's background color in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellBackgroundInternal(row, column, rgb);
        }
    });
}

void GTable::setColumnBackground(int column, const std::string& color) {
    setColumnBackground(column, GColor::convertColorToRGB(color));
}

void GTable::setColumnFont(int column, const std::string& font) {
    checkColumn("setColumnFont", column);

    GThread::runOnQtGuiThread([this, column, font]() {
        // save this font in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].font = font;

        // set each cell's font in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellFontInternal(row, column, font);
        }
    });
}

void GTable::setColumnForeground(int column, int rgb) {
    checkColumn("setColumnForeground", column);

    GThread::runOnQtGuiThread([this, column, rgb]() {
        // save this foreground color in the column style (for later cells on resize() etc)
        ensureColumnStyle(column);
        _columnStyles[column].foreground = rgb;

        // set each cell's foreground color in that column
        for (int row = 0, nr = numRows(); row < nr; row++) {
            setCellForegroundInternal(row, column, rgb);
        }
    });
}

void GTable::setColumnForeground(int column, const std::string& color) {
    setColumnForeground(column, GColor::convertColorToRGB(color));
}

void GTable::setColumnHeaderStyle(GTable::ColumnHeaderStyle style) {
    GThread::runOnQtGuiThread([this, style]() {
        _columnHeaderStyle = style;
        if (style == GTable::COLUMN_HEADER_NONE) {
            // no headers
            setRowColumnHeadersVisible(false);
            return;
        } else {
            // build list of column names to display
            updateColumnHeaders();
            _iqtableview->horizontalHeader()->setVisible(true);
            _iqtableview->verticalHeader()->setVisible(true);
        }
    });
}

void GTable::setColumnWidth(int column, double width) {
    checkColumn("setColumnWidth", column);
    if (width < 0) {
        error("GTable::setColumnWidth: width cannot be negative");
    }
    GThread::runOnQtGuiThread([this, column, width]() {
        _iqtableview->setColumnWidth(column, (int) width);
    });
}

void GTable::setEditable(bool editable) {
    GThread::runOnQtGuiThread([this, editable]() {
        if (editable) {
            _iqtableview->setEditTriggers(
                        QAbstractItemView::CurrentChanged
                        | QAbstractItemView::DoubleClicked
                        | QAbstractItemView::EditKeyPressed
                        | QAbstractItemView::AnyKeyPressed);
        } else {
            _iqtableview->setEditTriggers(QAbstractItemView::NoEditTriggers);
        }
    });
}

void GTable::setEditorValue(int row, int column, const std::string& text) {
    checkIndex("setEditorValue", row, column);
    GThread::runOnQtGuiThread([this, text]() {
        _Internal_QItemDelegate* delegate = _iqtableview->getItemDelegate();
        if (delegate != nullptr) {
            QWidget* editor = delegate->getEditor();
            if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor)) {
                lineEdit->setText(QString::fromStdString(text));
            }
        }
    });
}

void GTable::setFont(const QFont& font) {
    setFont(GFont::toFontString(font));
}

void GTable::setFont(const std::string& font) {
    GInteractor::setFont(font);   // call super

    GThread::runOnQtGuiThread([this, font]() {
        // save this font in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();
        _globalCellStyle.font = font;

        // remove fonts from any row/column styles because they are
        // now overridden by this global font style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].font = unset.font;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].font = unset.font;
        }

        // set each cell's foreground color
        for (int row = 0, nr = numRows(); row < nr; row++) {
            for (int col = 0, nc = numCols(); col < nc; col++) {
                setCellFontInternal(row, col, font);
            }
        }
    });
}

void GTable::setForeground(int rgb) {
    GThread::runOnQtGuiThread([this, rgb]() {
        // GInteractor::setForeground(rgb);
        // (don't call super; that will set the FG of the scrollbars and everything)

        // save this foreground color in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();
        _globalCellStyle.foreground = rgb;

        // remove foreground colors from any row/column styles because they are
        // now overridden by this global foreground color style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].foreground = unset.foreground;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].foreground = unset.foreground;
        }

        // set each cell's foreground color
        for (int row = 0, nr = numRows(); row < nr; row++) {
            for (int col = 0, nc = numCols(); col < nc; col++) {
                setCellForegroundInternal(row, col, rgb);
            }
        }
    });
}

void GTable::setForeground(const std::string& color) {
    setForeground(GColor::convertColorToRGB(color));
}

void GTable::setHorizontalAlignment(HorizontalAlignment alignment) {
    GThread::runOnQtGuiThread([this, alignment]() {
        // save this alignment in the global cell style (for later cells on resize() etc)
        ensureDefaultFormatting();

        _globalCellStyle.alignment = alignment;

        // remove alignment from any row/column styles because they are
        // now overridden by this global alignment style
        TableStyle unset = TableStyle::unset();
        for (int row : _rowStyles) {
            _rowStyles[row].alignment = unset.alignment;
        }
        for (int col : _columnStyles) {
            _columnStyles[col].alignment = unset.alignment;
        }

        // set each cell's horizontal alignment
        for (int row = 0, nr = numRows(), nc = numCols(); row < nr; row++) {
            for (int col = 0; col < nc; col++) {
                setCellAlignmentInternal(row, col, alignment);
            }
        }
    });
}

void GTable::setRowAlignment(int row, HorizontalAlignment alignment) {
    checkRow("setRowAlignment", row);

    // save this alignment in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, alignment]() {
        ensureRowStyle(row);
        _rowStyles[row].alignment = alignment;

        // set each cell's alignment in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellAlignmentInternal(row, col, alignment);
        }
    });
}

void GTable::setRowBackground(int row, int rgb) {
    checkRow("setRowBackground", row);

    // save this background color in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, rgb]() {
        ensureRowStyle(row);
        _rowStyles[row].background = rgb;

        // set each cell's background color in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellBackgroundInternal(row, col, rgb);
        }
    });
}

void GTable::setRowBackground(int row, const std::string& color) {
    setRowBackground(row, GColor::convertColorToRGB(color));
}

void GTable::setRowFont(int row, const std::string& font) {
    checkRow("setRowFont", row);

    // save this font in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, font]() {
        ensureRowStyle(row);
        _rowStyles[row].font = font;

        // set each cell's font in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellFontInternal(row, col, font);
        }
    });
}

void GTable::setRowForeground(int row, int rgb) {
    checkRow("setRowForeground", row);

    // save this foreground color in the row style (for later cells on resize() etc)
    GThread::runOnQtGuiThread([this, row, rgb]() {
        ensureRowStyle(row);
        _rowStyles[row].foreground = rgb;

        // set each cell's foreground color in that row
        for (int col = 0, nc = numCols(); col < nc; col++) {
            setCellForegroundInternal(row, col, rgb);
        }
    });
}

void GTable::setRowForeground(int row, const std::string& color) {
    checkRow("setRowForeground", row);
    setRowForeground(row, GColor::convertColorToRGB(color));
}

void GTable::setRowColumnHeadersVisible(bool visible) {
    GThread::runOnQtGuiThread([this, visible]() {
        _iqtableview->horizontalHeader()->setVisible(visible);
        _iqtableview->verticalHeader()->setVisible(visible);
    });
}

void GTable::setRowHeight(int row, double height) {
    checkRow("setRowHeight", row);
    if (height < 0) {
        error("GTable::setRowHeight: height cannot be negative");
    }
    GThread::runOnQtGuiThread([this, row, height]() {
        _iqtableview->setRowHeight(row, (int) height);
    });
}

void GTable::setSelectedCellValue(const std::string& text) {
    if (hasSelectedCell()) {
        GridLocation loc = getSelectedCell();
        set(loc.row, loc.col, text);
    }
}

void GTable::setTableListener(GEventListener func) {
    setEventListeners({"table",
                       "tableupdate",
                       "tableselect",
                       "tableeditbegin",
                       "tablereplacebegin",
                       "tablecut",
                       "tablecopy",
                       "tablepaste"}, func);
}

void GTable::setTableListener(GEventListenerVoid func) {
    setEventListeners({"table",
                       "tableupdate",
                       "tableselect",
                       "tableeditbegin",
                       "tablereplacebegin",
                       "tablecut",
                       "tablecopy",
                       "tablepaste"}, func);
}

std::string GTable::toExcelColumnName(int col) {
    // convert column into a roughly base-26 Excel column name,
    // e.g. 0 -> "A", 1 -> "B", 26 -> "AA", ...
    std::string colStr;
    col = col + 1;   // 1-based
    while (col-- > 0) {
        colStr = charToString((char) ('A' + (col % 26))) + colStr;
        col /= 26;
    }
    return colStr;
}

void GTable::updateColumnHeaders() {
    GThread::runOnQtGuiThread([this]() {
        if (_columnHeaderStyle == GTable::COLUMN_HEADER_NONE) {
            return;
        }

        // Qt wants me to put the headers into a string list, and apparently
        // you add things to the list using << like it was a ostream (bleh)
        QStringList columnHeaders;
        for (int col = 0, nc = numCols(); col < nc; col++) {
            if (_columnHeaderStyle == GTable::COLUMN_HEADER_EXCEL) {
                columnHeaders << QString::fromStdString(toExcelColumnName(col));
            } else {
                // style == GTable::COLUMN_HEADER_NUMERIC
                columnHeaders << QString::fromStdString(std::to_string(col));
            }
        }
        _iqtableview->setHorizontalHeaderLabels(columnHeaders);
    });
}

int GTable::width() const {
    return numCols();
}


_Internal_QItemDelegate::_Internal_QItemDelegate(QObject* parent)
        : QStyledItemDelegate(parent),
          _editor(nullptr) {
    // empty
}

QWidget* _Internal_QItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QWidget* editor = QStyledItemDelegate::createEditor(parent, option, index);
    _Internal_QItemDelegate* hack = (_Internal_QItemDelegate*) this;
    hack->_editor = editor;
    return editor;
}

void _Internal_QItemDelegate::destroyEditor(QWidget* editor, const QModelIndex& index) const {
    _Internal_QItemDelegate* hack = (_Internal_QItemDelegate*) this;
    hack->_editor = nullptr;
    QStyledItemDelegate::destroyEditor(editor, index);
}

QWidget* _Internal_QItemDelegate::getEditor() const {
    return _editor;
}


_Internal_QTableWidget::_Internal_QTableWidget(GTable* gtable, int rows, int columns, QWidget* parent)
        : QTableWidget(rows, columns, parent),
          _gtable(gtable),
          _delegate(nullptr) {
    require::nonNull(gtable, "_Internal_QTableWidget::constructor");
    setObjectName(QString::fromStdString("_Internal_QTableWidget_" + std::to_string(gtable->getID())));
    _delegate = new _Internal_QItemDelegate();
    setItemDelegate(_delegate);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(handleCellChange(int, int)));
    connect(this, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(handleCellDoubleClick(int, int)));
    connect(this->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(handleSelectionChange(const QItemSelection&, const QItemSelection&)));
}

void _Internal_QTableWidget::fireTableEvent(EventType eventType, const std::string& eventName, int row, int col) {
    GEvent tableEvent(
                /* class  */ TABLE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ _gtable);
    if (row < 0 && col < 0) {
        tableEvent.setRowAndColumn(_gtable->getSelectedRow(), _gtable->getSelectedColumn());
    } else {
        tableEvent.setRowAndColumn(row, col);
    }
    tableEvent.setActionCommand(_gtable->getActionCommand());
    _gtable->fireEvent(tableEvent);
}

bool _Internal_QTableWidget::edit(const QModelIndex& index, QAbstractItemView::EditTrigger trigger, QEvent* event) {
    bool result = QAbstractItemView::edit(index, trigger, event);   // call super
    if (result) {
        bool isEdit = _lastKeyPressed == 0 || _lastKeyPressed == Qt::Key_F2 || _lastKeyPressed == Qt::Key_Tab;
        _lastKeyPressed = 0;
        if (isEdit) {
            fireTableEvent(TABLE_EDIT_BEGIN, "tableeditbegin", index.row(), index.column());
        } else {
            fireTableEvent(TABLE_REPLACE_BEGIN, "tablereplacebegin", index.row(), index.column());
        }
    }
    return result;
}

QWidget* _Internal_QTableWidget::getEditor() const {
    return _delegate->getEditor();
}

_Internal_QItemDelegate* _Internal_QTableWidget::getItemDelegate() const {
    return _delegate;
}

bool _Internal_QTableWidget::isEditing() const {
    return getEditor() != nullptr;
}

void _Internal_QTableWidget::closeEditor(QWidget* editor, QAbstractItemDelegate::EndEditHint hint) {
    QTableWidget::closeEditor(editor, hint);
    // TODO: doesn't this fire even if the edit is committed?
    fireTableEvent(TABLE_EDIT_CANCEL, "tableeditcancel");
}

void _Internal_QTableWidget::handleCellChange(int row, int column) {
    fireTableEvent(TABLE_UPDATED, "tableupdate", row, column);
}

void _Internal_QTableWidget::handleCellDoubleClick(int /*row*/, int /*column*/) {
    _lastKeyPressed = Qt::Key_F2;   // pretend we pressed F2
    // edit/replace begin event will be fired by edit()
    // fireTableEvent(GEvent::TABLE_EDIT_BEGIN, "tableeditbegin", row, column);
}

void _Internal_QTableWidget::handleSelectionChange(const QItemSelection& selected, const QItemSelection& /*deselected*/) {
    QItemSelectionRange range;
    if (!selected.empty()) {
        range = selected.at(0);
        QPersistentModelIndex index = range.topLeft();
        fireTableEvent(TABLE_SELECTED, "tableselect", index.row(), index.column());
    }
}

void _Internal_QTableWidget::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QTableWidget::keyPressEvent", "event");
    _lastKeyPressed = event->key();
    bool wasEditing = isEditing();
    if (!wasEditing && event->key() == Qt::Key_Delete) {
        // clear data from selected cell
        if (_gtable->hasSelectedCell()) {
            GridLocation loc = _gtable->getSelectedCell();
            _gtable->clearCell(loc.row, loc.col);
            return;
        }
    }

    // any other key
    if (wasEditing || !_gtable->hasSelectedCell()) {
        QTableWidget::keyPressEvent(event);   // call super
        return;
    }

    bool nowEditing = isEditing();

    if (GClipboard::isCut(event)) {
        // keyboard "cut" command; remove data from cell into clipboard
        GridLocation loc = _gtable->getSelectedCell();
        std::string cellValue = _gtable->get(loc.row, loc.col);
        GClipboard::set(cellValue);
        _gtable->clearCell(loc.row, loc.col);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(TABLE_CUT, "tablecut");
        return;
    }

    if (GClipboard::isCopy(event)) {
        // keyboard "copy" command; copy data from cell into clipboard
        std::string cellValue = _gtable->getSelectedCellValue();
        GClipboard::set(cellValue);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(TABLE_COPY, "tablecopy");
        return;
    }

    if (GClipboard::isPaste(event)) {
        // keyboard "paste" command; copy data from clipboard into cell
        std::string cellValue = GClipboard::get();
        _gtable->setSelectedCellValue(cellValue);
        QTableWidget::keyPressEvent(event);   // call super
        fireTableEvent(TABLE_PASTE, "tablepaste");
        return;
    }

    // if cell went from non-editing state to editing state, edit has begun
    if (nowEditing) {
        if (event->key() == Qt::Key_F2) {
            // F2 key begins editing existing value for a cell
            // edit_begin will be fired by edit() method
            // fireTableEvent(GEvent::TABLE_EDIT_BEGIN, "tableeditbegin");
        } else if (event->key() == Qt::Key_Tab) {
            // Tab key jumps to edit the neighboring cell
        } else {
            // any other text starts replacing the value with a new value
            // replace_begin will be fired by edit() method
            // fireTableEvent(GEvent::TABLE_REPLACE_BEGIN, "tablereplacebegin");
        }
    }
    QTableWidget::keyPressEvent(event);   // call super
}

QSize _Internal_QTableWidget::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTableWidget::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gtable.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gcolorchooser.cpp
 * -----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gcolorchooser.cpp to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gcolorchooser.h"
#include <QColorDialog>
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#undef INTERNAL_INCLUDE

GColorChooser::GColorChooser() {
    // empty
}

std::string GColorChooser::showDialog(const std::string& title, int initialColor) {
    return showDialog(/* parent */ (QWidget*) nullptr, title, initialColor);
}

std::string GColorChooser::showDialog(GWindow* parent, const std::string& title, int initialColor) {
    return showDialog(parent ? parent->getWidget() : nullptr, title, GColor::convertRGBToColor(initialColor));
}

std::string GColorChooser::showDialog(QWidget* parent, const std::string& title, int initialColor) {
    return showDialog(parent, title, GColor::convertRGBToColor(initialColor));
}

std::string GColorChooser::showDialog(const std::string& title, const std::string& initialColor) {
    return showDialog(/* parent */ (QWidget*) nullptr, title, initialColor);
}

std::string GColorChooser::showDialog(GWindow* parent, const std::string& title, const std::string& initialColor) {
    return showDialog(parent ? parent->getWidget() : nullptr, title, initialColor);
}

std::string GColorChooser::showDialog(QWidget* parent, const std::string& title, const std::string& initialColor) {
    QColor initialQColor = initialColor.empty() ? Qt::white : GColor::toQColor(initialColor);
    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, initialQColor, &result]() {
        QColor selectedColor = QColorDialog::getColor(initialQColor, parent, QString::fromStdString(title));
        if (selectedColor.isValid()) {
            result = GColor::convertQColorToColor(selectedColor);
        }
    });
    return result;
}

/*
 * File: gtextarea.cpp
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gtextarea.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gtextarea.h"
#include <QScrollBar>
#include <QTextCursor>
#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#define INTERNAL_INCLUDE 1
#include "gfont.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GTextArea::GTextArea(int rows, int columns, QWidget* parent)
        : _contextMenuEnabled(true) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqtextedit = new _Internal_QTextEdit(this, getInternalParent(parent));
    });
    setRowsColumns(rows, columns);
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextArea::GTextArea(const std::string& text, QWidget* parent)
        : _contextMenuEnabled(true) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqtextedit = new _Internal_QTextEdit(this, getInternalParent(parent));
    });
    setText(text);
    setVisible(false);   // all widgets are not shown until added to a window
}

GTextArea::~GTextArea() {
    // TODO: delete _iqtextedit;
    _iqtextedit = nullptr;
}

void GTextArea::appendFormattedText(const std::string& text, const std::string& color, const std::string& font) {
    moveCursorToEnd();

    // create a formatted block with the font and color
    QTextCharFormat format;
    if (!color.empty()) {
        format.setForeground(QBrush(GColor::convertColorToRGB(color)));
    }
    if (!font.empty()) {
        // carry over only the font's weight, not size/face
        QFont qfont = GFont::toQFont(_iqtextedit->font(), font);
        format.setFontWeight(qfont.weight());
    }

    QTextCursor cursor = _iqtextedit->textCursor();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    cursor.insertText(QString::fromStdString(text), format);
    cursor.endEditBlock();
    GThread::runOnQtGuiThread([this, cursor]() {
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });

    moveCursorToEnd();
}

void GTextArea::appendHtml(const std::string& html) {
    // TODO: use insertHtml for speed?
    setHtml(getHtml() + html);
}

void GTextArea::appendText(const std::string& text) {
    QTextCursor cursor = _iqtextedit->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    cursor.insertText(QString::fromStdString(text));
    moveCursorToEnd();
}

void GTextArea::clearSelection() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.clearSelection();
        _iqtextedit->setTextCursor(cursor);
    });
}

void GTextArea::clearText() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextedit->clear();
    });
}

int GTextArea::getColumns() const {
    return (int) (getHeight() / getRowColumnSize().getWidth());
}

int GTextArea::getCursorPosition() const {
    return _iqtextedit->textCursor().position();
}

std::string GTextArea::getHtml() const {
    return _iqtextedit->toHtml().toStdString();
}

_Internal_QWidget* GTextArea::getInternalWidget() const {
    return _iqtextedit;
}

std::string GTextArea::getPlaceholder() const {
    return _iqtextedit->placeholderText().toStdString();
}

GDimension GTextArea::getRowColumnSize() const {
    QFontMetrics metrics(_iqtextedit->font());
    return GDimension(metrics.width(QString::fromStdString("mmmmmmmmmm")) / 10.0, metrics.lineSpacing() + 2);
}

int GTextArea::getRows() const {
    return (int) (getHeight() / getRowColumnSize().getHeight());
}

std::string GTextArea::getSelectedText() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return getText().substr(start, end - start);
    } else {
        return "";
    }
}

int GTextArea::getSelectionEnd() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return end;
    } else {
        // no selection; cursor sets selection start/end to be equal
        return -1;
    }
}

int GTextArea::getSelectionLength() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    return end - start;
}

int GTextArea::getSelectionStart() const {
    QTextCursor cursor = _iqtextedit->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    if (end > start) {
        return start;
    } else {
        return -1;
    }
}

std::string GTextArea::getText() const {
    return _iqtextedit->toPlainText().toStdString();
}

std::string GTextArea::getType() const {
    return "GTextArea";
}

QWidget* GTextArea::getWidget() const {
    return static_cast<QWidget*>(_iqtextedit);
}

bool GTextArea::isContextMenuEnabled() const {
    return _contextMenuEnabled;
}

bool GTextArea::isEditable() const {
    return !_iqtextedit->isReadOnly();
}

bool GTextArea::isLineWrap() const {
    return _iqtextedit->lineWrapMode() != QTextEdit::NoWrap;
}

void GTextArea::moveCursorToEnd() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });
}

void GTextArea::moveCursorToStart() {
    GThread::runOnQtGuiThread([this]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });
}

void GTextArea::removeKeyListener() {
    removeEventListeners({"keypress",
                          "keyrelease",
                          "keytype"});
}

void GTextArea::removeMouseListener() {
    removeEventListeners({"mousepress",
                          "mouserelease"});
}

void GTextArea::removeTextChangeListener() {
    removeEventListener("textchange");
}

void GTextArea::scrollToBottom() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextedit->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
        scrollbar->setSliderPosition(scrollbar->maximum());
    });
}

void GTextArea::scrollToTop() {
    GThread::runOnQtGuiThread([this]() {
        QScrollBar* scrollbar = _iqtextedit->verticalScrollBar();
        scrollbar->setValue(0);
        scrollbar->setSliderPosition(0);
    });
}

void GTextArea::select(int startIndex, int length) {
    require::nonNegative(startIndex, 0, "GTextArea::select", "startIndex");
    require::nonNegative(length, 0, "GTextArea::select", "length");
    GThread::runOnQtGuiThread([this, startIndex, length]() {
        QTextCursor cursor = _iqtextedit->textCursor();
        cursor.setPosition(startIndex);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);
        _iqtextedit->setTextCursor(cursor);
    });
}

void GTextArea::selectAll() {
    GThread::runOnQtGuiThread([this]() {
        _iqtextedit->selectAll();
    });
}

void GTextArea::setColumns(int columns) {
    require::nonNegative(columns, "GTextArea::setColumns");
    double desiredWidth = getRowColumnSize().getWidth() * columns;
    setPreferredSize(desiredWidth, getHeight());
    setSize(desiredWidth, getHeight());
}

void GTextArea::setContextMenuEnabled(bool enabled) {
    _contextMenuEnabled = enabled;
}

void GTextArea::setCursorPosition(int index, bool keepAnchor) {
    require::nonNegative(index, "TextArea::setCursorPosition", "index");
    GThread::runOnQtGuiThread([this, index, keepAnchor]() {
        QTextCursor cursor(_iqtextedit->textCursor());
        cursor.setPosition(index, keepAnchor ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
        _iqtextedit->setTextCursor(cursor);
        _iqtextedit->ensureCursorVisible();
    });
}

void GTextArea::setEditable(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqtextedit->setReadOnly(!value);
    });
}

void GTextArea::setHtml(const std::string& html) {
    GThread::runOnQtGuiThread([this, html]() {
        _iqtextedit->setHtml(QString::fromStdString(html));
    });
}

void GTextArea::setPlaceholder(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqtextedit->setPlaceholderText(QString::fromStdString(text));
    });
}

void GTextArea::setRows(int rows) {
    require::nonNegative(rows, "GTextArea::setRows");
    double desiredHeight = getRowColumnSize().getHeight() * rows;
    setPreferredSize(getWidth(), desiredHeight);
    setSize(getWidth(), desiredHeight);
}

void GTextArea::setRowsColumns(int rows, int columns) {
    require::nonNegative2D(rows, columns, "GTextArea::setRowsColumns", "rows", "columns");
    double desiredWidth = getRowColumnSize().getWidth() * columns;
    double desiredHeight = getRowColumnSize().getHeight() * rows;
    setPreferredSize(desiredWidth, desiredHeight);
    setSize(desiredWidth, desiredHeight);
}

void GTextArea::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqtextedit->setText(QString::fromStdString(text));
    });
}

void GTextArea::setKeyListener(GEventListener func) {
    GThread::runOnQtGuiThread([this]() {
        _iqtextedit->setFocusPolicy(Qt::StrongFocus);
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GTextArea::setKeyListener(GEventListenerVoid func) {
    GThread::runOnQtGuiThread([this]() {
        _iqtextedit->setFocusPolicy(Qt::StrongFocus);
    });
    setEventListeners({"keypress",
                       "keyrelease",
                       "keytype"}, func);
}

void GTextArea::setMouseListener(GEventListener func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GTextArea::setMouseListener(GEventListenerVoid func) {
    setEventListeners({"mousepress",
                       "mouserelease"}, func);
}

void GTextArea::setLineWrap(bool wrap) {
    GThread::runOnQtGuiThread([this, wrap]() {
        _iqtextedit->setLineWrapMode(wrap ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    });
}

void GTextArea::setTextChangeListener(GEventListener func) {
    setEventListener("textchange", func);
}

void GTextArea::setTextChangeListener(GEventListenerVoid func) {
    setEventListener("textchange", func);
}


_Internal_QTextEdit::_Internal_QTextEdit(GTextArea* gtextArea, QWidget* parent)
        : QTextEdit(parent),
          _gtextarea(gtextArea) {
    require::nonNull(gtextArea, "_Internal_QTextEdit::constructor");
    setObjectName(QString::fromStdString("_Internal_QTextEdit_" + std::to_string(gtextArea->getID())));
    ensureCursorVisible();
    setTabChangesFocus(false);
    document()->setUndoRedoEnabled(false);
    connect(this, SIGNAL(textChanged()), this, SLOT(handleTextChange()));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(handleScroll(int)));
}

void _Internal_QTextEdit::contextMenuEvent(QContextMenuEvent* event) {
    if (_gtextarea->isContextMenuEnabled()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void _Internal_QTextEdit::handleScroll(int value) {
    if (_gtextarea && _gtextarea->isAcceptingEvent("scroll")) {
        GEvent scrollEvent(
                    /* class  */ SCROLL_EVENT,
                    /* type   */ SCROLL_SCROLLED,
                    /* name   */ "scroll",
                    /* source */ _gtextarea);
        scrollEvent.setActionCommand(_gtextarea->getActionCommand());
        scrollEvent.setY(value);   // approximate
        _gtextarea->fireEvent(scrollEvent);
    }
}

void _Internal_QTextEdit::handleTextChange() {
    if (_gtextarea && _gtextarea->isAcceptingEvent("textchange")) {
        GEvent textChangeEvent(
                    /* class  */ KEY_EVENT,
                    /* type   */ KEY_TYPED,
                    /* name   */ "textchange",
                    /* source */ _gtextarea);
        textChangeEvent.setActionCommand(_gtextarea->getActionCommand());
        _gtextarea->fireEvent(textChangeEvent);
    }
}

void _Internal_QTextEdit::keyPressEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::keyPressEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("keypress")) {
        event->accept();
        _gtextarea->fireGEvent(event, KEY_PRESSED, "keypress");
        if (event->isAccepted()) {
            QTextEdit::keyPressEvent(event);   // call super
        }
    } else {
        QTextEdit::keyPressEvent(event);   // call super
    }
}

void _Internal_QTextEdit::keyReleaseEvent(QKeyEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::keyPressEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("keyrelease")) {
        event->accept();
        _gtextarea->fireGEvent(event, KEY_RELEASED, "keyrelease");
        if (event->isAccepted()) {
            QTextEdit::keyReleaseEvent(event);   // call super
        }
    } else {
        QTextEdit::keyReleaseEvent(event);   // call super
    }
}

void _Internal_QTextEdit::mousePressEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::mousePressEvent", "event");
    if (_gtextarea->isAcceptingEvent("mousepress")) {
        event->accept();
        _gtextarea->fireGEvent(event, MOUSE_PRESSED, "mousepress");
        if (event->isAccepted()) {
            QTextEdit::mousePressEvent(event);   // call super
        }
    } else {
        QTextEdit::mousePressEvent(event);   // call super
    }
}

void _Internal_QTextEdit::mouseReleaseEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QTextEdit::mouseReleaseEvent", "event");
    if (_gtextarea && _gtextarea->isAcceptingEvent("mouserelease")) {
        event->accept();
        _gtextarea->fireGEvent(event, MOUSE_RELEASED, "mouserelease");
        if (event->isAccepted()) {
            QTextEdit::mouseReleaseEvent(event);   // call super
        }
    } else {
        QTextEdit::mouseReleaseEvent(event);   // call super
    }
}

QSize _Internal_QTextEdit::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QTextEdit::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gtextarea.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gscrollpane.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/09/01
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gscrollpane.h"
#define INTERNAL_INCLUDE 1
#include "glayout.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GScrollPane::GScrollPane(GInteractor* interactor, QWidget* parent)
        : _iqscrollarea(nullptr),
          _interactor(interactor),
          _horizontalScrollBarPolicy(GScrollPane::SCROLLBAR_AS_NEEDED),
          _verticalScrollBarPolicy(GScrollPane::SCROLLBAR_AS_NEEDED) {
    require::nonNull(interactor, "GScrollPane::constructor", "interactor");
    GThread::runOnQtGuiThread([this, interactor, parent]() {
        _iqscrollarea = new _Internal_QScrollArea(this, getInternalParent(parent));
        _iqscrollarea->setWidget(interactor->getWidget());
    });
    setInteractorStretch(true);
    setVisible(false);   // all widgets are not shown until added to a window
}

GScrollPane::~GScrollPane() {
    // TODO: delete _iqscrollarea;
    _iqscrollarea = nullptr;
}

GScrollPane::ScrollBarPolicy GScrollPane::getHorizontalScrollBarPolicy() const {
    return _horizontalScrollBarPolicy;
}

GInteractor* GScrollPane::getInteractor() const {
    return _interactor;
}

_Internal_QWidget* GScrollPane::getInternalWidget() const {
    return _iqscrollarea;
}

std::string GScrollPane::getType() const {
    return "GScrollPane";
}

GScrollPane::ScrollBarPolicy GScrollPane::getVerticalScrollBarPolicy() const {
    return _verticalScrollBarPolicy;
}

QWidget* GScrollPane::getWidget() const {
    return static_cast<QWidget*>(_iqscrollarea);
}

bool GScrollPane::isInteractorStretch() const {
    return _iqscrollarea->widgetResizable();
}

void GScrollPane::setHorizontalScrollBarPolicy(ScrollBarPolicy policy) {
    GThread::runOnQtGuiThread([this, policy]() {
        Qt::ScrollBarPolicy qtScrollBarPolicy = toQtScrollBarPolicy(policy);
        _iqscrollarea->setHorizontalScrollBarPolicy(qtScrollBarPolicy);
    });
}

void GScrollPane::setInteractorStretch(bool stretch) {
    GThread::runOnQtGuiThread([this, stretch]() {
        _iqscrollarea->setWidgetResizable(stretch);
        GLayout::forceUpdate(_iqscrollarea);
    });
}

void GScrollPane::setScrollBarPolicy(ScrollBarPolicy policy) {
    setHorizontalScrollBarPolicy(policy);
    setVerticalScrollBarPolicy(policy);
}

void GScrollPane::setVerticalScrollBarPolicy(ScrollBarPolicy policy) {
    GThread::runOnQtGuiThread([this, policy]() {
        Qt::ScrollBarPolicy qtScrollBarPolicy = toQtScrollBarPolicy(policy);
        _iqscrollarea->setVerticalScrollBarPolicy(qtScrollBarPolicy);
    });
}

Qt::ScrollBarPolicy GScrollPane::toQtScrollBarPolicy(ScrollBarPolicy policy) {
    switch (policy) {
        case GScrollPane::SCROLLBAR_ALWAYS:
            return Qt::ScrollBarAlwaysOn;
        case GScrollPane::SCROLLBAR_NEVER:
            return Qt::ScrollBarAlwaysOff;
        case GScrollPane::SCROLLBAR_AS_NEEDED:
        default:
            return Qt::ScrollBarAsNeeded;
    }
}


_Internal_QScrollArea::_Internal_QScrollArea(GScrollPane* gscrollpane, QWidget* parent)
        : QScrollArea(parent)
          /*_gscrollpane(gscrollpane)*/ {
    require::nonNull(gscrollpane, "_Internal_QScrollArea::constructor");
    setObjectName(QString::fromStdString("_Internal_QScrollArea_" + std::to_string(gscrollpane->getID())));
}

QSize _Internal_QScrollArea::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QScrollArea::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gscrollpane.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gdownloader.cpp
 * ---------------------
 * This file implements the GDownloader class as declared in gdownloader.h.
 * See the .h file for the declarations of each member and comments.
 *
 * @author Marty Stepp
 * @version 2018/09/23
 * - added macro checks to improve compatibility with old Qt versions
 * @version 2018/09/18
 * - working version; had to fix various threading / Qt signal issues
 * @version 2018/08/23
 * - renamed to gdownloader.cpp to replace Java version
 * @version 2018/08/03
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gdownloader.h"
#include <iomanip>
#include <iostream>
#include <QtGlobal>
#include <QFile>
#include <QIODevice>
#include <QTimer>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#undef INTERNAL_INCLUDE

GDownloader::GDownloader()
        : _manager(nullptr),
          _reply(nullptr),
          _httpStatusCode(0),
          _downloadComplete(false) {
}

GDownloader::~GDownloader() {
    // TODO: delete
    _manager = nullptr;
    _reply = nullptr;
}

std::string GDownloader::downloadAsString(const std::string& url) {
    _url = url;
    _filename = "";
    _httpStatusCode = 0;
    _lastErrorMessage = "";

    // actually download the file (block/wait for it to finish)
    downloadInternal();

    // save download to string
    saveDownloadedData("downloadAsString");

    // return downloaded text as string (saved in member variable)
    return _filedata;
}

void GDownloader::downloadToFile(const std::string& url, const std::string& file) {
    _url = url;
    _filename = file;
    _httpStatusCode = 0;
    _lastErrorMessage = "";

    // actually download the file (block/wait for it to finish)
    downloadInternal();

    // save download to file
    saveDownloadedData("downloadToFile", file);
}

void GDownloader::downloadInternal() {
    GThread::runOnQtGuiThreadAsync([this]() {
        if (!_manager) {
            _manager = new QNetworkAccessManager();
            // disabling Qt signal-handling because it doesn't seem to work at all
            // connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloaded(QNetworkReply*)));
        }
        QNetworkRequest* request = new QNetworkRequest(QUrl(QString::fromStdString(_url)));

        // set up SSL / HTTPS settings, if possible
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#if QT_CONFIG(ssl)
        request->setSslConfiguration(QSslConfiguration::defaultConfiguration());
#endif // QT_CONFIG(ssl)
#endif // QT_VERSION

        for (std::string headerKey : _headers) {
            request->setRawHeader(QByteArray(headerKey.c_str()), QByteArray(_headers[headerKey].c_str()));
        }

        _reply = _manager->get(*request);

        // disabling Qt signal-handling because it doesn't seem to work at all
        // _reply->connect(
        //         _reply, SIGNAL(finished()),
        //         this, SLOT(fileDownloadFinished()));

        // this seemingly won't be called ever (thanks, useless Qt networking tutorials!)
        _reply->connect(
                _reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(fileDownloadError(QNetworkReply::NetworkError)));

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#if QT_CONFIG(ssl)
        _reply->connect(
                _reply, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
#endif // QT_CONFIG(ssl)
#endif // QT_VERSION
    });

    // wait for download to finish (in student thread)
    waitForDownload();
}

void GDownloader::fileDownloadError(QNetworkReply::NetworkError nerror) {
    error("file download error: " + std::to_string(nerror));
}

std::string GDownloader::getErrorMessage() const {
    return _lastErrorMessage;
}

int GDownloader::getHttpStatusCode() const {
    // all HTTP status codes are between 1xx and 5xx, inclusive
    return _httpStatusCode >= 100 && _httpStatusCode <= 599 ? _httpStatusCode : 0;
}

std::string GDownloader::getHeader(const std::string& name) const {
    return _headers[name];
}

std::string GDownloader::getUserAgent() const {
    if (_headers.containsKey("User-Agent")) {
        return _headers["User-Agent"];
    } else {
        return "";
    }
}

bool GDownloader::hasError() const {
    if (_httpStatusCode != 0) {
        // values 2xx indicate success
        return _httpStatusCode < 200 || _httpStatusCode > 299;
    } else {
        return _lastErrorMessage.empty();
    }
}

std::string GDownloader::qtNetworkErrorToString(QNetworkReply::NetworkError nerror) {
    // http://doc.qt.io/qt-5/qnetworkreply.html#NetworkError-enum
    switch (nerror) {
    case QNetworkReply::ConnectionRefusedError: return "the remote server refused the connection (the server is not accepting requests)";
    case QNetworkReply::RemoteHostClosedError: return "the remote server closed the connection prematurely, before the entire reply was received and processed";
    case QNetworkReply::HostNotFoundError: return "the remote host name was not found (invalid hostname)";
    case QNetworkReply::TimeoutError: return "the connection to the remote server timed out";
    case QNetworkReply::OperationCanceledError: return "the operation was canceled via calls to abort() or close() before it was finished.";
    case QNetworkReply::SslHandshakeFailedError: return "the SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted.";
    case QNetworkReply::TemporaryNetworkFailureError: return "the connection was broken due to disconnection from the network, however the system has initiated roaming to another access point. The request should be resubmitted and will be processed as soon as the connection is re-established.";
    case QNetworkReply::NetworkSessionFailedError: return "the connection was broken due to disconnection from the network or failure to start the network.";
    case QNetworkReply::BackgroundRequestNotAllowedError: return "the background request is not currently allowed due to platform policy.";
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    case QNetworkReply::TooManyRedirectsError: return "while following redirects, the maximum limit was reached. The limit is by default set to 50 or as set by QNetworkRequest::setMaxRedirectsAllowed(). (This value was introduced in 5.6.)";
    case QNetworkReply::InsecureRedirectError: return "while following redirects, the network access API detected a redirect from a encrypted protocol (https) to an unencrypted one (http). (This value was introduced in 5.6.)";
#endif // QT_VERSION
    case QNetworkReply::ProxyConnectionRefusedError: return "the connection to the proxy server was refused (the proxy server is not accepting requests)";
    case QNetworkReply::ProxyConnectionClosedError: return "the proxy server closed the connection prematurely, before the entire reply was received and processed";
    case QNetworkReply::ProxyNotFoundError: return "the proxy host name was not found (invalid proxy hostname)";
    case QNetworkReply::ProxyTimeoutError: return "the connection to the proxy timed out or the proxy did not reply in time to the request sent";
    case QNetworkReply::ProxyAuthenticationRequiredError: return "the proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)";
    case QNetworkReply::ContentAccessDenied: return "access denied";
    case QNetworkReply::ContentOperationNotPermittedError: return "the operation requested on the remote content is not permitted";
    case QNetworkReply::ContentNotFoundError: return "the remote content was not found at the server";
    case QNetworkReply::AuthenticationRequiredError: return "the remote server requires authentication to serve the content but the credentials provided were not accepted (if any)";
    case QNetworkReply::ContentReSendError: return "the request needed to be sent again, but this failed for example because the upload data could not be read a second time.";
    case QNetworkReply::ContentConflictError: return "the request could not be completed due to a conflict with the current state of the resource.";
    case QNetworkReply::ContentGoneError: return "the requested resource is no longer available at the server.";
    case QNetworkReply::InternalServerError: return "the server encountered an unexpected condition which prevented it from fulfilling the request.";
    case QNetworkReply::OperationNotImplementedError: return "the server does not support the functionality required to fulfill the request.";
    case QNetworkReply::ServiceUnavailableError: return "the server is unable to handle the request at this time.";
    case QNetworkReply::ProtocolUnknownError: return "the Network Access API cannot honor the request because the protocol is not known";
    case QNetworkReply::ProtocolInvalidOperationError: return "the requested operation is invalid for this protocol";
    case QNetworkReply::UnknownNetworkError: return "an unknown network-related error was detected";
    case QNetworkReply::UnknownProxyError: return "an unknown proxy-related error was detected";
    case QNetworkReply::UnknownContentError: return "an unknown error related to the remote content was detected";
    case QNetworkReply::ProtocolFailure: return "a breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.)";
    case QNetworkReply::UnknownServerError: return "unknown server error";
    case QNetworkReply::NoError:
        break;
    }
    return "";
}

void GDownloader::saveDownloadedData(const std::string& member, const std::string& filename) {
    if (_reply) {
        QNetworkReply::NetworkError nerror = _reply->error();
        if (nerror) {
            // connection failed; log the error message
            _lastErrorMessage = qtNetworkErrorToString(nerror);
        } else if (filename.empty()) {
            // save to a string
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
            _filedata = _reply->readAll().toStdString();
#endif // QT_VERSION
        } else {
            // save to a file
            QFile outfile(QString::fromStdString(filename));
            if (!outfile.open(QIODevice::WriteOnly)) {
                error("GDownloader::" + member + ": cannot open file " + filename + " for writing");
            }
            outfile.write(_reply->readAll());
            outfile.close();
        }

        // clean up the connection
        _reply->deleteLater();
        _reply = nullptr;
        _downloadComplete = true;
    }
}

void GDownloader::setHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void GDownloader::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}

void GDownloader::sslErrors(QList<QSslError>) {
    std::cout << "  DEBUG: sslErrors" << std::endl;
}

void GDownloader::waitForDownload() {
    // wait for download to finish
    while (!_downloadComplete) {
        GThread::sleep(10);
        if (_reply && _reply->isFinished()) {
            _downloadComplete = true;
            break;
        }
    }

    // grab the HTTP status code
    QVariant statusCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.isValid()) {
        _httpStatusCode = statusCode.toInt();
        _lastErrorMessage = _reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString().toStdString();
    } else {
        _httpStatusCode = -1;
        _lastErrorMessage = "Unable to connect to URL";
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gdownloader.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: GSlider.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gslider.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gslider.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#undef INTERNAL_INCLUDE

const int GSlider::DEFAULT_MIN_VALUE = 0;
const int GSlider::DEFAULT_MAX_VALUE = 100;
const int GSlider::DEFAULT_INITIAL_VALUE = 50;

GSlider::GSlider(int min, int max, int value, QWidget* parent) {
    require::require(min <= max, "GSlider::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this,
                                          Qt::Horizontal,
                                          getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::GSlider(Orientation orientation, int min, int max, int value, QWidget* parent) {
    require::require(min <= max, "GSlider::constructor", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::constructor", "value");
    GThread::runOnQtGuiThread([this, orientation, min, max, value, parent]() {
        _iqslider = new _Internal_QSlider(this,
                                          orientation == HORIZONTAL ? Qt::Horizontal : Qt::Vertical,
                                          getInternalParent(parent));
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSlider::~GSlider() {
    // TODO: delete _iqslider;
    _iqslider = nullptr;
}

_Internal_QWidget* GSlider::getInternalWidget() const {
    return _iqslider;
}

int GSlider::getMajorTickSpacing() const {
    return _iqslider->tickInterval();
}

int GSlider::getMax() const {
    return _iqslider->maximum();
}

int GSlider::getMin() const {
    return _iqslider->minimum();
}

int GSlider::getMinorTickSpacing() const {
    return _iqslider->tickInterval();
}

GSlider::Orientation GSlider::getOrientation() const {
    return _iqslider->orientation() == Qt::Horizontal ? HORIZONTAL : VERTICAL;
}

bool GSlider::getPaintLabels() const {
    // TODO
    return false;
}

bool GSlider::getPaintTicks() const {
    // TODO
    return true;
}

bool GSlider::getSnapToTicks() const {
    // TODO
    return true;
}

std::string GSlider::getType() const {
    return "GSlider";
}

int GSlider::getValue() const {
    return _iqslider->value();
}

QWidget* GSlider::getWidget() const {
    return static_cast<QWidget*>(_iqslider);
}

void GSlider::removeActionListener() {
    removeEventListener("change");
}

void GSlider::setActionListener(GEventListener func) {
    setEventListener("change", func);
}

void GSlider::setActionListener(GEventListenerVoid func) {
    setEventListener("change", func);
}

void GSlider::setMajorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setMax(int max) {
    int min = getMin();
    require::require(min <= max, "GSlider::setMax", "max (" + std::to_string(max) + ") cannot be less than min (" + std::to_string(min) + ")");
    GThread::runOnQtGuiThread([this, max]() {
        _iqslider->setMaximum(max);
    });
}

void GSlider::setMin(int min) {
    int max = getMax();
    require::require(min <= max, "GSlider::setMin", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min]() {
        _iqslider->setMinimum(min);
    });
}

void GSlider::setMinorTickSpacing(int value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickInterval(value);
    });
}

void GSlider::setPaintLabels(bool /* value */) {
    // not supported
}

void GSlider::setPaintTicks(bool value) {
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setTickPosition(value ? QSlider::TicksBothSides : QSlider::NoTicks);
    });
}

void GSlider::setRange(int min, int max) {
    require::require(min <= max, "GSlider::setRange", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    GThread::runOnQtGuiThread([this, min, max]() {
        _iqslider->setRange(min, max);
    });
}

void GSlider::setSnapToTicks(bool /* value */) {
    // TODO
}

void GSlider::setState(int min, int max, int value) {
    require::require(min <= max, "GSlider::setState", "min (" + std::to_string(min) + ") cannot be greater than max (" + std::to_string(max) + ")");
    require::inRange(value, min, max, "GSlider::setState", "value");
    GThread::runOnQtGuiThread([this, min, max, value]() {
        _iqslider->setRange(min, max);
        _iqslider->setValue(value);
    });
}

void GSlider::setValue(int value) {
    require::inRange(value, getMin(), getMax(), "GSlider::setValue", "value");
    GThread::runOnQtGuiThread([this, value]() {
        _iqslider->setValue(value);
    });
}


_Internal_QSlider::_Internal_QSlider(GSlider* gslider, Qt::Orientation orientation, QWidget* parent)
        : QSlider(orientation, parent),
          _gslider(gslider) {
    require::nonNull(gslider, "_Internal_QSlider::constructor");
    setObjectName(QString::fromStdString("_Internal_QSlider_" + std::to_string(gslider->getID())));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleChange(int)));
}

void _Internal_QSlider::handleChange(int /* value */) {
    GEvent changeEvent(
                /* class  */ CHANGE_EVENT,
                /* type   */ STATE_CHANGED,
                /* name   */ "change",
                /* source */ _gslider);
    changeEvent.setActionCommand(_gslider->getActionCommand());
    _gslider->fireEvent(changeEvent);
}

QSize _Internal_QSlider::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QSlider::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gslider.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: gcolor.cpp
 * ----------------
 *
 * @author Marty Stepp
 * @version 2018/09/16
 * - added splitRGB/ARGB, hasAlpha; better ARGB support
 * @version 2018/08/23
 * - renamed to gcolor.cpp to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gcolor.h"
#include <iomanip>
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

Map<std::string, int> GColor::_colorTable;
Map<std::string, std::string> GColor::_colorNameTable;

GColor::GColor() {
    // empty
}

std::string GColor::canonicalColorName(const std::string& str) {
    std::string result = "";
    int nChars = static_cast<int>(str.length());
    for (int i = 0; i < nChars; i++) {
        char ch = str[i];
        if (!isspace(ch) && ch != '_') result += tolower(ch);
    }
    return result;
}

const Map<std::string, int>& GColor::colorTable() {
    if (_colorTable.isEmpty()) {
        _colorTable["black"] = 0x000000;
        _colorTable["blue"] = 0x0000FF;
        _colorTable["brown"] = 0x926239;
        _colorTable["cyan"] = 0x00FFFF;
        _colorTable["darkgray"] = 0x595959;
        _colorTable["gray"] = 0x999999;
        _colorTable["green"] = 0x00FF00;
        _colorTable["lightgray"] = 0xBFBFBF;
        _colorTable["magenta"] = 0xFF00FF;
        _colorTable["orange"] = 0xFFC800;
        _colorTable["pink"] = 0xFFAFAF;
        _colorTable["purple"] = 0xFF00FF;
        _colorTable["red"] = 0xFF0000;
        _colorTable["white"] = 0xFFFFFF;
        _colorTable["yellow"] = 0xFFFF00;
    }
    return _colorTable;
}

const Map<std::string, std::string>& GColor::colorNameTable() {
    if (_colorNameTable.isEmpty()) {
        _colorNameTable["#000000"] = "black";
        _colorNameTable["#ff000000"] = "black";
        _colorNameTable["#0000ff"] = "blue";
        _colorNameTable["#ff0000ff"] = "blue";
        _colorNameTable["#926239"] = "brown";
        _colorNameTable["#ff926239"] = "brown";
        _colorNameTable["#00ffff"] = "cyan";
        _colorNameTable["#ff00ffff"] = "cyan";
        _colorNameTable["#595959"] = "darkgray";
        _colorNameTable["#ff595959"] = "darkgray";
        _colorNameTable["#999999"] = "gray";
        _colorNameTable["#ff999999"] = "gray";
        _colorNameTable["#00ff00"] = "green";
        _colorNameTable["#ff00ff00"] = "green";
        _colorNameTable["#bfbfbf"] = "lightgray";
        _colorNameTable["#ffbfbfbf"] = "lightgray";
        _colorNameTable["#ff00ff"] = "magenta";
        _colorNameTable["#ffff00ff"] = "magenta";
        _colorNameTable["#ffc800"] = "orange";
        _colorNameTable["#ffffc800"] = "orange";
        _colorNameTable["#ffafaf"] = "pink";
        _colorNameTable["#ffffafaf"] = "pink";
        _colorNameTable["#ff00ff"] = "purple";
        _colorNameTable["#ffff00ff"] = "purple";
        _colorNameTable["#ff0000"] = "red";
        _colorNameTable["#ffff0000"] = "red";
        _colorNameTable["#ffffff"] = "white";
        _colorNameTable["#ffffffff"] = "white";
        _colorNameTable["#ffff00"] = "yellow";
        _colorNameTable["#ffffff00"] = "yellow";
    }
    return _colorNameTable;
}

int GColor::convertARGBToARGB(int a, int r, int g, int b) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

std::string GColor::convertARGBToColor(int a, int r, int g, int b) {
    if (a < 0 || a > 255 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        error("GColor::convertARGBToColor: invalid ARGB value (must be 0-255)");
    }
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (a & 0xFF);
    os << std::setw(2) << (r & 0xFF);
    os << std::setw(2) << (g & 0xFF);
    os << std::setw(2) << (b & 0xFF);
    return os.str();
}

std::string GColor::convertARGBToColor(int argb) {
    int a, r, g, b;
    splitARGB(argb, a, r, g, b);
    return convertARGBToColor(a, r, g, b);
}

int GColor::convertColorToARGB(const std::string& colorName) {
    return convertColorToRGB(colorName);
}

int GColor::convertColorToRGB(const std::string& colorName) {
    if (colorName == "") return -1;
    if (colorName[0] == '#') {
        std::istringstream is(colorName.substr(1) + "@");
        unsigned int rgb;
        char terminator = '\0';
        is >> std::hex >> rgb >> terminator;
        if (terminator != '@') {
            error("GColor::convertColorToRGB: Illegal color - \"" + colorName + "\"");
        }
        return static_cast<int>(rgb & 0xffffffff);
    }
    std::string name = canonicalColorName(colorName);
    if (!colorTable().containsKey(name)) {
        error("GColor::convertColorToRGB: Undefined color - \"" + colorName + "\"");
    }
    return colorTable()[name];
}

std::string GColor::convertQColorToColor(const QColor& color) {
    return convertRGBToColor(color.red(), color.green(), color.blue());
}

int GColor::convertQColorToRGB(const QColor& color) {
    return convertRGBToRGB(color.red(), color.green(), color.blue());
}

std::string GColor::convertRGBToColor(int rgb) {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (rgb >> 16 & 0xFF);
    os << std::setw(2) << (rgb >> 8 & 0xFF);
    os << std::setw(2) << (rgb & 0xFF);
    std::string color = os.str();
    if (colorNameTable().containsKey(toLowerCase(color))) {
        return colorNameTable()[toLowerCase(color)];
    } else {
        return color;
    }
}

std::string GColor::convertRGBToColor(int r, int g, int b) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        error("GColor::convertRGBToColor: invalid RGB value (must be 0-255)");
    }
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (r & 0xFF);
    os << std::setw(2) << (g & 0xFF);
    os << std::setw(2) << (b & 0xFF);
    std::string color = os.str();
    if (colorNameTable().containsKey(toLowerCase(color))) {
        return colorNameTable()[toLowerCase(color)];
    } else {
        return color;
    }
}

int GColor::convertRGBToRGB(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

bool GColor::hasAlpha(const std::string& color) {
    return static_cast<int>(color.length()) == 9
            && color[0] == '#';
}

void GColor::splitARGB(int argb, int& a, int& r, int& g, int& b) {
    a = ((static_cast<unsigned int>(argb) & 0xff000000) >> 24) & 0x000000ff;
    r = (argb & 0x00ff0000) >> 16;
    g = (argb & 0x0000ff00) >> 8;
    b = (argb & 0x000000ff);
}

void GColor::splitRGB(int rgb, int& r, int& g, int& b) {
    r = (rgb & 0x00ff0000) >> 16;
    g = (rgb & 0x0000ff00) >> 8;
    b = (rgb & 0x000000ff);
}

QColor GColor::toQColor(const std::string& color) {
    if (hasAlpha(color)) {
        int argb = convertColorToARGB(color);
        int a, r, g, b;
        splitARGB(argb, a, r, g, b);
        return QColor(r, g, b, a);
    } else {
        int rgb = convertColorToRGB(color);
        return QColor(rgb | 0xff000000);
    }
}

QColor GColor::toQColorARGB(int argb) {
    int a, r, g, b;
    splitARGB(argb, a, r, g, b);
    return QColor(r, g, b, a);
}

int GColor::fixAlpha(int argb) {
    int alpha = ((argb & 0xff000000) >> 24) & 0x000000ff;
    if (alpha == 0 && (argb & 0x00ffffff) != 0) {
        argb = argb | 0xff000000;   // set full 255 alpha
    }
    return argb;
}

/*
 * File: gspacer.cpp
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/10/06
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gspacer.h"
#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GSpacer::GSpacer(double width, double height, QWidget* parent)
        : _iqspacer(nullptr) {
    require::nonNegative2D(width, height, "GSpacer::constructor", "width", "height");
    GThread::runOnQtGuiThread([this, width, height, parent]() {
        _iqspacer = new _Internal_QSpacer(this, width, height, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GSpacer::~GSpacer() {
    // TODO: delete _iqscrollarea;
    _iqspacer = nullptr;
}

_Internal_QWidget* GSpacer::getInternalWidget() const {
    return _iqspacer;
}

std::string GSpacer::getType() const {
    return "GSpacer";
}

QWidget* GSpacer::getWidget() const {
    return static_cast<QWidget*>(_iqspacer);
}


_Internal_QSpacer::_Internal_QSpacer(GSpacer* gspacer, double width, double height, QWidget* parent)
        : QWidget(parent) {
    require::nonNull(gspacer, "_Internal_QSpacer::constructor");
    setObjectName(QString::fromStdString("_Internal_QSpacer_" + std::to_string(gspacer->getID())));
    setFixedSize(static_cast<int>(width), static_cast<int>(height));
}

QSize _Internal_QSpacer::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#define INTERNAL_INCLUDE 1
#include "moc_gspacer.cpp"   // speeds up compilation of auto-generated Qt files
#undef INTERNAL_INCLUDE
#endif // SPL_PRECOMPILE_QT_MOC_FILES

/*
 * File: basicgraph.cpp
 * --------------------
 * This file implements any non-template functionality used by
 * the BasicGraph class.
 *
 * @version 2016/12/01
 */

#define INTERNAL_INCLUDE 1
#include "basicgraph.h"
#undef INTERNAL_INCLUDE

int hashCode(const BasicGraph& graph) {
    int code = hashSeed();
    for (Vertex* v : graph) {
        code = hashMultiplier() * code + hashCode(v->name);
    }
    for (Edge* e : graph.getEdgeSet()) {
        code = hashMultiplier() * code + hashCode(e->start->name);
        code = hashMultiplier() * code + hashCode(e->finish->name);
    }
    return (code & hashMask());
}

/*
 * File: shuffle.cpp
 * -----------------
 * Implementation of the functions in shuffle.h.
 * See shuffle.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/02/01
 */

#define INTERNAL_INCLUDE 1
#include "shuffle.h"
#undef INTERNAL_INCLUDE

/*
 * Randomly rearranges the characters of the given string and returns the
 * rearranged version.
 */
std::string shuffle(std::string s) {
    for (int i = 0, length = s.length(); i < length; i++) {
        int j = randomInteger(i, length - 1);
        if (i != j) {
            std::string::value_type temp = s[i];
            s[i] = s[j];
            s[j] = temp;
        }
    }
    return s;
}

/*
 * File: stl.cpp
 * -------------
 * Implements bodies of functions declared in stl.h.
 *
 * @author Marty Stepp
 * @version 2018/10/19
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "stl.h"
#undef INTERNAL_INCLUDE

std::set<std::string> toStlSet(const DawgLexicon& lex) {
    std::set<std::string> result;
    for (const std::string& s : lex) {
        result.insert(s);
    }
    return result;
}

/**
 * Returns an STL set object with the same elements as this Lexicon.
 */
std::set<std::string> toStlSet(const Lexicon& lex) {
    std::set<std::string> result;
    for (const std::string& s : lex) {
        result.insert(s);
    }
    return result;
}

/*
 * File: gridlocation.cpp
 * ----------------------
 * This file implements the members of the <code>GridLocation</code> structure
 * and the <code>GridLocationRange</code> class.
 * See gridlocation.h for the declarations of each member.
 *
 * @version 2018/03/12
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gridlocation.h"
#include <sstream>
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE

GridLocation::GridLocation(int row, int col) {
    this->row = row;
    this->col = col;
}

GridLocationRange GridLocation::neighbors(int range, bool rowMajor) const {
    return GridLocationRange(row - range, col - range, row + range, col + range, rowMajor);
}

std::string GridLocation::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

int hashCode(const GridLocation& loc) {
    return hashCode(loc.row, loc.col);
}

bool operator <(const GridLocation& loc1, const GridLocation& loc2) {
    return loc1.row < loc2.row ||
            (loc1.row == loc2.row && loc1.col < loc2.col);
}

bool operator <=(const GridLocation& loc1, const GridLocation& loc2) {
    return loc1 < loc2 || loc1 == loc2;
}

bool operator ==(const GridLocation& loc1, const GridLocation& loc2) {
    return loc1.row == loc2.row && loc1.col == loc2.col;
}

bool operator !=(const GridLocation& loc1, const GridLocation& loc2) {
    return !(loc1 == loc2);
}

bool operator >(const GridLocation& loc1, const GridLocation& loc2) {
    return loc2 < loc1;
}

bool operator >=(const GridLocation& loc1, const GridLocation& loc2) {
    return !(loc1 < loc2);
}

std::ostream& operator <<(std::ostream& out, const GridLocation& loc) {
    return out << "r" << loc.row << "c" << loc.col;
}

std::istream& operator >>(std::istream& input, GridLocation& loc) {
    // read 'r'
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read row
    int row;
    if (!(input >> row)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read 'c'
    input.get();
    if (!input) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // read row
    int col;
    if (!(input >> col)) {
        input.setstate(std::ios_base::failbit);
        return input;
    }

    // success!
    loc.row = row;
    loc.col = col;
    return input;
}

GridLocationRange::GridLocationRange(int startRow, int startCol, int endRow, int endCol, bool isRowMajor)
        : _start(startRow, startCol),
          _end(endRow, endCol),
          _isRowMajor(isRowMajor) {
    // empty
}

GridLocationRange::GridLocationRange(const GridLocation& startLoc, const GridLocation& endLoc, bool isRowMajor)
        : _start(startLoc),
          _end(endLoc),
          _isRowMajor(isRowMajor) {
    // empty
}

GridLocationRange::GridLocationRangeIterator GridLocationRange::begin() const {
    return GridLocationRangeIterator(this, /* end */ false);
}

bool GridLocationRange::contains(const GridLocation& loc) const {
    return _start <= loc && loc <= _end;
}

GridLocationRange::GridLocationRangeIterator GridLocationRange::end() const {
    return GridLocationRangeIterator(this, /* end */ true);
}

int GridLocationRange::endCol() const {
    return _end.col;
}

const GridLocation& GridLocationRange::endLocation() const {
    return _end;
}

int GridLocationRange::endRow() const {
    return _end.row;
}

bool GridLocationRange::isEmpty() const {
    return _start > _end;
}

bool GridLocationRange::isRowMajor() const {
    return _isRowMajor;
}

int GridLocationRange::startCol() const {
    return _start.col;
}

const GridLocation& GridLocationRange::startLocation() const {
    return _start;
}

int GridLocationRange::startRow() const {
    return _start.row;
}

std::string GridLocationRange::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& out, const GridLocationRange& range) {
    return out << range.startLocation() << " .. " << range.endLocation();
}

/*
 * File: collections.cpp
 * ---------------------
 * This file implements the collections.h interface.
 * 
 * @version 2018/10/20
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE
#include <iomanip>
#include <iostream>

/*
 * Implementation notes: readQuotedString and writeQuotedString
 * ------------------------------------------------------------
 * Most of the work in these functions has to do with escape sequences.
 */

STATIC_CONST_VARIABLE_DECLARE(std::string, STRING_DELIMITERS, ",:)}]\n")

bool stringNeedsQuoting(const std::string& str) {
    int n = str.length();
    for (int i = 0; i < n; i++) {
        char ch = str[i];
        if (isspace(ch)) return false;
        if (STATIC_VARIABLE(STRING_DELIMITERS).find(ch) != std::string::npos) return true;
    }
    return false;
}

bool readQuotedString(std::istream& is, std::string& str, bool throwOnError) {
    str = "";
    char ch;
    while (is.get(ch) && isspace(ch)) {
        /* Empty */
    }
    if (is.fail()) {
        return true;   // empty string?
    }
    if (ch == '\'' || ch == '"') {
        char delim = ch;
        while (is.get(ch) && ch != delim) {
            if (is.fail()) {
                if (throwOnError) {
                    error("Unterminated string");
                }
                return false;
            }
            if (ch == '\\') {
                if (!is.get(ch)) {
                    if (throwOnError) {
                        error("Unterminated string");
                    }
                    is.setstate(std::ios_base::failbit);
                    return false;
                }
                if (isdigit(ch) || ch == 'x') {
                    int maxDigits = 3;
                    int base = 8;
                    if (ch == 'x') {
                        base = 16;
                        maxDigits = 2;
                    }
                    int result = 0;
                    int digit = 0;
                    for (int i = 0; i < maxDigits && ch != delim; i++) {
                        if (isdigit(ch)) {
                            digit = ch - '0';
                        } else if (base == 16 && isxdigit(ch)) {
                            digit = toupper(ch) - 'A' + 10;
                        } else {
                            break;
                        }
                        result = base * result + digit;
                        if (!is.get(ch)) {
                            if (throwOnError) {
                                error("Unterminated string");
                            }
                            is.setstate(std::ios_base::failbit);
                            return false;
                        }
                    }
                    ch = char(result);
                    is.unget();
                } else {
                    switch (ch) {
                    case 'a': ch = '\a'; break;
                    case 'b': ch = '\b'; break;
                    case 'f': ch = '\f'; break;
                    case 'n': ch = '\n'; break;
                    case 'r': ch = '\r'; break;
                    case 't': ch = '\t'; break;
                    case 'v': ch = '\v'; break;
                    case '"': ch = '"'; break;
                    case '\'': ch = '\''; break;
                    case '\\': ch = '\\'; break;
                    }
                }
            }
            str += ch;
        }
    } else {
        str += ch;
        int endTrim = 0;
        while (is.get(ch) && STATIC_VARIABLE(STRING_DELIMITERS).find(ch) == std::string::npos) {
            str += ch;
            if (!isspace(ch)) endTrim = str.length();
        }
        if (is) is.unget();
        str = str.substr(0, endTrim);
    }
    return true;   // read successfully
}

std::ostream& writeQuotedString(std::ostream& os, const std::string& str, bool forceQuotes) {
    if (!forceQuotes && stringNeedsQuoting(str)) {
        forceQuotes = true;
    }
    if (forceQuotes) {
        os << '"';
    }
    int len = str.length();
    for (int i = 0; i < len; i++) {
        char ch = str.at(i);
        switch (ch) {
        case '\a': os << "\\a"; break;
        case '\b': os << "\\b"; break;
        case '\f': os << "\\f"; break;
        case '\n': os << "\\n"; break;
        case '\r': os << "\\r"; break;
        case '\t': os << "\\t"; break;
        case '\v': os << "\\v"; break;
        case '\\': os << "\\\\"; break;
        default:
            if (isprint(ch) && ch != '"') {
                os << ch;
            } else {
                std::ostringstream oss;
                oss << std::oct << std::setw(3) << std::setfill('0') << (int(ch) & 0xFF);
                os << "\\" << oss.str();
            }
        }
    }
    if (forceQuotes) {
        os << '"';
    }
    return os;
}

/*
 * File: dawglexicon.cpp
 * ---------------------
 * A lexicon is a word list. This lexicon is backed by two separate data
 * structures for storing the words in the list:
 *
 * 1) a DAWG (directed acyclic word graph)
 * 2) a Set<string> of other words.
 *
 * Typically the DAWG is used for a large list read from a file in binary
 * format.  The STL set is for words added piecemeal at runtime.
 *
 * The DAWG idea comes from an article by Appel & Jacobson, CACM May 1988.
 * This lexicon implementation only has the code to load/search the DAWG.
 * The DAWG builder code is quite a bit more intricate, see Julie Zelenski
 * if you need it.
 * 
 * @version 2018/03/10
 * - added method front
 * @version 2017/11/14
 * - added iterator version checking support
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {"a", "b", "c"}
 * @version 2016/08/04
 * - added operator >>
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added hashCode function
 * @version 2014/10/10
 * - removed 'using namespace' statement
 * - added equals method, ==, != operators
 * - fixed inclusion of foreach macro to avoid errors
 * - BUGFIX: operator << now shows "" marks around words to match Lexicon
 */

#define INTERNAL_INCLUDE 1
#include "dawglexicon.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

static uint32_t my_ntohl(uint32_t arg);

/*
 * The DAWG is stored as an array of edges. Each edge is represented by
 * one 32-bit struct.  The 5 "letter" bits indicate the character on this
 * transition (expressed as integer from 1 to 26), the  "accept" bit indicates
 * if you accept after appending that char (current path forms word), and the
 * "lastEdge" bit marks this as the last edge in a sequence of childeren.
 * The bulk of the bits (24) are used for the index within the edge array for
 * the children of this node. The children are laid out contiguously in
 * alphabetical order.  Since we read edges as binary bits from a file in
 * a big-endian format, we have to swap the struct order for little-endian
 * machines.
 */

DawgLexicon::DawgLexicon() :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    // empty
}

DawgLexicon::DawgLexicon(std::istream& input) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    addWordsFromFile(input);
}

DawgLexicon::DawgLexicon(const std::string& filename) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    addWordsFromFile(filename);
}

DawgLexicon::DawgLexicon(const DawgLexicon& src) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    deepCopy(src);
}

DawgLexicon::DawgLexicon(std::initializer_list<std::string> list) :
        edges(nullptr),
        start(nullptr),
        numEdges(0),
        numDawgWords(0) {
    addAll(list);
}

DawgLexicon::~DawgLexicon() {
    if (edges) {
        delete[] edges;
    }
}

void DawgLexicon::add(const std::string& word) {
    std::string copy = word;
    toLowerCaseInPlace(copy);
    if (!contains(copy)) {
        otherWords.add(copy);
    }
}

DawgLexicon& DawgLexicon::addAll(const DawgLexicon& lex) {
    for (const std::string& word : lex) {
        add(word);
    }
    return *this;
}

DawgLexicon& DawgLexicon::addAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        add(word);
    }
    return *this;
}

/*
 * Check for DAWG in first 4 to identify as special binary format,
 * otherwise assume ASCII, one word per line
 */
void DawgLexicon::addWordsFromFile(std::istream& input) {
    char firstFour[4], expected[] = "DAWG";
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't read input");
    }
    input.read(firstFour, 4);
    if (strncmp(firstFour, expected, 4) == 0) {
        if (otherWords.size() != 0) {
            error("DawgLexicon::addWordsFromFile: Binary files require an empty lexicon");
        }
        readBinaryFile(input);
    } else {
        // plain text file
        input.seekg(0);
        std::string line;
        while (getline(input, line)) {
            add(line);
        }
    }
}

/*
 * Check for DAWG in first 4 to identify as special binary format,
 * otherwise assume ASCII, one word per line
 */
void DawgLexicon::addWordsFromFile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
    }
    addWordsFromFile(input);
    input.close();
}

void DawgLexicon::clear() {
    if (edges) {
        delete[] edges;
    }
    edges = start = nullptr;
    numEdges = numDawgWords = 0;
    otherWords.clear();
}

bool DawgLexicon::contains(const std::string& word) const {
    std::string copy = word;
    toLowerCaseInPlace(copy);
    Edge* lastEdge = traceToLastEdge(copy);
    if (lastEdge && lastEdge->accept) {
        return true;
    }
    return otherWords.contains(copy);
}

bool DawgLexicon::containsAll(const DawgLexicon& lex2) const {
    for (const std::string& word : lex2) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool DawgLexicon::containsAll(std::initializer_list<std::string> list) const {
    for (const std::string& word : list) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool DawgLexicon::containsPrefix(const std::string& prefix) const {
    if (prefix.empty()) {
        return true;
    }
    std::string copy = prefix;
    toLowerCaseInPlace(copy);
    if (traceToLastEdge(copy)) {
        return true;
    }
    for (std::string word : otherWords) {
        if (startsWith(word, copy)) {
            return true;
        }
        if (copy < word) {
            return false;
        }
    }
    return false;
}

bool DawgLexicon::equals(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::equals(*this, lex2);
}

std::string DawgLexicon::front() const {
    if (isEmpty()) {
        error("DawgLexicon::front: lexicon is empty");
    }
    auto it = begin();
    return *it;
}

void DawgLexicon::insert(const std::string& word) {
    add(word);
}

bool DawgLexicon::isEmpty() const {
    return size() == 0;
}

bool DawgLexicon::isSubsetOf(const DawgLexicon& lex2) const {
    auto it = begin();
    auto end = this->end();
    while (it != end) {
        if (!lex2.contains(*it)) {
            return false;
        }
        ++it;
    }
    return true;
}

bool DawgLexicon::isSubsetOf(std::initializer_list<std::string> list) const {
    DawgLexicon lex2(list);
    return isSubsetOf(lex2);
}

bool DawgLexicon::isSupersetOf(const DawgLexicon& lex2) const {
    return containsAll(lex2);
}

bool DawgLexicon::isSupersetOf(std::initializer_list<std::string> list) const {
    return containsAll(list);
}

void DawgLexicon::mapAll(void (*fn)(std::string)) const {
    for (std::string word : *this) {
        fn(word);
    }
}

void DawgLexicon::mapAll(void (*fn)(const std::string &)) const {
    for (std::string word : *this) {
        fn(word);
    }
}

int DawgLexicon::size() const {
    return numDawgWords + otherWords.size();
}

std::string DawgLexicon::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

/*
 * Operators
 */
bool DawgLexicon::operator ==(const DawgLexicon& lex2) const {
    return equals(lex2);
}

bool DawgLexicon::operator !=(const DawgLexicon& lex2) const {
    return !equals(lex2);
}

bool DawgLexicon::operator <(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) < 0;
}

bool DawgLexicon::operator <=(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) <= 0;
}

bool DawgLexicon::operator >(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) > 0;
}

bool DawgLexicon::operator >=(const DawgLexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) >= 0;
}

DawgLexicon DawgLexicon::operator +(const DawgLexicon& lex2) const {
    DawgLexicon lex = *this;
    lex.addAll(lex2);
    return lex;
}

DawgLexicon DawgLexicon::operator +(std::initializer_list<std::string> list) const {
    DawgLexicon lex = *this;
    lex.addAll(list);
    return lex;
}

DawgLexicon DawgLexicon::operator +(const std::string& word) const {
    DawgLexicon lex = *this;
    lex.add(word);
    return lex;
}

DawgLexicon& DawgLexicon::operator +=(const DawgLexicon& lex2) {
    return addAll(lex2);
}

DawgLexicon& DawgLexicon::operator +=(std::initializer_list<std::string> list) {
    return addAll(list);
}

DawgLexicon& DawgLexicon::operator +=(const std::string& word) {
    add(word);
    return *this;
}

/*
 * Private methods and helpers
 */

DawgLexicon& DawgLexicon::operator ,(const std::string& word) {
    add(word);
    return *this;
}

int DawgLexicon::countDawgWords(Edge* ep) const {
    int count = 0;
    while (true) {
        if (ep->accept) count++;
        if (ep->children != 0) {
            count += countDawgWords(&edges[ep->children]);
        }
        if (ep->lastEdge) break;
        ep++;
    }
    return count;
}

void DawgLexicon::deepCopy(const DawgLexicon& src) {
    if (!src.edges) {
        edges = nullptr;
        start = nullptr;
    } else {
        numEdges = src.numEdges;
        edges = new Edge[src.numEdges];
        memcpy(edges, src.edges, sizeof(Edge)*src.numEdges);
        start = edges + (src.start - src.edges);
    }
    numDawgWords = src.numDawgWords;
    otherWords = src.otherWords;
}

/*
 * Implementation notes: findEdgeForChar
 * -------------------------------------
 * Iterate over sequence of children to find one that
 * matches the given char.  Returns nullptr if we get to
 * last child without finding a match (thus no such
 * child edge exists).
 */
DawgLexicon::Edge* DawgLexicon::findEdgeForChar(Edge* children, char ch) const {
    Edge* curEdge = children;
    while (true) {
        if (curEdge->letter == charToOrd(ch)) {
            return curEdge;
        }
        if (curEdge->lastEdge) {
            return nullptr;
        }
        curEdge++;
    }
}

/*
 * Implementation notes: readBinaryFile
 * ------------------------------------
 * The binary lexicon file format must follow this pattern:
 * DAWG:<startnode index>:<num bytes>:<num bytes block of edge data>
 */
void DawgLexicon::readBinaryFile(std::istream& input) {
    input.clear();
    input.seekg(0, std::ios::beg);
    long startIndex, numBytes;
    char firstFour[4], expected[] = "DAWG";
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't read input");
    }
    input.read(firstFour, 4);
    input.get();
    input >> startIndex;
    input.get();
    input >> numBytes;
    input.get();
    if (input.fail() || strncmp(firstFour, expected, 4) != 0
            || startIndex < 0 || numBytes < 0) {
        error("DawgLexicon::addWordsFromFile: Improperly formed lexicon file");
    }
    numEdges = numBytes / sizeof(Edge);
    edges = new Edge[numEdges];
    start = &edges[startIndex];
    input.read((char*) edges, numBytes);
    if (input.fail() && !input.eof()) {
        error("DawgLexicon::addWordsFromFile: Improperly formed lexicon file");
    }

#if defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN
    // uint32_t* cur = (uint32_t*) edges;
    uint32_t* cur = reinterpret_cast<uint32_t*>(edges);
    for (int i = 0; i < numEdges; i++, cur++) {
        *cur = my_ntohl(*cur);
    }
#endif

    numDawgWords = countDawgWords(start);
}

/*
 * Implementation notes: readBinaryFile
 * ------------------------------------
 * The binary lexicon file format must follow this pattern:
 * DAWG:<startnode index>:<num bytes>:<num bytes block of edge data>
 */
void DawgLexicon::readBinaryFile(const std::string& filename) {
#ifdef _foreachpatch_h
    std::ifstream input(filename.c_str(), __IOS_IN__ | __IOS_BINARY__);
#else
    std::ifstream input(filename.c_str(), std::ios::in | std::ios::binary);
#endif // _foreachpatch_h
    if (input.fail()) {
        error("DawgLexicon::addWordsFromFile: Couldn't open lexicon file " + filename);
    }
    readBinaryFile(input);
    input.close();
}

/*
 * Implementation notes: traceToLastEdge
 * -------------------------------------
 * Given a string, trace out path through the DAWG edge-by-edge.
 * If a path exists, return last edge; otherwise return nullptr.
 */

DawgLexicon::Edge* DawgLexicon::traceToLastEdge(const std::string& s) const {
    if (!start) {
        return nullptr;
    }
    Edge* curEdge = findEdgeForChar(start, s[0]);
    int len = (int) s.length();
    for (int i = 1; i < len; i++) {
        if (!curEdge || !curEdge->children) {
            return nullptr;
        }
        curEdge = findEdgeForChar(&edges[curEdge->children], s[i]);
    }
    return curEdge;
}

DawgLexicon& DawgLexicon::operator =(const DawgLexicon& src) {
    if (this != &src) {
        if (edges) {
            delete[] edges;
        }
        deepCopy(src);
    }
    return *this;
}

void DawgLexicon::iterator::advanceToNextWordInSet() {
    if (setIterator == setEnd) {
        currentSetWord = "";
    } else {
        currentSetWord = *setIterator;
        ++setIterator;
    }
}

void DawgLexicon::iterator::advanceToNextEdge() {
    Edge *ep = edgePtr;
    if (ep->children == 0) {
        while (ep && ep->lastEdge) {
            if (stack.isEmpty()) {
                edgePtr = nullptr;
                return;
            } else {
                ep = stack.pop();
                currentDawgPrefix.resize(currentDawgPrefix.length() - 1);
            }
        }
        edgePtr = ep + 1;
    } else {
        stack.push(ep);
        currentDawgPrefix.push_back(lp->ordToChar(ep->letter));
        edgePtr = &lp->edges[ep->children];
    }
}

void DawgLexicon::iterator::advanceToNextWordInDawg() {
    if (!edgePtr) {
        edgePtr = lp->start;
    } else {
        advanceToNextEdge();
    }
    while (edgePtr && !edgePtr->accept) {
        advanceToNextEdge();
    }
}

std::ostream& operator <<(std::ostream& os, const DawgLexicon& lex) {
    return stanfordcpplib::collections::writeCollection(os, lex);
}

std::istream& operator >>(std::istream& is, DawgLexicon& lex) {
    std::string element;
    return stanfordcpplib::collections::readCollection(is, lex, element, /* descriptor */ "DawgLexicon::operator >>");
}

int hashCode(const DawgLexicon& lex) {
    return stanfordcpplib::collections::hashCodeCollection(lex);
}

/*
 * Swaps a 4-byte long from big to little endian byte order
 */
static uint32_t my_ntohl(uint32_t arg) {
    uint32_t result = ((arg & 0xff000000) >> 24) |
            ((arg & 0x00ff0000) >> 8) |
            ((arg & 0x0000ff00) << 8) |
            ((arg & 0x000000ff) << 24);
    return result;
}

/*
 * File: hashcode.cpp
 * ------------------
 * This file implements the interface declared in hashcode.h.
 *
 * @version 2018/08/10
 * - bugfixes involving negative hash codes, unified string hashing
 * @version 2017/10/21
 * - added hash codes for short, unsigned integers
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 */

#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#undef INTERNAL_INCLUDE
#include <cstddef>       // For size_t
#include <cstring>       // For strlen

static const int HASH_SEED = 5381;               // Starting point for first cycle
static const int HASH_MULTIPLIER = 33;           // Multiplier for each cycle
static const int HASH_MASK = unsigned(-1) >> 1;  // All 1 bits except the sign

int hashSeed() {
    return HASH_SEED;
}

int hashMultiplier() {
    return HASH_MULTIPLIER;
}

int hashMask() {
    return HASH_MASK;
}

/* 
 * Implementation notes: hashCode(int)
 * -----------------------------------
 * Hash code for integers masks off the sign bit, guaranteeing a nonnegative value.
 */
int hashCode(int key) {
    return key & HASH_MASK;
}

/* 
 * Implementation notes: hashCode(other primitive types)
 * -----------------------------------------------------
 * Hash codes for all other primitive types forward to the hash code for integers.
 * This ensures that all hash codes get the proper masking treatment.
 *
 * Thanks to Jeremy Barenholtz for identifying that the original versions of these
 * functions, which just cast their arguments to integers, could lead to negative
 * results.
 */
int hashCode(bool key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(char key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(unsigned int key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(long key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(unsigned long key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(short key) {
    return hashCode(static_cast<int>(key));
}

int hashCode(unsigned short key) {
    return hashCode(static_cast<int>(key));
}

/* 
 * Implementation notes: hashCode(void*)
 * -----------------------------------------------------
 * Catch-all handler for pointers not matched by other
 * overloads just treats the pointer value numerically.
 */
int hashCode(void* key) {
    return hashCode(reinterpret_cast<long>(key));
}

/*
 * Implementation notes: hashCode(string), hashCode(double)
 * --------------------------------------------------------
 * This function takes a string key and uses it to derive a hash code,
 * which is a nonnegative integer related to the key by a deterministic
 * function that distributes keys well across the space of integers.
 * The general method is called linear congruence, which is also used
 * in random-number generators.  The specific algorithm used here is
 * called djb2 after the initials of its inventor, Daniel J. Bernstein,
 * Professor of Mathematics at the University of Illinois at Chicago.
 */
int hashCode(const char* base, size_t numBytes) {
    unsigned hash = HASH_SEED;
    for (size_t i = 0; i < numBytes; i++) {
        hash = HASH_MULTIPLIER * hash + base[i];
    }
    return hashCode(hash);
} 

int hashCode(const char* str) {
    return hashCode(str, strlen(str));
}

int hashCode(const std::string& str) {
    return hashCode(str.data(), str.length());
}

int hashCode(double key) {
    return hashCode(reinterpret_cast<const char *>(&key), sizeof(double));
}

int hashCode(float key) {
    return hashCode(reinterpret_cast<const char *>(&key), sizeof(float));
}

int hashCode(long double key) {
    return hashCode(reinterpret_cast<const char *>(&key), sizeof(long double));
}

/*
 * File: lexicon.cpp
 * -----------------
 * A Lexicon is a word list. This Lexicon is backed by a data
 * structure called a prefix tree or trie ("try").
 *
 * This is a re-implementation of Lexicon.  Its previous implementation used
 * a pair of structures: a directed acyclic word graph (DAWG) and an STL set.
 * That implementation was discarded because of several reasons:
 *
 * - It relied on binary file formats that were not readable by students.
 * - It did not provide for expected class members like remove.
 * - It had a clunky pair of data structures that had to be searched separately.
 * - It was optimized for space usage over ease of use and maintenance.
 *
 * The original DAWG implementation is retained as dawglexicon.h/cpp.
 * 
 * @version 2018/03/10
 * - added method front
 * @version 2016/09/24
 * - refactored to use collections.h utility functions
 * @version 2016/08/11
 * - added operators +, +=, -, -=, *, *= to better match Set/HashSet
 * @version 2016/08/10
 * - added constructor support for std initializer_list usage, such as {"a", "b", "c"}
 * @version 2016/08/04
 * - fixed operator >> to not throw errors
 * @version 2015/07/05
 * - using global hashing functions rather than global variables
 * @version 2014/11/13
 * - added comparison operators <, >=, etc.
 * - added hashCode function
 * @version 2014/10/10
 * - added comparison operators ==, !=
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "lexicon.h"
#undef INTERNAL_INCLUDE
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define INTERNAL_INCLUDE 1
#include "collections.h"
#define INTERNAL_INCLUDE 1
#include "dawglexicon.h"
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "hashcode.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

static bool scrub(std::string& str);

Lexicon::Lexicon() :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    // empty
}

Lexicon::Lexicon(std::istream& input) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    addWordsFromFile(input);
}

Lexicon::Lexicon(const std::string& filename) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    addWordsFromFile(filename);
}

Lexicon::Lexicon(std::initializer_list<std::string> list) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    addAll(list);
}

Lexicon::Lexicon(const Lexicon& src) :
        m_root(nullptr),
        m_size(0),
        m_removeFlag(false) {
    deepCopy(src);
}

Lexicon::~Lexicon() {
    clear();
}

bool Lexicon::add(const std::string& word) {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return addHelper(m_root, scrubbed, /* originalWord */ scrubbed);
}

Lexicon& Lexicon::addAll(const Lexicon& lex) {
    for (const std::string& word : lex) {
        add(word);
    }
    return *this;
}

Lexicon& Lexicon::addAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        add(word);
    }
    return *this;
}

void Lexicon::addWordsFromFile(std::istream& input) {
    bool isDAWG = isDAWGFile(input);
    rewindStream(input);
    if (isDAWG) {
        readBinaryFile(input);
    } else {
        if (input.fail()) {
            error("Lexicon::addWordsFromFile: Couldn't read from input");
        }
        std::string line;
        while (getline(input, line)) {
            add(trim(line));
        }
    }
}

void Lexicon::addWordsFromFile(const std::string& filename) {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error("Lexicon::addWordsFromFile: Couldn't read from input file " + filename);
    }
    addWordsFromFile(input);
    input.close();
}

std::string Lexicon::back() const {
    if (isEmpty()) {
        error("Lexicon::back: lexicon is empty");
    }
    return m_allWords.back();
}

void Lexicon::clear() {
    m_size = 0;
    m_allWords.clear();
    deleteTree(m_root);
    m_root = nullptr;
}

bool Lexicon::contains(const std::string& word) const {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return containsHelper(m_root, scrubbed, /* isPrefix */ false);
}

bool Lexicon::containsAll(const Lexicon& lex2) const {
    for (const std::string& word : lex2) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool Lexicon::containsAll(std::initializer_list<std::string> list) const {
    for (const std::string& word : list) {
        if (!contains(word)) {
            return false;
        }
    }
    return true;
}

bool Lexicon::containsPrefix(const std::string& prefix) const {
    if (prefix.empty()) {
        return true;
    }
    std::string scrubbed = prefix;
    if (!scrub(scrubbed)) {
        return false;
    }
    return containsHelper(m_root, scrubbed, /* isPrefix */ true);
}

bool Lexicon::equals(const Lexicon& lex2) const {
    return stanfordcpplib::collections::equals(*this, lex2);
}

std::string Lexicon::first() const {
    if (isEmpty()) {
        error("Lexicon::first: lexicon is empty");
    }
    return m_allWords.front();
}

std::string Lexicon::front() const {
    if (isEmpty()) {
        error("Lexicon::front: lexicon is empty");
    }
    return m_allWords.front();
}

void Lexicon::insert(const std::string& word) {
    add(word);
}

bool Lexicon::isEmpty() const {
    return size() == 0;
}

bool Lexicon::isSubsetOf(const Lexicon& lex2) const {
    auto it = begin();
    auto end = this->end();
    while (it != end) {
        if (!lex2.contains(*it)) {
            return false;
        }
        ++it;
    }
    return true;
}

bool Lexicon::isSubsetOf(std::initializer_list<std::string> list) const {
    Lexicon lex2(list);
    return isSubsetOf(lex2);
}

bool Lexicon::isSupersetOf(const Lexicon& lex2) const {
    return containsAll(lex2);
}

bool Lexicon::isSupersetOf(std::initializer_list<std::string> list) const {
    return containsAll(list);
}

void Lexicon::mapAll(void (*fn)(std::string)) const {
    for (std::string word : m_allWords) {
        fn(word);
    }
}

void Lexicon::mapAll(void (*fn)(const std::string&)) const {
    for (std::string word : m_allWords) {
        fn(word);
    }
}

bool Lexicon::remove(const std::string& word) {
    if (word.empty()) {
        return false;
    }
    std::string scrubbed = word;
    if (!scrub(scrubbed)) {
        return false;
    }
    return removeHelper(m_root, scrubbed, /* originalWord */ scrubbed, /* isPrefix */ false);
}

Lexicon& Lexicon::removeAll(const Lexicon& lex2) {
    Vector<std::string> toRemove;
    for (const std::string& word : *this) {
        if (lex2.contains(word)) {
            toRemove.add(word);
        }
    }
    for (const std::string& word : toRemove) {
        remove(word);
    }
    return *this;
}

Lexicon& Lexicon::removeAll(std::initializer_list<std::string> list) {
    for (const std::string& word : list) {
        remove(word);
    }
    return *this;
}

bool Lexicon::removePrefix(const std::string& prefix) {
    if (prefix.empty()) {
        bool result = !isEmpty();
        clear();
        return result;
    }
    std::string scrubbed = prefix;
    if (!scrub(scrubbed)) {
        return false;
    }
    
    return removeHelper(m_root, scrubbed, /* originalWord */ scrubbed, /* isPrefix */ true);
}

Lexicon& Lexicon::retainAll(const Lexicon& lex2) {
    Vector<std::string> toRemove;
    for (const std::string& word : *this) {
        if (!lex2.contains(word)) {
            toRemove.add(word);
        }
    }
    for (const std::string& word : toRemove) {
        remove(word);
    }
    return *this;
}

Lexicon& Lexicon::retainAll(std::initializer_list<std::string> list) {
    Lexicon lex2(list);
    return retainAll(lex2);
}

int Lexicon::size() const {
    return m_size;
}

std::string Lexicon::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

/*
 * Operators
 */
bool Lexicon::operator ==(const Lexicon& lex2) const {
    return equals(lex2);
}

bool Lexicon::operator !=(const Lexicon& lex2) const {
    return !equals(lex2);
}

bool Lexicon::operator <(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) < 0;
}

bool Lexicon::operator <=(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) <= 0;
}

bool Lexicon::operator >(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) > 0;
}

bool Lexicon::operator >=(const Lexicon& lex2) const {
    return stanfordcpplib::collections::compare(*this, lex2) >= 0;
}

Lexicon Lexicon::operator +(const Lexicon& lex2) const {
    Lexicon lex = *this;
    lex.addAll(lex2);
    return lex;
}

Lexicon Lexicon::operator +(std::initializer_list<std::string> list) const {
    Lexicon lex = *this;
    lex.addAll(list);
    return lex;
}

Lexicon Lexicon::operator +(const std::string& word) const {
    Lexicon lex = *this;
    lex.add(word);
    return lex;
}

Lexicon Lexicon::operator *(const Lexicon& lex2) const {
    Lexicon lex = *this;
    return lex.retainAll(lex2);
}

Lexicon Lexicon::operator *(std::initializer_list<std::string> list) const {
    Lexicon lex = *this;
    return lex.retainAll(list);
}

Lexicon Lexicon::operator -(const Lexicon& lex2) const {
    Lexicon lex = *this;
    return lex.removeAll(lex2);
}

Lexicon Lexicon::operator -(std::initializer_list<std::string> list) const {
    Lexicon lex = *this;
    return lex.removeAll(list);
}

Lexicon Lexicon::operator -(const std::string& word) const {
    Lexicon lex = *this;
    lex.remove(word);
    return lex;
}

Lexicon& Lexicon::operator +=(const Lexicon& lex2) {
    return addAll(lex2);
}

Lexicon& Lexicon::operator +=(std::initializer_list<std::string> list) {
    return addAll(list);
}

Lexicon& Lexicon::operator +=(const std::string& word) {
    add(word);
    m_removeFlag = false;
    return *this;
}

Lexicon& Lexicon::operator *=(const Lexicon& lex2) {
    return retainAll(lex2);
}

Lexicon& Lexicon::operator *=(std::initializer_list<std::string> list) {
    return retainAll(list);
}

Lexicon& Lexicon::operator -=(const Lexicon& lex2) {
    return removeAll(lex2);
}

Lexicon& Lexicon::operator -=(std::initializer_list<std::string> list) {
    return removeAll(list);
}

Lexicon& Lexicon::operator -=(const std::string& word) {
    remove(word);
    m_removeFlag = true;
    return *this;
}

/* private helpers implementation */

Lexicon& Lexicon::operator ,(const std::string& word) {
    if (m_removeFlag) {
        remove(word);
    } else {
        add(word);
    }
    return *this;
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::addHelper(TrieNode*& node, const std::string& word, const std::string& originalWord) {
    if (!node) {
        // create nodes all the way down, one for each letter of the word
        node = new TrieNode();
    }

    if (word.empty()) {
        // base case: we have added all of the letters of this word
        if (node->isWord()) {
            return false;   // duplicate word; already present
        } else {
            // new word; add it
            node->setWord(true);
            m_size++;
            m_allWords.add(originalWord);
            return true;
        }
    } else {
        // recursive case: chop off first letter, traverse the rest
        return addHelper(node->child(word[0]), word.substr(1), originalWord);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::containsHelper(TrieNode* node, const std::string& word, bool isPrefix) const {
    if (!node) {
        // base case: no pointer down to here, so prefix must not exist
        return false;
    } else if (word.length() == 0) {
        // base case: Found nodes all the way down.
        // If we are looking for a prefix, this means this path IS a prefix,
        // so we should return true.
        // If we are looking for an exact word match rather than a prefix,
        // we must check the isWord flag to see that this word was added
        return (isPrefix ? true : node->isWord());
    } else {
        // recursive case: follow appropriate child pointer for one letter
        return containsHelper(node->child(word[0]), word.substr(1), isPrefix);
    }
}

// pre: word is scrubbed to contain only lowercase a-z letters
bool Lexicon::removeHelper(TrieNode*& node, const std::string& word, const std::string& originalWord, bool isPrefix) {
    if (!node) {
        // base case: dead end; this word/prefix must not be contained
        return false;
    } else if (word.empty()) {
        // base case: we have walked all of the letters of this word/prefix
        // and now we must do the removal
        if (isPrefix) {
            // remove this node and all of its descendents
            removeSubtreeHelper(node, originalWord);   // removes from m_allWords, sets m_size
            node = nullptr;
        } else {
            // found this word in the lexicon;
            if (node->isLeaf()) {
                // remove this leaf node only
                delete node;
                node = nullptr;
            } else {
                // de-word-ify this node, but leave it because it may
                // still have children that are valid words
                if (node->isWord()) {
                    node->setWord(false);
                }
            }
            m_allWords.remove(originalWord);
            m_size--;
        }
        return true;
    } else {
        // recursive case: chop off first letter, traverse the rest
        bool wasLeaf = node->isLeaf();
        bool result = removeHelper(node->child(word[0]), word.substr(1), originalWord, isPrefix);

        // memory cleanup: if I wasn't a leaf but now am, and am not a word,
        // then I am now unneeded, so remove me too
        if (result && !wasLeaf && node
                && node->isLeaf() && !node->isWord()) {
            delete node;
            node = nullptr;
        }
        return result;
    }
}

// remove/free this node and all descendents
void Lexicon::removeSubtreeHelper(TrieNode*& node, const std::string& originalWord) {
    if (node) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            removeSubtreeHelper(node->child(letter), originalWord + letter);
        }
        if (node->isWord()) {
            m_allWords.remove(originalWord);
            m_size--;
        }
        delete node;
        node = nullptr;
    }
}

void Lexicon::deepCopy(const Lexicon& src) {
    for (std::string word : src.m_allWords) {
        add(word);
    }
}

void Lexicon::deleteTree(TrieNode* node) {
    if (node) {
        for (char letter = 'a'; letter <= 'z'; letter++) {
            deleteTree(node->child(letter));
        }
        delete node;
    }
}

/*
 * Returns true if the given file (probably) represents a
 * binary DAWG lexicon data file.
 */
bool Lexicon::isDAWGFile(std::istream& input) const {
    char firstFour[4], expected[] = "DAWG";
    if (input.fail()) {
        error(std::string("Lexicon::addWordsFromFile: Couldn't read input"));
    }
    input.read(firstFour, 4);
    bool result = strncmp(firstFour, expected, 4) == 0;
    return result;
}

/*
 * Returns true if the given file (probably) represents a
 * binary DAWG lexicon data file.
 */
bool Lexicon::isDAWGFile(const std::string& filename) const {
    std::ifstream input(filename.c_str());
    if (input.fail()) {
        error(std::string("Lexicon::addWordsFromFile: Couldn't open lexicon file ") + filename);
    }
    bool result = isDAWGFile(input);
    input.close();
    return result;
}

/*
 * We just delegate to DawgLexicon, the old implementation, to read a binary
 * lexicon data file, and then we extract its yummy data into our trie.
 */
void Lexicon::readBinaryFile(std::istream& input) {
    DawgLexicon ldawg(input);
    for (std::string word : ldawg) {
        add(word);
    }
}

/*
 * We just delegate to DawgLexicon, the old implementation, to read a binary
 * lexicon data file, and then we extract its yummy data into our trie.
 */
void Lexicon::readBinaryFile(const std::string& filename) {
    DawgLexicon ldawg(filename);
    for (std::string word : ldawg) {
        add(word);
    }
}

Lexicon& Lexicon::operator =(const Lexicon& src) {
    if (this != &src) {
        clear();
        deepCopy(src);
    }
    return *this;
}

std::ostream& operator <<(std::ostream& out, const Lexicon& lex) {
    out << lex.m_allWords;
    return out;
}

std::istream& operator >>(std::istream& is, Lexicon& lex) {
    std::string element;
    return stanfordcpplib::collections::readCollection(is, lex, element, /* descriptor */ "Lexicon::operator >>");
}

/*
 * Hash function for lexicons.
 */
int hashCode(const Lexicon& lex) {
    return stanfordcpplib::collections::hashCodeCollection(lex);
}

static bool scrub(std::string& str) {
    size_t nChars = str.length();
    size_t outIndex = 0;
    for (size_t i = 0; i < nChars; i++) {
        std::string::value_type ch = tolower(str[i]);
        if (ch < 'a' || ch > 'z') {
            return false;   // illegal string
        } else {
            str[outIndex] = ch;
            outIndex++;
        }
    }
    if (outIndex != nChars) {
        str.erase(outIndex, nChars - outIndex);
    }
    return true;
}

/*
 * File: server.cpp
 * ----------------
 * This file exports a set of functions that implement a simple HTTP server
 * that can listen for connections.
 *
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
 * @version 2016/03/16
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "server.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

namespace HttpServer {
STATIC_CONST_VARIABLE_DECLARE(std::string, CONTENT_TYPE_DEFAULT, "text/html")
// STATIC_CONST_VARIABLE_DECLARE(std::string, CONTENT_TYPE_ERROR, "text/plain")
STATIC_VARIABLE_DECLARE(bool, isRunning, false)

bool isRunning() {
    return STATIC_VARIABLE(isRunning);
}

std::string getContentType(const std::string& extension) {
    static Map<std::string, std::string> CONTENT_TYPE_MAP;   // extension => MIME type

    if (extension.empty()) {
        return STATIC_VARIABLE(CONTENT_TYPE_DEFAULT);
    }

    // populate map of content types, if needed
    if (CONTENT_TYPE_MAP.isEmpty()) {
        CONTENT_TYPE_MAP["bmp"] = "image/bmp";
        CONTENT_TYPE_MAP["bz"] = "application/x-bzip";
        CONTENT_TYPE_MAP["bz2"] = "application/x-bzip2";
        CONTENT_TYPE_MAP["c"] = "text/plain";
        CONTENT_TYPE_MAP["cc"] = "text/plain";
        CONTENT_TYPE_MAP["com"] = "application/octet-stream";
        CONTENT_TYPE_MAP["cpp"] = "text/plain";
        CONTENT_TYPE_MAP["css"] = "text/css";
        CONTENT_TYPE_MAP["doc"] = "application/msword";
        CONTENT_TYPE_MAP["dot"] = "application/msword";
        CONTENT_TYPE_MAP["exe"] = "application/octet-stream";
        CONTENT_TYPE_MAP["gif"] = "image/gif";
        CONTENT_TYPE_MAP["gz"] = "application/x-gzip";
        CONTENT_TYPE_MAP["gzip"] = "application/x-gzip";
        CONTENT_TYPE_MAP["h"] = "text/plain";
        CONTENT_TYPE_MAP["hh"] = "text/plain";
        CONTENT_TYPE_MAP["hpp"] = "text/plain";
        CONTENT_TYPE_MAP["htm"] = "text/html";
        CONTENT_TYPE_MAP["html"] = "text/html";
        CONTENT_TYPE_MAP["htmls"] = "text/html";
        CONTENT_TYPE_MAP["ico"] = "image/x-icon";
        CONTENT_TYPE_MAP["inf"] = "text/plain";
        CONTENT_TYPE_MAP["jar"] = "application/octet-stream";
        CONTENT_TYPE_MAP["jav"] = "text/plain";
        CONTENT_TYPE_MAP["java"] = "text/plain";
        CONTENT_TYPE_MAP["jpe"] = "image/jpeg";
        CONTENT_TYPE_MAP["jpeg"] = "image/jpeg";
        CONTENT_TYPE_MAP["jpg"] = "image/jpeg";
        CONTENT_TYPE_MAP["mid"] = "audio/midi";
        CONTENT_TYPE_MAP["midi"] = "audio/midi";
        CONTENT_TYPE_MAP["mod"] = "audio/mod";
        CONTENT_TYPE_MAP["mov"] = "video/quicktime";
        CONTENT_TYPE_MAP["mp3"] = "text/plain";
        CONTENT_TYPE_MAP["mpg"] = "video/mpeg";
        CONTENT_TYPE_MAP["o"] = "application/octet-stream";
        CONTENT_TYPE_MAP["odc"] = "application/vnd.oasis.opendocument.chart";
        CONTENT_TYPE_MAP["odp"] = "application/vnd.oasis.opendocument.presentation";
        CONTENT_TYPE_MAP["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
        CONTENT_TYPE_MAP["odt"] = "application/vnd.oasis.opendocument.text";
        CONTENT_TYPE_MAP["pct"] = "image/x-pict";
        CONTENT_TYPE_MAP["pcx"] = "image/x-pcx";
        CONTENT_TYPE_MAP["pdf"] = "application/pdf";
        CONTENT_TYPE_MAP["pl"] = "text/plain";
        CONTENT_TYPE_MAP["pm"] = "text/plain";
        CONTENT_TYPE_MAP["ppt"] = "application/powerpoint";
        CONTENT_TYPE_MAP["ps"] = "application/postscript";
        CONTENT_TYPE_MAP["psd"] = "application/octet-stream";
        CONTENT_TYPE_MAP["py"] = "text/plain";
        CONTENT_TYPE_MAP["qt"] = "video/quicktime";
        CONTENT_TYPE_MAP["ra"] = "audio/x-realaudio";
        CONTENT_TYPE_MAP["rb"] = "text/plain";
        CONTENT_TYPE_MAP["rm"] = "application/vnd.rn-realmedia";
        CONTENT_TYPE_MAP["rtf"] = "application/rtf";
        CONTENT_TYPE_MAP["s"] = "text/x-asm";
        CONTENT_TYPE_MAP["sh"] = "text/plain";
        CONTENT_TYPE_MAP["shtml"] = "text/html";
        CONTENT_TYPE_MAP["swf"] = "application/x-shockwave-flash";
        CONTENT_TYPE_MAP["tcl"] = "application/x-tcl";
        CONTENT_TYPE_MAP["tex"] = "application/x-tex";
        CONTENT_TYPE_MAP["tgz"] = "application/x-compressed";
        CONTENT_TYPE_MAP["tif"] = "image/tiff";
        CONTENT_TYPE_MAP["tiff"] = "image/tiff";
        CONTENT_TYPE_MAP["txt"] = "text/plain";
        CONTENT_TYPE_MAP["voc"] = "audio/voc";
        CONTENT_TYPE_MAP["wav"] = "audio/wav";
        CONTENT_TYPE_MAP["xls"] = "application/excel";
        CONTENT_TYPE_MAP["xlt"] = "application/excel";
        CONTENT_TYPE_MAP["xpm"] = "image/xpm";
        CONTENT_TYPE_MAP["z"] = "application/x-compressed";
        CONTENT_TYPE_MAP["zip"] = "application/zip";
    }

    // "foo.BAZ.BaR" => "bar"
    std::string ext = toLowerCase(extension);
    int dot = stringLastIndexOf(ext, ".");
    if (dot >= 0) {
        ext = ext.substr(dot + 1);
    }

    if (CONTENT_TYPE_MAP.containsKey(ext)) {
        return CONTENT_TYPE_MAP[ext];
    } else {
        return STATIC_VARIABLE(CONTENT_TYPE_DEFAULT);
    }
}

std::string getErrorMessage(int httpErrorCode) {
    static Map<int, std::string> ERROR_MESSAGE_MAP;   // 404 => "File not found"

    // populate map of content types, if needed
    if (ERROR_MESSAGE_MAP.isEmpty()) {
        ERROR_MESSAGE_MAP[200] = "HTTP ERROR 200: OK";
        ERROR_MESSAGE_MAP[201] = "HTTP ERROR 201: Created";
        ERROR_MESSAGE_MAP[202] = "HTTP ERROR 202: Accepted";
        ERROR_MESSAGE_MAP[204] = "HTTP ERROR 204: No content";
        ERROR_MESSAGE_MAP[301] = "HTTP ERROR 301: Moved permanently";
        ERROR_MESSAGE_MAP[302] = "HTTP ERROR 302: Found";
        ERROR_MESSAGE_MAP[303] = "HTTP ERROR 303: See other";
        ERROR_MESSAGE_MAP[304] = "HTTP ERROR 304: Not modified";
        ERROR_MESSAGE_MAP[305] = "HTTP ERROR 305: Use proxy";
        ERROR_MESSAGE_MAP[307] = "HTTP ERROR 307: Temporary redirect";
        ERROR_MESSAGE_MAP[308] = "HTTP ERROR 308: Permanent redirect";
        ERROR_MESSAGE_MAP[400] = "HTTP ERROR 400: Bad request";
        ERROR_MESSAGE_MAP[401] = "HTTP ERROR 401: Unauthorized";
        ERROR_MESSAGE_MAP[402] = "HTTP ERROR 402: Payment required";
        ERROR_MESSAGE_MAP[403] = "HTTP ERROR 403: Forbidden";
        ERROR_MESSAGE_MAP[404] = "HTTP ERROR 404: Not found";
        ERROR_MESSAGE_MAP[405] = "HTTP ERROR 405: Request method not allowed";
        ERROR_MESSAGE_MAP[406] = "HTTP ERROR 406: Not acceptable";
        ERROR_MESSAGE_MAP[407] = "HTTP ERROR 407: Proxy authentication failed";
        ERROR_MESSAGE_MAP[408] = "HTTP ERROR 408: Request timeout";
        ERROR_MESSAGE_MAP[409] = "HTTP ERROR 409: Conflict";
        ERROR_MESSAGE_MAP[410] = "HTTP ERROR 410: Gone";
        ERROR_MESSAGE_MAP[413] = "HTTP ERROR 413: Payload too large";
        ERROR_MESSAGE_MAP[415] = "HTTP ERROR 415: Unsupported media type";
        ERROR_MESSAGE_MAP[420] = "HTTP ERROR 420: Enhance your calm; hey whatever man";
        ERROR_MESSAGE_MAP[429] = "HTTP ERROR 429: Too many requests";
        ERROR_MESSAGE_MAP[500] = "HTTP ERROR 500: Internal server error";
        ERROR_MESSAGE_MAP[501] = "HTTP ERROR 501: Not implemented";
        ERROR_MESSAGE_MAP[502] = "HTTP ERROR 502: Bad gateway";
        ERROR_MESSAGE_MAP[503] = "HTTP ERROR 503: Service unavailable";
        ERROR_MESSAGE_MAP[504] = "HTTP ERROR 504: Gateway timeout";
        ERROR_MESSAGE_MAP[508] = "HTTP ERROR 508: Loop detected";
        ERROR_MESSAGE_MAP[511] = "HTTP ERROR 511: Network authentication required";
    }

    if (ERROR_MESSAGE_MAP.containsKey(httpErrorCode)) {
        return ERROR_MESSAGE_MAP[httpErrorCode];
    } else {
        return "HTTP ERROR " + std::to_string(httpErrorCode) + ": Unknown error";
    }
}

// "http://foo.bar.com/a/b/c.txt?a=b&c=d#lol" => "txt"
// "http://foo.bar.com/index.html" => "html"
// "http://foo.bar.com/a/b/" => ""
std::string getUrlExtension(const std::string& url) {
    std::string url2 = toLowerCase(trim(url));
    int questionMark = stringIndexOf(url2, "?");
    if (questionMark >= 0) {
        url2 = url2.substr(0, questionMark);
    }
    int hash = stringIndexOf(url2, "#");
    if (hash >= 0) {
        url2 = url2.substr(0, hash);
    }
    int slash = stringLastIndexOf(url2, "/");
    if (slash >= 0) {
        url2 = url2.substr(slash + 1);
    }
    int dot = stringLastIndexOf(url2, ".");
    if (dot >= 0) {
        url2 = url2.substr(dot + 1);
    }
    return url2;
}

void sendResponse(const GEvent& /*event*/, const std::string& /* responseText */,
                  const std::string& contentType) {
    if (!isRunning()) {
        error("HttpServer::sendResponse: server is not running");
    }
    std::string contentTypeActual = contentType;
    if (contentTypeActual.empty()) {
        // TODO
        // contentTypeActual = getContentType(getUrlExtension(event.getRequestURL()));
    }
    // TODO
    // stanfordcpplib::getPlatform()->httpserver_sendResponse(event.getRequestID(), HTTP_ERROR_OK, contentTypeActual, responseText);
}

void sendResponseError(const GEvent& /*event*/, int httpErrorCode,
                       const std::string& errorMessage) {
    std::string errorMessageActual = errorMessage;
    if (errorMessageActual.empty()) {
        errorMessageActual = getErrorMessage(httpErrorCode);
    }
    // TODO
    // stanfordcpplib::getPlatform()->httpserver_sendResponse(event.getRequestID(), httpErrorCode, STATIC_VARIABLE(CONTENT_TYPE_ERROR), errorMessageActual);
}

void sendResponseFile(const GEvent& /*event*/, const std::string& responseFilePath,
                      const std::string& contentType) {
    if (!isRunning()) {
        error("HttpServer::sendResponse: server is not running");
    }
    std::string contentTypeActual = contentType;
    if (contentTypeActual.empty()) {
        contentTypeActual = getContentType(getExtension(responseFilePath));
    }
    // TODO
    // stanfordcpplib::getPlatform()->httpserver_sendResponseFile(event.getRequestID(), contentType, responseFilePath);
}


void startServer(int /*port*/) {
    if (!STATIC_VARIABLE(isRunning)) {
        // stanfordcpplib::getPlatform()->httpserver_start(port);
        STATIC_VARIABLE(isRunning) = true;
    }
}

void stopServer() {
    if (STATIC_VARIABLE(isRunning)) {
        STATIC_VARIABLE(isRunning) = false;
        // stanfordcpplib::getPlatform()->httpserver_stop();
    }
}
} // namespace HttpServer

/*
 * File: plainconsole.cpp
 * ----------------------
 * This file defines the implementation of functions to add utility to the
 * C++ plain text console streams, cin/cout/cerr.
 * See plainconsole.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2017/11/12
 * - changed limited stream to throw error rather than raise SIGABRT for better displaying
 * @version 2017/10/20
 * - fixed compiler warning about 0 vs nullptr
 * @version 2015/10/21
 * @since 2015/10/21
 */

#define INTERNAL_INCLUDE 1
#include "plainconsole.h"
#include <csignal>
#include <iostream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "error.h"
#undef INTERNAL_INCLUDE

namespace plainconsole {
/*
 * A stream buffer that just forwards everything to a delegate,
 * but echoes any user input read from it.
 * Used to (sometimes) echo console input when redirected in from a file.
 * http://www.cplusplus.com/reference/streambuf/streambuf/
 */
class EchoingStreambuf : public std::streambuf {
private:
    /* Constants */
    static const int BUFFER_SIZE = 4096;

    /* Instance variables */
    char inBuffer[BUFFER_SIZE];
    char outBuffer[BUFFER_SIZE];
    std::istream instream;
    std::ostream& outstream;
    int outputLimit;
    int outputPrinted;

public:
    EchoingStreambuf(std::streambuf& buf, std::ostream& out)
            : instream(&buf),
              outstream(out),
              outputLimit(0),
              outputPrinted(0) {
        // outstream.rdbuf(&buf);
        setg(inBuffer, inBuffer, inBuffer);
        setp(outBuffer, outBuffer + BUFFER_SIZE);
    }

    ~EchoingStreambuf() {
        /* Empty */
    }
    
    virtual void setOutputLimit(int limit) {
        outputLimit = limit;
    }

    virtual int underflow() {
        // 'return 0' handles end-of-input from stdin redirect
        std::string line;
        if (!getline(instream, line)) {
            return 0;
        }
        
        int n = line.length();
        if (n + 1 >= BUFFER_SIZE) {
            error("EchoingStreambuf::underflow: String too long");
        }
        for (int i = 0; i < n; i++) {
            inBuffer[i] = line[i];
        }
        inBuffer[n++] = '\n';
        inBuffer[n] = '\0';
        setg(inBuffer, inBuffer, inBuffer + n);
        
        // this is the place to echo the input
        // fprintf(stdout, "inBuffer: \"%s\"\n", inBuffer);
        // fflush(stdout);
        outstream << inBuffer;
        outstream.flush();
        
        return inBuffer[0];
    }

    virtual int overflow(int ch = EOF) {
        std::string line = "";
        for (char *cp = pbase(); cp < pptr(); cp++) {
            if (*cp == '\n') {
                // puts(line.c_str());
                outputPrinted += line.length();
                if (outputLimit > 0 && outputPrinted > outputLimit) {
                    error("excessive output printed");
                }
                line = "";
            } else {
                line += *cp;
            }
        }
        if (line != "") {
            // puts(line.c_str());
            outputPrinted += line.length();
            if (outputLimit > 0 && outputPrinted > outputLimit) {
                error("excessive output printed");
            }
        }
        setp(outBuffer, outBuffer + BUFFER_SIZE);
        if (ch != EOF) {
            outBuffer[0] = ch;
            pbump(1);
        }
        return ch != EOF;
    }
    
    virtual int sync() {
        return overflow();
    }
};

/*
 * A stream buffer that limits how many characters you can print to it.
 * If you exceed that many, it throws an ErrorException.
 */
class LimitedStreambuf : public std::streambuf {
private:
    std::ostream outstream;
    int outputLimit;
    int outputPrinted;

public:
    LimitedStreambuf(std::streambuf& buf, int limit)
            : outstream(&buf),
              outputLimit(limit),
              outputPrinted(0) {
        setp(nullptr, nullptr);   // // no buffering, overflow on every char
    }

    virtual void setOutputLimit(int limit) {
        outputLimit = limit;
    }

    virtual int overflow(int ch = EOF) {
        outputPrinted++;
        if (outputLimit > 0 && outputPrinted > outputLimit) {
            // error("excessive output printed");
            // outstream.setstate(std::ios::failbit | std::ios::badbit | std::ios::eofbit);
            // kill the program
            // (use a signal rather than error/exception
            // so student won't try to catch it)
            // error("Excessive output printed; you may have an infinite loop in your code.");
            raise(SIGUSR1);
        } else {
            outstream.put(ch);
        }
        return ch;
    }
};

void setOutputLimit(int limit) {
    if (limit <= 0) {
        error("Platform::setConsoleOutputLimit: limit must be a positive integer");
    }
    LimitedStreambuf* limitedbufOut = new LimitedStreambuf(*std::cout.rdbuf(), limit);
    LimitedStreambuf* limitedbufErr = new LimitedStreambuf(*std::cerr.rdbuf(), limit);
    std::cout.rdbuf(limitedbufOut);
    std::cerr.rdbuf(limitedbufErr);
}

void setEcho(bool value) {
    static EchoingStreambuf* echobufIn = nullptr;
    static std::streambuf* oldBuf = nullptr;
    
    if (!echobufIn && value) {
        // start to echo user input pulled from cin
        oldBuf = std::cin.rdbuf();
        echobufIn = new EchoingStreambuf(*std::cin.rdbuf(), std::cout);
        std::cin.rdbuf(echobufIn);
    } else if (echobufIn && !value) {
        // stop echo
        std::cin.rdbuf(oldBuf);
        oldBuf = nullptr;
        echobufIn = nullptr;
    }
}

} // namespace plainconsole

/*
 * File: urlstream.cpp
 * -------------------
 * This file contains the implementation of the iurlstream class.
 * Please see urlstream.h for information about how to use these classes.
 *
 * @author Marty Stepp
 * @version 2018/10/02
 * - added close() method for backward compatibility (does nothing)
 * @version 2018/09/18
 * - refactored to integrate with pure-C++ GDownloader implementation
 * - added getErrorMessage method
 * @version 2018/06/20
 * - support for setting headers such as user agent
 * - https URL support
 * - changed string to const string&
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/14
 * - fixed .c_str() Mac bug on ifstream::open() call
 * @since 2014/10/08
 */

#define INTERNAL_INCLUDE 1
#include "urlstream.h"
#include <sstream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "gdownloader.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

namespace {
    /*
     * Given a status code, determines whether it's successful.
     * All successful HTTP status codes are of the form 2xx.
     */
    bool isHttpSuccess(int code) {
        return code >= 200 && code <= 299;
    }
}

iurlstream::iurlstream()
        : _url(""),
          _httpStatusCode(0) {
    // empty
}

iurlstream::iurlstream(const std::string& url)
        : _url(url),
          _httpStatusCode(0) {
    open(url);
}

void iurlstream::close() {
    // empty
}

int iurlstream::getErrorCode() const {
    return isHttpSuccess(_httpStatusCode)? 0 : _httpStatusCode;
}

std::string iurlstream::getErrorMessage() const {
    return _errorMessage;
}

int iurlstream::getHttpStatusCode() const {
    /* All HTTP status codes are between 1xx and 5xx, inclusive. */
    return _httpStatusCode >= 100 && _httpStatusCode <= 599? _httpStatusCode : 0;
}

std::string iurlstream::getHeader(const std::string& name) const {
    return _headers[name];
}

std::string iurlstream::getUrl() const {
    return _url;
}

std::string iurlstream::getUserAgent() const {
    if (_headers.containsKey("User-Agent")) {
        return _headers["User-Agent"];
    } else {
        return "";
    }
}

void iurlstream::open(const std::string& url) {
    if (!url.empty()) {
        _url = url;
    }
    _errorMessage = "";
    
    // GDownloader does the heavy lifting of downloading the file for us
    GDownloader downloader;

    // insert/send headers if needed
    if (!_headers.isEmpty()) {
        for (std::string headerName : _headers) {
            downloader.setHeader(headerName, _headers[headerName]);
        }
    }
    std::string urlData = downloader.downloadAsString(_url);
    _httpStatusCode = downloader.getHttpStatusCode();

    if (downloader.hasError()) {
        setstate(std::ios::failbit);
        _errorMessage = downloader.getErrorMessage();
    } else {
        clear();
        this->write(urlData.c_str(), static_cast<std::streamsize>(urlData.length()));
        this->seekg(0);
    }
}

void iurlstream::setHeader(const std::string& name, const std::string& value) {
    _headers[name] = value;
}

void iurlstream::setUserAgent(const std::string& userAgent) {
    setHeader("User-Agent", userAgent);
}

/*
 * File: filelib.cpp
 * -----------------
 * This file implements the filelib.h interface.
 * Platform-dependent functions are handled through filelib_* functions
 * defined in filelibunix.cpp and filelibwindows.cpp.
 * 
 * @version 2016/11/20
 * - small bug fix in readEntireStream method (failed for non-text files)
 * @version 2016/11/12
 * - added fileSize, readEntireStream
 * @version 2016/08/12
 * - added second overload of openFileDialog that accepts path parameter
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * - moved appendSpace function to simpio
 * @version 2015/04/12
 * - added promptUserForFile overload without stream parameter
 * @version 2014/10/19
 * - alphabetized function declarations
 * - converted many funcs to take const string& rather than string for efficiency
 * - added listDirectory overload that returns a Vector
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "filelib.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#define INTERNAL_INCLUDE 1
#include "simpio.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

/* Prototypes */

static void splitPath(const std::string& path, Vector<std::string> list);
static bool recursiveMatch(const std::string& str, int sx, const std::string& pattern, int px);

/* Implementations */

void createDirectory(const std::string& path) {
    return platform::filelib_createDirectory(expandPathname(path));
}

void createDirectoryPath(const std::string& path) {
    size_t cp = 0;
    if (path == "") return;
    std::string expandedPath = expandPathname(path);
    char sep = getDirectoryPathSeparator()[0];
    if (expandedPath.substr(1, 2) == ":\\") { // Windows drive letter followed by ':\'
        cp = 2;
    }
    while ((cp = expandedPath.find(sep, cp + 1)) != std::string::npos) {
       createDirectory(expandedPath.substr(0, cp));
    }
    createDirectory(expandedPath);
}

std::string defaultExtension(const std::string& filename, const std::string& ext) {
    std::string extCopy = ext;
    bool force = (extCopy[0] == '*');
    if (force) extCopy = extCopy.substr(1);
    int dot = -1;
    int len = filename.length();
    for (int i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '.') dot = i;
        if (ch == '/' || ch == '\\') dot = -1;
    }
    if (dot == -1) {
        force = true;
        dot = len;
    }
    if (force) {
        return filename.substr(0, dot) + extCopy;
    } else {
        return filename;
    }
}

void deleteFile(const std::string& filename) {
    platform::filelib_deleteFile(expandPathname(filename));
}

std::string expandPathname(const std::string& filename) {
    return platform::filelib_expandPathname(filename);
}

bool fileExists(const std::string& filename) {
    return platform::filelib_fileExists(filename);
}

int fileSize(const std::string& filename) {
    std::ifstream input;
    input.open(filename.c_str(), std::ifstream::binary);
    if (input.fail()) {
        return -1;
    } else {
        input.seekg(0, std::ifstream::end);
        return (int) input.tellg();
    }
}

std::string findOnPath(const std::string& path, const std::string& filename) {
    std::ifstream stream;
    std::string result = openOnPath(stream, path, filename);
    if (result != "") stream.close();
    return result;
}

std::string getAbsolutePath(const std::string& path) {
    return platform::filelib_getAbsolutePath(path);
}

std::string getCurrentDirectory() {
    return platform::filelib_getCurrentDirectory();
}

std::string getDirectoryPathSeparator() {
    return platform::filelib_getDirectoryPathSeparator();
}

std::string getExtension(const std::string& filename) {
    int dot = -1;
    int len = filename.length();
    for (int i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '.') dot = i;
        if (ch == '/' || ch == '\\') dot = -1;
    }
    if (dot == -1) {
        return "";
    } else {
        return filename.substr(dot);
    }
}

std::string getHead(const std::string& filename) {
    size_t slash = std::string::npos;
    size_t len = filename.length();
    for (size_t i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '/' || ch == '\\') slash = i;
    }
    if (slash == std::string::npos) {
        return "";
    } else if (slash == 0) {
        return "/";
    } else {
        return filename.substr(0, slash);
    }
}

std::string getRoot(const std::string& filename) {
    int dot = -1;
    int len = filename.length();
    for (int i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '.') dot = i;
        if (ch == '/' || ch == '\\') dot = -1;
    }
    if (dot == -1) {
        return filename;
    } else {
        return filename.substr(0, dot);
    }
}

std::string getSearchPathSeparator() {
    return platform::filelib_getSearchPathSeparator();
}

std::string getTail(const std::string& filename) {
    size_t slash = std::string::npos;
    size_t len = filename.length();
    for (size_t i = 0; i < len; i++) {
        char ch = filename[i];
        if (ch == '/' || ch == '\\') slash = i;
    }
    if (slash == std::string::npos) {
        return filename;
    } else {
        return filename.substr(slash + 1);
    }
}

std::string getTempDirectory() {
    return platform::filelib_getTempDirectory();
}

bool isDirectory(const std::string& filename) {
    return platform::filelib_isDirectory(expandPathname(filename));
}

bool isFile(const std::string& filename) {
    return platform::filelib_isFile(expandPathname(filename));
}

bool isSymbolicLink(const std::string& filename) {
    return platform::filelib_isSymbolicLink(filename);
}

void listDirectory(const std::string& path, Vector<std::string>& list) {
    list.clear();
    return platform::filelib_listDirectory(expandPathname(path), list);
}

Vector<std::string> listDirectory(const std::string& path) {
    Vector<std::string> vec;
    listDirectory(path, vec);
    return vec;
}

bool matchFilenamePattern(const std::string& filename, const std::string& pattern) {
    return recursiveMatch(filename, 0, pattern, 0);
}

bool openFile(std::ifstream& stream, const std::string& filename) {
    stream.clear();
    stream.open(expandPathname(filename).c_str());
    return !stream.fail();
}

bool openFile(std::ofstream& stream, const std::string& filename) {
    stream.clear();
    stream.open(expandPathname(filename).c_str());
    return !stream.fail();
}

std::string openFileDialog(std::ifstream& stream) {
    return openFileDialog(stream, "Open File", "");
}

std::string openFileDialog(std::ifstream& stream,
                           const std::string& title) {
    return openFileDialog(stream, title, "");
}

std::string openFileDialog(std::ifstream& stream,
                           const std::string& title,
                           const std::string& path) {
    std::string filename = platform::file_openFileDialog(title, "load", path);
    if (filename == "") return "";
    stream.open(filename.c_str());
    return (stream.fail()) ? "" : filename;
}

std::string openFileDialog(const std::string& title,
                           const std::string& path) {
    std::string filename = platform::file_openFileDialog(title, "load", path);
    if (filename == "") return "";
    return (fileExists(filename)) ? filename : "";
}

std::string openFileDialog(std::ofstream& stream) {
    return openFileDialog(stream, "Open File", "");
}

std::string openFileDialog(std::ofstream& stream,
                           const std::string& title) {
    return openFileDialog(stream, title, "");
}

std::string openFileDialog(std::ofstream& stream,
                           const std::string& title,
                           const std::string& path) {
    std::string filename = platform::file_openFileDialog(title, "save", path);
    if (filename == "") return "";
    stream.open(filename.c_str());
    return (stream.fail()) ? "" : filename;
}

std::string openOnPath(std::ifstream& stream,
                       const std::string& path,
                       const std::string& filename) {
    Vector<std::string> paths;
    splitPath(path, paths);
    for (std::string dir : paths) {
        std::string pathname = dir + "/" + filename;
        if (openFile(stream, pathname)) return pathname;
    }
    return "";
}

std::string openOnPath(std::ofstream& stream,
                       const std::string& path,
                       const std::string& filename) {
    Vector<std::string> paths;
    splitPath(path, paths);
    for (std::string dir : paths) {
        std::string pathname = dir + "/" + filename;
        if (openFile(stream, pathname)) return pathname;
    }
    return "";
}

std::string promptUserForFile(std::ifstream& stream,
                              const std::string& prompt,
                              const std::string& reprompt) {
    std::string promptCopy = prompt;
    std::string repromptCopy = reprompt;
    if (reprompt == "") {
        repromptCopy = "Unable to open that file.  Try again.";
    }
    appendSpace(promptCopy);
    while (true) {
        std::cout << promptCopy;
        std::string filename;
        getline(std::cin, filename);
        if (!filename.empty()) {
            openFile(stream, filename);
            if (!stream.fail()) return filename;
            stream.clear();
        }
        std::cout << repromptCopy << std::endl;
        if (promptCopy == "") promptCopy = "Input file: ";
    }
}

std::string promptUserForFile(std::ofstream& stream,
                              const std::string& prompt,
                              const std::string& reprompt) {
    std::string promptCopy = prompt;
    std::string repromptCopy = reprompt;
    if (reprompt == "") {
        repromptCopy = "Unable to open that file.  Try again.";
    }
    appendSpace(promptCopy);
    while (true) {
        std::cout << promptCopy;
        std::string filename;
        getline(std::cin, filename);
        if (!filename.empty()) {
            openFile(stream, filename);
            if (!stream.fail()) return filename;
            stream.clear();
        }
        std::cout << repromptCopy << std::endl;
        if (promptCopy == "") promptCopy = "Output file: ";
    }
}

std::string promptUserForFile(const std::string& prompt,
                              const std::string& reprompt) {
    std::string promptCopy = prompt;
    std::string repromptCopy = reprompt;
    if (reprompt == "") {
        repromptCopy = "Unable to open that file.  Try again.";
    }
    appendSpace(promptCopy);
    while (true) {
        std::cout << promptCopy;
        std::string filename;
        getline(std::cin, filename);
        if (!filename.empty()) {
            std::ifstream stream;
            openFile(stream, filename);
            if (!stream.fail()) {
                stream.close();
                return filename;
            }
        }
        std::cout << repromptCopy << std::endl;
        if (promptCopy == "") promptCopy = "Input file: ";
    }
}

void readEntireFile(std::istream& is, Vector<std::string>& lines) {
    lines.clear();
    while (true) {
        std::string line;
        getline(is, line);
        if (is.fail()) break;
        lines.add(line);
    }
}

std::string readEntireFile(const std::string& filename) {
    std::string out;
    if (readEntireFile(filename, out)) {
        return out;
    } else {
        error(std::string("input file not found or cannot be opened: ") + filename);
        return "";
    }
}

bool readEntireFile(const std::string& filename, std::string& out) {
    std::ifstream input;
    input.open(filename.c_str());
    if (input.fail()) {
        return false;
    }
    readEntireStream(input, out);
    input.close();
    return true;
}

std::string readEntireStream(std::istream& input) {
    std::string out;
    readEntireStream(input, out);
    return out;
}

void readEntireStream(std::istream& input, std::string& out) {
    std::ostringstream output;
    while (true) {
        int ch = input.get();
        if (input.fail()) {
            break;
        }
        output.put(ch);
    }
    out = output.str();
}

void renameFile(const std::string& oldname, const std::string& newname) {
    std::string oldExpand = expandPathname(oldname);
    std::string newExpand = expandPathname(newname);
    rename(oldExpand.c_str(), newExpand.c_str());
}

void rewindStream(std::istream& input) {
    input.clear();                  // removes any current eof/failure flags
    input.seekg(0, std::ios::beg);  // tells the stream to seek back to the beginning
}

void setCurrentDirectory(const std::string& path) {
    return platform::filelib_setCurrentDirectory(path);
}

bool writeEntireFile(const std::string& filename,
                     const std::string& text,
                     bool append) {
    std::ofstream output;
    if (append) {
        output.open(filename.c_str(), std::ios_base::out | std::ios_base::app);
    } else {
        output.open(filename.c_str());
    }
    if (output.fail()) {
        return false;
    }
    output << text;
    output.close();
    return !output.fail();
}

/* Private functions */

static void splitPath(const std::string& path, Vector<std::string> list) {
    char sep = (path.find(';') == std::string::npos) ? ':' : ';';
    std::string pathCopy = path + sep;
    size_t start = 0;
    while (true) {
        size_t finish = pathCopy.find(sep, start);
        if (finish == std::string::npos) break;
        if (finish > start + 1) {
            list.add(pathCopy.substr(start, finish - start - 1));
        }
        start = finish + 1;
    }
}

static bool recursiveMatch(const std::string& str, int sx, const std::string& pattern, int px) {
    int slen = str.length();
    int plen = pattern.length();
    if (px == plen) return (sx == slen);
    char pch = pattern[px];
    if (pch == '*') {
        for (int i = sx; i <= slen; i++) {
            if (recursiveMatch(str, i, pattern, px + 1)) return true;
        }
        return false;
    }
    if (sx == slen) return false;
    char sch = str[sx];
    if (pch == '[') {
        bool match = false;
        bool invert = false;
        px++;
        if (px == plen) {
            error("matchFilenamePattern: missing ]");
        }
        if (pattern[px] == '^') {
            px++;
            invert = true;
        }
        while (px < plen && pattern[px] != ']') {
            if (px + 2 < plen && pattern[px + 1] == '-') {
                match |= (sch >= pattern[px] && sch <= pattern[px + 2]);
                px += 3;
            } else {
                match |= (sch == pattern[px]);
                px++;
            }
        }
        if (px == plen) {
            error("matchFilenamePattern: missing ]");
        }
        if (match == invert) return false;
    } else if (pch != '?') {
        if (pch != sch) return false;
    }
    return recursiveMatch(str, sx + 1, pattern, px + 1);
}

/*
 * File: bitstream.cpp
 * -------------------
 * This file contains the implementation of ibitstream and obitstream classes.
 * These classes are patterned after (and, in fact, inherit from) the standard
 * ifstream and ofstream classes.  Please see bitstream.h for information about
 * how a client properly uses these classes.
 *
 * @author Keith Schwarz, Eric Roberts, Marty Stepp
 * @version 2016/11/12
 * - made toPrintable non-static and visible
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * 2014/01/23
 * - added slightly more descriptive error messages e.g. in writeBit
 * - whitespace reformatting
 * Previously last modified on Mon May 21 19:50:00 PST 2012 by Keith Schwarz
 */

#define INTERNAL_INCLUDE 1
#include "bitstream.h"
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

static const int NUM_BITS_IN_BYTE = 8;

inline int GetNthBit(int n, int fromByte) {
    return ((fromByte & (1 << n)) != 0);
}

inline void SetNthBit(int n, int & inByte) {
    inByte |= (1 << n);
}

std::string toPrintable(int ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == ' ') {
        return "' '";
    } else if (ch == (int) PSEUDO_EOF) {
        return "EOF";
    } else if (ch == (int) NOT_A_CHAR) {
        return "NONE";
    } else if (!isgraph(ch)) {
        return "???";
    } else {
        return std::string("'") + (char) ch + std::string("'");
    }
}

/* Constructor ibitstream::ibitstream
 * ----------------------------------
 * Each ibitstream tracks 3 integers as private data.
 * "lastTell" is streampos of the last byte that was read (this is used
 * to detect when other non-readBit activity has changed the tell)
 * "curByte" contains contents of byte currently being read
 * "pos" is the bit position within curByte that is next to read
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next readBit will trigger a fresh read.
 */
ibitstream::ibitstream() : std::istream(nullptr), lastTell(0), curByte(0), pos(NUM_BITS_IN_BYTE) {
    this->fake = false;
}

/* Member function ibitstream::readBit
 * -----------------------------------
 * If bits remain in curByte, retrieve next and increment pos
 * Else if end of curByte (or some other read happened), then read next byte
 * and start reading from bit position 0 of that byte.
 * If read byte from file at EOF, return EOF.
 */
int ibitstream::readBit() {
    if (!is_open()) {
        error("ibitstream::readBit: Cannot read a bit from a stream that is not open.");
    }

    if (this->fake) {
        int bit = get();
        if (bit == 0 || bit == '0') {
            return 0;
        } else {
            return 1;
        }
    } else {
        // if just finished bits from curByte or if data read from stream after last readBit()
        if (lastTell != tellg() || pos == NUM_BITS_IN_BYTE) {
            if ((curByte = get()) == EOF) {
                // read next single byte from file
                return EOF;
            }
            pos = 0; // start reading from first bit of new byte
            lastTell = tellg();
        }
        int result = GetNthBit(pos, curByte);
        pos++;   // advance bit position for next call to readBit
        return result;
    }
}

/* Member function ibitstream::rewind
 * ----------------------------------
 * Simply seeks back to beginning of file, so reading begins again
 * from start.
 */
void ibitstream::rewind() {
    if (!is_open()) {
        error("ibitstream::rewind: Cannot rewind stream that is not open.");
    }
    clear();
    seekg(0, std::ios::beg);
}

void ibitstream::setFake(bool fake) {
    this->fake = fake;
}

/* Member function ibitstream::size
 * --------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt reading, we also record cur streampos and
 * re-seek to there before returning.
 */
long ibitstream::size() {
    if (!is_open()) {
        error("ibitstream::size: Cannot get size of stream which is not open.");
    }
    clear();                    // clear any error state
    streampos cur = tellg();    // save current streampos
    seekg(0, std::ios::end);    // seek to end
    streampos end = tellg();    // get offset
    seekg(cur);                 // seek back to original pos
    return long(end);
}

/* Member function ibitstream::is_open
 * -----------------------------------
 * Default implementation of is_open has the stream always
 * open.  Subclasses can customize this if they'd like.
 */
bool ibitstream::is_open() {
    return true;
}

/* Constructor obitstream::obitstream
 * ----------------------------------
 * Each obitstream tracks 3 integers as private data.
 * "lastTell" is streampos of the last byte that was written (this is used
 * to detect when other non-writeBit activity has changed the tell)
 * "curByte" contains contents of byte currently being written
 * "pos" is the bit position within curByte that is next to write
 * We set initial state for lastTell and curByte to 0, then pos is
 * set at 8 so that next writeBit will start a new byte.
 */
obitstream::obitstream() : std::ostream(nullptr), lastTell(0), curByte(0), pos(NUM_BITS_IN_BYTE) {
    this->fake = false;
}

/* Member function obitstream::writeBit
 * ------------------------------------
 * If bits remain to be written in curByte, add bit into byte and increment pos
 * Else if end of curByte (or some other write happened), then start a fresh
 * byte at position 0.
 * We write the byte out for each bit (backing up to overwrite as needed), rather
 * than waiting for 8 bits.  This is because the client might make
 * 3 writeBit calls and then start using << so we can't wait til full-byte
 * boundary to flush any partial-byte bits.
 */
void obitstream::writeBit(int bit) {
    if (bit != 0 && bit != 1) {
        error(std::string("obitstream::writeBit: must pass an integer argument of 0 or 1. You passed the integer ")
              + toPrintable(bit) + " (" + std::to_string(bit) + ").");
    }
    if (!is_open()) {
        error("obitstream::writeBit: stream is not open");
    }

    if (this->fake) {
        put(bit == 1 ? '1' : '0');
    } else {
        // if just filled curByte or if data written to stream after last writeBit()
        if (lastTell != tellp() || pos == NUM_BITS_IN_BYTE) {
            curByte = 0;   // zero out byte for next writes
            pos = 0;       // start writing to first bit of new byte
        }

        if (bit) {
            // only need to change if bit needs to be 1 (byte starts already zeroed)
            SetNthBit(pos, curByte);
        }

        if (pos == 0 || bit) {   // only write if first bit in byte or changing 0 to 1
            if (pos != 0) {
                seekp(-1, std::ios::cur);   // back up to overwite if pos > 0
            }
            put(curByte);
        }

        pos++; // advance to next bit position for next write
        lastTell = tellp();
    }
}

void obitstream::setFake(bool fake) {
    this->fake = fake;
}

/* Member function obitstream::size
 * --------------------------------
 * Seek to file end and use tell to retrieve position.
 * In order to not disrupt writing, we also record cur streampos and
 * re-seek to there before returning.
 */
long obitstream::size() {
    if (!is_open()) {
        error("obitstream::size: stream is not open");
    }
    clear();                    // clear any error state
    streampos cur = tellp();    // save current streampos
    seekp(0, std::ios::end);    // seek to end
    streampos end = tellp();    // get offset
    seekp(cur);                 // seek back to original pos
    return long(end);
}

/* Member function obitstream::is_open
 * -----------------------------------
 * Default implementation of is_open has the stream always
 * open.  Subclasses can customize this if they'd like.
 */
bool obitstream::is_open() {
    return true;
}

/* Constructor ifbitstream::ifbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to read data
 * from disk.
 */
ifbitstream::ifbitstream() {
    init(&fb);
}

/* Constructor ifbitstream::ifbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to read data
 * from disk, then opens the given file.
 */
ifbitstream::ifbitstream(const char* filename) {
    init(&fb);
    open(filename);
}
ifbitstream::ifbitstream(const std::string& filename) {
    init(&fb);
    open(filename);
}

/* Member function ifbitstream::open
 * ---------------------------------
 * Attempts to open the specified file, failing if unable
 * to do so.
 */
void ifbitstream::open(const char* filename) {
    if (!fb.open(filename, std::ios::in | std::ios::binary)) {
        setstate(std::ios::failbit);
    }
}

void ifbitstream::open(const std::string& filename) {
    open(filename.c_str());
}

/* Member function ifbitstream::is_open
 * ------------------------------------
 * Determines whether the file stream is open.
 */
bool ifbitstream::is_open() {
    return fb.is_open();
}

/* Member function ifbitstream::close
 * ----------------------------------
 * Closes the file stream, if one is open.
 */
void ifbitstream::close() {
    if (!fb.close()) {
        setstate(std::ios::failbit);
    }
}

/* Constructor ofbitstream::ofbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to write data
 * to disk.
 */
ofbitstream::ofbitstream() {
    init(&fb);
}

/* Constructor ofbitstream::ofbitstream
 * ------------------------------------
 * Wires up the stream class so that it knows to write data
 * to disk, then opens the given file.
 */
ofbitstream::ofbitstream(const char* filename) {
    init(&fb);
    open(filename);
}

ofbitstream::ofbitstream(const std::string& filename) {
    init(&fb);
    open(filename);
}

/* Member function ofbitstream::open
 * ---------------------------------
 * Attempts to open the specified file, failing if unable
 * to do so.
 */
void ofbitstream::open(const char* filename) {
    // Confirm we aren't about to do something that could potentially be a
    // Very Bad Idea.
    if (endsWith(filename, ".cpp") || endsWith(filename, ".h") ||
            endsWith(filename, ".hh") || endsWith(filename, ".cc")) {
        error(std::string("ofbitstream::open: It is potentially dangerous to write to file ")
              + filename + ", because that might be your own source code.  "
              + "We are explicitly disallowing this operation.  Please choose a "
              + "different filename.");
        setstate(std::ios::failbit);
    } else {
        if (!fb.open(filename, std::ios::out | std::ios::binary)) {
            setstate(std::ios::failbit);
        }
    }
}
void ofbitstream::open(const std::string& filename) {
    open(filename.c_str());
}

/* Member function ofbitstream::is_open
 * ------------------------------------
 * Determines whether the file stream is open.
 */
bool ofbitstream::is_open() {
    return fb.is_open();
}

/* Member function ofbitstream::close
 * ----------------------------------
 * Closes the given file.
 */
void ofbitstream::close() {
    if (!fb.close()) {
        setstate(std::ios::failbit);
    }
}

/* Constructor istringbitstream::istringbitstream
 * ----------------------------------------------
 * Sets the stream to use the string buffer, then sets
 * the initial string to the specified value.
 */
istringbitstream::istringbitstream(const std::string& s) {
    init(&sb);
    sb.str(s);
}

/* Member function istringbitstream::str
 * -------------------------------------
 * Sets the underlying string in the buffer to the
 * specified string.
 */
void istringbitstream::str(const std::string& s) {
    sb.str(s);
}

/* Member function ostringbitstream::ostringbitstream
 * --------------------------------------------------
 * Sets the stream to use the string buffer.
 */
ostringbitstream::ostringbitstream() {
    init(&sb);
}

/* Member function ostringbitstream::str
 * -------------------------------------
 * Retrives the underlying string data.
 */
std::string ostringbitstream::str() {
    return sb.str();
}

/*
 * File: tokenscanner.cpp
 * ----------------------
 * Implementation for the TokenScanner class.
 * 
 * @version 2016/11/26
 * - added getInput method
 * - replaced occurrences of string with const string& for efficiency
 * - alphabetized method ordering
 * - added operator << for printing a scanner
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "tokenscanner.h"
#include <cctype>
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "stack.h"
#undef INTERNAL_INCLUDE

TokenScanner::TokenScanner() {
    initScanner();
    setInput("");
}

TokenScanner::TokenScanner(std::istream& infile) {
    initScanner();
    setInput(infile);
}

TokenScanner::TokenScanner(const std::string& str) {
    initScanner();
    setInput(str);
}

TokenScanner::~TokenScanner() {
    if (stringInputFlag) {
        delete isp;
    }
}

void TokenScanner::addOperator(const std::string& op) {
    StringCell* cp = new StringCell;
    cp->str = op;
    cp->link = operators;
    operators = cp;
}

void TokenScanner::addWordCharacters(const std::string& str) {
    wordChars += str;
}

int TokenScanner::getChar() {
    return isp->get();
}

std::string TokenScanner::getInput() const {
    return buffer;
}

int TokenScanner::getPosition() const {
    if (!savedTokens) {
        return int(isp->tellg());
    } else {
        return int(isp->tellg()) - savedTokens->str.length();
    }
}

std::string TokenScanner::getStringValue(const std::string& token) const {
    std::string str = "";
    int start = 0;
    int finish = token.length();
    if (finish > 1 && (token[0] == '"' || token[0] == '\'')) {
        start = 1;
        finish--;
    }
    for (int i = start; i < finish; i++) {
        char ch = token[i];
        if (ch == '\\') {
            ch = token[++i];
            if (isdigit(ch) || ch == 'x') {
                int base = 8;
                if (ch == 'x') {
                    base = 16;
                    i++;
                }
                int result = 0;
                int digit = 0;
                while (i < finish) {
                    ch = token[i];
                    if (isdigit(ch)) {
                        digit = ch - '0';
                    } else if (isalpha(ch)) {
                        digit = toupper(ch) - 'A' + 10;
                    } else {
                        digit = base;
                    }
                    if (digit >= base) {
                        break;
                    }
                    result = base * result + digit;
                    i++;
                }
                ch = char(result);
                i--;
            } else {
                switch (ch) {
                case 'a': ch = '\a'; break;
                case 'b': ch = '\b'; break;
                case 'f': ch = '\f'; break;
                case 'n': ch = '\n'; break;
                case 'r': ch = '\r'; break;
                case 't': ch = '\t'; break;
                case 'v': ch = '\v'; break;
                case '"': ch = '"'; break;
                case '\'': ch = '\''; break;
                case '\\': ch = '\\'; break;
                }
            }
        }
        str += ch;
    }
    return str;
}

TokenScanner::TokenType TokenScanner::getTokenType(const std::string& token) const {
    if (token.empty()) {
        return TokenType(EOF);
    }

    char ch = token[0];
    if (isspace(ch)) {
        return SEPARATOR;
    } else if (ch == '"' || (ch == '\'' && token.length() > 1)) {
        return STRING;
    } else if (isdigit(ch)) {
        return NUMBER;
    } else if (isWordCharacter(ch)) {
        return WORD;
    } else {
        return OPERATOR;
    }
}

bool TokenScanner::hasMoreTokens() {
    std::string token = nextToken();
    saveToken(token);
    return !token.empty();
}

void TokenScanner::ignoreComments() {
    ignoreCommentsFlag = true;
}

void TokenScanner::ignoreWhitespace() {
    ignoreWhitespaceFlag = true;
}

bool TokenScanner::isWordCharacter(char ch) const {
    return isalnum(ch) || wordChars.find(ch) != std::string::npos;
}

std::string TokenScanner::nextToken() {
    if (savedTokens) {
        StringCell* cp = savedTokens;
        std::string token = cp->str;
        savedTokens = cp->link;
        delete cp;
        return token;
    }

    while (true) {
        if (ignoreWhitespaceFlag) {
            skipSpaces();
        }
        int ch = isp->get();
        if (ch == '/' && ignoreCommentsFlag) {
            ch = isp->get();
            if (ch == '/') {
                while (true) {
                    ch = isp->get();
                    if (ch == '\n' || ch == '\r' || ch == EOF) {
                        break;
                    }
                }
                continue;
            } else if (ch == '*') {
                int prev = EOF;
                while (true) {
                    ch = isp->get();
                    if (ch == EOF || (prev == '*' && ch == '/')) {
                        break;
                    }
                    prev = ch;
                }
                continue;
            }
            if (ch != EOF) {
                isp->unget();
            }
            ch = '/';
        }
        if (ch == EOF) {
            return "";
        }
        if ((ch == '"' || ch == '\'') && scanStringsFlag) {
            isp->unget();
            return scanString();
        }
        if (isdigit(ch) && scanNumbersFlag) {
            isp->unget();
            return scanNumber();
        }
        if (isWordCharacter(ch)) {
            isp->unget();
            return scanWord();
        }
        std::string op = std::string(1, ch);
        while (isOperatorPrefix(op)) {
            ch = isp->get();
            if (ch == EOF) {
                break;
            }
            op += ch;
        }
        while (op.length() > 1 && !isOperator(op)) {
            isp->unget();
            op.erase(op.length() - 1, 1);
        }
        return op;
    }
}

void TokenScanner::saveToken(const std::string& token) {
    StringCell* cp = new StringCell;
    cp->str = token;
    cp->link = savedTokens;
    savedTokens = cp;
}

void TokenScanner::scanNumbers() {
    scanNumbersFlag = true;
}

void TokenScanner::scanStrings() {
    scanStringsFlag = true;
}

void TokenScanner::setInput(std::istream& infile) {
    stringInputFlag = false;
    isp = &infile;
    savedTokens = nullptr;
}

void TokenScanner::setInput(const std::string& str) {
    stringInputFlag = true;
    buffer = str;
    isp = new std::istringstream(buffer);
    savedTokens = nullptr;
}

void TokenScanner::ungetChar(int) {
    isp->unget();
}

void TokenScanner::verifyToken(const std::string& expected) {
    std::string token = nextToken();
    if (token != expected) {
        std::string msg = "TokenScanner::verifyToken: Found \"" + token + "\""
                + " when expecting \"" + expected + "\"";
        if (!buffer.empty()) {
            msg += "\ninput = \"" + buffer + "\"";
        }
        error(msg);
    }
}

/* Private methods */

void TokenScanner::initScanner() {
    ignoreWhitespaceFlag = false;
    ignoreCommentsFlag = false;
    scanNumbersFlag = false;
    scanStringsFlag = false;
    operators = nullptr;
}

/*
 * Implementation notes: isOperator, isOperatorPrefix
 * --------------------------------------------------
 * These methods search the list of operators and return true if the
 * specified operator is either in the list or a prefix of an operator
 * in the list, respectively.  This code could be made considerably more
 * efficient by implementing operators as a trie.
 */
bool TokenScanner::isOperator(const std::string& op) {
    for (StringCell *cp = operators; cp != nullptr; cp = cp->link) {
        if (op == cp->str) {
            return true;
        }
    }
    return false;
}

bool TokenScanner::isOperatorPrefix(const std::string& op) {
    for (StringCell* cp = operators; cp != nullptr; cp = cp->link) {
        if (startsWith(cp->str, op)) {
            return true;
        }
    }
    return false;
}

/*
 * Implementation notes: scanNumber
 * --------------------------------
 * Reads characters until the scanner reaches the end of a legal number.
 * The function operates by simulating what computer scientists
 * call a finite-state machine.  The program uses the variable
 * <code>state</code> to record the history of the process and
 * determine what characters would be legal at this point in time.
 */
std::string TokenScanner::scanNumber() {
    std::string token = "";
    NumberScannerState state = INITIAL_STATE;
    while (state != FINAL_STATE) {
        int ch = isp->get();
        switch (state) {
        case INITIAL_STATE:
            if (!isdigit(ch)) {
                error("TokenScanner::scanNumber: internal error: illegal call");
            }
            state = BEFORE_DECIMAL_POINT;
            break;
        case BEFORE_DECIMAL_POINT:
            if (ch == '.') {
                state = AFTER_DECIMAL_POINT;
            } else if (ch == 'E' || ch == 'e') {
                state = STARTING_EXPONENT;
            } else if (!isdigit(ch)) {
                if (ch != EOF) {
                    isp->unget();
                }
                state = FINAL_STATE;
            }
            break;
        case AFTER_DECIMAL_POINT:
            if (ch == 'E' || ch == 'e') {
                state = STARTING_EXPONENT;
            } else if (!isdigit(ch)) {
                if (ch != EOF) {
                    isp->unget();
                }
                state = FINAL_STATE;
            }
            break;
        case STARTING_EXPONENT:
            if (ch == '+' || ch == '-') {
                state = FOUND_EXPONENT_SIGN;
            } else if (isdigit(ch)) {
                state = SCANNING_EXPONENT;
            } else {
                if (ch != EOF) {
                    isp->unget();
                }
                isp->unget();
                state = FINAL_STATE;
            }
            break;
        case FOUND_EXPONENT_SIGN:
            if (isdigit(ch)) {
                state = SCANNING_EXPONENT;
            } else {
                if (ch != EOF) {
                    isp->unget();
                }
                isp->unget();
                isp->unget();
                state = FINAL_STATE;
            }
            break;
        case SCANNING_EXPONENT:
            if (!isdigit(ch)) {
                if (ch != EOF) {
                    isp->unget();
                }
                state = FINAL_STATE;
            }
            break;
        default:
            state = FINAL_STATE;
            break;
        }
        if (state != FINAL_STATE) {
            token += char(ch);
        }
    }
    return token;
}

/*
 * Implementation notes: scanString
 * --------------------------------
 * Reads and returns a quoted string from the scanner, continuing until
 * it scans the matching delimiter.  The scanner generates an error if
 * there is no closing quotation mark before the end of the input.
 */
std::string TokenScanner::scanString() {
    std::string token = "";
    char delim = isp->get();
    token += delim;
    bool escape = false;
    while (true) {
        int ch = isp->get();
        if (ch == EOF) {
            error("TokenScanner::scanString: found unterminated string");
        }
        if (ch == delim && !escape) {
            break;
        }
        escape = (ch == '\\') && !escape;
        token += ch;
    }
    return token + delim;
}

/*
 * Implementation notes: scanWord
 * ------------------------------
 * Reads characters until the scanner reaches the end of a sequence
 * of word characters.
 */
std::string TokenScanner::scanWord() {
    std::string token = "";
    while (true) {
        int ch = isp->get();
        if (ch == EOF) {
            break;
        }
        if (!isWordCharacter(ch)) {
            isp->unget();
            break;
        }
        token += char(ch);
    }
    return token;
}

/*
 * Implementation notes: skipSpaces
 * --------------------------------
 * Advances the position of the scanner until the current character is
 * not a whitespace character.
 */
void TokenScanner::skipSpaces() {
    while (true) {
        int ch = isp->get();
        if (ch == EOF) {
            return;
        }
        if (!isspace(ch)) {
            isp->unget();
            return;
        }
    }
}

std::ostream& operator <<(std::ostream& out, const TokenScanner& scanner) {
    out << "TokenScanner{";
    bool first = true;
    if (!scanner.buffer.empty()) {
        out << "input=\"" << scanner.buffer << "\"";
        first = false;
    }
    out << (first ? "" : ",") << "position=" << scanner.getPosition();
    first = false;

    if (scanner.scanNumbersFlag) {
        out << (first ? "" : ",") << "scanNumbers";
        first = false;
    }
    if (scanner.scanStringsFlag) {
        out << (first ? "" : ",") << "scanStrings";
        first = false;
    }
    if (!scanner.wordChars.empty()) {
        out << (first ? "" : ",") << "wordChars=[" << scanner.wordChars << "]";
        first = false;
    }
    if (scanner.ignoreWhitespaceFlag) {
        out << (first ? "" : ",") << "ignoreWhitespace";
        first = false;
    }
    if (scanner.ignoreCommentsFlag) {
        out << (first ? "" : ",") << "ignoreComments";
        // first = false;
    }
    out << "}";
    return out;
}

/*
 * File: base64.cpp
 * ----------------
 * This file defines a set of functions for encoding and decoding binary data
 * in the base64 format, as declared in base64.h.  See:
 * http://en.wikipedia.org/wiki/Base64
 *
 * @author Marty Stepp, based upon open-source Apache Base64 en/decoder
 * @version 2017/10/18
 * - fixed compiler warnings
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * 2014/08/14
 * - Fixed bug with variables declared with deprecated 'register' keyword.
 * @since 2014/08/03
 */

#define INTERNAL_INCLUDE 1
#include "base64.h"
#undef INTERNAL_INCLUDE
#include <cstring>
#include <sstream>

/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] = {
    /* ASCII table */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode_len(const char *bufcoded) {
    int nbytesdecoded;
    const unsigned char *bufin;
    int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);

    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    return nbytesdecoded + 1;
}

int Base64decode(char *bufplain, const char *bufcoded) {
    int nbytesdecoded;
    const unsigned char *bufin;
    unsigned char *bufout;
    int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;

    while (nprbytes > 4) {
        *(bufout++) =
                (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
        *(bufout++) =
                (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
        *(bufout++) =
                (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
        bufin += 4;
        nprbytes -= 4;
    }

    /* Note: (nprbytes == 1) would be an error, so just ingore that case */
    if (nprbytes > 1) {
        *(bufout++) =
                (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
        *(bufout++) =
                (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
        *(bufout++) =
                (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len(int len) {
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode(char *encoded, const char *string, int len) {
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
        *p++ = basis_64[(string[i] >> 2) & 0x3F];
        *p++ = basis_64[((string[i] & 0x3) << 4) | ((string[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((string[i + 1] & 0xF) << 2) | ((string[i + 2] & 0xC0) >> 6)];
        *p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
        *p++ = basis_64[(string[i] >> 2) & 0x3F];
        if (i == (len - 1)) {
            *p++ = basis_64[((string[i] & 0x3) << 4)];
            *p++ = '=';
        }
        else {
            *p++ = basis_64[((string[i] & 0x3) << 4) | ((string[i + 1] & 0xF0) >> 4)];
            *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
        }
        *p++ = '=';
    }

    *p++ = '\0';
    return p - encoded;
}

namespace Base64 {
std::string encode(const std::string& s) {
    // make C char* buffer to store the encoded output (for compatibility)
    int len = Base64encode_len(s.length());
    char* buf = (char*) malloc(len);
    memset(buf, 0, len);
    Base64encode(buf, s.c_str(), s.length());
    
    // convert back into a C++ string, and return it
    // (unlike below in decode(), I can just directly construct the C++
    // string from the C one, because the Base64-encoded C string will
    // not contain any intermediate null bytes by definition)
    std::string result(buf);
    free(buf);
    return result;
}

std::string decode(const std::string& s) {
    // convert into C string and decode into that char* buffer
    const char* cstr = s.c_str();
    int len = Base64decode_len(cstr);
    char* buf = (char*) malloc(len);
    memset(buf, 0, len);
    Base64decode(buf, cstr);
    
    // read bytes from that buffer into a C++ string
    // (cannot just construct/assign C++ string from C char* buffer,
    // because that will terminate the string at the first null \0 byte)
    std::ostringstream out;
    for (int i = 0; i < len; i++) {
        out << buf[i];
    }
    std::string result = out.str();
    
    free(buf);
    return result;
}

} // namespace Base64

/*
 * File: simpio.cpp
 * ----------------
 * This file implements the simpio.h interface.
 * 
 * @version 2016/09/29
 * - added getDouble method
 * @version 2015/07/05
 * - increased visibility of appendSpace function used by various IO
 *   prompting functions (no longer static)
 * @version 2014/10/19
 * - alphabetized functions
 * - converted many funcs to take const string& rather than string for efficiency
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#define INTERNAL_INCLUDE 1
#include "simpio.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

STATIC_CONST_VARIABLE_DECLARE(std::string, GETCHAR_DEFAULT_PROMPT, "Enter a character: ")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETCHAR_DEFAULT_REPROMPT, "You must type a single character. Try again.")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETINTEGER_DEFAULT_PROMPT, "Enter an integer: ")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETINTEGER_DEFAULT_REPROMPT, "Illegal integer format. Try again.")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETREAL_DEFAULT_PROMPT, "Enter a number: ")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETREAL_DEFAULT_REPROMPT, "Illegal numeric format. Try again.")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETYESORNO_DEFAULT_PROMPT, "Try again: ")
STATIC_CONST_VARIABLE_DECLARE(std::string, GETYESORNO_DEFAULT_REPROMPT, "Please type a word that starts with 'Y' or 'N'.")

/*
 * Implementation notes: getChar, getDouble, getInteger, getReal
 * -------------------------------------------------------------
 * Each of these functions reads a complete input line and then uses the
 * <sstream> library to parse that line into a value of the desired type.
 * If that fails, the implementation asks the user for a new value.
 */

char getChar(const std::string& prompt,
             const std::string& reprompt) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    char value = '\0';
    while (true) {
        std::cout << promptCopy;
        std::string line;
        if (!getline(std::cin, line)) {
            error("getChar: End of input reached while waiting for character value.");
        }
        if (line.length() == 1) {
            value = line[0];
            break;
        }

        std::cout.flush();
        std::cerr << (reprompt.empty() ? STATIC_VARIABLE(GETCHAR_DEFAULT_REPROMPT) : reprompt) << std::endl;
        std::cerr.flush();
        if (promptCopy.empty()) {
            promptCopy = STATIC_VARIABLE(GETCHAR_DEFAULT_PROMPT);
        }
    }
    return value;
}

double getDouble(const std::string& prompt,
                 const std::string& reprompt) {
    return getReal(prompt, reprompt);
}

double getDoubleBetween(const std::string& prompt, double min, double max) {
    return getRealBetween(prompt, min, max);
}

int getInteger(const std::string& prompt,
               const std::string& reprompt) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    int value = 0;
    while (true) {
        std::cout << promptCopy;
        std::string line;
        if (!getline(std::cin, line)) {
            error("getInteger: End of input reached while waiting for integer value.");
        }
        trimInPlace(line);
        std::istringstream stream(line);
        stream >> value;
        if (!stream.fail() && stream.eof()) {
            break;
        }
        std::cout.flush();
        std::cerr << (reprompt.empty() ? STATIC_VARIABLE(GETINTEGER_DEFAULT_REPROMPT) : reprompt) << std::endl;
        std::cerr.flush();
        if (promptCopy.empty()) {
            promptCopy = STATIC_VARIABLE(GETINTEGER_DEFAULT_PROMPT);
        }
    }
    return value;
}

int getIntegerBetween(const std::string& prompt, int min, int max) {
    int value = 0;
    while (true) {
        value = getInteger(prompt);
        if (value < min || value > max) {
            std::cout.flush();
            std::cerr << "Please type a value between " << min
                      << " and " << max << "." << std::endl;
            std::cerr.flush();
        } else {
            break;
        }
    }
    return value;
}

/*
 * Implementation notes: getLine
 * -----------------------------
 * The getLine function simply combines the process of displaying a
 * prompt and reading an input line into a single call.  The primary
 * reason for including this function in the library is to ensure
 * that the process of reading integers, floating-point numbers, and
 * strings remains as consistent as possible.
 */
std::string getLine(const std::string& prompt) {
    std::string line;
    getLine(prompt, line);
    return line;
}

void getLine(const std::string& prompt,
             std::string& out) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    std::cout << promptCopy;
    if (!getline(std::cin, out)) {
        error("getLine: End of input reached while waiting for line.");
    }
}

void getLine(std::istream& input,
             std::string& out) {
    if (!getline(input, out)) {
        error("getLine: End of input reached while waiting for line.");
    }
}

double getReal(const std::string& prompt,
               const std::string& reprompt) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    double value = 0.0;
    while (true) {
        std::cout << promptCopy;
        std::string line;
        if (!getline(std::cin, line)) {
            error("getReal: End of input reached while waiting for real value.");
        }
        trimInPlace(line);
        std::istringstream stream(line);
        stream >> value;
        if (!stream.fail() && stream.eof()) {
            break;
        }
        std::cout.flush();
        std::cerr << (reprompt.empty() ? STATIC_VARIABLE(GETREAL_DEFAULT_REPROMPT) : reprompt) << std::endl;
        std::cerr.flush();
        if (promptCopy.empty()) {
            promptCopy = STATIC_VARIABLE(GETREAL_DEFAULT_PROMPT);
        }
    }
    return value;
}

double getRealBetween(const std::string& prompt, double min, double max) {
    double value = 0;
    while (true) {
        value = getReal(prompt);
        if (value < min || value > max) {
            std::cout.flush();
            std::cerr << "Please type a value between " << min
                      << " and " << max << "." << std::endl;
            std::cerr.flush();
        } else {
            break;
        }
    }
    return value;
}

bool getYesOrNo(const std::string& prompt,
                const std::string& reprompt,
                const std::string& defaultValue) {
    std::string promptCopy = prompt;
    appendSpace(promptCopy);
    bool value = false;
    while (true) {
        std::cout << promptCopy;
        std::string line;
        if (!getline(std::cin, line)) {
            error("getYesOrNo: End of input reached while waiting for yes/no value.");
        }
        if (line.empty()) {
            line = defaultValue;
        }
        if ((int) line.length() > 0) {
            char first = tolower(line[0]);
            if (first == 'y') {
                value = true;
                break;
            } else if (first == 'n') {
                value = false;
                break;
            }
        }
        std::cout.flush();
        std::cerr << (reprompt.empty() ? STATIC_VARIABLE(GETYESORNO_DEFAULT_REPROMPT) : reprompt) << std::endl;
        std::cerr.flush();
        if (promptCopy.empty()) {
            promptCopy = STATIC_VARIABLE(GETYESORNO_DEFAULT_PROMPT);
        }
    }
    return value;
}

void appendSpace(std::string& prompt) {
    if (!prompt.empty() && !isspace(prompt[prompt.length() - 1])) {
        prompt += ' ';
    }
}

/*
 * File: xmlutils.cpp
 * ------------------
 * This file contains implementations of utility functions related to processing
 * and extracting information from XML documents.
 * See xmlutils.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2016/10/22
 * - changed openXmlDocument to print error message rather than crash on file-not-found
 * @version 2016/10/14
 * - changed NULL to nullptr as appropriate
 * @version 2014/10/14
 * @since 2014/03/01
 */

#define INTERNAL_INCLUDE 1
#include "xmlutils.h"
#include <cstring>
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "rapidxml.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

namespace xmlutils {
int getAttributeInt(rapidxml::xml_node<>* node, const std::string& attrName, int defaultValue) {
    std::string value = getAttribute(node, attrName, std::to_string(defaultValue));
    return stringToInteger(value);
}

bool getAttributeBool(rapidxml::xml_node<>* node, const std::string& attrName, bool defaultValue) {
    std::string value = toLowerCase(getAttribute(node, attrName, boolToString(defaultValue)));
    return value == "true" || value == "t" || value == "1" || value == "on";
}

std::string getAttribute(rapidxml::xml_node<>* node, const std::string& attrName, const std::string& defaultValue) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    if (!attr) {
        return defaultValue;
    } else {
        // RapidXML stores its value strings in a funny way, as non-zero-terminated
        // strings indexed into the original char buffer you passed it,
        // along with a size field.
        char* cstrVal = attr->value();
        return std::string(cstrVal);
    }
}

Vector<rapidxml::xml_node<>*> getChildNodes(rapidxml::xml_node<>* node, const std::string& nodeName) {
    Vector<rapidxml::xml_node<>*> v;
    for (rapidxml::xml_node<>* childNode = node->first_node(nodeName.c_str());
         childNode != nullptr;
         childNode = childNode->next_sibling(nodeName.c_str())) {
        v.push_back(childNode);
    }
    return v;
}

bool hasAttribute(rapidxml::xml_node<>* node, const std::string& attrName) {
    rapidxml::xml_attribute<>* attr = node->first_attribute(attrName.c_str());
    return (attr != nullptr);
}

rapidxml::xml_node<>* openXmlDocument(const std::string& filename, const std::string& documentNode) {
    std::string xmlFileText = readEntireFile(filename);
    char* buf = new char[xmlFileText.length() + 1024]();   // *** memory leak (but MUST be heap-allocated)
    memset(buf, 0, xmlFileText.length() + 1024);
    strcpy(buf, xmlFileText.c_str());
    rapidxml::xml_document<char>* xmlDoc = new rapidxml::xml_document<char>;   // *** memory leak (but MUST be heap-allocated)
    xmlDoc->parse<NULL>(buf);
    rapidxml::xml_node<>* node = xmlDoc->first_node(documentNode.c_str());
    return node;
}
} // namespace xmlutils

/*
 * File: version.cpp
 * -----------------
 * This file implements functions for checking the version number of the
 * Stanford C++ library, its Java back-end, and the project properties
 * file (.pro).
 *
 * @author Marty Stepp 
 * @version 2016/09/24
 * - bug fix for std::quick_exit function
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/11/13
 * - ifdef'd out macroIntegerToDate function to remove unused function warning on some platforms
 * @since 2014/10/31
 */

#define INTERNAL_INCLUDE 1
#include "private/version.h"
#undef INTERNAL_INCLUDE
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace version {
#ifdef SPL_PROJECT_VERSION
/*
 * converts an 8-digit integer into a YYYY/MM/DD date string,
 * e.g. 20140907 -> "2014/09/07"
 * needed because I can't have a string as a -D command-line defined flag
 */
static std::string macroIntegerToDate(int macroInteger) {
    int year = (macroInteger / 10000);          // 2014
    int month = (macroInteger % 10000 / 100);   // 09
    int day = (macroInteger % 100);             // 07
    std::ostringstream out;
    out << std::setw(4) << std::setfill('0') << year
        << "/" << std::setw(2) << std::setfill('0') << month
        << "/" << std::setw(2) << std::setfill('0') << day;
    return out.str();
}
#endif // SPL_PROJECT_VERSION

static void ensureProjectVersionHelper(std::string minVersion) {
#ifdef SPL_PROJECT_VERSION
    std::string projectVersion = macroIntegerToDate(SPL_PROJECT_VERSION);
#else
    std::string projectVersion = "";
#endif

    if (projectVersion < minVersion) {
        fputs("\n", stderr);
        fputs("***\n", stderr);
        fputs("*** STANFORD C++ LIBRARY ERROR:\n", stderr);
        fputs("*** Your project's properties (from your .pro file) are too old\n", stderr);
        fputs("*** and are not compatible with this version of the C++ library.\n", stderr);
        fputs("*** Exiting immediately to avoid compatibility errors.\n", stderr);
        fputs("*** Please update your .pro to a newer compatible version.\n", stderr);
        fprintf(stderr, "*** Your .pro version found : %s\n", projectVersion.c_str());
        fprintf(stderr, "*** Minimum version accepted: %s\n", minVersion.c_str());
        fputs("***\n", stderr);
        fputs("\n", stderr);
        fflush(stderr);
        
        std::exit(1);
    }
}

void ensureJavaBackEndVersion(std::string /*minVersion*/) {
    // empty
}

void ensureProjectVersion(std::string minVersion) {
    if (minVersion.empty()) {
#ifdef SPL_MINIMUM_PROJECT_VERSION
        minVersion = SPL_MINIMUM_PROJECT_VERSION;
#else
        minVersion = STANFORD_CPP_PROJECT_MINIMUM_VERSION;
#endif
    }
    ensureProjectVersionHelper(minVersion);
}

std::string getCppLibraryDocsUrl() {
    return STANFORD_CPP_LIB_DOCS_URL;
}

std::string getCppLibraryVersion() {
    return STANFORD_CPP_LIB_VERSION;
}

std::string getJavaBackEndVersion() {
    return "?";
}

/*
 * Returns version of project properties as defined in .pro file.
 */
std::string getProjectVersion() {
#ifdef SPL_PROJECT_VERSION
    // 20141031 -> "2014/10/31"
    std::ostringstream out;
    out << (SPL_PROJECT_VERSION / 10000);         // 2014
    out << "/";
    out << (SPL_PROJECT_VERSION % 10000 / 100);   // 10
    out << "/";
    out << (SPL_PROJECT_VERSION % 100);           // 31
    return out.str();
#endif
    return "";
}

} // namespace version

/*
 * File: filelibwindows.cpp
 * ------------------------
 * This file contains Windows implementations of filelib.h primitives.
 * This code used to live in platform.cpp before the Java back-end was retired.
 *
 * @version 2018/10/23
 * - added getAbsolutePath
 */

#define INTERNAL_INCLUDE 1
#include "filelib.h"

// define all of the following only on Windows OS
// (see filelibunix.cpp for non-Windows versions)
#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#include <tchar.h>
#undef MOUSE_EVENT
#undef KEY_EVENT
#undef MOUSE_MOVED
#undef HELP_KEY
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <string>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

namespace platform {

void filelib_createDirectory(const std::string& path) {
    std::string pathStr = path;
    if (endsWith(path, "\\")) {
        pathStr = path.substr(0, path.length() - 1);
    }
    if (CreateDirectoryA(path.c_str(), nullptr) == 0) {
        if (GetLastError() == ERROR_ALREADY_EXISTS && filelib_isDirectory(pathStr)) {
            return;
        }
        error("createDirectory: Can't create " + path);
    }
}

void filelib_deleteFile(const std::string& path) {
    if(filelib_isDirectory(path)) {
        RemoveDirectoryA(path.c_str());
    } else {
        DeleteFileA(path.c_str());
    }
}

std::string filelib_expandPathname(const std::string& filename) {
    if (filename.empty()) {
        return "";
    }
    std::string filenameStr = filename;
    for (int i = 0, len = filename.length(); i < len; i++) {
        if (filenameStr[i] == '/') {
            filenameStr[i] = '\\';
        }
    }
    return filenameStr;
}

bool filelib_fileExists(const std::string& filename) {
    return GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES;
}

std::string filelib_getAbsolutePath(const std::string& path) {
    char realpathOut[4096];
    if (_fullpath(realpathOut, path.c_str(), 4095)) {
        std::string absPath(realpathOut);
        return absPath;
    } else {
        return path;
    }
}

std::string filelib_getCurrentDirectory() {
    char path[MAX_PATH + 1];
    int n = GetCurrentDirectoryA(MAX_PATH + 1, path);
    return std::string(path, n);
}

std::string filelib_getDirectoryPathSeparator() {
    return "\\";
}

std::string filelib_getSearchPathSeparator() {
    return ";";
}

std::string filelib_getTempDirectory() {
    char path[MAX_PATH + 1];
    int n = GetTempPathA(MAX_PATH + 1, path);
    return std::string(path, n);
}

bool filelib_isDirectory(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

// https://msdn.microsoft.com/en-us/library/windows/desktop/gg258117(v=vs.85).aspx
bool filelib_isFile(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

bool filelib_isSymbolicLink(const std::string& filename) {
    DWORD attr = GetFileAttributesA(filename.c_str());
    return attr != INVALID_FILE_ATTRIBUTES
            && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

void filelib_listDirectory(const std::string& path, Vector<std::string> & list) {
    std::string pathStr = path;
    if (pathStr == "") {
        pathStr = ".";
    }
    std::string pattern = pathStr + "\\*.*";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) {
        error("listDirectory: Can't list directory \"" + pathStr + "\"");
    }
    list.clear();
    while (true) {
        std::string name = std::string(fd.cFileName);
        if (name != "." && name != "..") {
            list.push_back(name);
        }
        if (!FindNextFileA(h, &fd)) {
            break;
        }
    }
    FindClose(h);
    sort(list.begin(), list.end());
}

std::string file_openFileDialog(const std::string& /*title*/,
                                const std::string& /*mode*/,
                                const std::string& /*path*/) {
    // TODO
    return "";
}

void filelib_setCurrentDirectory(const std::string& path) {
    if (!filelib_isDirectory(path) || !SetCurrentDirectoryA(path.c_str())) {
        error("setCurrentDirectory: Can't change to " + path);
    }
}

} // namespace platform

#endif // _WIN32

/*
 * File: multimain.cpp
 * -------------------
 * ...
 *
 * @version 2018/11/14
 * - added Cancel option to multimain popup dialog
 * @version 2018/10/18
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "private/multimain.h"
#include <iomanip>
#include <iostream>
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#define INTERNAL_INCLUDE 1
#include "filelib.h"
#define INTERNAL_INCLUDE 1
#include "goptionpane.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#define INTERNAL_INCLUDE 1
#include "simpio.h"
#undef INTERNAL_INCLUDE

// helper to store [name => main func pointer]
STATIC_VARIABLE_DECLARE_MAP_EMPTY(Map, std::string, GThunkInt, funcMap);

extern int main();

namespace stanfordcpplib {

// whether the popup list of mains should be graphical
static bool& mainSelectorIsGraphical() {
    static bool _isGraphical = false;
    return _isGraphical;
}

int registerMainFunction(const std::string& name, int (*mainFunc)()) {
    GThunkInt mainThunk = mainFunc;
    STATIC_VARIABLE(funcMap)[name] = mainThunk;
    return 0;
}

MultiMainRegisterStaticInitializer::MultiMainRegisterStaticInitializer(
        const std::string& name, int (* mainFunc)(), bool graphical) {
    STATIC_VARIABLE(funcMap)[name] = mainFunc;
    mainSelectorIsGraphical() |= graphical;
}

#ifndef SPL_AUTOGRADER_MODE

// function prototype declarations;
// I declare these rather than including init.h to avoid
// triggering library initialization if lib is not used
// (keep in sync with init.h/cpp)
extern void initializeLibrary(int argc, char** argv);
extern void runMainInThread(int (* mainFunc)(void));
extern void shutdownLibrary();

int selectMainFunction() {
    Map<std::string, GThunkInt>& funcMap = STATIC_VARIABLE(funcMap);
    if (funcMap.isEmpty()) {

        //////////////////////////////////////////////////////////////////////////
        /// NOTE TO STUDENT!                                                   ///
        /// If you are directed here by a compiler error,                      ///
        /// it means that you have not written a main function or that it      ///
        /// has the wrong parameters.                                          ///
        /// The heading of your main function must be:                         ///
        ///                                                                    ///
        /// int main() { ... }                                                 ///
        ///                                                                    ///
        /// (Our library secretly renames your main function to "qMain"        ///
        ///  so that we can actually control the main flow of execution.)      ///
        ///                                                                    ///
        /// You may also need to include a .h header from the Stanford         ///
        /// C++ library in the file of your project that contains the          ///
        /// 'main' function.                                                   ///
        //////////////////////////////////////////////////////////////////////////
        stanfordcpplib::runMainInThread(main);

    } else if (funcMap.size() == 1) {
        // just one main, so just run it
        std::string mainFuncName = funcMap.front();
        GThunkInt mainFunc = funcMap[mainFuncName];
        stanfordcpplib::runMainInThread(mainFunc);
    } else {
        // multiple mains; ask user which one to run
        if (stanfordcpplib::mainSelectorIsGraphical()) {
            GThunkInt mainFuncWrapper = [&funcMap]() {
                Vector<std::string> options;
                for (std::string functionName : funcMap) {
                    options.add(functionName);
                }

                // add a "Cancel" option so we can press Escape to abort
                options.add("Cancel");

                std::string choice = "";
                choice = GOptionPane::showOptionDialog(
                        /* message */ "main functions available:",
                        /* options */ options,
                        /* title   */ "Choose main function to run");
                if (choice.empty()) {
                    return 0;
                }
                GThunkInt mainFunc = funcMap[choice];
                if (mainFunc) {
                    return mainFunc();
                } else {
                    return 0;
                }
            };
            stanfordcpplib::runMainInThread(mainFuncWrapper);
        } else {
            // plain text console
            GThunkInt mainFuncWrapper = [&funcMap]() {
                // print list of mains
                std::cout << "main functions available:" << std::endl;
                int i = 1;
                Map<int, std::string> numberToName;
                for (std::string functionName : funcMap) {
                    std::cout << std::setw(2) << i << ") " << functionName << std::endl;
                    numberToName[i] = functionName;
                    i++;
                }
                int choice = getIntegerBetween("Your choice? ", 0, funcMap.size());
                if (choice == 0) {
                    return 0;
                }
                GThunkInt mainFunc = funcMap[numberToName[choice]];
                return mainFunc();
            };
            stanfordcpplib::runMainInThread(mainFuncWrapper);
        }
    }

    stanfordcpplib::shutdownLibrary();
    return 0;
}
#endif // SPL_AUTOGRADER_MODE

} // namespace stanfordcpplib

/*
 * File: init.cpp
 * --------------
 *
 * TODO
 *
 * @author Marty Stepp
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/08/28
 * - refactor to use stanfordcpplib namespace
 * @version 2018/08/27
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "private/init.h"
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32


namespace stanfordcpplib {

namespace qtgui {
extern void initializeQtGraphicalConsole();
extern void shutdownConsole();
}

static void parseArgsQt(int argc, char** argv);

STATIC_VARIABLE_DECLARE(bool, isExitEnabled, true)

bool exitEnabled() {
    return STATIC_VARIABLE(isExitEnabled);
}

// called automatically by real main() function;
// call to this is inserted by library init.h
// to be run in Qt GUI main thread
void initializeLibrary(int argc, char** argv) {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

#ifndef SPL_HEADLESS_MODE
    GThread::setMainThread();
#endif // SPL_HEADLESS_MODE

    parseArgsQt(argc, argv);

#ifndef SPL_HEADLESS_MODE
    // initialize the main Qt graphics subsystem
    QtGui::instance()->setArgs(argc, argv);
    QtGui::instance()->initializeQt();

    // initialize Qt graphical console (if student #included it)
    initializeQtGraphicalConsole();
#endif // SPL_HEADLESS_MODE
}

void initializeLibraryStudentThread() {
#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true, /* force */ true);
#endif
}

// this should be roughly the same code as platform.cpp's parseArgs function
static void parseArgsQt(int argc, char** argv) {
    if (argc <= 0) {
        return;
    }
    std::string arg0 = argv[0];
    exceptions::setProgramNameForStackTrace(argv[0]);
    // programName() = getRoot(getTail(arg0));

#ifndef _WIN32
    // on Mac only, may need to change folder because of app's nested dir structure
    size_t ax = arg0.find(".app/Contents/");
    if (ax != std::string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            std::string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str());
        }
    }
#endif // _WIN32

    char* noConsoleFlag = getenv("NOCONSOLE");
    if (noConsoleFlag && startsWith(std::string(noConsoleFlag), "t")) {
        return;
    }
}

// called automatically by real main() function;
// call to this is inserted by library init.h
// to be run in Qt main thread
#ifdef SPL_HEADLESS_MODE
void runMainInThread(int (* mainFunc)(void)) {
    mainFunc();
}

void runMainInThread(std::function<int()> mainFunc) {
    mainFunc();
}

void runMainInThreadVoid(void (* mainFuncVoid)(void)) {
    mainFuncVoid();
}

void runMainInThreadVoid(std::function<void()> mainFuncVoid) {
    mainFuncVoid();
}
#else // SPL_HEADLESS_MODE
void runMainInThread(int (* mainFunc)(void)) {
    QtGui::instance()->startBackgroundEventLoop(mainFunc);
}

void runMainInThread(std::function<int()> mainFunc) {
    QtGui::instance()->startBackgroundEventLoop(mainFunc);
}

void runMainInThreadVoid(void (* mainFuncVoid)(void)) {
    QtGui::instance()->startBackgroundEventLoopVoid(mainFuncVoid);
}

void runMainInThreadVoid(std::function<void()> mainFuncVoid) {
    QtGui::instance()->startBackgroundEventLoopVoid(mainFuncVoid);
}
#endif // SPL_HEADLESS_MODE

void setExitEnabled(bool enabled) {
    STATIC_VARIABLE(isExitEnabled) = enabled;
    // TODO: notify GConsoleWindow?
}

// shut down the Qt graphical console window;
// to be run in Qt main thread
void shutdownLibrary() {
#ifdef SPL_HEADLESS_MODE
    // empty
#else
    shutdownConsole();
#endif // SPL_HEADLESS_MODE
}

void staticInitializeLibrary() {
    // empty
}

} // namespace stanfordcpplib

namespace std {
void __stanfordcpplib__exitLibrary(int status) {
    if (stanfordcpplib::exitEnabled()) {
        // call std::exit (has been renamed)

#ifdef exit
#undef exit
        std::exit(status);
#define exit __stanfordcpplib__exitLibrary
#endif // exit

    } else {
        // not allowed to call exit(); produce error message
        std::ostringstream out;
        out << "Program tried to call exit(" << status << ") to quit. " << std::endl;
        out << "*** This function has been disabled; main should end through " << std::endl;
        out << "*** normal program control flow." << std::endl;
        error(out.str());
    }
}

} // namespace std

/*
 * File: filelibunix.cpp
 * ---------------------
 * This file contains Unix implementations of filelib.h primitives.
 * This code used to live in platform.cpp before the Java back-end was retired.
 *
 * @version 2018/10/23
 * - added getAbsolutePath
 */

#define INTERNAL_INCLUDE 1
#include "filelib.h"

// define all of the following only on non-Windows OS
// (see filelibwindows.cpp for Windows versions)
#ifndef _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <stdint.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ios>
#include <string>
#define INTERNAL_INCLUDE 1
#include "error.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#undef INTERNAL_INCLUDE

namespace platform {

void filelib_createDirectory(const std::string& path) {
    std::string pathStr = path;
    if (endsWith(path, "/")) {
        pathStr = path.substr(0, path.length() - 1);
    }
    if (mkdir(pathStr.c_str(), 0777) != 0) {
        if (errno == EEXIST && filelib_isDirectory(pathStr)) {
            return;
        }
        std::string msg = "createDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
    }
}

void filelib_deleteFile(const std::string& path) {
    remove(path.c_str());
}

std::string filelib_expandPathname(const std::string& filename) {
    if (filename == "") {
        return "";
    }
    int len = filename.length();
    std::string expanded = filename;
    if (expanded[0] == '~') {
        int spos = 1;
        while (spos < len && expanded[spos] != '\\' && expanded[spos] != '/') {
            spos++;
        }
        char *homedir = nullptr;
        if (spos == 1) {
            homedir = getenv("HOME");
            if (!homedir) {
                homedir = getpwuid(getuid())->pw_dir;
            }
        } else {
            struct passwd *pw = getpwnam(expanded.substr(1, spos - 1).c_str());
            if (!pw) {
                error("expandPathname: No such user");
            } else {
                homedir = pw->pw_dir;
            }
        }
        expanded = std::string(homedir) + expanded.substr(spos);
        len = expanded.length();
    }
    for (int i = 0; i < len; i++) {
        if (expanded[i] == '\\') {
            expanded[i] = '/';
        }
    }
    return expanded;
}

bool filelib_fileExists(const std::string& filename) {
    struct stat fileInfo;
    return stat(filename.c_str(), &fileInfo) == 0;
}

std::string filelib_getAbsolutePath(const std::string& path) {
    char realpathOut[4096];
    realpath(path.c_str(), realpathOut);
    std::string absPath(realpathOut);
    return absPath;
}

std::string filelib_getCurrentDirectory() {
    char currentDirBuf[4096] = {'\0'};
    char* cwd = getcwd(currentDirBuf, 4096 - 1);
    std::string result;
    if (cwd) {
        result = std::string(cwd);
    } else {
        error("getCurrentDirectory: " + std::string(strerror(errno)));
    }
    return result;
}

std::string filelib_getDirectoryPathSeparator() {
    return "/";
}

std::string filelib_getSearchPathSeparator() {
    return ":";
}

// http://stackoverflow.com/questions/8087805/
// how-to-get-system-or-user-temp-folder-in-unix-and-windows
std::string filelib_getTempDirectory() {
    char* dir = getenv("TMPDIR");
    if (!dir) dir = getenv("TMP");
    if (!dir) dir = getenv("TEMP");
    if (!dir) dir = getenv("TEMPDIR");
    if (!dir) return "/tmp";
    return dir;
}

bool filelib_isDirectory(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISDIR(fileInfo.st_mode) != 0;
}

bool filelib_isFile(const std::string& filename) {
    struct stat fileInfo;
    if (stat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISREG(fileInfo.st_mode) != 0;
}

bool filelib_isSymbolicLink(const std::string& filename) {
    struct stat fileInfo;
    if (lstat(filename.c_str(), &fileInfo) != 0) {
        return false;
    }
    return S_ISLNK(fileInfo.st_mode) != 0;
}

void filelib_listDirectory(const std::string& path, Vector<std::string>& list) {
    DIR* dir = opendir(path.empty() ? "." : path.c_str());
    if (!dir) {
        error(std::string("listDirectory: Can't open \"") + path + "\"");
    }
    list.clear();
    while (true) {
        struct dirent* ep = readdir(dir);
        if (!ep) {
            break;
        }
        std::string name = std::string(ep->d_name);
        if (name != "." && name != "..") {
            list.push_back(name);
        }
    }
    closedir(dir);
    sort(list.begin(), list.end());
}

std::string file_openFileDialog(const std::string& /*title*/,
                                const std::string& /*mode*/,
                                const std::string& /*path*/) {
    // TODO
    return "";
}

void filelib_setCurrentDirectory(const std::string& path) {
    if (chdir(path.c_str()) != 0) {
        std::string msg = "setCurrentDirectory: ";
        std::string err = std::string(strerror(errno));
        error(msg + err);
    }
}

} // namespace platform

#endif // _WIN32

/*
 * File: mainwrapper.cpp
 * ---------------------
 * This file contains a 'main' function definition that renames the student's
 * main function to 'qMain' and wraps it with a real 'main' function that
 * initializes the Stanford C++ library, then runs the student's main function
 * in its own thread.  This is necessary for the Qt version of the library to
 * function properly.
 *
 * If you receive compiler errors that direct you to this file, you may need to
 * include a .h header from the Stanford C++ library in the file of your project
 * that contains the 'main' function.
 *
 * @version 2018/10/18
 * - multi-main initial implementation
 * @version 2018/10/07
 * - bug fixes for autograder mode
 * @version 2018/09/23
 * - bug fixes for windows Qt main wrapper
 * @version 2018/09/17
 * - initial version
 */

#include "private/init.h"

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef SPL_AUTOGRADER_MODE
int qMain(int argc, char** argv);

// function prototype declarations;
// I declare these rather than including init.h to avoid
// triggering library initialization if lib is not used
// (keep in sync with init.h/cpp)
namespace stanfordcpplib {
extern void initializeLibrary(int argc, char** argv);
extern void runMainInThread(int (* mainFunc)(void));
extern int selectMainFunction();
extern void shutdownLibrary();
}

#ifndef QT_NEEDS_QMAIN
#undef main
int main(int argc, char** argv) {
    return qMain(argc, argv);
}
// keep in sync with definition in .pro file
#ifdef REPLACE_MAIN_FUNCTION
#define main qMain
#endif // REPLACE_MAIN_FUNCTION
#endif // QT_NEEDS_QMAIN

// initializes the Qt GUI library subsystems and Qt graphical console as needed
// (autograders will insert their own main wrapper)
int qMain(int argc, char** argv) {
    extern int main();
    stanfordcpplib::initializeLibrary(argc, argv);
    int result = stanfordcpplib::selectMainFunction();
    stanfordcpplib::shutdownLibrary();
    return result;
}
#endif // SPL_AUTOGRADER_MODE

