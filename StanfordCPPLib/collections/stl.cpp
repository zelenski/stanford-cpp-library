/*
 * File: stl.cpp
 * -------------
 * Implements bodies of functions declared in stl.h.
 *
 * @author Marty Stepp
 * @version 2018/10/19
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "stl.h"
#undef INTERNAL_INCLUDE

std::set<std::string> toStlSet(const DawgLexicon& lex) {
    std::set<std::string> result;
    for (const std::string& s : lex) {
        result.insert(s);
    }
    return result;
}

/**
 * Returns an STL set object with the same elements as this Lexicon.
 */
std::set<std::string> toStlSet(const Lexicon& lex) {
    std::set<std::string> result;
    for (const std::string& s : lex) {
        result.insert(s);
    }
    return result;
}
