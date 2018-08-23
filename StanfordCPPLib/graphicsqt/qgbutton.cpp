/*
 * File: qgbutton.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qgbutton.h"
#include <QKeySequence>
#include "qgthread.h"
#include "qgwindow.h"

_Internal_QPushButton::_Internal_QPushButton(QGButton* button, QWidget* parent)
        : QToolButton(parent),
          _qgbutton(button) {
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QPushButton::handleClick() {
    QGEvent actionEvent(
                /* class  */ QGEvent::ACTION_EVENT,
                /* type   */ QGEvent::ACTION_PERFORMED,
                /* name   */ "click",
                /* source */ _qgbutton);
    actionEvent.setActionCommand(_qgbutton->getActionCommand());
    _qgbutton->fireEvent(actionEvent);
}

QSize _Internal_QPushButton::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QToolButton::sizeHint();
    }
}


QGButton::QGButton(const std::string& text, const std::string& iconFileName, QWidget* parent) {
    QGThread::runOnQtGuiThread([this, parent]() {
        _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    });
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

std::string QGButton::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* QGButton::getInternalWidget() const {
    return _iqpushbutton;
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

void QGButton::removeActionListener() {
    removeEventListener("click");
}

void QGButton::setAccelerator(const std::string& accelerator) {
    QGThread::runOnQtGuiThread([this, accelerator]() {
        QKeySequence keySeq(QString::fromStdString(normalizeAccelerator(accelerator)));
        _iqpushbutton->setShortcut(keySeq);
    });
}

void QGButton::setActionListener(QGEventListener func) {
    setEventListener("click", func);
}

void QGButton::setActionListener(QGEventListenerVoid func) {
    setEventListener("click", func);
}

void QGButton::setIcon(const std::string& filename, bool retainIconSize) {
    QGInteractor::setIcon(filename, retainIconSize);
    QGThread::runOnQtGuiThread([this, filename, retainIconSize]() {
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
    });
}

void QGButton::setText(const std::string& text) {
    QGThread::runOnQtGuiThread([this, text]() {
        _iqpushbutton->setText(QString::fromStdString(text));
    });
    setActionCommand(text);
}

void QGButton::setTextPosition(QGInteractor::TextPosition position) {
    QGThread::runOnQtGuiThread([this, position]() {
        if (position == QGInteractor::TEXT_UNDER_ICON) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        } else if (position == QGInteractor::TEXT_BESIDE_ICON) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        } else if (position == QGInteractor::TEXT_ONLY) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        }
    });
}

#endif // SPL_QT_GUI
