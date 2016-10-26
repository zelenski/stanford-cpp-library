/*
 * File: gfilechooser.h
 * --------------------
 * This file defines the <code>GFileChooser</code> class which supports
 * popping up graphical dialog boxes to select file names.
 * 
 * @author Marty Stepp
 * @version 2016/10/25
 * - added file filter option
 * @version 2014/07/09
 * - initial version
 * @since 2014/07/09
 */

#ifndef _gfilechooser_h
#define _gfilechooser_h

#include <string>

class GFileChooser {
public:
    static std::string showOpenDialog(const std::string& currentDir = "", const std::string& fileFilter = "");
    static std::string showSaveDialog(const std::string& currentDir = "", const std::string& fileFilter = "");

private:
    /*
     * Constants for dialog types, taken from Java's JFileChooser
     */
    static const int OPEN_DIALOG = 0;
    static const int SAVE_DIALOG = 1;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gfilechooser_h
