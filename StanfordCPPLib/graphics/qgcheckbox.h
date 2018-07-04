/*
 * File: qgcheckbox.h
 * ------------------
 *
 * @version 2018/06/29
 * - added change event
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgcheckbox_h
#define _qgcheckbox_h

#include <functional>
#include <string>
#include <QCheckBox>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGCheckBox;

// Internal class; not to be used by clients.
class _Q_Internal_CheckBox : public QCheckBox {
    Q_OBJECT

public:
    _Q_Internal_CheckBox(QGCheckBox* checkBox, bool checked = false, QWidget* parent = nullptr);

public slots:
    void handleChange(bool);

private:
    QGCheckBox* _checkBox;
};

/*
 * ...
 */
class QGCheckBox : public QGInteractor {
public:
    QGCheckBox(const std::string& text = "", bool checked = false, QWidget* parent = nullptr);
    std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    bool isChecked() const;
    bool isSelected() const;
    void setChecked(bool checked);
    void setChangeHandler(std::function<void()> func);
    void setSelected(bool selected);
    void setText(const std::string& text);

private:
    _Q_Internal_CheckBox _checkBox;

    friend class _Q_Internal_CheckBox;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcheckbox_h
