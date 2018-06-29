/*
 * File: qgcheckbox.h
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgcheckbox_h
#define _qgcheckbox_h

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
    void setChecked(bool checked);
    void setText(const std::string& text);

private:
    _Q_Internal_CheckBox _checkBox;

    friend class _Q_Internal_CheckBox;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcheckbox_h
