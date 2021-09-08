/*
 * File: gfilechooser.cpp
 * ----------------------
 * This file implements the operations declared in the gfilechooser.h file.
 * See gfilechooser.h for documentation of each member.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gfilechooser.cpp to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#include "gfilechooser.h"
#include <QFileDialog>
#include "gthread.h"
#include "strlib.h"
#include "vector.h"

GFileChooser::GFileChooser() {
    // empty
}

std::string GFileChooser::showOpenDialog(const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showOpenDialog(/* parent */ (QWidget*) nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showOpenDialog(GWindow* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showOpenDialog(parent ? parent->getWidget() : nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showOpenDialog(QWidget* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    // Qt filter spec:
    // If you want multiple filters, separate them with ';;', for example:
    // "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"

    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, currentDir, fileFilter, &result]() {
        result = QFileDialog::getOpenFileName(parent,
                QString::fromStdString(title),
                QString::fromStdString(currentDir),
                QString::fromStdString(normalizeFileFilter(fileFilter))).toStdString();
    });
    return result;
}

std::string GFileChooser::showSaveDialog(const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showSaveDialog(/* parent */ (QWidget*) nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showSaveDialog(GWindow* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    return showSaveDialog(parent ? parent->getWidget() : nullptr, title, currentDir, fileFilter);
}

std::string GFileChooser::showSaveDialog(QWidget* parent, const std::string& title, const std::string& currentDir, const std::string& fileFilter) {
    std::string result = "";
    GThread::runOnQtGuiThread([parent, title, currentDir, fileFilter, &result]() {
        result = QFileDialog::getSaveFileName(parent,
                QString::fromStdString(title),
                QString::fromStdString(currentDir),
                QString::fromStdString(normalizeFileFilter(fileFilter))).toStdString();
    });
    return result;
}

std::string GFileChooser::normalizeFileFilter(const std::string& fileFilter) {
    Vector<std::string> tokens = stringSplit(fileFilter, ",");
    for (int i = 0; i < tokens.size(); i++) {
        std::string token = tokens[i];
        token = trim(token);
        // TODO: more processing
        tokens[i] = token;
    }
    return stringJoin(tokens, ";;");
}
