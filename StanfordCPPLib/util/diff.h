/*
 * File: diff.h
 * ------------
 * This file contains declarations of functions that perform a text 'diff'
 * operation to compare two strings and output the differences.
 * 
 * @author Marty Stepp
 * @version 2016/10/30
 * - fixed diff flags; added punctuation flag
 * @version 2016/10/28
 * - added DEFAULT_STRICT_FLAGS
 * @version 2016/10/22
 * - added diffPass (for autograder assertDiff)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */


#ifndef _diff_h
#define _diff_h

#include <string>

namespace diff {
const std::string NO_DIFFS_MESSAGE = "No differences found";

enum DiffFlags {
    IGNORE_NONE         = 0x0,
    IGNORE_LEADING      = 0x1,
    IGNORE_TRAILING     = 0x2,
    IGNORE_WHITESPACE   = 0x4,
    IGNORE_BLANKLINES   = 0x8,
    IGNORE_CASE         = 0x10,
    IGNORE_NUMBERS      = 0x20,
    IGNORE_NONNUMBERS   = 0x40,
    IGNORE_PUNCTUATION  = 0x80,
    IGNORE_AFTERDECIMAL = 0x100,
    IGNORE_CHARORDER    = 0x200,
    IGNORE_LINEORDER    = 0x400,
    IGNORE_EVERYTHING   = 0x100000
};

const int DIFF_STRICT_FLAGS = IGNORE_TRAILING;
const int DIFF_DEFAULT_FLAGS = IGNORE_CASE | IGNORE_TRAILING | IGNORE_WHITESPACE | IGNORE_PUNCTUATION;

std::string diff(std::string s1, std::string s2, int flags = DIFF_DEFAULT_FLAGS);
bool diffPass(const std::string& s1, const std::string& s2, int flags = DIFF_DEFAULT_FLAGS);
bool isDiffMatch(const std::string& diffs);
} // namespace diff

#endif // _diff_h
