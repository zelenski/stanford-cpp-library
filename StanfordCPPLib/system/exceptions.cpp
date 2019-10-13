/*
 * File: exceptions.cpp
 * --------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * 
 * @author Marty Stepp
 * @version 2019/05/16
 * - added more function names to filter from stack trace
 * @version 2019/04/16
 * - filter Qt/std thread methods from stack trace
 * @version 2019/04/02
 * - small fix for warning about -Wreturn-std-move on string exception
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
#include <cstdlib>
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
[[noreturn]] static void stanfordCppLibTerminateHandler();
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
    stringReplaceInPlace(function, "stanfordcpplib::collections::GenericSet", "Set");

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
#if _WIN64
    if (exceptionInfo && exceptionInfo->ContextRecord && exceptionInfo->ContextRecord->Rip) {
        stacktrace::fakeCallStackPointer() = (void*) exceptionInfo;
    }
#else
    if (exceptionInfo && exceptionInfo->ContextRecord && exceptionInfo->ContextRecord->Eip) {
        stacktrace::fakeCallStackPointer() = (void*) exceptionInfo;
    }
#endif // _WIN64
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
        "__func::",
        "__function::",
        "_endthreadex",
        "_Function_base::_Base_manager::",
        "_Function_handler",
        "_Internal_",
        "__invoke_impl",
        "__invoke_result::type",
        "__invoke_void",
        "__unexpected",
        "thread::_Invoker",
        "thread::_State_impl",
        "_M_invoke",
        "_sigtramp",
        "autograderMain",
        "BaseThreadInitThunk",
        "call_stack_gcc.cpp",
        "call_stack_windows.cpp",
        "CFRunLoopDoSource",
        "CFRunLoopRun",
        "CFRUNLOOP_IS",
        "crtexe.c",
        "decltype(forward",
        "ErrorException::ErrorException",
        "exceptions.cpp",
        "function::operator",
        "GetModuleFileName",
        "GetProfileString",
        // "GStudentThread::run",
        "GThreadQt::run",
        "GThreadQt::start",
        "GThreadStd::run",
        "GThreadStd::start",
        "InitializeExceptionChain",
        "KnownExceptionFilter",
        "M_invoke",
        "multimain.cpp",
        // "operator",
        "pthread_body",
        "pthread_start",
        "printStackTrace",
        // "QAbstractItemModel::",
        // "QAbstractProxyModel::",
        "QApplication::notify",
        "QApplicationPrivate::",
        "QCoreApplication::",
        "QGuiApplicationPrivate::",
        "QMetaMethod::",
        "QMetaObject::",
        "QObjectPrivate::",
        "qt_plugin_instance",
        "QtGui::startBackgroundEventLoop",
        // "QWidget::",
        "QWidgetBackingStore::",
        "QWindowSystemInterface::",
        "require::_errorMessage",
        "RunCurrentEventLoopInMode",
        "shouldFilterOutFromStackTrace",
        "stacktrace::",
        "stanfordCppLibPosixSignalHandler",
        "stanfordCppLibSignalHandler",
        "stanfordCppLibTerminateHandler",
        "stanfordCppLibUnexpectedHandler",
        "testing::",
        "UnhandledException",
        "WinMain@"
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
            std::ostringstream lineout;
            lineout << "*** " << std::setw(lineStrLength) << std::left
                    << "file:line" << "  " << "function" << std::endl
                    << "*** " << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
            out << lineout.str() << std::endl;
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
        
        // we use a temporary 'lineout' because cerr aggressively flushes on <<,
        // leading to awkward line breaks in the output pane
        std::ostringstream lineout;
        lineout << "*** " << std::left << std::setw(lineStrLength) << lineStr
                  << "  " << entry.function;
        out << lineout.str() << std::endl;
        
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
        std::ostringstream lineout;
        lineout << "*** " << std::string(lineStrLength + 2 + funcNameLength, '=');
        out << lineout.str() << std::endl;
    }
    
    out << "***" << std::endl;
    out << "*** To learn more about the crash, we strongly" << std::endl;
    out << "*** suggest running your program under the debugger." << std::endl;
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
    out << msg; \
    out.flush(); \
    printStackTrace(out); \
    THROW_NOT_ON_WINDOWS(ex); \
    }

#define FILL_IN_EXCEPTION_TRACE_AND_THROW_ERROREXCEPTION(ex, kind, desc) \
    {\
    std::string __kind = (kind); \
    std::string __desc = (desc); \
    if ((!__kind.empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_KIND, __kind); } \
    if ((!__desc.empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_DETAILS, __desc); } \
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
        SIGNAL_DETAILS = "This typically happens when you try to dereference a pointer\n*** that is null or invalid.";
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
    std::cerr << (std::string("*** ") + SIGNAL_KIND + " occurred during program execution.") << std::endl;
    std::cerr << (std::string("*** ") + SIGNAL_DETAILS) << std::endl;
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
[[noreturn]] static void stanfordCppLibTerminateHandler() {
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
    } catch (const std::string& str) {
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
        FILL_IN_EXCEPTION_TRACE(d, "A double exception", realToString(d));
    } catch (...) {
        std::string ex = "Unknown";
        FILL_IN_EXCEPTION_TRACE(ex, "An exception", std::string());
    }

    abort();   // terminate the program with a SIGABRT signal
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
        message = realToString(d);
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

