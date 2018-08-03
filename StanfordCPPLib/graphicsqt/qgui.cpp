/*
 * File: qgui.cpp
 * --------------
 *
 * @version 2018/07/03
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgui.h"
#include <QEvent>
#include <QThread>
#include "error.h"
#include "exceptions.h"
#include "qgthread.h"
#include "strlib.h"

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32


// QGui members
QApplication* QGui::_app = nullptr;
int QGui::_argc = 0;
char** QGui::_argv = nullptr;
QGui* QGui::_instance = nullptr;

QGui::QGui()
        : _initialized(false) {
    connect(QGuiEventQueue::instance(), SIGNAL(mySignal()), this, SLOT(mySlot()));
}

void QGui::exitGraphics(int exitCode) {
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

void QGui::initializeQt() {
    QGThread::ensureThatThisIsTheQtGuiThread("QGui::initializeQt");
    if (!_app) {
        _app = new QApplication(_argc, _argv);
        _initialized = true;
    }
}

QGui* QGui::instance() {
    if (!_instance) {
        _instance = new QGui();
        QGuiEventQueue::instance();   // create event queue on Qt GUI main thread
    }
    return _instance;
}

void QGui::mySlot() {
    if (!QGuiEventQueue::instance()->isEmpty()) {
        QGThunk thunk = QGuiEventQueue::instance()->peek();
        thunk();
        QGuiEventQueue::instance()->dequeue();
    }
}

// this should be called by the Qt main thread
void QGui::startBackgroundEventLoop(QGThunkInt mainFunc) {
    QGThread::ensureThatThisIsTheQtGuiThread("QGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!QGThread::studentThreadExists()) {
        QGStudentThread::startStudentThread(mainFunc);
        startEventLoop();   // begin Qt event loop on main thread
    }
}

// this should be called by the Qt main thread
void QGui::startEventLoop() {
    QGThread::ensureThatThisIsTheQtGuiThread("QGui::startEventLoop");
    if (!_app) {
        error("QGui::startEventLoop: need to initialize Qt first");
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

    QGThread::setMainThread();
    QGui::_argc = argc;
    QGui::_argv = argv;
    __parseArgsQt(argc, argv);

    // initialize the main Qt graphics subsystem
    QGui::instance()->initializeQt();

    // initialize Qt graphical console (if student #included it)
    ::stanfordcpplib::qtgui::initializeQtGraphicalConsole();

    // TODO: remove
    // extern void initPipe();
    // initPipe();

    // set up student's main function
    _mainFunc = mainFunc;
    QGui::instance()->startBackgroundEventLoop(mainFunc);
}

// to be run in Qt main thread
void __shutdownStanfordCppLibraryQt() {
    // shut down the Qt graphical console window
    stanfordcpplib::qtgui::shutdownConsole();
}

#endif // SPL_QT_GUI
