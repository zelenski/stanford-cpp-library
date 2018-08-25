/*
 * File: gwindow.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gwindow.h to replace Java version
 * @version 2018/07/29
 * - menu bars
 * @version 2018/06/25
 * - initial version
 */

#ifndef _gwindow_h
#define _gwindow_h

#include <string>
#include <QApplication>
#include <QWindow>
#include <QCloseEvent>
#include <QEvent>
#include <QLayout>
#include <QMainWindow>
#include <QRect>
#include "gcanvas.h"
#include "gdrawingsurface.h"
#include "geventqueue.h"
#include "ginteractor.h"
#include "glayout.h"
#include "grid.h"
#include "gtypes.h"
#include "map.h"
#include "point.h"

// forward declaration
class GWindow;

// Internal class; not to be used by clients.
class _Internal_QMainWindow : public QMainWindow {
    Q_OBJECT

public:
    static const int SPACING;
    static const int MARGIN;

    _Internal_QMainWindow(GWindow* gwindow, QWidget* parent = nullptr);
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
    GWindow* _gwindow;
    int _timerID;

    void processTimerEvent();

    friend class GWindow;
};

/*
 * ...
 */
class GWindow : public GObservable, public virtual GForwardDrawingSurface {
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

    GWindow(double width = 0, double height = 0, bool visible = true);
    virtual ~GWindow();
    virtual void add(GInteractor* interactor);
    virtual void add(GInteractor* interactor, double x, double y);
    virtual void add(GObject* obj);
    virtual void add(GObject* obj, double x, double y);
    virtual void add(GObject& obj);
    virtual void add(GObject& obj, double x, double y);
    virtual QMenu* addMenu(const std::string& text);
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const std::string& icon = "");
    virtual QAction* addMenuItem(const std::string& menu, const std::string& item,
                                 const std::string& icon, GEventListenerVoid func);
    virtual QAction* addMenuItemCheckBox(const std::string& menu, const std::string& item,
                                         bool checked = false,
                                         const std::string& icon = "");
    virtual QAction* addMenuItemCheckBox(const std::string& menu, const std::string& item,
                                         bool checked,
                                         const std::string& icon, GEventListenerVoid func);
    virtual QAction* addMenuSeparator(const std::string& menu);
    virtual QMenu* addSubMenu(const std::string& menu, const std::string& submenu);
    virtual void addToRegion(GInteractor* interactor, Region region);
    virtual void addToRegion(GInteractor* interactor, const std::string& region = "Center");
    virtual void clear() Q_DECL_OVERRIDE;
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
    virtual GDimension getCanvasSize() const;
    virtual double getCanvasWidth() const;
    virtual CloseOperation getCloseOperation() const;
    virtual GObject* getGObject(int index) const;
    virtual GObject* getGObjectAt(double x, double y) const;
    virtual int getGObjectCount() const;
    virtual GPoint getLocation() const;
    virtual double getHeight() const;
    virtual GDimension getPreferredSize() const;
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
    virtual void hide();
    virtual bool inBounds(double x, double y) const;
    virtual bool inCanvasBounds(double x, double y) const;
    virtual bool isMaximized() const;
    virtual bool isMinimized() const;
    virtual bool isOpen() const;
    virtual bool isRepaintImmediately() const Q_DECL_OVERRIDE;
    virtual bool isResizable() const;
    virtual bool isVisible() const;
    virtual void loadCanvasPixels(const std::string& filename);
    virtual void pack();
    virtual void pause(double ms);
    virtual void remove(GObject* obj);
    virtual void remove(GObject& obj);
    virtual void remove(GInteractor* interactor);
    virtual void removeClickListener();
    virtual void removeFromRegion(GInteractor* interactor, Region region);
    virtual void removeFromRegion(GInteractor* interactor, const std::string& region);
    virtual void removeKeyListener();
    virtual void removeMenuListener();
    virtual void removeMouseListener();
    virtual void removeTimerListener();
    virtual void removeWindowListener();
    virtual void requestFocus();
    virtual void saveCanvasPixels(const std::string& filename);
    virtual void setBackground(int color) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setCanvasHeight(double height);
    virtual void setCanvasSize(double width, double height);
    virtual void setCanvasSize(const GDimension& size);
    virtual void setCanvasWidth(double width);
    virtual void setClickListener(GEventListener func);
    virtual void setClickListener(GEventListenerVoid func);
    virtual void setCloseOperation(CloseOperation op);
    virtual void setExitOnClose(bool exitOnClose);
    virtual void setHeight(double width);
    virtual void setKeyListener(GEventListener func);
    virtual void setKeyListener(GEventListenerVoid func);
    virtual void setLocation(double x, double y);
    virtual void setLocation(const GPoint& p);
    virtual void setLocation(const Point& p);
    virtual void setMenuItemEnabled(const std::string& menu, const std::string& item, bool enabled);
    virtual void setMenuListener(GEventListener func);
    virtual void setMenuListener(GEventListenerVoid func);
    virtual void setMouseListener(GEventListener func);
    virtual void setMouseListener(GEventListenerVoid func);
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
    virtual void setTimerListener(double ms, GEventListener func);
    virtual void setTimerListener(double ms, GEventListenerVoid func);
    // TODO: setTimerListenerOnce
    virtual void setTitle(const std::string& title);
    virtual void setVisible(bool visible);
    virtual void setWidth(double width);
    virtual void setWindowListener(GEventListener func);
    virtual void setWindowListener(GEventListenerVoid func);
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
    GCanvas* _canvas;
    bool _resizable;
    CloseOperation _closeOperation;
    Map<Region, HorizontalAlignment> _halignMap;
    Map<Region, VerticalAlignment> _valignMap;
    Map<std::string, QMenu*> _menuMap;
    Map<std::string, QAction*> _menuActionMap;

    friend class GInteractor;
    friend class _Internal_QMainWindow;
};


// global functions for compatibility

/*
 * Function: convertColorToRGB
 * Usage: int rgb = convertColorToRGB(colorName);
 * ----------------------------------------------
 * Converts a color name into an integer that encodes the
 * red, green, and blue components of the color.
 */
int convertColorToRGB(const std::string& colorName);

/*
 * Function: convertRGBToColor
 * Usage: int colorName = convertRGBToColor(rgb);
 * ----------------------------------------------
 * Converts an <code>rgb</code> value into a color name in the
 * form <code>"#rrggbb"</code>.  Each of the <code>rr</code>,
 * <code>gg</code>, and <code>bb</code> values are two-digit
 * hexadecimal numbers indicating the intensity of that component.
 */
std::string convertRGBToColor(int rgb);

/*
 * Function: exitGraphics
 * Usage: exitGraphics();
 * ----------------------
 * Closes all graphics windows and exits from the application without
 * waiting for any additional user interaction.
 */
void exitGraphics();

/*
 * Function: getScreenHeight
 * Usage: height = getScreenHeight();
 * ----------------------------------
 * Returns the height of the entire display screen.
 */
double getScreenHeight();

/*
 * Function: getScreenSize
 * Usage: size = getScreenSize();
 * ----------------------------------
 * Returns the width/height of the entire display screen.
 */
GDimension getScreenSize();

/*
 * Function: getScreenWidth
 * Usage: width = getScreenWidth();
 * --------------------------------
 * Returns the width of the entire display screen.
 */
double getScreenWidth();

/*
 * Function: pause
 * Usage: pause(milliseconds);
 * ---------------------------
 * Pauses for the indicated number of milliseconds.  This function is
 * useful for animation where the motion would otherwise be too fast.
 */
void pause(double milliseconds);

/*
 * Function: repaint
 * Usage: repaint();
 * -----------------
 * Issues a request to update all graphics windows.  This function
 * is called automatically when the program pauses, waits for an
 * event, waits for user input on the console, or terminates.  As
 * a result, most clients never need to call repaint explicitly.
 */
void repaint();

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gwindow_h
