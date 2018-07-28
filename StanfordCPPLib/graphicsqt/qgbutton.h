/*
 * File: qgbutton.h
 * ----------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgbutton_h
#define _qgbutton_h

#include <string>
#include <QPushButton>
#include <QSize>
#include <QToolButton>
#include <QWidget>
#include "qgborderlayout.h"
#include "qginteractor.h"

// forward declaration
class QGButton;

// Internal class; not to be used by clients.
class _Internal_QPushButton : public QToolButton, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QPushButton(QGButton* button, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleClick();

private:
    QGButton* _qgbutton;
};

/*
 * ...
 */
class QGButton : public QGInteractor {
public:
    QGButton(const std::string& text = "", const std::string& iconFileName = "", QWidget* parent = nullptr);
    virtual ~QGButton();
    virtual std::string getAccelerator() const;
    virtual std::string getActionCommand() const Q_DECL_OVERRIDE;
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getText() const;
    virtual QGInteractor::TextPosition getTextPosition() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual void removeActionListener();
    virtual void setAccelerator(const std::string& accelerator);
    virtual void setActionListener(QGEventListener func);
    virtual void setActionListener(QGEventListenerVoid func);
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);
    virtual void setText(const std::string& text);
    virtual void setTextPosition(QGInteractor::TextPosition position);

private:
    _Internal_QPushButton* _iqpushbutton;

    friend class _Internal_QPushButton;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgbutton_h
