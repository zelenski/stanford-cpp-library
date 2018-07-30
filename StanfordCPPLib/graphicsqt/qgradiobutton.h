/*
 * File: qgradiobutton.h
 * ---------------------
 *
 * @version 2018/06/29
 * - added change event
 * @version 2018/06/25
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgradiobutton_h
#define _qgradiobutton_h

#include <string>
#include <QButtonGroup>
#include <QRadioButton>
#include <QSize>
#include <QWidget>
#include "qginteractor.h"
#include "map.h"

// forward declaration
class QGRadioButton;

// Internal class; not to be used by clients.
class _Internal_QRadioButton : public QRadioButton, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QRadioButton(QGRadioButton* qgradioButton, bool checked = false, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleClick();

private:
    QGRadioButton* _qgradioButton;
};

/*
 * ...
 */
class QGRadioButton : public QGInteractor {
public:
    QGRadioButton(const std::string& text = "", const std::string& group = "default", bool checked = false, QWidget* parent = nullptr);
    virtual ~QGRadioButton();
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isChecked() const;
    virtual bool isSelected() const;
    virtual void removeActionListener();
    virtual void setActionListener(QGEventListener func);
    virtual void setActionListener(QGEventListenerVoid func);
    virtual void setChecked(bool checked);
    virtual void setSelected(bool selected);
    virtual void setText(const std::string& text);

private:
    static Map<std::string, QButtonGroup*> _buttonGroups;
    QButtonGroup* getButtonGroup(const std::string& group);

    _Internal_QRadioButton* _iqradioButton;

    friend class _Internal_QRadioButton;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgradiobutton_h
#endif // SPL_QT_GUI
