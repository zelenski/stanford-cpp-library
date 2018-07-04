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
class _Q_Internal_Button : public QPushButton {
    Q_OBJECT

public:
    _Q_Internal_Button(QGButton* button, QWidget* parent = nullptr);

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
    std::string getText() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    void setClickHandler(std::function<void()> func);
    void setText(const std::string& text);
    void setTextPosition(QGBorderLayout::Position horizontal, QGBorderLayout::Position vertical);

private:
    _Q_Internal_Button _button;

    friend class _Q_Internal_Button;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgbutton_h
