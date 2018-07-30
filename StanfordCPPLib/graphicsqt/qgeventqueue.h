/*
 * File: qgeventqueue.h
 * --------------------
 *
 * @version 2018/07/03
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgeventqueue_h
#define _qgeventqueue_h

#include <string>
#include <QObject>
#include <QReadWriteLock>
#include "qgtypes.h"
#include "queue.h"

class QGThread;
class QGui;

class QGuiEventQueue : public QObject {
    Q_OBJECT
signals:
    void mySignal();

private:
    static QGuiEventQueue* instance();

    QGuiEventQueue();

    QGThunk dequeue();
    bool isEmpty() const;
    QGThunk peek();
    void runOnQtGuiThreadAsync(QGThunk thunk);
    void runOnQtGuiThreadSync(QGThunk thunk);

    static QGuiEventQueue* _instance;
    Queue<QGThunk> _functionQueue;
    QReadWriteLock _queueMutex;

    friend class QGThread;
    friend class QGui;
};


#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgeventqueue_h
#endif // SPL_QT_GUI
