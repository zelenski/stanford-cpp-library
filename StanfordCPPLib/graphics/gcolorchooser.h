/*
 * File: gcolorchooser.h
 * ---------------------
 * This file defines the <code>GColorChooser</code> class which supports
 * popping up graphical dialog boxes to select colors.
 * 
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * - added overloads that accept GWindow* parent
 * @version 2018/08/23
 * - renamed to gcolorchooser.h to replace Java version
 * @version 2018/07/29
 * - initial version
 */


#ifndef _gcolorchooser_h
#define _gcolorchooser_h

#include <string>
#include <QWidget>

#include "gwindow.h"

/**
 * The GColorChooser class contains static methods for popping up color-choosing
 * dialog boxes that allow the user to select a color.
 * Each method blocks until the color is chosen and the dialog is closed,
 * returning the color the user chose (or an empty string if the user canceled).
 */
class GColorChooser {
public:
    /**
     * Pops up a color chooser dialog with the given top title text,
     * with the given initial color selected.
     * Returns the color that was chosen as a color string such as "#ff0077" or "blue".
     */
    static std::string showDialog(const std::string& title, int initialColor);

    /**
     * Pops up a color chooser dialog with the given top title text,
     * with the given initial color selected.
     * Returns the color that was chosen as a color string such as "#ff0077" or "blue".
     */
    static std::string showDialog(GWindow* parent, const std::string& title, int initialColor);

    /**
     * Pops up a color chooser dialog with the given top title text,
     * with the given initial color selected.
     * Returns the color that was chosen as a color string such as "#ff0077" or "blue".
     */
    static std::string showDialog(QWidget* parent, const std::string& title, int initialColor);

    /**
     * Pops up a color chooser dialog with the given top title text,
     * with the given initial color selected.
     * Returns the color that was chosen as a color string such as "#ff0077" or "blue".
     */
    static std::string showDialog(const std::string& title = "", const std::string& initialColor = "");

    /**
     * Pops up a color chooser dialog with the given top title text,
     * with the given initial color selected.
     * Returns the color that was chosen as a color string such as "#ff0077" or "blue".
     */
    static std::string showDialog(GWindow* parent, const std::string& title = "", const std::string& initialColor = "");

    /**
     * Pops up a color chooser dialog with the given top title text,
     * with the given initial color selected.
     * Returns the color that was chosen as a color string such as "#ff0077" or "blue".
     */
    static std::string showDialog(QWidget* parent, const std::string& title = "", const std::string& initialColor = "");

private:
    GColorChooser();   // prevent construction
};

#endif // _gcolorchooser_h
