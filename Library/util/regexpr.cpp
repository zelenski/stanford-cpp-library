/*
 * File: regexpr.cpp
 * -----------------
 * Implementation of the functions in regexpr.h.
 * See regexpr.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2018/12/16
 * - added CodeStepByStep disabling of regexes
 * @version 2018/11/22
 * - added headless (non-Qt) mode support
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/03/01
 */

#include "regexpr.h"
#ifndef SPL_HEADLESS_MODE
#include <QtGlobal>
#endif // SPL_HEADLESS_MODE
#include "error.h"
#include "stringutils.h"

#if defined(SPL_CODESTEPBYSTEP) || QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
bool regexMatch(const std::string& /*s*/, const std::string& /*regexp*/) {
    return false;   // not supported
}

int regexMatchCount(const std::string& /*s*/, const std::string& /*regexp*/) {
    return 0;   // not supported
}

void regexMatchCountWithLines(const std::string& /*s*/, const std::string& /*regexp*/,
                             Vector<int>& /*linesOut*/) {
    // empty; not supported
}

std::string regexReplace(const std::string& s, const std::string& /*regexp*/, const std::string& /*replacement*/, int /*limit*/) {
    return s;   // not supported
}

#else // QT_VERSION

// C++ regex support
#include <iterator>
#include <regex>

bool regexMatch(const std::string& s, const std::string& regexp) {
    std::regex reg(regexp);
    std::smatch match;
    return std::regex_search(s, match, reg);
}

int regexMatchCount(const std::string& s, const std::string& regexp) {
    std::regex reg(regexp);
    auto it1 = std::sregex_iterator(s.begin(), s.end(), reg);
    auto it2 = std::sregex_iterator();
    return std::distance(it1, it2);
}

void regexMatchCountWithLines(const std::string& s, const std::string& regexp,
                             Vector<int>& linesOut) {
    linesOut.clear();

    // keep a running index and line#, and each time we find a regex match,
    // "walk" forward to its index.  when we see a \n, increment line number
    int currentIndex = 0;
    int currentLine = 1;

    // get all regex matches by character position/index
    std::regex reg(regexp);
    for (std::sregex_iterator itr = std::sregex_iterator(s.begin(), s.end(), reg),
            end = std::sregex_iterator();
            itr != end;
            ++itr) {
        std::smatch match = *itr;
        int matchIndex = (int) match.position();
        while (currentIndex < (int) s.length() && currentIndex < matchIndex) {
            if (s[currentIndex] == '\n') {
                currentLine++;
            }
            currentIndex++;
        }
        linesOut.add(currentLine);
    }
}

std::string regexReplace(const std::string& s, const std::string& regexp, const std::string& replacement, int limit) {
    std::regex reg(regexp);
    std::string result;
    if (limit == 1) {
        // replace single occurrence
        result = std::regex_replace(s, reg, replacement,
                                    std::regex_constants::format_first_only);
    } else if (limit <= 0) {
        // replace all
        result = std::regex_replace(s, reg, replacement);
    } else {
        error("regexReplace: given limit not supported.");
    }
    return result;
}
#endif // QT_VERSION

// this function can be implemented the same way whether regexes are available or not
int regexMatchCountWithLines(const std::string& s, const std::string& regexp, std::string& linesOut) {
    Vector<int> linesOutVec;
    regexMatchCountWithLines(s, regexp, linesOutVec);

    // concatenate the vector into a string like "1, 4, 7, 7, 19"
    linesOut = "";
    if (!linesOutVec.isEmpty()) {
        linesOut += std::to_string(linesOutVec[0]);
        for (int i = 1; i < linesOutVec.size(); i++) {
            linesOut += ", " + std::to_string(linesOutVec[i]);
        }
    }
    return linesOutVec.size();
}
