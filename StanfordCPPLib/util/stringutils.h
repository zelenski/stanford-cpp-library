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

/**
 * @private
 */
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
std::string removeBlankLines(const std::string& s);
std::string toLowerCase(const std::string& s);
std::string trimR(const std::string& s);
std::string trimToHeight(const std::string& s, int height, const std::string& suffix = "...");
std::string trimToSize(const std::string& s, int width, int height, const std::string& suffix = " ...");
std::string trimToWidth(const std::string& s, int width, const std::string& suffix = " ...");
std::string stripWhitespace(const std::string& s);
std::string truncate(const std::string& s, int length, const std::string& suffix = " ...");
int width(const std::string& s);

/**
 * Returns a printable string for the given character.
 * @example toPrintable('c') returns "c"
 * @example toPrintable('\n') returns "\\n"
 */
std::string toPrintable(int ch);

/**
 * Returns a string with each non-printable character in the given string
 * replaced by one that is printable.
 * Certain common escape characters are replaced by a backslash representation,
 * and non-printable ASCII characters are replaced by a backslash and their
 * ASCII numeric representation, such as \255.
 * @example toPrintable("hi \0 there\n') returns "hi \\0 there\\n"
 */
std::string toPrintable(const std::string& s);

} // namespace stringutils

#endif // _stringutils_h
