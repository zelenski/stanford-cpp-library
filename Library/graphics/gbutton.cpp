/*
 * File: gbutton.cpp
 * ------------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - moved some event-handling code to GInteractor superclass
 * @version 2019/04/22
 * - added setIcon with QIcon and QPixmap
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gbutton.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "gbutton.h"
#include <QKeySequence>
#include "filelib.h"
#include "gthread.h"
#include "gwindow.h"
#include "require.h"

GButton::GButton(const std::string& text, const std::string& iconFileName, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    });
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GButton::GButton(const std::string& text, const QIcon& icon, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    });
    setText(text);
    setIcon(icon);
    setVisible(false);   // all widgets are not shown until added to a window
}

GButton::GButton(const std::string& text, const QPixmap& icon, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqpushbutton = new _Internal_QPushButton(this, getInternalParent(parent));
    });
    setText(text);
    setIcon(icon);
    setVisible(false);   // all widgets are not shown until added to a window
}

GButton::~GButton() {
    // TODO: delete _iqpushbutton;
    _iqpushbutton->detach();
    _iqpushbutton = nullptr;
}

std::string GButton::getAccelerator() const {
    return _iqpushbutton->shortcut().toString().toStdString();
}

std::string GButton::getActionCommand() const {
    if (_actionCommand.empty()) {
        return getText();
    } else {
        return _actionCommand;
    }
}

_Internal_QWidget* GButton::getInternalWidget() const {
    return _iqpushbutton;
}

std::string GButton::getText() const {
    return _iqpushbutton->text().toStdString();
}

GInteractor::TextPosition GButton::getTextPosition() const {
    switch (_iqpushbutton->toolButtonStyle()) {
    case Qt::ToolButtonTextBesideIcon:
        return GInteractor::TEXT_BESIDE_ICON;
    case Qt::ToolButtonTextUnderIcon:
        return GInteractor::TEXT_UNDER_ICON;
    case Qt::ToolButtonTextOnly:
    default:
        return GInteractor::TEXT_ONLY;
    }
}

std::string GButton::getType() const {
    return "GButton";
}

QWidget* GButton::getWidget() const {
    return static_cast<QWidget*>(_iqpushbutton);
}

void GButton::setAccelerator(const std::string& accelerator) {
    GThread::runOnQtGuiThread([this, accelerator]() {
        QKeySequence keySeq(QString::fromStdString(normalizeAccelerator(accelerator)));
        _iqpushbutton->setShortcut(keySeq);
    });
}

void GButton::setIcon(const QIcon& icon) {
    GInteractor::setIcon(icon);
    GThread::runOnQtGuiThread([this, &icon]() {
        _iqpushbutton->setIcon(icon);
        _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        if (!icon.availableSizes().empty()) {
            _iqpushbutton->setIconSize(icon.availableSizes()[0]);
        }
        _iqpushbutton->updateGeometry();
        _iqpushbutton->update();
    });
}

void GButton::setIcon(const QPixmap& icon) {
    GInteractor::setIcon(icon);
    GThread::runOnQtGuiThread([this, &icon]() {
        _iqpushbutton->setIcon(icon);
        _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        _iqpushbutton->setIconSize(icon.size());
        _iqpushbutton->updateGeometry();
        _iqpushbutton->update();
    });
}

void GButton::setIcon(const std::string& filename, bool retainIconSize) {
    GInteractor::setIcon(filename, retainIconSize);
    if (!filename.empty() && fileExists(filename)) {
        GThread::runOnQtGuiThread([this, filename, retainIconSize]() {
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
}

void GButton::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqpushbutton->setText(QString::fromStdString(text));
    });
    setActionCommand(text);
}

void GButton::setTextPosition(GInteractor::TextPosition position) {
    GThread::runOnQtGuiThread([this, position]() {
        if (position == GInteractor::TEXT_UNDER_ICON) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        } else if (position == GInteractor::TEXT_BESIDE_ICON) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        } else if (position == GInteractor::TEXT_ONLY) {
            _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
        }
    });
}

_Internal_QPushButton::_Internal_QPushButton(GButton* button, QWidget* parent)
        : QToolButton(parent),
          _gbutton(button) {
    require::nonNull(button, "_Internal_QPushButton::constructor");
    setObjectName(QString::fromStdString("_Internal_QPushButton_" + std::to_string(button->getID())));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    connect(this, SIGNAL(clicked()), this, SLOT(handleClick()));
}

void _Internal_QPushButton::detach() {
    _gbutton = nullptr;
}

void _Internal_QPushButton::handleClick() {
    if (!_gbutton || !_gbutton->isAcceptingEvent("click")) {
        return;
    }
    GEvent actionEvent(
                /* class  */ ACTION_EVENT,
                /* type   */ ACTION_PERFORMED,
                /* name   */ "click",
                /* source */ _gbutton);
    actionEvent.setActionCommand(_gbutton->getActionCommand());
    _gbutton->fireEvent(actionEvent);
}

void _Internal_QPushButton::mouseDoubleClickEvent(QMouseEvent* event) {
    require::nonNull(event, "_Internal_QPushButton::mouseDoubleClickEvent", "event");
    QWidget::mouseDoubleClickEvent(event);   // call super
    if (!_gbutton || !_gbutton->isAcceptingEvent("doubleclick")) {
        return;
    }
    emit doubleClicked();
    GEvent mouseEvent(
                /* class  */ MOUSE_EVENT,
                /* type   */ MOUSE_DOUBLE_CLICKED,
                /* name   */ "doubleclick",
                /* source */ _gbutton);
    mouseEvent.setActionCommand(_gbutton->getActionCommand());
    mouseEvent.setButton((int) event->button());
    mouseEvent.setX(event->pos().x());
    mouseEvent.setY(event->pos().y());
    _gbutton->fireEvent(mouseEvent);
}

QSize _Internal_QPushButton::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QToolButton::sizeHint();
    }
}
