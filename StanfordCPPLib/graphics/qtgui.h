/*
 * File: qtgui.h
 * -------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to qtgui.h
 * @version 2018/07/03
 * - initial version
 */

#ifndef _qtgui_h
#define _qtgui_h

#include <functional>
#include <string>
#include <QApplication>
#include <QObject>
#include <QReadWriteLock>
#include "geventqueue.h"
#include "gthread.h"
#include "gtypes.h"
#include "queue.h"

/*
 * ...
 */
class QtGui : public QObject {
    Q_OBJECT

public:
    void exitGraphics(int exitCode = 0);
    void initializeQt();
    static QtGui* instance();
    void setArgs(int argc, char** argv);
    void startBackgroundEventLoop(GThunkInt mainFunc);
    void startBackgroundEventLoopVoid(GThunk mainFunc);
    void startEventLoop();

public slots:
    void mySlot();

signals:
    void mySignal();

private:
    QtGui();   // forbid construction

    bool _initialized;
    int _argc;
    char** _argv;

    static QApplication* _app;
    static QThread* _qtMainThread;
    static GStudentThread* _studentThread;
    static QtGui* _instance;

    friend class GStudentThread;
    friend class GEventQueue;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qtgui_h
