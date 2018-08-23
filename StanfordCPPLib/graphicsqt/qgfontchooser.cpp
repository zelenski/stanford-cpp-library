/*
 * File: qgcolorchooser.cpp
 * ------------------------
 * This file implements the operations declared in the qgfilechooser.h file.
 * See qgfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/07/29
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgfontchooser.h"
#include <QFontDialog>
#include "qgfont.h"
#include "qgthread.h"

std::string QGFontChooser::showDialog(const std::string& title, const std::string& initialFont) {
    return showDialog(/* parent */ nullptr, title, initialFont);
}

std::string QGFontChooser::showDialog(QWidget* parent, const std::string& title, const std::string& initialFont) {
    QFont initialQFont = QGFont::toQFont(initialFont);
    std::string result = "";
    QGThread::runOnQtGuiThread([parent, title, initialQFont, &result]() {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, initialQFont, parent, QString::fromStdString(title));
        if (ok) {
            result = QGFont::toFontString(font);
        }
    });
    return result;
}

#endif // SPL_QT_GUI
