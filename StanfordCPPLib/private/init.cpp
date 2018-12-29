/*
 * File: init.cpp
 * --------------
 *
 * TODO
 *
 * @author Marty Stepp
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/08/28
 * - refactor to use stanfordcpplib namespace
 * @version 2018/08/27
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "private/init.h"
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "exceptions.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "strlib.h"
#define INTERNAL_INCLUDE 1
#include "private/static.h"
#undef INTERNAL_INCLUDE

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32


namespace stanfordcpplib {

namespace qtgui {
extern void initializeQtGraphicalConsole();
extern void shutdownConsole();
}

static void parseArgsQt(int argc, char** argv);

STATIC_VARIABLE_DECLARE(bool, isExitEnabled, true)

bool exitEnabled() {
    return STATIC_VARIABLE(isExitEnabled);
}

// called automatically by real main() function;
// call to this is inserted by library init.h
// to be run in Qt GUI main thread
void initializeLibrary(int argc, char** argv) {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

#ifndef SPL_HEADLESS_MODE
    GThread::setMainThread();
#endif // SPL_HEADLESS_MODE

    parseArgsQt(argc, argv);

#ifndef SPL_HEADLESS_MODE
    // initialize the main Qt graphics subsystem
    QtGui::instance()->setArgs(argc, argv);
    QtGui::instance()->initializeQt();

    // initialize Qt graphical console (if student #included it)
    initializeQtGraphicalConsole();
#endif // SPL_HEADLESS_MODE
}

void initializeLibraryStudentThread() {
#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    setConsolePrintExceptions(true, /* force */ true);
#endif
}

// this should be roughly the same code as platform.cpp's parseArgs function
static void parseArgsQt(int argc, char** argv) {
    if (argc <= 0) {
        return;
    }
    std::string arg0 = argv[0];
    exceptions::setProgramNameForStackTrace(argv[0]);
    // programName() = getRoot(getTail(arg0));

#ifndef _WIN32
    // on Mac only, may need to change folder because of app's nested dir structure
    size_t ax = arg0.find(".app/Contents/");
    if (ax != std::string::npos) {
        while (ax > 0 && arg0[ax] != '/') {
            ax--;
        }
        if (ax > 0) {
            std::string cwd = arg0.substr(0, ax);
            chdir(cwd.c_str());
        }
    }
#endif // _WIN32

    char* noConsoleFlag = getenv("NOCONSOLE");
    if (noConsoleFlag && startsWith(std::string(noConsoleFlag), "t")) {
        return;
    }
}

// called automatically by real main() function;
// call to this is inserted by library init.h
// to be run in Qt main thread
#ifdef SPL_HEADLESS_MODE
void runMainInThread(int (* mainFunc)(void)) {
    mainFunc();
}

void runMainInThread(std::function<int()> mainFunc) {
    mainFunc();
}

void runMainInThreadVoid(void (* mainFuncVoid)(void)) {
    mainFuncVoid();
}

void runMainInThreadVoid(std::function<void()> mainFuncVoid) {
    mainFuncVoid();
}
#else // SPL_HEADLESS_MODE
void runMainInThread(int (* mainFunc)(void)) {
    QtGui::instance()->startBackgroundEventLoop(mainFunc);
}

void runMainInThread(std::function<int()> mainFunc) {
    QtGui::instance()->startBackgroundEventLoop(mainFunc);
}

void runMainInThreadVoid(void (* mainFuncVoid)(void)) {
    QtGui::instance()->startBackgroundEventLoopVoid(mainFuncVoid);
}

void runMainInThreadVoid(std::function<void()> mainFuncVoid) {
    QtGui::instance()->startBackgroundEventLoopVoid(mainFuncVoid);
}
#endif // SPL_HEADLESS_MODE

void setExitEnabled(bool enabled) {
    STATIC_VARIABLE(isExitEnabled) = enabled;
    // TODO: notify GConsoleWindow?
}

// shut down the Qt graphical console window;
// to be run in Qt main thread
void shutdownLibrary() {
#ifdef SPL_HEADLESS_MODE
    // empty
#else
    shutdownConsole();
#endif // SPL_HEADLESS_MODE
}

void staticInitializeLibrary() {
    // empty
}

} // namespace stanfordcpplib

namespace std {
void __stanfordcpplib__exitLibrary(int status) {
    if (stanfordcpplib::exitEnabled()) {
        // call std::exit (has been renamed)

#ifdef exit
#undef exit
        std::exit(status);
#define exit __stanfordcpplib__exitLibrary
#endif // exit

    } else {
        // not allowed to call exit(); produce error message
        std::ostringstream out;
        out << "Program tried to call exit(" << status << ") to quit. " << std::endl;
        out << "*** This function has been disabled; main should end through " << std::endl;
        out << "*** normal program control flow." << std::endl;
        error(out.str());
    }
}

} // namespace std
