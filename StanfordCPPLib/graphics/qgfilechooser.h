/*
 * File: qgfilechooser.h
 * ---------------------
 * This file defines the <code>QGFileChooser</code> class which supports
 * popping up graphical dialog boxes to select file names.
 * 
 * @author Marty Stepp
 * @version 2018/06/28
 * - initial version
 */

#ifndef _qgfilechooser_h
#define _qgfilechooser_h

#include <string>
#include <QWidget>

class QGFileChooser {
public:
    static std::string showOpenDialog(const std::string& title = "Open file", const std::string& currentDir = "", const std::string& fileFilter = "");
    static std::string showOpenDialog(QWidget* parent, const std::string& title = "Open file", const std::string& currentDir = "", const std::string& fileFilter = "");
    static std::string showSaveDialog(const std::string& title = "Save file", const std::string& currentDir = "", const std::string& fileFilter = "");
    static std::string showSaveDialog(QWidget* parent, const std::string& title = "Open file", const std::string& currentDir = "", const std::string& fileFilter = "");

private:
    /*
     * Constants for dialog types, similar to those in Java's JFileChooser
     */
    enum DialogType {
        OPEN_DIALOG = 0,
        SAVE_DIALOG = 1
    };

    static std::string normalizeFileFilter(const std::string& fileFilter);
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgfilechooser_h
