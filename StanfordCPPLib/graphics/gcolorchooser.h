/*
 * File: gcolorchooser.h
 * ---------------------
 * This file defines the <code>GColorChooser</code> class which supports
 * popping up graphical dialog boxes to select colors.
 * 
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gcolorchooser.h to replace Java version
 * @version 2018/07/29
 * - initial version
 */

#ifndef _gcolorchooser_h
#define _gcolorchooser_h

#include <string>
#include <QWidget>

class GColorChooser {
public:
    static std::string showDialog(const std::string& title, int initialColor);
    static std::string showDialog(QWidget* parent, const std::string& title, int initialColor);
    static std::string showDialog(const std::string& title = "", const std::string& initialColor = "");
    static std::string showDialog(QWidget* parent, const std::string& title = "", const std::string& initialColor = "");

private:
    GColorChooser();   // prevent construction
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gcolorchooser_h
