/*
 * File: qgui.cpp
 * --------------
 *
 * @version 2018/07/03
 * - initial version
 */

#include "qgui.h"
#include <QEvent>
#include <QThread>
#include "error.h"
#include "qgwindow.h"
#include "strlib.h"

QGStudentThread::QGStudentThread(int (* mainFunc)(void))
        : _mainFunc(mainFunc) {
    // empty
}

int QGStudentThread::getResult() const {
    return _result;
}

void QGStudentThread::run() {
    this->msleep(200);
    _result = _mainFunc();
}


QGuiEventQueue* QGuiEventQueue::_instance = nullptr;

QGuiEventQueue::QGuiEventQueue() {
    // empty
}

QGThunk QGuiEventQueue::dequeue() {
    _queueMutex.lock();
    QGThunk thunk = _functionQueue.dequeue();
    _queueMutex.unlock();
    return thunk;
}

QGuiEventQueue* QGuiEventQueue::instance() {
    if (!_instance) {
        _instance = new QGuiEventQueue();
    }
    return _instance;
}

bool QGuiEventQueue::isEmpty() const {
    return _functionQueue.isEmpty();
}

QGThunk QGuiEventQueue::peek() {
    _queueMutex.lock();
    QGThunk thunk = _functionQueue.peek();
    _queueMutex.unlock();
    return thunk;
}

void QGuiEventQueue::runOnQtGuiThreadAsync(QGThunk thunk) {
    // cout << "QGuiEventQueue::runOnQtGuiThreadAsync: my current thread is " << QGui::instance()->getCurrentThread() << endl;
    _queueMutex.lock();
    _functionQueue.add(thunk);
    _queueMutex.unlock();
    emit mySignal();
}

void QGuiEventQueue::runOnQtGuiThreadSync(QGThunk thunk) {
    // cout << "QGuiEventQueue::runOnQtGuiThreadSync: my current thread is " << QGui::instance()->getCurrentThread() << endl;
    _queueMutex.lock();
    _functionQueue.add(thunk);
    _queueMutex.unlock();
    emit mySignal();

    // TODO: "empty" is not quite right condition
    while (true) {
        _queueMutex.lock();
        bool empty = _functionQueue.isEmpty();
        _queueMutex.unlock();
        if (empty) {
            break;
        } else {
            QGui::instance()->getCurrentThread()->msleep(50);
        }
    }
}


QApplication* QGui::_app = nullptr;
QThread* QGui::_qtMainThread = nullptr;
QGStudentThread* QGui::_studentThread = nullptr;
int QGui::_argc = 0;
char** QGui::_argv = nullptr;
QGui* QGui::_instance = nullptr;

QGui::QGui() {
    connect(QGuiEventQueue::instance(),
            SIGNAL(mySignal()),
            this,
            SLOT(mySlot()));
}

void QGui::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    QThread* currentThread = QThread::currentThread();
    if (currentThread != _qtMainThread) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

QThread* QGui::getCurrentThread() {
    return QThread::currentThread();
}

QThread* QGui::getQtMainThread() {
    return _qtMainThread;
}

QThread* QGui::getStudentThread() {
    return _studentThread;
}

void QGui::initializeQt() {
    if (!_app) {
        _app = new QApplication(_argc, _argv);
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
    // cout << "QGui::mySlot:: my thread is " << QThread::currentThread() << endl;
    if (!QGuiEventQueue::instance()->isEmpty()) {
        QGThunk thunk = QGuiEventQueue::instance()->peek();
        // cout << "QGui::mySlot:: about to run thunk" << endl;
        thunk();
        // cout << "QGui::mySlot:: done running thunk" << endl;
        QGuiEventQueue::instance()->dequeue();
    }
}

void QGui::runOnQtGuiThread(QGThunk func) {
    // send timer "event" telling GUI thread what to do
//    QEvent* event = new QEvent((QEvent::Type) (QEvent::User + 106));
//    QCoreApplication::postEvent(QGWindow::getLastWindow(), event);
    QGuiEventQueue::instance()->runOnQtGuiThreadSync(func);
}

void QGui::runOnQtGuiThreadAsync(QGThunk func) {
    QGuiEventQueue::instance()->runOnQtGuiThreadAsync(func);
}

// this should be called by the Qt main thread
void QGui::startBackgroundEventLoop(int (* mainFunc)(void)) {
    // TODO: make it possible to stop;  exitGraphics?

    // start student's main function in its own second thread
    if (!_qtMainThread && !_studentThread) {
        _qtMainThread = QThread::currentThread();
        _studentThread = new QGStudentThread(mainFunc);
        _studentThread->start();

        // begin Qt event loop on main thread
        startEventLoop();
    }
}

// this should be called by the Qt main thread
void QGui::startEventLoop() {
    initializeQt();
    _app->exec();   // start Qt event loop on main thread; blocks
}

#define __DONT_ENABLE_GRAPHICAL_CONSOLE
#include "console.h"
#undef __DONT_ENABLE_GRAPHICAL_CONSOLE

static int (* _mainFunc)(void);

// to be run in Qt thread
int __initializeQtAndRunMainFunc() {
    QGui::instance()->initializeQt();
    return _mainFunc();
}

void __initializeStanfordCppLibraryQt(int argc, char** argv, int (* mainFunc)(void)) {
    // ensure that library is initialized only once
    static bool _initialized = false;
    if (_initialized) {
        return;
    }
    _initialized = true;
    QGui::_argc = argc;
    QGui::_argv = argv;

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    // TODO: remove/change to Qt graphical console
    static std::ios_base::Init ios_base_init;

#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    // TODO: remove/change to Qt graphical console
    setConsolePrintExceptions(true);
#endif

    // TODO: remove
    extern void initPipe();
    // initPipe();

    // set up student's main function
    _mainFunc = mainFunc;
    QGui::instance()->startBackgroundEventLoop(__initializeQtAndRunMainFunc);
}

void __shutdownStanfordCppLibraryQt() {
    // TODO
//    const std::string PROGRAM_COMPLETED_TITLE_SUFFIX = " [completed]";


//    if (getConsoleEnabled()) {
//        std::string title = getConsoleWindowTitle();
//        if (title == "") {
//            title = "Console";
//        }
//        if (title.find("terminated") == std::string::npos) {
//            ::setConsoleWindowTitle(title + PROGRAM_COMPLETED_TITLE_SUFFIX);
//        }
//    }
}
