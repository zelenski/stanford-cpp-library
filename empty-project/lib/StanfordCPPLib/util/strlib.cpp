/*
 * File: strlib.cpp
 * ----------------
 * This file implements the strlib.h interface.
 * 
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
std::string doubleToString(double d) {
    return realToString(d);
}

std::string integerToString(int n) {
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

std::string longToString(long n) {
    std::ostringstream stream;
    stream << n;
    return stream.str();
}

std::string pointerToString(void* p) {
    if (p) {
        std::ostringstream stream;
        stream << std::hex;
        stream << "0x" << p;
        return stream.str();
    } else {
        return "NULL";
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
        error("stringIsInteger: Illegal radix: " + integerToString(radix));
    }
    std::istringstream stream(trim(str));
    stream >> std::setbase(radix);
    int value;
    stream >> value;
    return !(stream.fail() || !stream.eof());
}

bool stringIsLong(const std::string& str, int radix) {
    if (radix <= 0) {
        error("stringIsLong: Illegal radix: " + integerToString(radix));
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

bool stringToBool(const std::string& str) {
    std::istringstream stream(trim(str));
    if (str == "true" || str == "1") {
        return true;
    } else if (str == "false" || str == "0") {
        return false;
    }
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
        error("stringToInteger: Illegal radix: " + integerToString(radix));
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
        error("stringToLong: Illegal radix: " + integerToString(radix));
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

/*
 * Implementation notes: case conversion
 * -------------------------------------
 * The functions toUpperCase and toLowerCase return a new string whose
 * characters appear in the desired case. These implementations rely on
 * the fact that the characters in the string are copied when the
 * argument is passed to the function, which makes it possible to change
 * the case of the copy without affecting the original.
 */

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

bool stringContains(const std::string& s, const std::string& substring) {
    return s.find(substring) != std::string::npos;
}

int stringIndexOf(const std::string& s, const std::string& substring, int startIndex) {
    size_t index = s.find(substring, (size_t) startIndex);
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

std::string stringReplace(const std::string& str, const std::string& old, const std::string& replacement, int limit) {
    std::string str2 = str;
    stringReplaceInPlace(str2, old, replacement, limit);
    return str2;
}

int stringReplaceInPlace(std::string& str, const std::string& old, const std::string& replacement, int limit) {
    int count = 0;
    size_t startIndex = 0;
    size_t rlen = replacement.length();
    while (limit < 0 || count < limit) {
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

std::vector<std::string> stringSplit(const std::string& str, const std::string& delimiter, int limit) {
    std::string str2 = str;
    std::vector<std::string> result;
    int count = 0;
    size_t index = 0;
    while (limit < 0 || count < limit) {
        index = str2.find(delimiter);
        if (index == std::string::npos) {
            break;
        }
        result.push_back(str2.substr(0, index));
        str2.erase(str2.begin(), str2.begin() + index + delimiter.length());
        count++;
    }
    if ((int) str2.length() > 0) {
        result.push_back(str2);
    }

    return result;
}

std::string stringJoin(const std::vector<std::string>& v, const std::string& delimiter) {
    if (v.empty()) {
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

std::string urlDecode(const std::string& str) {
    std::ostringstream unescaped;
    for (std::string::const_iterator i = str.begin(), n = str.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '*') {
            unescaped << c;
        } else if (c == '+')  {
            unescaped << ' ';
        } else if (c == '%') {
            // decode a URL-encoded ASCII character, e.g. %40 => &
            // TODO: fails if string is invalid and doesn't have 2 char after %
            //       or if it has non-hex chars here
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
                << (int) c << " (" << (char) c << ")";
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


/*
 * Implementation notes: readQuotedString and writeQuotedString
 * ------------------------------------------------------------
 * Most of the work in these functions has to do with escape sequences.
 */

static const std::string STRING_DELIMITERS = ",:)}]\n";

bool stringNeedsQuoting(const std::string& str) {
    int n = str.length();
    for (int i = 0; i < n; i++) {
        char ch = str[i];
        if (isspace(ch)) return false;
        if (STRING_DELIMITERS.find(ch) != std::string::npos) return true;
    }
    return false;
}

bool readQuotedString(std::istream& is, std::string& str, bool throwOnError) {
    str = "";
    char ch;
    while (is.get(ch) && isspace(ch)) {
        /* Empty */
    }
    if (is.fail()) {
        return true;   // empty string?
    }
    if (ch == '\'' || ch == '"') {
        char delim = ch;
        while (is.get(ch) && ch != delim) {
            if (is.fail()) {
                if (throwOnError) {
                    error("Unterminated string");
                }
                return false;
            }
            if (ch == '\\') {
                if (!is.get(ch)) {
                    if (throwOnError) {
                        error("Unterminated string");
                    }
                    is.setstate(std::ios_base::failbit);
                    return false;
                }
                if (isdigit(ch) || ch == 'x') {
                    int maxDigits = 3;
                    int base = 8;
                    if (ch == 'x') {
                        base = 16;
                        maxDigits = 2;
                    }
                    int result = 0;
                    int digit = 0;
                    for (int i = 0; i < maxDigits && ch != delim; i++) {
                        if (isdigit(ch)) {
                            digit = ch - '0';
                        } else if (base == 16 && isxdigit(ch)) {
                            digit = toupper(ch) - 'A' + 10;
                        } else {
                            break;
                        }
                        result = base * result + digit;
                        if (!is.get(ch)) {
                            if (throwOnError) {
                                error("Unterminated string");
                            }
                            is.setstate(std::ios_base::failbit);
                            return false;
                        }
                    }
                    ch = char(result);
                    is.unget();
                } else {
                    switch (ch) {
                    case 'a': ch = '\a'; break;
                    case 'b': ch = '\b'; break;
                    case 'f': ch = '\f'; break;
                    case 'n': ch = '\n'; break;
                    case 'r': ch = '\r'; break;
                    case 't': ch = '\t'; break;
                    case 'v': ch = '\v'; break;
                    case '"': ch = '"'; break;
                    case '\'': ch = '\''; break;
                    case '\\': ch = '\\'; break;
                    }
                }
            }
            str += ch;
        }
    } else {
        str += ch;
        int endTrim = 0;
        while (is.get(ch) && STRING_DELIMITERS.find(ch) == std::string::npos) {
            str += ch;
            if (!isspace(ch)) endTrim = str.length();
        }
        if (is) is.unget();
        str = str.substr(0, endTrim);
    }
    return true;   // read successfully
}

std::ostream& writeQuotedString(std::ostream& os, const std::string& str, bool forceQuotes) {
    if (!forceQuotes && stringNeedsQuoting(str)) {
        forceQuotes = true;
    }
    if (forceQuotes) {
        os << '"';
    }
    int len = str.length();
    for (int i = 0; i < len; i++) {
        char ch = str.at(i);
        switch (ch) {
        case '\a': os << "\\a"; break;
        case '\b': os << "\\b"; break;
        case '\f': os << "\\f"; break;
        case '\n': os << "\\n"; break;
        case '\r': os << "\\r"; break;
        case '\t': os << "\\t"; break;
        case '\v': os << "\\v"; break;
        case '\\': os << "\\\\"; break;
        default:
            if (isprint(ch) && ch != '"') {
                os << ch;
            } else {
                std::ostringstream oss;
                oss << std::oct << std::setw(3) << std::setfill('0') << (int(ch) & 0xFF);
                os << "\\" << oss.str();
            }
        }
    }
    if (forceQuotes) {
        os << '"';
    }
    return os;
}
