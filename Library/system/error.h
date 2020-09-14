/*
 * File: error.h
 * -------------
 * This file defines the <code>ErrorException</code> class and the
 * <code>error</code> function.
 *
 * @version 2018/10/18
 * - added getKind for ErrorExceptions that wrap other types of errors
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

// bug fix for missing signals on some Windows systems
#ifndef SIGUSR1
#define SIGUSR1 10
#endif // SIGUSR1

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
    /**
     * Creates a new ErrorException with the given error message.
     */
    ErrorException(std::string msg);

    /**
     * Frees any memory allocated by the exception.
     */
    virtual ~ErrorException() noexcept = default;

    /**
     * Returns what kind of exception this is.
     * In general this returns "error", but in some cases we catch other kinds
     * of exceptions (like thrown ints or strings) and wrap them up as
     * ErrorExceptions. In such cases, the kind will be "int" or "string" etc.
     */
    virtual std::string getKind() const;

    /**
     * Returns the exception's error message as passed to its constructor.
     */
    virtual std::string getMessage() const;

    /**
     * Sets what kind of exception this is.
     * Default is "error".
     */
    void setKind(const std::string& kind);

    /**
     * Returns the exception's error message as a C string.
     */
    virtual const char* what() const noexcept;

private:
    std::string _kind;
    std::string _msg;
};

/**
 * Prints the error exception to the given output stream.
 */
std::ostream& operator <<(std::ostream& out, const ErrorException& ex);

/**
 * Signals an error condition in a program by throwing an
 * <code>ErrorException</code> with the specified message.
 */
[[noreturn]] void error(const std::string& msg);

#endif // _error_h
