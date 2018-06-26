/*
 * File: qgwindow.h
 * ----------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qgwindow_h
#define _qgwindow_h

#include <string>
#include <QLayout>
#include <QApplication>
#include <QMainWindow>
#include <QRect>
#include "gtypes.h"
#include "point.h"
#include "qgborderlayout.h"
#include "qginteractor.h"

// set up Qt for gui stuff
void initializeQt();

// global function for starting the app
void startEventLoop();

// forward declaration
class QGWindow;

// Internal class; not to be used by clients.
class _Q_Internal_Window : public QMainWindow {
    Q_OBJECT

public:
    _Q_Internal_Window(QGWindow* window, QWidget* parent = nullptr);

    QLayout* getLayout() const;

//public slots:
//    void handleClick();

private:

    QGWindow* _qgwindow;
    QWidget _qcentralWidget;
    QGBorderLayout _qblayout;
};

/*
 * ...
 */
class QGWindow {
public:
    QGWindow(double width = 0, double height = 0, bool visible = true);
    virtual ~QGWindow();
    void add(QGInteractor* interactor);
    void addToRegion(QGInteractor* interactor, const std::string& region = "Center");

    /*
    clear
    draw***
    fill***
    getColor
    getGObjectAt
    remove
*   removeFromRegion
    repaint
    requestFocus
    setColor
     */

    void center();
    void close();

    Point getLocation() const;
    double getHeight() const;
    static double getScreenHeight();
    static GDimension getScreenSize();
    static double getScreenWidth();
    GDimension getSize() const;
    std::string getTitle() const;
    double getWidth() const;
    double getX() const;
    double getY() const;

    bool isOpen() const;
    bool isResizable() const;
    bool isVisible() const;

    void remove(QGInteractor* interactor);
    void removeFromRegion(QGInteractor* interactor, const std::string& region);

    void setLocation(double x, double y);
    void setResizable(bool resizable);
    void setSize(double width, double height);
    void setTitle(const std::string& title);
    void setVisible(bool visible);
    void setWindowTitle(const std::string& title);
    void setX(double x);
    void setY(double y);

    static QMainWindow* getLastWindow();

private:
    static QApplication* _app;
    static _Q_Internal_Window* _lastWindow;

    _Q_Internal_Window _qwindow;
    bool _resizable;

    friend class QGInteractor;
    friend class _Q_Internal_Window;
    friend void initializeQt();
    friend void startEventLoop();
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgwindow_h
