/*
 * File: gcheckbox.h
 * -----------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added key event support
 * @version 2018/10/06
 * - added toggle()
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gcheckbox.h to replace Java version
 * @version 2018/06/29
 * - added change event
 * @version 2018/06/25
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gcheckbox_h
#define _gcheckbox_h

// signal that GUI system is in use (so it will be initialized)
#ifndef INTERNAL_INCLUDE
#define SPL_QT_GUI_IN_USE 1
#endif // INTERNAL_INCLUDE

#include <functional>
#include <string>
#include <QWindow>
#include <QEvent>
#include <QCheckBox>
#include <QMouseEvent>
#include <QSize>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

class _Internal_QCheckBox;

/**
 * This interactor subclass represents an onscreen check box.
 * Clicking once on the check box selects it; clicking again removes the selection.
 * You can listen for clicks on a checkbox by setting an action listener,
 * passing the function you want to call on each click.
 */
class GCheckBox : public GInteractor {
public:
    /**
     * Creates a checkbox with the given text.
     * You can pass an optional second parameter to initially check the box.
     */
    GCheckBox(const std::string& text = "", bool checked = false, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the checkbox.
     */
    virtual ~GCheckBox() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the text next to the checkbox.
     */
    virtual std::string getText() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns true if the checkbox is currently checked.
     * Equivalent to isSelected.
     */
    virtual bool isChecked() const;

    /**
     * Returns true if the checkbox is currently checked.
     * Equivalent to isChecked.
     */
    virtual bool isSelected() const;

    /**
     * Sets whether the checkbox should be checked.
     * Equivalent to setSelected.
     */
    virtual void setChecked(bool checked);

    /**
     * Sets whether the checkbox should be checked.
     * Equivalent to setChecked.
     */
    virtual void setSelected(bool selected);

    /**
     * Sets the text that will appear next to the checkbox.
     */
    virtual void setText(const std::string& text);

    /**
     * Reverses the checked state of the box, setting it to be checked if it was
     * unchecked or to be unchecked if it was checked.
     */
    virtual void toggle();

protected:
    /**
     * @private
     */
    virtual std::string getActionEventType() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(GCheckBox)

    _Internal_QCheckBox* _iqcheckBox;

    friend class _Internal_QCheckBox;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QCheckBox : public QCheckBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCheckBox(GCheckBox* gcheckBox, bool checked = false, QWidget* parent = nullptr);
    virtual void detach() Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

signals:
    void doubleClicked();

public slots:
    void handleStateChange(int);

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) Q_DECL_OVERRIDE;

private:
    GCheckBox* _gcheckBox;

    friend class GCheckBox;
};

#endif // _gcheckbox_h
