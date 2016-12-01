/*
 * File: error.h
 * -------------
 * This file defines the <code>ErrorException</code> class and the
 * <code>error</code> function.
 *
 * @version 2016/11/29
 * - changed error() to accept const string& instead of string
 * @version 2016/11/23
 * - added operator << to print ErrorExceptions
 */

#ifndef _error_h
#define _error_h

#include <exception>
#include <iostream>
#include <string>

/*
 * Class: ErrorException
 * ---------------------
 * This exception is thrown by calls to the <code>error</code>
 * function.  Typical code for catching errors looks like this:
 *
 *<pre>
 *    try {
 *       ... code in which an error might occur ...
 *    } catch (ErrorException& ex) {
 *       ... code to handle the error condition ...
 *    }
 *</pre>
 *
 * If an <code>ErrorException</code> is thrown at any point in the
 * range of the <code>try</code> (including in functions called from
 * that code), control will jump immediately to the error handler.
 */

class ErrorException : public std::exception {
public:
    ErrorException(std::string msg);
    virtual ~ErrorException() throw ();
    virtual std::string getMessage() const;
    virtual std::string getStackTrace() const;
    virtual const char* what() const throw ();

private:
    std::string msg;
    std::string stackTrace;
};

std::ostream& operator <<(std::ostream& out, const ErrorException& ex);

/*
 * Thrown when a blocking I/O call is interrupted by closing the program.
 */
class InterruptedIOException : public std::exception {
    // empty
};

/*
 * Function: error
 * Usage: error(msg);
 * ------------------
 * Signals an error condition in a program by throwing an
 * <code>ErrorException</code> with the specified message.
 *
 * TODO: use [[noreturn]] to indicate that function does not ever return
 * (added in c++11; still incompatible with some compilers?)
 */
/* [[noreturn]] */ void error(const std::string& msg);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif
