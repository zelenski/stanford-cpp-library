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
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2018/09/20
 * - added Qt version checking around some regex functions for compatibility
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * @since 2014/03/01
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _regexpr_h
#define _regexpr_h

#include <string>

#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

/**
 * Returns true if the given string s matches the given regular expression
 * as a substring.
 * For an exact match instead, put "^" and "$" in your regex as start/end anchors.
 */
bool regexMatch(const std::string& s, const std::string& regexp);

/**
 * Returns the number of times the given regular expression is found inside
 * the given string s.
 * Returns 0 if there are no matches for the regexp.
 */
int regexMatchCount(const std::string& s, const std::string& regexp);

/**
 * Finds all matches of the given regular expression in the given string s
 * and fills 'linesOut' with a comma-separated string representing the line
 * numbers within the string at which the matches occur, such as "2,14,27,36".
 * This is mainly useful for grading programs.
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
int regexMatchCountWithLines(const std::string& s, const std::string& regexp,
                             std::string& linesOut);

/**
 * Finds all matches of the given regular expression in the given string s
 * and fills 'linesOut' with the line numbers within the string at which the
 * matches occur, such as [2, 14, 27, 36].
 * This is mainly useful for grading programs.
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
void regexMatchCountWithLines(const std::string& s, const std::string& regexp,
                              Vector<int>& linesOut);

/**
 * Replaces occurrences of the given regular expression in s with the given
 * replacement text, and returns the resulting string.
 * If limit >  0 is passed, replaces that many occurrences of the regex.
 * If limit <= 0 is passed (or omitted), all occurrences are replaced.
 */
std::string regexReplace(const std::string& s, const std::string& regexp,
                         const std::string& replacement, int limit = -1);

#endif // _regexpr_h
