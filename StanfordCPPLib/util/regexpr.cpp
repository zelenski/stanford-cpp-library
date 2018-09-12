/*
 * File: regexpr.cpp
 * -----------------
 * Implementation of the functions in regexpr.h.
 * See regexpr.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/03/01
 */

#include "regexpr.h"
#include <iterator>
#include <regex>
#include "error.h"
#include "stringutils.h"

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

int regexMatchCountWithLines(const std::string& s, const std::string& regexp, std::string& linesOut) {
    Vector<int> linesOutVec;
    regexMatchCountWithLines(s, regexp, linesOutVec);

    // concatenate the vector into a string like "1, 4, 7, 7, 19"
    linesOut = "";
    if (!linesOutVec.isEmpty()) {
        linesOut += integerToString(linesOutVec[0]);
        for (int i = 1; i < linesOutVec.size(); i++) {
            linesOut += ", " + integerToString(linesOutVec[i]);
        }
    }
    return linesOutVec.size();
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
