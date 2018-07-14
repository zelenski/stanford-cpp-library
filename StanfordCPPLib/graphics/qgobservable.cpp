/*
 * File: qgobservable.cpp
 * ----------------------
 *
 * @version 2018/07/11
 * - initial version
 */

#include "qgobservable.h"
#include <iostream>
#include <sstream>
#include "qgui.h"

QGObservable::QGObservable() {
    // empty
    // ensureThreadSafety();
}

QGObservable::~QGObservable() {
    // empty
}

void QGObservable::clearEventHandlers() {
    _eventMap.clear();
}

void QGObservable::ensureThreadSafety(const std::string& memberName) {
    QGui::instance()->ensureThatThisIsTheQtGuiThread(memberName);
}

void QGObservable::fireEvent(QGEvent& event) {
    if (hasEventHandler(event.getName())) {
        event.setSource(this);
        _eventMap[event.getName()].fireEvent(event);
    }
}

void QGObservable::fireQGEvent(QEvent* /* event */,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent generalEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    fireEvent(generalEvent);
}

void QGObservable::fireQGEvent(QCloseEvent* /* event */,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent windowEvent(
                /* class  */ QGEvent::WINDOW_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
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
    keyEvent.setKeyChar(event->text().toStdString());
    keyEvent.setModifiers(event->modifiers());
    fireEvent(keyEvent);
}

void QGObservable::fireQGEvent(QMouseEvent* event,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent mouseEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->x());
    mouseEvent.setY(event->y());
    mouseEvent.setModifiers(event->modifiers());
    fireEvent(mouseEvent);
}

void QGObservable::fireQGEvent(QResizeEvent* /* event */,
                               QGEvent::EventType /* eventType */,
                               const std::string& eventName) {
    QGEvent windowEvent(
                /* class  */ QGEvent::WINDOW_EVENT,
                /* type   */ QGEvent::WINDOW_RESIZED,
                /* name   */ eventName,
                /* source */ this);
    fireEvent(windowEvent);
}

void QGObservable::fireQGEvent(QTimerEvent* /* event */,
                               QGEvent::EventType /* eventType */,
                               const std::string& /* eventName */) {
    QGEvent timerEvent(
                /* class  */ QGEvent::TIMER_EVENT,
                /* type   */ QGEvent::TIMER_TICKED,
                /* name   */ "timer",
                /* source */ this);
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
    fireEvent(wheelEvent);
}

void QGObservable::fireQGEvent(QWindowStateChangeEvent* /* event */,
                               QGEvent::EventType eventType,
                               const std::string& eventName) {
    QGEvent windowEvent(
                /* class  */ QGEvent::WINDOW_EVENT,
                /* type   */ eventType,
                /* name   */ eventName,
                /* source */ this);
    fireEvent(windowEvent);
}

bool QGObservable::hasEventHandler(const std::string& eventName) const {
    return _eventMap.containsKey(eventName);
}

void QGObservable::removeEventHandler(const std::string& eventName) {
    _eventMap.remove(eventName);
}

void QGObservable::removeEventHandlers(std::initializer_list<std::string> eventNames) {
    for (std::string eventName : eventNames) {
        removeEventHandler(eventName);
    }
}

void QGObservable::setEventHandler(const std::string& eventName, QGEventHandler func) {
    QGEvent::EventHandlerWrapper wrapper;
    wrapper.type = QGEvent::HANDLER_EVENT;
    wrapper.handler = func;
    _eventMap[eventName] = wrapper;
}

void QGObservable::setEventHandler(const std::string& eventName, QGEventHandlerVoid func) {
    QGEvent::EventHandlerWrapper wrapper;
    wrapper.type = QGEvent::HANDLER_VOID;
    wrapper.handlerVoid = func;
    _eventMap[eventName] = wrapper;
}

void QGObservable::setEventHandlers(std::initializer_list<std::string> eventNames, QGEventHandler func) {
    for (std::string eventName : eventNames) {
        setEventHandler(eventName, func);
    }
}

void QGObservable::setEventHandlers(std::initializer_list<std::string> eventNames, QGEventHandlerVoid func) {
    for (std::string eventName : eventNames) {
        setEventHandler(eventName, func);
    }
}

std::string QGObservable::toString() const {
    std::ostringstream out;
    out << getType() << "@" << this;
    return out.str();
}
