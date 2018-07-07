/*
 * File: qgevent.h
 * ---------------
 *
 * @version 2018/07/06
 * - initial version
 */

#ifndef _qgevent_h
#define _qgevent_h

#include <functional>
#include <iostream>
#include <string>
#include <QEvent>

class QGInteractor;        // forward declaration
class QGEvent;             // forward declaration
class _Internal_QWidget;   // forward declaration

typedef std::function<void(QGEvent)> QGEventHandler;
typedef std::function<void()>        QGEventHandlerVoid;

/*
 * ...
 */
class QGEvent {
public:
    enum EventClass {
        NULL_EVENT      = 0x0000,
        ACTION_EVENT    = 0x0010,
        KEY_EVENT       = 0x0020,
        TIMER_EVENT     = 0x0040,
        WINDOW_EVENT    = 0x0080,
        MOUSE_EVENT     = 0x0100,
        CLICK_EVENT     = 0x0200,
        TABLE_EVENT     = 0x0400,
        SERVER_EVENT    = 0x0800,
        CHANGE_EVENT    = 0x1000,
        HYPERLINK_EVENT = 0x2000
    };

    /*
     * Type: EventType
     * ---------------
     * This enumeration type defines the event types for all events.
     */
    enum EventType {
        NULL_TYPE           = 0,

        WINDOW_CLOSED       = WINDOW_EVENT + 1,
        WINDOW_RESIZED      = WINDOW_EVENT + 2,
        CONSOLE_CLOSED      = WINDOW_EVENT + 3,
        WINDOW_CLOSING      = WINDOW_EVENT + 4,

        ACTION_PERFORMED    = ACTION_EVENT + 1,

        MOUSE_CLICKED       = MOUSE_EVENT + 1,
        MOUSE_PRESSED       = MOUSE_EVENT + 2,
        MOUSE_RELEASED      = MOUSE_EVENT + 3,
        MOUSE_MOVED         = MOUSE_EVENT + 4,
        MOUSE_DRAGGED       = MOUSE_EVENT + 5,
        MOUSE_ENTERED       = MOUSE_EVENT + 6,
        MOUSE_EXITED        = MOUSE_EVENT + 7,
        MOUSE_WHEEL_DOWN    = MOUSE_EVENT + 8,
        MOUSE_WHEEL_UP      = MOUSE_EVENT + 9,

        KEY_PRESSED         = KEY_EVENT + 1,
        KEY_RELEASED        = KEY_EVENT + 2,
        KEY_TYPED           = KEY_EVENT + 3,

        TIMER_TICKED        = TIMER_EVENT + 1,

        TABLE_UPDATED       = TABLE_EVENT + 1,
        TABLE_SELECTED      = TABLE_EVENT + 2,
        TABLE_EDIT_BEGIN    = TABLE_EVENT + 3,
        TABLE_REPLACE_BEGIN = TABLE_EVENT + 4,   // like an edit but wipes out previous value
        TABLE_CUT           = TABLE_EVENT + 5,   // clipboard stuff
        TABLE_COPY          = TABLE_EVENT + 6,
        TABLE_PASTE         = TABLE_EVENT + 7,

        SERVER_REQUEST      = SERVER_EVENT + 1,

        STATE_CHANGED       = CHANGE_EVENT + 1,

        HYPERLINK_CLICKED   = HYPERLINK_EVENT + 1
    };

    /*
     * Type: Modifier
     * --------------
     * This enumeration type defines a set of constants used to check whether
     * modifiers are in effect.
     */
    enum Modifier {
        SHIFT_DOWN     = 1 << 0,
        CTRL_DOWN      = 1 << 1,
        META_DOWN      = 1 << 2,
        ALT_DOWN       = 1 << 3,
        ALT_GRAPH_DOWN = 1 << 4,
        BUTTON1_DOWN   = 1 << 5,
        BUTTON2_DOWN   = 1 << 6,
        BUTTON3_DOWN   = 1 << 7
    };

    enum EventHandlerType {
        HANDLER_EVENT,
        HANDLER_VOID
    };

    struct EventHandlerWrapper {
        QGEventHandler handler;
        QGEventHandlerVoid handlerVoid;
        EventHandlerType type;

        void fireEvent(const QGEvent& event) {
            if (type == HANDLER_EVENT) {
                handler(event);
            } else {
                handlerVoid();
            }
        }
    };


    /*
     * Type: KeyCode
     * -------------
     * This type defines the names of the key codes returned in a key event.
     */
    enum KeyCode {
        BACKSPACE_KEY = 8,
        TAB_KEY = 9,
        ENTER_KEY = 10,
        CLEAR_KEY = 12,
        ESCAPE_KEY = 27,
        PAGE_UP_KEY = 33,
        PAGE_DOWN_KEY = 34,
        END_KEY = 35,
        HOME_KEY = 36,
        LEFT_ARROW_KEY = 37,
        UP_ARROW_KEY = 38,
        RIGHT_ARROW_KEY = 39,
        DOWN_ARROW_KEY = 40,
        F1_KEY = 112,
        F2_KEY = 113,
        F3_KEY = 114,
        F4_KEY = 115,
        F5_KEY = 116,
        F6_KEY = 117,
        F7_KEY = 118,
        F8_KEY = 119,
        F9_KEY = 120,
        F10_KEY = 121,
        F11_KEY = 122,
        F12_KEY = 123,
        DELETE_KEY = 127,
        HELP_KEY = 156
    };



    QGEvent(EventClass eventClass = NULL_EVENT,
            EventType eventType = NULL_TYPE,
            const std::string& eventName = "",
            QGInteractor* source = nullptr);
    virtual ~QGEvent();
    static std::string classToString(EventClass eventClass);
    virtual std::string getActionCommand() const;
    virtual int getButton() const;
    virtual EventClass getClass() const;
    static long getCurrentTimeMS();
    virtual char getKeyChar() const;
    virtual int getKeyCode() const;
    virtual int getModifiers() const;
    virtual std::string getName() const;
    virtual QGInteractor* getSource() const;
    virtual long getTime() const;
    virtual EventType getType() const;
    virtual double getX() const;
    virtual double getY() const;

    /*
     * Method: isAltKeyDown
     * Usage: if (e.isAltKeyDown()) ...
     * --------------------------------
     * Returns <code>true</code> if the Alt key was held down during this event.
     */
    virtual bool isAltKeyDown() const;

    /*
     * Method: isCtrlKeyDown
     * Usage: if (e.isCtrlKeyDown()) ...
     * ---------------------------------
     * Returns <code>true</code> if the Ctrl key was held down during this event.
     */
    virtual bool isCtrlKeyDown() const;

    /*
     * Method: isCtrlOrCommandKeyDown
     * Usage: if (e.isCtrlOrCommandKeyDown()) ...
     * ---------------------------------
     * Returns <code>true</code> if the Ctrl key, or the Command key (Mac),
     * was held down during this event.
     */
    virtual bool isCtrlOrCommandKeyDown() const;

    /*
     * Method: isMetaKeyDown
     * Usage: if (e.isMetaKeyDown()) ...
     * ---------------------------------
     * Returns <code>true</code> if the Meta/Command key was held down during this event.
     */
    virtual bool isMetaKeyDown() const;

    /*
     * Method: isShiftKeyDown
     * Usage: if (e.isShiftKeyDown()) ...
     * ----------------------------------
     * Returns <code>true</code> if the Shift key was held down during this event.
     */
    virtual bool isShiftKeyDown() const;

    virtual void setButton(int button);
    virtual void setX(double x);
    virtual void setY(double y);

    virtual std::string toString() const;
    static std::string typeToString(EventType eventType);


private:
    std::string _actionCommand;
    int _button;
    EventClass _class;
    char _keyChar;
    int _keyCode;
    int _modifiers;
    std::string _name;
    QGInteractor* _source;
    long _time;
    EventType _type;
    double _x;
    double _y;

    friend class QGInteractor;
    friend class _Internal_QWidget;
};

std::ostream& operator <<(std::ostream& out, const QGEvent& event);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgevent_h
