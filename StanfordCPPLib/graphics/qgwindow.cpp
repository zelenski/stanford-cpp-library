/*
 * File: qgwindow.cpp
 * ------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#include "qgwindow.h"
#include "strlib.h"
#include <QDesktopWidget>
#include <QStatusBar>

_Q_Internal_Window::_Q_Internal_Window(QGWindow* window, QWidget* parent)
        : QMainWindow(parent),
          _qgwindow(window),
          _qcentralWidget(),
          _qblayout() {
    // _qcentralWidget.setGeometry(10, 10, 300, 300);
    setCentralWidget(&_qcentralWidget);
    _qcentralWidget.setLayout(new QHBoxLayout);
    // _qcentralWidget.setLayout(&_qblayout);
}

QLayout* _Q_Internal_Window::getLayout() const {
    return _qcentralWidget.layout();
    // return (QGBorderLayout*) &_qblayout;
}


QApplication* QGWindow::_app = nullptr;
_Q_Internal_Window* QGWindow::_lastWindow = nullptr;

QGWindow::QGWindow(double width, double height, bool visible)
        : _qwindow(this) {
    if (!_app) {
        // TODO: throw: "need to initialize Qt"
    }
    _lastWindow = &_qwindow;
    setSize(width, height);
    setVisible(visible);
}

QGWindow::~QGWindow() {
    if (_lastWindow == &_qwindow) {
        _lastWindow = nullptr;
    }
}

void QGWindow::add(QGInteractor* interactor) {
    addToRegion(interactor, "Center");
}

void QGWindow::addToRegion(QGInteractor* interactor, const std::string& region) {
    QGBorderLayout::Position position = QGBorderLayout::toPosition(region);
    // _qwindow.getLayout()->addWidget(interactor->getWidget(), position);
    _qwindow.getLayout()->addWidget(interactor->getWidget());
}

void QGWindow::center() {
    GDimension screenSize = getScreenSize();
    GDimension windowSize = getSize();
    setLocation(screenSize.getWidth()  / 2 - windowSize.getWidth()  / 2,
                screenSize.getHeight() / 2 - windowSize.getHeight() / 2);
}

void QGWindow::close() {
    _qwindow.close();
}

Point QGWindow::getLocation() const {
    QRect geom = _qwindow.geometry();
    return Point(geom.x(), geom.y());
}

double QGWindow::getHeight() const {
    return _qwindow.geometry().height();
}

GDimension QGWindow::getSize() const {
    QRect geom = _qwindow.geometry();
    return GDimension(geom.width(), geom.height());
}

std::string QGWindow::getTitle() const {
    return _qwindow.windowTitle().toStdString();
}

double QGWindow::getWidth() const {
    return _qwindow.geometry().width();
}

double QGWindow::getX() const {
    return _qwindow.geometry().x();
}

double QGWindow::getY() const {
    return _qwindow.geometry().y();
}

bool QGWindow::isOpen() const {
    return isVisible();
}

bool QGWindow::isResizable() const {
    return _resizable;
}

bool QGWindow::isVisible() const {
    return _qwindow.isVisible();
}

void QGWindow::remove(QGInteractor* interactor) {
    // TODO
}

void QGWindow::removeFromRegion(QGInteractor* interactor, const std::string& region) {
    // TODO
}

void QGWindow::setLocation(double x, double y) {
    _qwindow.setGeometry((int) x, (int) y, getWidth(), getHeight());
}

void QGWindow::setResizable(bool resizable) {
    if (resizable) {
        if (!_resizable) {
            _qwindow.resize((int) getWidth(), (int) getHeight());
            _qwindow.setMinimumSize(_qwindow.minimumSizeHint());
            GDimension screenSize = getScreenSize();
            _qwindow.setMaximumSize((int) screenSize.getWidth(), (int) screenSize.getHeight());
        }
    } else {
        if (_resizable) {
            _qwindow.setFixedSize((int) getWidth(), (int) getHeight());
        }
    }
    _resizable = resizable;
}

void QGWindow::setSize(double width, double height) {
    _qwindow.resize((int) width, (int) height);
}

void QGWindow::setTitle(const std::string& title) {
    _qwindow.setWindowTitle(QString::fromStdString(title));
}

void QGWindow::setVisible(bool visible) {
    _qwindow.setVisible(visible);
}

void QGWindow::setWindowTitle(const std::string& title) {
    setTitle(title);
}

void QGWindow::setX(double x) {
    setLocation(x, getY());
}

void QGWindow::setY(double y) {
    setLocation(getX(), y);
}

QMainWindow* QGWindow::getLastWindow() {
    return _lastWindow;
}

double QGWindow::getScreenHeight() {
    return getScreenSize().getHeight();
}

GDimension QGWindow::getScreenSize() {
    QRect rec = QApplication::desktop()->availableGeometry();
    return GDimension(rec.width(), rec.height());
}

double QGWindow::getScreenWidth() {
    return getScreenSize().getWidth();
}


void initializeQt() {
    static int argc = 1;
    static char* argv[1] = {"Stanford Cpp Lib"};
    if (!QGWindow::_app) {
        QGWindow::_app = new QApplication(argc, argv);
    }
}

void startEventLoop() {
    if (QGWindow::_app) {
        QGWindow::_app->exec();
    }
}
