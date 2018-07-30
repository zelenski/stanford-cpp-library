/*
 * File: qgthread.h
 * ----------------
 *
 * @version 2018/07/28
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgthread_h
#define _qgthread_h

#include <QThread>
#include "qgtypes.h"

class QGStudentThread;

class QGThread : public QThread {
public:
    // empty
    static void ensureThatThisIsTheQtGuiThread(const std::string& message = "");
    static QThread* getCurrentThread();
    static QThread* getQtMainThread();
    static QThread* getStudentThread();
    static bool iAmRunningOnTheQtGuiThread();
    static bool iAmRunningOnTheStudentThread();
    static bool qtGuiThreadExists();
    static void runOnQtGuiThread(QGThunk func);
    static void runOnQtGuiThreadAsync(QGThunk func);
    static void setMainThread();
    static void sleep(double ms);
    static bool studentThreadExists();

protected:
    static QThread* _qtMainThread;
    static QThread* _studentThread;
};

class QGStudentThread : public QGThread {
public:
    static void startStudentThread(QGThunkInt mainFunc);
    static bool studentThreadExists();

    QGStudentThread(QGThunkInt mainFunc);
    int getResult() const;

protected:
    void run();

private:

    QGThunkInt _mainFunc;
    int _result;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgthread_h
#endif // SPL_QT_GUI
