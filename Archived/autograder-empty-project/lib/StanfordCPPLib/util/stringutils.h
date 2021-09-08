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

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _stringutils_h
#define _stringutils_h

#include <string>

#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

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
std::string toPrintable(int ch);
int width(const std::string& s);
} // namespace stringutils

#endif // _stringutils_h
