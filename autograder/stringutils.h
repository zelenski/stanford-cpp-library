/*
 * File: stringutils.h
 * -------------------
 * This file contains declarations of utility functions related to strings.
 * Arguably some of this functionality could be moved into strlib.{h,cpp},
 * but it is mostly written to support autograders so it is placed here.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/03/01
 */

#ifndef _stringutils_h
#define _stringutils_h

#include <string>
#include "vector.h"

namespace stringutils {
int charsDifferent(std::string s1, std::string s2);
std::string collapseSpaces(std::string s);
Vector<std::string> explodeLines(std::string s);
int height(std::string s);
std::string implode(const Vector<std::string>& v, std::string delimiter = "\n");
std::string indent(std::string s, int spaces);

/*
 * Removes blank lines and collapses multiple spaces into one.
 * Used to facilitate approximate output matching.
 */
std::string makeSloppy(std::string s);

/*
 * Finds all matches of the given regular expression in the given string s
 * and fills 'linesOut' with a comma-separated string representing the line
 * numbers within the string at which the matches occur, such as "2,14,27,36".
 * This is mainly useful for grading programs.
 * Returns the number of times the given regular expression is found inside
 * the given string s.  Returns 0 if there are no matches for the regexp.
 */
int regexMatchCountWithLines(std::string s, std::string regexp,
                             std::string& linesOut);

std::string removeBlankLines(std::string s);
std::string toLowerCase(std::string s);
std::string trimR(std::string s);
std::string trimToHeight(std::string s, int height, std::string suffix = "...");
std::string trimToWidth(std::string s, int width, std::string suffix = " ...");
std::string stripWhitespace(std::string s);
std::string truncate(std::string s, int length);
std::string toPrintable(int ch);
int width(std::string s);
} // namespace stringutils

#endif
