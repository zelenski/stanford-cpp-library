/*
 * File: ginteractor.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added set/removeActionListener
 * - added set/removeClickListener
 * - added set/removeDoubleClickListener
 * - added set/removeKeyListener
 * - added set/removeMouseListener
 * @version 2019/04/22
 * - added setIcon with QIcon and QPixmap
 * @version 2018/09/20
 * - added read/write lock to avoid race conditions
 * @version 2018/09/05
 * - added getContainer and is/setVisible logic
 * @version 2018/08/23
 * - renamed to ginteractor.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */


#ifndef _ginteractor_h
#define _ginteractor_h

#include <string>
#include <QReadWriteLock>
#include <QWidget>

#include "gevent.h"
#include "gobservable.h"

class QPixmap;
class QIcon;

class GContainer;
class GDiffGui;
class GWindow;
class _Internal_QWidget;

/**
 * This abstract class is the superclass for all graphical interactors.
 * In most applications, interactors will be added to a control strip along
 * one of the regions of a GWindow.
 */
class GInteractor : public GObservable {
public:
    /**
     * The places where an interactor can place its text relative to its icon.
     */
    enum TextPosition {
        TEXT_BESIDE_ICON,
        TEXT_UNDER_ICON,
        TEXT_ONLY
    };

    /**
     * Initializes a newly created interactor.
     * If the Qt graphical subsystem has not yet been initialized, constructing
     * an interactor will initialize it.
     */
    GInteractor();

    /**
     * Frees memory allocated internally by the interactor.
     */
    virtual ~GInteractor();

    /**
     * Adds an event listener to be notified when this interactor is clicked or
     * generally interacted with.
     *
     * @deprecated does nothing; use setActionListener instead
     */
    virtual void addActionListener() Q_DECL_DEPRECATED;   // does nothing

    /**
     * Returns true if this interactor is currently accepting events.
     * Initially true.
     * An interactor must be visible and added to an onscreen window to receive events.
     */
    bool eventsEnabled() const override;

    /**
     * Returns a string representing a hotkey for this interactor,
     * or an empty string if no accelerator has been set.
     * @return an accelerator such as "Ctrl-S"
     */
    virtual std::string getAccelerator() const;

    /**
     * Returns an action command for this interactor,
     * which is a semi-unique string you can use to identify it when events occur.
     * For example, for buttons, the default action command is the button's text.
     */
    virtual std::string getActionCommand() const;

    /**
     * Returns the background color of the interactor as a string.
     * @return a string such as "blue" or "#7700ff"
     */
    virtual std::string getBackground() const;

    /**
     * Returns the background color of the interactor as an RGB integer.
     * @return an integer such as 0x7700ff
     */
    virtual int getBackgroundInt() const;

    /**
     * Returns a rectangle representing the x/y position and size of this interactor.
     */
    virtual GRectangle getBounds() const;

    /**
     * Returns the foreground/text color of the interactor as a string.
     * Equivalent to getForeground.
     * @return a string such as "blue" or "#7700ff"
     */
    virtual std::string getColor() const;

    /**
     * Returns the foreground/text color of the interactor as an RGB integer.
     * Equivalent to getForegroundInt.
     * @return an integer such as 0x7700ff
     */
    virtual int getColorInt() const;

    /**
     * Returns a pointer to the onscreen container holding this interactor.
     * When an interactor is created, its container is initially null.
     * This will become non-null automatically if you add the interactor to
     * a window or other layout container.
     * Interactors must be added to a container or window to receive events
     * or to become visible on the screen.
     * @return the container, or nullptr if interactor has not yet been
     *         added to any container
     */
    virtual GContainer* getContainer() const;

    /**
     * Returns the font of this interactor's text as a font string such as
     * "Helvetica-12-Bold".
     * @return a font string such as "Helvetica-12-Bold"
     */
    virtual std::string getFont() const;

    /**
     * Returns the foreground/text color of the interactor as a string.
     * Equivalent to getColor.
     * @return a string such as "blue" or "#7700ff"
     */
    virtual std::string getForeground() const;

    /**
     * Returns the foreground/text color of the interactor as an RGB integer.
     * Equivalent to getColorInt.
     * @return an integer such as 0x7700ff
     */
    virtual int getForegroundInt() const;

    /**
     * Returns the current onscreen height of this interactor in pixels.
     */
    virtual double getHeight() const;

    /**
     * Returns a globally unique identifier for this interactor,
     * which is set when the interactor is constructed.
     * These IDs can be useful for debugging to help identify interactors uniquely.
     */
    virtual int getID() const;

    /**
     * Returns the file name of the icon associated with this interactor,
     * or an empty string if no icon has been set.
     * Not all types of interactors support icons.
     */
    virtual std::string getIcon() const;

    /**
     * Returns a direct pointer to the internal Qt widget being wrapped by this
     * interactor.  This must be overridden by all interactor subclasses.
     * Students/clients generally should not need to call this.
     * @private
     */
    virtual _Internal_QWidget* getInternalWidget() const = 0;

    /**
     * Returns an (x, y) point representing the onscreen location of the top-left
     * corner of this interactor within its containing window.
     */
    virtual GPoint getLocation() const;

    /**
     * Returns the minimum height in pixels that this interactor will permit
     * itself to be resized to.
     */
    virtual double getMinimumHeight() const;

    /**
     * Returns a GDimension structure representing the minimum size in pixels
     * that this interactor will permit itself to be resized to.
     */
    virtual GDimension getMinimumSize() const;

    /**
     * Returns the minimum width in pixels that this interactor will permit
     * itself to be resized to.
     */
    virtual double getMinimumWidth() const;

    /**
     * Returns a one-character 'mnemonic' shortcut that can be used to activate
     * this interactor.  This method is a relic from a previous Java-based
     * GUI system and should no longer be used.
     * @deprecated
     * @private
     */
    virtual char getMnemonic() const Q_DECL_DEPRECATED;

    /**
     * Returns a string representing a unique name for this interactor.
     * The default name string uses the interactor's type and its ID to make
     * a string like "GButton_14", but you can override this by calling setName.
     * @return a string such as "GButton_14"
     */
    virtual std::string getName() const;

    /**
     * Returns the height in pixels that this interactor would prefer to be,
     * which would exactly fit its contents with no stretching or scrollbars.
     */
    virtual double getPreferredHeight() const;

    /**
     * Returns a GDimension structure storing the width and height in pixels
     * that this interactor would prefer to be,
     * which would exactly fit its contents with no stretching or scrollbars.
     */
    virtual GDimension getPreferredSize() const;

    /**
     * Returns the height in pixels that this interactor would prefer to be,
     * which would exactly fit its contents with no stretching or scrollbars.
     */
    virtual double getPreferredWidth() const;

    /**
     * Returns a GDimension structure storing the current onscreen width and height
     * of this interactor in pixels.
     */
    virtual GDimension getSize() const;

    /**
     * Returns a string representing the class name of this interactor,
     * such as "GButton" or "GCheckBox".
     * All subclasses of GInteractor must implement this method.
     * @return a string such as "GCheckBox"
     */
    std::string getType() const override = 0;

    /**
     * Returns a direct pointer to the internal Qt widget being wrapped by this
     * interactor.  This must be overridden by all interactor subclasses.
     * Students/clients generally should not need to call this.
     * @private
     */
    virtual QWidget* getWidget() const = 0;

    /**
     * Returns the current onscreen width of this interactor in pixels.
     */
    virtual double getWidth() const;

    /**
     * Returns the x-coordinate of the top-left pixel of this interactor
     * within its onscreen window.
     */
    virtual double getX() const;

    /**
     * Returns the y-coordinate of the top-left pixel of this interactor
     * within its onscreen window.
     */
    virtual double getY() const;

    /**
     * Returns true if the given x/y pixel is within the bounds of this interactor.
     */
    virtual bool inBounds(double x, double y) const;

    /**
     * Returns true if the given x/y pixel is within the bounds of this interactor.
     */
    virtual bool inBounds(int x, int y) const;

    /**
     * Returns true if this interactor is currently enabled.
     * Most interactors begin as enabled but can be disabled to stop them from
     * being able to be clicked on or otherwise emit events.
     */
    virtual bool isEnabled() const;

    /**
     * Returns true if the interactor is visible on the screen.
     * Interactors will not be visible until they are added to an onscreen
     * window or container.
     */
    virtual bool isVisible() const;

    /**
     * Removes the action listener from this interactor so that it will no longer
     * call it when events occur.
     */
    virtual void removeActionListener();

    /**
     * Removes the click listener from this interactor so that it will no longer
     * call it when events occur.
     */
    virtual void removeClickListener();

    /**
     * Removes the double-click listener from this interactor so that it will no longer
     * call it when events occur.
     */
    virtual void removeDoubleClickListener();

    /**
     * Removes the key listener from this interactor so that it will no longer
     * call it when key events occur.
     */
    virtual void removeKeyListener();

    /**
     * Removes the mouse listener from this interactor so that it will no longer
     * call it when events occur.
     */
    virtual void removeMouseListener();

    /**
     * Transfers keyboard focus to this interactor.
     */
    virtual void requestFocus();

    /**
     * Sets an accelerator hotkey for this interactor, such as "Ctrl-S".
     * Not all interactor types support accelerators.
     * @param accelerator a hotkey such as "Ctrl-S"
     */
    virtual void setAccelerator(const std::string& accelerator);

    /**
     * Sets the action command for this interactor.
     * The action command is meant to be a semi-unique string you can use to
     * identify the interactor when events occur.
     * For example, for buttons, the default action command is the button's text,
     * but you can change it to a different string if you prefer.
     * The main usage of this feature is if you want to use the same function as
     * an event listener for many interactors, you can use the action command
     * to help distinguish which interactor generates each event.
     */
    virtual void setActionCommand(const std::string& actionCommand);

    /**
     * Sets an action listener on this interactor so that it will be called
     * when it is interacted with in its primary way.
     * For example, if this interactor is a button, this will fire when it is clicked.
     * Any existing action listener will be replaced.
     */
    virtual void setActionListener(GEventListener func);

    /**
     * Sets an action listener on this interactor so that it will be called
     * when it is interacted with in its primary way.
     * For example, if this interactor is a button, this will fire when it is clicked.
     * Any existing action listener will be replaced.
     */
    virtual void setActionListener(GEventListenerVoid func);

    /**
     * Sets the background color of the interactor to the color represented by
     * the given RGB integer.
     * @param rgb an RGB integer such as 0x7700ff
     */
    virtual void setBackground(int rgb);

    /**
     * Sets the background color of the interactor to the color represented by
     * the given string.
     * @param color a string such as "blue" or "#7700ff"
     */
    virtual void setBackground(const std::string& color);

    /**
     * Sets the size and location of the widget.
     * In general you should avoid explicitly sizing and positioning widgets
     * in this way; instead, use containers and regions to help you lay out
     * widgets at the proper sizes.
     */
    virtual void setBounds(double x, double y, double width, double height);

    /**
     * Sets the size and location of the widget.
     * In general you should avoid explicitly sizing and positioning widgets
     * in this way; instead, use containers and regions to help you lay out
     * widgets at the proper sizes.
     */
    virtual void setBounds(const GRectangle& size);

    /**
     * Sets a mouse listener on this interactor so that it will be called
     * when the mouse is clicked on it.
     * Any existing click listener will be replaced.
     */
    virtual void setClickListener(GEventListener func);

    /**
     * Sets a mouse listener on this interactor so that it will be called
     * when the mouse is clicked on it.
     * Any existing click listener will be replaced.
     */
    virtual void setClickListener(GEventListenerVoid func);

    /**
     * Sets the foreground/text color of the interactor to the color represented by
     * the given RGB integer.
     * Equivalent to setForeground.
     * @param rgb an RGB integer such as 0x7700ff
     */
    virtual void setColor(int rgb);

    /**
     * Sets the foreground/text color of the interactor to the color represented by
     * the given string.
     * Equivalent to setForeground.
     * @param color a string such as "blue" or "#7700ff"
     */
    virtual void setColor(const std::string& color);

    /**
     * Sets a mouse listener on this interactor so that it will be called
     * when the mouse is double-clicked on it.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListener func);

    /**
     * Sets a mouse listener on this interactor so that it will be called
     * when the mouse is double-clicked on it.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListenerVoid func);

    /**
     * Sets whether this interactor is currently enabled.
     * Most interactors begin as enabled but can be disabled to stop them from
     * being able to be clicked on or otherwise emit events.
     */
    virtual void setEnabled(bool value);

    /**
     * Sets the font used by this widget to the given Qt font.
     * Clients should generally use the string version of this method.
     */
    virtual void setFont(const QFont& font);

    /**
     * Sets the font used by this widget to the font represented by the
     * given font string, such as "Helvetica-16-Bold".
     * @param font a font string such as "Helvetica-16-Bold"
     */
    virtual void setFont(const std::string& font);

    /**
     * Sets the foreground/text color of the interactor to the color represented by
     * the given RGB integer.
     * Equivalent to setColor.
     * @param rgb an RGB integer such as 0x7700ff
     */
    virtual void setForeground(int rgb);

    /**
     * Sets the foreground/text color of the interactor to the color represented by
     * the given string.
     * Equivalent to setColor.
     * @param color a string such as "blue" or "#7700ff"
     */
    virtual void setForeground(const std::string& color);

    /**
     * Sets the onscreen height of the interactor in pixels.
     * @throw ErrorException if height is negative
     */
    virtual void setHeight(double height);

    /**
     * Sets the icon associated with this interactor.
     * Not all types of interactors support icons.
     * @param icon the icon to use
     */
    virtual void setIcon(const QIcon& icon);

    /**
     * Sets the icon associated with this interactor.
     * Not all types of interactors support icons.
     * @param icon the icon to use
     */
    virtual void setIcon(const QPixmap& icon);

    /**
     * Sets the file name of the icon associated with this interactor,
     * or an empty string if no icon has been set.
     * Not all types of interactors support icons.
     * @param filename icon file path to use
     * @param retainIconSize true if icon should stay at its existing pixel size (default),
     *                       or false if it should be resized to fit the interactor
     */
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);

    /**
     * Sets a key listener on this interactor so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListener func);

    /**
     * Sets a key listener on this interactor so that it will be called
     * when the user presses any key.
     * Any existing key listener will be replaced.
     */
    virtual void setKeyListener(GEventListenerVoid func);

    /**
     * Sets the onscreen x/y-coordinate of the top-left corner of the interactor
     * relative to its window.
     * Generally clients should not call this and should instead use containers
     * and layout regions to position interactors.
     */
    virtual void setLocation(double x, double y);

    /**
     * Sets the minimum size in pixels that this interactor will permit itself
     * to be resized to.
     * @throw ErrorException if width or height is negative
     */
    virtual void setMinimumSize(double width, double height);

    /**
     * Sets the minimum size in pixels that this interactor will permit itself
     * to be resized to.
     * @throw ErrorException if width or height is negative
     */
    virtual void setMinimumSize(const GDimension& size);

    /**
     * Sets a one-character 'mnemonic' shortcut that can be used to activate
     * this interactor.  This method is a relic from a previous Java-based
     * GUI system and should no longer be used.
     * @deprecated
     * @private
     */
    virtual void setMnemonic(char mnemonic) Q_DECL_DEPRECATED;

    /**
     * Sets a mouse listener on this interactor so that it will be called
     * when the mouse is moved or clicked on it.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListener func);

    /**
     * Sets a mouse listener on this interactor so that it will be called
     * when the mouse is moved or clicked on it.
     * Any existing mouse listener will be replaced.
     */
    virtual void setMouseListener(GEventListenerVoid func);

    /**
     * Sets a string representing a unique name for this interactor.
     * The default name string uses the interactor's type and its ID to make
     * a string like "GButton_14", but you can override this by calling setName.
     * @param name a string such as "GButton_14"
     */
    virtual void setName(const std::string& name);

    /**
     * Sets the height in pixels that this interactor would prefer to be.
     * Normally clients do not need to call this method; the interactor can
     * figure out its own preferred size.
     * But calling it can help you to hint to the container/layout system if
     * you want a given interactor to "prefer" to make itself larger or smaller
     * for the purposes of your particular program.
     */
    virtual void setPreferredHeight(double height);

    /**
     * Sets the width and height in pixels that this interactor would prefer to be.
     * Normally clients do not need to call this method; the interactor can
     * figure out its own preferred size.
     * But calling it can help you to hint to the container/layout system if
     * you want a given interactor to "prefer" to make itself larger or smaller
     * for the purposes of your particular program.
     */
    virtual void setPreferredSize(double width, double height);


    /**
     * Sets the size in pixels that this interactor would prefer to be.
     * Normally clients do not need to call this method; the interactor can
     * figure out its own preferred size.
     */
    virtual void setPreferredSize(const GDimension& size);


    /**
     * Sets the width in pixels that this interactor would prefer to be.
     * Normally clients do not need to call this method; the interactor can
     * figure out its own preferred size.
     */
    virtual void setPreferredWidth(double width);

    /**
     * Sets the onscreen width and height of the interactor in pixels.
     * @throw ErrorException if width or height is negative
     */
    virtual void setSize(double width, double height);

    /**
     * Sets the onscreen width and height of the interactor in pixels.
     * @throw ErrorException if width or height is negative
     */
    virtual void setSize(const GDimension& size);

    /**
     * Sets a "tooltip" that will appear if the user hovers their mouse
     * over the interactor.
     * Set an empty string to clear the tooltip.
     */
    virtual void setTooltip(const std::string& tooltipText);

    /**
     * Returns true if the interactor is visible on the screen.
     * Interactors will not be visible until they are added to an onscreen
     * window or container.
     * If you call setVisible on an interactor that is not in any onscreen
     * container, it will have no effect.
     */
    virtual void setVisible(bool visible);

    /**
     * Sets the onscreen width of the interactor in pixels.
     * @throw ErrorException if width is negative
     */
    virtual void setWidth(double width);

    /**
     * Sets the onscreen x-coordinate of the top-left corner of the interactor
     * relative to its window.
     * Generally clients should not call this and should instead use containers
     * and layout regions to position interactors.
     */
    virtual void setX(double x);

    /**
     * Sets the onscreen y-coordinate of the top-left corner of the interactor
     * relative to its window.
     * Generally clients should not call this and should instead use containers
     * and layout regions to position interactors.
     */
    virtual void setY(double y);

protected:
    /**
     * @private
     */
    std::string _actionCommand;

    /**
     * @private
     */
    std::string _actionEventType;

    /**
     * @private
     */
    std::string _icon;

    /**
     * @private
     */
    std::string _name;

    /**
     * @private
     */
    int _id;

    /**
     * @private
     */
    GContainer* _container;

    /**
     * @private
     */
    QReadWriteLock _lock;    // avoid thread race conditions

    /**
     * @private
     */
    static QWidget* getInternalParent(QWidget* parent);

    /**
     * @private
     */
    virtual std::string getActionEventType() const;

    /**
     * @private
     */
    virtual std::string getDefaultInteractorName() const;

    /**
     * @private
     */
    virtual void lockForRead();

    /**
     * @private
     */
    virtual void lockForReadConst() const;

    /**
     * @private
     */
    virtual void lockForWrite();

    /**
     * @private
     */
    virtual void lockForWriteConst() const;

    /**
     * @private
     */
    static std::string normalizeAccelerator(const std::string& accelerator);

    /**
     * @private
     */
    virtual void setContainer(GContainer* container);

    /**
     * @private
     */
    virtual void unlock();

    /**
     * @private
     */
    virtual void unlockConst() const;

    friend class GContainer;
    friend class GDiffGui;
    friend class GWindow;
    friend class _Internal_QWidget;

private:
    static int _interactorCount;
};


/**
 * A class representing internal Qt widgets that are wrapped inside of
 * GInteractor objects.
 * Every GInteractor subclass must define a class that is a subclass of
 * _Internal_QWidget to be wrapped by that interactor.
 * @private
 */
class _Internal_QWidget {
public:
    _Internal_QWidget();
    virtual ~_Internal_QWidget();
    virtual void detach();
    virtual QSize getMinimumSize() const;
    virtual bool hasMinimumSize() const;
    virtual QSize getPreferredSize() const;
    virtual bool hasPreferredSize() const;
    virtual void setMinimumSize(double width, double height);
    virtual void setMinimumSize(const QSize& size);
    virtual void setPreferredSize(double width, double height);
    virtual void setPreferredSize(const QSize& size);

private:
    GDimension _minimumSize;
    GDimension _preferredSize;

    friend class GInteractor;
};

/**
 * A generic simple GInteractor that wraps any Qt QWidget you pass it.
 * This is meant to make the library extensible and allow you to wrap other
 * Qt widgets that were not built into the original library.
 * @private
 */
template <typename T>
class GGenericInteractor : public GInteractor {
public:
    GGenericInteractor(T* widget)
            : _widget(widget) {
        _iqwidget = new _Internal_QWidget();   // dummy
    }

    _Internal_QWidget* getInternalWidget() const override {
        return _iqwidget;
    }

    std::string getType() const override {
        std::string typeName = typeid(T).name();
        return std::string("GGenericInteractor<") + typeName + ">";
    }

    QWidget* getWidget() const override {
        return _widget;
    }

private:
    _Internal_QWidget* _iqwidget;
    T* _widget;
};

#endif // _ginteractor_h
