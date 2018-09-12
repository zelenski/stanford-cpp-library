/*
 * File: gcolorchooser.cpp
 * -----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gfontchooser.cpp to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#include "gfontchooser.h"
#include <QFontDialog>
#include "gfont.h"
#include "gthread.h"

GFontChooser::GFontChooser() {
    // empty
}

std::string GFontChooser::showDialog(const std::string& title, const std::string& initialFont) {
    return showDialog(/* parent */ (QWidget*) nullptr, title, initialFont);
}

std::string GFontChooser::showDialog(GWindow* parent, const std::string& title, const std::string& initialFont) {
    return showDialog(parent ? parent->getWidget() : nullptr, title, initialFont);
}

std::string GFontChooser::showDialog(QWidget* parent, const std::string& title, const std::string& initialFont) {
    QFont initialQFont = GFont::toQFont(initialFont);
    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, initialQFont, &result]() {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, initialQFont, parent, QString::fromStdString(title));
        if (ok) {
            result = GFont::toFontString(font);
        }
    });
    return result;
}
