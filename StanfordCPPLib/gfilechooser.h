/*
 * File: gfilechooser.h
 * --------------------
 * This file defines the <code>GFileChooser</code> class which supports
 * popping up graphical dialog boxes to select file names.
 * 
 * @author Marty Stepp
 * @version 2014/07/09
 * @since 2014/07/09
 */

#ifndef _gfilechooser_h
#define _gfilechooser_h

#include <string>

class GFileChooser {
public:
    static std::string showOpenDialog(std::string currentDir = "");
    static std::string showSaveDialog(std::string currentDir = "");

private:
    /*
     * Constants for dialog types, taken from Java's JFileChooser
     */
    static const int SAVE_DIALOG = 1;
    static const int OPEN_DIALOG = 1;
};

#endif
