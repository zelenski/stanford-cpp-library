/*
 * File: gthread.cpp
 * -----------------
 *
 * This file implements the members declared in gthread.h.
 *
 * @version 2018/10/18
 * - improved thread names
 * @version 2018/10/01
 * - bug fix where output wasn't showing up on the console if main ended too soon
 * @version 2018/09/23
 * - bug fix to shut down console at end of program
 * @version 2018/08/23
 * - renamed to gthread.h to replace Java version
 * @version 2018/07/28
 * - initial version
 */

#define INTERNAL_INCLUDE 1
#include "gthread.h"
#define INTERNAL_INCLUDE 1
#include "consoletext.h"
#define INTERNAL_INCLUDE 1
#include "gconsolewindow.h"
#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "geventqueue.h"
#define INTERNAL_INCLUDE 1
#include "qtgui.h"
#define INTERNAL_INCLUDE 1
#include "require.h"
#undef INTERNAL_INCLUDE

GFunctionThread::GFunctionThread(GThunk func, const std::string& threadName)
        : _func(func) {
    if (!threadName.empty()) {
        setObjectName(QString::fromStdString(threadName));
    }
}

void GFunctionThread::run() {
    _func();
}


/*static*/ QThread* GThread::_qtMainThread = nullptr;
/*static*/ QThread* GThread::_studentThread = nullptr;

GThread::GThread() {
    // empty
}

/*static*/ void GThread::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

/*static*/ QThread* GThread::getCurrentThread() {
    return QThread::currentThread();
}

/*static*/ QThread* GThread::getQtMainThread() {
    return _qtMainThread;
}

/*static*/ QThread* GThread::getStudentThread() {
    return _studentThread;
}

/*static*/ bool GThread::iAmRunningOnTheQtGuiThread() {
    return _qtMainThread && _qtMainThread == QThread::currentThread();
}

/*static*/ bool GThread::iAmRunningOnTheStudentThread() {
    return _studentThread && _studentThread == QThread::currentThread();
}

/*static*/ bool GThread::qtGuiThreadExists() {
    return _qtMainThread != nullptr;
}

/*static*/ void GThread::runInNewThread(GThunk func, const std::string& threadName) {
    GFunctionThread* thread = new GFunctionThread(func, threadName);
    thread->start();
    while (!thread->isFinished()) {
        sleep(10);
    }
    delete thread;
}

/*static*/ QThread* GThread::runInNewThreadAsync(GThunk func, const std::string& threadName) {
    GFunctionThread* thread = new GFunctionThread(func, threadName);
    thread->start();
    return thread;
}

/*static*/ void GThread::runOnQtGuiThread(GThunk func) {
    // send timer "event" telling GUI thread what to do
    // TODO: enable
//    if (!_initialized) {
//        error("GThread::runOnQtGuiThread: Qt GUI system has not been initialized.\n"
//              "You must #include one of the \"q*.h\" files in your main program file.");
//    }
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        GEventQueue::instance()->runOnQtGuiThreadSync(func);
    } else {
        error("GThread::runOnQtGuiThread: Qt GUI thread has not been initialized properly. \n"
              "Make sure that the file containing your main() function #includes at least \n"
              "one .h header from the Stanford C++ library.");
    }
}

/*static*/ void GThread::runOnQtGuiThreadAsync(GThunk func) {
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        GEventQueue::instance()->runOnQtGuiThreadAsync(func);
    } else {
        error("GThread::runOnQtGuiThreadAsync: Qt GUI thread has not been initialized properly. \n"
              "Make sure that the file containing your main() function #includes at least \n"
              "one .h header from the Stanford C++ library.");
    }
}

/*static*/ void GThread::setMainThread() {
    if (!_qtMainThread) {
        _qtMainThread = QThread::currentThread();
        _qtMainThread->setObjectName("Qt GUI Thread");
    }
}

/*static*/ void GThread::sleep(double ms) {
    require::nonNegative(ms, "GThread::sleep", "delay (ms)");
    getCurrentThread()->msleep(static_cast<unsigned long>(ms));
}

/*static*/ bool GThread::studentThreadExists() {
    return _studentThread != nullptr;
}

/*static*/ bool GThread::wait(QThread* thread, long ms) {
    QThread* currentThread = getCurrentThread();
    if (currentThread == thread) {
        error("GThread::wait: a thread cannot wait for itself");
    }

    long startTime = GEvent::getCurrentTimeMS();
    unsigned long msToSleep = static_cast<unsigned long>(ms > 10 ? 10 : ms);
    while (thread && thread->isRunning()) {
        currentThread->msleep(msToSleep);

        // stop if we have waited at least the given amount of time
        if (ms > 0 && GEvent::getCurrentTimeMS() - startTime >= ms) {
            break;
        }
    }
    return thread->isRunning();
}

void GThread::yield() {
    QThread::yieldCurrentThread();
}


GStudentThread::GStudentThread(GThunkInt mainFunc)
        : _mainFunc(mainFunc),
          _mainFuncVoid(nullptr),
          _result(0) {
    this->setObjectName(QString::fromStdString("Main (student)"));
}

GStudentThread::GStudentThread(GThunk mainFunc)
        : _mainFunc(nullptr),
          _mainFuncVoid(mainFunc) {
    this->setObjectName(QString::fromStdString("Main (student)"));
}

int GStudentThread::getResult() const {
    return _result;
}

void GStudentThread::run() {
    yield();

    // perform any thread-specific initialization
    stanfordcpplib::initializeLibraryStudentThread();

    if (_mainFunc) {
        _result = _mainFunc();
    } else {
        _mainFuncVoid();
    }

    // briefly wait for the console to finish printing any/all output
    yield();
    pause(1);

    // if I get here, student's main() has finished running;
    // indicate this by showing a completed title on the graphical console
    if (getConsoleEnabled()) {
        GConsoleWindow* console = getConsoleWindow();
        if (console) {
            console->shutdown();
        }
    }
//    else {
//        // need to exit here else program will not terminate
//        QtGui::instance()->exitGraphics(_result);
//    }
}

/*static*/ void GStudentThread::startStudentThread(GThunkInt mainFunc) {
    if (!_studentThread) {
        _studentThread = new GStudentThread(mainFunc);
        _studentThread->start();
    }
}

/*static*/ void GStudentThread::startStudentThreadVoid(GThunk mainFunc) {
    if (!_studentThread) {
        _studentThread = new GStudentThread(mainFunc);
        _studentThread->start();
    }
}

/*static*/ bool GStudentThread::studentThreadExists() {
    return _studentThread != nullptr;
}
