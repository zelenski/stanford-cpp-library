/*
 * File: error.cpp
 * ---------------
 * Implementation of the error function.
 */

#include <exception>
#include <string>
#include <iostream>
#include "error.h"
using namespace std;

/* Definitions for the ErrorException class */

ErrorException::ErrorException(string msg) {
   this->msg = msg;
}

ErrorException::~ErrorException() throw () {
   /* Empty */
}

string ErrorException::getMessage() const {
   return msg;
}

const char *ErrorException::what() const throw () {
   return ("Error: " + msg).c_str();
}

/*
 * Implementation notes: error
 * ---------------------------
 * Earlier implementations of error made it possible, at least on the
 * Macintosh, to help the debugger generate a backtrace at the point
 * of the error.  Unfortunately, doing so is no longer possible if
 * the errors are catchable.
 */

void error(string msg) {
   throw ErrorException(msg);
}
