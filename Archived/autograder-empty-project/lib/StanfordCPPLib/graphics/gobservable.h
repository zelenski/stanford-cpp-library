/*
 * File: gobservable.h
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gobservable.h to replace Java version
 * @version 2018/07/11
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gobservable_h
#define _gobservable_h

#include <functional>
#include <string>
#include <QObject>
#include <QWidget>
#include <QWindow>
#include <QEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QWheelEvent>
#include <QWindowStateChangeEvent>

#define INTERNAL_INCLUDE 1
#include "gevent.h"
#define INTERNAL_INCLUDE 1
#include "map.h"
#undef INTERNAL_INCLUDE

class GInteractor;
class _Internal_QWidget;

/**
 * A GObservable object is one that is able to send out events.
 * Listeners can register themselves to be notified when events occur.
 * This serves as a base class for the various GInteractor subclasses, as well
 * as for the GWindow class, so that clients can attach listeners to them.
 */
class GObservable {
public:
    /**
     * Initializes a newly created observable object.
     */
    GObservable();

    /**
     * Frees any memory used internally by the observable object.
     */
    virtual ~GObservable();

    /**
     * Returns true if the object is currently allowing itself to fire events.
     * Initially this is true unless the client has called
     * <code>setEventsEnabled(false)</code> or the object is not visible.
     */
    virtual bool eventsEnabled() const;

    /**
     * Returns the concrete type of the object as a string, such as
     * <code>"GButton"</code> or <code>"GWindow"</code>.
     * Each GObservable subtype must override this method.
     */
    virtual std::string getType() const = 0;

    /**
     * Sets whether the object is currently allowing itself to fire events.
     * Initially this is true.
     */
    virtual void setEventsEnabled(bool eventsEnabled);

    /**
     * Returns a string representation of this observable object's state.
     * Primarily used for debugging purposes.
     */
    virtual std::string toString() const;

protected:
    /**
     * Removes all event listeners from this object.
     */
    virtual void clearEventListeners();

    /**
     * Ensures that we are currently in the Qt GUI thread.
     */
    virtual void ensureThreadSafety(const std::string& memberName = "");

    /**
     * Sends out the given event to any attached listeners.
     */
    virtual void fireEvent(GEvent& event);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QCloseEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QKeyEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QMouseEvent* event, EventType eventType, const std::string& eventName, const std::string& actionCommand = "");

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QResizeEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QTimerEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QWheelEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Creates an event of the given type, then sends it out to any
     * attached listeners.
     */
    virtual void fireGEvent(QWindowStateChangeEvent* event, EventType eventType, const std::string& eventName);

    /**
     * Returns true if the observable object has a listener for the given type
     * of event.
     */
    virtual bool hasEventListener(const std::string& eventName) const;

    /**
     * Returns true if the observable object has a listener for the given type
     * of event.
     * See gevent.h for event types and masks.
     */
    virtual bool isAcceptingEvent(int eventMask) const;

    /**
     * Returns true if the observable object has a listener for the given type
     * of event.
     */
    virtual bool isAcceptingEvent(const GEvent& event) const;

    /**
     * Returns true if the observable object has a listener for the given type
     * of event.
     */
    virtual bool isAcceptingEvent(const std::string& eventType) const;

    /**
     * Removes any event listener from this observable object that would respond
     * to the given type of event, such as "click" or "keydown".
     */
    virtual void removeEventListener(const std::string& eventName);

    /**
     * Removes any event listener from this observable object that would respond
     * to the given types of events, such as "click" or "keydown".
     */
    virtual void removeEventListeners(std::initializer_list<std::string> eventNames);

    /**
     * Adds an event listener from this observable object to respond
     * to the given type of event, such as "click" or "keydown".
     * Any prior listener for that type of event is replaced.
     */
    virtual void setEventListener(const std::string& eventName, GEventListener func);

    /**
     * Adds an event listener from this observable object to respond
     * to the given type of event, such as "click" or "keydown".
     * Any prior listener for that type of event is replaced.
     */
    virtual void setEventListener(const std::string& eventName, GEventListenerVoid func);

    /**
     * Adds an event listener from this observable object to respond
     * to the given types of events, such as "click" or "keydown".
     * Any prior listener for those types of event are replaced.
     */
    virtual void setEventListeners(std::initializer_list<std::string> eventNames, GEventListener func);

    /**
     * Adds an event listener from this observable object to respond
     * to the given types of events, such as "click" or "keydown".
     * Any prior listener for those types of event are replaced.
     */
    virtual void setEventListeners(std::initializer_list<std::string> eventNames, GEventListenerVoid func);

private:
    Map<std::string, GEvent::EventListenerWrapper> _eventMap;
    bool _eventsEnabled;

    // allow all interactors and their inner QWidgets to call observable methods
    friend class GInteractor;
    friend class _Internal_QWidget;
};

#endif // _gobservable_h
