/*
 * File: strlib.cpp
 * ----------------
 * This file implements the strlib.h interface.
 * 
 * @version 2018/11/14
 * - added std::to_string for bool, char, pointer, and generic template type T
 * - bug fix for pointerToString (was putting two "0x" prefixes)
 * @version 2018/09/02
 * - added padLeft, padRight
 * @version 2017/10/24
 * - print nullptr instead of null in uppercase
 * @version 2016/11/07
 * - bug fix: urlDecode throws error on invalid encodings (courtesy GitHub @scinart)
 * @version 2016/10/30
 * - alphabetized functions
 * - added overloads that take type char instead of string:
 *   stringContains, stringIndexOf, stringJoin, stringLastIndexOf, stringReplace,
 *   stringSplit, toLowerCase, toUpperCase
 * @version 2016/10/13
 * - modified writeQuotedString to return ostream
 * @version 2016/08/03
 * - modified readQuotedString not to throw error() on parse failures
 *   (needed to support idiomatic silent-failing >> operators)
 * @version 2015/11/07
 * - fixed bugs in urlDecode (wasn't decoding % sequences properly, oops)
 * @version 2015/10/26
 * - added charToInteger/integerToChar functions
 * @version 2015/06/19
 * - slight bug fix to make stringToInteger functions compile with int radix
 * @version 2015/05/22
 * - slight bug fix in stringToBool function
 * @version 2014/10/31
 * - fixed infinite loop bug in stringReplace function
 * @version 2014/10/19
 * - alphabetized functions
 * - added several 'inPlace' variants of existing functions that return strings
 * @version 2014/10/08
 * - removed 'using namespace' statement
 */

#include "strlib.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "error.h"
#include "vector.h"

/* Function prototypes */

std::string boolToString(bool b) {
    return (b ? "true" : "false");
}

std::string boolToString(int b) {
    return (b ? "true" : "false");
}

int charToInteger(char c) {
    if (c < '0' || c > '9') {
        std::ostringstream out;
        out << "charToInteger: character is not numeric: '" << c
            << "' (ASCII value " << (int) c << ")";
        error(out.str());
    }
    return c - '0';
}

std::string charToString(char c) {
    std::string s;
    s += c;
    return s;
}

std::string doubleToString(double d) {
    return realToString(d);
}

bool endsWith(const std::string& str, char suffix) {
    return str.length() > 0 && str[str.length() - 1] == suffix;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    int nChars = suffix.length();
    int start = str.length() - nChars;
    if (start < 0) return false;
    for (int i = 0; i < nChars; i++) {
        if (str[start + i] != suffix[i]) return false;
    }
    return true;
}

/*
 * Implementation notes: equalsIgnoreCase
 * --------------------------------------
 * This implementation uses a for loop to cycle through the characters in
 * each string.  Converting each string to uppercase and then comparing
 * the results makes for a shorter but less efficient implementation.
 */
bool equalsIgnoreCase(const std::string& s1, const std::string& s2) {
    if (s1.length() != s2.length()) return false;
    int nChars = s1.length();
    for (int i = 0; i < nChars; i++) {
        if (tolower(s1[i]) != tolower(s2[i])) return false;
    }
    return true;
}

std::string htmlDecode(const std::string& s) {
    std::string result = s;
    stringReplaceInPlace(result, "&lt;", "<");
    stringReplaceInPlace(result, "&gt;", ">");
    stringReplaceInPlace(result, "&quot;", "\"");
    stringReplaceInPlace(result, "&amp;", "&");
    return result;
}

std::string htmlEncode(const std::string& s) {
    std::string result = s;
    stringReplaceInPlace(result, "&", "&amp;");
    stringReplaceInPlace(result, "<", "&lt;");
    stringReplaceInPlace(result, ">", "&gt;");
    stringReplaceInPlace(result, "\"", "&quot;");
    return result;
}

char integerToChar(int n) {
    if (n < 0 || n > 9) {
        std::ostringstream out;
        out << "integerToChar: number must be between 0-9: " << n;
        error(out.str());
    }
    return (char) (n + '0');
}

/*
 * Implementation notes: numeric conversion
 * ----------------------------------------
 * These functions use the <sstream> library to perform the conversion.
 */
std::string integerToString(int n, int radix) {
    if (radix <= 0) {
        error("integerToString: Illegal radix: " + std::to_string(radix));
    }
    std::ostringstream stream;
    if (radix != 10) {
        stream << std::setbase(radix);
    }
    stream << n;
    return stream.str();
}

std::string longToString(long n, int radix) {
    if (radix <= 0) {
        error("longToString: Illegal radix: " + std::to_string(radix));
    }
    std::ostringstream stream;
    if (radix != 10) {
        stream << std::setbase(radix);
    }
    stream << n;
    return stream.str();
}

std::string padLeft(const std::string& s, int length, char fill) {
    if ((int) s.length() >= length) {
        return s;
    } else {
        std::ostringstream out;
        for (int i = 0, count = length - (int) s.length(); i < count; i++) {
            out << fill;
        }
        out << s;
        return out.str();
    }
}

std::string padRight(const std::string& s, int length, char fill) {
    if ((int) s.length() >= length) {
        return s;
    } else {
        std::ostringstream out;
        out << s;
        for (int i = 0, count = length - (int) s.length(); i < count; i++) {
            out << fill;
        }
        return out.str();
    }
}

std::string pointerToString(void* p) {
    if (p) {
        std::ostringstream stream;
        stream << std::hex;
        stream << p;
        return stream.str();
    } else {
        return "nullptr";
    }
}

std::string realToString(double d) {
    std::ostringstream stream;
    stream << std::uppercase << d;
    return stream.str();
}

bool startsWith(const std::string& str, char prefix) {
    return str.length() > 0 && str[0] == prefix;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) return false;
    int nChars = prefix.length();
    for (int i = 0; i < nChars; i++) {
        if (str[i] != prefix[i]) return false;
    }
    return true;
}

bool stringIsBool(const std::string& str) {
    return str == "true" || str == "false";
}

bool stringIsDouble(const std::string& str) {
    return stringIsReal(str);
}

bool stringIsInteger(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringIsInteger: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    int value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringIsLong(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringIsLong: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    long value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringIsReal(const std::string& str) {
    std::istringstream stream(trim(str));
    double value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringContains(const std::string& s, char ch) {
    return s.find(ch) != std::string::npos;
}

bool stringContains(const std::string& s, const std::string& substring) {
    return s.find(substring) != std::string::npos;
}

int stringIndexOf(const std::string& s, char ch, int startIndex) {
    size_t index = s.find(ch, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

int stringIndexOf(const std::string& s, const std::string& substring, int startIndex) {
    size_t index = s.find(substring, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

std::string stringJoin(const Vector<std::string>& v, char delimiter) {
    std::string delim = charToString(delimiter);
    return stringJoin(v, delim);
}

std::string stringJoin(const Vector<std::string>& v, const std::string& delimiter) {
    if (v.isEmpty()) {
        return "";
    } else {
        std::ostringstream out;
        out << v[0];
        for (int i = 1; i < (int) v.size(); i++) {
            out << delimiter;
            out << v[i];
        }
        return out.str();
    }
}

int stringLastIndexOf(const std::string& s, char ch, int startIndex) {
    size_t index = s.rfind(ch, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

int stringLastIndexOf(const std::string& s, const std::string& substring, int startIndex) {
    size_t index = s.rfind(substring, (size_t) startIndex);
    if (index == std::string::npos) {
        return -1;
    } else {
        return index;
    }
}

std::string stringReplace(const std::string& str, char old, char replacement, int limit) {
    std::string str2 = str;
    stringReplaceInPlace(str2, old, replacement, limit);
    return str2;
}

std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit) {
    std::string str2 = str;
    stringReplaceInPlace(str2, old, replacement, limit);
    return str2;
}

int stringReplaceInPlace(std::string& str, char old, char replacement, int limit) {
    int count = 0;
    for (size_t i = 0, len = str.length(); i < len; i++) {
        if (str[i] == old) {
            str[i] = replacement;
            count++;
            if (limit > 0 && count >= limit) {
                break;
            }
        }
    }
    return count;
}

int stringReplaceInPlace(std::string& str, const std::string& old, const std::string& replacement, int limit) {
    int count = 0;
    size_t startIndex = 0;
    size_t rlen = replacement.length();
    while (limit <= 0 || count < limit) {
        size_t index = str.find(old, startIndex);
        if (index == std::string::npos) {
            break;
        }
        str.replace(index, old.length(), replacement);
        startIndex = index + rlen;
        count++;
    }
    return count;
}

Vector<std::string> stringSplit(const std::string& str, char delimiter, int limit) {
    std::string delim = charToString(delimiter);
    return stringSplit(str, delim, limit);
}

Vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit) {
    std::string str2 = str;
    Vector<std::string> result;
    int count = 0;
    size_t index = 0;
    while (limit < 0 || count < limit) {
        index = str2.find(delimiter);
        if (index == std::string::npos) {
            break;
        }
        result.add(str2.substr(0, index));
        str2.erase(str2.begin(), str2.begin() + index + delimiter.length());
        count++;
    }
    if ((int) str2.length() > 0) {
        result.add(str2);
    }

    return result;
}

bool stringToBool(const std::string& str) {
    if (str == "true" || str == "1") {
        return true;
    } else if (str == "false" || str == "0") {
        return false;
    }
    std::istringstream stream(trim(str));
    bool value;
    stream >> std::boolalpha >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToBool: Illegal bool format (" + str + ")");
    }
    return value;
}

char stringToChar(const std::string& str) {
    std::string str2 = trim(str);
    if ((int) str2.length() != 1) {
        error("stringToChar: string must contain exactly 1 non-whitespace character");
    }
    return str2[0];
}

double stringToDouble(const std::string& str) {
    return stringToReal(str);
}

int stringToInteger(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringToInteger: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    int value;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToInteger: Illegal integer format: \"" + str + "\"");
    }
    return value;
}

long stringToLong(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringToLong: Illegal radix: " + std::to_string(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    long value;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToLong: Illegal long format \"" + str + "\"");
    }
    return value;
}

double stringToReal(const std::string& str) {
    std::istringstream stream(trim(str));
    double value;
    stream >> value;
    if (stream.fail() || !stream.eof()) {
        error("stringToReal: Illegal floating-point format (" + str + ")");
    }
    return value;
}

char toLowerCase(char ch) {
    return (char) tolower(ch);
}

std::string toLowerCase(const std::string& str) {
    std::string str2 = str;
    toLowerCaseInPlace(str2);
    return str2;
}

void toLowerCaseInPlace(std::string& str) {
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        str[i] = tolower(str[i]);
    }
}

char toUpperCase(char ch) {
    return (char) toupper(ch);
}

std::string toUpperCase(const std::string& str) {
    std::string str2 = str;
    toUpperCaseInPlace(str2);
    return str2;
}

void toUpperCaseInPlace(std::string& str) {
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        str[i] = toupper(str[i]);
    }
}

std::string trim(const std::string& str) {
    std::string str2 = str;
    trimInPlace(str2);
    return str2;
}

void trimInPlace(std::string& str) {
    trimEndInPlace(str);
    trimStartInPlace(str);
}

std::string trimEnd(const std::string& str) {
    std::string str2 = str;
    trimEndInPlace(str2);
    return str2;
}

void trimEndInPlace(std::string& str) {
    int end = (int) str.length();
    int finish = end;
    while (finish > 0 && isspace(str[finish - 1])) {
        finish--;
    }
    if (finish < end) {
        str.erase(finish, end - finish);
    }
}

std::string trimStart(const std::string& str) {
    std::string str2 = str;
    trimStartInPlace(str2);
    return str2;
}

void trimStartInPlace(std::string& str) {
    int start = 0;
    int finish = (int) str.length() - 1;
    while (start <= finish && isspace(str[start])) {
        start++;
    }
    if (start > 0) {
        str.erase(0, start);
    }
}

std::string urlDecode(const std::string& str) {
    std::ostringstream unescaped;
    for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            unescaped << c;
        } else if (c == '+')  {
            unescaped << ' ';
        } else if (c == '%') {
            // throw error if string is invalid and doesn't have 2 char after,
            // or if it has non-hex chars here (courtesy GitHub @scinart)
            if (i + 2 >= n || !isxdigit(*(i + 1)) || !isxdigit(*(i + 2))) {
                error("urlDecode: Invalid percent-encoding");
            }

            // decode a URL-encoded ASCII character, e.g. %40 => &
            char ch1 = *(i + 1);
            char ch2 = *(i + 2);
            int hex1 = (isdigit(ch1) ? (ch1 - '0') : (toupper(ch1) - 'A' + 10));
            int hex2 = (isdigit(ch2) ? (ch2 - '0') : (toupper(ch2) - 'A' + 10));
            int decodedChar = (hex1 << 4) + hex2;
            unescaped << (char) decodedChar;
            i += 2;
        } else {
            std::ostringstream msg;
            msg << "urlDecode: Unexpected character in string: "
                << (int) c << " (" << c << ")";
            error(msg.str());
        }
    }

    return unescaped.str();
}

void urlDecodeInPlace(std::string& str) {
    str = urlDecode(str);   // no real efficiency gain here
}

std::string urlEncode(const std::string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex << std::uppercase;

    for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            escaped << c;
        } else if (c == ' ')  {
            escaped << '+';
        } else {
            escaped << '%' << std::setw(2) << ((int) c) << std::setw(0);
        }
    }

    return escaped.str();
}

void urlEncodeInPlace(std::string& str) {
    str = urlEncode(str);   // no real efficiency gain here
}

namespace std {
bool stob(const std::string& str) {
    return ::stringToBool(str);
}

char stoc(const std::string& str) {
    return ::stringToChar(str);
}

std::string to_string(bool b) {
    return ::boolToString(b);
}

std::string to_string(char c) {
    return ::charToString(c);
}

std::string to_string(void* p) {
    return ::pointerToString(p);
}
} // namespace std
