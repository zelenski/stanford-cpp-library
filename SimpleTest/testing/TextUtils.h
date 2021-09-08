
#pragma once

#include <string>

/**
 * Given a number, returns a version of that number with commas separating the digits.
 *
 * @param number The number in question
 * @return The number with commas added.
 */
std::string addCommasTo(int number);

/* Given a quantity, returns a string of that quantity plus an appropriately-pluralized
 * version of what it is.
 */
template <typename ValueType>
std::string pluralize(const ValueType& value, const std::string& singular, const std::string& plural) {
    return addCommasTo(value) + " " + (value == 1? singular : plural);
}

/* Assume we suffix with s to pluralize unless specified otherwise. */
template <typename ValueType>
std::string pluralize(const ValueType& value, const std::string& singular) {
    return pluralize(value, singular, singular + "s");
}

/**
 * Given a string or character, quotes that string in a manner similar to C++14's std::quoted.
 *
 * @param input The text to escape.
 * @return A quoted version of that string.
 */
std::string quotedVersionOf(const std::string& input);
std::string quotedVersionOf(char input);
