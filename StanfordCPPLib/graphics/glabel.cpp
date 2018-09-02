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
        : _gtext(nullptr) {
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
    // TODO: if (_gtext) { delete _gtext; }
    // TODO: delete _iqlabel;
    _iqlabel = nullptr;
}

void GLabel::ensureGText() {
    _gtext = new GText(getText());
    _gtext->setColor(getColor());
    _gtext->setFont(getFont());
}

GText* GLabel::getGText() const {
    return _gtext;
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

bool GLabel::hasGText() const {
    return _gtext != nullptr;
}

void GLabel::setBounds(double x, double y, double width, double height) {
    if (_gtext) {
        _gtext->setBounds(x, y, width, height);
    }
    GInteractor::setBounds(x, y, width, height);
}

void GLabel::setBounds(const GRectangle& size) {
    if (_gtext) {
        _gtext->setBounds(size);
    }
    GInteractor::setBounds(size);
}

void GLabel::setColor(int rgb) {
    if (_gtext) {
        _gtext->setColor(rgb);
    }
    GInteractor::setColor(rgb);   // call super
}

void GLabel::setColor(const std::string& color) {
    if (_gtext) {
        _gtext->setColor(color);
    }
    GInteractor::setColor(color);   // call super
}

void GLabel::setFont(const QFont& font) {
    if (_gtext) {
        _gtext->setFont(font);
    }
    GInteractor::setFont(font);   // call super
}

void GLabel::setFont(const std::string& font) {
    if (_gtext) {
        _gtext->setFont(font);
    }
    GInteractor::setFont(font);   // call super
}

void GLabel::setForeground(int rgb) {
    if (_gtext) {
        _gtext->setForeground(rgb);
    }
    GInteractor::setForeground(rgb);   // call super
}

void GLabel::setForeground(const std::string& color) {
    if (_gtext) {
        _gtext->setForeground(color);
    }
    GInteractor::setForeground(color);   // call super
}

void GLabel::setHeight(double height) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setHeight(height);
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
    ensureGText();   // setting location triggers GText mode
    _gtext->setLocation(x, y);
    GInteractor::setLocation(x, y);
}

void GLabel::setSize(double width, double height) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setSize(width, height);
    GInteractor::setSize(width, height);
}

void GLabel::setSize(const GDimension& size) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setSize(size);
    GInteractor::setSize(size);
}

void GLabel::setText(const std::string& text) {
    if (_gtext) {
        _gtext->setText(text);
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
    if (_gtext) {
        _gtext->setVisible(visible);
    }
    GInteractor::setVisible(visible);   // call super
}

void GLabel::setWidth(double width) {
    ensureGText();   // setting size triggers GText mode
    _gtext->setWidth(width);
    GInteractor::setWidth(width);
}

void GLabel::setX(double x) {
    ensureGText();   // setting location triggers GText mode
    _gtext->setX(x);
    GInteractor::setX(x);
}

void GLabel::setY(double y) {
    ensureGText();   // setting location triggers GText mode
    _gtext->setY(y);
    GInteractor::setY(y);
}
