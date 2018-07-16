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
#include "exceptions.h"
#include "qgwindow.h"
#include "strlib.h"

#ifdef _WIN32
#  include <direct.h>   // for chdir
#else // _WIN32
#  include <unistd.h>   // for chdir
#endif // _WIN32

QGStudentThread::QGStudentThread(QGThunkInt mainFunc)
        : _mainFunc(mainFunc) {
    this->setObjectName(QString::fromStdString("QGStudentThread"));
}

int QGStudentThread::getResult() const {
    return _result;
}

void QGStudentThread::run() {
    this->yieldCurrentThread();
    _result = _mainFunc();

    // if I get here, student's main() has finished running;
    // indicate this by showing a new title on the graphical console
    QGui::instance()->runOnQtGuiThreadAsync([]() {
        // flush out any unwritten output to the standard output streams
        std::cout.flush();
        std::cerr.flush();

        extern void __shutdownStanfordCppLibraryQt();
        __shutdownStanfordCppLibraryQt();
    });
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
    _queueMutex.lock();
    _functionQueue.add(thunk);
    _queueMutex.unlock();
    emit mySignal();
}

void QGuiEventQueue::runOnQtGuiThreadSync(QGThunk thunk) {
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


// QGui members
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
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
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

QThread* QGui::getCurrentThread() {
    return QThread::currentThread();
}

QThread* QGui::getQtMainThread() {
    return _qtMainThread;
}

QThread* QGui::getStudentThread() {
    return _studentThread;
}

bool QGui::iAmRunningOnTheQtGuiThread() {
    return _qtMainThread && _qtMainThread == QThread::currentThread();
}

bool QGui::iAmRunningOnTheStudentThread() {
    return _studentThread && _studentThread == QThread::currentThread();
}

void QGui::initializeQt() {
    ensureThatThisIsTheQtGuiThread("QGui::initializeQt");
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
    if (!QGuiEventQueue::instance()->isEmpty()) {
        QGThunk thunk = QGuiEventQueue::instance()->peek();
        thunk();
        QGuiEventQueue::instance()->dequeue();
    }
}

bool QGui::qtGuiThreadExists() {
    return _qtMainThread != nullptr;
}

void QGui::runOnQtGuiThread(QGThunk func) {
    // send timer "event" telling GUI thread what to do
//    QEvent* event = new QEvent((QEvent::Type) (QEvent::User + 106));
//    QCoreApplication::postEvent(QGWindow::getLastWindow(), event);
    if (iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (qtGuiThreadExists()) {
        QGuiEventQueue::instance()->runOnQtGuiThreadSync(func);
    } else {
        error("QGui::runOnQtGuiThread: Qt GUI thread no longer exists");
    }
}

void QGui::runOnQtGuiThreadAsync(QGThunk func) {
    if (QGui::instance()->iAmRunningOnTheQtGuiThread()) {
        // already on Qt GUI thread; just run the function!
        func();
    } else if (QGui::instance()->qtGuiThreadExists()) {
        QGuiEventQueue::instance()->runOnQtGuiThreadAsync(func);
    } else {
        error("QGui::runOnQtGuiThreadAsync: Qt GUI thread no longer exists");
    }
}

// this should be called by the Qt main thread
void QGui::startBackgroundEventLoop(QGThunkInt mainFunc) {
    ensureThatThisIsTheQtGuiThread("QGui::startBackgroundEventLoop");

    // start student's main function in its own second thread
    if (!_studentThread) {
        _studentThread = new QGStudentThread(mainFunc);
        _studentThread->start();

        // begin Qt event loop on main thread
        startEventLoop();
    }
}

// this should be called by the Qt main thread
void QGui::startEventLoop() {
    ensureThatThisIsTheQtGuiThread("QGui::startEventLoop");
    if (!_app) {
        error("QGui::startEventLoop: need to initialize Qt first");
    }
    int exitCode = _app->exec();   // start Qt event loop on main thread; blocks

    // if I get here, it means an "exit on close" window was just closed;
    // it's time to shut down the Qt system and exit the C++ program
    exitGraphics(exitCode);
}

bool QGui::studentThreadExists() {
    return _studentThread != nullptr;
}

#define __DONT_ENABLE_QT_GRAPHICAL_CONSOLE
#include "qconsole.h"
#undef __DONT_ENABLE_QT_GRAPHICAL_CONSOLE

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
        extern void ensureConsoleWindow();
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

    QGui::_qtMainThread = QThread::currentThread();
    QGui::_argc = argc;
    QGui::_argv = argv;
    __parseArgsQt(argc, argv);

    QGui::instance()->initializeQt();

    // declaring this object ensures that std::cin, cout, cerr are initialized
    // properly before our lib tries to mess with them / redirect them
    // TODO: remove/change to Qt graphical console
    static std::ios_base::Init ios_base_init;

    // initialize Qt graphical console
    if (::stanfordcpplib::qtgui::getConsoleEnabled()) {
        ::stanfordcpplib::qtgui::ensureConsoleWindow();
    }


#if defined(SPL_CONSOLE_PRINT_EXCEPTIONS)
    ::stanfordcpplib::qtgui::setConsolePrintExceptions(true);
#endif

    // TODO: remove
    // extern void initPipe();
    // initPipe();

    // set up student's main function
    _mainFunc = mainFunc;
    QGui::instance()->startBackgroundEventLoop(mainFunc);
}

// to be run in Qt main thread
void __shutdownStanfordCppLibraryQt() {
    // TODO
    const std::string PROGRAM_COMPLETED_TITLE_SUFFIX = " [completed]";

    if (stanfordcpplib::qtgui::getConsoleEnabled()) {
        std::string title = stanfordcpplib::qtgui::getConsoleWindowTitle();
        if (title == "") {
            title = "Console";
        }
        if (title.find("terminated") == std::string::npos) {
            stanfordcpplib::qtgui::setConsoleWindowTitle(title + PROGRAM_COMPLETED_TITLE_SUFFIX);
        }
    }
}
