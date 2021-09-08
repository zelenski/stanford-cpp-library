/*
 * File: gfontchooser.h
 * ----------------------
 * This file defines the <code>GFontChooser</code> class which supports
 * popping up graphical dialog boxes to select fonts.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gfontchooser.h to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gfontchooser_h
#define _gfontchooser_h

#include <string>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "gwindow.h"
#undef INTERNAL_INCLUDE

/**
 * The GFontChooser class contains static methods for popping up font-choosing
 * dialog boxes that allow the user to select a font family, size, and style.
 * Each method blocks until the font is chosen and the dialog is closed,
 * returning the font the user chose (or an empty string if the user canceled).
 *
 * Fonts are represented as font strings such as "Helvetica-12-Bold" or
 * "Times New Roman-14".
 *
 * See the GFont class for more information about fonts and font strings.
 */
class GFontChooser {
public:
    /**
     * Pops up a font chooser dialog with the given top title text
     * and the given initially selected font.  All arguments are optional.
     * @param initialFont a font string such as "Helvetica-12-Bold".
     */
    static std::string showDialog(const std::string& title = "", const std::string& initialFont = "");

    /**
     * Pops up a font chooser dialog with the given top title text
     * and the given initially selected font.  All arguments are optional.
     * @param initialFont a font string such as "Helvetica-12-Bold".
     */
    static std::string showDialog(GWindow* parent, const std::string& title = "", const std::string& initialFont = "");

    /**
     * Pops up a font chooser dialog with the given top title text
     * and the given initially selected font.  All arguments are optional.
     * @param initialFont a font string such as "Helvetica-12-Bold".
     */
    static std::string showDialog(QWidget* parent, const std::string& title = "", const std::string& initialFont = "");

private:
    GFontChooser();   // prevent construction
};

#endif // _gfontchooser_h
