/*
 * File: gthread.cpp
 * -----------------
 *
 * @version 2018/08/23
 * - renamed to gthread.h to replace Java version
 * @version 2018/07/28
 * - initial version
 */

#include "gthread.h"
#include "geventqueue.h"

GFunctionThread::GFunctionThread(GThunk func)
        : _func(func) {
    // empty
}

void GFunctionThread::run() {
    _func();
}


QThread* GThread::_qtMainThread = nullptr;
QThread* GThread::_studentThread = nullptr;

void GThread::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

QThread* GThread::getCurrentThread() {
    return QThread::currentThread();
}

QThread* GThread::getQtMainThread() {
    return _qtMainThread;
}

QThread* GThread::getStudentThread() {
    return _studentThread;
}

bool GThread::iAmRunningOnTheQtGuiThread() {
    return _qtMainThread && _qtMainThread == QThread::currentThread();
}

bool GThread::iAmRunningOnTheStudentThread() {
    return _studentThread && _studentThread == QThread::currentThread();
}

bool GThread::qtGuiThreadExists() {
    return _qtMainThread != nullptr;
}

void GThread::runInNewThread(GThunk func) {
    GFunctionThread thread(func);
    thread.start();
    while (!thread.isFinished()) {
        sleep(10);
    }
}

void GThread::runInNewThreadAsync(GThunk func) {
    GFunctionThread thread(func);
    thread.start();
}

void GThread::runOnQtGuiThread(GThunk func) {
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

void GThread::runOnQtGuiThreadAsync(GThunk func) {
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

void GThread::setMainThread() {
    if (!_qtMainThread) {
        _qtMainThread = QThread::currentThread();
    }
}

void GThread::sleep(double ms) {
    getCurrentThread()->msleep((long) ms);
}

bool GThread::studentThreadExists() {
    return _studentThread != nullptr;
}

void GThread::yield() {
    QThread::yieldCurrentThread();
}


GStudentThread::GStudentThread(GThunkInt mainFunc)
        : _mainFunc(mainFunc),
          _mainFuncVoid(nullptr) {
    this->setObjectName(QString::fromStdString("GStudentThread"));
}

GStudentThread::GStudentThread(GThunk mainFunc)
        : _mainFunc(nullptr),
          _mainFuncVoid(mainFunc) {
    this->setObjectName(QString::fromStdString("GStudentThread"));
}

int GStudentThread::getResult() const {
    return _result;
}

void GStudentThread::run() {
    yield();
    if (_mainFunc) {
        _result = _mainFunc();
    } else {
        _mainFuncVoid();
    }

    // if I get here, student's main() has finished running;
    // indicate this by showing a new title on the graphical console
    runOnQtGuiThreadAsync([]() {
        // flush out any unwritten output to the standard output streams
        std::cout.flush();
        std::cerr.flush();
    });
}

void GStudentThread::startStudentThread(GThunkInt mainFunc) {
    if (!_studentThread) {
        _studentThread = new GStudentThread(mainFunc);
        _studentThread->start();
    }
}

void GStudentThread::startStudentThreadVoid(GThunk mainFunc) {
    if (!_studentThread) {
        _studentThread = new GStudentThread(mainFunc);
        _studentThread->start();
    }
}

bool GStudentThread::studentThreadExists() {
    return _studentThread != nullptr;
}
