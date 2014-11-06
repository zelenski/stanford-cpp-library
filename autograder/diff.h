/*
 * File: diff.h
 * ------------
 * This file contains declarations of functions that perform a text 'diff'
 * operation to compare two strings and output the differences.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/10/14
 */

#ifndef _diff_h
#define _diff_h

#include <string>

namespace diff {
const std::string NO_DIFFS_MESSAGE = "No differences found";

enum DiffFlags {
    IGNORE_LEADING      = 0x1,
    IGNORE_TRAILING     = 0x2,
    IGNORE_WHITESPACE   = 0x4,
    IGNORE_BLANKLINES   = 0x8,
    IGNORE_CASE         = 0x10,
    IGNORE_NUMBERS      = 0x20,
    IGNORE_NONNUMBERS   = 0x40,
    IGNORE_AFTERDECIMAL = 0x80,
    IGNORE_CHARORDER    = 0x100,
    IGNORE_LINEORDER    = 0x200,
    IGNORE_EVERYTHING   = 0x1000
    // IGNORE_ORDER        = 0x100,   // not implemented
    // IGNORE_PRINTF       = 0x800    // not implemented
};

std::string diff(std::string s1, std::string s2);
std::string diff(std::string s1, std::string s2, int flags);
bool isDiffMatch(std::string diffs);
} // namespace diff

#endif // _diff_h
