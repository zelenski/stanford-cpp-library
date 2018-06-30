/*
 * File: qgradiobutton.h
 * ---------------------
 *
 * @version 2018/06/29
 * - added change event
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgradiobutton_h
#define _qgradiobutton_h

#include <string>
#include <QButtonGroup>
#include <QRadioButton>
#include <QWidget>
#include "qginteractor.h"
#include "map.h"

// forward declaration
class QGRadioButton;

// Internal class; not to be used by clients.
class _Q_Internal_RadioButton : public QRadioButton {
    Q_OBJECT

public:
    _Q_Internal_RadioButton(QGRadioButton* radioButton, bool checked = false, QWidget* parent = nullptr);

public slots:
    void handleChange(bool);

private:
    QGRadioButton* _radioButton;
};

/*
 * ...
 */
class QGRadioButton : public QGInteractor {
public:
    QGRadioButton(const std::string& text = "", const std::string& group = "default", bool checked = false, QWidget* parent = nullptr);
    std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    bool isChecked() const;
    void setChecked(bool checked);
    void setChangeHandler(void (* func)());
    void setText(const std::string& text);

private:
    static Map<std::string, QButtonGroup*> _buttonGroups;
    QButtonGroup* getButtonGroup(const std::string& group);

    _Q_Internal_RadioButton _radioButton;

    void (* _changeHandler)();

    friend class _Q_Internal_RadioButton;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgradiobutton_h
