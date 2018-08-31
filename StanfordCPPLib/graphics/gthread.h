/*
 * File: gthread.h
 * ---------------
 *
 * @version 2018/08/23
 * - renamed to gthread.h to replace Java version
 * @version 2018/07/28
 * - initial version
 */

#ifndef _gthread_h
#define _gthread_h

#include <QThread>
#include "gtypes.h"

class GStudentThread;

class GFunctionThread : public QThread {
public:
    GFunctionThread(GThunk func);

protected:
    void run();

private:
    GThunk _func;
};


class GThread : public QThread {
public:
    // empty
    static void ensureThatThisIsTheQtGuiThread(const std::string& message = "");
    static QThread* getCurrentThread();
    static QThread* getQtMainThread();
    static QThread* getStudentThread();
    static bool iAmRunningOnTheQtGuiThread();
    static bool iAmRunningOnTheStudentThread();
    static bool qtGuiThreadExists();
    static void runInNewThread(GThunk func);
    static void runInNewThreadAsync(GThunk func);
    static void runOnQtGuiThread(GThunk func);
    static void runOnQtGuiThreadAsync(GThunk func);
    static void setMainThread();
    static void sleep(double ms);
    static bool studentThreadExists();
    static void yield();

protected:
    static QThread* _qtMainThread;
    static QThread* _studentThread;
};


class GStudentThread : public GThread {
public:
    static void startStudentThread(GThunkInt mainFunc);
    static void startStudentThreadVoid(GThunk mainFunc);
    static bool studentThreadExists();

    GStudentThread(GThunk mainFunc);
    GStudentThread(GThunkInt mainFunc);
    int getResult() const;

protected:
    void run();

private:
    GThunkInt _mainFunc;
    GThunk _mainFuncVoid;
    int _result;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gthread_h
