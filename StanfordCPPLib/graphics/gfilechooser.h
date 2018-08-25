/*
 * File: gfilechooser.h
 * --------------------
 * This file defines the <code>GFileChooser</code> class which supports
 * popping up graphical dialog boxes to select file names.
 * 
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gfilechooser.h to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#ifndef _gfilechooser_h
#define _gfilechooser_h

#include <string>
#include <QWidget>

class GFileChooser {
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

#endif // _gfilechooser_h
