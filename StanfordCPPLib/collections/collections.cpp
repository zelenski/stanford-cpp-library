/*
 * File: collections.cpp
 * ---------------------
 * This file implements the collections.h interface.
 * 
 * @version 2019/04/11
 * - added functions to read/write quoted char values
 * @version 2018/10/20
 * - initial version
 */

#include "collections.h"
#include "private/static.h"
#include <iomanip>
#include <iostream>

/*
 * Implementation notes: readQuotedString and writeQuotedString
 * ------------------------------------------------------------
 * Most of the work in these functions has to do with escape sequences.
 */

STATIC_CONST_VARIABLE_DECLARE(std::string, STRING_DELIMITERS, ",:)}]\n")

bool stringNeedsQuoting(const std::string& str) {
    int n = str.length();
    for (int i = 0; i < n; i++) {
        char ch = str[i];
        if (isspace(ch)) return false;
        if (STATIC_VARIABLE(STRING_DELIMITERS).find(ch) != std::string::npos) return true;
    }
    return false;
}

bool readQuotedChar(std::istream& is, char& ch, bool throwOnError) {
    // skip whitespace
    char temp;
    while (is.get(temp) && isspace(temp)) {
        // empty
    }
    if (is.fail()) {
        return true;
    }

    // now we are either at a character, like X, or at the start of a quoted
    // character such as 'X' or '\n'
    if (temp == '\'' || temp == '"') {
        // quoted character; defer to string-reading code
        is.unget();
        std::string s;
        bool result = readQuotedString(is, s, throwOnError);
        if (result && !s.empty()) {
            ch = s[0];
        }
        return result;
    } else {
        // unquoted character; read it ourselves
        // special case: \ (e.g. \n, \t)
        if (temp == '\\') {
            // TODO
            char temp2;
            if (is.get(temp2)) {
                switch (temp2) {
                    case 'a':  ch = '\a'; break;
                    case 'b':  ch = '\b'; break;
                    case 'f':  ch = '\f'; break;
                    case 'n':  ch = '\n'; break;
                    case 'r':  ch = '\r'; break;
                    case 't':  ch = '\t'; break;
                    case 'v':  ch = '\v'; break;
                    case '0':  ch = '\0'; break;
                    case '\\': ch = '\\'; break;
                    case '\'': ch = '\''; break;
                    case '"':  ch = '"'; break;
                    default:   ch = '\0'; break;
                }
            }
        } else {
            ch = temp;
        }
        return true;
    }
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
        while (is.get(ch) && STATIC_VARIABLE(STRING_DELIMITERS).find(ch) == std::string::npos) {
            str += ch;
            if (!isspace(ch)) {
                endTrim = str.length();
            }
        }
        if (is) is.unget();
        str = str.substr(0, endTrim);
    }
    return true;   // read successfully
}

std::ostream& writeQuotedChar(std::ostream& os, char ch, bool forceQuotes) {
    if (forceQuotes) {
        os << '\'';
    }
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
        if (isprint(ch) && ch != '\'') {
            os << ch;
        } else {
            std::ostringstream oss;
            oss << std::oct << std::setw(3) << std::setfill('0') << (int(ch) & 0xFF);
            os << "\\" << oss.str();
        }
    }
    if (forceQuotes) {
        os << '\'';
    }
    return os;
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
