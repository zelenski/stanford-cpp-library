/*
 * File: plainconsole.h
 * --------------------
 * This file declares functions to add utility to the
 * C++ plain text console streams, cin/cout/cerr.
 * See plainconsole.cpp for implementation of each function.
 *
 * @author Marty Stepp
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2015/10/21
 * @since 2015/10/21
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _plainconsole_h
#define _plainconsole_h

namespace plainconsole {
/**
 * Sets the cout console to throw an error if more than the given number of
 * total characters are ever printed to it.
 * This can be used to stop student programs that try to print infinite output.
 */
void setOutputLimit(int limit);

/**
 * Sets whether user input values read from cin are echoed back to cout.
 * This is useful when capturing output dialogues for comparison/diffing.
 */
void setEcho(bool value);
} // namespace plainconsole

#endif // _plainconsole_h
