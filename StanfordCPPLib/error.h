/*
 * File: error.h
 * -------------
 * This file defines the <code>ErrorException</code> class and the
 * <code>error</code> function.
 */

#ifndef _error_h
#define _error_h

#include <string>
#include <exception>

/*
 * Class: ErrorException
 * ---------------------
 * This exception is thrown by calls to the <code>error</code>
 * function.  Typical code for catching errors looks like this:
 *
 *<pre>
 *    try {
 *       ... code in which an error might occur ...
 *    } catch (ErrorException & ex) {
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
    virtual const char *what() const throw ();

private:
    std::string msg;
};

/*
 * Thrown when a blocking I/O call is interrupted by closing the program.
 */
class InterruptedIOException : public std::exception {};

/*
 * Function: error
 * Usage: error(msg);
 * ------------------
 * Signals an error condition in a program by throwing an
 * <code>ErrorException</code> with the specified message.
 */

void error(std::string msg);

#include "private/main.h"

#endif
