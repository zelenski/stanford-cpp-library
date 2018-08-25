/*
 * File: gcolor.cpp
 * ----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gcolor.cpp to replace Java version
 * @version 2018/06/30
 * - initial version
 */

#include "gcolor.h"
#include <iomanip>
#include <iostream>
#include "error.h"
#include "strlib.h"

Map<std::string, int> GColor::_colorTable;
Map<std::string, std::string> GColor::_colorNameTable;

GColor::GColor() {
    // empty
}

std::string GColor::canonicalColorName(const std::string& str) {
    std::string result = "";
    int nChars = str.length();
    for (int i = 0; i < nChars; i++) {
        char ch = str[i];
        if (!isspace(ch) && ch != '_') result += tolower(ch);
    }
    return result;
}

const Map<std::string, int>& GColor::colorTable() {
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

const Map<std::string, std::string>& GColor::colorNameTable() {
    if (_colorNameTable.isEmpty()) {
        _colorNameTable["#000000"] = "black";
        _colorNameTable["#0000ff"] = "blue";
        _colorNameTable["#926239"] = "brown";
        _colorNameTable["#00ffff"] = "cyan";
        _colorNameTable["#595959"] = "darkgray";
        _colorNameTable["#999999"] = "gray";
        _colorNameTable["#00ff00"] = "green";
        _colorNameTable["#bfbfbf"] = "lightgray";
        _colorNameTable["#ff00ff"] = "magenta";
        _colorNameTable["#ffc800"] = "orange";
        _colorNameTable["#ffafaf"] = "pink";
        _colorNameTable["#ff00ff"] = "purple";
        _colorNameTable["#ff0000"] = "red";
        _colorNameTable["#ffffff"] = "white";
        _colorNameTable["#ffff00"] = "yellow";
    }
    return _colorNameTable;
}

std::string GColor::convertARGBToColor(int a, int r, int g, int b) {
    if (a < 0 || a > 255 || r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        error("GColor::convertARGBToColor: invalid ARGB value (must be 0-255)");
    }
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (a & 0xFF);
    os << std::setw(2) << (r & 0xFF);
    os << std::setw(2) << (g & 0xFF);
    os << std::setw(2) << (b & 0xFF);
    return os.str();
}

int GColor::convertColorToRGB(const std::string& colorName) {
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

std::string GColor::convertRGBToColor(int rgb) {
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (rgb >> 16 & 0xFF);
    os << std::setw(2) << (rgb >> 8 & 0xFF);
    os << std::setw(2) << (rgb & 0xFF);
    std::string color = os.str();
    if (colorNameTable().containsKey(toLowerCase(color))) {
        return colorNameTable()[toLowerCase(color)];
    } else {
        return color;
    }
}

std::string GColor::convertRGBToColor(int r, int g, int b) {
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        error("GColor::convertRGBToColor: invalid RGB value (must be 0-255)");
    }
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::uppercase << "#";
    os << std::setw(2) << (r & 0xFF);
    os << std::setw(2) << (g & 0xFF);
    os << std::setw(2) << (b & 0xFF);
    std::string color = os.str();
    if (colorNameTable().containsKey(toLowerCase(color))) {
        return colorNameTable()[toLowerCase(color)];
    } else {
        return color;
    }
}

int GColor::convertARGBToARGB(int a, int r, int g, int b) {
    return (a << 24) | (r << 16) | (g << 8) | b;
}

int GColor::convertRGBToRGB(int r, int g, int b) {
    return (r << 16) | (g << 8) | b;
}

std::string GColor::convertQColorToColor(const QColor& color) {
    return convertRGBToColor(color.red(), color.green(), color.blue());
}

int GColor::convertQColorToRGB(const QColor& color) {
    return convertRGBToRGB(color.red(), color.green(), color.blue());
}

QColor GColor::toQColor(const std::string& color) {
    int rgb = convertColorToRGB(color);
    return QColor(rgb | 0xff000000);
}

// if RGB is not completely black, but alpha is 0, assume that the
// client meant to use an opaque color and add ff as alpha channel
int GColor::fixAlpha(int argb) {
    int alpha = ((argb & 0xff000000) >> 24) & 0x000000ff;
    if (alpha == 0 && (argb & 0x00ffffff) != 0) {
        argb = argb | 0xff000000;   // set full 255 alpha
    }
    return argb;
}
