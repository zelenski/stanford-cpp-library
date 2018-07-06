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
#include <QWidget>
#include "qgborderlayout.h"
#include "qginteractor.h"

// forward declaration
class QGButton;

// Internal class; not to be used by clients.
class _Internal_QPushButton : public QPushButton {
    Q_OBJECT

public:
    _Internal_QPushButton(QGButton* button, QWidget* parent = nullptr);

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
    QGButton(const std::string& text = "", QWidget* parent = nullptr);
    virtual ~QGButton();
    std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual void setClickHandler(std::function<void()> func);
    virtual void setIcon(const std::string& filename);
    virtual void setText(const std::string& text);
    virtual void setTextPosition(QGBorderLayout::Position horizontal, QGBorderLayout::Position vertical);

private:
    _Internal_QPushButton* _iqpushbutton;

    friend class _Internal_QPushButton;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgbutton_h
