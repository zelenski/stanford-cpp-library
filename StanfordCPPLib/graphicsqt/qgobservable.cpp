/*
 * File: qgobservable.cpp
 * ----------------------
 *
 * @version 2018/07/11
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgobservable.h"
#include <iostream>
#include <sstream>
#include "qgui.h"

QGObservable::QGObservable()
        : _eventsEnabled(true) {
    // empty
}

QGObservable::~QGObservable() {
    // empty
}

void QGObservable::clearEventListeners() {
    _eventMap.clear();
}

void QGObservable::ensureThreadSafety(const std::string& memberName) {
    QGThread::ensureThatThisIsTheQtGuiThread(memberName);
}

bool QGObservable::eventsEnabled() const {
    return _eventsEnabled;
}

void QGObservable::fireEvent(QGEvent& event) {
    if (eventsEnabled() && hasEventListener(event.getName())) {
        event.setSource(this);
        _eventMap[event.getName()].fireEvent(event);
    }
}

void QGObservable::fireQGEvent(QEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent generalEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    generalEvent.setInternalEvent(event);
    fireEvent(generalEvent);
}

void QGObservable::fireQGEvent(QCloseEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent windowEvent(
                /* class  */ QGEvent::WINDOW_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    windowEvent.setInternalEvent(event);
    fireEvent(windowEvent);
}

void QGObservable::fireQGEvent(QKeyEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent keyEvent(
                /* class  */ QGEvent::KEY_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    keyEvent.setKeyCode(event->key());
    std::string text = event->text().toStdString();
    if (!text.empty()) {
        keyEvent.setKeyChar(text);
    } else {
        // int keyChar = event->key() & 0x0000ffff & ~Qt::KeyboardModifierMask;
        // keyEvent.setKeyChar((char) keyChar);
    }
    keyEvent.setModifiers(event->modifiers());
    keyEvent.setInternalEvent(event);
    fireEvent(keyEvent);
}

void QGObservable::fireQGEvent(QMouseEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName,
                               const std::string& actionCommand) {
    QGEvent mouseEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    mouseEvent.setModifiers(event->modifiers());
    mouseEvent.setActionCommand(actionCommand);
    mouseEvent.setInternalEvent(event);
    fireEvent(mouseEvent);
}

void QGObservable::fireQGEvent(QResizeEvent* event,
                               QGEvent::EventType /* eventType */,
                               const std::string& eventName) {
    QGEvent windowEvent(
                /* class  */ QGEvent::WINDOW_EVENT,
                /* type   */ QGEvent::WINDOW_RESIZED,
                /* name   */ eventName,
                /* source */ this);
    windowEvent.setInternalEvent(event);
    fireEvent(windowEvent);
}

void QGObservable::fireQGEvent(QTimerEvent* event,
                               QGEvent::EventType /* eventType */,
                               const std::string& /* eventName */) {
    QGEvent timerEvent(
                /* class  */ QGEvent::TIMER_EVENT,
                /* type   */ QGEvent::TIMER_TICKED,
                /* name   */ "timer",
                /* source */ this);
    timerEvent.setInternalEvent(event);
    fireEvent(timerEvent);
}

void QGObservable::fireQGEvent(QWheelEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent wheelEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    wheelEvent.setButton((int) event->buttons());
    wheelEvent.setX(event->x());
    wheelEvent.setY(event->y());
    wheelEvent.setModifiers(event->modifiers());
    wheelEvent.setInternalEvent(event);
    fireEvent(wheelEvent);
}

void QGObservable::fireQGEvent(QWindowStateChangeEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent windowEvent(
                /* class  */ QGEvent::WINDOW_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    windowEvent.setInternalEvent(event);
    fireEvent(windowEvent);
}

bool QGObservable::hasEventListener(const std::string& eventName) const {
    return _eventMap.containsKey(eventName);
}

void QGObservable::removeEventListener(const std::string& eventName) {
    _eventMap.remove(eventName);
}

void QGObservable::removeEventListeners(std::initializer_list<std::string> eventNames) {
    for (std::string eventName : eventNames) {
        removeEventListener(eventName);
    }
}

void QGObservable::setEventListener(const std::string& eventName, QGEventListener func) {
    QGEvent::EventListenerWrapper wrapper;
    wrapper.type = QGEvent::HANDLER_EVENT;
    wrapper.handler = func;
    _eventMap[eventName] = wrapper;
}

void QGObservable::setEventListener(const std::string& eventName, QGEventListenerVoid func) {
    QGEvent::EventListenerWrapper wrapper;
    wrapper.type = QGEvent::HANDLER_VOID;
    wrapper.handlerVoid = func;
    _eventMap[eventName] = wrapper;
}

void QGObservable::setEventListeners(std::initializer_list<std::string> eventNames, QGEventListener func) {
    for (std::string eventName : eventNames) {
        setEventListener(eventName, func);
    }
}

void QGObservable::setEventListeners(std::initializer_list<std::string> eventNames, QGEventListenerVoid func) {
    for (std::string eventName : eventNames) {
        setEventListener(eventName, func);
    }
}

void QGObservable::setEventsEnabled(bool eventsEnabled) {
    _eventsEnabled = eventsEnabled;
}

std::string QGObservable::toString() const {
    std::ostringstream out;
    out << getType() << "@" << this;
    return out.str();
}

#endif // SPL_QT_GUI
