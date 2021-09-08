/*
 * File: ioutils.h
 * ---------------
 * This file contains declarations of functions to help capture, redirect,
 * and feed input to cin/cout/err.
 * 
 * @author Marty Stepp
 * @version 2014/10/14
 * @since 2014/03/01
 */

#ifndef _ioutils_h
#define _ioutils_h

#include <string>

namespace ioutils {
void captureStderrBegin();
std::string captureStderrEnd();
void captureStdoutBegin(bool alsoStderr = false);
std::string captureStdoutEnd();

/*
 * Function: getConsoleEchoUserInput
 * Usage: bool echo = getConsoleEchoUserInput();
 * ---------------------------------------------
 * Returns whether or not the input from the Stanford graphical
 * console window is being echoed onto the standard operating system terminal
 * window. Initially this is false unless set to true by a previous call to
 * setConsoleEchoUserInput(true).
 */
bool getConsoleEchoUserInput();
int getConsoleOutputLimit();

void redirectStdinBegin(std::string userInput = "");
void redirectStdinFeedInput(std::string userInput);
void redirectStdinEnd();

/*
 * Function: setConsoleEchoUserInput
 * Usage: setConsoleEchoUserInput(true);
 * -------------------------------------
 * Enables or disables echoing the input from the Stanford
 * console window onto the standard operating system terminal window.
 * Normally you don't need this echoing, but if you want to be able to copy
 * and paste your console interaction into another window, it is useful.
 */
void setConsoleEchoUserInput(bool echo);

/*
 * Sets console to throw an error if more than 'limit' total chars are printed.
 * Used to stop students who print infinite or semi-infinite output.
 * If limit <= 0, no limit.
 */
void setConsoleOutputLimit(int limit = 0);
} // namespace ioutils

#endif // _ioutils_h
