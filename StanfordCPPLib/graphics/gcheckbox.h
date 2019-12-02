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


#ifndef _gcheckbox_h
#define _gcheckbox_h

#include <string>
#include <QCheckBox>

#include "ginteractor.h"

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
    ~GCheckBox() override;

    /* @inherit */
    std::string getActionCommand() const override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /**
     * Returns the text next to the checkbox.
     */
    virtual std::string getText() const;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

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
    std::string getActionEventType() const override;

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
    void detach() override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    QSize sizeHint() const override;

signals:
    void doubleClicked();

public slots:
    void handleStateChange(int);

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
    GCheckBox* _gcheckBox;

    friend class GCheckBox;
};

#endif // _gcheckbox_h
