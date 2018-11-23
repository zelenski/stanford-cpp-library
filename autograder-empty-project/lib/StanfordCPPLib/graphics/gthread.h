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

#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#undef INTERNAL_INCLUDE

class GStudentThread;
class QtGui;

/**
 * A GFunctionThread is an object that runs a function in its own
 * thread of execution.
 * You construct it, passing a void function to run as a parameter,
 * and then call its <code>run()</code> method to run that function in its
 * own thread.
 * Clients generally do not need to access this class directly.
 * To use threads with our library, use the static methods
 * <code>GThread::runInNewThread</code> and
 * <code>GThread::runInNewThreadAsync</code>.
 * @private
 */
class GFunctionThread : public QThread {
public:
    /**
     * Constructs a new thread to execute, with an optional thread name.
     */
    GFunctionThread(GThunk func, const std::string& threadName = "");

protected:
    /**
     * Executes the function passed to the constructor in its own thread.
     */
    void run();

private:
    Q_DISABLE_COPY(GFunctionThread)

    GThunk _func;
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
class GThread : public QThread {
public:
    /**
     * Generates an error if the caller is not running on the Qt GUI main thread.
     * An optional error detail message can be passed.
     */
    static void ensureThatThisIsTheQtGuiThread(const std::string& message = "");

    /**
     * Returns the caller's Qt thread object.
     */
    static QThread* getCurrentThread();

    /**
     * Returns the Qt thread object representing the main thread for the
     * application, also referred to as the Qt GUI thread.
     */
    static QThread* getQtMainThread();

    /**
     * Returns the Qt thread object representing the thread on which the
     * student's main() function runs.
     */
    static QThread* getStudentThread();

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
    static QThread* runInNewThreadAsync(GThunk func, const std::string& threadName = "");

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
     * Causes the current thread to pause itself for the given number of milliseconds.
     * @throw ErrorException if ms is negative
     */
    static void sleep(double ms);

    /**
     * Waits the given number of milliseconds for the given thread to finish.
     * @return true if the entire amount of ms was elapsed without the thread finishing
     */
    static bool wait(QThread* thread, long ms);

    /**
     * Indicates that the current thread is willing to yield execution to any
     * other threads that want to run.
     * This differs slightly from sleep() in that sleep() mandates to pause the
     * current thread for a given amount of time, while yield() is more of an
     * offer to other threads that they may run now if they so choose.
     */
    static void yield();

protected:
    // pointers to the two core library threads
    static QThread* _qtMainThread;
    static QThread* _studentThread;

    // forbid construction
    GThread();

private:
    /**
     * Sets the current thread to be the "main" thread for the application.
     * This is called by our library initialization code to inform the GThread
     * class what thread is the main thread.
     * Clients do not need to call this method directly.
     * @private
     */
    static void setMainThread();

    /**
     * Returns true if the student thread has been initialized already.
     * Clients do not need to call this method directly.
     * @private
     */
    static bool studentThreadExists();

    friend class QtGui;
    friend void stanfordcpplib::initializeLibrary(int argc, char** argv);
};


/**
 * This class is used to manage and initialize the student's "main" thread
 * that runs their main() function.
 * Clients should not need to access this class directly.
 * @private
 */
class GStudentThread : public GThread {
protected:
    void run();

private:
    Q_DISABLE_COPY(GStudentThread)

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
     * Constructs a new student thread to run the given void function.
     */
    GStudentThread(GThunk mainFunc);

    /**
     * Constructs a new student thread to run the given function,
     * which returns an int.
     */
    GStudentThread(GThunkInt mainFunc);

    /**
     * Returns the int returned by the main function passed to the constructor,
     * if any.  If the void version was used, returns 0.
     */
    int getResult() const;

    GThunkInt _mainFunc;
    GThunk _mainFuncVoid;
    int _result;

    friend class QtGui;
};

#endif // _gthread_h
