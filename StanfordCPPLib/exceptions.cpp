#include <iostream>
#include <string>
#include "error.h"
#include "filelib.h"
#include "strlib.h"
#include "call_stack.h"

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

void setTopLevelExceptionHandlerEnabled(bool enabled) {
    if (!topLevelExceptionHandlerEnabled && enabled) {
        old_terminate = std::set_terminate(stanfordCppLibTerminateHandler);
    } else if (topLevelExceptionHandlerEnabled && !enabled) {
        std::set_terminate(old_terminate);
    }
    topLevelExceptionHandlerEnabled = enabled;
}

void print_stack_trace() {
    std::cerr << " *** Stack trace:" << std::endl;
    stacktrace::call_stack trace;
    std::vector<stacktrace::entry> entries = trace.stack;
    for (int i = 0; i < entries.size(); ++i) {
        stacktrace::entry entry = entries[i];
        entry.file = getTail(entry.file);
        
        stringReplaceInPlace(entry.function, "std::", "");
        
        // skip certain entries for clarity
        if (startsWith(entry.function, "__")
                || entry.function.find("stacktrace::") != std::string::npos
                || entry.function == "exceptions::print_stack_trace()"
                || entry.function == "exceptions::stanfordCppLibTerminateHandler()"
                || entry.function == "error(string)"
                || entry.function == "startupMain(int, char**)") {
            continue;
        }
        
        // show Main() as main() to hide hidden case-change by Stanford C++ lib internals
        if (startsWith(entry.function, "Main(")) {
            entry.function.replace(0, 5, "main(");
        }
        
        std::cerr << " ***     " << entry.function;
        std::string lineStr = "";
        if (entry.line > 0) {
            lineStr = ", near line " + integerToString(entry.line);
        } else if (!entry.lineStr.empty()) {
            lineStr = ", near " + entry.lineStr;
        }
        
        if (!lineStr.empty()) {
            std::cerr << lineStr;
        }
        std::cerr << std::endl;
        
        // don't show entries beneath the student's main() function, for simplicity
        if (entry.function == "main()") {
            break;
        }
    }
    std::cerr << " ***" << std::endl;
}

static void stanfordCppLibTerminateHandler() {
    std::ostream& out = std::cerr;
    try {
        throw;   // re-throws the exception that already occurred
    } catch (const ErrorException& ex) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An ErrorException occurred during program execution: \n";
        msg += " *** ";
        msg += ex.what();
        msg += "\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw ex;
    } catch (const InterruptedIOException& /* iex */) {
        // blocked console I/O was interrupted; just exit program immediately
        // (doesn't close any other JBE-generated GUI windows, but oh well)
        std::cout.flush();
        exit(0);
    } catch (const std::exception& ex) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An exception occurred during program execution: \n";
        msg += " *** ";
        msg += ex.what();
        msg += "\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw ex;
    } catch (std::string str) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A string exception occurred during program execution: \n";
        msg += " *** \"";
        msg += str;
        msg += "\"\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw str;
    } catch (char const* str) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A string exception occurred during program execution: \n";
        msg += " *** \"";
        msg += str;
        msg += "\"\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw str;
    } catch (int n) {
        char buf[128];
        snprintf(buf, 128, "%d", n);
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** An int exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw n;
    } catch (long l) {
        char buf[128];
        snprintf(buf, 128, "%ld", l);
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A long exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw l;
    } catch (char c) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A char exception occurred during program execution: \n";
        msg += " *** '";
        msg += c;
        msg += "'\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw c;
    } catch (bool b) {
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A bool exception occurred during program execution: \n";
        msg += " *** ";
        msg += (b ? "true" : "false");
        msg += "\n ***\n";
        std::cout.flush();
        out << msg;
        std::cout << msg;
        throw b;
    } catch (double d) {
        char buf[128];
        snprintf(buf, 128, "%lf", d);
        std::string msg = "\n ***\n";
        msg += " *** STANFORD C++ LIBRARY \n";
        msg += " *** A double exception occurred during program execution: \n";
        msg += " *** ";
        msg += buf;
        msg += "\n ***\n";
        std::cout.flush();
        out << msg;
        print_stack_trace();
        throw d;
    }
}

} // namespace exceptions
