/*
 * File: geventqueue.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to geventqueue.h
 * @version 2018/07/03
 * - initial version
 */

#ifndef _geventqueue_h
#define _geventqueue_h

#include <string>
#include <QObject>
#include <QReadWriteLock>
#include "gevent.h"
#include "gtypes.h"
#include "queue.h"

// forward declarations
class GObservable;
class GThread;
class QtGui;

class GEventQueue : public QObject {
    Q_OBJECT

public:
    static GEventQueue* instance();
    int getEventMask() const;
    GEvent getNextEvent(int mask = ANY_EVENT);
    bool isAcceptingEvent(const GEvent& event) const;
    bool isAcceptingEvent(int type) const;
    void setEventMask(int mask);
    GEvent waitForEvent(int mask = ANY_EVENT);

signals:
    void mySignal();

private:
    Q_DISABLE_COPY(GEventQueue)

    GEventQueue();

    GThunk dequeue();
    void enqueueEvent(const GEvent& event);
    bool isEmpty() const;
    GThunk peek();
    void runOnQtGuiThreadAsync(GThunk thunk);
    void runOnQtGuiThreadSync(GThunk thunk);

    static GEventQueue* _instance;
    Queue<GThunk> _functionQueue;
    Queue<GEvent> _eventQueue;
    QReadWriteLock _eventQueueMutex;
    QReadWriteLock _functionQueueMutex;
    int _eventMask;

    friend class GObservable;
    friend class GThread;
    friend class QtGui;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _geventqueue_h
