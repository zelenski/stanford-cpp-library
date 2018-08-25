/*
 * File: gcolorchooser.cpp
 * -----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gcolorchooser.cpp to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#include "gcolorchooser.h"
#include <QColorDialog>
#include "gcolor.h"
#include "gthread.h"

std::string GColorChooser::showDialog(const std::string& title, int initialColor) {
    return showDialog(/* parent */ nullptr, title, initialColor);
}

std::string GColorChooser::showDialog(QWidget* parent, const std::string& title, int initialColor) {
    return showDialog(parent, title, GColor::convertRGBToColor(initialColor));
}

std::string GColorChooser::showDialog(const std::string& title, const std::string& initialColor) {
    return showDialog(/* parent */ nullptr, title, initialColor);
}

std::string GColorChooser::showDialog(QWidget* parent, const std::string& title, const std::string& initialColor) {
    QColor initialQColor = initialColor.empty() ? Qt::white : GColor::toQColor(initialColor);
    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, initialQColor, &result]() {
        QColor selectedColor = QColorDialog::getColor(initialQColor, parent, QString::fromStdString(title));
        if (selectedColor.isValid()) {
            result = GColor::convertQColorToColor(selectedColor);
        }
    });
    return result;
}
