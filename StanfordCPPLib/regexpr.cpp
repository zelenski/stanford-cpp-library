/*
 * File: regexpr.cpp
 * -----------------
 * Implementation of the functions in regexpr.h.
 * See regexpr.h for documentation of each function.
 *
 * @author Marty Stepp
 * @version 2014/10/14
 * - removed regexMatchCountWithLines for simplicity
 * 2014/10/08
 * - removed 'using namespace' statement
 * @since 2014/03/01
 */

#include "regexpr.h"
#include "platform.h"

static Platform *pp = getPlatform();

bool regexMatch(std::string s, std::string regexp) {
    return pp->regex_match(s, regexp);
}

int regexMatchCount(std::string s, std::string regexp) {
    return pp->regex_matchCount(s, regexp);
}

std::string regexReplace(std::string s, std::string regexp, std::string replacement, int limit) {
    return pp->regex_replace(s, regexp, replacement, limit);
}
