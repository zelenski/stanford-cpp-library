/*
 * File: qgevent.cpp
 * -----------------
 *
 * @version 2018/07/06
 * - initial version
 */

#include "qgevent.h"
#include <sys/time.h>
#include "qginteractor.h"
#include "strlib.h"

QGEvent::QGEvent(EventClass eventClass,
                 EventType eventType,
                 const std::string& eventName,
                 QGInteractor* source)
        :
          _actionCommand(""),
          _class(eventClass),
          _keyChar('\0'),
          _keyCode(0),
          _modifiers(0),
          _name(eventName),
          _source(source),
          _time(getCurrentTimeMS()),
          _type(eventType),
          _x(0),
          _y(0) {
    // empty
}

QGEvent::~QGEvent() {
    _source = nullptr;
}

std::string QGEvent::classToString(EventClass eventClass) {
    switch (eventClass) {
    case NULL_EVENT:      return "NULL_EVENT";
    case ACTION_EVENT:    return "ACTION_EVENT";
    case KEY_EVENT:       return "KEY_EVENT";
    case TIMER_EVENT:     return "TIMER_EVENT";
    case WINDOW_EVENT:    return "WINDOW_EVENT";
    case MOUSE_EVENT:     return "MOUSE_EVENT";
    case CLICK_EVENT:     return "CLICK_EVENT";
    case TABLE_EVENT:     return "TABLE_EVENT";
    case SERVER_EVENT:    return "SERVER_EVENT";
    case CHANGE_EVENT:    return "CHANGE_EVENT";
    case HYPERLINK_EVENT: return "HYPERLINK_EVENT";
    default:              return "UNKNOWN";
    }
}

std::string QGEvent::typeToString(EventType eventType) {
    switch (eventType) {
    case NULL_TYPE:           return "NULL_TYPE";
    case WINDOW_CLOSED:       return "WINDOW_CLOSED";
    case WINDOW_RESIZED:      return "WINDOW_RESIZED";
    case CONSOLE_CLOSED:      return "CONSOLE_CLOSED";
    case WINDOW_CLOSING:      return "WINDOW_CLOSING";
    case ACTION_PERFORMED:    return "ACTION_PERFORMED";
    case MOUSE_CLICKED:       return "MOUSE_CLICKED";
    case MOUSE_PRESSED:       return "MOUSE_PRESSED";
    case MOUSE_RELEASED:      return "MOUSE_RELEASED";
    case MOUSE_MOVED:         return "MOUSE_MOVED";
    case MOUSE_DRAGGED:       return "MOUSE_DRAGGED";
    case MOUSE_ENTERED:       return "MOUSE_ENTERED";
    case MOUSE_EXITED:        return "MOUSE_EXITED";
    case MOUSE_WHEEL_DOWN:    return "MOUSE_WHEEL_DOWN";
    case MOUSE_WHEEL_UP:      return "MOUSE_WHEEL_UP";
    case KEY_PRESSED:         return "KEY_PRESSED";
    case KEY_RELEASED:        return "KEY_RELEASED";
    case KEY_TYPED:           return "KEY_TYPED";
    case TIMER_TICKED:        return "TIMER_TICKED";
    case TABLE_UPDATED:       return "TABLE_UPDATED";
    case TABLE_SELECTED:      return "TABLE_SELECTED";
    case TABLE_EDIT_BEGIN:    return "TABLE_EDIT_BEGIN";
    case TABLE_REPLACE_BEGIN: return "TABLE_REPLACE_BEGIN";
    case TABLE_CUT:           return "TABLE_CUT";
    case TABLE_COPY:          return "TABLE_COPY";
    case TABLE_PASTE:         return "TABLE_PASTE";
    case SERVER_REQUEST:      return "SERVER_REQUEST";
    case STATE_CHANGED:       return "STATE_CHANGED";
    case HYPERLINK_CLICKED:   return "HYPERLINK_CLICKED";
    default:                  return "UNKNOWN";
    }
}


std::string QGEvent::getActionCommand() const {
    return _actionCommand;
}

int QGEvent::getButton() const {
    return _button;
}

QGEvent::EventClass QGEvent::getClass() const {
    return _class;
}

long QGEvent::getCurrentTimeMS() {
    struct timeval tp;
    gettimeofday(&tp, nullptr);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    return ms;
}

char QGEvent::getKeyChar() const {
    return _keyChar;
}

int QGEvent::getKeyCode() const {
    return _keyCode;
}

int QGEvent::getModifiers() const {
    return _modifiers;
}

std::string QGEvent::getName() const {
    return _name;
}

QGInteractor* QGEvent::getSource() const {
    return _source;
}

long QGEvent::getTime() const {
    return _time;
}

QGEvent::EventType QGEvent::getType() const {
    return _type;
}

double QGEvent::getX() const {
    return _x;
}

double QGEvent::getY() const {
    return _y;
}

bool QGEvent::isAltKeyDown() const {
    return (_modifiers & ALT_DOWN) != 0;
}

bool QGEvent::isCtrlKeyDown() const {
    return (_modifiers & CTRL_DOWN) != 0;
}

bool QGEvent::isCtrlOrCommandKeyDown() const {
    return isCtrlKeyDown() || isMetaKeyDown();
}

bool QGEvent::isMetaKeyDown() const {
    return (_modifiers & META_DOWN) != 0;
}

bool QGEvent::isShiftKeyDown() const {
    return (_modifiers & SHIFT_DOWN) != 0;
}

void QGEvent::setButton(int button) {
    _button = button;
}

void QGEvent::setX(double x) {
    _x = x;
}

void QGEvent::setY(double y) {
    _y = y;
}


std::string QGEvent::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::ostream& operator <<(std::ostream& out, const QGEvent& event) {
    out << "QGEvent(";
    out << "class=" << QGEvent::classToString(event.getClass());
    out << ",type=" << QGEvent::typeToString(event.getType());
    out << ",name=" << event.getName();
    out << ",time=" << event.getTime();
    if (event.getSource()) {
        out << ",source=" << event.getSource()->toString();
    }
    out << ")";
    return out;
}
