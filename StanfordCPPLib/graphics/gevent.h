/*
 * File: gevent.h
 * --------------
 *
 * @author Marty Stepp
 * @version 2018/09/20
 * - removed deprecation warning on waitForEvent/Click global functions (for now)
 * @version 2018/09/07
 * - added doc comments for new documentation generation
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

#include "gtypes.h"

class QEvent;
class GEvent;
class GInteractor;
class GObservable;
class _Internal_QCanvas;
class _Internal_QCheckBox;
class _Internal_QPushButton;
class _Internal_QWidget;

/** Types for the event listener functions to be passed to various interactors. */
typedef std::function<void(GEvent)> GEventListener;

/** Types for the event listener functions to be passed to various interactors. */
typedef std::function<void()>       GEventListenerVoid;

/**
 * Represents all major categories of events.
 */
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
    SCROLL_EVENT    = 0x4000,
    ANY_EVENT       = ACTION_EVENT | KEY_EVENT | TIMER_EVENT | WINDOW_EVENT
                    | MOUSE_EVENT | CLICK_EVENT | TABLE_EVENT | SERVER_EVENT
                    | CHANGE_EVENT | HYPERLINK_EVENT | SCROLL_EVENT
};
// Note: If you add any new classes of events, you must also add logic to the
// GEvent::classToString function in gevent.cpp.


/**
 * Defines the event subtypes for all events.
 * An event type is a subcategory within an event class.
 */
enum EventType {
    NULL_TYPE            = 0,

    WINDOW_CLOSED        = WINDOW_EVENT + 1,
    WINDOW_RESIZED       = WINDOW_EVENT + 2,
    CONSOLE_CLOSED       = WINDOW_EVENT + 3,
    WINDOW_CLOSING       = WINDOW_EVENT + 4,
    WINDOW_MINIMIZED     = WINDOW_EVENT + 5,
    WINDOW_RESTORED      = WINDOW_EVENT + 6,
    WINDOW_MAXIMIZED     = WINDOW_EVENT + 7,

    ACTION_PERFORMED     = ACTION_EVENT + 1,
    ACTION_MENU          = ACTION_EVENT + 2,

    MOUSE_CLICKED        = MOUSE_EVENT + 1,
    MOUSE_PRESSED        = MOUSE_EVENT + 2,
    MOUSE_RELEASED       = MOUSE_EVENT + 3,
    MOUSE_MOVED          = MOUSE_EVENT + 4,
    MOUSE_DRAGGED        = MOUSE_EVENT + 5,
    MOUSE_ENTERED        = MOUSE_EVENT + 6,
    MOUSE_EXITED         = MOUSE_EVENT + 7,
    MOUSE_WHEEL_DOWN     = MOUSE_EVENT + 8,
    MOUSE_WHEEL_UP       = MOUSE_EVENT + 9,
    MOUSE_DOUBLE_CLICKED = MOUSE_EVENT + 10,

    KEY_PRESSED          = KEY_EVENT + 1,
    KEY_RELEASED         = KEY_EVENT + 2,
    KEY_TYPED            = KEY_EVENT + 3,

    TIMER_TICKED         = TIMER_EVENT + 1,

    TABLE_UPDATED        = TABLE_EVENT + 1,   // when a cell's value gets set
    TABLE_SELECTED       = TABLE_EVENT + 2,   // cursor moves onto a cell
    TABLE_EDIT_BEGIN     = TABLE_EVENT + 3,   // user presses F2 or double clicks to start editing a cell
    TABLE_REPLACE_BEGIN  = TABLE_EVENT + 4,   // user starts typing on a cell; like TABLE_EDIT_BEGIN but wipes out previous value
    TABLE_EDIT_CANCEL    = TABLE_EVENT + 5,   // user presses Esc or otherwise stops editing a cell
    TABLE_CUT            = TABLE_EVENT + 6,   // user cuts cell value to clipboard
    TABLE_COPY           = TABLE_EVENT + 7,   // user copies cell value to clipboard
    TABLE_PASTE          = TABLE_EVENT + 8,   // user pastes cell value from clipboard

    SERVER_REQUEST       = SERVER_EVENT + 1,

    STATE_CHANGED        = CHANGE_EVENT + 1,

    HYPERLINK_CLICKED    = HYPERLINK_EVENT + 1,

    SCROLL_SCROLLED      = SCROLL_EVENT + 1
};
// Note: If you add any new classes of events, you must also add logic to the
// GEvent::typeToString function in gevent.cpp.

/**
 * A set of constants used to check whether various event modifiers are in effect.
 * These constants can be combined in a single modifier int using bitwise operators.
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

/**
 * A GEvent represents a user action that has occurred on a graphical interactor.
 *
 * Older versions of this library used an event-polling model where the client
 * was encouraged to write a while (true) loop and call waitForEvent(...) to
 * get each event and process it.
 * The current design instead prefers that you attach event listener functions
 * to be called when events occur.
 * These listener functions can accept an optional GEvent as a parameter.
 * The GEvent object will contain information about the event that occurred.
 *
 * Older versions of this library had an inheritance hierarchy for various
 * event types, such as GMouseEvent, GKeyEvent, etc.
 * The current design has a single type GEvent that is a union of all data
 * needed by any kind of event.
 * The previous subclass names such as GMouseEvent are retained for backward
 * compatibility, but they are now just aliases for the type GEvent.
 */
class GEvent {
public:
    /**
     * An empty event handler that can be passed that does nothing.
     */
    static GEventListener EMPTY_EVENT_LISTENER;

    /**
     * An event listener that just prints the event that occurred.
     * This listener is useful for debugging.
     */
    static GEventListener LOG_EVENT;

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

    /**
     * Creates a new event of the given type.
     */
    GEvent(EventClass eventClass = NULL_EVENT,
            EventType eventType = NULL_TYPE,
            const std::string& eventName = "",
            GObservable* source = nullptr);

    /**
     * Frees memory allocated internally by the event.
     */
    virtual ~GEvent();

    /**
     * Converts an event class such as ACTION_PERFORMED
     * to a string such as "ACTION_PERFORMED".
     * @private
     */
    static std::string classToString(EventClass eventClass);

    /**
     * Returns the action command associated with the event.
     * For some interactors such as buttons, this will be the text of the
     * interactor.
     */
    virtual std::string getActionCommand() const;

    /**
     * Returns which mouse button was clicked, if this is a mouse event.
     * If this is not a mouse event, returns 0.
     */
    virtual int getButton() const;

    /**
     * Returns this event's class (major type such as MOUSE_EVENT).
     * Equivalent to getEventClass.
     */
    virtual EventClass getClass() const;

    /**
     * Returns the column that was interacted with, if this is a table event.
     * If this is not a table event, returns 0.
     */
    virtual int getColumn() const;

    /**
     * Returns the current time as a number of milliseconds elapsed since the
     * epoch of 1970/01/01 12:00am.
     * Used to supply timestamps to individual events.
     * @private
     */
    static long getCurrentTimeMS();

    /**
     * Returns this event's class (major type such as MOUSE_EVENT).
     * Equivalent to getClass.
     */
    virtual EventClass getEventClass() const;

    /**
     * Returns the event's type (minor type such as MOUSE_PRESSED).
     * Equivalent to getType.
     */
    virtual EventType getEventType() const;

    /**
     * Returns the source interactor that generated this event.
     */
    virtual GInteractor* getInteractor() const;

    /**
     * Returns the Qt event being wrapped by this event, if any.
     * If this event does not wrap a Qt event, returns nullptr.
     */
    virtual QEvent* getInternalEvent() const;

    /**
     * Returns the key character that was typed, if this is a key event.
     * If this is not a key event, returns '\0'.
     */
    virtual char getKeyChar() const;

    /**
     * Returns the integer key code that was typed, if this is a key event.
     * See the KeyCode enumeration for helpful constants for comparing key values.
     * If this is not a key event, returns 0.
     */
    virtual int getKeyCode() const;

    /**
     * Returns an (x, y) point representing the mouse position within the interactor
     * when this event occurred.
     * If this is not a mouse event, returns (0, 0).
     */
    virtual GPoint getLocation() const;

    /**
     * Returns the modifiers active during this event.
     * See the Modifiers enumeration for more information.
     */
    virtual int getModifiers() const;

    /**
     * Returns this event's name such as "click" or "keydown" or "actionperformed".
     */
    virtual std::string getName() const;

    /**
     * Returns this event's request URL, if this is a server URL event.
     * If this is not a server URL event, returns an empty string.
     */
    virtual std::string getRequestURL() const;

    /**
     * Returns the row that was interacted with, if this is a table event.
     * If this is not a table event, returns 0.
     */
    virtual int getRow() const;

    /**
     * Returns the source object that generated this event.
     */
    virtual GObservable* getSource() const;

    /**
     * Returns this event's timestamp, as a number of milliseconds elapsed
     * since the epoch of 1970/01/01 12:00am.
     */
    virtual long getTime() const;

    /**
     * Returns the event's type (major type such as MOUSE_EVENT).
     * Equivalent to getEventType.
     */
    virtual EventType getType() const;

    /**
     * Returns the x-coordinate of the mouse position within the interactor
     * when this event occurred.
     * If this is not a mouse event, returns 0.
     */
    virtual double getX() const;

    /**
     * Returns the y-coordinate of the mouse position within the interactor
     * when this event occurred.
     * If this is not a mouse event, returns 0.
     */
    virtual double getY() const;

    /**
     * Instructs the GUI system to ignore or cancel this event.
     * For example, if you listen to window-closing events and ignore them,
     * the window will stay open.
     */
    virtual void ignore();

    /**
     * Returns <code>true</code> if the Alt key was held down during this event.
     * If this is not a mouse or key event, returns false.
     */
    virtual bool isAltKeyDown() const;

    /**
     * Returns <code>true</code> if the Ctrl key was held down during this event.
     * If this is not a mouse or key event, returns false.
     */
    virtual bool isCtrlKeyDown() const;

    /**
     * Returns <code>true</code> if the Ctrl key, or the Command key (Mac),
     * was held down during this event.
     * If this is not a mouse or key event, returns false.
     */
    virtual bool isCtrlOrCommandKeyDown() const;

    /**
     * Returns true if the user pressed the mouse button multiple times.
     * If this is not a mouse event, returns false.
     */
    virtual bool isDoubleClick() const;

    /**
     * Returns true if the user pressed the left mouse button.
     * If this is not a mouse event, returns false.
     */
    virtual bool isLeftClick() const;

    /**
     * Returns true if the user pressed the middle mouse button.
     * (Note that not every mouse has a simple delineation of "left, right,
     * and middle" buttons; this was implemented on a standard 3-button mouse
     * with scroll wheel.)
     * If this is not a mouse event, returns false.
     */
    virtual bool isMiddleClick() const;

    /**
     * Returns true if the user pressed the right mouse button.
     * If this is not a mouse event, returns false.
     */
    virtual bool isRightClick() const;

    /**
     * Returns <code>true</code> if the Meta/Command key was held down during this event.
     * If this is not a mouse or key event, returns false.
     */
    virtual bool isMetaKeyDown() const;

    /**
     * Returns <code>true</code> if the Shift key was held down during this event.
     * If this is not a mouse or key event, returns false.
     */
    virtual bool isShiftKeyDown() const;

    /**
     * Converts a key code such as 67 into a string such as "A".
     * Works for special keys such as "Enter" and "Tab".
     */
    static std::string keyCodeToString(int keyCode);

    /**
     * @private
     */
    virtual void setActionCommand(const std::string& actionCommand);

    /**
     * @private
     */
    virtual void setButton(int button);

    /**
     * @private
     */
    virtual void setInternalEvent(QEvent* event);

    /**
     * @private
     */
    virtual void setKeyChar(char keyChar);

    /**
     * @private
     */
    virtual void setKeyChar(const std::string& keyCharString);

    /**
     * @private
     */
    virtual void setKeyCode(int keyCode);

    /**
     * @private
     */
    virtual void setModifiers(Qt::KeyboardModifiers modifiers);

    /**
     * @private
     */
    virtual void setRequestURL(const std::string& requestUrl);

    /**
     * @private
     */
    virtual void setRowAndColumn(int row, int col);

    /**
     * @private
     */
    virtual void setSource(GObservable* source);

    /**
     * @private
     */
    virtual void setX(double x);

    /**
     * @private
     */
    virtual void setY(double y);

    /**
     * Returns a text representation of the event for debugging.
     */
    virtual std::string toString() const;

    /**
     * Converts an event type such as MOUSE_EVENT to a string such as
     * "MOUSE_EVENT".
     */
    static std::string typeToString(EventType eventType);

private:
    /*
     * Represents the two types of event listeners.
     */
    enum EventListenerType {
        HANDLER_EVENT,
        HANDLER_VOID
    };

    /*
     * A wrapper that can hold either of the two types of event listeners.
     */
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

    // member variables
    std::string _actionCommand;
    int _button;
    EventClass _class;
    char _keyChar;
    int _keyCode;
    int _modifiers;
    std::string _name;
    std::string _requestUrl;
    GObservable* _source;
    long _time;
    EventType _type;
    double _x;
    double _y;
    int _row;
    int _col;
    QEvent* _internalQtEvent;

    friend class GInteractor;
    friend class GObservable;
    friend class _Internal_QWidget;
};

/**
 * Writes the given event to the given output stream.
 */
std::ostream& operator <<(std::ostream& out, const GEvent& event);

// alias GEvent to all event types
typedef GEvent GActionEvent;
typedef GEvent GChangeEvent;
typedef GEvent GHyperlinkEvent;
typedef GEvent GKeyEvent;
typedef GEvent GMouseEvent;
typedef GEvent GScrollEvent;
typedef GEvent GServerEvent;
typedef GEvent GTableEvent;
typedef GEvent GTimerEvent;
typedef GEvent GWindowEvent;

// global functions for backward compatibility
// see geventqueue.cpp for implementation

/**
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
GEvent getNextEvent(int mask = ANY_EVENT) /*Q_DECL_DEPRECATED*/;

/**
 * Waits for a mouse click to occur anywhere in any window,
 * returning the event that occurred.
 *
 * @deprecated
 * This function is deprecated and discouraged from use.
 * Instead of calling waitForClick in an event loop, you should attach an
 * event-listening function to the widget of choice using that object's methods
 * such as setActionListener or setMouseListener.
 */
GMouseEvent waitForClick() /*Q_DECL_DEPRECATED*/;

/**
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
GEvent waitForEvent(int mask = ANY_EVENT) /*Q_DECL_DEPRECATED*/;

#endif // _gevent_h
