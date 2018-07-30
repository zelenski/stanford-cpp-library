/*
 * File: qgfont.cpp
 * ----------------
 *
 * @version 2018/07/05
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgfont.h"
#include <iomanip>
#include <iostream>
#include "error.h"
#include "strlib.h"
#include "vector.h"

QGFont::QGFont() {
    // empty
}

QFont QGFont::deriveQFont(const QFont& font, QFont::Weight weight, int size) {
    return QFont(font.family(), size, weight, /* italic */ font.italic());
}

QFont QGFont::deriveQFont(const QFont& font, const std::string& fontFamily, QFont::Weight weight, int size) {
    return QFont(QString::fromStdString(fontFamily), size, weight, /* italic */ font.italic());
}

// http://doc.qt.io/qt-5/qfont.html#StyleHint-enum
QFont::StyleHint QGFont::getStyleHint(const std::string& fontFamily) {
    std::string fontFamilyStr = toLowerCase(trim(fontFamily));
    if (fontFamilyStr == "arial"
            || fontFamilyStr == "helvetica"
            || fontFamilyStr == "sansserif"
            || fontFamilyStr == "sans-serif") {
        return QFont::Helvetica;
    } else if (fontFamilyStr == "serif"
            || fontFamilyStr == "times"
            || fontFamilyStr == "times roman"
            || fontFamilyStr == "times new roman") {
        return QFont::Times;
    } else if (fontFamilyStr == "courier"
            || fontFamilyStr == "courier new"
            || fontFamilyStr == "consolas"
            || fontFamilyStr == "monospace"
            || fontFamilyStr == "monospaced"
            || endsWith(fontFamilyStr, " mono")) {
        return QFont::Monospace;
    } else if (fontFamilyStr == "comic sans"
            || fontFamilyStr == "comic sans ms"
            || fontFamilyStr == "fantasy") {
        return QFont::Fantasy;
    } else if (fontFamilyStr == "cursive") {
        return QFont::Cursive;
    } else if (fontFamilyStr == "system") {
        return QFont::System;
    } else {
        return QFont::AnyStyle;
    }
}

// example font string:
// "Courier New-Bold-12"
std::string QGFont::toFontString(const QFont& font) {
    std::ostringstream out;
    out << font.family().toStdString();

    // http://doc.qt.io/qt-5/qfont.html#Weight-enum
    bool dashPrinted = false;
    switch (font.weight()) {
        case QFont::DemiBold:
        case QFont::Bold:
        case QFont::ExtraBold:
        case QFont::Black:
            out << "-bold";
            dashPrinted = true;
        case QFont::Thin:
        case QFont::ExtraLight:
        case QFont::Light:
        case QFont::Normal:
        case QFont::Medium:
        default:
            break;
    }
    if (font.italic()) {
        if (!dashPrinted) {
            dashPrinted = true;
            out << "-";
        }
        out << "italic";
    }

    out << "-" << font.pointSize();
    return out.str();
}

// example font string:
// "Courier New-Bold-12"
QFont QGFont::toQFont(const std::string& fontString) {
    Vector<std::string> tokens = stringSplit(trim(fontString), "-");
    if (tokens.isEmpty()) {
        return QFont();
    }

    std::string fontFamily = "";
    bool fontBold = false;
    bool fontItalic = false;
    int fontSize = 12;   // 12pt standard font size

    if (!tokens.isEmpty()) {
        fontFamily = trim(tokens.removeFront());
        if (fontFamily == "*") {
            fontFamily = "SansSerif";
        }
    }
    for (int i = 0; i < 2; i++) {
        if (!tokens.isEmpty()) {
            // tokens 2-3 can be size-style or style-size
            std::string fontWeightStr = toLowerCase(trim(tokens.removeFront()));
            if (stringIsInteger(fontWeightStr)) {
                fontSize = stringToInteger(fontWeightStr);
            }
            if (stringContains(fontWeightStr, "bold")) {
                fontBold = true;
            }
            if (stringContains(fontWeightStr, "italic")) {
                fontItalic = true;
            }
        }
    }

    QFont font = QFont(QString::fromStdString(fontFamily),
                       fontSize,
                       fontBold ? QFont::Bold : QFont::Normal,
                       fontItalic);
    font.setStyleHint(getStyleHint(fontFamily));
    return font;
}

QFont QGFont::toQFont(const QFont& basisFont, const std::string& fontString) {
    Vector<std::string> tokens = stringSplit(trim(fontString), "-");
    if (tokens.isEmpty()) {
        return QFont();
    }

    std::string fontFamily = "";
    bool fontBold = basisFont.bold();
    bool fontItalic = basisFont.italic();
    int fontSize = basisFont.pointSize();   // 12pt standard font size

    if (!tokens.isEmpty()) {
        fontFamily = trim(tokens.removeFront());
        if (fontFamily == "*") {
            fontFamily = basisFont.family().toStdString();
        }
    }
    for (int i = 0; i < 2; i++) {
        if (!tokens.isEmpty()) {
            // tokens 2-3 can be size-style or style-size
            std::string fontWeightStr = toLowerCase(trim(tokens.removeFront()));
            if (stringIsInteger(fontWeightStr)) {
                fontSize = stringToInteger(fontWeightStr);
            } else if (stringContains(fontWeightStr, "bold")) {
                fontBold = true;
            } else if (stringContains(fontWeightStr, "italic")) {
                fontItalic = true;
            }
        }
    }

    QFont font = QFont(QString::fromStdString(fontFamily),
                       fontSize,
                       fontBold ? QFont::Bold : QFont::Normal,
                       fontItalic);
    font.setStyleHint(getStyleHint(fontFamily));
    return font;
}

#endif // SPL_QT_GUI
