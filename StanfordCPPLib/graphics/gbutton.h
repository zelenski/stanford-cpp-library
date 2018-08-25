/*
 * File: gbutton.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gbutton.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#ifndef _gbutton_h
#define _gbutton_h

#include <string>
#include <QPushButton>
#include <QSize>
#include <QToolButton>
#include <QWidget>
#include "glayout.h"
#include "ginteractor.h"

// forward declaration
class GButton;

// Internal class; not to be used by clients.
class _Internal_QPushButton : public QToolButton, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QPushButton(GButton* button, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleClick();

private:
    GButton* _gbutton;
};

/*
 * ...
 */
class GButton : public GInteractor {
public:
    GButton(const std::string& text = "", const std::string& iconFileName = "", QWidget* parent = nullptr);
    virtual ~GButton();
    virtual std::string getAccelerator() const Q_DECL_OVERRIDE;
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual std::string getText() const;
    virtual GInteractor::TextPosition getTextPosition() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual void removeActionListener();
    virtual void setAccelerator(const std::string& accelerator) Q_DECL_OVERRIDE;
    virtual void setActionListener(GEventListener func);
    virtual void setActionListener(GEventListenerVoid func);
    virtual void setIcon(const std::string& filename, bool retainIconSize = true) Q_DECL_OVERRIDE;
    virtual void setText(const std::string& text);
    virtual void setTextPosition(GInteractor::TextPosition position);
    virtual void setTextPosition(SwingConstants horizontal, SwingConstants vertical) Q_DECL_DEPRECATED;

private:
    _Internal_QPushButton* _iqpushbutton;

    friend class _Internal_QPushButton;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gbutton_h
