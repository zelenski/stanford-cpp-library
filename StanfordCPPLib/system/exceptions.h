/*
 * File: exceptions.h
 * ------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * It also contains some wizardry to try to retrieve a stack trace when the
 * exception is thrown, though it is hard to consistently do this on all platforms.
 * 
 * @author Marty Stepp
 * @version 2014/11/12
 * - made printStackTrace function publicly available
 * - added top-level signal handler (for null-pointer derefs etc.)
 * @since 2014/11/05
 */

#ifndef _exceptions_h
#define _exceptions_h

#include <iostream>

namespace exceptions {
/*
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
std::string& getProgramNameForStackTrace();

/*
 * Returns whether the top-level exception handler is enabled.
 * Initially false.
 */
bool getTopLevelExceptionHandlerEnabled();

/*
 * Prints a stack trace to the given output stream.
 * Defaults to the system standard error console (cerr).
 * (Stack traces are highly OS- and compiler-dependent, so this function
 *  may not work perfectly on all platforms.  It has been tested to work
 *  on Linux with GCC/G++, Mac OS X with clang++, and Windows with MinGW.)
 */
void printStackTrace();
void printStackTrace(std::ostream& out);

/*
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
void setProgramNameForStackTrace(char* programName);

/*
 * Sets whether the top-level exception handler is enabled.
 */
void setTopLevelExceptionHandlerEnabled(bool enabled);
}

// free functions

/*
 * Returns number of calls deep we are in the current recursive function.
 * For example, if f() calls f() calls f(), this function returns 3.
 * NOTE: Doesn't usually work when used with static functions, because their names
 * are not exported or revealed to the internal stack trace grabber.
 * So if you want to use this function, consider making your function non-static.
 */
int getRecursionIndentLevel();

/*
 * Returns a string of indentation that can be used to pretty-print recursive calls
 * at their corresponding level of nesting.
 * Indents by 4 spaces per level but can be overridden by passing 'indenter' param.
 * NOTE: Doesn't usually work when used with static functions, because their names
 * are not exported or revealed to the internal stack trace grabber.
 * So if you want to use this function, consider making your function non-static.
 */
std::string recursionIndent(std::string indenter = "    ");

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _exceptions_h
