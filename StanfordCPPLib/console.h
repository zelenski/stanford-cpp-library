/*
 * File: console.h
 * ---------------
 * This file redirects the <code>cin</code>, <code>cout</code>,
 * and <code>cerr</code> channels to use a console window.  This file
 * must be included in the source file that contains the <code>main</code>
 * method, although it may be included in other source files as well.
 */

#ifndef _console_h
#define _console_h

#include <string>

/*
 * Function: clearConsole
 * Usage: clearConsole();
 * ----------------------
 * Erases the contents of the console window.
 */

void clearConsole();

/*
 * Function: setConsoleFont
 * Usage: setConsoleFont(font);
 * ----------------------------
 * Changes the font used for the console.  The <code>font</code> parameter
 * is typically a string in the form <code>family-style-size</code>.
 * In this string, <code>family</code> is the name of the font family;
 * <code>style</code> is either missing (indicating a plain font) or one
 * of the strings <code>Bold</code>, <code>Italic</code>, or
 * <code>BoldItalic</code>; and <code>size</code> is an integer
 * indicating the point size.  If any of these components is
 * specified as an asterisk, the existing value is retained.
 * The <code>font</code> parameter can also be a sequence of
 * such specifications separated by semicolons, in which case the
 * first available font on the system is used.
 */

void setConsoleFont(const std::string & font);

/*
 * Function: setConsoleSize
 * Usage: setConsoleSize(width, height);
 * -------------------------------------
 * Changes the size of the console to the specified dimensions, measured
 * in pixels.
 */

void setConsoleSize(double width, double height);

#include "private/main.h"

#endif
