/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 *
 * @version 2015/07/05
 * - removed static global Platform variable, replaced by getPlatform as needed
 * @version 2015/04/25
 * - added methods to get/set console output color
 * @version 2015/04/15
 * - set consoleClearEnabled to true by default (why was it false?)
 * @version 2014/11/13
 * - added setConsoleWindowTitle method
 * @version 2014/11/05
 * - moved exception / stack-trace code out to exceptions.h/cpp
 * @version 2014/10/14
 * - removed some autograder code, moved into autograder/ioutils.{h,cpp}
 * - exposed 'clear enabled' functions rather than 'autograder mode'
 * @version 2014/10/08
 * - removed 'using namespace' statement
 * @version 2014/07/08
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

static bool consoleClearEnabled = true;
static bool consoleEcho = false;
static bool consoleEventOnClose = false;
static bool consoleExitProgramOnClose = false;
static bool consoleLocationSaved = false;
static bool consoleLocked = false;
static ConsoleCloseOperation consoleCloseOperation = ConsoleCloseOperation::CONSOLE_HIDE_ON_CLOSE;

void clearConsole() {
    std::string msg = "==================== (console cleared) ====================";
    if (getConsoleClearEnabled()) {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());

        // clear the graphical console window
        getPlatform()->jbeconsole_clear();
    } else {
        // don't actually clear the window, just display 'cleared' message on it
        std::cout << msg << std::endl;
    }
}

bool getConsoleClearEnabled() {
    return consoleClearEnabled;
}

ConsoleCloseOperation getConsoleCloseOperation() {
    return consoleCloseOperation;
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

void setConsoleCloseOperation(ConsoleCloseOperation op) {
    if (consoleLocked) { return; }
    consoleCloseOperation = op;
    consoleExitProgramOnClose = op == ConsoleCloseOperation::CONSOLE_EXIT_ON_CLOSE;
    getPlatform()->jbeconsole_setCloseOperation(op);
}

void setConsoleEcho(bool echo) {
    if (consoleLocked) { return; }
    consoleEcho = echo;
}

void setConsoleErrorColor(const std::string& color) {
    if (consoleLocked) { return; }
    // consoleOutputColor = color;
    getPlatform()->jbeconsole_setErrorColor(color);
}

void setConsoleEventOnClose(bool eventOnClose) {
    if (consoleLocked) { return; }
    consoleEventOnClose = eventOnClose;
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (consoleLocked) { return; }
    consoleExitProgramOnClose = exitOnClose;
    getPlatform()->jbeconsole_setExitProgramOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (consoleLocked) { return; }
    getPlatform()->jbeconsole_setFont(font);
}

void setConsoleLocation(int x, int y) {
    if (consoleLocked) { return; }
    getPlatform()->jbeconsole_setLocation(x, y);
}

void setConsoleLocationSaved(bool value) {
    if (consoleLocked) { return; }
    consoleLocationSaved = value;
    getPlatform()->jbeconsole_setLocationSaved(value);
}

void setConsoleOutputColor(const std::string& color) {
    if (consoleLocked) { return; }
    // consoleOutputColor = color;
    getPlatform()->jbeconsole_setOutputColor(color);
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
    getPlatform()->jbeconsole_setSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (consoleLocked) { return; }
    getPlatform()->jbeconsole_setTitle(title);
}
