/*
 * File: gthread.h
 * ---------------
 *
 * This file contains code related to multithreading.
 * Qt requires at least two threads to run: a main Qt GUI thread,
 * and a separate student code thread.
 * The student's main() function runs in this latter student thread.
 * You can also run code in a new thread using the static method
 * GThread::runInNewThread or GThread::runInNewThreadAsync.
 *
 * @version 2019/04/13
 * - reimplement GThread to wrap either QThread or std::thread
 * - add GThread abstract base class for thread abstractions
 * - add GThreadQt and GThreadStd subclasses
 * - rename GFunctionThread to QFunctionThread to reduce name confusion
 * - remove GStudentThread subclass and combine functionality into GThread
 * @version 2018/10/18
 * - improved thread names
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gthread.h to replace Java version
 * @version 2018/07/28
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gthread_h
#define _gthread_h

#include <QThread>
#include <atomic>
#include <thread>

#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#undef INTERNAL_INCLUDE

class QtGui;

/**
 * A QFunctionThread is an object that runs a function in its own
 * thread of execution.
 *
 * You construct it, passing a void function to run as a parameter,
 * and then call its <code>run()</code> method to run that function in its
 * own thread.
 *
 * Clients generally do not need to access this class directly.
 * To use threads with our library, use the static methods
 * <code>GThread::runInNewThread</code> and
 * <code>GThread::runInNewThreadAsync</code>.
 * @private
 */
class QFunctionThread : public QThread {
public:
    /**
     * Constructs a new thread to execute.
     */
    QFunctionThread(GThunk func);

    /**
     * Constructs a new thread to execute.
     */
    QFunctionThread(GThunkInt func);

    /**
     * Returns the value returned by the function, if any, else 0.
     */
    int returnValue() const;

protected:
    /**
     * Executes the function passed to the constructor in its own thread.
     */
    void run();

private:
    Q_DISABLE_COPY(QFunctionThread)

    GThunk _func;
    GThunkInt _funcInt;
    bool _hasReturn;
    int _returnValue;
};

/**
 * The GThread class is a utility class containing static methods that allow
 * you to run code on various system threads.
 * The library has the following two standard threads running at all times:
 *
 * <ol>
 *     <li> The Qt GUI thread, which runs Qt's master exec() loop,
 *          handles all GUI object creation and events
 *          (this is technically the program's main thread)
 *
 *     <li> The student thread, which runs the student's main() function and
 *          any sub-functions called by main
 *
 * Students and clients normally do not need to worry about threading issues.
 * These methods are called internally by many of the graphical interactors
 * to make sure that all internal Qt GUI widgets are initialized on the Qt GUI
 * thread.  This is required for them to function properly.
 *
 * If you want to run a piece of code in its own thread, use static methods
 * <code>GThread::runInNewThread</code> and
 * <code>GThread::runInNewThreadAsync</code>.
 */
class GThread {
public:
    /**
     * Returns the value returned by the thread's function.
     * This will be 0 until the function is done running.
     * This method only has meaning if your thread executes a function that
     * returns an int.
     */
    virtual int getResult() const = 0;

    /**
     * Returns true if the given thread is currently actively running.
     */
    virtual bool isRunning() const = 0;

    /**
     * Waits for this thread to finish.
     * Will wait indefinitely as needed.
     */
    virtual void join() = 0;

    /**
     * Waits for this thread to finish.
     * Will wait up to the given number of milliseconds.
     * Returns true if the thread has finished or false if it is still running.
     */
    virtual bool join(long ms) = 0;

    /**
     * Returns the thread's name as passed to the constructor, or a default
     * name if none was passed.
     * Not all thread implementations support names.
     */
    virtual std::string name() const = 0;

    /**
     * Returns the thread's priority.
     * Threads with higher priorities tend to run more than ones that are lower.
     * Not all thread implementations support priorities.
     */
    virtual int priority() const = 0;

    /**
     * Sets the thread's name to the given value.
     * Not all thread implementations support names.
     */
    virtual void setName(const std::string& name) = 0;

    /**
     * Sets the thread's priority to the given value.
     * Not all thread implementations support priorities.
     */
    virtual void setPriority(int priority) = 0;

    /**
     * Causes the thread to pause itself for the given number of milliseconds.
     * @throw ErrorException if ms is negative
     */
    virtual void sleep(double ms) = 0;

    /**
     * Tells the thread to start running.
     */
    virtual void start() = 0;

    /**
     * Forcibly terminates the thread.
     * You probably should not call this unless absolutely necessary,
     * since it can lead to messed up state in the program.
     */
    virtual void stop() = 0;

    /**
     * Indicates that the current thread is willing to yield execution to any
     * other threads that want to run.
     * This differs slightly from sleep() in that sleep() mandates to pause the
     * current thread for a given amount of time, while yield() is more of an
     * offer to other threads that they may run now if they so choose.
     */
    virtual void yield() = 0;

    // TODO: methods to set a top-level exception handler


    /**
     * Generates an error if the caller is not running on the Qt GUI main thread.
     * An optional error detail message can be passed.
     */
    static void ensureThatThisIsTheQtGuiThread(const std::string& message = "");

    /**
     * Returns the caller's Qt thread object.
     */
    static GThread* getCurrentThread();

    /**
     * Returns the Qt thread object representing the main thread for the
     * application, also referred to as the Qt GUI thread.
     */
    static GThread* getQtMainThread();

    /**
     * Returns the Qt thread object representing the thread on which the
     * student's main() function runs.
     */
    static GThread* getStudentThread();

    /**
     * Returns true if the caller is running on the Qt GUI thread.
     */
    static bool iAmRunningOnTheQtGuiThread();

    /**
     * Returns true if the caller is running on the student thread.
     */
    static bool iAmRunningOnTheStudentThread();

    /**
     * Returns true if the Qt GUI thread has been created.
     * This will happen right before the student's main() function runs.
     */
    static bool qtGuiThreadExists();

    /**
     * Runs the given void function in its own new thread,
     * blocking the current thread to wait until it is done.
     * You can pass an optional name for the thread which can help when looking
     * through the list of threads in a debugger.
     *
     * Any uncaught exceptions or errors in the new thread will crash the
     * program and cannot be caught by the calling thread.
     *
     * If you want the new thread to run in the background,
     * use the <code>runInNewThreadAsync</code> function instead.
     */
    static void runInNewThread(GThunk func, const std::string& threadName = "");

    /**
     * Runs the given void function in its own new thread in the background;
     * the current thread does not block and keeps going.
     * You can pass an optional name for the thread which can help when looking
     * through the list of threads in a debugger.
     * Returns a pointer to the given thread in case you want to wait a given
     * amount of time for the thread to do its work.
     *
     * Any uncaught exceptions or errors in the new thread will crash the
     * program and cannot be caught by the calling thread.
     *
     * If you want the caller to wait for the new thread to finish running,
     * use the <code>runInNewThread</code> function instead.
     */
    static GThread* runInNewThreadAsync(GThunk func, const std::string& threadName = "");

    /**
     * Runs the given void function on the Qt GUI thread,
     * blocking the current thread to wait until it is done.
     * This function is called heavily by the internal GUI widgets and
     * interactors of the library, because all Qt GUI operations are required
     * to be done on the application's main thread.
     *
     * Any uncaught exceptions or errors in the Qt GUI thread will crash the
     * program and cannot be caught by the calling thread.
     *
     * If you want the new thread to run in the background,
     * use the <code>runOnQtGuiThreadAsync</code> function instead.
     */
    static void runOnQtGuiThread(GThunk func);

    /**
     * Runs the given void function on the Qt GUI thread in the background;
     * the current thread does not block and keeps going.
     *
     * Any uncaught exceptions or errors in the Qt GUI thread will crash the
     * program and cannot be caught by the calling thread.
     *
     * If you want the caller to wait for the Qt GUI thread code to finish running,
     * use the <code>runOnQtGuiThread</code> function instead.
     */
    static void runOnQtGuiThreadAsync(GThunk func);

    /**
     * Starts the student's thread, telling it to run the given function,
     * which accepts no arguments and returns an int.
     */
    static void startStudentThread(GThunkInt mainFunc);

    /**
     * Starts the student's thread, telling it to run the given function,
     * which accepts no arguments and returns void.
     */
    static void startStudentThreadVoid(GThunk mainFunc);

    /**
     * Returns true if the student's thread has already been created.
     */
    static bool studentThreadExists();

    /**
     * Waits the given number of milliseconds for the given thread to finish.
     * @return true if the entire amount of ms was elapsed without the thread finishing
     */
    static bool wait(GThread* thread, long ms);

protected:
    // forbid construction
    GThread();
    virtual ~GThread() = default;

    virtual void run() = 0;

    // member variables
    GThunk _func;
    GThunkInt _funcInt;
    bool _hasReturn;
    int _returnValue;

    // pointers to the two core library threads
    static GThread* _qtMainThread;
    static GThread* _studentThread;

    // mapping between QThreads and their related GThread wrappings
    static Map<QThread*, GThread*> _allGThreadsQt;
    static Map<std::thread*, GThread*> _allGThreadsStd;

private:
    /**
     * Sets the current thread to be the "main" thread for the application.
     * This is called by our library initialization code to inform the GThread
     * class what thread is the main thread.
     * Clients do not need to call this method directly.
     * @private
     */
    static void setMainThread();

    friend class QtGui;
    friend void stanfordcpplib::initializeLibrary(int argc, char** argv);
};


/**
 * A GThreadQt is an object that runs a function in its own
 * Qt thread of execution.
 * You construct it, passing a void function to run as a parameter,
 * and then call its <code>run()</code> method to run that function in its
 * own thread.
 *
 * This is provided so that you don't need to subclass QThread yourself
 * just to run a given piece of code.
 *
 * Clients generally do not need to access this class directly.
 * To use threads with our library, use the static methods
 * <code>GThread::runInNewThread</code> and
 * <code>GThread::runInNewThreadAsync</code>.
 * @private
 */
class GThreadQt : public GThread {
public:
    /**
     * Constructs a new thread to execute, with an optional thread name.
     */
    GThreadQt(GThunk func, const std::string& threadName = "");

    /**
     * Constructs a new thread to execute, with an optional thread name.
     */
    GThreadQt(GThunkInt func, const std::string& threadName = "");

    /**
     * Constructs a new thread to wrap the given existing Qt thread.
     */
    GThreadQt(QThread* qthread);

    virtual ~GThreadQt();

    /* @inherit */
    virtual int getResult() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual bool isRunning() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void join() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual bool join(long ms) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string name() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual int priority() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setName(const std::string& name) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setPriority(int priority) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void sleep(double ms) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void start() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void stop() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void yield() Q_DECL_OVERRIDE;

protected:
    /* @inherit */
    virtual void run() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(GThreadQt)

    QThread* _qThread;   // underlying real Qt thread
};


/**
 * A GThreadQt is an object that runs a function in its own
 * std::thread thread of execution.
 * You construct it, passing a void function to run as a parameter,
 * and then call its <code>run()</code> method to run that function in its
 * own thread.
 *
 * This is provided so that you don't need to subclass QThread yourself
 * just to run a given piece of code.
 *
 * Clients generally do not need to access this class directly.
 * To use threads with our library, use the static methods
 * <code>GThread::runInNewThread</code> and
 * <code>GThread::runInNewThreadAsync</code>.
 * @private
 */
class GThreadStd : public GThread {
public:
    /**
     * Constructs a new thread to execute, with an optional thread name.
     */
    GThreadStd(GThunk func, const std::string& threadName = "");

    /**
     * Constructs a new thread to execute, with an optional thread name.
     */
    GThreadStd(GThunkInt func, const std::string& threadName = "");

    /**
     * Constructs a new thread to wrap the given existing Qt thread.
     */
    GThreadStd(std::thread* stdThread);

    virtual ~GThreadStd();

    /* @inherit */
    virtual int getResult() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual bool isRunning() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void join() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual bool join(long ms) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string name() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual int priority() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setName(const std::string& name) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setPriority(int priority) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void sleep(double ms) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void start() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void stop() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void yield() Q_DECL_OVERRIDE;

protected:
    /* @inherit */
    virtual void run() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(GThreadStd)

    std::thread* _stdThread;   // underlying real Qt thread
    std::string _name;
    std::atomic<bool> _running;
};

#endif // _gthread_h
