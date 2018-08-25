/*
 * File: glabel.cpp
 * ----------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to glabel.cpp to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "glabel.h"
#include "glayout.h"
#include "gthread.h"
#include "gwindow.h"
#include "strlib.h"

_Internal_QLabel::_Internal_QLabel(GLabel* glabel, QWidget* parent)
        : QLabel(parent),
          _glabel(glabel) {
    // empty
}

QSize _Internal_QLabel::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QLabel::sizeHint();
    }
}


GLabel::GLabel(const std::string& text, const std::string& iconFileName, QWidget* parent) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlabel = new _Internal_QLabel(this, getInternalParent(parent));
    });
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
}

GLabel::~GLabel() {
    // TODO: delete _iqlabel;
    _iqlabel = nullptr;
}

_Internal_QWidget* GLabel::getInternalWidget() const {
    return _iqlabel;
}

std::string GLabel::getLabel() const {
    return getText();
}

std::string GLabel::getText() const {
    return _iqlabel->text().toStdString();
}

GInteractor::TextPosition GLabel::getTextPosition() const {
    // TODO
//    switch (_iqlabel->toolButtonStyle()) {
//    case Qt::ToolButtonTextBesideIcon:
//        return GInteractor::TEXT_BESIDE_ICON;
//    case Qt::ToolButtonTextUnderIcon:
//        return GInteractor::TEXT_UNDER_ICON;
//    case Qt::ToolButtonTextOnly:
//    default:
//        return GInteractor::TEXT_ONLY;
//    }

    return GInteractor::TEXT_BESIDE_ICON;
}

std::string GLabel::getType() const {
    return "GLabel";
}

QWidget* GLabel::getWidget() const {
    return static_cast<QWidget*>(_iqlabel);
}

void GLabel::setIcon(const std::string& filename, bool retainIconSize) {
    GInteractor::setIcon(filename, retainIconSize);
    GThread::runOnQtGuiThread([this, filename, retainIconSize]() {
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
    });
}

void GLabel::setLabel(const std::string& text) {
    setText(text);
}

void GLabel::setText(const std::string& text) {
    GThread::runOnQtGuiThread([this, text]() {
        _iqlabel->setText(QString::fromStdString(text));
        GLayout::forceUpdate(_iqlabel);
    });
}

void GLabel::setTextPosition(GInteractor::TextPosition position) {
    // TODO
    if (position == GInteractor::TEXT_UNDER_ICON) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    } else if (position == GInteractor::TEXT_BESIDE_ICON) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    } else if (position == GInteractor::TEXT_ONLY) {
        // _iqpushbutton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}
