/*
 * File: gevent.h
 * --------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gevent.h to replace Java version
 * @version 2018/07/06
 * - initial version
 */

#ifndef _gevent_h
#define _gevent_h

#include <functional>
#include <iostream>
#include <string>
#include <QEvent>
#include "gtypes.h"

class GEvent;              // forward declaration
class GInteractor;         // forward declaration
class GObservable;         // forward declaration
class _Internal_QWidget;   // forward declaration

// types for the event listener functions to be passed to various interactors
typedef std::function<void(GEvent)> GEventListener;
typedef std::function<void()>       GEventListenerVoid;

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
    HYPERLINK_EVENT = 0x2000,
    ANY_EVENT       = ACTION_EVENT | KEY_EVENT | TIMER_EVENT | WINDOW_EVENT
                    | MOUSE_EVENT | CLICK_EVENT | TABLE_EVENT | SERVER_EVENT
                    | CHANGE_EVENT | HYPERLINK_EVENT
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
    ACTION_MENU         = ACTION_EVENT + 2,

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
    TABLE_EDIT_CANCEL   = TABLE_EVENT + 5,   // user presses Esc or otherwise stops editing a cell
    TABLE_CUT           = TABLE_EVENT + 6,   // user cuts cell value to clipboard
    TABLE_COPY          = TABLE_EVENT + 7,   // user copies cell value to clipboard
    TABLE_PASTE         = TABLE_EVENT + 8,   // user pastes cell value from clipboard

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

/*
 * ...
 */
class GEvent {
public:
    // empty event handlers that can be passed that do nothing
    static GEventListener EMPTY_EVENT_LISTENER;
    static GEventListenerVoid EMPTY_EVENT_LISTENER_VOID;

    enum EventListenerType {
        HANDLER_EVENT,
        HANDLER_VOID
    };

    struct EventListenerWrapper {
        GEventListener handler;
        GEventListenerVoid handlerVoid;
        EventListenerType type;

        void fireEvent(const GEvent& event) {
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
        RETURN_KEY = 13,
        SHIFT_KEY = Qt::Key_Shift,
        CTRL_KEY = Qt::Key_Control,
        ALT_KEY = Qt::Key_Alt,
        PAUSE_KEY = 19,
        CAPS_LOCK_KEY = 20,
        ESCAPE_KEY = 27,
        PAGE_UP_KEY = Qt::Key_PageUp,
        PAGE_DOWN_KEY = Qt::Key_PageDown,
        END_KEY = Qt::Key_End,
        HOME_KEY = Qt::Key_Home,
        LEFT_ARROW_KEY = Qt::Key_Left,
        UP_ARROW_KEY = Qt::Key_Up,
        RIGHT_ARROW_KEY = Qt::Key_Right,
        DOWN_ARROW_KEY = Qt::Key_Down,
        F1_KEY = Qt::Key_F1,
        F2_KEY = Qt::Key_F2,
        F3_KEY = Qt::Key_F3,
        F4_KEY = Qt::Key_F4,
        F5_KEY = Qt::Key_F5,
        F6_KEY = Qt::Key_F6,
        F7_KEY = Qt::Key_F7,
        F8_KEY = Qt::Key_F8,
        F9_KEY = Qt::Key_F9,
        F10_KEY = Qt::Key_F10,
        F11_KEY = Qt::Key_F11,
        F12_KEY = Qt::Key_F12,
        DELETE_KEY = 127,
        NUM_LOCK_KEY = Qt::Key_NumLock,
        SCROLL_LOCK_KEY = Qt::Key_ScrollLock,
        PRINT_SCREEN_KEY = Qt::Key_Print,
        INSERT_KEY = Qt::Key_Insert,
        HELP_KEY = Qt::Key_Help,
        META_KEY = Qt::Key_Meta,
        WINDOWS_KEY = Qt::Key_Super_L,
        MENU_KEY = Qt::Key_Menu
    };

    GEvent(EventClass eventClass = NULL_EVENT,
            EventType eventType = NULL_TYPE,
            const std::string& eventName = "",
            GObservable* source = nullptr);
    virtual ~GEvent();
    static std::string classToString(EventClass eventClass);
    virtual std::string getActionCommand() const;
    virtual int getButton() const;
    virtual EventClass getClass() const;
    virtual int getColumn() const;
    static long getCurrentTimeMS();
    virtual EventClass getEventClass() const;
    virtual EventType getEventType() const;
    virtual GInteractor* getInteractor() const;
    virtual QEvent* getInternalEvent() const;
    virtual char getKeyChar() const;
    virtual int getKeyCode() const;
    virtual GPoint getLocation() const;
    virtual int getModifiers() const;
    virtual std::string getName() const;
    virtual int getRow() const;
    virtual GObservable* getSource() const;
    virtual long getTime() const;
    virtual EventType getType() const;
    virtual double getX() const;
    virtual double getY() const;

    virtual void ignore();

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
    virtual void setInternalEvent(QEvent* event);
    virtual void setKeyChar(char keyChar);
    virtual void setKeyChar(const std::string& keyCharString);
    virtual void setKeyCode(int keyCode);
    virtual void setModifiers(Qt::KeyboardModifiers modifiers);
    virtual void setRowAndColumn(int row, int col);
    virtual void setSource(GObservable* source);
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
    GObservable* _source;
    long _time;
    EventType _type;
    double _x;
    double _y;
    int _row;
    int _col;
    QEvent* _internalQtEvent;

    friend class GInteractor;
    friend class _Internal_QWidget;
};

std::ostream& operator <<(std::ostream& out, const GEvent& event);

// alias GEvent to all event types
typedef GEvent GActionEvent;
typedef GEvent GChangeEvent;
typedef GEvent GHyperlinkEvent;
typedef GEvent GKeyEvent;
typedef GEvent GMouseEvent;
typedef GEvent GServerEvent;
typedef GEvent GTableEvent;
typedef GEvent GTimerEvent;
typedef GEvent GWindowEvent;

// global functions for backward compatibility
// see geventqueue.cpp for implementation

/*
 * Function: getNextEvent
 * Usage: GEvent e = getNextEvent(mask);
 * -------------------------------------
 * Checks to see if there are any events of the desired type waiting on the
 * event queue.  If so, this function returns the event in exactly the same
 * fashion as <code>waitForEvent</code>; if not, <code>getNextEvent</code>
 * returns an invalid event.  The <code>mask</code> parameter is optional.
 * If it is missing, <code>getNextEvent</code> accepts any event.
 *
 * @deprecated
 * This function is deprecated and discouraged from use.
 * Instead of calling waitForClick in an event loop, you should attach an
 * event-listening function to the widget of choice using that object's methods
 * such as setActionListener or setMouseListener.
 */
GEvent getNextEvent(int mask = ANY_EVENT) Q_DECL_DEPRECATED;

/*
 * Function: waitForClick
 * Usage: GMouseEvent event = waitForClick();
 * ------------------------------------------
 * Waits for a mouse click to occur anywhere in any window.
 *
 * @deprecated
 * This function is deprecated and discouraged from use.
 * Instead of calling waitForClick in an event loop, you should attach an
 * event-listening function to the widget of choice using that object's methods
 * such as setActionListener or setMouseListener.
 */
GMouseEvent waitForClick() Q_DECL_DEPRECATED;

/*
 * Function: waitForEvent
 * Usage: GEvent e = waitForEvent(mask);
 * -------------------------------------
 * Dismisses the process until an event occurs whose type is covered by
 * the event mask.  The mask parameter is a combination of the events of
 * interest.  For example, to wait for a mouse event or an action event,
 * clients can use the following call:
 *
 *<pre>
 *    e = waitForEvent(MOUSE_EVENT + ACTION_EVENT);
 *</pre>
 *
 * The <code>mask</code> parameter is optional.  If it is missing,
 * <code>waitForEvent</code> accepts any event.
 *
 * <p>As a more sophisticated example, the following code is the canonical
 * event loop for an animated application that needs to respond to mouse,
 * key, and timer events:
 *
 *<pre>
 *    GTimer timer(ANIMATION_DELAY_IN_MILLISECONDS);
 *    timer.start();
 *    while (true) {
 *       GEvent e = waitForEvent(TIMER_EVENT + MOUSE_EVENT + KEY_EVENT);
 *       switch (e.getEventClass()) {
 *        case TIMER_EVENT:
 *          takeAnimationStep();
 *          break;
 *        case MOUSE_EVENT:
 *          handleMouseEvent(GMouseEvent(e));
 *          break;
 *        case KEY_EVENT:
 *          handleKeyEvent(GKeyEvent(e));
 *          break;
 *       }
 *    }
 *</pre>
 *
 * @deprecated
 * This function is deprecated and discouraged from use.
 * Instead of calling waitForClick in an event loop, you should attach an
 * event-listening function to the widget of choice using that object's methods
 * such as setActionListener or setMouseListener.
 */
GEvent waitForEvent(int mask = ANY_EVENT) Q_DECL_DEPRECATED;


#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gevent_h
