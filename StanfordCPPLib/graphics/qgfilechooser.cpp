/*
 * File: qgfilechooser.cpp
 * -----------------------
 * This file implements the operations declared in the qgfilechooser.h file.
 * See qgfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/06/28
 * - initial version
 */

#include "qgfilechooser.h"
#include <QFileDialog>
#include "strlib.h"
#include "vector.h"

std::string QGFileChooser::showOpenDialog(const std::string& currentDir, const std::string& fileFilter) {
    // Qt filter spec:
    // If you want multiple filters, separate them with ';;', for example:
    // "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"

    return QFileDialog::getOpenFileName(/* parent */ nullptr,
                                 /* title */ "Select a file to open",
                                 QString::fromStdString(currentDir),
                                 QString::fromStdString(normalizeFileFilter(fileFilter))).toStdString();
}

std::string QGFileChooser::showSaveDialog(const std::string& currentDir, const std::string& fileFilter) {
    return QFileDialog::getSaveFileName(/* parent */ nullptr,
                                 /* title */ "Select a file to save",
                                 QString::fromStdString(currentDir),
                                 QString::fromStdString(normalizeFileFilter(fileFilter))).toStdString();
}

std::string QGFileChooser::normalizeFileFilter(const std::string& fileFilter) {
    Vector<std::string> tokens = stringSplit(fileFilter, ",");
    for (int i = 0; i < tokens.size(); i++) {
        std::string token = tokens[i];
        token = trim(token);
        // TODO: more processing
        tokens[i] = token;
    }
    return stringJoin(tokens.toStlVector(), ";;");
}
