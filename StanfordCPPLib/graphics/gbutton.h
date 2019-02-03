/*
 * File: gbutton.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gbutton.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gbutton_h
#define _gbutton_h

#include <string>
#include <QWindow>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QSize>
#include <QToolButton>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

class _Internal_QPushButton;

/**
 * This interactor subclass represents an onscreen button.
 * You can listen for clicks on a button using the setActionListener method,
 * passing the function you want to call on each click.
 */
class GButton : public GInteractor {
public:
    /**
     * Creates a button with the specified text label and optional icon.
     */
    GButton(const std::string& text = "", const std::string& iconFileName = "", QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the button.
     */
    virtual ~GButton() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string getAccelerator() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the button's text.
     * @return the text
     */
    virtual std::string getText() const;

    /**
     * Returns the button's text position relative to its icon.
     * The default is TEXT_BESIDE_ICON, but it can be changed to TEXT_UNDER_ICON
     * by calling the setTextPosition method.
     */
    virtual GInteractor::TextPosition getTextPosition() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Removes the action listener from this button so that it will no longer
     * call it when events occur.
     */
    virtual void removeActionListener();

    /**
     * Removes the double-click listener from this button so that it will no longer
     * call it when events occur.
     */
    virtual void removeDoubleClickListener();

    /* @inherit */
    virtual void setAccelerator(const std::string& accelerator) Q_DECL_OVERRIDE;

    /**
     * Sets an action listener on this button so that it will be called
     * when the button is clicked.
     * Any existing action listener will be replaced.
     */
    virtual void setActionListener(GEventListener func);

    /**
     * Sets an action listener on this button so that it will be called
     * when the button is clicked.
     * Any existing action listener will be replaced.
     */
    virtual void setActionListener(GEventListenerVoid func);

    /**
     * Sets a listener on this button so that it will be called
     * when the button is double-clicked.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListener func);

    /**
     * Sets a listener on this button so that it will be called
     * when the button is double-clicked.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListenerVoid func);

    /* @inherit */
    virtual void setIcon(const std::string& filename, bool retainIconSize = true) Q_DECL_OVERRIDE;

    /**
     * Sets the text on the button to be the given text.
     */
    virtual void setText(const std::string& text);

    /**
     * Sets the button's text position relative to its icon.
     * The default is TEXT_BESIDE_ICON, but it can be changed to TEXT_UNDER_ICON.
     */
    virtual void setTextPosition(GInteractor::TextPosition position);

    /**
     * Sets the button's text position relative to its icon.
     * Clients should not call this method; it is a relic from an older Java-based
     * GUI system.
     * @private
     */
    virtual void setTextPosition(SwingConstants horizontal, SwingConstants vertical) Q_DECL_DEPRECATED;

private:
    Q_DISABLE_COPY(GButton)
    _Internal_QPushButton* _iqpushbutton;

    friend class _Internal_QPushButton;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QPushButton : public QToolButton, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QPushButton(GButton* button, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

signals:
    void doubleClicked();

public slots:
    void handleClick();

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
    GButton* _gbutton;

    friend class GButton;
};

#endif // _gbutton_h
