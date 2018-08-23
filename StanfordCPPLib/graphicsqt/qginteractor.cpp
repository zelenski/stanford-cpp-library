/*
 * File: qginteractor.cpp
 * ----------------------
 *
 * @version 2018/06/29
 * - initial version
 */

#ifdef SPL_QT_GUI
#include "qginteractor.h"
#include <iostream>
#include <sstream>
#include "qgcolor.h"
#include "qgfont.h"
#include "qgui.h"
#include "qgwindow.h"

_Internal_QWidget::_Internal_QWidget()
        : _minimumSize(-1, -1),
          _preferredSize(-1, -1) {
    // empty
}

QSize _Internal_QWidget::getMinimumSize() const {
    return QSize((int) _minimumSize.getWidth(), (int) _minimumSize.getHeight());
}

bool _Internal_QWidget::hasMinimumSize() const {
    return _minimumSize.getWidth() >= 0 && _minimumSize.getHeight() >= 0;
}

QSize _Internal_QWidget::getPreferredSize() const {
    return QSize((int) _preferredSize.getWidth(), (int) _preferredSize.getHeight());
}

bool _Internal_QWidget::hasPreferredSize() const {
    return _preferredSize.getWidth() >= 0 && _preferredSize.getHeight() >= 0;
}

void _Internal_QWidget::setMinimumSize(double width, double height) {
    _minimumSize = QGDimension(width, height);
}

void _Internal_QWidget::setMinimumSize(const QSize& size) {
    setMinimumSize(size.width(), size.height());
}

void _Internal_QWidget::setPreferredSize(double width, double height) {
    _preferredSize = QGDimension(width, height);
}

void _Internal_QWidget::setPreferredSize(const QSize& size) {
    setPreferredSize(size.width(), size.height());
}


QGInteractor::QGInteractor()
        : _actionCommand(""),
          _icon("") {
    QGui::instance()->initializeQt();   // make sure Qt system is initialized
}

QGInteractor::~QGInteractor() {
    // empty
}

bool QGInteractor::eventsEnabled() const {
    return QGObservable::eventsEnabled() && getWidget() != nullptr && isVisible();
}

std::string QGInteractor::getAccelerator() const {
    // override in subclasses
    return "";
}

std::string QGInteractor::getActionCommand() const {
    return _actionCommand;
}

std::string QGInteractor::getBackground() const {
    int rgb = getBackgroundInt();
    return QGColor::convertRGBToColor(rgb);
}

int QGInteractor::getBackgroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->backgroundRole());
    return QGColor::convertQColorToRGB(color);
}

QGRectangle QGInteractor::getBounds() const {
    return QGRectangle(getX(), getY(), getWidth(), getHeight());
}

std::string QGInteractor::getColor() const {
    int rgb = getColorInt();
    return QGColor::convertRGBToColor(rgb);
}

int QGInteractor::getColorInt() const {
    QColor color = getWidget()->palette().color(getWidget()->foregroundRole());
    return QGColor::convertQColorToRGB(color);
}

std::string QGInteractor::getFont() const {
    return QGFont::toFontString(getWidget()->font());
}

std::string QGInteractor::getForeground() const {
    int rgb = getForegroundInt();
    return QGColor::convertRGBToColor(rgb);
}

int QGInteractor::getForegroundInt() const {
    QColor color = getWidget()->palette().color(getWidget()->foregroundRole());
    return QGColor::convertQColorToRGB(color);
}

double QGInteractor::getHeight() const {
    return getWidget()->height();
}

std::string QGInteractor::getIcon() const {
    return _icon;
}

QGPoint QGInteractor::getLocation() const {
    return QGPoint(getX(), getY());
}

QWidget* QGInteractor::getInternalParent(QWidget* parent) const {
    return parent ? parent : (QWidget*) QGWindow::getLastWindow();
}

double QGInteractor::getMinimumHeight() const {
    return getMinimumSize().getHeight();
}

QGDimension QGInteractor::getMinimumSize() const {
    QSize size = getInternalWidget()->getMinimumSize();
    return QGDimension(size.width(), size.height());
}

double QGInteractor::getMinimumWidth() const {
    return getMinimumSize().getWidth();
}

char QGInteractor::getMnemonic() const {
    // TODO
    return '?';
}

double QGInteractor::getPreferredHeight() const {
    return getPreferredSize().getHeight();
}

QGDimension QGInteractor::getPreferredSize() const {
    QSize size = getInternalWidget()->getPreferredSize();
    return QGDimension(size.width(), size.height());
}

double QGInteractor::getPreferredWidth() const {
    return getPreferredSize().getWidth();
}

QGDimension QGInteractor::getSize() const {
    return QGDimension(getWidth(), getHeight());
}

double QGInteractor::getWidth() const {
    return getWidget()->width();
}

double QGInteractor::getX() const {
    return getWidget()->x();
}

double QGInteractor::getY() const {
    return getWidget()->y();
}

bool QGInteractor::inBounds(double x, double y) const {
    return 0 <= x && x < getWidth() && 0 <= y && y < getHeight();
}

bool QGInteractor::inBounds(int x, int y) const {
    return 0 <= x && x < (int) getWidth() && 0 <= y && y < (int) getHeight();
}

bool QGInteractor::isEnabled() const {
    return getWidget()->isEnabled();
}

bool QGInteractor::isVisible() const {
    return getWidget()->isVisible();
}

std::string QGInteractor::normalizeAccelerator(const std::string& accelerator) const {
    std::string acceleratorStr = stringReplace(accelerator, "Alt-", "Alt+");
    acceleratorStr = stringReplace(acceleratorStr, "Command-", "Command+");
    acceleratorStr = stringReplace(acceleratorStr, "Ctrl-", "Ctrl+");
    acceleratorStr = stringReplace(acceleratorStr, "Meta-", "Meta+");
    acceleratorStr = stringReplace(acceleratorStr, "Shift-", "Shift+");
    return acceleratorStr;
}

void QGInteractor::requestFocus() {
    getWidget()->setFocus();
}

void QGInteractor::setActionCommand(const std::string& actionCommand) {
    _actionCommand = actionCommand;
}

void QGInteractor::setAccelerator(const std::string& /* accelerator */) {
    // override in subclasses
}

void QGInteractor::setBackground(int rgb) {
    QGThread::runOnQtGuiThread([this, rgb]() {
        QPalette palette(getWidget()->palette());
        palette.setColor(getWidget()->backgroundRole(), QColor(rgb));

        // additional palette color settings for QGChooser and other widgets
        // TODO: does not totally work for some widgets, e.g. QGChooser popup menu
        palette.setColor(QPalette::Base, QColor(rgb));
        palette.setColor(QPalette::Active, QPalette::Button, QColor(rgb));
        palette.setColor(QPalette::Inactive, QPalette::Button, QColor(rgb));

        getWidget()->setAutoFillBackground(true);
        getWidget()->setPalette(palette);
    });
}

void QGInteractor::setBackground(const std::string& color) {
    int rgb = QGColor::convertColorToRGB(color);
    setBackground(rgb);
}

void QGInteractor::setBounds(double x, double y, double width, double height) {
    QGThread::runOnQtGuiThread([this, x, y, width, height]() {
        getWidget()->setGeometry((int) x, (int) y, (int) width, (int) height);
        getWidget()->setFixedSize((int) width, (int) height);
    });
}

void QGInteractor::setBounds(const QGRectangle& size) {
    setBounds(size.getX(), size.getY(), size.getWidth(), size.getHeight());
}

void QGInteractor::setColor(int rgb) {
    setForeground(rgb);
}

void QGInteractor::setColor(const std::string& color) {
    setForeground(color);
}

void QGInteractor::setEnabled(bool value) {
    QGThread::runOnQtGuiThread([this, value]() {
        getWidget()->setEnabled(value);
    });
}

void QGInteractor::setForeground(int rgb) {
    QGThread::runOnQtGuiThread([this, rgb]() {
        QPalette palette(getWidget()->palette());
        palette.setColor(getWidget()->foregroundRole(), QColor(rgb));
        // TODO: does not totally work for some widgets, e.g. QGChooser popup menu
        getWidget()->setPalette(palette);
    });
}

void QGInteractor::setForeground(const std::string& color) {
    int rgb = QGColor::convertColorToRGB(color);
    setForeground(rgb);
}

void QGInteractor::setFont(const std::string& font) {
    QGThread::runOnQtGuiThread([this, font]() {
        QFont qfont = QGFont::toQFont(font);
        getWidget()->setFont(qfont);
    });
}

void QGInteractor::setHeight(double height) {
    QGThread::runOnQtGuiThread([this, height]() {
        getWidget()->setFixedHeight((int) height);
    });
}

void QGInteractor::setIcon(const std::string& filename, bool /* retainIconSize */) {
    _icon = filename;

    // override in subclasses as appropriate; make sure to call super
}

void QGInteractor::setLocation(double x, double y) {
    QGThread::runOnQtGuiThread([this, x, y]() {
        getWidget()->setGeometry(x, y, getWidth(), getHeight());
    });
}

void QGInteractor::setMinimumSize(double width, double height) {
    QGThread::runOnQtGuiThread([this, width, height]() {
        getInternalWidget()->setMinimumSize(width, height);
    });
}

void QGInteractor::setMinimumSize(const QGDimension& size) {
    QGThread::runOnQtGuiThread([this, size]() {
        getInternalWidget()->setMinimumSize(size.getWidth(), size.getHeight());
    });
}

void QGInteractor::setMnemonic(char /* mnemonic */) {
    // TODO
}

void QGInteractor::setPreferredHeight(double height) {
    setPreferredSize(getPreferredWidth(), height);
}

void QGInteractor::setPreferredSize(double width, double height) {
    QGThread::runOnQtGuiThread([this, width, height]() {
        getInternalWidget()->setPreferredSize(width, height);
    });
}

void QGInteractor::setPreferredSize(const QGDimension& size) {
    QGThread::runOnQtGuiThread([this, size]() {
        getInternalWidget()->setPreferredSize(size.getWidth(), size.getHeight());
    });
}

void QGInteractor::setPreferredWidth(double width) {
    setPreferredSize(width, getPreferredHeight());
}

void QGInteractor::setSize(double width, double height) {
    QGThread::runOnQtGuiThread([this, width, height]() {
        // setBounds(QGRectangle(getX(), getY(), width, height));
        getWidget()->setGeometry((int) getX(), (int) getY(), (int) width, (int) height);
        getWidget()->setFixedSize((int) width, (int) height);
    });
}

void QGInteractor::setSize(const QGDimension& size) {
    setSize(size.getWidth(), size.getHeight());
}

void QGInteractor::setTooltip(const std::string& tooltipText) {
    QGThread::runOnQtGuiThread([this, tooltipText]() {
        getWidget()->setToolTip(QString::fromStdString(tooltipText));
    });
}

void QGInteractor::setVisible(bool visible) {
    QGThread::runOnQtGuiThread([this, visible]() {
        getWidget()->setVisible(visible);
    });
}

void QGInteractor::setWidth(double width) {
    QGThread::runOnQtGuiThread([this, width]() {
        getWidget()->setFixedWidth((int) width);
    });
}

void QGInteractor::setX(double x) {
    setLocation(x, getY());
}

void QGInteractor::setY(double y) {
    setLocation(getX(), y);
}

#endif // SPL_QT_GUI
