/*
 * File: regexpr.h
 * ---------------
 * This file exports functions for performing regular expression operations
 * on C++ strings.  It will be unnecessary once the C++11 regex library
 * is widely available, but as of this writing the regex library is not
 * supported on gcc and other major C++ compilers.
 * 
 * The regular expression functions are implemented by sending the strings and
 * regexes to the Java Back-End to run the operations in Java.  This is a bit
 * kludgy but we don't want to write our own regex parser from scratch.
 * Using Java's is a compromise for now.
 *
 * @author Marty Stepp
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * @since 2014/03/01
 */

#ifndef _regexpr_h
#define _regexpr_h

#include <string>

/*
 * Returns true if the given string s matches the given regular expression
 * as a substring.
 */
bool regexMatch(std::string s, std::string regexp);

/*
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
int regexMatchCount(std::string s, std::string regexp);

/*
 * Replaces all occurrences of the given regular expression in s with the given
 * replacement text, and returns the resulting string.
 * If 'limit' >= 0 is passed, replaces that many occurrences of the regex rather
 * than replacing all occurrences.
 */
std::string regexReplace(std::string s, std::string regexp,
                         std::string replacement, int limit = -1);

#endif
