/*
 * File: qgwindow.h
 * ----------------
 *
 * @version 2018/07/29
 * - menu bars
 * @version 2018/06/25
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgwindow_h
#define _qgwindow_h

#include <string>
#include <QApplication>
#include <QWindow>
#include <QCloseEvent>
#include <QEvent>
#include <QLayout>
#include <QMainWindow>
#include <QRect>
#include "grid.h"
#include "qgtypes.h"
#include "map.h"
#include "point.h"
#include "qgborderlayout.h"
#include "qgcanvas.h"
#include "qginteractor.h"
#include "qgdrawingsurface.h"

// forward declaration
class QGWindow;

// Internal class; not to be used by clients.
class _Internal_QMainWindow : public QMainWindow {
    Q_OBJECT

public:
    static const int SPACING;
    static const int MARGIN;

    _Internal_QMainWindow(QGWindow* qgwindow, QWidget* parent = nullptr);
    // virtual bool event(QEvent* event) Q_DECL_OVERRIDE;

    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
    virtual void fixMargins();
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    virtual void timerEvent(QTimerEvent* event) Q_DECL_OVERRIDE;
    virtual bool timerExists();
    virtual void timerStart(double ms);
    virtual void timerStop();

public slots:
    void handleMenuAction(const std::string& menu, const std::string& item);

private:
    // border layout regions for N/S/W/E/C:
    // +------------------------+
    // |         north          |
    // |------------------------|
    // |         middle         |
    // |+----------------------+|
    // || west | center | east ||
    // |+----------------------+|
    // |------------------------|
    // |         south          |
    // +------------------------+
    // sizing/stretching rules:
    // - N/S expand horizontally
    // - W/E expand vertically
    // - C takes all remaining space
    // - each widget other than Center widget appears at its preferred ("hinted") size
    QVBoxLayout* _overallLayout;
    QHBoxLayout* _northLayout;
    QHBoxLayout* _southLayout;
    QVBoxLayout* _westLayout;
    QVBoxLayout* _eastLayout;
    QHBoxLayout* _centerLayout;
    QHBoxLayout* _middleLayout;
    QGWindow* _qgwindow;
    int _timerID;

    void processTimerEvent();

    friend class QGWindow;
};

/*
 * ...
 */
class QGWindow : public QGObservable, public virtual QGForwardDrawingSurface {
public:
    enum Region {
        REGION_CENTER,
        REGION_EAST,
        REGION_NORTH,
        REGION_SOUTH,
        REGION_WEST
    };

    enum CloseOperation {
        CLOSE_DO_NOTHING,
        CLOSE_HIDE,
        CLOSE_DISPOSE,
        CLOSE_EXIT
    };

    static const int DEFAULT_WIDTH = 500;
    static const int DEFAULT_HEIGHT = 300;

    QGWindow(double width = 0, double height = 0, bool visible = true);
    virtual ~QGWindow();
    virtual void add(QGInteractor* interactor);
    virtual void add(QGInteractor* interactor, double x, double y);
    virtual void add(QGObject* obj);
    virtual void add(QGObject* obj, double x, double y);
    virtual void add(QGObject& obj);
    virtual void add(QGObject& obj, double x, double y);
    virtual QMenu* addMenu(const std::string& text);
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const std::string& icon = "");
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const std::string& icon, QGEventListenerVoid func);
    virtual QAction* addMenuItemCheckBox(const std::string& menu, const std::string& item,
                                         bool checked = false,
                                         const std::string& icon = "");
    virtual QAction* addMenuItemCheckBox(const std::string& menu, const std::string& item,
                                         bool checked,
                                         const std::string& icon, QGEventListenerVoid func);
    virtual QAction* addMenuSeparator(const std::string& menu);
    virtual QMenu* addSubMenu(const std::string& menu, const std::string& submenu);
    virtual void addToRegion(QGInteractor* interactor, Region region);
    virtual void addToRegion(QGInteractor* interactor, const std::string& region = "Center");
    virtual void clearConsole() Q_DECL_OVERRIDE;
    virtual void clearCanvas();
    virtual void clearCanvasObjects();
    virtual void clearCanvasPixels();
    virtual void clearRegion(Region region);
    virtual void clearRegion(const std::string& region);
    virtual void center();
    virtual void close();
    virtual void compareToImage(const std::string& filename, bool ignoreWindowSize = true) const;
    virtual bool eventsEnabled() const;
    virtual double getCanvasHeight() const;
    virtual QGDimension getCanvasSize() const;
    virtual double getCanvasWidth() const;
    virtual CloseOperation getCloseOperation() const;
    virtual QGObject* getQGObject(int index) const;
    virtual QGObject* getQGObjectAt(double x, double y) const;
    virtual int getQGObjectCount() const;
    virtual QGPoint getLocation() const;
    virtual double getHeight() const;
    virtual QGDimension getPreferredSize() const;
    virtual double getRegionHeight(Region region) const;
    virtual double getRegionHeight(const std::string& region) const;
    virtual QGDimension getRegionSize(Region region) const;
    virtual QGDimension getRegionSize(const std::string& region) const;
    virtual double getRegionWidth(Region region) const;
    virtual double getRegionWidth(const std::string& region) const;
    static double getScreenHeight();
    static QGDimension getScreenSize();
    static double getScreenWidth();
    virtual QGDimension getSize() const;
    virtual std::string getTitle() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const;
    virtual double getWidth() const;
    virtual double getX() const;
    virtual double getY() const;
    virtual void hide();
    virtual bool inBounds(double x, double y) const;
    virtual bool inCanvasBounds(double x, double y) const;
    virtual bool isMaximized() const;
    virtual bool isMinimized() const;
    virtual bool isOpen() const;
    virtual bool isRepaintImmediately() const;
    virtual bool isResizable() const;
    virtual bool isVisible() const;
    virtual void loadCanvasPixels(const std::string& filename);
    virtual void pack();
    virtual void pause(double ms);
    virtual void remove(QGObject* obj);
    virtual void remove(QGObject& obj);
    virtual void remove(QGInteractor* interactor);
    virtual void removeClickListener();
    virtual void removeFromRegion(QGInteractor* interactor, Region region);
    virtual void removeFromRegion(QGInteractor* interactor, const std::string& region);
    virtual void removeKeyListener();
    virtual void removeMenuListener();
    virtual void removeMouseListener();
    virtual void removeTimerListener();
    virtual void removeWindowListener();
    virtual void requestFocus();
    virtual void saveCanvasPixels(const std::string& filename);
    virtual void setBackground(int color);
    virtual void setBackground(const std::string& color);
    virtual void setCanvasHeight(double height);
    virtual void setCanvasSize(double width, double height);
    virtual void setCanvasSize(const QGDimension& size);
    virtual void setCanvasWidth(double width);
    virtual void setClickListener(QGEventListener func);
    virtual void setClickListener(QGEventListenerVoid func);
    virtual void setCloseOperation(CloseOperation op);
    virtual void setExitOnClose(bool exitOnClose);
    virtual void setHeight(double width);
    virtual void setKeyListener(QGEventListener func);
    virtual void setKeyListener(QGEventListenerVoid func);
    virtual void setLocation(double x, double y);
    virtual void setLocation(const QGPoint& p);
    virtual void setLocation(const Point& p);
    virtual void setMenuItemEnabled(const std::string& menu, const std::string& item, bool enabled);
    virtual void setMenuListener(QGEventListener func);
    virtual void setMenuListener(QGEventListenerVoid func);
    virtual void setMouseListener(QGEventListener func);
    virtual void setMouseListener(QGEventListenerVoid func);
    virtual void setRegionAlignment(Region region, qgenum::HorizontalAlignment halign);
    virtual void setRegionAlignment(Region region, qgenum::VerticalAlignment valign);
    virtual void setRegionAlignment(Region region, qgenum::HorizontalAlignment halign, qgenum::VerticalAlignment valign);
    virtual void setRegionAlignment(const std::string& region, const std::string& align);
    virtual void setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign);
    virtual void setRegionHorizontalAlignment(Region region, qgenum::HorizontalAlignment halign);
    virtual void setRegionHorizontalAlignment(const std::string& region, const std::string& halign);
    virtual void setRegionVerticalAlignment(const std::string& region, const std::string& valign);
    virtual void setRegionVerticalAlignment(Region region, qgenum::VerticalAlignment valign);
    virtual void setResizable(bool resizable);
    virtual void setSize(double width, double height);
    virtual void setSize(const QGDimension& size);
    virtual void setTimerListener(double ms, QGEventListener func);
    virtual void setTimerListener(double ms, QGEventListenerVoid func);
    // TODO: setTimerListenerOnce
    virtual void setTitle(const std::string& title);
    virtual void setVisible(bool visible);
    virtual void setWidth(double width);
    virtual void setWindowListener(QGEventListener func);
    virtual void setWindowListener(QGEventListenerVoid func);
    virtual void setWindowTitle(const std::string& title);
    virtual void setX(double x);
    virtual void setY(double y);
    virtual void show();
    virtual void sleep(double ms);
    virtual void toBack();
    virtual void toFront();

    // not to be called by students
    static QMainWindow* getLastWindow();

protected:
    virtual void processKeyPressEventInternal(QKeyEvent* event);

private:
    static _Internal_QMainWindow* _lastWindow;

    virtual void ensureForwardTarget() Q_DECL_OVERRIDE;
    QLayout* layoutForRegion(Region region) const;
    QLayout* layoutForRegion(const std::string& region) const;
    static std::string regionToString(Region region);
    static Region stringToRegion(const std::string& regionStr);

    _Internal_QMainWindow* _iqmainwindow;
    QGCanvas* _canvas;
    bool _resizable;
    CloseOperation _closeOperation;
    Map<Region, qgenum::HorizontalAlignment> _halignMap;
    Map<Region, qgenum::VerticalAlignment> _valignMap;
    Map<std::string, QMenu*> _menuMap;
    Map<std::string, QAction*> _menuActionMap;

    friend class QGInteractor;
    friend class _Internal_QMainWindow;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgwindow_h
#endif // SPL_QT_GUI
