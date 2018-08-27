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


GLabel::GLabel(const std::string& text, const std::string& iconFileName, QWidget* parent)
        : _gstring(nullptr) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqlabel = new _Internal_QLabel(this, getInternalParent(parent));
    });
    setText(text);
    if (!iconFileName.empty()) {
        setIcon(iconFileName);
    }
    setVisible(false);   // all widgets are not shown until added to a window
}

GLabel::~GLabel() {
    // TODO: if (_gstring) { delete _gstring; }
    // TODO: delete _iqlabel;
    _iqlabel = nullptr;
}

void GLabel::ensureGString() {
    _gstring = new GString(getText());
    _gstring->setColor(getColor());
    _gstring->setFont(getFont());
}

GString* GLabel::getGString() const {
    return _gstring;
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

bool GLabel::hasGString() const {
    return _gstring != nullptr;
}

void GLabel::setBounds(double x, double y, double width, double height) {
    if (_gstring) {
        _gstring->setBounds(x, y, width, height);
    }
    GInteractor::setBounds(x, y, width, height);
}

void GLabel::setBounds(const GRectangle& size) {
    if (_gstring) {
        _gstring->setBounds(size);
    }
    GInteractor::setBounds(size);
}

void GLabel::setColor(int rgb) {
    if (_gstring) {
        _gstring->setColor(rgb);
    }
    GInteractor::setColor(rgb);   // call super
}

void GLabel::setColor(const std::string& color) {
    if (_gstring) {
        _gstring->setColor(color);
    }
    GInteractor::setColor(color);   // call super
}

void GLabel::setFont(const std::string& font) {
    if (_gstring) {
        _gstring->setFont(font);
    }
    GInteractor::setFont(font);   // call super
}

void GLabel::setForeground(int rgb) {
    if (_gstring) {
        _gstring->setForeground(rgb);
    }
    GInteractor::setForeground(rgb);   // call super
}

void GLabel::setForeground(const std::string& color) {
    if (_gstring) {
        _gstring->setForeground(color);
    }
    GInteractor::setForeground(color);   // call super
}

void GLabel::setHeight(double height) {
    ensureGString();   // setting size triggers GString mode
    _gstring->setHeight(height);
    GInteractor::setHeight(height);
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

void GLabel::setLocation(double x, double y) {
    ensureGString();   // setting location triggers GString mode
    _gstring->setLocation(x, y);
    GInteractor::setLocation(x, y);
}

void GLabel::setSize(double width, double height) {
    ensureGString();   // setting size triggers GString mode
    _gstring->setSize(width, height);
    GInteractor::setSize(width, height);
}

void GLabel::setSize(const GDimension& size) {
    ensureGString();   // setting size triggers GString mode
    _gstring->setSize(size);
    GInteractor::setSize(size);
}

void GLabel::setText(const std::string& text) {
    if (_gstring) {
        _gstring->setText(text);
    }
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

void GLabel::setVisible(bool visible) {
    if (_gstring) {
        _gstring->setVisible(visible);
    }
    GInteractor::setVisible(visible);   // call super
}

void GLabel::setWidth(double width) {
    ensureGString();   // setting size triggers GString mode
    _gstring->setWidth(width);
    GInteractor::setWidth(width);
}

void GLabel::setX(double x) {
    ensureGString();   // setting location triggers GString mode
    _gstring->setX(x);
    GInteractor::setX(x);
}

void GLabel::setY(double y) {
    ensureGString();   // setting location triggers GString mode
    _gstring->setY(y);
    GInteractor::setY(y);
}
