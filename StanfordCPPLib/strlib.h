/*
 * File: strlib.h
 * --------------
 * This file exports several useful string functions that are not
 * included in the C++ string library.
 * 
 * @version 2015/08/02
 * - added htmlEncode/Decode functions (not 100% perfect but works for common cases)
 * @version 2014/10/19
 * - alphabetized functions
 * - added several 'inPlace' variants of existing functions that return strings
 * @version 2014/10/08
 * - removed dependency on 'using namespace' statement
 */

#ifndef _strlib_h
#define _strlib_h

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/*
 * Returns the string "true" if b is true, or "false" if b is false.
 */
std::string boolToString(bool b);

/*
 * Returns the string "true" if b is nonzero, or "false" if b is 0.
 */
std::string boolToString(int b);

/*
 * Returns a single-character string containing the given character.
 * For example, charToString('Q') returns the string "Q".
 */
std::string charToString(char c);

/*
 * Function: endsWith
 * Usage: if (endsWith(str, suffix)) ...
 * -------------------------------------
 * Returns <code>true</code> if the string <code>str</code> ends with
 * the specified suffix, which may be either a string or a character.
 */
bool endsWith(const std::string& str, const std::string& suffix);
bool endsWith(const std::string& str, char suffix);

/*
 * Function: equalsIgnoreCase
 * Usage: if (equalsIgnoreCase(s1, s2)) ...
 * ----------------------------------------
 * Returns <code>true</code> if <code>s1</code> and <code>s2</code> are
 * equal discounting differences in case.
 */
bool equalsIgnoreCase(const std::string& s1, const std::string& s2);

/*
 * Function: htmlDecode
 * Usage: string s = htmlDecode(s2);
 * ---------------------------------
 * Converts the given string from an HTML-encoded version to its decoded
 * equivalent; the opposite of htmlEncode. Any escaped HTML character entities
 * will be replaced by their unescaped equivalents.
 * For example, <code>htmlEncode("&lt;p class=&quot;abc&quot;&gt;I love you &amp; me&lt;/p&gt;")
 * returns "<p class=\"abc\">I love you & me</p>".
 */
std::string htmlDecode(const std::string& s);

/*
 * Function: htmlEncode
 * Usage: string s = htmlEncode(s2);
 * ---------------------------------
 * Converts the given string into an HTML-encoded equivalent version, with
 * any relevant HTML character entities replaced by escaped equivalents.
 * For example, <code>htmlEncode("<p class=\"abc\">I love you & me</p>") returns
 * "&lt;p class=&quot;abc&quot;&gt;I love you &amp; me&lt;/p&gt;".
 */
std::string htmlEncode(const std::string& s);

/*
 * Function: integerToString
 * Usage: string s = integerToString(n);
 * -------------------------------------
 * Converts an integer into the corresponding string of digits.
 * For example, calling <code>integerToString(123)</code> returns
 * the string <code>"123"</code>.
 */
std::string integerToString(int n);

/*
 * Function: longToString
 * Usage: string s = longToString(n);
 * ----------------------------------
 * Converts an integer into the corresponding string of digits.
 * For example, calling <code>longToString(123)</code> returns
 * the string <code>"123"</code>.
 */
std::string longToString(long n);

/*
 * Function: realToString
 * Usage: string s = realToString(d);
 * ----------------------------------
 * Converts a floating-point number into the corresponding string form.
 * For example, calling <code>realToString(23.45)</code> returns
 * the string <code>"23.45"</code>.
 */
std::string realToString(double d);
std::string doubleToString(double d);   // alias

/*
 * Function: startsWith
 * Usage: if (startsWith(str, prefix)) ...
 * ---------------------------------------
 * Returns <code>true</code> if the string <code>str</code> starts with
 * the specified prefix, which may be either a string or a character.
 */
bool startsWith(const std::string& str, char prefix);
bool startsWith(const std::string& str, const std::string& prefix);

/*
 * Returns true if the given substring occurs somewhere in s.
 */
bool stringContains(const std::string& s, const std::string& substring);

/*
 * Returns the index of the start of the first occurrence of the given substring
 * in s, if it occurs in s.  If it does not occur, returns -1.
 * This function is very similar to string.find, but find returns string::npos
 * when the string is not found.
 */
int stringIndexOf(const std::string& s, const std::string& substring, int startIndex = 0);

/*
 * Returns true if the given string is either "true" or "false".
 */
bool stringIsBool(const std::string& str);

/*
 * Returns true if the given string could be converted to an integer
 * successfully by the stringToInteger function, which will be true if
 * the string has the format of an integer such as "1234" or "-8".
 * Optionally accepts a radix (base) parameter if base-10 is not desired.
 */
bool stringIsInteger(const std::string& str, int radix = 10);

/*
 * Returns true if the given string could be converted to a long
 * successfully by the stringToLong function, which will be true if
 * the string has the format of an integer such as "1234" or "-8".
 * Optionally accepts a radix (base) parameter if base-10 is not desired.
 */
bool stringIsLong(const std::string& str, int radix = 10);

/*
 * Returns true if the given string could be converted to an real number
 * successfully by the stringToReal function, which will be true if
 * the string has the format of a real number such as "3.14" or "-46".
 */
bool stringIsReal(const std::string& str);
bool stringIsDouble(const std::string& str);   // alias

/*
 * Combines the elements of the given STL vector into a single string,
 * with the given delimiter separating neighboring elements, and returns it.
 * For example, joining the elements of the vector
 * {"Hi", "there", "", "Jim"} with the delimiter "?" returns "Hi?there??Jim".
 */
std::string stringJoin(const std::vector<std::string>& v, const std::string& delimiter = "\n");

/*
 * Returns the index of the start of the last occurrence of the given substring
 * in s, if it occurs in s.  If it does not occur, returns -1.
 * This function is very similar to string.rfind, but rfind returns string::npos
 * when the string is not found.
 */
int stringLastIndexOf(const std::string& s, const std::string& substring, int startIndex = 0);

/*
 * Returns a new string formed by replacing any occurrences of the given 'old'
 * text with the given replacement text in 'str'.
 * Note that this is NOT a regular expression replacement; it looks for the
 * 'old' string literally.  If you want regular expressions, see regexpr.h.
 * The 'inPlace' variant modifies an existing string rather than returning a new one,
 * and returns the number of occurrences of 'old' were replaced.
 */
std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit = -1);
int stringReplaceInPlace(std::string& str, const std::string& old, const std::string& replacement, int limit = -1);

/*
 * Returns an STL vector whose elements are strings formed by splitting the
 * given string 'str' by the given separator.
 * For example, splitting "Hi there  Jim!" on " " returns
 * {"Hi", "there", "", "Jim!"}.
 */
std::vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit = -1);

/*
 * If str is "true", returns the bool value true.
 * If str is "false", returns the bool value false.
 * If str is any other value, this function throws an error.
 */
bool stringToBool(const std::string& str);

/*
 * Converts a single-character string into its corresponding char value.
 * For example, stringToChar("hello") returns the char 'h'.
 * Throws an error if the given string does not contain exactly 1 character.
 */
char stringToChar(const std::string& str);

/*
 * Function: stringToInteger
 * Usage: int n = stringToInteger(str);
 * ------------------------------------
 * Converts a string of digits into an integer.  If the string is not a
 * legal integer or contains extraneous characters other than whitespace,
 * <code>stringToInteger</code> calls <code>error</code> with an
 * appropriate message.
 * The function accepts an optional radix (base); for example,
 * stringToInteger("234", 16) assumes that the string is in base-16 and
 * returns 2*16*16 + 3*16 + 4 = 564.
 */
int stringToInteger(const std::string& str, int radix = 10);

/*
 * Function: stringToLong
 * Usage: long n = stringToLong(str);
 * ----------------------------------
 * Converts a string of digits into a long.  If the string is not a
 * legal long or contains extraneous characters other than whitespace,
 * <code>stringToLong</code> calls <code>error</code> with an
 * appropriate message.
 * The function accepts an optional radix (base); for example,
 * stringToLong("234", 16) assumes that the string is in base-16 and
 * returns 2*16*16 + 3*16 + 4 = 564.
 */
long stringToLong(const std::string& str, int radix = 10);

/*
 * Function: stringToReal
 * Usage: double d = stringToReal(str);
 * ------------------------------------
 * Converts a string representing a real number into its corresponding
 * value.  If the string is not a legal floating-point number or contains
 * extraneous characters other than whitespace, <code>stringToReal</code>
 * calls <code>error</code> with an appropriate message.
 */
double stringToReal(const std::string& str);
double stringToDouble(const std::string& str);   // alias

/*
 * Function: toLowerCase
 * Usage: string s = toLowerCase(str);
 * -----------------------------------
 * Returns a new string in which all uppercase characters have been converted
 * into their lowercase equivalents.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string toLowerCase(const std::string& str);
void toLowerCaseInPlace(std::string& str);

/*
 * Function: toUpperCase
 * Usage: string s = toUpperCase(str);
 * -----------------------------------
 * Returns a new string in which all lowercase characters have been converted
 * into their uppercase equivalents.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string toUpperCase(const std::string& str);
void toUpperCaseInPlace(std::string& str);

/*
 * Function: trim
 * Usage: string trimmed = trim(str);
 * ----------------------------------
 * Returns a new string after removing any whitespace characters
 * from the beginning and end of the argument.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string trim(const std::string& str);
void trimInPlace(std::string& str);

/*
 * Function: trimEnd
 * Usage: string trimmed = trimEnd(str);
 * -------------------------------------
 * Returns a new string after removing any whitespace characters
 * from the end of the argument.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string trimEnd(const std::string& str);
void trimEndInPlace(std::string& str);

/*
 * Function: trimStart
 * Usage: string trimmed = trimStart(str);
 * ---------------------------------------
 * Returns a new string after removing any whitespace characters
 * from the beginning of the argument.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string trimStart(const std::string& str);
void trimStartInPlace(std::string& str);

/*
 * Returns a URL-decoded version of the given string, where any %xx character
 * codes are converted back to the equivalent characters.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string urlDecode(const std::string& str);
void urlDecodeInPlace(std::string& str);

/*
 * Returns a URL-encoded version of the given string, where most non-
 * alphabetic characters are replaced by %xx character codes.
 * The 'inPlace' version modifies an existing string rather than returning a new one.
 */
std::string urlEncode(const std::string& str);
void urlEncodeInPlace(std::string& str);

/* Private section */

/**********************************************************************/
/* Note: Everything below this point in the file is logically part    */
/* of the implementation and should not be of interest to clients.    */
/**********************************************************************/

/*
 * Friend function: readQuotedString
 * Usage: readQuotedString(infile, str);
 * -------------------------------------
 * Reads the next string from infile into the reference parameter str.
 * If the first character (other than whitespace) is either a single
 * or a double quote, this function reads characters up to the
 * matching quote, processing standard escape sequences as it goes.
 * If not, readString reads characters up to any of the characters
 * in the string STRING_DELIMITERS in the implementation file.
 */
void readQuotedString(std::istream& is, std::string& str);

/*
 * Friend function: writeQuotedString
 * Usage: writeQuotedString(outfile, str, forceQuotes);
 * ----------------------------------------------------
 * Writes the string str to outfile surrounded by double quotes, converting
 * special characters to escape sequences, as necessary.  If the optional
 * parameter forceQuotes is explicitly set to false, quotes are included
 * in the output only if necessary.
 */
void writeQuotedString(std::ostream& os, const std::string& str,
                       bool forceQuotes = true);

/*
 * Friend function: stringNeedsQuoting
 * Usage: if (stringNeedsQuoting(str)) ...
 * ---------------------------------------
 * Checks whether the string needs quoting in order to be read correctly.
 */
bool stringNeedsQuoting(const std::string& str);

/*
 * Friend function: writeGenericValue
 * Usage: writeGenericValue(os, value, forceQuotes);
 * -------------------------------------------------
 * Writes a generic value to the output stream.  If that value is a string,
 * this function uses writeQuotedString to write the value.
 */
template <typename ValueType>
void writeGenericValue(std::ostream& os, const ValueType& value, bool) {
    os << value;
}

template <>
inline void writeGenericValue(std::ostream& os, const std::string& value,
                              bool forceQuotes) {
    writeQuotedString(os, value, forceQuotes);
}

template <typename ValueType>
inline std::string genericValueToString(const ValueType& value,
                                        bool forceQuotes = false) {
    std::ostringstream os;
    writeGenericValue(os, value, forceQuotes);
    return os.str();
}

template <>
inline std::string genericValueToString(const std::string& value,
                                        bool forceQuotes) {
    std::ostringstream os;
    writeQuotedString(os, value, forceQuotes);
    return os.str();
}

/*
 * Friend function: readGenericValue
 * Usage: readGenericValue(is, value);
 * -----------------------------------
 * Reads a generic value from the input stream.  If that value is a string,
 * this function uses readQuotedString to read the value.
 */
template <typename ValueType>
void readGenericValue(std::istream& is, ValueType& value) {
    is >> value;
}

template <>
inline void readGenericValue(std::istream& is, std::string& value) {
    readQuotedString(is, value);
}

#endif
