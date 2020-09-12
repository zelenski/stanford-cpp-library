/*
 * File: geventqueue.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2019/01/08
 * - bug fix in waitForClick function (was never returning!)
 * @version 2018/08/23
 * - renamed to geventqueue.cpp
 * @version 2018/07/03
 * - initial version
 */

#include "qtgui.h"
#include <QEvent>
#include "error.h"
#include "exceptions.h"
#include "gwindow.h"
#include "gthread.h"
#include "gtypes.h"
#include "strlib.h"

GEventQueue* GEventQueue::_instance = nullptr;

GEventQueue::GEventQueue()
        : _eventMask(0) {
    // empty
}

GThunk GEventQueue::dequeue() {
    _functionQueueMutex.lockForWrite();
    GThunk thunk = _functionQueue.dequeue();
    _functionQueueMutex.unlock();
    return thunk;
}

void GEventQueue::enqueueEvent(const GEvent& event) {
    if (isAcceptingEvent(event.getEventClass())) {
        _eventQueueMutex.lockForWrite();
        _eventQueue.enqueue(event);
        _eventQueueMutex.unlock();
    }
}

int GEventQueue::getEventMask() const {
    return _eventMask;
}

GEvent GEventQueue::getNextEvent(int mask) {
    setEventMask(mask);

    // check if any events have arrived
    _eventQueueMutex.lockForRead();
    bool empty = _eventQueue.isEmpty();
    _eventQueueMutex.unlock();

    if (!empty) {
        // grab the event and return it
        _eventQueueMutex.lockForWrite();
        while (!_eventQueue.isEmpty()) {
            GEvent event = _eventQueue.dequeue();
            if (isAcceptingEvent(event)) {
                _eventQueueMutex.unlock();
                return event;
            }
        }
        _eventQueueMutex.unlock();
    }

    GEvent bogusEvent;
    return bogusEvent;
}

GEventQueue* GEventQueue::instance() {
    if (!_instance) {
        _instance = new GEventQueue();
    }
    return _instance;
}

bool GEventQueue::isAcceptingEvent(const GEvent& event) const {
    return isAcceptingEvent(event.getEventClass());
}

bool GEventQueue::isAcceptingEvent(int eventClass) const {
    return (_eventMask & eventClass) != 0;
}

bool GEventQueue::isEmpty() const {
    return _functionQueue.isEmpty();
}

GThunk GEventQueue::peek() {
    _functionQueueMutex.lockForRead();
    GThunk thunk = _functionQueue.peek();
    _functionQueueMutex.unlock();
    return thunk;
}

void GEventQueue::runOnQtGuiThreadAsync(GThunk thunk) {
    _functionQueueMutex.lockForWrite();
    _functionQueue.enqueue(thunk);
    _functionQueueMutex.unlock();
    emit eventReady();
}

void GEventQueue::runOnQtGuiThreadSync(GThunk thunk) {
    _functionQueueMutex.lockForWrite();
    _functionQueue.enqueue(thunk);
    _functionQueueMutex.unlock();
    emit eventReady();

    // TODO: "empty" is not quite right condition
    while (true) {
        _functionQueueMutex.lockForRead();
        bool empty = _functionQueue.isEmpty();
        _functionQueueMutex.unlock();
        if (empty) {
            break;
        } else {
            GThread::getCurrentThread()->sleep(1);
        }
    }
}

void GEventQueue::setEventMask(int mask) {
    _eventMask = mask;
}

GEvent GEventQueue::waitForEvent(int mask) {
    setEventMask(mask);
    while (true) {
        // check if any events have arrived
        _eventQueueMutex.lockForRead();
        bool empty = _eventQueue.isEmpty();
        _eventQueueMutex.unlock();

        if (!empty) {
            // grab the event and return it
            _eventQueueMutex.lockForWrite();
            while (!_eventQueue.isEmpty()) {
                GEvent event = _eventQueue.dequeue();
                if (isAcceptingEvent(event)) {
                    _eventQueueMutex.unlock();
                    return event;
                }
            }
            _eventQueueMutex.unlock();
        }

        GThread::getCurrentThread()->sleep(1);
    }
}

GEvent getNextEvent(int mask) {
    return GEventQueue::instance()->getNextEvent(mask);
}

GMouseEvent waitForClick() {
    while (true) {
        GMouseEvent event = GEventQueue::instance()->waitForEvent(MOUSE_EVENT);
        if (event.getEventType() == MOUSE_CLICKED) {
            return event;
        }
    }
}

GEvent waitForEvent(int mask) {
    return GEventQueue::instance()->waitForEvent(mask);
}


