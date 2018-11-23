/*
 * File: geventqueue.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to geventqueue.h
 * @version 2018/07/03
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _geventqueue_h
#define _geventqueue_h

#include <string>
#include <QObject>
#include <QReadWriteLock>

#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "gtypes.h"
#define INTERNAL_INCLUDE 1
#include "queue.h"
#undef INTERNAL_INCLUDE

class GObservable;
class GThread;
class QtGui;

/**
 * The GEventQueue is a first-in, first-out (FIFO) queue of events that occur
 * on graphical interactors.
 *
 * This queue is used by our interactor classes (GButton, GCheckBox, etc.)
 * internally.  When events occur on these widgets, we place them into a GEventQueue
 * so that they can be sent out to the various event listeners in client code.
 *
 * Most clients do not need to use GEventQueue directly.
 *
 * @private
 */
class GEventQueue : public QObject {
    Q_OBJECT

public:
    /**
     * Returns the single instance of the event queue.
     * If no instance yet exists, one is created.
     * @return
     */
    static GEventQueue* instance();

    /**
     * Returns the current event mask used by the semi-deprecated global
     * event-handling functions like waitForEvent.
     */
    int getEventMask() const;

    /**
     * Returns the next event that occurs that matches the given mask
     * of event types.
     */
    GEvent getNextEvent(int mask = ANY_EVENT);

    /**
     * Returns true if the given event would be accepted by the current
     * event mask, as per setEventMask.
     */
    bool isAcceptingEvent(const GEvent& event) const;
    bool isAcceptingEvent(int type) const;

    /**
     * Sets a bit-flagged mask of event types to listen for
     * in the semi-deprecated global event-handling functions like waitForEvent.
     * If setEventMask has not been called, all events are accepted.
     */
    void setEventMask(int mask);

    /**
     * Pauses the current thread until an event occurs that matches the given
     * bit-flagged mask.  The event is then returned.
     * The current event mask is also set to the given mask, as if
     * setEventMask had been called.
     */
    GEvent waitForEvent(int mask = ANY_EVENT);

signals:
    /**
     * Fires when an event is present in the queue and ready to be processed.
     */
    void eventReady();

private:
    Q_DISABLE_COPY(GEventQueue)

    /*
     * Prevents construction.  Use instance() instead.
     */
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

#endif // _geventqueue_h
