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
#include <QtGlobal>
#include <QThread>
#include "consoletext.h"
#include "error.h"
#include "exceptions.h"
#include "gconsolewindow.h"
#include "gthread.h"
#include "strlib.h"
#include "private/static.h"
#include "private/init.h"

// QSPLApplication members
QSPLApplication::QSPLApplication(int& argc, char *argv[])
        : QApplication(argc, argv) {
    // empty
}

bool QSPLApplication::notify(QObject* receiver, QEvent* e) {
    // could use try/catch here to handle exceptions on gui thread
    // but this disguises where came from (loses backtrace)
    return QApplication::notify(receiver, e);   // call super
}


// QtGui members
QSPLApplication* QtGui::_app = nullptr;
QtGui* QtGui::_instance = nullptr;

QtGui::QtGui()
        : _initialized(false) {
    connect(GEventQueue::instance(), SIGNAL(eventReady()), this, SLOT(processEventFromQueue()));
}

void QtGui::exitGraphics(int exitCode) {
    if (exitCode == EXITING_DUE_TO_WINDOW_CLOSE) {
        std::cout << std::endl << std::endl << "[Program exiting due to window close event]" << std::endl;
        exitCode = 0;
    }
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

QSPLApplication* QtGui::getApplication() {
    return _app;
}

std::string QtGui::getApplicationDisplayName() const {
    return (_app ? _app->applicationDisplayName().toStdString() : "");
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
            _app = new QSPLApplication(_argc, _argv);
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

void QtGui::processEventFromQueue() {
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
    native_set_thread_name("Qt GUI Event Loop");
    // start student's main function in its own second thread

    if (!GThread::studentThreadExists()) {
        GThread::startStudentThread([&]() -> int {
            stanfordcpplib::initializeStudentThread();
            int result = mainFunc();
            stanfordcpplib::studentThreadHasExited("Completed");
            return result;
        });

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
    // Qt GUI main thread blocks here until student main() finishes (in its own thread)
    int exitCode = _app->exec();

    // if I get here, it means an "exit on close" window was just closed;
    // it's time to shut down the Qt system and exit the C++ program
    if (exitAfter) {
        exitGraphics(exitCode);
    }
}



namespace stanfordcpplib {
void studentThreadHasExited(const std::string& reason) {
    // briefly wait for the console to finish printing any/all output
    GThread::getCurrentThread()->yield();
    GThread::getCurrentThread()->sleep(1);

    // if I get here, student's main() has finished running;
    // indicate this by showing a completed title on the graphical console
    if (getConsoleEnabled()) {
#ifndef SPL_HEADLESS_MODE
        GConsoleWindow* console = getConsoleWindow();
        if (console) {
            console->shutdown(reason);
        }
#endif // SPL_HEADLESS_MODE
    } else {
        // need to exit here else program will not terminate
        // BUGFIX: no, this is not needed and is bad; it exits the window too soon; disable
        // QtGui::instance()->exitGraphics(result);
    }
}
} // namespace stanfordcpplib


