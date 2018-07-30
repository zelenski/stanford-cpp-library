/*
 * File: qgui.h
 * ------------
 *
 * @version 2018/07/03
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgui_h
#define _qgui_h

#include <functional>
#include <string>
#include <QApplication>
#include <QObject>
#include <QReadWriteLock>
#include "qgeventqueue.h"
#include "qgthread.h"
#include "qgtypes.h"
#include "queue.h"

// function pointer (no params / no return)

void __initializeStanfordCppLibraryQt(int argc, char** argv, int (* mainFunc)(void));

/*
 * ...
 */
class QGui : public QObject {
    Q_OBJECT

public:
    void exitGraphics(int exitCode = 0);
    void initializeQt();
    static QGui* instance();
    void startBackgroundEventLoop(QGThunkInt mainFunc);
    void startEventLoop();

public slots:
    void mySlot();

signals:
    void mySignal();

private:
    QGui();   // forbid construction

    bool _initialized;

    static QApplication* _app;
    static QThread* _qtMainThread;
    static QGStudentThread* _studentThread;
    static int _argc;
    static char** _argv;
    static QGui* _instance;

    friend void __initializeStanfordCppLibraryQt(int argc, char** argv, int (* mainFunc)(void));
    friend class QGStudentThread;
    friend class QGuiEventQueue;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgui_h
#endif // SPL_QT_GUI
