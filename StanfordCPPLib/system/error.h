/*
 * File: error.h
 * -------------
 * This file defines the <code>ErrorException</code> class and the
 * <code>error</code> function.
 *
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2017/11/29
 * - fix for undefined SIGUSR1 on Windows systems
 * @version 2016/11/29
 * - changed error() to accept const string& instead of string
 * @version 2016/11/23
 * - added operator << to print ErrorExceptions
 */

#ifndef _error_h
#define _error_h

#include <csignal>
#include <exception>
#include <iostream>
#include <string>

// bug fix for missing SIGUSR1 on some Windows systems
#ifndef SIGUSR1
#define SIGUSR1 10
#endif // SIGUSR2

/**
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
    virtual void dump() const;
    virtual void dump(std::ostream& out) const;
    virtual std::string getMessage() const;
    virtual std::string getStackTrace() const;
    virtual bool hasStackTrace() const;
    virtual const char* what() const throw ();

protected:
    void setStackTrace(const std::string& stackTrace);

private:
    std::string _msg;
    std::string _stackTrace;

    static std::string insertStarsBeforeEachLine(const std::string& s);
};

/**
 * Prints the error exception to the given output stream.
 */
std::ostream& operator <<(std::ostream& out, const ErrorException& ex);

// TODO: use [[noreturn]] or Q_NO_RETURN to indicate that function does not ever return
// (added in c++11; still incompatible with some compilers?)

/**
 * Signals an error condition in a program by throwing an
 * <code>ErrorException</code> with the specified message.
 */
/* [[noreturn]] */ void error(const std::string& msg);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _error_h

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE
