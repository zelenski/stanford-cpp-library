/*
 * File: exceptions.h
 * ------------------
 * This file contains a top-level exception handler to print exceptions thrown
 * by student code on the console.
 * It also contains some wizardry to try to retrieve a stack trace when the
 * exception is thrown, though it is hard to consistently do this on all platforms.
 * 
 * @author Marty Stepp
 * @version 2014/11/05
 * @since 2014/11/05
 */

#ifndef _exceptions_h
#define _exceptions_h

namespace exceptions {
/*
 * Called by C++ lib's main wrapper so that the stack trace knows the program's name.
 * (Taken from argv[0].)
 */
std::string getProgramNameForStackTrace();

/*
 * Returns whether the top-level exception handler is enabled.
 * Initially false.
 */
bool getTopLevelExceptionHandlerEnabled();

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

#endif // _exceptions_h
