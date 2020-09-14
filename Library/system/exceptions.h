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


#ifndef _exceptions_h
#define _exceptions_h

#include <string>

namespace exceptions {

/**
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
std::string& getProgramName();

/**
 * Returns whether the top-level exception handler is enabled.
 */
bool getTopLevelExceptionHandlerEnabled();

/**
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
void setProgramName(char* programName);

/**
 * Sets whether the top-level exception handler is enabled.
 */
void setTopLevelExceptionHandlerEnabled(bool enabled);

/**
 * If running under debugger, will interrupt program and return control to debugger (as if you
 * pressed "interrupt" button). If not running under debugger, does nothing.
 */
void interruptIfDebug();

}

#endif // _exceptions_h
