/*
 * File: exceptions.h
 * ------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * It also contains some wizardry to try to retrieve a stack trace when the
 * exception is thrown, though it is hard to consistently do this on all platforms.
 * 
 * @author Marty Stepp
 * @version 2018/09/25
 * - add 'force' parameter to setTopLevelExceptionHandlerEnabled
 *   (helps it to work better with threads)
 * - added doc comments for new documentation generation
 * @version 2016/11/07
 * - added cleanupFunctionNameForStackTrace
 * @version 2016/10/30
 * - moved recursion functions to recursion.h/cpp
 * @version 2014/11/12
 * - made printStackTrace function publicly available
 * - added top-level signal handler (for null-pointer derefs etc.)
 * @since 2014/11/05
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _exceptions_h
#define _exceptions_h

#include <iostream>

namespace exceptions {
/**
 * Strips some extraneous text from a function's name/header to make it look
 * better when printed in a stack trace.
 * e.g. basic_string -> string, removes std::, removes some weird compiler gibberish.
 */
std::string cleanupFunctionNameForStackTrace(std::string function);

/**
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
std::string& getProgramNameForStackTrace();

/**
 * Returns whether the top-level exception handler is enabled.
 * Initially false.
 */
bool getTopLevelExceptionHandlerEnabled();

/**
 * Prints a stack trace to the system standard error console (cerr).
 * (Stack traces are highly OS- and compiler-dependent, so this function
 *  may not work perfectly on all platforms.  It has been tested to work
 *  on Linux with GCC/G++, Mac OS X with clang++, and Windows with MinGW.)
 */
void printStackTrace();

/**
 * Prints a stack trace to the given output stream.
 * Defaults to the system standard error console (cerr).
 * (Stack traces are highly OS- and compiler-dependent, so this function
 *  may not work perfectly on all platforms.  It has been tested to work
 *  on Linux with GCC/G++, Mac OS X with clang++, and Windows with MinGW.)
 */
void printStackTrace(std::ostream& out);

/**
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
void setProgramNameForStackTrace(char* programName);

/**
 * Sets whether the top-level exception handler is enabled.
 * If the optional 'force' parameter is passed, will set the handler again
 * even if it was set before.
 */
void setTopLevelExceptionHandlerEnabled(bool enabled, bool force = false);

/**
 * Whether the given function should be filtered out when displaying a stack trace.
 * Not meant to be called by clients.
 */
bool shouldFilterOutFromStackTrace(const std::string& function);
}

#endif // _exceptions_h
