/*
 * File: gfilechooser.h
 * --------------------
 * This file defines the <code>GFileChooser</code> class which supports
 * popping up graphical dialog boxes to select file names.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gfilechooser.h to replace Java version
 * @version 2018/06/28
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gfilechooser_h
#define _gfilechooser_h

#include <string>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#undef INTERNAL_INCLUDE

/**
 * The GFileChooser class contains static methods for popping up file-choosing
 * dialog boxes that allow the user to select a file.
 * Each method blocks until the file is chosen and the dialog is closed,
 * returning the file the user chose (or an empty string if the user canceled).
 */
class GFileChooser {
public:
    /**
     * Pops up a file "Open" chooser dialog with the given top title text,
     * current directory, and file filter.  All arguments are optional.
     * @param fileFilter a file filter string such as "*.gif,*.jpg,*.png".
     */
    static std::string showOpenDialog(const std::string& title = "Open file", const std::string& currentDir = "", const std::string& fileFilter = "");

    /**
     * Pops up a file "Open" chooser dialog with the given top title text,
     * current directory, and file filter.  All arguments are optional.
     * @param fileFilter a file filter string such as "*.gif,*.jpg,*.png".
     */
    static std::string showOpenDialog(GWindow* parent, const std::string& title = "Open file", const std::string& currentDir = "", const std::string& fileFilter = "");

    /**
     * Pops up a file "Open" chooser dialog with the given top title text,
     * current directory, and file filter.  All arguments are optional.
     * @param fileFilter a file filter string such as "*.gif,*.jpg,*.png".
     */
    static std::string showOpenDialog(QWidget* parent, const std::string& title = "Open file", const std::string& currentDir = "", const std::string& fileFilter = "");

    /**
     * Pops up a file "Save" chooser dialog with the given top title text,
     * current directory, and file filter.  All arguments are optional.
     *
     * The main difference between an "Open" dialog and a "Save" dialog,
     * aside from the different title bar text, is that a "Save" dialog allows
     * the user to type a name of a file that does not yet exist.
     *
     * If the user chooses the name of a file that already exists, they will
     * be prompted to overwrite this file.  If they choose No, the dialog will
     * remain up; if they choose Yes, it will close.
     * @param fileFilter a file filter string such as "*.gif,*.jpg,*.png".
     */
    static std::string showSaveDialog(const std::string& title = "Save file", const std::string& currentDir = "", const std::string& fileFilter = "");

    /**
     * Pops up a file "Save" chooser dialog with the given top title text,
     * current directory, and file filter.  All arguments are optional.
     *
     * The main difference between an "Open" dialog and a "Save" dialog,
     * aside from the different title bar text, is that a "Save" dialog allows
     * the user to type a name of a file that does not yet exist.
     *
     * If the user chooses the name of a file that already exists, they will
     * be prompted to overwrite this file.  If they choose No, the dialog will
     * remain up; if they choose Yes, it will close.
     * @param fileFilter a file filter string such as "*.gif,*.jpg,*.png".
     */
    static std::string showSaveDialog(GWindow* parent, const std::string& title = "Save file", const std::string& currentDir = "", const std::string& fileFilter = "");

    /**
     * Pops up a file "Save" chooser dialog with the given top title text,
     * current directory, and file filter.  All arguments are optional.
     *
     * The main difference between an "Open" dialog and a "Save" dialog,
     * aside from the different title bar text, is that a "Save" dialog allows
     * the user to type a name of a file that does not yet exist.
     *
     * If the user chooses the name of a file that already exists, they will
     * be prompted to overwrite this file.  If they choose No, the dialog will
     * remain up; if they choose Yes, it will close.
     * @param fileFilter a file filter string such as "*.gif,*.jpg,*.png".
     */
    static std::string showSaveDialog(QWidget* parent, const std::string& title = "Save file", const std::string& currentDir = "", const std::string& fileFilter = "");

private:
    GFileChooser();   // prevent construction

    /*
     * Constants for dialog types, similar to those in Java's JFileChooser
     */
    enum DialogType {
        OPEN_DIALOG = 0,
        SAVE_DIALOG = 1
    };

    /*
     * Converts between our comma-separated file filter format to the one
     * used by Qt that uses ;; as its separator.
     */
    static std::string normalizeFileFilter(const std::string& fileFilter);
};

#endif // _gfilechooser_h
