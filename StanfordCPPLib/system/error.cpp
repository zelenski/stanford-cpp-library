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

const char* ErrorException::what() const noexcept {
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
