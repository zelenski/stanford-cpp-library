/*
 * File: gfontchooser.h
 * ----------------------
 * This file defines the <code>GFontChooser</code> class which supports
 * popping up graphical dialog boxes to select fonts.
 * 
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gfontchooser.h to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#ifndef _gfontchooser_h
#define _gfontchooser_h

#include <string>
#include <QWidget>

class GFontChooser {
public:
    static std::string showDialog(const std::string& title = "", const std::string& initialFont = "");
    static std::string showDialog(QWidget* parent, const std::string& title = "", const std::string& initialFont = "");
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gfontchooser_h
