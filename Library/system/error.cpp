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

#include "error.h"
#include "exceptions.h"
#include "strlib.h"
#include <sstream>

/* Definitions for the ErrorException class */

ErrorException::ErrorException(std::string msg)
        : _kind("error") {
    _msg = msg;
}

std::string ErrorException::getKind() const {
    return _kind;
}

std::string ErrorException::getMessage() const {
    return _msg;
}

void ErrorException::setKind(const std::string& kind) {
    _kind = kind;
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
