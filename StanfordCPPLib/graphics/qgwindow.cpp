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
          _qgwindow(window) {

    QWidget* dummyWidget = new QWidget(this);
    _northLayout.addStretch(99);
    _northLayout.addStretch(99);
    _southLayout.addStretch(99);
    _southLayout.addStretch(99);
    _westLayout.addStretch(99);
    _westLayout.addStretch(99);
    _eastLayout.addStretch(99);
    _eastLayout.addStretch(99);

    _overallLayout.addLayout(&_northLayout, /* stretch */ 1);
    _middleLayout.addLayout(&_westLayout, /* stretch */ 1);
    _middleLayout.addLayout(&_centerLayout, /* stretch */ 99);
    _middleLayout.addLayout(&_eastLayout, /* stretch */ 1);
    _overallLayout.addLayout(&_middleLayout, /* stretch */ 99);
    _overallLayout.addLayout(&_southLayout, /* stretch */ 1);
    dummyWidget->setLayout(&_overallLayout);
    setCentralWidget(dummyWidget);
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
    if (position == QGBorderLayout::North) {
        // _qwindow._northLayout.addWidget(interactor->getWidget());
        _qwindow._northLayout.insertWidget(/* index */ _qwindow._northLayout.count() - 1, interactor->getWidget());
    } else if (position == QGBorderLayout::South) {
        // _qwindow._southLayout.addWidget(interactor->getWidget());
        _qwindow._southLayout.insertWidget(/* index */ _qwindow._southLayout.count() - 1, interactor->getWidget());
    } else if (position == QGBorderLayout::West) {
        // _qwindow._westLayout.addWidget(interactor->getWidget());
        _qwindow._westLayout.insertWidget(/* index */ _qwindow._westLayout.count() - 1, interactor->getWidget());
    } else if (position == QGBorderLayout::East) {
        // _qwindow._eastLayout.addWidget(interactor->getWidget());
        _qwindow._eastLayout.insertWidget(/* index */ _qwindow._eastLayout.count() - 1, interactor->getWidget());
    } else {
        // center holds at most one widget
        if (_qwindow._centerLayout.isEmpty()) {
            _qwindow._centerLayout.addWidget(interactor->getWidget());
        } else {
            _qwindow._centerLayout.replaceWidget(
                        /* from */ _qwindow._centerLayout.takeAt(0)->widget(),
                        /* to   */ interactor->getWidget());
        }
    }
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
    removeFromRegion(interactor, "North");
    removeFromRegion(interactor, "South");
    removeFromRegion(interactor, "East");
    removeFromRegion(interactor, "West");
    removeFromRegion(interactor, "Center");
}

void QGWindow::removeFromRegion(QGInteractor* interactor, const std::string& region) {
    QGBorderLayout::Position position = QGBorderLayout::toPosition(region);
    if (position == QGBorderLayout::North) {
        _qwindow._northLayout.removeWidget(interactor->getWidget());
    } else if (position == QGBorderLayout::South) {
        _qwindow._southLayout.removeWidget(interactor->getWidget());
    } else if (position == QGBorderLayout::West) {
        _qwindow._westLayout.removeWidget(interactor->getWidget());
    } else if (position == QGBorderLayout::East) {
        _qwindow._eastLayout.removeWidget(interactor->getWidget());
    } else {
        _qwindow._centerLayout.removeWidget(interactor->getWidget());
    }
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
