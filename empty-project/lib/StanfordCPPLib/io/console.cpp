/*
 * File: console.cpp
 * -----------------
 * This file implements the console.h interface.
 *
 * @version 2016/10/04
 * - removed all static variables (replaced with STATIC_VARIABLE macros)
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

#define __DONT_ENABLE_GRAPHICAL_CONSOLE
#include "console.h"
#undef __DONT_ENABLE_GRAPHICAL_CONSOLE

#include <cstdio>
#include <iostream>
#include <string>
#include "error.h"
#include "exceptions.h"
#include "gwindow.h"
#include "private/platform.h"
#include "private/static.h"

// static 'variables' (as functions to avoid initialization ordering bugs)
STATIC_VARIABLE_DECLARE(bool, consoleClearEnabled, true)
STATIC_VARIABLE_DECLARE(bool, consoleEcho, false)
STATIC_VARIABLE_DECLARE(bool, consoleEventOnClose, false)
STATIC_VARIABLE_DECLARE(bool, consoleExitProgramOnClose, false)
STATIC_VARIABLE_DECLARE(bool, consoleLocationSaved, false)
STATIC_VARIABLE_DECLARE(bool, consoleLocked, false)
STATIC_VARIABLE_DECLARE(ConsoleCloseOperation, consoleCloseOperation, ConsoleCloseOperation::CONSOLE_HIDE_ON_CLOSE)
// end static 'variables'

void clearConsole() {
    std::string msg = "==================== (console cleared) ====================";
    if (getConsoleClearEnabled()) {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());

        // clear the graphical console window
        stanfordcpplib::getPlatform()->jbeconsole_clear();
    } else {
        // don't actually clear the window, just display 'cleared' message on it
        std::cout << msg << std::endl;
    }
}

bool getConsoleClearEnabled() {
    return STATIC_VARIABLE(consoleClearEnabled);
}

ConsoleCloseOperation getConsoleCloseOperation() {
    return STATIC_VARIABLE(consoleCloseOperation);
}

bool getConsoleEcho() {
    return STATIC_VARIABLE(consoleEcho);
}

bool getConsoleEventOnClose() {
    return STATIC_VARIABLE(consoleEventOnClose);
}

bool getConsoleExitProgramOnClose() {
    return STATIC_VARIABLE(consoleExitProgramOnClose);
}

bool getConsoleLocationSaved() {
    return STATIC_VARIABLE(consoleLocationSaved);
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return STATIC_VARIABLE(consoleLocked);
}

void setConsoleClearEnabled(bool value) {
    STATIC_VARIABLE(consoleClearEnabled) = value;
}

void setConsoleCloseOperation(ConsoleCloseOperation op) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleCloseOperation) = op;
    STATIC_VARIABLE(consoleExitProgramOnClose) = op == ConsoleCloseOperation::CONSOLE_EXIT_ON_CLOSE;
    stanfordcpplib::getPlatform()->jbeconsole_setCloseOperation(op);
}

void setConsoleEcho(bool echo) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleEcho) = echo;
}

void setConsoleErrorColor(const std::string& color) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    stanfordcpplib::getPlatform()->jbeconsole_setErrorColor(color);
}

void setConsoleEventOnClose(bool eventOnClose) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleEventOnClose) = eventOnClose;
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleExitProgramOnClose) = exitOnClose;
    stanfordcpplib::getPlatform()->jbeconsole_setExitProgramOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    stanfordcpplib::getPlatform()->jbeconsole_setFont(font);
}

void setConsoleLocation(int x, int y) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    stanfordcpplib::getPlatform()->jbeconsole_setLocation(x, y);
}

void setConsoleLocationSaved(bool value) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleLocationSaved) = value;
    stanfordcpplib::getPlatform()->jbeconsole_setLocationSaved(value);
}

void setConsoleOutputColor(const std::string& color) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    stanfordcpplib::getPlatform()->jbeconsole_setOutputColor(color);
}

void setConsolePrintExceptions(bool printExceptions) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    exceptions::setTopLevelExceptionHandlerEnabled(printExceptions);
}

void setConsoleSettingsLocked(bool value) {
    STATIC_VARIABLE(consoleLocked) = value;
}

void setConsoleSize(double width, double height) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    stanfordcpplib::getPlatform()->jbeconsole_setSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    stanfordcpplib::getPlatform()->jbeconsole_setTitle(title);
}
