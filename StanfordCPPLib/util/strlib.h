/*
 * File: strlib.h
 * --------------
 * This file exports several useful string functions that are not
 * included in the C++ string library.
 * 
 * @version 2018/11/14
 * - added std::to_string for bool, char, pointer, and generic template type T
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 * @version 2018/09/02
 * - added padLeft, padRight
 * @version 2016/11/09
 * - added boolalpha to writeGenericValue (improves bool printing in
 *   collection toString output)
 * @version 2016/10/30
 * - added overloads that take type char instead of string:
 *   stringContains, stringIndexOf, stringJoin, stringLastIndexOf, stringReplace,
 *   stringSplit, toLowerCase, toUpperCase
 * @version 2016/10/26
 * - bug fix for stringLastIndexOf default index arg
 * @version 2016/10/13
 * - modified writeGenericValue, writeQuotedString to return ostream
 * @version 2016/08/03
 * - modified readGenericValue not to throw error() on parse failures
 *   (needed to support idiomatic silent-failing >> operators)
 * @version 2015/10/26
 * - added charToInteger/integerToChar functions
 * @version 2015/08/02
 * - added htmlEncode/Decode functions (not 100% perfect but works for common cases)
 * @version 2014/10/19
 * - alphabetized functions
 * - added several 'inPlace' variants of existing functions that return strings
 * @version 2014/10/08
 * - removed dependency on 'using namespace' statement
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _strlib_h
#define _strlib_h

#include <iostream>
#include <sstream>
#include <string>

#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

/**
 * Returns the string "true" if b is true, or "false" if b is false.
 */
std::string boolToString(bool b);

/**
 * Returns the string "true" if b is nonzero, or "false" if b is 0.
 */
std::string boolToString(int b);

/**
 * Returns an integer equivalent of the given numeric character.
 * For example, charToInteger('3') returns the int 3.
 * If the character is not '0' through '9', throws an error.
 */
int charToInteger(char c);

/**
 * Returns a single-character string containing the given character.
 * For example, charToString('Q') returns the string "Q".
 */
std::string charToString(char c);

/**
 * Converts a floating-point number into the corresponding string form.
 * For example, calling <code>doubleToString(23.45)</code> returns
 * the string <code>"23.45"</code>.
 * Equivalent to realToString.
 */
std::string doubleToString(double d);

/**
 * Returns <code>true</code> if the string <code>str</code> ends with
 * the specified suffix.
 */
bool endsWith(const std::string& str, const std::string& suffix);

/**
 * Returns <code>true</code> if the string <code>str</code> ends with
 * the specified character.
 */
bool endsWith(const std::string& str, char suffix);

/**
 * Returns <code>true</code> if <code>s1</code> and <code>s2</code> are
 * equal discounting differences in case.
 */
bool equalsIgnoreCase(const std::string& s1, const std::string& s2);

/**
 * Converts the given string from an HTML-encoded version to its decoded
 * equivalent; the opposite of htmlEncode. Any escaped HTML character entities
 * will be replaced by their unescaped equivalents.
 * For example, <code>htmlEncode("&lt;p class=&quot;abc&quot;&gt;I love you &amp; me&lt;/p&gt;")
 * returns "<p class=\"abc\">I love you & me</p>".
 */
std::string htmlDecode(const std::string& s);

/**
 * Converts the given string into an HTML-encoded equivalent version, with
 * any relevant HTML character entities replaced by escaped equivalents.
 * For example, <code>htmlEncode("<p class=\"abc\">I love you & me</p>") returns
 * "&lt;p class=&quot;abc&quot;&gt;I love you &amp; me&lt;/p&gt;".
 */
std::string htmlEncode(const std::string& s);

/**
 * Converts an integer into the corresponding numeric character.
 * For example, calling <code>integerToChar(3)</code> returns
 * the char <code>'3'</code>.
 * @throw ErrorException if the given integer is not a single-digit number from 0-9 inclusive
 */
char integerToChar(int n);

/**
 * Converts an integer into the corresponding string of digits.
 * For example, calling <code>integerToString(123)</code> returns
 * the string <code>"123"</code>.
 */
std::string integerToString(int n, int radix = 10);

/**
 * Converts an integer into the corresponding string of digits.
 * For example, calling <code>longToString(123)</code> returns
 * the string <code>"123"</code>.
 */
std::string longToString(long n, int radix = 10);

/**
 * Inserts spaces at the start of the given string until it is at least
 * the given length.
 * You can optionally pass a fill character (other than space) as a third parameter.
 *
 * Note that if you are printing strings and want your output to line up
 * at a given number of characters, you may be better served using the built-in
 * C++ ostream manipulators such as setw(int), left, and right, as defined in
 * the iomanip library.
 */
std::string padLeft(const std::string& s, int length, char fill = ' ');

/**
 * Inserts spaces at the end of the given string until it is at least
 * the given length.
 * You can optionally pass a fill character (other than space) as a third parameter.
 *
 * Note that if you are printing strings and want your output to line up
 * at a given number of characters, you may be better served using the built-in
 * C++ ostream manipulators such as setw(int), left, and right, as defined in
 * the iomanip library.
 */
std::string padRight(const std::string& s, int length);

/**
 * Returns a hexadecimal string for the given pointer, such as "0x3f0427b".
 * Returns "nullptr" if p is a null pointer.
 */
std::string pointerToString(void* p);

/**
 * Converts a floating-point number into the corresponding string form.
 * For example, calling <code>realToString(23.45)</code> returns
 * the string <code>"23.45"</code>.
 */
std::string realToString(double d);

/**
 * Returns <code>true</code> if the string <code>str</code> starts with
 * the specified prefix.
 */
bool startsWith(const std::string& str, char prefix);

/**
 * Returns <code>true</code> if the string <code>str</code> starts with
 * the specified character.
 */
bool startsWith(const std::string& str, const std::string& prefix);

/**
 * Returns true if the given character occurs somewhere in s.
 */
bool stringContains(const std::string& s, char ch);

/**
 * Returns true if the given substring occurs somewhere in s.
 */
bool stringContains(const std::string& s, const std::string& substring);

/**
 * Returns the index of the start of the first occurrence of the given character
 * in s, if it occurs in s.  If it does not occur, returns -1.
 * This function is very similar to string.find, but find returns string::npos
 * when the string is not found.
 */
int stringIndexOf(const std::string& s, char ch, int startIndex = 0);

/**
 * Returns the index of the start of the first occurrence of the given substring
 * in s, if it occurs in s.  If it does not occur, returns -1.
 * This function is very similar to string.find, but find returns string::npos
 * when the string is not found.
 */
int stringIndexOf(const std::string& s, const std::string& substring, int startIndex = 0);

/**
 * Returns true if the given string is either "true" or "false".
 */
bool stringIsBool(const std::string& str);

/**
 * Returns true if the given string could be converted to an real number
 * successfully by the stringToReal function, which will be true if
 * the string has the format of a real number such as "3.14" or "-46".
 * Equivalent to stringIsReal.
 */
bool stringIsDouble(const std::string& str);   // alias

/**
 * Returns true if the given string could be converted to an integer
 * successfully by the stringToInteger function, which will be true if
 * the string has the format of an integer such as "1234" or "-8".
 * Optionally accepts a radix (base) parameter if base-10 is not desired.
 */
bool stringIsInteger(const std::string& str, int radix = 10);

/**
 * Returns true if the given string could be converted to a long
 * successfully by the stringToLong function, which will be true if
 * the string has the format of an integer such as "1234" or "-8".
 * Optionally accepts a radix (base) parameter if base-10 is not desired.
 */
bool stringIsLong(const std::string& str, int radix = 10);

/**
 * Returns true if the given string could be converted to an real number
 * successfully by the stringToReal function, which will be true if
 * the string has the format of a real number such as "3.14" or "-46".
 */
bool stringIsReal(const std::string& str);

/**
 * Combines the elements of the given vector into a single string,
 * with the given delimiter separating neighboring elements, and returns it.
 * For example, joining the elements of the vector
 * {"Hi", "there", "", "Jim"} with the delimiter '?' returns "Hi?there??Jim".
 */
std::string stringJoin(const Vector<std::string>& v, char delimiter = '\n');

/**
 * Combines the elements of the given STL vector into a single string,
 * with the given delimiter separating neighboring elements, and returns it.
 * For example, joining the elements of the vector
 * {"Hi", "there", "", "Jim"} with the delimiter "??" returns "Hi??there????Jim".
 */
std::string stringJoin(const Vector<std::string>& v, const std::string& delimiter = "\n");

/**
 * Returns the index of the start of the last occurrence of the given character
 * in s, if it occurs in s.  If it does not occur, returns -1.
 * This function is very similar to string.rfind, but rfind returns string::npos
 * when the string is not found.
 */
int stringLastIndexOf(const std::string& s, char ch, int startIndex = (int) std::string::npos);

/**
 * Returns the index of the start of the last occurrence of the given substring
 * in s, if it occurs in s.  If it does not occur, returns -1.
 * This function is very similar to string.rfind, but rfind returns string::npos
 * when the string is not found.
 */
int stringLastIndexOf(const std::string& s, const std::string& substring, int startIndex = (int) std::string::npos);

/**
 * Returns a new string formed by replacing any occurrences of the given 'old'
 * character with the given replacement character in 'str'.
 * Note that this is NOT a regular expression replacement; it looks for the
 * 'old' string literally.  If you want regular expressions, see regexpr.h.
 * The 'inPlace' variant modifies an existing string rather than returning a new one,
 * and returns the number of occurrences of 'old' were replaced.
 */
std::string stringReplace(const std::string& str, char old, char replacement, int limit = -1);

/**
 * Returns a new string formed by replacing any occurrences of the given 'old'
 * text with the given replacement text in 'str'.
 * Note that this is NOT a regular expression replacement; it looks for the
 * 'old' string literally.  If you want regular expressions, see regexpr.h.
 * The 'inPlace' variant modifies an existing string rather than returning a new one,
 * and returns the number of occurrences of 'old' were replaced.
 */
std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit = -1);

/**
 * A variant of stringReplace, except that it accepts the string as a reference
 * and modifies it in-place rather than returning a new string.
 */
int stringReplaceInPlace(std::string& str, char old, char replacement, int limit = -1);

/**
 * A variant of stringReplace, except that it accepts the string as a reference
 * and modifies it in-place rather than returning a new string.
 */
int stringReplaceInPlace(std::string& str, const std::string& old, const std::string& replacement, int limit = -1);

/**
 * Returns a vector whose elements are strings formed by splitting the
 * given string 'str' by the given separator character.
 * For example, splitting "Hi there  Jim!" on " " returns
 * {"Hi", "there", "", "Jim!"}.
 */
Vector<std::string> stringSplit(const std::string& str, char delimiter, int limit = -1);

/**
 * Returns a vector whose elements are strings formed by splitting the
 * given string 'str' by the given separator text.
 * For example, splitting "Hi there  Jim!" on " " returns
 * {"Hi", "there", "", "Jim!"}.
 */
Vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit = -1);

/**
 * If str is "true", returns the bool value true.
 * If str is "false", returns the bool value false.
 * @throw ErrorException if str is any other value than "true" or "false"
 */
bool stringToBool(const std::string& str);

/**
 * Converts a single-character string into its corresponding char value.
 * For example, stringToChar("hello") returns the char 'h'.
 * @throw ErrorException if the given string does not contain exactly 1 character
 */
char stringToChar(const std::string& str);

/**
 * Converts a string representing a real number into its corresponding
 * value.
 * Equivalent to stringToReal.
 * @throw ErrorException if the string is not a legal floating-point number
 *        or contains extraneous characters other than whitespace
 */
double stringToDouble(const std::string& str);   // alias

/**
 * Converts a string of digits into an integer.
 * The function accepts an optional radix (base); for example,
 * stringToInteger("234", 16) assumes that the string is in base-16 and
 * returns 2*16*16 + 3*16 + 4 = 564.
 * @throw ErrorException if the string is not a legal integer or contains
 *        extraneous characters other than whitespace
 */
int stringToInteger(const std::string& str, int radix = 10);

/**
 * Converts a string of digits into a long.
 * The function accepts an optional radix (base); for example,
 * stringToLong("234", 16) assumes that the string is in base-16 and
 * returns 2*16*16 + 3*16 + 4 = 564.
 * @throw ErrorException if the string is not a legal long or contains
 *        extraneous characters other than whitespace
 */
long stringToLong(const std::string& str, int radix = 10);

/**
 * Converts a string representing a real number into its corresponding
 * value.
 * Equivalent to stringToDouble.
 * @throw ErrorException if the string is not a legal floating-point number or
 * contains extraneous characters other than whitespace
 */
double stringToReal(const std::string& str);

/**
 * Returns a new character in which the given uppercase character has been
 * converted into its lowercase equivalent.
 */
char toLowerCase(char ch);

/**
 * Returns a new string in which all uppercase characters have been converted
 * into their lowercase equivalents.
 */
std::string toLowerCase(const std::string& str);

/**
 * Modifies the given string in-place such that all uppercase characters have
 * been converted into their lowercase equivalents.
 */
void toLowerCaseInPlace(std::string& str);

/**
 * Returns a new character in which the given lowercase character has been
 * converted into its uppercase equivalent.
 */
char toUpperCase(char ch);

/**
 * Returns a new string in which all lowercase characters have been converted
 * into their uppercase equivalents.
 */
std::string toUpperCase(const std::string& str);

/**
 * Modifies the given string in-place such that all lowercase characters have
 * been converted into their uppercase equivalents.
 */
void toUpperCaseInPlace(std::string& str);

/**
 * Returns a new string after removing any whitespace characters
 * from the beginning and end of the argument.
 */
std::string trim(const std::string& str);

/**
 * Modifies the given string in-place where any whitespace characters
 * from the beginning and end of the argument are removed.
 */
void trimInPlace(std::string& str);

/**
 * Returns a new string after removing any whitespace characters
 * from the end of the argument.
 */
std::string trimEnd(const std::string& str);

/**
 * Modifies the given string in-place to remove any whitespace characters
 * from its end.
 */
void trimEndInPlace(std::string& str);

/**
 * Returns a new string after removing any whitespace characters
 * from the beginning of the argument.
 */
std::string trimStart(const std::string& str);

/**
 * Modifies the given string in-place to remove removing any whitespace characters
 * from the beginning of it.
 */
void trimStartInPlace(std::string& str);

/**
 * Returns a URL-decoded version of the given string, where any %xx character
 * codes are converted back to the equivalent characters.
 */
std::string urlDecode(const std::string& str);

/**
 * Modifies the given string in-place into a URL-decoded version of itself,
 * where any %xx character codes are converted back to the equivalent characters.
 */
void urlDecodeInPlace(std::string& str);

/**
 * Returns a URL-encoded version of the given string, where most non-
 * alphabetic characters are replaced by %xx character codes.
 */
std::string urlEncode(const std::string& str);

/**
 * Modifies the given string in-place into a URL-encoded version of itself,
 * where most non- alphabetic characters are replaced by %xx character codes.
 */
void urlEncodeInPlace(std::string& str);

// add to_string overloads for some common types missing from C++ standard
namespace std {
/**
 * String-to-bool conversion function.
 * If str is "true", returns the bool value true.
 * If str is "false", returns the bool value false.
 * @throw ErrorException if str is any other value than "true" or "false"
 */
bool stob(const std::string& str);

/**
 * String-to-char conversion function.
 * Converts a single-character string into its corresponding char value.
 * For example, stringToChar("hello") returns the char 'h'.
 * @throw ErrorException if the given string does not contain exactly 1 character
 */
char stoc(const std::string& str);

/**
 * Returns the string "true" if b is true, or "false" if b is false.
 */
std::string to_string(bool b);

/**
 * Returns a single-character string containing the given character.
 * For example, charToString('Q') returns the string "Q".
 */
std::string to_string(char c);

/**
 * Returns a hexadecimal string for the given pointer, such as "0x3f0427b".
 * Returns "nullptr" if p is a null pointer.
 */
std::string to_string(void* p);

/**
 * Generic to_string function for any type that has an operator <<.
 */
template <typename T>
std::string to_string(const T& value) {
    std::ostringstream out;
    out << value;   // if you get an error here, your type might not have a << operator
    return out.str();
}
} // namespace std

#endif // _strlib_h
