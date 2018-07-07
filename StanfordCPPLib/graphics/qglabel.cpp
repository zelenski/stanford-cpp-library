/*
 * File: qglabel.cpp
 * -----------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qglabel.h"
#include "qgwindow.h"
#include "strlib.h"

_Internal_QLabel::_Internal_QLabel(QGLabel* qglabel, QWidget* parent)
        : QLabel(parent),
          _qglabel(qglabel) {
    // empty
}

QGLabel::QGLabel(const std::string& text, const std::string& iconFileName, QWidget* parent) {
    _iqlabel = new _Internal_QLabel(this, getInternalParent(parent));
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
}

QGLabel::~QGLabel() {
    // TODO: delete _iqlabel;
    _iqlabel = nullptr;
}

std::string QGLabel::getLabel() const {
    return getText();
}

std::string QGLabel::getText() const {
    return _iqlabel->text().toStdString();
}

QGInteractor::TextPosition QGLabel::getTextPosition() const {
    // TODO
//    switch (_iqlabel->toolButtonStyle()) {
//    case Qt::ToolButtonTextBesideIcon:
//        return QGInteractor::TEXT_BESIDE_ICON;
//    case Qt::ToolButtonTextUnderIcon:
//        return QGInteractor::TEXT_UNDER_ICON;
//    case Qt::ToolButtonTextOnly:
//    default:
//        return QGInteractor::TEXT_ONLY;
//    }

    return QGInteractor::TEXT_BESIDE_ICON;
}

std::string QGLabel::getType() const {
    return "QGLabel";
}

QWidget* QGLabel::getWidget() const {
    return static_cast<QWidget*>(_iqlabel);
}

void QGLabel::setIcon(const std::string& filename, bool retainIconSize) {
    QGInteractor::setIcon(filename, retainIconSize);
    if (filename.empty()) {
        _iqlabel->setPixmap(QPixmap());
    } else {
        QPixmap pixmap(QString::fromStdString(filename));
        _iqlabel->setPixmap(pixmap);
        if (retainIconSize) {
            // TODO
            // _iqlabel->setIconSize(pixmap.size());
            _iqlabel->updateGeometry();
            _iqlabel->update();
        }
        // TODO: loses text; how to have both icon and text in same label?
    }
}

void QGLabel::setLabel(const std::string& text) {
    setText(text);
}

void QGLabel::setText(const std::string& text) {
    _iqlabel->setText(QString::fromStdString(text));
    _iqlabel->updateGeometry();
}

void QGLabel::setTextPosition(QGInteractor::TextPosition position) {
    // TODO
    if (position == QGInteractor::TEXT_UNDER_ICON) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    } else if (position == QGInteractor::TEXT_BESIDE_ICON) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else if (position == QGInteractor::TEXT_ONLY) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}
