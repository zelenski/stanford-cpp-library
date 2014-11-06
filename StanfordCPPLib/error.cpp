/*
 * File: error.cpp
 * ---------------
 * Implementation of the error function.
 * 
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "error.h"
#include <exception>
#include <string>
#include <iostream>

/* Definitions for the ErrorException class */

ErrorException::ErrorException(std::string msg) {
    this->msg = msg;
}

ErrorException::~ErrorException() throw () {
    /* Empty */
}

std::string ErrorException::getMessage() const {
    return msg;
}

const char *ErrorException::what() const throw () {
    // stepp : The original "Error: " prefix is commented out here,
    // because in many error cases, the attempt to do the string concatenation
    // ends up garbling the string and leading to garbage exception text
    // return ("Error: " + msg).c_str();
    return msg.c_str();
}

/*
 * Implementation notes: error
 * ---------------------------
 * Earlier implementations of error made it possible, at least on the
 * Macintosh, to help the debugger generate a backtrace at the point
 * of the error.  Unfortunately, doing so is no longer possible if
 * the errors are catchable.
 */

void error(std::string msg) {
    throw ErrorException(msg);
}
