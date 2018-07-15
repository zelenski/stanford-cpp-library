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
#include <QSize>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGCheckBox;

// Internal class; not to be used by clients.
class _Internal_QCheckBox : public QCheckBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCheckBox(QGCheckBox* qgcheckBox, bool checked = false, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleStateChange(int);

private:
    QGCheckBox* _qgcheckBox;
};

/*
 * ...
 */
class QGCheckBox : public QGInteractor {
public:
    QGCheckBox(const std::string& text = "", bool checked = false, QWidget* parent = nullptr);
    virtual ~QGCheckBox();
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isChecked() const;
    virtual bool isSelected() const;
    virtual void removeChangeHandler();
    virtual void setChecked(bool checked);
    virtual void setChangeHandler(QGEventHandler func);
    virtual void setChangeHandler(QGEventHandlerVoid func);
    virtual void setSelected(bool selected);
    virtual void setText(const std::string& text);

private:
    _Internal_QCheckBox* _iqcheckBox;

    friend class _Internal_QCheckBox;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcheckbox_h
