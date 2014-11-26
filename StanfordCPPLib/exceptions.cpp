/*
 * File: exceptions.cpp
 * --------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * 
 * @author Marty Stepp
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
#include "error.h"
#include "filelib.h"
#include "strlib.h"
#include "call_stack.h"
#ifdef _WIN32
#include <windows.h>
#  undef MOUSE_EVENT
#  undef KEY_EVENT
#  undef MOUSE_MOVED
#  undef HELP_KEY
#endif
#include "platform.h"

// uncomment the definition below to use an alternative 'signal stack'
// which helps in handling stack overflow errors
// (disabled because it currently breaks stack traces for other errors)
// #define SHOULD_USE_SIGNAL_STACK

namespace exceptions {
// just some value that is not any existing signal
#define SIGSTACK ((int) 0xdeadbeef)
#define SIGUNKNOWN ((int) 0xcafebabe)
#define SIGTIMEOUT ((int) 0xf00df00d)
static const bool STACK_TRACE_SHOULD_FILTER = true;
static const bool STACK_TRACE_SHOW_TOP_BOTTOM_BARS = false;
static bool topLevelExceptionHandlerEnabled = false;
static void (*old_terminate)() = NULL;
static std::string PROGRAM_NAME = "";
static std::vector<int> SIGNALS_HANDLED;

static void signalHandlerDisable();
static void signalHandlerEnable();
static void stanfordCppLibSignalHandler(int sig);
static void stanfordCppLibTerminateHandler();

std::string getProgramNameForStackTrace() {
    return PROGRAM_NAME;
}

bool getTopLevelExceptionHandlerEnabled() {
    return topLevelExceptionHandlerEnabled;
}

void setProgramNameForStackTrace(char* programName) {
    PROGRAM_NAME = programName;
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
        // stacktrace::setFakeCallStackPointer((void*) exceptionInfo->ContextRecord->Eip);
        stacktrace::setFakeCallStackPointer((void*) exceptionInfo);
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
    if (!topLevelExceptionHandlerEnabled && enabled) {
        old_terminate = std::set_terminate(stanfordCppLibTerminateHandler);
#ifdef _WIN32
        // disabling this code for now because it messes with the
        // newly added uncaught signal handler
        // SetErrorMode(SEM_NOGPFAULTERRORBOX);
        SetErrorMode(SEM_FAILCRITICALERRORS);
        // SetThreadErrorMode(SEM_FAILCRITICALERRORS, NULL);
        SetUnhandledExceptionFilter(UnhandledException);
        // _invalid_parameter_handler newHandler;
        // newHandler = myInvalidParameterHandler;
        // _set_invalid_parameter_handler(newHandler);
        //_set_error_mode(_OUT_TO_STDERR);
#endif // _WIN32
        
        // also set up a signal handler for things like segfaults / null-pointer-dereferences
        signalHandlerEnable();
    } else if (topLevelExceptionHandlerEnabled && !enabled) {
        std::set_terminate(old_terminate);
    }
    topLevelExceptionHandlerEnabled = enabled;
}

/*
 * Some lines from the stack trace are filtered out because they come from
 * private library code or OS code and would confuse the student.
 */
static bool shouldFilterOutFromStackTrace(const std::string& function) {
    return startsWith(function, "__")
            || function == "??"
            || function == "error(string)"
            || function == "error"
            || function == "startupMain(int, char**)"
            || function.find("stacktrace::") != std::string::npos
            || function.find("testing::") != std::string::npos
            || function.find("printStackTrace") != std::string::npos
            || function.find("stanfordCppLibSignalHandler") != std::string::npos
            || function.find("stanfordCppLibPosixSignalHandler") != std::string::npos
            || function.find("stanfordCppLibTerminateHandler") != std::string::npos
            || function.find("InitializeExceptionChain") != std::string::npos
            || function.find("BaseThreadInitThunk") != std::string::npos
            || function.find("GetProfileString") != std::string::npos
            || function.find("KnownExceptionFilter") != std::string::npos
            || function.find("UnhandledException") != std::string::npos
            || function.find("crtexe.c") != std::string::npos
            || function.find("_sigtramp") != std::string::npos
            || function.find("autograderMain") != std::string::npos;
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
    void* fakeStackPtr = stacktrace::getFakeCallStackPointer();
    int entriesToShowCount = 0;
    int funcNameLength = 0;
    int lineStrLength = 0;
    for (size_t i = 0; i < entries.size(); ++i) {
        // remove references to std:: namespace
        stringReplaceInPlace(entries[i].function, "std::", "");
        
        // a few substitutions related to predefined types for simplicity
        stringReplaceInPlace(entries[i].function, "basic_ostream", "ostream");
        stringReplaceInPlace(entries[i].function, "basic_istream", "istream");
        stringReplaceInPlace(entries[i].function, "basic_ofstream", "ofstream");
        stringReplaceInPlace(entries[i].function, "basic_ifstream", "ifstream");
        stringReplaceInPlace(entries[i].function, "basic_string", "string");
        
        // remove template arguments
        // TODO: does not work well for nested templates
        int lessThan = stringIndexOf(entries[i].function, "<");
        while (lessThan >= 0) {
            // see if there is a matching > for this <
            int greaterThan = lessThan + 1;
            int count = 1;
            while (greaterThan < (int) entries[i].function.length()) {
                if (entries[i].function[greaterThan] == '<') {
                    count++;
                } else if (entries[i].function[greaterThan] == '>') {
                    count--;
                    if (count == 0) {
                        break;
                    }
                }
                greaterThan++;
            }
            if (count == 0 && lessThan >= 0 && greaterThan > lessThan) {
                entries[i].function.erase(lessThan, greaterThan - lessThan + 1);
            } else {
                // look for the next < in the string, if any, to see if it has a matching >
                lessThan = stringIndexOf(entries[i].function, "<", lessThan + 1);
            }
        }
        
        if (!STACK_TRACE_SHOULD_FILTER || !shouldFilterOutFromStackTrace(entries[i].function)) {
            lineStrLength = std::max(lineStrLength, (int) entries[i].lineStr.length());
            funcNameLength = std::max(funcNameLength, (int) entries[i].function.length());
            entriesToShowCount++;
        }
    }
    
    if (entries.empty() || entriesToShowCount == 0) {
        return;   // couldn't get a stack trace, or had no useful data  :-(
    }
    
    if (lineStrLength > 0) {
        out << " *** Stack trace (line numbers are approximate):" << std::endl;
        if (STACK_TRACE_SHOW_TOP_BOTTOM_BARS) {
            out << " *** "
                      << std::setw(lineStrLength) << std::left
                      << "file:line" << "  " << "function" << std::endl;
            out << " *** "
                      << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
        }
    } else {
        out << " *** Stack trace:" << std::endl;
    }
    
    for (size_t i = 0; i < entries.size(); ++i) {
        stacktrace::entry entry = entries[i];
        entry.file = getTail(entry.file);
        
        // skip certain entries for clarity
        if (STACK_TRACE_SHOULD_FILTER && shouldFilterOutFromStackTrace(entry.function)) {
            continue;
        }
        
        // show Main() as main() to hide hidden case-change by Stanford C++ lib internals
        if (startsWith(entry.function, "Main(")) {
            entry.function.replace(0, 5, "main(");
        }
        
        std::string lineStr = "";
        if (!entry.lineStr.empty()) {
            lineStr = entry.lineStr;
            if (lineStr == "?? ??:0") {
                lineStr = "(unknown)";
            }
        } else if (entry.line > 0) {
            lineStr = "line " + integerToString(entry.line);
        }
        
        out << " *** " << std::left << std::setw(lineStrLength) << lineStr
                  << "  " << entry.function << std::endl;
        
        // don't show entries beneath the student's main() function, for simplicity
        if (entry.function == "main()") {
            break;
        }
    }
    if (entries.size() == 1 && entries[0].address == fakeStackPtr) {
        out << " *** (partial stack due to crash)" << std::endl;
    }

    if (STACK_TRACE_SHOW_TOP_BOTTOM_BARS && lineStrLength > 0) {
        out << " *** "
                  << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
    }
    
//    out << " ***" << std::endl;
//    out << " *** NOTE:" << std::endl;
//    out << " *** Any line numbers listed above are approximate." << std::endl;
//    out << " *** To learn more about why the program crashed, we" << std::endl;
//    out << " *** suggest running your program under the debugger." << std::endl;
    
    out << " ***" << std::endl;
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
    for (int sig : SIGNALS_HANDLED) {
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
    sigaltstack(&ss, NULL);
    
    struct sigaction sig_action = {};
    sig_action.sa_sigaction = stanfordCppLibPosixSignalHandler;
    sigemptyset(&sig_action.sa_mask);
#ifdef __APPLE__
    // backtrace() doesn't work on OS X when we use an alternate stack
    sig_action.sa_flags = SA_SIGINFO;
#else
    sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif // __APPLE__
    sigaction(SIGSEGV, &sig_action, NULL);
    sigaction(SIGFPE,  &sig_action, NULL);
    sigaction(SIGILL,  &sig_action, NULL);
    sigaction(SIGTERM, &sig_action, NULL);
#ifdef SPL_AUTOGRADER_MODE
    sigaction(SIGINT,  &sig_action, NULL);
#else // not SPL_AUTOGRADER_MODE
    sigaction(SIGABRT, &sig_action, NULL);
#endif // SPL_AUTOGRADER_MODE
    handled = true;
#endif
#endif // SHOULD_USE_SIGNAL_STACK

    SIGNALS_HANDLED.clear();
    SIGNALS_HANDLED.push_back(SIGSEGV);
    SIGNALS_HANDLED.push_back(SIGILL);
    SIGNALS_HANDLED.push_back(SIGFPE);
#ifdef SPL_AUTOGRADER_MODE
    SIGNALS_HANDLED.push_back(SIGINT);
#else // not SPL_AUTOGRADER_MODE
    SIGNALS_HANDLED.push_back(SIGABRT);
#endif // SPL_AUTOGRADER_MODE
    if (!handled) {
        for (int sig : SIGNALS_HANDLED) {
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
        SIGNAL_DETAILS = "This typically happens when you try to dereference a pointer\n *** that is NULL or invalid.";
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
    std::cerr << " ***" << std::endl;
    std::cerr << " *** STANFORD C++ LIBRARY" << std::endl;
    std::cerr << " *** " << SIGNAL_KIND << " occurred during program execution." << std::endl;
    std::cerr << " *** " << SIGNAL_DETAILS << std::endl;;
    std::cerr << " ***" << std::endl;;
    
//    if (sig != SIGSTACK) {
        exceptions::printStackTrace();
//    } else {
//        std::string line;
//        stacktrace::addr2line(stacktrace::getFakeCallStackPointer(), line);
//        std::cerr << " *** (unable to print stack trace because of stack memory corruption.)" << std::endl;
//        std::cerr << " *** " << line << std::endl;
//    }
    std::cerr.flush();
    raise(sig == SIGSTACK ? SIGABRT : sig);
}

/*
 * A general handler for any uncaught exception.
 * Prints details about the exception and then tries to print a stack trace.
 */
static void stanfordCppLibTerminateHandler() {
    signalHandlerDisable();   // don't want both a signal AND a terminate() call
    std::string DEFAULT_EXCEPTION_KIND = "An exception";
    std::string DEFAULT_EXCEPTION_DETAILS = "(unknown exception details)";
    
    std::string msg;
    msg += "\n";
    msg += " ***\n";
    msg += " *** STANFORD C++ LIBRARY \n";
    msg += " *** " + DEFAULT_EXCEPTION_KIND + " occurred during program execution: \n";
    msg += " *** " + DEFAULT_EXCEPTION_DETAILS + "\n";
    msg += " ***\n";
    
    std::ostream& out = std::cerr;
    try {
        throw;   // re-throws the exception that already occurred
    } catch (const ErrorException& ex) {
        FILL_IN_EXCEPTION_TRACE(ex, "An ErrorException", ex.what());
    } catch (const InterruptedIOException& /* iex */) {
        // blocked console I/O was interrupted; just exit program immediately
        // (doesn't close any other JBE-generated GUI windows, but oh well)
        std::cout.flush();
        exit(0);
    } catch (const std::exception& ex) {
        FILL_IN_EXCEPTION_TRACE(ex, "A C++ exception", ex.what());
    } catch (std::string str) {
        FILL_IN_EXCEPTION_TRACE(str, "A string exception", str);
    } catch (char const* str) {
        FILL_IN_EXCEPTION_TRACE(str, "A string exception", str);
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
