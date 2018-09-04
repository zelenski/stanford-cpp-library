/*
 * File: gradiobutton.h
 * --------------------
 *
 * @author Marty Stepp
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

// forward declaration
class _Internal_QRadioButton;

/*
 * ...
 */
class GRadioButton : public GInteractor {
public:
    GRadioButton(const std::string& text = "", const std::string& group = "default", bool checked = false, QWidget* parent = nullptr);
    virtual ~GRadioButton();
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual std::string getText() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual bool isChecked() const;
    virtual bool isSelected() const;
    virtual void removeActionListener();
    virtual void removeDoubleClickListener();
    virtual void setActionListener(GEventListener func);
    virtual void setActionListener(GEventListenerVoid func);
    virtual void setDoubleClickListener(GEventListener func);
    virtual void setDoubleClickListener(GEventListenerVoid func);
    virtual void setChecked(bool checked);
    virtual void setSelected(bool selected);
    virtual void setText(const std::string& text);

private:
    Q_DISABLE_COPY(GRadioButton)

    static Map<std::string, QButtonGroup*> _buttonGroups;
    QButtonGroup* getButtonGroup(const std::string& group);

    _Internal_QRadioButton* _iqradioButton;

    friend class _Internal_QRadioButton;
};


// Internal class; not to be used by clients.
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
