/*
 * File: qgfontchooser.h
 * ----------------------
 * This file defines the <code>QGFontChooser</code> class which supports
 * popping up graphical dialog boxes to select fonts.
 * 
 * @author Marty Stepp
 * @version 2018/07/29
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgfontchooser_h
#define _qgfontchooser_h

#include <string>
#include <QWidget>

class QGFontChooser {
public:
    static std::string showDialog(const std::string& title = "", const std::string& initialFont = "");
    static std::string showDialog(QWidget* parent, const std::string& title = "", const std::string& initialFont = "");
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgfontchooser_h
#endif // SPL_QT_GUI
