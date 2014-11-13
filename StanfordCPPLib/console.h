/*
 * File: console.h
 * ---------------
 * This file redirects the <code>cin</code>, <code>cout</code>,
 * and <code>cerr</code> channels to use a console window.  This file
 * must be included in the source file that contains the <code>main</code>
 * method, although it may be included in other source files as well.
 * 
 * @version 2014/11/13
 * - added setConsoleWindowTitle method
 * @version 2014/11/05
 * - moved exception / stack-trace code out to exceptions.h/cpp
 * @version 2014/10/14
 * - removed some autograder code, moved into autograder/ioutils.{h,cpp}
 * - exposed 'clear enabled' functions rather than 'autograder mode'
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
 * Function: getConsoleClearEnabled
 * Usage: bool mode = getConsoleClearEnabled();
 * --------------------------------------------
 * Returns whether the console's "clear" function should be enabled.
 * Default true.  If false, clear() will just print a message
 * rather than actually clearing the console.
 */
bool getConsoleClearEnabled();

/*
 * Function: getConsoleEcho
 * Usage: bool echo = getConsoleEcho();
 * ----------------------------
 * Returns whether or not the input/output from the Stanford graphical
 * console window is being echoed onto the standard operating system terminal
 * window. Initially this is false unless set to true by a previous call to
 * setConsoleEcho(true).
 */
bool getConsoleEcho();

/*
 * Returns whether an event should be generated if the console
 * window is closed.  By default this is false initially.
 */
bool getConsoleEventOnClose();

/*
 * Returns whether the overall C++ program will terminate if the console
 * window is closed.  By default this is false initially.
 */
bool getConsoleExitProgramOnClose();

/*
 * Returns whether the location of the console window should be remembered and
 * restored between runs of the program.  Initially false.
 */
bool getConsoleLocationSaved();

/*
 * Function: getConsolePrintExceptions
 * Usage: bool ex = getConsolePrintExceptions();
 * ----------------------------
 * Returns whether or not a feature is enabled that causes exceptions to be
 * echoed to the Stanford graphical console window when they are thrown.
 * Disabled (false) by default.
 */
bool getConsolePrintExceptions();

/*
 * Returns whether functions like setConsoleFont or setConsoleSize
 * are currently ignored.  See setConsoleSettingsLocked.
 */
bool getConsoleSettingsLocked();

/*
 * Function: setConsoleClearEnabled
 * Usage: setConsoleClearEnabled(true);
 * ------------------------------------
 * Returns whether the console's "clear" function should be enabled.
 * Default true.  If false, clear() will just print a message
 * rather than actually clearing the console.
 * This is sometimes used to facilitate creation of auto-grading scripts.
 */
void setConsoleClearEnabled(bool value);

/*
 * Function: setConsoleEcho
 * Usage: setConsoleEcho(true);
 * ----------------------------
 * Enables or disables echoing the input/output from the Stanford graphical
 * console window onto the standard operating system terminal window.
 * Normally you don't need this echoing, but if you want to be able to copy
 * and paste your console interaction into another window, it is useful.
 */
void setConsoleEcho(bool echo);   // added by Marty

/*
 * Sets whether an event should be generated if the console
 * window is closed.  By default this is false initially.
 */
void setConsoleEventOnClose(bool eventOnClose);

/*
 * Sets whether the overall C++ program should terminate if the console
 * window is closed.  By default this is false initially.
 */
void setConsoleExitProgramOnClose(bool exitOnClose);

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
void setConsoleFont(const std::string& font);

/*
 * Function: setConsoleLocation
 * Usage: setConsoleLocation(x, y);
 * --------------------------------
 * Changes the (x, y) pixel position of the console to the specified
 * coordinates, relative to the top/left of the screen.
 */
void setConsoleLocation(int x, int y);

/*
 * Sets whether the location of the console window should be remembered and
 * restored between runs of the program.  Default false.
 */
void setConsoleLocationSaved(bool value);

/*
 * Function: setConsolePrintExceptions
 * Usage: setConsolePrintExceptions(true);
 * ----------------------------
 * Enables or disables a feature that causes exceptions to be echoed to the
 * Stanford graphical console window when they are thrown.
 * Disabled (false) by default.
 * Note that using this feature may make it harder to get a stack trace in the
 * debugger if you are debugging the cause of an exception.
 */
void setConsolePrintExceptions(bool printExceptions);

/*
 * If set to true, disables functions like setConsoleFont or setConsoleSize,
 * to facilitate autograder construction by negating any such calls in the
 * student's main program.  Initially false.
 */
void setConsoleSettingsLocked(bool value);

/*
 * Function: setConsoleSize
 * Usage: setConsoleSize(width, height);
 * -------------------------------------
 * Changes the size of the console to the specified dimensions, measured
 * in pixels.
 */
void setConsoleSize(double width, double height);

/*
 * Function: setConsoleWindowTitle
 * Usage: setConsoleWindowTitle(title);
 * ------------------------------------
 * Changes the title bar text of the console window to the specified text.
 */
void setConsoleWindowTitle(const std::string& title);

#include "private/main.h"

#endif
