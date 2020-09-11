/*
 * File: console.cpp
 * -----------------
 * This file implements the console .h interface.
 *
 * @author Marty Stepp
 * @version 2019/04/12
 * - added pause() implementation (empty) in headless mode
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/10/01
 * - bug fix for graphical console popping up even if not included
 * @version 2018/08/23
 * - renamed to console .cpp/h to replace Java version
 * - separated out gconsolewindow.h/cpp
 * @version 2018/07/15
 * - initial version, based on io/console .cpp
 */

#include "consoletext.h"
#include <cstdio>
#include "error.h"
#include "exceptions.h"
#include "filelib.h"
#include "gconsolewindow.h"
#include "gthread.h"
#include "private/static.h"

// Default can be overridden with explicit call to setConsoleBlah
static const int DEFAULT_X = -1, DEFAULT_Y = -1; // will center
static const int DEFAULT_WIDTH = 900, DEFAULT_HEIGHT = 500;
static const bool DEFAULT_ECHO = true, DEFAULT_EXIT_ON_CLOSE = true;

#ifdef SPL_HEADLESS_MODE

void clearConsole() {
    // empty
}

bool getConsoleClearEnabled() {
    return true;
}

/* GWindow::CloseOperation */ int getConsoleCloseOperation() {
    return 0;
}

bool getConsoleEcho() {
    return true;
}

bool getConsoleEnabled() {
    return true;
}

bool getConsoleEventOnClose() {
    return true;
}

bool getConsoleExitProgramOnClose() {
    return true;
}

std::string getConsoleFont() {
    return "";
}

double getConsoleHeight() {
    return 0;
}

bool getConsoleLocationSaved() {
    return false;
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return false;
}

double getConsoleWidth() {
    return 0;
}

std::string getConsoleWindowTitle() {
    return "";
}

void pause(double /*milliseconds*/) {
    // empty
}

void setConsoleClearEnabled(bool /*value*/) {
    // empty
}

void setConsoleCloseOperation(int /*op*/) {
    // empty
}

void setConsoleEcho(bool /*echo*/) {
    // empty
}

void setConsoleEnabled(bool /*enabled*/) {
    // empty
}

void setConsoleErrorColor(const std::string& /*color*/) {
    // empty
}

void setConsoleEventOnClose(bool /*eventOnClose*/) {
    // empty
}

void setConsoleExitProgramOnClose(bool /*exitOnClose*/) {
    // empty
}

void setConsoleFont(const std::string& /*font*/) {
    // empty
}

void setConsoleLocation(double /*x*/, double /*y*/) {
    // empty
}

void setConsoleLocationSaved(bool /*value*/) {
    // empty
}

void setConsoleOutputColor(const std::string& /*color*/) {
    // empty
}

void setConsoleSettingsLocked(bool /*value*/) {
    // empty
}

void setConsoleSize(double /*width*/, double /*height*/) {
    // empty
}

void setConsoleWindowTitle(const std::string& /*title*/) {
    // empty
}

void shutdownConsole() {
    // empty
}

#else // SPL_HEADLESS_MODE

void clearConsole() {
    GConsoleWindow::instance()->clearConsole();
}

bool getConsoleClearEnabled() {
    return GConsoleWindow::instance()->isClearEnabled();
}

/* GWindow::CloseOperation */ int getConsoleCloseOperation() {
    return GConsoleWindow::instance()->getCloseOperation();
}

bool getConsoleEcho() {
    return GConsoleWindow::instance()->isEcho();
}

bool getConsoleEnabled() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return false;
#else
    return GConsoleWindow::consoleEnabled();
#endif
}

bool getConsoleEventOnClose() {
    return true;
}

bool getConsoleExitProgramOnClose() {
    return GConsoleWindow::instance()->getCloseOperation() == GWindow::CLOSE_EXIT;
}

std::string getConsoleFont() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return GConsoleWindow::getDefaultFont();
#else
    return GConsoleWindow::instance()->getFont();
#endif
}

double getConsoleHeight() {
    return GConsoleWindow::instance()->getHeight();
}

GPoint getConsoleLocation() {
    return GConsoleWindow::instance()->getLocation();
}

bool getConsoleLocationSaved() {
    return GConsoleWindow::instance()->isLocationSaved();
}

bool getConsolePrintExceptions() {
    return exceptions::getTopLevelExceptionHandlerEnabled();
}

bool getConsoleSettingsLocked() {
    return GConsoleWindow::isInitialized()
            && GConsoleWindow::instance()->isLocked();
}

GDimension getConsoleSize() {
    return GConsoleWindow::instance()->getSize();
}

double getConsoleWidth() {
    return GConsoleWindow::instance()->getWidth();
}

GConsoleWindow* getConsoleWindow() {
    return GConsoleWindow::instance();
}

std::string getConsoleWindowTitle() {
    return GConsoleWindow::instance()->getTitle();
}

void setConsoleClearEnabled(bool value) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setClearEnabled(value);
}

void setConsoleCloseOperation(/*GWindow::CloseOperation*/ int op) {
    GWindow::CloseOperation gwcop = static_cast<GWindow::CloseOperation>(op);
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setCloseOperation(gwcop);
}

void setConsoleEcho(bool echo) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setEcho(echo);
}

void setConsoleEnabled(bool enabled) {
    GConsoleWindow::setConsoleEnabled(enabled);
}

void setConsoleErrorColor(const std::string& color) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setErrorColor(color);
}

void setConsoleEventOnClose(bool /*eventOnClose*/) {
    // empty
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setExitOnClose(exitOnClose);
}

void setConsoleFont(const std::string& font) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setFont(font);
}

void setConsoleLocation(double x, double y) {
    if (getConsoleSettingsLocked()) { return; }
    if (floatingPointEqual(x, -1) && floatingPointEqual(y, -1)) {
        GConsoleWindow::instance()->center();
    } else {
        GConsoleWindow::instance()->setLocation(x, y);
    }
}

void setConsoleLocationSaved(bool value) {
    GConsoleWindow::instance()->setLocationSaved(value);
}

void setConsoleOutputColor(const std::string& color) {
    GConsoleWindow::instance()->setOutputColor(color);
}

void setConsoleSettingsLocked(bool value) {
    GConsoleWindow::instance()->setLocked(value);
}

void setConsoleSize(double width, double height) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setConsoleSize(width, height);
}

void setConsoleWindowTitle(const std::string& title) {
    if (getConsoleSettingsLocked()) { return; }
    GConsoleWindow::instance()->setTitle(title);
}

void shutdownConsole() {
    if (getConsoleEnabled() && !GConsoleWindow::instance()->isLocked()) {
        GConsoleWindow::instance()->shutdown("Shutdown");
    }
}

#endif // SPL_HEADLESS_MODE

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * to default values
 */
void setConsolePropertiesQt() {
#if defined(SPL_CONSOLE_FONTSIZE)
    std::string fontStr = std::string("Monospaced-Bold-") + std::to_string(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

    setConsoleSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    setConsoleLocation(DEFAULT_X, DEFAULT_Y);
 #if defined(SPL_CONSOLE_LOCATION_SAVED)
    setConsoleLocationSaved(true);
 #endif
    setConsoleEcho(DEFAULT_ECHO);
    setConsoleExitProgramOnClose(DEFAULT_EXIT_ON_CLOSE);
}

void initializeQtGraphicalConsole() {
#ifndef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    // ensure that console is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    static std::ios_base::Init ios_base_init;

#ifndef SPL_HEADLESS_MODE
    if (GConsoleWindow::consoleEnabled()) {
        GConsoleWindow::instance();   // ensure that console window is ready
        setConsolePropertiesQt();
    }
#endif // SPL_HEADLESS_MODE

#endif // __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
}
