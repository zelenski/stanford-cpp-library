/*
 * File: gthread.cpp
 * -----------------
 *
 * This file implements the members declared in gthread.h.
 *
 * @version 2019/04/13
 * - reimplement GThread to wrap either QThread or std::thread
 * - add GThread abstract base class for thread abstractions
 * - add GThreadQt and GThreadStd subclasses
 * - rename GFunctionThread to QFunctionThread to reduce name confusion
 * - remove GStudentThread subclass and combine functionality into GThread
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

#include "gthread.h"
#include "gevent.h"
#include "geventqueue.h"
#include "require.h"
#include <pthread.h>

// the native thread name seems to be what is shown in debugger
void native_set_thread_name(const char *name) {
#ifdef __APPLE__
    pthread_setname_np(name);
#elif defined _WIN32
    pthread_setname_np(pthread_self(), name);
#else
    pthread_setname_np(pthread_self(), name);
    // JDZ: believe same on linux as windows
#endif
}

void native_thread_exit() {
    pthread_exit(nullptr);
    // JDZ:  Mac+Windows, but also linux? Need test
}


QFunctionThread::QFunctionThread(GThunk func, const std::string & threadName)
        : _func(func),
          _hasReturn(false),
          _returnValue(0),
          _name(threadName) {
    // empty
}

QFunctionThread::QFunctionThread(GThunkInt func, const std::string & threadName)
        : _funcInt(func),
          _hasReturn(true),
          _returnValue(0),
          _name(threadName) {
    // empty
}

int QFunctionThread::returnValue() const {
    return _returnValue;
}

void QFunctionThread::run() {
    native_set_thread_name(_name.c_str()); // can only do this when running in the thread
    if (_hasReturn) {
        _returnValue = _funcInt();
    } else {
        _func();
    }
}

void QFunctionThread::start() {
    if (!_name.empty()) {
        setObjectName(QString::fromStdString(_name)); // have to do before start thread
    }
    QThread::start();
}


/*static*/ GThread* GThread::_qtGuiThread = nullptr;
/*static*/ GThread* GThread::_studentThread = nullptr;

GThread::GThread() {
    // empty
}

/*static*/ void GThread::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

/*static*/ bool GThread::iAmRunningOnTheQtGuiThread() {
    return QThread::isMainThread();
}

/*static*/ void GThread::msleep(double ms) {
    require::nonNegative(ms, "GThread::msleep", "delay (ms)");
    QThread::msleep(static_cast<unsigned long>(ms));
}

/*static*/ bool GThread::qtGuiThreadExists() {
    return _qtGuiThread != nullptr;
}

/*static*/ void GThread::runInNewThread(GThunk func, const std::string& threadName) {
    GThreadQt* thread = new GThreadQt(func, threadName);
    thread->start();
    while (thread->isRunning()) {
        GThread::msleep(10);
    }
    delete thread;
}

/*static*/ GThread* GThread::runInNewThreadAsync(GThunk func, const std::string& threadName) {
    GThreadQt* thread = new GThreadQt(func, threadName);
    thread->start();
    return thread;
}

/*static*/ void GThread::runOnQtGuiThread(GThunk func) {
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

/*static*/ void GThread::setCurrentThreadAsGuiThread() {
    if (!_qtGuiThread) {
        _qtGuiThread = new GThreadQt(QThread::currentThread(), "Qt GUI Thread"); // add GThreadQt wrapper
    }
}

/*static*/ void GThread::startStudentThread(GThunkInt mainFunc) {
    if (!_studentThread) {
        _studentThread = new GThreadQt(mainFunc, "Student main()");
        _studentThread->start();
    }
}

/*static*/ bool GThread::studentThreadExists() {
    return _studentThread != nullptr;
}

/*static*/ bool GThread::wait(GThread* thread, long ms) {
    long startTime = GEvent::getCurrentTimeMS();
    unsigned long msToSleep = static_cast<unsigned long>(ms > 10 ? 10 : ms);
    while (thread && thread->isRunning()) {
        GThread::msleep(msToSleep);

        // stop if we have waited at least the given amount of time
        if (ms > 0 && GEvent::getCurrentTimeMS() - startTime >= ms) {
            break;
        }
    }
    return thread->isRunning();
}

/*static*/ void GThread::yield() {
    QThread::yieldCurrentThread();
}


GThreadQt::GThreadQt(GThunk func, const std::string& threadName)
        : _qThread(nullptr) {
    _qThread = new QFunctionThread(func, threadName);
}

GThreadQt::GThreadQt(GThunkInt func, const std::string& threadName)
        : _qThread(nullptr) {
    _qThread = new QFunctionThread(func, threadName);
}

GThreadQt::GThreadQt(QThread* qthread, const std::string& threadName)
        : _qThread(qthread) {
    qthread->setObjectName(QString::fromStdString(threadName));
}

GThreadQt::~GThreadQt() {
    // TODO: delete _qThread;
    _qThread = nullptr;
}

bool GThreadQt::isRunning() const {
    return _qThread->isRunning();
}

void GThreadQt::join() {
    if (_qThread->isRunning()) {
        _qThread->wait();
    }
}

// Implementation note: This may be the wrong behavior.
// I think the current behavior tells this thread to pause itself,
// while the desired behavior is to have the calling thread wait for this thread.
bool GThreadQt::join(long ms) {
    require::nonNegative(ms, "GThread::join", "ms");
    return _qThread->wait(ms);
}

void GThreadQt::start() {
    _qThread->start();
}
