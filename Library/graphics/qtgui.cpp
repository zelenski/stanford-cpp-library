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
#include "consoletext.h"
#include "error.h"
#include "exceptions.h"
#include "gconsolewindow.h"
#include "gthread.h"
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

QtGui::QtGui(int argc, char **argv) {
    connect(GEventQueue::instance(), SIGNAL(eventReady()), this, SLOT(processEventFromQueue()));
    Q_INIT_RESOURCE(images);
    _argc = argc;
    _argv = argv;
}

void QtGui::createInstance(int argc, char **argv) {
    if (_instance) {
        error("Attempt to re-create already running Qt GUI.");
    }
    GThread::ensureThatThisIsTheQtGuiThread("Qt GUI must be created on the main thread.");
    _instance = new QtGui(argc, argv);
    _app = new QSPLApplication(_instance->_argc, _instance->_argv);
    _app->setQuitOnLastWindowClosed(false);
}

void QtGui::exitGraphics(int exitCode) {
    static bool already_exited = false;
    if (already_exited) return;
    already_exited = true;

    if (exitCode == EXITING_DUE_TO_WINDOW_CLOSE) {
        std::cout << std::endl << std::endl << "[Program exiting due to window close event]" << std::endl;
        exitCode = 0;
    }
    if (_app) {
        auto saved = _app;
        _app = nullptr;
        saved->quit();
        delete saved;
        _Exit(exitCode);  // exit w/o calling destructors (avoid order problems between static/tls)
        // sad news: _Exit on mingw *does* call some dtors (in violation of C++ standard)
        // https://github.com/qt/qtbase/blob/fd1e3fe0d77e1faf6572790d4c8cef4fabc6e54c/src/corelib/global/qassert.cpp#L29C58-L34C34
        // from what I can tell, not currently causing problem but if it becomes one, switch to abort/terminate
    } else {
        std::exit(exitCode);
    }
}

int QtGui::getArgc() const {
    return _argc;
}

char** QtGui::getArgv() const {
    return _argv;
}

QtGui* QtGui::instance() {
    if (!_instance) {
        error("No Qt GUI instance!");
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

// this should be called by the Qt main thread
void QtGui::startBackgroundEventLoop(GThunkInt mainFunc, bool exitAfter) {
    GThread::ensureThatThisIsTheQtGuiThread("QtGui::startBackgroundEventLoop");
    native_set_thread_name("Qt GUI Event Loop");
    // start student's main function in its own second thread

    if (!GThread::studentThreadExists()) {
        GThread::startStudentThread([&]() -> int {
            exceptions::setTopLevelExceptionHandlerEnabled(true);
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
    GThread::msleep(10);

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


