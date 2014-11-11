#include <iomanip>
#include <iostream>
#include <string>
#include "error.h"
#include "filelib.h"
#include "strlib.h"
#include "call_stack.h"
#ifdef _WIN32
#include <windows.h>
#endif

namespace exceptions {
static bool topLevelExceptionHandlerEnabled = false;
static void (*old_terminate)() = NULL;
static std::string PROGRAM_NAME = "";

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

LONG WINAPI UnhandledException(LPEXCEPTION_POINTERS /*exceptionInfo*/) {
    printf("In unhandled exception filter func\n");
    fflush(stdout);
    stanfordCppLibTerminateHandler();
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif // _WIN32

void setTopLevelExceptionHandlerEnabled(bool enabled) {
    if (!topLevelExceptionHandlerEnabled && enabled) {
        old_terminate = std::set_terminate(stanfordCppLibTerminateHandler);
#ifdef _WIN32

        SetUnhandledExceptionFilter(UnhandledException);
        _invalid_parameter_handler newHandler;
        newHandler = myInvalidParameterHandler;
        _set_invalid_parameter_handler(newHandler);
        // Disable the message box for assertions.
        //_CrtSetReportMode(_CRT_ASSERT, 0);

#endif // _WIN32
    } else if (topLevelExceptionHandlerEnabled && !enabled) {
        std::set_terminate(old_terminate);
    }
    topLevelExceptionHandlerEnabled = enabled;
}

static bool shouldFilterOutFromStackTrace(const std::string& function) {
    return startsWith(function, "__")
            || function == "error(string)"
            || function.find("stacktrace::") != std::string::npos
            || function.find("print_stack_trace") != std::string::npos
            || function.find("stanfordCppLibTerminateHandler") != std::string::npos
            || function.find("InitializeExceptionChain") != std::string::npos
            || function.find("BaseThreadInitThunk") != std::string::npos
            || function.find("crtexe.c") != std::string::npos
            || function == "startupMain(int, char**)";
}

void print_stack_trace() {
    stacktrace::call_stack trace;
    std::vector<stacktrace::entry> entries = trace.stack;
    
    // get longest line string length to line up stack traces
    const bool SHOULD_FILTER = true;
    const bool SHOW_TOP_BOTTOM_BARS = false;
    int funcNameLength = 0;
    int lineStrLength = 0;
    for (size_t i = 0; i < entries.size(); ++i) {
        // remove references to std:: namespace
        stringReplaceInPlace(entries[i].function, "std::", "");
        
        // remove template arguments
        while (stringContains(entries[i].function, "<") && stringContains(entries[i].function, ">")) {
            int lessThan = stringIndexOf(entries[i].function, "<");
            int greaterThan = stringIndexOf(entries[i].function, ">", lessThan);
            if (lessThan >= 0 && greaterThan > lessThan) {
                entries[i].function.erase(lessThan, greaterThan - lessThan + 1);
            }
        }
        
        if (!SHOULD_FILTER || !shouldFilterOutFromStackTrace(entries[i].function)) {
            lineStrLength = std::max(lineStrLength, (int) entries[i].lineStr.length());
            funcNameLength = std::max(funcNameLength, (int) entries[i].function.length());
        }
    }
    
    if (entries.empty()) {
        return;   // couldn't get a stack trace  :-(
    }
    
    if (lineStrLength > 0) {
        std::cerr << " *** Stack trace (line numbers are approximate):" << std::endl;
        if (SHOW_TOP_BOTTOM_BARS) {
            std::cerr << " *** "
                      << std::setw(lineStrLength) << std::left
                      << "file:line" << "  " << "function" << std::endl;
            std::cerr << " *** "
                      << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
        }
    } else {
        std::cerr << " *** Stack trace:" << std::endl;
    }
    
    for (size_t i = 0; i < entries.size(); ++i) {
        stacktrace::entry entry = entries[i];
        entry.file = getTail(entry.file);
        
        // skip certain entries for clarity
        if (SHOULD_FILTER && shouldFilterOutFromStackTrace(entry.function)) {
            continue;
        }
        
        // show Main() as main() to hide hidden case-change by Stanford C++ lib internals
        if (startsWith(entry.function, "Main(")) {
            entry.function.replace(0, 5, "main(");
        }
        
        std::string lineStr = "";
        if (!entry.lineStr.empty()) {
            lineStr = entry.lineStr;
        } else if (entry.line > 0) {
            lineStr = "line " + integerToString(entry.line);
        }
        
        std::cerr << " *** " << std::left << std::setw(lineStrLength) << lineStr
                  << "  " << entry.function << std::endl;
        
        // don't show entries beneath the student's main() function, for simplicity
        if (entry.function == "main()") {
            break;
        }
    }
    if (SHOW_TOP_BOTTOM_BARS && lineStrLength > 0) {
        std::cerr << " *** "
                  << std::string(lineStrLength + 2 + funcNameLength, '=') << std::endl;
    }
    
//    std::cerr << " ***" << std::endl;
//    std::cerr << " *** NOTE:" << std::endl;
//    std::cerr << " *** Any line numbers listed above are approximate." << std::endl;
//    std::cerr << " *** To learn more about why the program crashed, we" << std::endl;
//    std::cerr << " *** suggest running your program under the debugger." << std::endl;
    
    std::cerr << " ***" << std::endl;
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
    print_stack_trace(); \
    THROW_NOT_ON_WINDOWS(ex);

static void stanfordCppLibTerminateHandler() {
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
        FILL_IN_EXCEPTION_TRACE(d, "A bool exception", realToString(d));
    }
}

} // namespace exceptions
