/*
 * File: qgobservable.h
 * ---------------------
 *
 * @version 2018/07/11
 * - initial version
 */

#ifndef _qgobservable_h
#define _qgobservable_h

#include <functional>
#include <string>
#include <QObject>
#include <QWidget>
#include <QWindow>
#include <QWindowStateChangeEvent>
#include "gtypes.h"
#include "map.h"
#include "qgevent.h"

/*
 * ...
 */
class QGObservable {
public:
    QGObservable();
    virtual ~QGObservable();

    virtual std::string getType() const = 0;
    virtual std::string toString() const;

protected:
    virtual void clearEventHandlers();
    virtual void ensureThreadSafety(const std::string& memberName = "");
    virtual void fireEvent(QGEvent& event);
    virtual void fireQGEvent(QEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual void fireQGEvent(QCloseEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual void fireQGEvent(QKeyEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual void fireQGEvent(QMouseEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual void fireQGEvent(QResizeEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual void fireQGEvent(QWheelEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual void fireQGEvent(QWindowStateChangeEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    virtual bool hasEventHandler(const std::string& eventName) const;
    virtual void removeEventHandler(const std::string& eventName);
    virtual void removeEventHandlers(std::initializer_list<std::string> eventNames);
    virtual void setEventHandler(const std::string& eventName, QGEventHandler func);
    virtual void setEventHandler(const std::string& eventName, QGEventHandlerVoid func);
    virtual void setEventHandlers(std::initializer_list<std::string> eventNames, QGEventHandler func);
    virtual void setEventHandlers(std::initializer_list<std::string> eventNames, QGEventHandlerVoid func);

    Map<std::string, QGEvent::EventHandlerWrapper> _eventMap;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgobservable_h
