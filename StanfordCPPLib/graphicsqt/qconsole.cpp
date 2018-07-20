/*
 * File: qconsole.cpp
 * ------------------
 * This file implements the qconsole.h interface.
 *
 * @version 2018/07/15
 * - initial version, based on io/console.cpp
 */

#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "qconsole.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE

#include <cstdio>
#include <iostream>
#include <string>
#include "error.h"
#include "exceptions.h"
#include "private/consolestreambuf.h"
#include "private/forwardingstreambuf.h"
#include "private/static.h"
#include "private/version.h"
#include "qgtextarea.h"
#include "qgui.h"

namespace stanfordcpplib {
namespace qtgui {
// static 'variables' (as functions to avoid initialization ordering bugs)
STATIC_VARIABLE_DECLARE(bool, consoleClearEnabled, true)
STATIC_VARIABLE_DECLARE(bool, consoleEcho, false)
STATIC_VARIABLE_DECLARE(bool, consoleEnabled, false)
STATIC_VARIABLE_DECLARE(bool, consoleEventOnClose, false)
STATIC_VARIABLE_DECLARE(bool, consoleExitProgramOnClose, false)
STATIC_VARIABLE_DECLARE(std::string, consoleFont, "Monospaced-14")
STATIC_VARIABLE_DECLARE(QGPoint, consoleLocation, QGPoint(10, 10))
STATIC_VARIABLE_DECLARE(bool, consoleLocationSaved, false)
STATIC_VARIABLE_DECLARE(bool, consoleLocked, false)
STATIC_VARIABLE_DECLARE(std::string, consoleTitle, "Console")
STATIC_VARIABLE_DECLARE(QGDimension, consoleSize, QGDimension(700, 500))
STATIC_VARIABLE_DECLARE(ConsoleCloseOperation, consoleCloseOperation, ConsoleCloseOperation::CONSOLE_HIDE_ON_CLOSE)
STATIC_VARIABLE_DECLARE(stanfordcpplib::qtgui::ConsoleStreambufQt*, cinout_new_buf, nullptr)
STATIC_VARIABLE_DECLARE(stanfordcpplib::qtgui::ConsoleStreambufQt*, cerr_new_buf, nullptr)
STATIC_VARIABLE_DECLARE(::QGWindow*, guiConsoleWindow, nullptr)
STATIC_VARIABLE_DECLARE(::QGTextArea*, guiConsoleTextArea, nullptr)
// end static 'variables'

void setConsolePropertiesQt();

// private functions
void ensureConsoleWindow() {
    if (!STATIC_VARIABLE(guiConsoleWindow)) {
        // TODO: getConsoleSize/Width/Height
        QGui::instance()->initializeQt();
        QGui::instance()->runOnQtGuiThread([]() {
            // these variables must come before the QGWindow is created
            bool exitOnClose = getConsoleExitProgramOnClose();
            std::string font = getConsoleFont();
            QGDimension size = getConsoleSize();
            std::string title = getConsoleWindowTitle();

            STATIC_VARIABLE(guiConsoleWindow) = new QGWindow(size.getWidth(), size.getHeight());
            STATIC_VARIABLE(guiConsoleWindow)->setTitle(title);
            STATIC_VARIABLE(guiConsoleTextArea) = new QGTextArea();
            STATIC_VARIABLE(guiConsoleTextArea)->setEditable(false);
            STATIC_VARIABLE(guiConsoleTextArea)->setFont(font);
            STATIC_VARIABLE(guiConsoleTextArea)->setColor("black");

            // TODO: key listener

            STATIC_VARIABLE(guiConsoleWindow)->addToRegion(STATIC_VARIABLE(guiConsoleTextArea), "Center");
            STATIC_VARIABLE(guiConsoleWindow)->setResizable(false);
            STATIC_VARIABLE(guiConsoleWindow)->setExitOnClose(exitOnClose);
            STATIC_VARIABLE(guiConsoleWindow)->setVisible(true);
            //    STATIC_VARIABLE(guiConsoleWindow)->setCloseOperation(
            //                op == CONSOLE_DO_NOTHING_ON_CLOSE ? QGWindow::CLOSE_DO_NOTHING
            //                : op == CONSOLE_HIDE_ON_CLOSE     ? QGWindow::CLOSE_HIDE
            //                : op == CONSOLE_DISPOSE_ON_CLOSE  ? QGWindow::CLOSE_DISPOSE
            //                : op == CONSOLE_EXIT_ON_CLOSE     ? QGWindow::CLOSE_EXIT : QGWindow::CLOSE_HIDE);

            setConsolePropertiesQt();
        });
    }
}

void endLineConsoleQt(bool isStderr) {
    putConsoleQt("\n", isStderr);
}

std::string getLineConsoleQt() {
    // printf("    DEBUG getLineConsoleQt! TODO\n");
    ensureConsoleWindow();
    if (getConsoleEcho()) {
        // TODO
    }

    // TODO
    return "";
}

void putConsoleQt(const std::string& str, bool isStderr) {
    // printf("    DEBUG putConsoleQt \"%s\"! %d TODO\n", str.c_str(), isStderr);
    ensureConsoleWindow();
    if (getConsoleEcho()) {
        fprintf(isStderr ? stderr : stdout, "%s", str.c_str());
        fflush(isStderr ? stderr : stdout);
    }
    // TODO: can this be async?
    QGui::instance()->runOnQtGuiThreadAsync([str, isStderr]() {
        if (isStderr) {
            // STATIC_VARIABLE(guiConsoleTextArea)->appendHtml("<code class=\"out\">" + htmlEncode(str) + "</code>");
            STATIC_VARIABLE(guiConsoleTextArea)->appendFormattedText(str, "red");
        } else {
            // STATIC_VARIABLE(guiConsoleTextArea)->appendHtml("<code class=\"err\">" + htmlEncode(str) + "</code>");
            STATIC_VARIABLE(guiConsoleTextArea)->appendFormattedText(str, "black");
        }
    });
}


void clearConsole() {
    std::string msg = "==================== (console cleared) ====================";
    if (getConsoleClearEnabled()) {
        // print to standard console (not Stanford graphical console)
        printf("%s\n", msg.c_str());
        // TODO: flush?

        // clear the graphical console window
        if (STATIC_VARIABLE(guiConsoleTextArea)) {
            STATIC_VARIABLE(guiConsoleTextArea)->clearText();
        }
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

bool getConsoleEnabled() {
#ifdef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
    return false;
#else
    return STATIC_VARIABLE(consoleEnabled);
#endif
}

bool getConsoleEventOnClose() {
    return STATIC_VARIABLE(consoleEventOnClose);
}

bool getConsoleExitProgramOnClose() {
    return STATIC_VARIABLE(consoleExitProgramOnClose);
}

std::string getConsoleFont() {
    return STATIC_VARIABLE(consoleFont);
}

double getConsoleHeight() {
    return getConsoleSize().getHeight();
}

QGPoint getConsoleLocation() {
    return STATIC_VARIABLE(consoleLocation);
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

QGDimension getConsoleSize() {
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        return STATIC_VARIABLE(guiConsoleWindow)->getSize();
    } else {
        return STATIC_VARIABLE(consoleSize);
    }
}

double getConsoleWidth() {
    return getConsoleSize().getWidth();
}

QGWindow* getConsoleWindow() {
    return STATIC_VARIABLE(guiConsoleWindow);
}

std::string getConsoleWindowTitle() {
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        return STATIC_VARIABLE(guiConsoleWindow)->getTitle();
    } else {
        return STATIC_VARIABLE(consoleTitle);
    }
}

void setConsoleClearEnabled(bool value) {
    STATIC_VARIABLE(consoleClearEnabled) = value;
}

void setConsoleCloseOperation(ConsoleCloseOperation op) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleCloseOperation) = op;
    setConsoleExitProgramOnClose(op == ConsoleCloseOperation::CONSOLE_EXIT_ON_CLOSE);
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        STATIC_VARIABLE(guiConsoleWindow)->setCloseOperation(
                  op == CONSOLE_DO_NOTHING_ON_CLOSE ? QGWindow::CLOSE_DO_NOTHING
                : op == CONSOLE_HIDE_ON_CLOSE     ? QGWindow::CLOSE_HIDE
                : op == CONSOLE_DISPOSE_ON_CLOSE  ? QGWindow::CLOSE_DISPOSE
                : op == CONSOLE_EXIT_ON_CLOSE     ? QGWindow::CLOSE_EXIT : QGWindow::CLOSE_HIDE);
    }
}

void setConsoleEcho(bool echo) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleEcho) = echo;
    // TODO: echo
}

void setConsoleErrorColor(const std::string& /* color */) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    // TODO(platform): set error color
    // stanfordcpplib::getPlatform()->jbeconsole_setErrorColor(color);
}

void setConsoleEventOnClose(bool eventOnClose) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleEventOnClose) = eventOnClose;
}

void setConsoleExitProgramOnClose(bool exitOnClose) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleExitProgramOnClose) = exitOnClose;
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        STATIC_VARIABLE(guiConsoleWindow)->setExitOnClose(exitOnClose);
    }
}

void setConsoleFont(const std::string& font) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleFont) = font;
    if (STATIC_VARIABLE(guiConsoleTextArea)) {
        STATIC_VARIABLE(guiConsoleTextArea)->setFont(font);
    }
}

void setConsoleLocation(double x, double y) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleLocation) = QGPoint(x, y);
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        STATIC_VARIABLE(guiConsoleWindow)->setLocation(STATIC_VARIABLE(consoleLocation));
    }
}

void setConsoleLocationSaved(bool value) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleLocationSaved) = value;
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        // TODO(platform): set location saved
    }
}

void setConsoleOutputColor(const std::string& /* color */) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    // TODO: set variable?
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        // TODO(platform): set output color
    }
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
    STATIC_VARIABLE(consoleSize) = QGDimension(width, height);
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        STATIC_VARIABLE(guiConsoleWindow)->setSize(STATIC_VARIABLE(consoleSize));
    }
}

void setConsoleWindowTitle(const std::string& title) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleTitle) = title;
    if (STATIC_VARIABLE(guiConsoleWindow)) {
        STATIC_VARIABLE(guiConsoleWindow)->setTitle(title);
    }
}

/*
 * Sets up console settings like window size, location, exit-on-close, etc.
 * based on compiler options set in the .pro file.
 */
void setConsolePropertiesQt() {
#if defined(SPL_CONSOLE_FONTSIZE)
    std::string fontStr = std::string("Monospaced-Bold-") + integerToString(SPL_CONSOLE_FONTSIZE);
    setConsoleFont(fontStr);
#endif

#if defined(SPL_CONSOLE_WIDTH) && defined(SPL_CONSOLE_HEIGHT)
    setConsoleSize(SPL_CONSOLE_WIDTH, SPL_CONSOLE_HEIGHT);
#endif

#if defined(SPL_CONSOLE_X) && defined(SPL_CONSOLE_Y)
    setConsoleLocation(SPL_CONSOLE_X, SPL_CONSOLE_Y);
#endif

#if defined(SPL_CONSOLE_ECHO)
    setConsoleEcho(true);
#endif

#if defined(SPL_CONSOLE_EXIT_ON_CLOSE)
    setConsoleExitProgramOnClose(true);
#endif

#if defined(SPL_CONSOLE_LOCATION_SAVED)
    setConsoleLocationSaved(true);
#endif

#if defined(SPL_VERIFY_PROJECT_VERSION)
    version::ensureProjectVersion();
#endif
}

void initializeStanfordCppLibraryQt() {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    static std::ios_base::Init ios_base_init;

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true);
#endif
}

void initializeQtGraphicalConsole() {
    // ensure that console is initialized only once
    static char stderrBuf[BUFSIZ + 10] = {'\0'};
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    initializeStanfordCppLibraryQt();

    // buffer C-style stderr
    std::ios::sync_with_stdio(false);
    setbuf(stderr, stderrBuf);

    // redirect cin/cout/cerr
    STATIC_VARIABLE(cinout_new_buf) = new stanfordcpplib::qtgui::ConsoleStreambufQt();
    STATIC_VARIABLE(cerr_new_buf) = new stanfordcpplib::qtgui::ConsoleStreambufQt(/* isStderr */ true);
    std::cin.rdbuf(STATIC_VARIABLE(cinout_new_buf));
    std::cout.rdbuf(STATIC_VARIABLE(cinout_new_buf));
    std::cerr.rdbuf(STATIC_VARIABLE(cerr_new_buf));
    // ::stanfordcpplib::ForwardingStreambuf* stderrStreamBuf = new ::stanfordcpplib::ForwardingStreambuf(*STATIC_VARIABLE(cinout_new_buf), /* isStderr */ true);
    // stderrStreamBuf->pubsetbuf(cerrBuf, BUFSIZ + 10);
    // std::cerr.rdbuf(stderrStreamBuf);
    // std::nounitbuf(std::cerr);   // disable buffering after each token

#ifdef _WIN32
//    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // _WIN32

    setConsolePropertiesQt();
}

// This one is at the bottom because it's not meant to be called by students.
void setConsoleEnabled(bool enabled) {
    if (STATIC_VARIABLE(consoleLocked)) { return; }
    STATIC_VARIABLE(consoleEnabled) = enabled;
}

} // namespace qtgui

} // namespace stanfordcpplib
