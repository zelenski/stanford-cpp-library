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
 * Function: getConsolePrintExceptions
 * Usage: bool ex = getConsolePrintExceptions();
 * ----------------------------
 * Returns whether or not a feature is enabled that causes exceptions to be
 * echoed to the Stanford graphical console window when they are thrown.
 * Disabled (false) by default.
 */
bool getConsolePrintExceptions();

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
 * Sets whether the location of the console window should be remembered and
 * restored between runs of the program.  Default false.
 */
void setConsoleLocationSaved(bool value);

/*
 * Returns whether the location of the console window should be remembered and
 * restored between runs of the program.  Initially false.
 */
bool getConsoleLocationSaved();

/*
 * Returns whether the overall C++ program will terminate if the console
 * window is closed.  By default this is false initially.
 */
bool getConsoleExitProgramOnClose();

/*
 * Returns whether an event should be generated if the console
 * window is closed.  By default this is false initially.
 */
bool getConsoleEventOnClose();

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
 * Function: setConsoleSize
 * Usage: setConsoleSize(width, height);
 * -------------------------------------
 * Changes the size of the console to the specified dimensions, measured
 * in pixels.
 */
void setConsoleSize(double width, double height);

/*
 * Function: setConsoleLocation
 * Usage: setConsoleLocation(x, y);
 * -------------------------------------
 * Changes the (x, y) pixel position of the console to the specified
 * coordinates, relative to the top/left of the screen.
 */
void setConsoleLocation(int x, int y);


// extra functions to facilitate creation of autograder programs
namespace autograder {
/*
 * Function: getConsoleEchoUserInput
 * Usage: bool echo = getConsoleEchoUserInput();
 * ----------------------------
 * Returns whether or not the input from the Stanford graphical
 * console window is being echoed onto the standard operating system terminal
 * window. Initially this is false unless set to true by a previous call to
 * setConsoleEchoUserInput(true).
 */
bool getConsoleEchoUserInput();

/*
 * Function: getConsoleLogFile
 * Usage: string consoleLogFile = getConsoleLogFile();
 * ----------------------------
 * Returns the file name, if any, that was set by a previous call to
 * setConsoleLogFile into which console in/output is being logged.
 * If setConsoleLogFile has not been called yet by this program, returns
 * an empty string ("").
 */
std::string getConsoleLogFile();

/*
 * Function: isConsoleAutograderMode
 * Usage: bool mode = isConsoleAutograderMode();
 * -------------------------------------
 * Returns whether the console is in "autograder mode".
 * Initially the console is not in autograder mode unless the function
 * setConsoleAutograderMode has been called.
 */
bool isConsoleAutograderMode();

/*
 * Function: setConsoleAutograderMode
 * Usage: setConsoleAutograderMode(true);
 * -------------------------------------
 * Puts the console in/out of "autograder mode", a mode where various changes
 * occur such as not actually clearing the console on a clearConsole() call.
 * This mode is used to facilitate creation of auto-grading scripts.
 */
void setConsoleAutograderMode(bool value);

/*
 * Function: setConsoleEchoUserInput
 * Usage: setConsoleEchoUserInput(true);
 * ----------------------------
 * Enables or disables echoing the input from the Stanford
 * console window onto the standard operating system terminal window.
 * Normally you don't need this echoing, but if you want to be able to copy
 * and paste your console interaction into another window, it is useful.
 */
void setConsoleEchoUserInput(bool echo);

/*
 * Function: setConsoleLog
 * Usage: setConsoleLog("myoutput.txt");
 * ----------------------------
 * Begins dumping a copy of all future console in/output to the given file name.
 * Useful for capturing output logs and writing auto-grader scripts.
 * If you are logging to a file, the output still also appears on the console.
 * By default, logging is not initially enabled.
 * Log text is appended to any existing content in the file as it is printed.
 * If you pass the name of an invalid file, or one that the current user does
 * not have permission to write, a file I/O error will occur the next time
 * your program performs a console I/O operation to cout or cin.
 * Set to an empty string ("") to disable logging.
 */
void setConsoleLogFile(const std::string & filename);

/*
 * If set to true, disables functions like setConsoleFont or setConsoleSize, to
 * facilitate autograder construction by negating any such calls in the
 * student's main program.  Initially false.
 */
void setConsoleSettingsLocked(bool value);

/*
 * Returns whether functions like setConsoleFont or setConsoleSize are currently
 * ignored.  See setConsoleSettingsLocked.
 */
bool isConsoleSettingsLocked();

/*
 * Adds/removes an input window button for auto-inserting autograder inputs.
 */
void addInputButton(const std::string& text, const std::string& input);
void removeInputButton(const std::string& text);

/*
 * Adds/removes an input category containing related autograder input buttons.
 */
void addInputCategory(const std::string& name);
void removeInputCategory(const std::string& name);
}

#include "private/main.h"

#endif
