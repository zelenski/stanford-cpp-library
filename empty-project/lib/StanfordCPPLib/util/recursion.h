/*
 * File: recursion.h
 * -----------------
 * This file includes a few utility functions related to recursion.
 * You can use them to print a debug message that is indented relative
 * to the level of recursion you are currently nested in.
 *
 * @author Marty Stepp
 * @version 2016/10/30
 * - initial version (extracted from exceptions.h)
 */

#ifndef _recursion_h
#define _recursion_h

#include <string>

/*
 * Returns number of calls deep we are in the current recursive function.
 * For example, if f() calls f() calls f(), this function returns 3.
 *
 * NOTE: Doesn't usually work when used with 'static' functions, because their names
 * are not exported or revealed to the internal stack trace grabber.
 * So if you want to use this function, make your function non-static.
 */
int getRecursionIndentLevel();

/*
 * Returns a string of indentation that can be used to pretty-print recursive calls
 * at their corresponding level of nesting.
 * Indents by 4 spaces per level but can be overridden by passing 'indenter' param.
 *
 * NOTE: Doesn't usually work when used with static functions, because their names
 * are not exported or revealed to the internal stack trace grabber.
 * So if you want to use this function, make your function non-static.
 */
std::string recursionIndent(const std::string& indenter = "    ");

#endif // _recursion_h
