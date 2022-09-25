/**
 * TextUtils.cpp
 *
 * @author Keith Schwarz
 * @version 2020/3/5
 *    Keith final revision from end of quarter 19-2
 */
#include "TextUtils.h"
#include <sstream>
#include <iomanip>
using namespace std;

namespace {
    /* Quotes the given object with the specified escape characters. */
    string quotedVersionOf(const string& str, char quoteChar) {
        ostringstream result;
        result << quoteChar;

        for (char ch: str) {
            /* Escape close quotes. */
            if (ch == '\"') result << "\\\"";
            if (ch == '\'') result << "\\\'";

            /* Escape slashes. */
            else if (ch == '\\') result << "\\\\";

            /* Print out any other printable characters. */
            else if (isgraph(ch) || ch == ' ') result << ch;

            /* Otherwise, escape it. */
            else {
                result << "\\x" << hex << setfill('0') << setw(2) << +static_cast<unsigned char>(ch);
            }
        }

        result << quoteChar;
        return result.str();
    }
}


/* Inserts commas into a numeric string. */
string addCommasTo(int val) {
    string asStr = to_string(val);
    const int length = int(asStr.length());

    string result;
    for (int i = 0; i < length; i++) {
        /* Run backwards through the source string so determining where commas go
         * becomes a lot easier.
         */
        result = asStr[length - 1 - i] + result;

        /* Put commas in provided we've already added three characters, but aren't
         * about to use all the digits up.
         */
        if (i % 3 == 2 && i < length - 1) {
            result = ',' + result;
        }
    }
    return result;
}

/* TODO: Once C++14 support is more widespread, replace with std::quoted. */
string quotedVersionOf(const string& source) {
    return quotedVersionOf(source, '"');
}
string quotedVersionOf(char source) {
    return quotedVersionOf(string(1, source), '\'');
}
