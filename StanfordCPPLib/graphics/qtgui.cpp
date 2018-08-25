/*
 * File: qtgui.cpp
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to qtgui.cpp
 * @version 2018/07/03
 * - initial version
 */

#include "qtgui.h"
#include <QEvent>
#include <QThread>
#include "error.h"
#include "exceptions.h"
#include "gthread.h"
#include "strlib.h"
#include "private/static.h"

#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "console.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32


// QtGui members
QApplication* QtGui::_app = nullptr;
int QtGui::_argc = 0;
char** QtGui::_argv = nullptr;
QtGui* QtGui::_instance = nullptr;

QtGui::QtGui()
        : _initialized(false) {
    connect(GEventQueue::instance(), SIGNAL(mySignal()), this, SLOT(mySlot()));
}

void QtGui::exitGraphics(int exitCode) {
    if (_app) {
// need to temporarily turn off C++ lib exit macro to call QApplication's exit method
#undef exit
        _app->quit();
        _app = nullptr;
        std::exit(exitCode);
#define exit __stanfordCppLibExit
    } else {
        std::exit(exitCode);
    }
}

void QtGui::initializeQt() {
    if (_app) return;

    GThread::runOnQtGuiThread([this]() {
        if (!_app) {
            _app = new QApplication(_argc, _argv);
            _initialized = true;
        }
    });
}

QtGui* QtGui::instance() {
    if (!_instance) {
        _instance = new QtGui();
        GEventQueue::instance();   // create event queue on Qt GUI main thread
    }
    return _instance;
}

void QtGui::mySlot() {
    if (!GEventQueue::instance()->isEmpty()) {
        GThunk thunk = GEventQueue::instance()->peek();
        thunk();
        GEventQueue::instance()->dequeue();
    }
}

// this should be called by the Qt main thread
void QtGui::startBackgroundEventLoop(GThunkInt mainFunc) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!GThread::studentThreadExists()) {
        GStudentThread::startStudentThread(mainFunc);
        startEventLoop();   // begin Qt event loop on main thread
    }
}

// this should be called by the Qt main thread
void QtGui::startEventLoop() {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startEventLoop");
    if (!_app) {
        error("QtGui::startEventLoop: need to initialize Qt first");
    }

    // start Qt event loop on main thread;
    // Qt GUI main thread blocks here until student main() is done
    int exitCode = _app->exec();

    // if I get here, it means an "exit on close" window was just closed;
    // it's time to shut down the Qt system and exit the C++ program
    exitGraphics(exitCode);
}

static int (* _mainFunc)(void);

// this should be roughly the same code as platform.cpp's parseArgs function
void __parseArgsQt(int argc, char** argv) {
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

namespace stanfordcpplib {
namespace qtgui {
extern void initializeQtGraphicalConsole();
extern void shutdownConsole();
}
}

// called automatically by real main() function;
// call to this is inserted by library init.h
// to be run in Qt main thread
void __initializeStanfordCppLibraryQt(int argc, char** argv, int (* mainFunc)(void)) {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;

    GThread::setMainThread();
    QtGui::_argc = argc;
    QtGui::_argv = argv;
    __parseArgsQt(argc, argv);

    // initialize the main Qt graphics subsystem
    QtGui::instance()->initializeQt();

    // initialize Qt graphical console (if student #included it)
    initializeQtGraphicalConsole();

    // TODO: remove
    // extern void initPipe();
    // initPipe();

    // set up student's main function
    _mainFunc = mainFunc;
    QtGui::instance()->startBackgroundEventLoop(mainFunc);
}

// to be run in Qt main thread
void __shutdownStanfordCppLibraryQt() {
    // shut down the Qt graphical console window
    shutdownConsole();
}

// see init.h

namespace stanfordcpplib {

STATIC_VARIABLE_DECLARE(bool, isExitEnabled, true)

bool exitEnabled() {
    return STATIC_VARIABLE(isExitEnabled);
}

void setExitEnabled(bool enabled) {
    STATIC_VARIABLE(isExitEnabled) = enabled;
    // TODO: notify GConsoleWindow?
}

} // namespace stanfordcpplib


namespace std {

void __stanfordCppLibExit(int status) {
    if (stanfordcpplib::exitEnabled()) {
        // call std::exit (has been renamed)

#undef exit
        std::exit(status);
#define exit __stanfordCppLibExit

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
