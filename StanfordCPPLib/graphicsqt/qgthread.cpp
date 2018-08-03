/*
 * File: qgthread.cpp
 * ------------------
 *
 * @version 2018/07/28
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgthread.h"
#include "qgeventqueue.h"

QGFunctionThread::QGFunctionThread(QGThunk func)
        : _func(func) {
    // empty
}

void QGFunctionThread::run() {
    _func();
}


QThread* QGThread::_qtMainThread = nullptr;
QThread* QGThread::_studentThread = nullptr;

void QGThread::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

QThread* QGThread::getCurrentThread() {
    return QThread::currentThread();
}

QThread* QGThread::getQtMainThread() {
    return _qtMainThread;
}

QThread* QGThread::getStudentThread() {
    return _studentThread;
}

bool QGThread::iAmRunningOnTheQtGuiThread() {
    return _qtMainThread && _qtMainThread == QThread::currentThread();
}

bool QGThread::iAmRunningOnTheStudentThread() {
    return _studentThread && _studentThread == QThread::currentThread();
}

bool QGThread::qtGuiThreadExists() {
    return _qtMainThread != nullptr;
}

void QGThread::runInNewThread(QGThunk func) {
    QGFunctionThread thread(func);
    thread.start();
    while (!thread.isFinished()) {
        sleep(10);
    }
}

void QGThread::runInNewThreadAsync(QGThunk func) {
    QGFunctionThread thread(func);
    thread.start();
}

void QGThread::runOnQtGuiThread(QGThunk func) {
    // send timer "event" telling GUI thread what to do
    // TODO: enable
//    if (!_initialized) {
//        error("QGThread::runOnQtGuiThread: Qt GUI system has not been initialized.\n"
//              "You must #include one of the \"q*.h\" files in your main program file.");
//    }
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        QGuiEventQueue::instance()->runOnQtGuiThreadSync(func);
    } else {
        error("QGui::runOnQtGuiThread: Qt GUI thread no longer exists");
    }
}

void QGThread::runOnQtGuiThreadAsync(QGThunk func) {
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        QGuiEventQueue::instance()->runOnQtGuiThreadAsync(func);
    } else {
        error("QGui::runOnQtGuiThreadAsync: Qt GUI thread no longer exists");
    }
}

void QGThread::setMainThread() {
    if (!_qtMainThread) {
        _qtMainThread = QThread::currentThread();
    }
}

void QGThread::sleep(double ms) {
    getCurrentThread()->msleep((long) ms);
}

bool QGThread::studentThreadExists() {
    return _studentThread != nullptr;
}

void QGThread::yield() {
    QThread::yieldCurrentThread();
}


QGStudentThread::QGStudentThread(QGThunkInt mainFunc)
        : _mainFunc(mainFunc) {
    this->setObjectName(QString::fromStdString("QGStudentThread"));
}

int QGStudentThread::getResult() const {
    return _result;
}

void QGStudentThread::run() {
    yield();
    _result = _mainFunc();

    // if I get here, student's main() has finished running;
    // indicate this by showing a new title on the graphical console
    runOnQtGuiThreadAsync([]() {
        // flush out any unwritten output to the standard output streams
        std::cout.flush();
        std::cerr.flush();

        extern void __shutdownStanfordCppLibraryQt();
        __shutdownStanfordCppLibraryQt();
    });
}

void QGStudentThread::startStudentThread(QGThunkInt mainFunc) {
    if (!_studentThread) {
        _studentThread = new QGStudentThread(mainFunc);
        _studentThread->start();
    }
}

bool QGStudentThread::studentThreadExists() {
    return _studentThread != nullptr;
}

#endif // SPL_QT_GUI
