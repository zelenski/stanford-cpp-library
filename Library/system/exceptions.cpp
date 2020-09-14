/*
 * File: exceptions.cpp
 * --------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 *
 * @author Julie Zelenski
 * @version 2020/08/28
 * - stack trace harvesting removed, consolidate handling of fatal errors
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

#include "exceptions.h"
#include <csignal>
#include <string>
#include "error.h"
#include "strlib.h"
#include "private/static.h"
#include "qtgui.h"
#include <QCoreApplication> // for application name
#ifdef _WIN32
#include <windows.h>
#include <Debugapi.h>
#endif
#ifdef __GNUG__ // gnu C++ compiler
#include <cxxabi.h>
#endif


namespace exceptions {
// just some value that is not any existing signal
#define SIGSTACK (static_cast<int>(0xdeadbeef))
#define SIGUNKNOWN (static_cast<int>(0xcafebabe))

// static 'variables' (as functions to avoid initialization ordering bugs)
STATIC_VARIABLE_DECLARE(std::string, gProgramName, "")
STATIC_CONST_VARIABLE_DECLARE_COLLECTION(Vector<int>, SIGNALS_HANDLED, SIGSEGV, SIGILL, SIGFPE, SIGABRT)

static void signalHandlerDisable();
static void signalHandlerEnable();
static void stanfordCppLibSignalHandler(int sig);
[[noreturn]] static void stanfordCppLibTerminateHandler();

#ifdef __GNUG__ // gnu C++ compiler

static std::string demangle(const char* mangled_name)
{
    int status = -99;
    // name is malloc'ed and will leak, but we only demangle on terminate...
    char *name = __cxxabiv1::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
    return (status == 0 && name) ? name : mangled_name;
}

#else

static std::string demangle(const char* mangled_name) { return mangled_name; }

#endif // _GNUG_

#if _WIN32

void interruptIfDebug()
{
    if (IsDebuggerPresent())
        DebugBreak();
}

#else

enum status_t { DBG_UNKNOWN = -1, DBG_NO, DBG_YES };
static status_t gStatus = DBG_UNKNOWN;

static void local_handler(int)
{
    gStatus = DBG_NO;
    signal(SIGTRAP, SIG_DFL); // reset to default handler
}

void interruptIfDebug()
{
    if (gStatus == DBG_UNKNOWN) { // first time through
        gStatus = DBG_YES;  // assume debugger unless we learn otherwise
        signal(SIGTRAP, local_handler); // install our signal handler
        raise(SIGTRAP); // raise; if our handler receives signal, there is no IsDebuggerPresent
                        // if debugger then it will receive it
    } else if (gStatus == DBG_YES) {
        raise(SIGTRAP);
    }
}

#endif

bool getTopLevelExceptionHandlerEnabled() {
    return std::get_terminate() == stanfordCppLibTerminateHandler;
}

std::string& getProgramName() {
    return STATIC_VARIABLE(gProgramName);
}

void setProgramName(char* programName) {
    STATIC_VARIABLE(gProgramName) = programName;
}

#ifdef _WIN32

LONG WINAPI UnhandledException(LPEXCEPTION_POINTERS exceptionInfo) {
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

    if (enabled) {
        void (* previous)() = std::set_terminate(stanfordCppLibTerminateHandler);
        if (previous != stanfordCppLibTerminateHandler) {
            old_terminate = previous;
        }
#ifdef _WIN32
        SetErrorMode(SEM_FAILCRITICALERRORS);
        SetUnhandledExceptionFilter(UnhandledException);
#endif // _WIN32
        signalHandlerEnable(); // to catch segfault / abort / fpe
    } else {
        std::set_terminate(old_terminate);
        signalHandlerDisable();
    }
}

static void signalHandlerDisable() {
    for (int sig : STATIC_VARIABLE(SIGNALS_HANDLED)) {
        signal(sig, SIG_DFL);
    }
}

static void signalHandlerEnable() {
#if _WIN32
    for (int sig : STATIC_VARIABLE(SIGNALS_HANDLED)) {
        signal(sig, stanfordCppLibSignalHandler); // Windows only signal, not sigaction, sigh
    }
#else
    struct sigaction action;
    action.sa_handler = stanfordCppLibSignalHandler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    // mask other signals during handler
    for (int sig : STATIC_VARIABLE(SIGNALS_HANDLED)) {
        sigaddset(&action.sa_mask, sig);
    }
    // use same action for all handled signals
    for (int sig : STATIC_VARIABLE(SIGNALS_HANDLED)) {
        sigaction(sig, &action, 0);
    }
#endif
}

static void reportFatalEvent(std::string event, std::string details)
{
    std::string indent = "    "; // used for details to stand out from boilerplate

    std::cerr << std::endl;
    std::cerr <<"*** STANFORD C++ LIBRARY" << std::endl;
    std::cerr << "*** The " << QCoreApplication::applicationName().toStdString() << " program has terminated unexpectedly (crashed)" << std::endl;
    std::cerr << "*** " << event << std::endl;
    stringReplaceInPlace(details ,"\n", "\n" + indent);
    std::cerr << std::endl << indent << details << std::endl << std::endl;
    std::cerr << "*** To get more information about a program crash," << std::endl;
    std::cerr << "*** run your program again under the debugger." << std::endl;
    std::cerr << std::endl;
    std::cerr.flush();
}

/*
 * A general handler for process signals.
 * Prints details about the signal.
 */
static void stanfordCppLibSignalHandler(int sig) {
    signalHandlerDisable();

   // tailor the error message to the kind of signal that occurred
    std::string event = "A fatal error (signal " + std::to_string(sig) + ") was received";
    std::string details = "No details were provided about the error";


    if (sig == SIGSEGV) {
        event = "A segmentation fault (SIGSEGV) occurred";
        details = "This error indicates your program attempted to dereference a pointer\nto an invalid memory address (possibly out of bounds, deallocated, nullptr, ...)";
    } else if (sig == SIGABRT) {
        event = "An abort error (SIGABRT) was raised";
        details = "This error is raised by system functions that detect corrupt state";
    } else if (sig == SIGFPE) {
        event = "An arithmetic error (SIGFPE) occurred";
        details = "This error happens when dividing by zero or producing numeric overflow";
    } else if (sig == SIGSTACK) {
        event = "A stack overflow occurred";
        details = "This can happen when your program has infinite recursion";
    } else if (sig == SIGUSR1) {
        event = "Custom signal 1 was raised";
        details = "This can happen when your program produces infinite output";
    }
    event += " during program execution";

    reportFatalEvent(event, details);

    if (GThread::iAmRunningOnTheQtGuiThread()) {
        // no recovery possible if gui thread was the one who crashed
        raise(sig); // our signal handling has been disabled, default handler will abort
    } else {
        interruptIfDebug();
        stanfordcpplib::studentThreadHasExited("Terminated");
        native_thread_exit(); // exit this thread (no return), gui loop can live on
    }
}

/*
 * A general handler for any uncaught exception.
 * Prints details about the exception.
 */
[[noreturn]] static void stanfordCppLibTerminateHandler() {
    signalHandlerDisable();

    std::string event  ="An exception was thrown during program execution";
    std::string details = "(details of exception unknown)";

    try {
        throw;   // re-throws the exception that already occurred
    } catch (const ErrorException& ex) {
        event = "A fatal error was reported:";
        details = ex.what();
    } catch (const std::exception& ex) {
        event = "A C++ standard exception was thrown during program execution";
        details = demangle(typeid(ex).name()) + " " + ex.what();
    } catch (const std::string& str) {
        details = str;
    } catch (char const* str) {
        details = str;
    } catch (int n) {   // throw primitive type is unhelpful, but I suppose some might do it
        details = "int exception " + std::to_string(n);
    } catch (double d) {
        details = "double exception " + realToString(d);
    } catch (...) {
        event = "An unexpected exception was thrown during program execution";
        // use default details
    }

    reportFatalEvent(event, details);

    if (GThread::iAmRunningOnTheQtGuiThread()) {
        // no recovery possible if gui thread was the one who crashed
        abort();
    } else {
        interruptIfDebug();
        stanfordcpplib::studentThreadHasExited("Terminated");
        native_thread_exit(); // exit this thread (no return), gui loop can live on
    }
}


} // namespace exceptions
