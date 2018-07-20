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
#include "qgtypes.h"

class QGEvent;             // forward declaration
class QGInteractor;        // forward declaration
class QGObservable;        // forward declaration
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
        WINDOW_MINIMIZED    = WINDOW_EVENT + 5,
        WINDOW_RESTORED     = WINDOW_EVENT + 6,
        WINDOW_MAXIMIZED    = WINDOW_EVENT + 7,

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

        TABLE_UPDATED       = TABLE_EVENT + 1,   // when a cell's value gets set
        TABLE_SELECTED      = TABLE_EVENT + 2,   // cursor moves onto a cell
        TABLE_EDIT_BEGIN    = TABLE_EVENT + 3,   // user presses F2 or double clicks to start editing a cell
        TABLE_REPLACE_BEGIN = TABLE_EVENT + 4,   // user starts typing on a cell; like TABLE_EDIT_BEGIN but wipes out previous value
        TABLE_CUT           = TABLE_EVENT + 5,   // user cuts cell value to clipboard
        TABLE_COPY          = TABLE_EVENT + 6,   // user copies cell value to clipboard
        TABLE_PASTE         = TABLE_EVENT + 7,   // user pastes cell value from clipboard

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
        SHIFT_KEY = 16,
        CTRL_KEY = 17,
        ALT_KEY = 18,
        PAUSE_KEY = 19,
        CAPS_LOCK_KEY = 20,
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
        NUM_LOCK_KEY = 144,
        SCROLL_LOCK_KEY = 145,
        PRINT_SCREEN_KEY = 154,
        INSERT_KEY = 155,
        HELP_KEY = 156,
        META_KEY = 157,
        WINDOWS_KEY = 524,
        MENU_KEY = 525
    };

    QGEvent(EventClass eventClass = NULL_EVENT,
            EventType eventType = NULL_TYPE,
            const std::string& eventName = "",
            QGObservable* source = nullptr);
    virtual ~QGEvent();
    static std::string classToString(EventClass eventClass);
    virtual std::string getActionCommand() const;
    virtual int getButton() const;
    virtual EventClass getClass() const;
    virtual int getColumn() const;
    static long getCurrentTimeMS();
    virtual EventClass getEventClass() const;
    virtual EventType getEventType() const;
    virtual QGInteractor* getInteractor() const;
    virtual char getKeyChar() const;
    virtual int getKeyCode() const;
    virtual QGPoint getLocation() const;
    virtual int getModifiers() const;
    virtual std::string getName() const;
    virtual int getRow() const;
    virtual QGObservable* getSource() const;
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
     * Returns true if the user pressed the left mouse button.
     */
    virtual bool isLeftClick() const;

    /*
     * Returns true if the user pressed the middle mouse button.
     */
    virtual bool isMiddleClick() const;

    /*
     * Returns true if the user pressed the right mouse button.
     */
    virtual bool isRightClick() const;

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

    static std::string keyCodeToString(int keyCode);

    virtual void setActionCommand(const std::string& actionCommand);
    virtual void setButton(int button);
    virtual void setKeyChar(char keyChar);
    virtual void setKeyChar(const std::string& keyCharString);
    virtual void setKeyCode(int keyCode);
    virtual void setModifiers(Qt::KeyboardModifiers modifiers);
    virtual void setRowAndColumn(int row, int col);
    virtual void setSource(QGObservable* source);
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
    QGObservable* _source;
    long _time;
    EventType _type;
    double _x;
    double _y;
    int _row;
    int _col;

    friend class QGInteractor;
    friend class _Internal_QWidget;
};

std::ostream& operator <<(std::ostream& out, const QGEvent& event);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgevent_h
