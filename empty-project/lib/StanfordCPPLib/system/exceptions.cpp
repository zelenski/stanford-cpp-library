/*
 * File: exceptions.cpp
 * --------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * 
 * @author Marty Stepp
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

#include "exceptions.h"
#include <csignal>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "call_stack.h"
#include "error.h"
#include "filelib.h"
#include "strlib.h"
#include "private/static.h"
#ifdef _WIN32
#include <windows.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#endif
#include "private/platform.h"

// uncomment the definition below to use an alternative 'signal stack'
// which helps in handling stack overflow errors
// (disabled because it currently breaks stack traces for other errors)
//#define SHOULD_USE_SIGNAL_STACK

namespace exceptions {
// just some value that is not any existing signal
#define SIGSTACK ((int) 0xdeadbeef)
#define SIGUNKNOWN ((int) 0xcafebabe)
#define SIGTIMEOUT ((int) 0xf00df00d)

// static 'variables' (as functions to avoid initialization ordering bugs)
STATIC_CONST_VARIABLE_DECLARE(bool, STACK_TRACE_SHOULD_FILTER, true)
STATIC_CONST_VARIABLE_DECLARE(bool, STACK_TRACE_SHOW_TOP_BOTTOM_BARS, false)

STATIC_VARIABLE_DECLARE(std::string, programNameForStackTrace, "")
STATIC_VARIABLE_DECLARE(bool, topLevelExceptionHandlerEnabled, false)

#ifdef SPL_AUTOGRADER_MODE
STATIC_CONST_VARIABLE_DECLARE_COLLECTION(std::vector<int>, SIGNALS_HANDLED, SIGSEGV, SIGILL, SIGFPE, SIGINT)
#else
STATIC_CONST_VARIABLE_DECLARE_COLLECTION(std::vector<int>, SIGNALS_HANDLED, SIGSEGV, SIGILL, SIGFPE, SIGABRT)
#endif // SPL_AUTOGRADER_MODE

static void signalHandlerDisable();
static void signalHandlerEnable();
static void stanfordCppLibSignalHandler(int sig);
static void stanfordCppLibTerminateHandler();

std::string cleanupFunctionNameForStackTrace(std::string function) {
    // remove references to std:: namespace
    stringReplaceInPlace(function, "std::", "");
    stringReplaceInPlace(function, "__cxx11::", "");
    stringReplaceInPlace(function, "[abi:cxx11]", "");
    stringReplaceInPlace(function, "__1::", "");   // on Mac

    // a few substitutions related to predefined types for simplicity
    stringReplaceInPlace(function, "basic_ostream", "ostream");
    stringReplaceInPlace(function, "basic_istream", "istream");
    stringReplaceInPlace(function, "basic_ofstream", "ofstream");
    stringReplaceInPlace(function, "basic_ifstream", "ifstream");
    stringReplaceInPlace(function, "basic_string", "string");

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

void setTopLevelExceptionHandlerEnabled(bool enabled) {
    static void (* old_terminate)() = nullptr;

    if (!STATIC_VARIABLE(topLevelExceptionHandlerEnabled) && enabled) {
        old_terminate = std::set_terminate(stanfordCppLibTerminateHandler);
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
    } else if (STATIC_VARIABLE(topLevelExceptionHandlerEnabled) && !enabled) {
        std::set_terminate(old_terminate);
    }
    STATIC_VARIABLE(topLevelExceptionHandlerEnabled) = enabled;
}

/*
 * Some lines from the stack trace are filtered out because they come from
 * private library code or OS code and would confuse the student.
 */
bool shouldFilterOutFromStackTrace(const std::string& function) {
    // exact names that should be matched and filtered
    static const std::vector<std::string> FORBIDDEN_NAMES {
        "??",
        "_clone",
        "_start",
        "_Unwind_Resume",
        "error",
        "error(string)",
        "startupMain(int, char**)"
    };

    // substrings to filter (don't show any func whose name contains these)
    static const std::vector<std::string> FORBIDDEN_SUBSTRINGS {
        " error(",
        "_sigtramp",
        "autograderMain"
        "BaseThreadInitThunk",
        "crtexe.c",
        "ErrorException::ErrorException",
        "GetProfileString",
        "InitializeExceptionChain",
        "KnownExceptionFilter",
        "printStackTrace",
        "shouldFilterOutFromStackTrace",
        "stacktrace::",
        "stanfordCppLibPosixSignalHandler",
        "stanfordCppLibSignalHandler",
        "stanfordCppLibTerminateHandler",
        "testing::",
        "UnhandledException"
    };

    // prefixes to filter (don't show any func whose name starts with these)
    static const std::vector<std::string> FORBIDDEN_PREFIXES {
        "__"
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
    std::vector<stacktrace::entry> entries = trace.stack;
    
    // get longest line string length to line up stack traces
    void* fakeStackPtr = stacktrace::fakeCallStackPointer();
    int entriesToShowCount = 0;
    int funcNameLength = 0;
    int lineStrLength = 0;
    for (size_t i = 0; i < entries.size(); ++i) {
        entries[i].function = cleanupFunctionNameForStackTrace(entries[i].function);
        
        if (!STATIC_VARIABLE(STACK_TRACE_SHOULD_FILTER) || !shouldFilterOutFromStackTrace(entries[i].function)) {
            lineStrLength = std::max(lineStrLength, (int) entries[i].lineStr.length());
            funcNameLength = std::max(funcNameLength, (int) entries[i].function.length());
            entriesToShowCount++;
        }
    }
    
    if (entries.empty() || entriesToShowCount == 0) {
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
    
    for (size_t i = 0; i < entries.size(); ++i) {
        stacktrace::entry entry = entries[i];
        entry.file = getTail(entry.file);
        
        // skip certain entries for clarity
        if (STATIC_VARIABLE(STACK_TRACE_SHOULD_FILTER) && shouldFilterOutFromStackTrace(entry.function)) {
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

        std::string lineStr = "";
        if (!entry.lineStr.empty()) {
            lineStr = trimEnd(entry.lineStr);
            if (lineStr == "?? ??:0") {
                lineStr = "(unknown)";
            }
        } else if (entry.line > 0) {
            lineStr = "line " + integerToString(entry.line);
        }
        
        out << "*** " << std::left << std::setw(lineStrLength) << lineStr
                  << "  " << entry.function << std::endl;
        
        // don't show entries beneath the student's main() function, for simplicity
        if (entry.function == "main"
                || entry.function == "main()"
                || entry.function == "main(int, char**)") {
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
    if ((!std::string(kind).empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_KIND, (kind)); } \
    if ((!std::string(desc).empty())) { stringReplaceInPlace(msg, DEFAULT_EXCEPTION_DETAILS, (desc)); } \
    std::cout.flush(); \
    out << msg; \
    printStackTrace(out); \
    THROW_NOT_ON_WINDOWS(ex);

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
    // turn the signal handler off (should run only once; avoid infinite cycle)
    signalHandlerDisable();

    // tailor the error message to the kind of signal that occurred
    std::string SIGNAL_KIND = "A fatal error";
    std::string SIGNAL_DETAILS = "No details were provided about the error.";
    if (sig == SIGSEGV) {
        SIGNAL_KIND = "A segmentation fault";
        SIGNAL_DETAILS = "This typically happens when you try to dereference a pointer\n*** that is NULL or invalid.";
    } else if (sig == SIGABRT) {
        SIGNAL_KIND = "An abort error";
        SIGNAL_DETAILS = "This error is thrown by system functions that detect corrupt state.";
    } else if (sig == SIGILL) {
        SIGNAL_KIND = "An illegal instruction error";
        SIGNAL_DETAILS = "This typically happens when you have corrupted your program's memory.";
    } else if (sig == SIGFPE) {
        SIGNAL_KIND = "An arithmetic error";
        SIGNAL_DETAILS = "This typically happens when you divide by 0 or produce an overflow.";
    } else if (sig == SIGINT) {
        SIGNAL_KIND = "An interrupt error";
        SIGNAL_DETAILS = "This typically happens when your code timed out because it was stuck in an infinite loop.";
    } else if (sig == SIGSTACK) {
        SIGNAL_KIND = "A stack overflow";
        SIGNAL_DETAILS = "This can happen when you have a function that calls itself infinitely.";
    }
    
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
    raise(sig == SIGSTACK ? SIGABRT : sig);
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
        FILL_IN_EXCEPTION_TRACE(ex, "An ErrorException", insertStarsBeforeEachLine(ex.what()));
    } catch (const InterruptedIOException& /* iex */) {
        // blocked console I/O was interrupted; just exit program immediately
        // (doesn't close any other JBE-generated GUI windows, but oh well)
        std::cout.flush();
        std::exit(0);
    } catch (const std::exception& ex) {
        FILL_IN_EXCEPTION_TRACE(ex, "A C++ exception", insertStarsBeforeEachLine(ex.what()));
    } catch (std::string str) {
        FILL_IN_EXCEPTION_TRACE(str, "A string exception", insertStarsBeforeEachLine(str));
    } catch (char const* str) {
        FILL_IN_EXCEPTION_TRACE(str, "A string exception", insertStarsBeforeEachLine(str));
    } catch (int n) {
        FILL_IN_EXCEPTION_TRACE(n, "An int exception", integerToString(n));
    } catch (long l) {
        FILL_IN_EXCEPTION_TRACE(l, "A long exception", longToString(l));
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
}

} // namespace exceptions

