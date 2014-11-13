/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 *
 * @version 2014/11/13
 * - added setConsoleWindowTitle method
 * @version 2014/11/05
 * - moved exception / stack-trace code out to exceptions.h/cpp
 * @version 2014/10/14
 * - removed some autograder code, moved into autograder/ioutils.{h,cpp}
 * - exposed 'clear enabled' functions rather than 'autograder mode'
 * 2014/10/08
 * - removed 'using namespace' statement
 * 2014/07/08
 * - modified clearConsole to print '=== cleared ===' message on normal text console
 *   for better matching against expected output log files
 */

#include "console.h"
#include <cstdio>
#include <string>
#include "error.h"
#include "exceptions.h"
#include "gwindow.h"
#include "platform.h"

static Platform* pp = getPlatform();
static bool consoleClearEnabled = false;
static bool consoleEcho = false;
static bool consoleEventOnClose = false;
static bool consoleExitProgramOnClose = false;
static bool consoleLocationSaved = false;
static bool consoleLocked = false;

void clearConsole() {
    std::string msg = "==================== (console cleared) ====================";
    if (getConsoleClearEnabled()) {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());

        // clear the graphical console window
        pp->jbeconsole_clear();
    } else {
        // don't actually clear the window, just display 'cleared' message on it
        std::cout << msg << std::endl;
    }
}

bool getConsoleClearEnabled() {
    return consoleClearEnabled;
}

bool getConsoleEcho() {
    return consoleEcho;
}

bool getConsoleEventOnClose() {
    return consoleEventOnClose;
}

bool getConsoleExitProgramOnClose() {
    return consoleExitProgramOnClose;
}

bool getConsoleLocationSaved() {
    return consoleLocationSaved;
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return consoleLocked;
}

void setConsoleClearEnabled(bool value) {
    consoleClearEnabled = value;
}

void setConsoleEcho(bool echo) {
    if (consoleLocked) { return; }
    consoleEcho = echo;
}

void setConsoleEventOnClose(bool eventOnClose) {
    if (consoleLocked) { return; }
    consoleEventOnClose = eventOnClose;
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (consoleLocked) { return; }
    consoleExitProgramOnClose = exitOnClose;
    pp->jbeconsole_setExitProgramOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (consoleLocked) { return; }
    pp->jbeconsole_setFont(font);
}

void setConsoleLocation(int x, int y) {
    if (consoleLocked) { return; }
    pp->jbeconsole_setLocation(x, y);
}

void setConsoleLocationSaved(bool value) {
    if (consoleLocked) { return; }
    consoleLocationSaved = value;
    pp->jbeconsole_setLocationSaved(value);
}

void setConsolePrintExceptions(bool printExceptions) {
    if (consoleLocked) { return; }
    exceptions::setTopLevelExceptionHandlerEnabled(printExceptions);
}

void setConsoleSettingsLocked(bool value) {
    consoleLocked = value;
}

void setConsoleSize(double width, double height) {
    if (consoleLocked) { return; }
    pp->jbeconsole_setSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (consoleLocked) { return; }
    pp->jbeconsole_setTitle(title);
}
