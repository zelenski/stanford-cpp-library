/*
 * File: gcheckbox.h
 * -----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gcheckbox.h to replace Java version
 * @version 2018/06/29
 * - added change event
 * @version 2018/06/25
 * - initial version
 */

#ifndef _gcheckbox_h
#define _gcheckbox_h

#include <functional>
#include <string>
#include <QCheckBox>
#include <QSize>
#include <QWidget>
#include "ginteractor.h"

class _Internal_QCheckBox;

/*
 * ...
 */
class GCheckBox : public GInteractor {
public:
    GCheckBox(const std::string& text = "", bool checked = false, QWidget* parent = nullptr);
    virtual ~GCheckBox();
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual std::string getText() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual bool isChecked() const;
    virtual bool isSelected() const;
    virtual void removeActionListener();
    virtual void setActionListener(GEventListener func);
    virtual void setActionListener(GEventListenerVoid func);
    virtual void setChecked(bool checked);
    virtual void setSelected(bool selected);
    virtual void setText(const std::string& text);

private:
    Q_DISABLE_COPY(GCheckBox)

    _Internal_QCheckBox* _iqcheckBox;

    friend class _Internal_QCheckBox;
};

// Internal class; not to be used by clients.
class _Internal_QCheckBox : public QCheckBox, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCheckBox(GCheckBox* gcheckBox, bool checked = false, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleStateChange(int);

private:
    GCheckBox* _gcheckBox;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gcheckbox_h
