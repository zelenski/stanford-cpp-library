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

#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#include <QEvent>
#include <QtGlobal>
#include <QThread>
#include "consoletext.h"
#include "error.h"
#include "exceptions.h"
#include "gthread.h"
#include "strlib.h"
#include "private/static.h"

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32

#undef INTERNAL_INCLUDE


// QtGui members
QApplication* QtGui::_app = nullptr;
QtGui* QtGui::_instance = nullptr;

QtGui::QtGui()
        : _initialized(false) {
    connect(GEventQueue::instance(), SIGNAL(mySignal()), this, SLOT(mySlot()));
}

void QtGui::exitGraphics(int exitCode) {
    if (_app) {
// need to temporarily turn off C++ lib exit macro to call QApplication's exit method
// (NOTE: must keep in sync with exit definition in init.h)
#undef exit
        _app->quit();
        _app = nullptr;
        std::exit(exitCode);
#define exit __stanfordcpplib__exitLibrary
    } else {
        std::exit(exitCode);
    }
}

QApplication* QtGui::getApplication() {
    return _app;
}

int QtGui::getArgc() const {
    return _argc;
}

char** QtGui::getArgv() const {
    return _argv;
}

void QtGui::initializeQt() {
    if (_app) return;

    GThread::runOnQtGuiThread([this]() {
        if (!_app) {
            qSetMessagePattern(
                    "Qt internal warning: %{message}\n"
                    "  - pid: %{pid}\n"
                    "  - thread: %{threadid}\n"

                    // backtrace doesn't work on windows and some other builds
#ifndef _WIN32
                    "  - stack:\n"
                    "      %{backtrace depth=20 separator=\"\n      \"}"
#endif // _WIN32
            );
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

void QtGui::setArgs(int argc, char** argv) {
    _argc = argc;
    _argv = argv;
}

// this should be called by the Qt main thread
void QtGui::startBackgroundEventLoop(GThunkInt mainFunc, bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!GThread::studentThreadExists()) {
        GStudentThread::startStudentThread(mainFunc);
        startEventLoop(exitAfter);   // begin Qt event loop on main thread
    }
}

// this should be called by the Qt main thread
void QtGui::startBackgroundEventLoopVoid(GThunk mainFunc, bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!GThread::studentThreadExists()) {
        GStudentThread::startStudentThreadVoid(mainFunc);
        startEventLoop(exitAfter);   // begin Qt event loop on main thread
    }
}

// this should be called by the Qt main thread
void QtGui::startEventLoop(bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startEventLoop");
    if (!_app) {
        error("QtGui::startEventLoop: need to initialize Qt first");
    }

    // start Qt event loop on main thread;
    // Qt GUI main thread blocks here until student main() is done
    int exitCode = _app->exec();

    // if I get here, it means an "exit on close" window was just closed;
    // it's time to shut down the Qt system and exit the C++ program
    if (exitAfter) {
        exitGraphics(exitCode);
    }
}

#ifdef SPL_PRECOMPILE_QT_MOC_FILES
#include "moc_qtgui.cpp"   // speeds up compilation of auto-generated Qt files
#endif // SPL_PRECOMPILE_QT_MOC_FILES
