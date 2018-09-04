/*
 * File: gobservable.h
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gobservable.h to replace Java version
 * @version 2018/07/11
 * - initial version
 */

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
#include "map.h"
#include "gevent.h"

class GInteractor;
class _Internal_QWidget;

/*
 * ...
 */
class GObservable {
public:
    GObservable();
    virtual ~GObservable();

    virtual bool eventsEnabled() const;
    virtual std::string getType() const = 0;
    virtual void setEventsEnabled(bool eventsEnabled);
    virtual std::string toString() const;

protected:
    virtual void clearEventListeners();
    virtual void ensureThreadSafety(const std::string& memberName = "");
    virtual void fireEvent(GEvent& event);
    virtual void fireGEvent(QEvent* event, EventType eventType, const std::string& eventName);
    virtual void fireGEvent(QCloseEvent* event, EventType eventType, const std::string& eventName);
    virtual void fireGEvent(QKeyEvent* event, EventType eventType, const std::string& eventName);
    virtual void fireGEvent(QMouseEvent* event, EventType eventType, const std::string& eventName, const std::string& actionCommand = "");
    virtual void fireGEvent(QResizeEvent* event, EventType eventType, const std::string& eventName);
    virtual void fireGEvent(QTimerEvent* event, EventType eventType, const std::string& eventName);
    virtual void fireGEvent(QWheelEvent* event, EventType eventType, const std::string& eventName);
    virtual void fireGEvent(QWindowStateChangeEvent* event, EventType eventType, const std::string& eventName);
    virtual bool hasEventListener(const std::string& eventName) const;
    virtual bool isAcceptingEvent(int eventMask) const;
    virtual bool isAcceptingEvent(const GEvent& event) const;
    virtual bool isAcceptingEvent(const std::string& eventType) const;
    virtual void removeEventListener(const std::string& eventName);
    virtual void removeEventListeners(std::initializer_list<std::string> eventNames);
    virtual void setEventListener(const std::string& eventName, GEventListener func);
    virtual void setEventListener(const std::string& eventName, GEventListenerVoid func);
    virtual void setEventListeners(std::initializer_list<std::string> eventNames, GEventListener func);
    virtual void setEventListeners(std::initializer_list<std::string> eventNames, GEventListenerVoid func);

    Map<std::string, GEvent::EventListenerWrapper> _eventMap;
    bool _eventsEnabled;

    // allow all interactors and their inner QWidgets to call observable methods
    friend class GInteractor;
    friend class _Internal_QWidget;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gobservable_h
