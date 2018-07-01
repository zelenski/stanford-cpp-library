/*
 * File: qgcolor.cpp
 * -----------------
 *
 * @version 2018/06/30
 * - initial version
 */

#include "qgcolor.h"
#include <iomanip>
#include <iostream>
#include "error.h"
#include "strlib.h"

Map<std::string, int> QGColor::_colorTable;

QGColor::QGColor() {
    // empty
}

std::string QGColor::canonicalColorName(const std::string& str) {
    std::string result = "";
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        char ch = str[i];
        if (!isspace(ch) && ch != '_') result += tolower(ch);
    }
    return result;
}

Map<std::string, int>& QGColor::colorTable() {
    if (_colorTable.isEmpty()) {
        _colorTable["black"] = 0x000000;
        _colorTable["blue"] = 0x0000FF;
        _colorTable["brown"] = 0x926239;
        _colorTable["cyan"] = 0x00FFFF;
        _colorTable["darkgray"] = 0x595959;
        _colorTable["gray"] = 0x999999;
        _colorTable["green"] = 0x00FF00;
        _colorTable["lightgray"] = 0xBFBFBF;
        _colorTable["magenta"] = 0xFF00FF;
        _colorTable["orange"] = 0xFFC800;
        _colorTable["pink"] = 0xFFAFAF;
        _colorTable["purple"] = 0xFF00FF;
        _colorTable["red"] = 0xFF0000;
        _colorTable["white"] = 0xFFFFFF;
        _colorTable["yellow"] = 0xFFFF00;
    }
    return _colorTable;
}

std::string QGColor::convertARGBToColor(int a, int r, int g, int b) {
    if (a < 0 || a > 255 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        error("QGColor::convertARGBToColor: invalid ARGB value (must be 0-255)");
    }
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (a & 0xFF);
    os << std::setw(2) << (r & 0xFF);
    os << std::setw(2) << (g & 0xFF);
    os << std::setw(2) << (b & 0xFF);
    return os.str();
}

int QGColor::convertColorToRGB(const std::string& colorName) {
    if (colorName == "") return -1;
    if (colorName[0] == '#') {
        std::istringstream is(colorName.substr(1) + "@");
        int rgb;
        char terminator = '\0';
        is >> std::hex >> rgb >> terminator;
        if (terminator != '@') {
            error("convertColorToRGB: Illegal color - " + colorName);
        }
        return rgb;
    }
    std::string name = canonicalColorName(colorName);
    if (!colorTable().containsKey(name)) {
        error("convertColorToRGB: Undefined color - " + colorName);
    }
    return colorTable()[name];
}

std::string QGColor::convertRGBToColor(int rgb) {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (rgb >> 16 & 0xFF);
    os << std::setw(2) << (rgb >> 8 & 0xFF);
    os << std::setw(2) << (rgb & 0xFF);
    return os.str();
}

std::string QGColor::convertRGBToColor(int r, int g, int b) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        error("QGColor::convertRGBToColor: invalid RGB value (must be 0-255)");
    }
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (r & 0xFF);
    os << std::setw(2) << (g & 0xFF);
    os << std::setw(2) << (b & 0xFF);
    return os.str();
}

int QGColor::convertARGBToARGB(int a, int r, int g, int b) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

int QGColor::convertRGBToRGB(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

// if RGB is not completely black, but alpha is 0, assume that the
// client meant to use an opaque color and add ff as alpha channel
int QGColor::fixAlpha(int argb) {
    int alpha = ((argb & 0xff000000) >> 24) & 0x000000ff;
    if (alpha == 0 && (argb & 0x00ffffff) != 0) {
        argb = argb | 0xff000000;   // set full 255 alpha
    }
    return argb;
}
