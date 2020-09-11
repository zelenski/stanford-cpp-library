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
#include "consoletext.h"
#include "gevent.h"
#include "geventqueue.h"
#include "qtgui.h"
#include "require.h"
#include <chrono>
#include <pthread.h>

void native_set_thread_name(const char *name)
{
#ifdef __APPLE__
    pthread_setname_np(name);
#elif defined _WIN32
    pthread_setname_np(pthread_self(), name);
#else
    // ignored for other platforms
    // JDZ: likely same on linux as windows
    // but I don't have linux system to test to be sure
#endif
}

void native_thread_exit()
{
    pthread_exit(nullptr);
    // JDZ:  Mac+Windows, but also linux? Need test
}


QFunctionThread::QFunctionThread(GThunk func)
        : _func(func),
          _hasReturn(false),
          _returnValue(0) {
    // empty
}

QFunctionThread::QFunctionThread(GThunkInt func)
        : _funcInt(func),
          _hasReturn(true),
          _returnValue(0) {
    // empty
}

int QFunctionThread::returnValue() const {
    return _returnValue;
}

void QFunctionThread::run() {
    if (_hasReturn) {
        _returnValue = _funcInt();
    } else {
        _func();
    }
}


/*static*/ GThread* GThread::_qtGuiThread = nullptr;
/*static*/ GThread* GThread::_studentThread = nullptr;
Map<QThread*, GThread*> GThread::_allGThreadsQt;
Map<std::thread*, GThread*> GThread::_allGThreadsStd;

GThread::GThread() {
    // empty
}

/*static*/ void GThread::ensureThatThisIsTheQtGuiThread(const std::string& message) {
    if (!iAmRunningOnTheQtGuiThread()) {
        error((message.empty() ? "" : (message + ": "))
              + "Qt GUI system must be initialized from the application's main thread.");
    }
}

/*static*/ GThread* GThread::getCurrentThread() {
    QThread* currentQtThread = QThread::currentThread();
    if (_allGThreadsQt.containsKey(currentQtThread)) {
        return _allGThreadsQt[currentQtThread];
    } else {
        return new GThreadQt(currentQtThread);
    }
}

/*static*/ GThread* GThread::getQtGuiThread() {
    return _qtGuiThread;
}

/*static*/ GThread* GThread::getStudentThread() {
    return _studentThread;
}

/*static*/ bool GThread::iAmRunningOnTheQtGuiThread() {
    return _qtGuiThread && _qtGuiThread == getCurrentThread();
}

/*static*/ bool GThread::iAmRunningOnTheStudentThread() {
    return _studentThread && _studentThread == getCurrentThread();
}

/*static*/ bool GThread::qtGuiThreadExists() {
    return _qtGuiThread != nullptr;
}

/*static*/ void GThread::runInNewThread(GThunk func, const std::string& threadName) {
    GThread* currentThread = getCurrentThread();
    GThreadQt* thread = new GThreadQt(func, threadName);
    thread->start();
    while (thread->isRunning()) {
        currentThread->sleep(10);
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

/*static*/ void GThread::setGuiThread() {
    if (!_qtGuiThread) {
        _qtGuiThread = new GThreadQt(QThread::currentThread());
        _qtGuiThread->setName("Qt GUI Thread");
    }
}

/*static*/ void GThread::startStudentThread(GThunkInt mainFunc) {
    if (!_studentThread) {
        _studentThread = new GThreadStd(mainFunc, "Student main()");
        _studentThread->start();
    }
}

/*static*/ bool GThread::studentThreadExists() {
    return _studentThread != nullptr;
}

/*static*/ bool GThread::wait(GThread* thread, long ms) {
    GThread* currentThread = getCurrentThread();
    if (currentThread == thread) {
        error("GThread::wait: a thread cannot wait for itself");
    }

    long startTime = GEvent::getCurrentTimeMS();
    unsigned long msToSleep = static_cast<unsigned long>(ms > 10 ? 10 : ms);
    while (thread && thread->isRunning()) {
        currentThread->sleep(msToSleep);

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


GThreadQt::GThreadQt(GThunk func, const std::string& threadName)
        : _qThread(nullptr) {
    _func = func;
    _hasReturn = false;
    _returnValue = 0;
    _qThread = new QFunctionThread(func);
    if (!threadName.empty()) {
        setName(threadName);
    }
    _allGThreadsQt[_qThread] = this;
}

GThreadQt::GThreadQt(GThunkInt func, const std::string& threadName)
        : _qThread(nullptr) {
    _funcInt = func;
    _hasReturn = true;
    _returnValue = 0;
    _qThread = new QFunctionThread(func);
    if (!threadName.empty()) {
        setName(threadName);
    }
    _allGThreadsQt[_qThread] = this;
}

GThreadQt::GThreadQt(QThread* qthread)
        : _qThread(qthread) {
    _hasReturn = false;
    _returnValue = 0;
    _allGThreadsQt[_qThread] = this;
}

GThreadQt::~GThreadQt() {
    // TODO: delete _qThread;
    _allGThreadsQt.remove(_qThread);
    _qThread = nullptr;
}

int GThreadQt::getResult() const {
    return _returnValue;
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

std::string GThreadQt::name() const {
    return _qThread->objectName().toStdString();
}

int GThreadQt::priority() const {
    return static_cast<int>(_qThread->priority());
}

void GThreadQt::run() {
    if (_hasReturn) {
        _returnValue = _funcInt();
    } else {
        _func();
    }
}

void GThreadQt::setName(const std::string& name) {
    _qThread->setObjectName(QString::fromStdString(name));
}

void GThreadQt::setPriority(int priority) {
    QThread::Priority priorityEnum;
    switch (priority) {
        case 0: priorityEnum = QThread::IdlePriority; break;
        case 1: priorityEnum = QThread::LowestPriority; break;
        case 2: priorityEnum = QThread::LowPriority; break;
        case 3: priorityEnum = QThread::NormalPriority; break;
        case 4: priorityEnum = QThread::HighPriority; break;
        case 5: priorityEnum = QThread::HighestPriority; break;
        case 6: priorityEnum = QThread::TimeCriticalPriority; break;
        case 7: priorityEnum = QThread::InheritPriority; break;
        default: priorityEnum = QThread::NormalPriority; break;
    }
    _qThread->setPriority(priorityEnum);
}

void GThreadQt::sleep(double ms) {
    require::nonNegative(ms, "GThread::sleep", "delay (ms)");
    _qThread->msleep(static_cast<unsigned long>(ms));
}

void GThreadQt::start() {
    _qThread->start();
}

void GThreadQt::stop() {
    _qThread->terminate();   // note: don't call this if possible!
}

void GThreadQt::yield() {
    QThread::yieldCurrentThread();   // meh
}


GThreadStd::GThreadStd(GThunk func, const std::string& threadName)
        : _stdThread(nullptr) {
    _func = func;
    _hasReturn = false;
    _returnValue = 0;
    _running = false;
    if (!threadName.empty()) {
        setName(threadName);
    }
    _allGThreadsStd[_stdThread] = this;
}

GThreadStd::GThreadStd(GThunkInt func, const std::string& threadName)
        : _stdThread(nullptr) {
    _funcInt = func;
    _hasReturn = true;
    _returnValue = 0;
    _running = false;
    if (!threadName.empty()) {
        setName(threadName);
    }
    _allGThreadsStd[_stdThread] = this;
}

GThreadStd::GThreadStd(std::thread* stdThread)
        : _stdThread(stdThread) {
    _hasReturn = false;
    _returnValue = 0;
    _allGThreadsStd[_stdThread] = this;
}

GThreadStd::~GThreadStd() {
    // TODO: delete _stdThread;
    _allGThreadsStd.remove(_stdThread);
    _running = false;
    _stdThread = nullptr;
}

int GThreadStd::getResult() const {
    return _returnValue;
}

bool GThreadStd::isRunning() const {
    return _stdThread != nullptr && _running;
}

// Implementation note: This may be the wrong behavior.
// I think the current behavior tells this thread to pause itself,
// while the desired behavior is to have the calling thread wait for this thread.
void GThreadStd::join() {
    if (isRunning() && _stdThread->joinable()) {
        _stdThread->join();
    }
}

// Implementation note: This may be the wrong behavior.
// I think the current behavior tells this thread to pause itself,
// while the desired behavior is to have the calling thread wait for this thread.
bool GThreadStd::join(long ms) {
    require::nonNegative(ms, "GThread::join", "ms");
    long elapsed = 0;
    long amountToSleep = ms >= 50 ? 50 : ms;
    while (elapsed < ms && isRunning()) {
        sleep(amountToSleep);
        elapsed += amountToSleep;
    }
    return !isRunning();
}

std::string GThreadStd::name() const {
    return _name;
}

int GThreadStd::priority() const {
    return static_cast<int>(QThread::NormalPriority);
}



void GThreadStd::run() {
    native_set_thread_name(_name.c_str());
    // run the given function
    _running = true;
    if (_hasReturn) {
        _returnValue = _funcInt();
    } else {
        _func();
    }
    _running = false;
}

void GThreadStd::setName(const std::string& name) {
    _name = name;
}

void GThreadStd::setPriority(int /*priority*/) {
    // unsupported
}

void GThreadStd::sleep(double ms) {
    require::nonNegative(ms, "GThread::sleep", "delay (ms)");
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(ms)));
}

void GThreadStd::start() {
    // not needed; std::thread implicitly auto-starts on creation
    _stdThread = new std::thread([&] {
        run();
    });
}

void GThreadStd::stop() {
    // not supported
}

void GThreadStd::yield() {
    std::this_thread::yield();
}
