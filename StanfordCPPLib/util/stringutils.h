/*
 * File: stringutils.h
 * -------------------
 * This file contains declarations of utility functions related to strings.
 * Arguably some of this functionality could be moved into strlib.{h,cpp},
 * but it is mostly written to support autograders so it is placed here.
 * 
 * @author Marty Stepp
 * @version 2017/10/20
 * - changed string to const string& in all functions
 * @version 2016/11/09
 * - added trimToSize function
 * @since 2014/03/01
 */

#ifndef _stringutils_h
#define _stringutils_h

#include <string>
#include "vector.h"

namespace stringutils {
int charsDifferent(const std::string& s1, const std::string& s2);
std::string collapseSpaces(const std::string& s);
Vector<std::string> explodeLines(const std::string& s);
int height(const std::string& s);
std::string implode(const Vector<std::string>& v, const std::string& delimiter = "\n");
std::string indent(const std::string& s, int spaces);

/*
 * Removes blank lines and collapses multiple spaces into one.
 * Used to facilitate approximate output matching.
 */
std::string makeSloppy(const std::string& s);

/*
 * Finds all matches of the given regular expression in the given string s
 * and fills 'linesOut' with a comma-separated string representing the line
 * numbers within the string at which the matches occur, such as "2,14,27,36".
 * This is mainly useful for grading programs.
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
int regexMatchCountWithLines(const std::string& s, const std::string& regexp,
                             std::string& linesOut);

std::string removeBlankLines(const std::string& s);
std::string toLowerCase(const std::string& s);
std::string trimR(const std::string& s);
std::string trimToHeight(const std::string& s, int height, const std::string& suffix = "...");
std::string trimToSize(const std::string& s, int width, int height, const std::string& suffix = " ...");
std::string trimToWidth(const std::string& s, int width, const std::string& suffix = " ...");
std::string stripWhitespace(const std::string& s);
std::string truncate(const std::string& s, int length);
std::string toPrintable(int ch);
int width(const std::string& s);
} // namespace stringutils

#endif
