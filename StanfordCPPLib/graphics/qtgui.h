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

// function pointer (no params / no return)

void __initializeStanfordCppLibraryQt(int argc, char** argv, int (* mainFunc)(void));

/*
 * ...
 */
class QtGui : public QObject {
    Q_OBJECT

public:
    void exitGraphics(int exitCode = 0);
    void initializeQt();
    static QtGui* instance();
    void startBackgroundEventLoop(GThunkInt mainFunc);
    void startEventLoop();

public slots:
    void mySlot();

signals:
    void mySignal();

private:
    QtGui();   // forbid construction

    bool _initialized;

    static QApplication* _app;
    static QThread* _qtMainThread;
    static GStudentThread* _studentThread;
    static int _argc;
    static char** _argv;
    static QtGui* _instance;

    friend void __initializeStanfordCppLibraryQt(int argc, char** argv, int (* mainFunc)(void));
    friend class GStudentThread;
    friend class GEventQueue;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qtgui_h
