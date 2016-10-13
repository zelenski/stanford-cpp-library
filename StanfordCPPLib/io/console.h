/*
 * File: console.h
 * ---------------
 * This file contains functions related to the library's graphical console window.
 * In general if you #include this file, it will implicitly enable the graphical
 * console.  If you don't want to do that, you should #define a flag named
 * __DONT_ENABLE_GRAPHICAL_CONSOLE right before #include'ing this header.
 * Once the graphical console has been enabled, it cannot easily be turned off
 * again for that program.
 * 
 * @version 2016/09/22
 * - added __DONT_ENABLE_GRAPHICAL_CONSOLE and optional graphical console
 * @version 2016/08/12
 * - added extern declaration of pause() function for pausing console programs
 * @version 2015/06/20
 * - added recursionIndent() function for pretty-printing indented recursive calls
 * @version 2015/04/25
 * - added methods to get/set console output color
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

enum ConsoleCloseOperation {
    CONSOLE_DO_NOTHING_ON_CLOSE = 0,
    CONSOLE_HIDE_ON_CLOSE = 1,
    CONSOLE_DISPOSE_ON_CLOSE = 2,
    CONSOLE_EXIT_ON_CLOSE = 3
};

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
 * Function: setConsoleCloseOperation();
 * Usage: setConsoleCloseOperation(op);
 * ------------------------------------
 * Returns what the console will do when the user hits its "close" button.
 * By default, this is CONSOLE_HIDE_ON_CLOSE unless set otherwise.
 */
ConsoleCloseOperation getConsoleCloseOperation();

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
 * Function: setConsoleCloseOperation();
 * Usage: setConsoleCloseOperation(op);
 * ------------------------------------
 * Sets what the console should do when the user hits its "close" button.
 */
void setConsoleCloseOperation(ConsoleCloseOperation op);

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
 * Sets the color used to display text in the console window.
 * The color string passed should be in a hex format such as "#ffa32f";
 */
void setConsoleErrorColor(const std::string& color);

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
 * Sets the color used to display text in the console window.
 * The color string passed should be in a hex format such as "#ffa32f";
 */
void setConsoleOutputColor(const std::string& color);

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

// defined in gwindow.h/cpp
extern void pause(double milliseconds);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _console_h

/*
 * console.h is weird in that a student's program must be able to #include it
 * and then magically receive the graphical console instead of the standard one;
 * but we want other lib files to be able to include console.h to get the
 * function prototypes without actually turning the graphical console on.
 * To achieve this, we have the __DONT_ENABLE_GRAPHICAL_CONSOLE flag that lib
 * files can set right before #include'ing console.h.  If they do so, it will
 * declare the prototypes but not initialize the graphical console.
 */
#ifndef __DONT_ENABLE_GRAPHICAL_CONSOLE

namespace stanfordcpplib {
extern void initializeGraphicalConsole();

class __ConsoleInitializer {
public:
    /*
     * Code to initialize the library.
     * Implemented as a class constructor so that it will run before the
     * student's main function.
     */
    __ConsoleInitializer() {
        initializeGraphicalConsole();
    }
};
static __ConsoleInitializer __console_init;
}

#endif // __DONT_ENABLE_GRAPHICAL_CONSOLE
