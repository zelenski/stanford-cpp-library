/*
 * File: diff.cpp
 * --------------
 * This file contains implementations of functions that perform a text 'diff'
 * operation to compare two strings and output the differences.
 * See diff.h for documentation of each function.
 * 
 * @author Marty Stepp
 * @version 2016/10/30
 * - fixed diff flags; added punctuation flag
 * @version 2016/10/22
 * - added diffPass (for autograder assertDiff)
 * @version 2014/10/14
 * - initial version
 * @since 2014/10/14
 */

#include "diff.h"
#include <algorithm>
#include "map.h"
#include "regexpr.h"
#include "set.h"
#include "stringutils.h"
#include "strlib.h"
#include "vector.h"

namespace diff {
std::string diff(std::string s1, std::string s2, int flags) {
    Vector<std::string> lines1 = stringutils::explodeLines(s1);
    Vector<std::string> lines2 = stringutils::explodeLines(s2);
    Vector<std::string> lines1Original = lines1;
    Vector<std::string> lines2Original = lines2;

    if (flags & IGNORE_NUMBERS) {
        s1 = regexReplace(s1, "[0-9]+", "###");
        s2 = regexReplace(s2, "[0-9]+", "###");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_NONNUMBERS) {
        s1 = regexReplace(s1, "[^0-9\n]+", " ");
        s2 = regexReplace(s2, "[^0-9\n]+", " ");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_PUNCTUATION) {
        std::string punct = "[.,?!'\"()\\/#$%@^&*_\\[\\]{}|<>:;-]+";
        s1 = regexReplace(s1, punct, "");
        s2 = regexReplace(s2, punct, "");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_AFTERDECIMAL) {
        s1 = regexReplace(s1, "\\.[0-9]+", ".#");
        s2 = regexReplace(s2, "\\.[0-9]+", ".#");
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_CASE) {
        s1 = stringutils::toLowerCase(s1);
        s2 = stringutils::toLowerCase(s2);
        lines1 = stringutils::explodeLines(s1);
        lines2 = stringutils::explodeLines(s2);
    }
    if (flags & IGNORE_CHARORDER) {
        Vector<std::string> lines1Sorted;
        for (std::string line : lines1) {
            sort(line.begin(), line.end());
            lines1Sorted.add(line);
        }
        lines1 = lines1Sorted;
        s1 = stringutils::implode(lines1);

        Vector<std::string> lines2Sorted;
        for (std::string line : lines2) {
            sort(line.begin(), line.end());
            lines2Sorted.add(line);
        }
        lines2 = lines2Sorted;
        s2 = stringutils::implode(lines2);
    }
    if (flags & IGNORE_LINEORDER) {
        sort(lines1.begin(), lines1.end());
        sort(lines2.begin(), lines2.end());
        s1 = stringutils::implode(lines1);
        s2 = stringutils::implode(lines2);
    }
    if (flags & IGNORE_WHITESPACE) {
        for (int i = 0; i < lines1.size(); i++) {
            lines1[i] = stringutils::stripWhitespace(lines1[i]);
        }
        for (int i = 0; i < lines2.size(); i++) {
            lines2[i] = stringutils::stripWhitespace(lines2[i]);
        }
    }

    if (stringutils::trimR(s1) == stringutils::trimR(s2)) {
        return NO_DIFFS_MESSAGE;
    }

    // build a reverse-index array using the line as key and line number as value
    // don't store blank lines, so they won't be targets of the shortest distance
    // search
    Map<std::string, Set<int>> reverse1;
    Map<std::string, Set<int>> reverse2;
    for (int i = 0; i < lines1.size(); i++) {
        std::string line = lines1[i];
        if ((int) line.length() > 0) {
            reverse1[line].add(i);
        }
    }
    for (int i = 0; i < lines2.size(); i++) {
        std::string line = lines2[i];
        if ((int) line.length() > 0) {
            reverse2[line].add(i);
        }
    }

    // indexes of current lines within each file
    // (start at beginning of each list)
    int index1 = 0;
    int index2 = 0;
    Vector<int> actions;

    // walk this loop until we reach the end of one of the lists of lines
    while (index1 < lines1.size() && index2 < lines2.size()) {
        // if we have a common line, save it and go to the next
        if (lines1[index1] == lines2[index2]) {
            actions += 4;
            index1++;
            index2++;
            continue;
        }
        // otherwise, find the shortest move (Manhattan-distance) from the
        // current location
        int best1 = lines1.size();
        int best2 = lines2.size();
        int s1 = index1;
        int s2 = index2;
        while ((s1 + s2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
            int d = -1;
            if (lines2.size() > s2 && reverse1.containsKey(lines2[s2])) {
                for (int lineNumber : reverse1[lines2[s2]]) {
                    if (lineNumber >= s1) {
                        d = lineNumber;
                        break;
                    }
                }
            }
            if (d >= s1 && (d + s2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
                best1 = d;
                best2 = s2;
            }

            d = -1;
            if (lines1.size() > s1 && reverse2.containsKey(lines1[s1])) {
                for (int lineNumber : reverse2[lines1[s1]]) {
                    if (lineNumber >= s2) {
                        d = lineNumber;
                        break;
                    }
                }
            }
            if (d >= s2 && (d + s1 - index1 - index2) < (best1 + best2 - index1 - index2)) {
                best1 = s1;
                best2 = d;
            }

            s1++;
            s2++;
        }

        while (index1 < best1) {
            actions += 1;
            index1++;
        }  // deleted elements

        while (index2 < best2) {
            actions += 2;
            index2++;
        }  // added elements
    }

    // we've reached the end of one list, now walk to the end of the other
    while (index1 < lines1.size()) {
        actions += 1;
        index1++;
    }  // deleted elements

    if (flags & IGNORE_TRAILING) {
        while (index2 < lines2.size()) {
            actions += 2;
            index2++;
        }  // added elements
    }

    // and this marks our ending point
    actions += 8;

    int op = 0;
    int x0 = 0;
    int x1 = 0;
    int y0 = 0;
    int y1 = 0;
    Vector<std::string> out;

    for (int action : actions) {
        if (action == 1) {
            op |= action;
            x1++;
            continue;
        } else if (action == 2) {
            op |= action;
            y1++;
            continue;
        }

        if (op > 0) {
            bool multipleLines = (x1 != x0 + 1);
            std::string xstr = std::string("") + (multipleLines ? (std::to_string(x0 + 1) + "-" + std::to_string(x1)) : std::to_string(x1));
            std::string ystr = std::string("") + ((y1 != y0 + 1) ? (std::to_string(y0 + 1) + "-" + std::to_string(y1)) : std::to_string(y1));
            std::string linesStr = std::string("\nLine") + (multipleLines ? "s " : " ");
            std::string doStr = std::string("do") + (multipleLines ? "" : "es");
            if (op == 1) {
                out += linesStr + xstr + " deleted near student line " + std::to_string(y1);
            } else if (op == 3) {
                if (xstr == ystr) {
                    out += linesStr + xstr + " " + doStr + " not match";
                } else {
                    out += linesStr + xstr + " changed to student line " + ystr;
                }
            }

            while (x0 < x1) {
                out += "EXPECTED < " + lines1Original[x0];
                x0++;
            }   // deleted elems

            if (op == 2) {
                if (!(flags & IGNORE_LEADING) || x1 > 0) {
                    out += linesStr + std::to_string(x1) + " added at student line " + ystr;
                }
            } else if (op == 3) {
                // out += "---";
            }

            while (y0 < y1) {
                if (!(flags & IGNORE_LEADING) || op != 2 || x1 > 0) {
                    out += "STUDENT  > " + lines2Original[y0];
                }
                y0++;
            }   // added elems
        }
        x1++;
        x0 = x1;
        y1++;
        y0 = y1;
        op = 0;
    }

    if (out.size() > 0) {
        out += "";
        return trim(stringutils::implode(out, "\n"));
    } else {
        return NO_DIFFS_MESSAGE;
    }
}

bool diffPass(const std::string& s1, const std::string& s2, int flags) {
    std::string diffs = diff(s1, s2, flags);
    bool result = trim(diffs) == NO_DIFFS_MESSAGE;
    return result;
}

bool isDiffMatch(const std::string& diffs) {
    return trim(diffs) == NO_DIFFS_MESSAGE;
}
} // namespace diff
