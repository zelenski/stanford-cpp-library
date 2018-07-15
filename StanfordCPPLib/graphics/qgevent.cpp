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
                 QGObservable* source)
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
    case WINDOW_MINIMIZED:    return "WINDOW_MINIMIZED";
    case WINDOW_RESTORED:     return "WINDOW_RESTORED";
    case WINDOW_MAXIMIZED:    return "WINDOW_MAXIMIZED";
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

QGEvent::EventClass QGEvent::getEventClass() const {
    return _class;
}

QGEvent::EventType QGEvent::getEventType() const {
    return _type;
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

QGInteractor* QGEvent::getInteractor() const {
    return static_cast<QGInteractor*>(_source);
}

QGPoint QGEvent::getLocation() const {
    return QGPoint(getX(), getY());
}

QGObservable* QGEvent::getSource() const {
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

bool QGEvent::isLeftClick() const {
    return !isMiddleClick() && !isRightClick();
}

bool QGEvent::isMiddleClick() const {
    return _button == 4 || ((_modifiers & BUTTON3_DOWN) != 0);
}

bool QGEvent::isRightClick() const {
    return _button == 2 || ((_modifiers & BUTTON2_DOWN) != 0);
}

bool QGEvent::isMetaKeyDown() const {
    return (_modifiers & META_DOWN) != 0;
}

bool QGEvent::isShiftKeyDown() const {
    return (_modifiers & SHIFT_DOWN) != 0;
}

std::string QGEvent::keyCodeToString(int keyCode) {
    switch (keyCode) {
    case ALT_KEY: return "Alt";
    case BACKSPACE_KEY: return "Bksp";
    case CAPS_LOCK_KEY: return "CapsLock";
    case CLEAR_KEY: return "Clear";
    case CTRL_KEY: return "Ctrl";
    case DELETE_KEY: return "Del";
    case DOWN_ARROW_KEY: return "Down";
    case END_KEY: return "End";
    case ENTER_KEY: return "Enter";
    case ESCAPE_KEY: return "Esc";
    case F10_KEY: return "F10";
    case F11_KEY: return "F11";
    case F12_KEY: return "F12";
    case F1_KEY: return "F1";
    case F2_KEY: return "F2";
    case F3_KEY: return "F3";
    case F4_KEY: return "F4";
    case F5_KEY: return "F5";
    case F6_KEY: return "F6";
    case F7_KEY: return "F7";
    case F8_KEY: return "F8";
    case F9_KEY: return "F9";
    case HELP_KEY: return "Help";
    case HOME_KEY: return "Home";
    case INSERT_KEY: return "Ins";
    case LEFT_ARROW_KEY: return "Left";
    case MENU_KEY: return "Menu";
    case META_KEY: return "Meta";
    case NUM_LOCK_KEY: return "NumLock";
    case PAGE_DOWN_KEY: return "PgDn";
    case PAGE_UP_KEY: return "PgUp";
    case PAUSE_KEY: return "Pause";
    case PRINT_SCREEN_KEY: return "PrtSc";
    case RIGHT_ARROW_KEY: return "Right";
    case SCROLL_LOCK_KEY: return "ScrollLock";
    case SHIFT_KEY: return "Shift";
    case TAB_KEY: return "Tab";
    case UP_ARROW_KEY: return "Up";
    case WINDOWS_KEY: return "Win";
    case '\r': return "Enter";
    case '\\': return "\\";
    default: return charToString((char) keyCode);
    }
}

void QGEvent::setActionCommand(const std::string& actionCommand) {
    _actionCommand = actionCommand;
}

void QGEvent::setButton(int button) {
    _button = button;
}

void QGEvent::setKeyChar(char keyChar) {
    if (keyChar == '\r') {
        keyChar = '\n';
    }
    _keyChar = keyChar;
    _keyCode = (int) keyChar;
}

void QGEvent::setKeyChar(const std::string& keyCharString) {
    if (!keyCharString.empty()) {
        setKeyChar(keyCharString[0]);
    }
}

void QGEvent::setKeyCode(int keyCode) {
    if (keyCode == (int) '\r') {
        keyCode = (int) '\n';
    }
    _keyCode = keyCode;
    if (keyCode > 0 && keyCode < 255) {
        _keyChar = (char) keyCode;
    } else {
        switch (keyCode) {
        case Qt::Key_Alt:
        case Qt::Key_AltGr:
            _keyChar = _keyCode = ALT_KEY;
            break;
        case Qt::Key_Backspace:
            _keyChar = _keyCode = BACKSPACE_KEY;
            break;
        case Qt::Key_CapsLock:
            _keyChar = _keyCode = CAPS_LOCK_KEY;
            break;
        case Qt::Key_Control:
            _keyChar = _keyCode = CTRL_KEY;
            break;
        case Qt::Key_Delete:
            _keyChar = _keyCode = DELETE_KEY;
            break;
        case Qt::Key_Down:
            _keyChar = _keyCode = DOWN_ARROW_KEY;
            break;
        case Qt::Key_End:
            _keyChar = _keyCode = END_KEY;
            break;
        case Qt::Key_Enter:
            _keyChar = _keyCode = ENTER_KEY;
            break;
        case Qt::Key_Escape:
            _keyChar = _keyCode = ESCAPE_KEY;
            break;
        case Qt::Key_F1:
            _keyChar = _keyCode = F1_KEY;
            break;
        case Qt::Key_F2:
            _keyChar = _keyCode = F2_KEY;
            break;
        case Qt::Key_F3:
            _keyChar = _keyCode = F3_KEY;
            break;
        case Qt::Key_F4:
            _keyChar = _keyCode = F4_KEY;
            break;
        case Qt::Key_F5:
            _keyChar = _keyCode = F5_KEY;
            break;
        case Qt::Key_F6:
            _keyChar = _keyCode = F6_KEY;
            break;
        case Qt::Key_F7:
            _keyChar = _keyCode = F7_KEY;
            break;
        case Qt::Key_F8:
            _keyChar = _keyCode = F8_KEY;
            break;
        case Qt::Key_F9:
            _keyChar = _keyCode = F9_KEY;
            break;
        case Qt::Key_F10:
            _keyChar = _keyCode = F10_KEY;
            break;
        case Qt::Key_F11:
            _keyChar = _keyCode = F11_KEY;
            break;
        case Qt::Key_F12:
            _keyChar = _keyCode = F12_KEY;
            break;
        case Qt::Key_Home:
            _keyChar = _keyCode = HOME_KEY;
            break;
        case Qt::Key_Insert:
            _keyChar = _keyCode = INSERT_KEY;
            break;
        case Qt::Key_Left:
            _keyChar = _keyCode = LEFT_ARROW_KEY;
            break;
        case Qt::Key_Menu:
            _keyChar = _keyCode = MENU_KEY;
            break;
        case Qt::Key_Meta:
            _keyChar = _keyCode = META_KEY;
            break;
        case Qt::Key_NumLock:
            _keyChar = _keyCode = NUM_LOCK_KEY;
            break;
        case Qt::Key_PageDown:
            _keyChar = _keyCode = PAGE_DOWN_KEY;
            break;
        case Qt::Key_PageUp:
            _keyChar = _keyCode = PAGE_UP_KEY;
            break;
        case Qt::Key_Pause:
            _keyChar = _keyCode = PAUSE_KEY;
            break;
        case Qt::Key_Print:
            _keyChar = _keyCode = PRINT_SCREEN_KEY;
            break;
        case Qt::Key_Return:
            _keyChar = _keyCode = ENTER_KEY;
            break;
        case Qt::Key_Right:
            _keyChar = _keyCode = RIGHT_ARROW_KEY;
            break;
        case Qt::Key_ScrollLock:
            _keyChar = _keyCode = SCROLL_LOCK_KEY;
            break;
        case Qt::Key_Shift:
            _keyChar = _keyCode = SHIFT_KEY;
            break;
        case Qt::Key_Super_L:
        case Qt::Key_Super_R:
            _keyChar = _keyCode = WINDOWS_KEY;
            break;
        case Qt::Key_Tab:
            _keyChar = _keyCode = TAB_KEY;
            break;
        case Qt::Key_Up:
            _keyChar = _keyCode = UP_ARROW_KEY;
            break;
        }
    }
}

void QGEvent::setModifiers(Qt::KeyboardModifiers modifiers) {
    _modifiers = 0;
    if (modifiers & Qt::AltModifier) {
        _modifiers |= ALT_DOWN;
    }
    if (modifiers & Qt::ControlModifier) {
        _modifiers |= CTRL_DOWN;
    }
    if (modifiers & Qt::MetaModifier) {
        _modifiers |= META_DOWN;
    }
    if (modifiers & Qt::ShiftModifier) {
        _modifiers |= SHIFT_DOWN;
    }
}

void QGEvent::setSource(QGObservable* source) {
    _source = source;
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
    if (!event.getActionCommand().empty()) {
        out << ",actionCommand=\"" << event.getActionCommand() << "\"";
    }
    if (event.getEventClass() == QGEvent::KEY_EVENT) {
        out << ",key=" << event.getKeyCode()
            << "(" << QGEvent::keyCodeToString(event.getKeyCode()) << ")";
    } else if (event.getEventClass() == QGEvent::MOUSE_EVENT) {
        out << ",x=" << event.getX() << ",y=" << event.getY() << ",button=" << event.getButton();
    }
    out << ")";
    return out;
}
