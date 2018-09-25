/*
 * File: gradiobutton.h
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gradiobutton.h to replace Java version
 * @version 2018/06/29
 * - added change event
 * @version 2018/06/25
 * - initial version
 */

#ifndef _gradiobutton_h
#define _gradiobutton_h

// signal that GUI system is in use (so it will be initialized)
#ifndef INTERNAL_INCLUDE
#define SPL_QT_GUI_IN_USE 1
#endif // INTERNAL_INCLUDE

#include <string>
#include <QWindow>
#include <QEvent>
#include <QButtonGroup>
#include <QMouseEvent>
#include <QRadioButton>
#include <QSize>
#include <QWidget>
#include "ginteractor.h"
#include "map.h"

class _Internal_QRadioButton;

/**
 * This interactor subclass represents an onscreen radio button.
 * Radio buttons are round buttons that can be "checked" by clicking them.
 * A radio button differs from a checkbox in that it is often part of a mutually
 * exclusive group of options, where at most one of the buttons can be checked
 * at a time.  Clicking one radio button from a group checks it and also unchecks
 * any other checked radio button from that same group.
 *
 * You can listen for clicks on a radio button using the setActionListener method,
 * passing the function you want to call on each click.
 */
class GRadioButton : public GInteractor {
public:
    /**
     * Creates a new radio button with the given text.
     * You can pass a string representing a logical group of radio buttons;
     * if you do, this radio button will be internally managed so that at most
     * one radio button from that group will be checked at any given time.
     * If no group is supplied, the radio button is put into a default group.
     */
    GRadioButton(const std::string& text = "", const std::string& group = "default", bool checked = false, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the radio button.
     */
    virtual ~GRadioButton();

    /* @inherit */
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the text next to the radio button.
     */
    virtual std::string getText() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns true if the radio button is currently checked.
     * Equivalent to isSelected.
     */
    virtual bool isChecked() const;

    /**
     * Returns true if the radio button is currently checked.
     * Equivalent to isChecked.
     */
    virtual bool isSelected() const;

    /**
     * Removes the action listener from this radio button so that it will
     * no longer call it when events occur.
     */
    virtual void removeActionListener();

    /**
     * Removes the double-click listener from this radio button so that it will
     * no longer call it when events occur.
     */
    virtual void removeDoubleClickListener();

    /**
     * Sets an action listener on this radio button so that it will be called
     * when the radio button is clicked.
     * Any existing action listener will be replaced.
     */
    virtual void setActionListener(GEventListener func);

    /**
     * Sets an action listener on this radio button so that it will be called
     * when the radio button is clicked.
     * Any existing action listener will be replaced.
     */
    virtual void setActionListener(GEventListenerVoid func);

    /**
     * Sets a listener on this radio button so that it will be called
     * when the radio button is double-clicked.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListener func);

    /**
     * Sets a listener on this radio button so that it will be called
     * when the radio button is double-clicked.
     * Any existing double-click listener will be replaced.
     */
    virtual void setDoubleClickListener(GEventListenerVoid func);

    /**
     * Sets whether the radio button should be checked.
     * Equivalent to setSelected.
     */
    virtual void setChecked(bool checked);

    /**
     * Sets whether the radio button should be checked.
     * Equivalent to setChecked.
     */
    virtual void setSelected(bool selected);

    /**
     * Sets the text that will appear next to the radio button.
     */
    virtual void setText(const std::string& text);

private:
    Q_DISABLE_COPY(GRadioButton)

    static Map<std::string, QButtonGroup*> _buttonGroups;
    static QButtonGroup* getButtonGroup(const std::string& group);

    _Internal_QRadioButton* _iqradioButton;

    friend class _Internal_QRadioButton;
};


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QRadioButton : public QRadioButton, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QRadioButton(GRadioButton* gradioButton, bool checked = false, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

signals:
    void doubleClicked();

public slots:
    void handleClick();

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
    GRadioButton* _gradioButton;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gradiobutton_h

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE
