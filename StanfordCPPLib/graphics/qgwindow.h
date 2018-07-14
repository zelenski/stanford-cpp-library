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
#include <QWindow>
#include <QCloseEvent>
#include <QEvent>
#include <QMainWindow>
#include <QRect>
#include "grid.h"
#include "gtypes.h"
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
    _Internal_QMainWindow(QGWindow* qgwindow, QWidget* parent = nullptr);
    // virtual bool event(QEvent* event) Q_DECL_OVERRIDE;

    virtual void changeEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    virtual void timerEvent(QTimerEvent* event) Q_DECL_OVERRIDE;
    virtual bool timerExists();
    virtual void timerStart(double ms);
    virtual void timerStop();

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
    enum HorizontalAlignment {
        ALIGN_CENTER,
        ALIGN_LEFT,
        ALIGN_RIGHT
    };
    enum VerticalAlignment {
        ALIGN_MIDDLE,
        ALIGN_TOP,
        ALIGN_BOTTOM
    };
    enum Region {
        REGION_CENTER,
        REGION_EAST,
        REGION_NORTH,
        REGION_SOUTH,
        REGION_WEST
    };
    enum CloseOperation { CLOSE_DO_NOTHING = 0, CLOSE_HIDE = 1, CLOSE_DISPOSE = 2, CLOSE_EXIT = 3 };

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
    virtual void addToRegion(QGInteractor* interactor, Region region);
    virtual void addToRegion(QGInteractor* interactor, const std::string& region = "Center");
    virtual void clear() Q_DECL_OVERRIDE;
    virtual void clearCanvas();
    virtual void clearCanvasObjects();
    virtual void clearCanvasPixels();
    virtual void clearRegion(Region region);
    virtual void clearRegion(const std::string& region);
    virtual void center();
    virtual void close();
    virtual void compareToImage(const std::string& filename, bool ignoreWindowSize = true) const;
    virtual double getCanvasHeight() const;
    virtual GDimension getCanvasSize() const;
    virtual double getCanvasWidth() const;
    virtual CloseOperation getCloseOperation() const;
    virtual QGObject* getQGObject(int index) const;
    virtual QGObject* getQGObjectAt(double x, double y) const;
    virtual int getQGObjectCount() const;
    virtual GPoint getLocation() const;
    virtual double getHeight() const;
    virtual double getRegionHeight(Region region) const;
    virtual double getRegionHeight(const std::string& region) const;
    virtual GDimension getRegionSize(Region region) const;
    virtual GDimension getRegionSize(const std::string& region) const;
    virtual double getRegionWidth(Region region) const;
    virtual double getRegionWidth(const std::string& region) const;
    static double getScreenHeight();
    static GDimension getScreenSize();
    static double getScreenWidth();
    virtual GDimension getSize() const;
    virtual std::string getTitle() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const;
    virtual double getWidth() const;
    virtual double getX() const;
    virtual double getY() const;
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
    virtual void removeClickHandler();
    virtual void removeFromRegion(QGInteractor* interactor, Region region);
    virtual void removeFromRegion(QGInteractor* interactor, const std::string& region);
    virtual void removeKeyHandler();
    virtual void removeMouseHandler();
    virtual void removeTimerHandler();
    virtual void removeWindowHandler();
    virtual void requestFocus();
    virtual void saveCanvasPixels(const std::string& filename);
    virtual void setBackground(int color);
    virtual void setBackground(const std::string& color);
    virtual void setCanvasHeight(double height);
    virtual void setCanvasSize(double width, double height);
    virtual void setCanvasSize(const GDimension& size);
    virtual void setCanvasWidth(double width);
    virtual void setCloseOperation(CloseOperation op);
    virtual void setExitOnClose(bool exitOnClose);
    virtual void setHeight(double width);
    virtual void setLocation(double x, double y);
    virtual void setLocation(const GPoint& p);
    virtual void setLocation(const Point& p);
    virtual void setClickHandler(QGEventHandler func);
    virtual void setClickHandler(QGEventHandlerVoid func);
    virtual void setKeyHandler(QGEventHandler func);
    virtual void setKeyHandler(QGEventHandlerVoid func);
    virtual void setMouseHandler(QGEventHandler func);
    virtual void setMouseHandler(QGEventHandlerVoid func);
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign);
    virtual void setRegionAlignment(Region region, VerticalAlignment valign);
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign);
    virtual void setRegionAlignment(const std::string& region, const std::string& align);
    virtual void setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign);
    virtual void setRegionHorizontalAlignment(Region region, HorizontalAlignment halign);
    virtual void setRegionHorizontalAlignment(const std::string& region, const std::string& halign);
    virtual void setRegionVerticalAlignment(const std::string& region, const std::string& valign);
    virtual void setRegionVerticalAlignment(Region region, VerticalAlignment valign);
    virtual void setResizable(bool resizable);
    virtual void setSize(double width, double height);
    virtual void setSize(const GDimension& size);
    virtual void setTimerHandler(double ms, QGEventHandler func);
    virtual void setTimerHandler(double ms, QGEventHandlerVoid func);
    // TODO: setTimerHandlerOnce
    virtual void setTitle(const std::string& title);
    virtual void setVisible(bool visible);
    virtual void setWidth(double width);
    virtual void setWindowHandler(QGEventHandler func);
    virtual void setWindowHandler(QGEventHandlerVoid func);
    virtual void setWindowTitle(const std::string& title);
    virtual void setX(double x);
    virtual void setY(double y);
    virtual void toBack();
    virtual void toFront();

    // not to be called by students
    static QMainWindow* getLastWindow();

private:
    static _Internal_QMainWindow* _lastWindow;

    static std::string alignmentToString(HorizontalAlignment alignment);
    static std::string alignmentToString(VerticalAlignment alignment);
    virtual void ensureForwardTarget() Q_DECL_OVERRIDE;
    QLayout* layoutForRegion(Region region);
    QLayout* layoutForRegion(const std::string& region);
    static std::string regionToString(Region region);
    static Region stringToRegion(const std::string& regionStr);
    static HorizontalAlignment stringToHorizontalAlignment(const std::string& alignmentStr);
    static VerticalAlignment stringToVerticalAlignment(const std::string& alignmentStr);
    static Qt::Alignment toQtAlignment(HorizontalAlignment alignment);
    static Qt::Alignment toQtAlignment(VerticalAlignment alignment);

    _Internal_QMainWindow* _iqmainwindow;
    QGCanvas* _canvas;
    bool _resizable;
    CloseOperation _closeOperation;
    Map<Region, HorizontalAlignment> _halignMap;
    Map<Region, VerticalAlignment> _valignMap;

    friend class QGInteractor;
    friend class _Internal_QMainWindow;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgwindow_h
