/*
 * File: qgbutton.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgbutton.h"
#include <QKeySequence>
#include "qgwindow.h"

_Internal_QPushButton::_Internal_QPushButton(QGButton* button, QWidget* parent)
        : QToolButton(parent),
          _qgbutton(button) {
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QPushButton::handleClick() {
    QGEvent clickEvent(
                /* class  */ QGEvent::MOUSE_EVENT,
                /* type   */ QGEvent::MOUSE_CLICKED,
                /* name   */ "click",
                /* source */ _qgbutton);
    _qgbutton->fireEvent(clickEvent);
}

QGButton::QGButton(const std::string& text, const std::string& iconFileName, QWidget* parent) {
    _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
}

QGButton::~QGButton() {
    // TODO: delete _button;
    _iqpushbutton = nullptr;
}

std::string QGButton::getAccelerator() const {
    return _iqpushbutton->shortcut().toString().toStdString();
}

std::string QGButton::getText() const {
    return _iqpushbutton->text().toStdString();
}

QGInteractor::TextPosition QGButton::getTextPosition() const {
    switch (_iqpushbutton->toolButtonStyle()) {
    case Qt::ToolButtonTextBesideIcon:
        return QGInteractor::TEXT_BESIDE_ICON;
    case Qt::ToolButtonTextUnderIcon:
        return QGInteractor::TEXT_UNDER_ICON;
    case Qt::ToolButtonTextOnly:
    default:
        return QGInteractor::TEXT_ONLY;
    }
}

std::string QGButton::getType() const {
    return "QGButton";
}

QWidget* QGButton::getWidget() const {
    return static_cast<QWidget*>(_iqpushbutton);
}

void QGButton::removeClickHandler() {
    removeEventHandler("click");
}

void QGButton::setAccelerator(const std::string& accelerator) {
    QKeySequence keySeq(QString::fromStdString(normalizeAccelerator(accelerator)));
    _iqpushbutton->setShortcut(keySeq);
}

void QGButton::setClickHandler(QGEventHandler func) {
    setEventHandler("click", func);
}

void QGButton::setClickHandler(QGEventHandlerVoid func) {
    setEventHandler("click", func);
}

void QGButton::setIcon(const std::string& filename, bool retainIconSize) {
    QGInteractor::setIcon(filename, retainIconSize);
    if (filename.empty()) {
        _iqpushbutton->setIcon(QIcon());
    } else {
        QPixmap pixmap(QString::fromStdString(filename));
        QIcon icon(pixmap);
        _iqpushbutton->setIcon(icon);
        _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        if (retainIconSize) {
            _iqpushbutton->setIconSize(pixmap.size());
            _iqpushbutton->updateGeometry();
            _iqpushbutton->update();
        }
    }
}

void QGButton::setText(const std::string& text) {
    _iqpushbutton->setText(QString::fromStdString(text));
    setActionCommand(text);
}

void QGButton::setTextPosition(QGInteractor::TextPosition position) {
    if (position == QGInteractor::TEXT_UNDER_ICON) {
        _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    } else if (position == QGInteractor::TEXT_BESIDE_ICON) {
        _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else if (position == QGInteractor::TEXT_ONLY) {
        _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}
