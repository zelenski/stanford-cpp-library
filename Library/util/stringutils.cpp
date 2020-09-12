/*
 * File: stringutils.cpp
 * ---------------------
 * This file contains implementations of utility functions related to strings.
 * See stringutils.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2017/10/20
 * - changed string to const string& in all functions
 * @version 2016/11/09
 * - added trimToSize function
 * @version 2014/10/31
 * - fixed bug in string trimToHeight function
 * @since 2014/03/01
 */

#include "stringutils.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>

namespace stringutils {
int charsDifferent(const std::string& s1, const std::string& s2) {
    int length = std::min(s1.length(), s2.length());
    int count = 0;
    for (int i = 0; i < length; i++) {
        if (s1[i] != s2[i]) {
            count++;
        }
    }
    return count;
}

std::string collapseSpaces(const std::string& s) {
    std::ostringstream result;
    char last = '\0';
    for (int i = 0; i < (int) s.length(); i++) {
        if (s[i] == ' ' && last == ' ') {
            continue;
        }
        last = s[i];
        result << last;
    }
    return result.str();
}

Vector<std::string> explodeLines(const std::string& s) {
    Vector<std::string> result;
    std::string line;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == '\n') {
            // end of line
            result.add(trimR(line));
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }
    if ((int) line.length() > 0 || s.empty()) {
        result.add(line);
    }
    return result;
}

int height(const std::string& s) {
    std::string line;
    int height = 0;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == '\n') {
            // end of line
            height++;
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }
    if ((int) line.length() > 0) {
        height++;
    }
    return height;
}

std::string implode(const Vector<std::string>& v, const std::string& delimiter) {
    if (v.isEmpty()) {
        return "";
    } else {
        std::string result = v[0];
        for (int i = 1; i < v.size(); i++) {
            result += delimiter;
            result += v[i];
        }
        return result;
    }
}

std::string indent(const std::string& s, int spaces) {
    Vector<std::string> lines = explodeLines(s);
    Vector<std::string> newLines;
    std::string indentStr = "";
    for (int i = 0; i < spaces; i++) {
        indentStr += " ";
    }
    for (const std::string& line : lines) {
        newLines.add(indentStr + line);
    }
    return implode(newLines);
}

std::string makeSloppy(const std::string& s) {
    return collapseSpaces(removeBlankLines(s));
}

std::string removeBlankLines(const std::string& s) {
    Vector<std::string> lines = explodeLines(s);
    Vector<std::string> newLines;
    for (std::string line : lines) {
        line = trimR(line);
        if (line.length() > 0) {
            newLines.add(line);
        }
    }
    return implode(newLines, "\n");
}

std::string stripWhitespace(const std::string& s) {
    std::ostringstream result;
    for (size_t i = 0; i < s.length(); i++) {
        if (!isspace(s[i])) {
            result << tolower(s[i]);
        }
    }
    return result.str();
}

std::string toLowerCase(const std::string& s) {
    std::string result;
    for (size_t i = 0; i < s.length(); i++) {
        result += tolower(s[i]);
    }
    return result;
}

std::string toPrintable(int ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == ' ') {
        return "' '";
    } else if (!isgraph(ch)) {
        return "???";
    } else {
        return std::string("'") + (char) ch + std::string("'");
    }
}

std::string toPrintable(const std::string& s) {
    std::ostringstream out;
    for (char ch : s) {
        if (ch == '\n') {
            out << "\\n";
        } else if (ch == '\t') {
            out << "\\t";
        } else if (ch == '\r') {
            out << "\\r";
        } else if (ch == '\f') {
            out << "\\f";
        } else if (ch == '\b') {
            out << "\\b";
        } else if (ch == '\0') {
            out << "\\0";
        } else if (ch == ' ') {
            out << ' ';
        } else if (isgraph(ch)) {
            out << ch;
        } else {
            out << '\\';
            out << (static_cast<int>(ch & 0xff));
        }
    }
    return out.str();
}
std::string trimR(const std::string& s) {
    std::string trimmed = s;
    while (!trimmed.empty() && isspace(trimmed[trimmed.length() - 1])) {
        trimmed.erase(trimmed.length() - 1, 1);
    }
    return trimmed;
}

std::string trimToHeight(const std::string& s, int height, const std::string& suffix) {
    Vector<std::string> lines = explodeLines(s);
    int lineCount = lines.size();
    bool wasTooTall = lineCount > height;
    while (lines.size() > height) {
        lines.remove(lines.size() - 1);
    }
    if (wasTooTall && !suffix.empty()) {
        lines.add(suffix + " (" + std::to_string(lineCount - height) + " line(s) truncated)");
    }
    return implode(lines);
}

std::string trimToSize(const std::string& s, int width, int height, const std::string& suffix) {
    std::string trimmed = trimToHeight(s, height, suffix);
    trimmed = trimToWidth(trimmed, width, suffix);
    return trimmed;
}

std::string trimToWidth(const std::string& s, int width, const std::string& suffix) {
    Vector<std::string> lines = explodeLines(s);
    for (int i = 0; i < lines.size(); i++) {
        if ((int) lines[i].length() > width) {
            lines[i] = lines[i].substr(0, width);
            if (!suffix.empty()) {
                lines[i] += suffix;
            }
        }
    }
    return implode(lines);
}

// truncate string with ... between
std::string truncate(const std::string& s, int length, const std::string& suffix) {
    int slength = s.length();
    if (slength > length) {
        return s.substr(0, length) + suffix;
    } else {
        return s;
    }
}

int width(const std::string& s) {
    std::string line;
    size_t width = 0;
    for (size_t i = 0; i < s.length(); i++) {
        char ch = s[i];
        if (ch == '\n') {
            // end of line
            if (line.length() > width) {
                width = line.length();
            }
            line = "";
        } else if (ch != '\r') {
            line += ch;
        }
    }
    if ((int) line.length() > 0) {
        if (line.length() > width) {
            width = line.length();
        }
    }
    return width;
}




} // namespace stringutils
