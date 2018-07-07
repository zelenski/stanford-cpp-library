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
#include "grid.h"
#include "gtypes.h"
#include "point.h"
#include "qgborderlayout.h"
#include "qgcanvas.h"
#include "qginteractor.h"

// forward declaration
class QGWindow;

// Internal class; not to be used by clients.
class _Internal_QMainWindow : public QMainWindow {
    Q_OBJECT

public:
    _Internal_QMainWindow(QGWindow* qgwindow, QWidget* parent = nullptr);
    bool event(QEvent* event) Q_DECL_OVERRIDE;

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

    friend class QGWindow;
};

/*
 * ...
 */
class QGWindow {
public:
    enum Alignment { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };
    enum Region { REGION_CENTER, REGION_EAST, REGION_NORTH, REGION_SOUTH, REGION_WEST };
    enum CloseOperation { CLOSE_DO_NOTHING = 0, CLOSE_HIDE = 1, CLOSE_DISPOSE = 2, CLOSE_EXIT = 3 };

    static const int DEFAULT_WIDTH = 500;
    static const int DEFAULT_HEIGHT = 300;

    QGWindow(double width = 0, double height = 0, bool visible = true);
    virtual ~QGWindow();
    virtual void add(QGInteractor* interactor);
    virtual void addToRegion(QGInteractor* interactor, Region region);
    virtual void addToRegion(QGInteractor* interactor, const std::string& region = "Center");
    virtual void clear();
    virtual void clearCanvas();
    virtual void clearRegion(Region region);
    virtual void clearRegion(const std::string& region);
    virtual void center();
    virtual void close();
    virtual void compareToImage(const std::string& filename, bool ignoreWindowSize = true) const;
    virtual void draw(QGObject* obj);
    virtual void draw(QGObject& obj, double x, double y);
    virtual void draw(QGObject* obj, double x, double y);
    virtual void drawImage(const std::string& filename, double x = 0, double y = 0);
    virtual void drawLine(const GPoint& p0, const GPoint& p1);
    virtual void drawLine(double x0, double y0, double x1, double y1);
    virtual void drawOval(const GRectangle& bounds);
    virtual void drawOval(double x, double y, double width, double height);
    virtual GPoint drawPolarLine(const GPoint& p0, double r, double theta);
    virtual GPoint drawPolarLine(double x0, double y0, double r, double theta);
    virtual void drawPixel(double x, double y);
    virtual void drawPixel(double x, double y, int color);
    virtual void drawPixel(double x, double y, const std::string& color);
    virtual void drawPolygon(std::initializer_list<double> coords);
    virtual void drawPolygon(std::initializer_list<GPoint> points);
    virtual void drawRect(const GRectangle& bounds);
    virtual void drawRect(double x, double y, double width, double height);
    virtual void drawString(const std::string& text, double x, double y);
    virtual void fillOval(const GRectangle& bounds);
    virtual void fillOval(double x, double y, double width, double height);
    virtual void fillPolygon(std::initializer_list<double> coords);
    virtual void fillRect(const GRectangle& bounds);
    virtual void fillRect(double x, double y, double width, double height);
    virtual double getCanvasHeight() const;
    virtual GDimension getCanvasSize() const;
    virtual double getCanvasWidth() const;
    virtual std::string getColor() const;
    virtual int getColorInt() const;
    virtual std::string getFillColor() const;
    virtual int getFillColorInt() const;
    virtual QGObject* getQGObject(int index) const;
    virtual QGObject* getQGObjectAt(double x, double y) const;
    virtual int getQGObjectCount() const;
    virtual Point getLocation() const;
    virtual double getHeight() const;
    virtual double getLineWidth() const;
    virtual int getPixel(double x, double y) const;
    virtual int getPixelARGB(double x, double y) const;
    virtual Grid<int> getPixels() const;
    virtual Grid<int> getPixelsARGB() const;
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
    virtual QWidget* getWidget() const;
    virtual double getWidth() const;
    virtual double getX() const;
    virtual double getY() const;
    virtual bool inBounds(double x, double y) const;
    virtual bool inCanvasBounds(double x, double y) const;
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
    virtual void removeFromRegion(QGInteractor* interactor, Region region);
    virtual void removeFromRegion(QGInteractor* interactor, const std::string& region);
    virtual void repaint();
    virtual void requestFocus();
    virtual void saveCanvasPixels(const std::string& filename);
    virtual void setCanvasHeight(double height);
    virtual void setCanvasSize(double width, double height);
    virtual void setCanvasSize(const GDimension& size);
    virtual void setCanvasWidth(double width);
    virtual void setCloseOperation(CloseOperation op);
    virtual void setColor(int color);
    virtual void setColor(const std::string& color);
    virtual void setExitOnClose(bool exitOnClose);
    virtual void setFillColor(int color);
    virtual void setFillColor(const std::string& color);
    virtual void setFont(const std::string& font);
    virtual void setHeight(double width);
    virtual void setLineWidth(double lineWidth);
    virtual void setLocation(double x, double y);
    virtual void setLocation(const GPoint& p);
    virtual void setLocation(const Point& p);
    virtual void setClickHandler(QGEventHandler func);
    virtual void setClickHandler(QGEventHandlerVoid func);
    virtual void setMouseHandler(QGEventHandler func);
    virtual void setMouseHandler(QGEventHandlerVoid func);
    virtual void setWindowHandler(QGEventHandler func);
    virtual void setWindowHandler(QGEventHandlerVoid func);
    virtual void setPixel(double x, double y, int rgb);
    virtual void setPixelARGB(double x, double y, int argb);
    virtual void setPixels(const Grid<int>& pixels);
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB);
    virtual void setRegionAlignment(Region region, Alignment align);
    virtual void setRegionAlignment(const std::string& region, const std::string& align);
    virtual void setRepaintImmediately(bool repaintImmediately);
    virtual void setResizable(bool resizable);
    virtual void setSize(double width, double height);
    virtual void setSize(const GDimension& size);
    virtual void setTitle(const std::string& title);
    virtual void setVisible(bool visible);
    virtual void setWidth(double width);
    virtual void setWindowTitle(const std::string& title);
    virtual void setX(double x);
    virtual void setY(double y);
    virtual void toBack();
    virtual void toFront();

    // not to be called by students
    static QMainWindow* getLastWindow();

private:
    static _Internal_QMainWindow* _lastWindow;

    void ensureCanvas();
    void initializeQGObject(QGObject* obj, bool filled = false);
    QLayout* regionToLayout(Region region);
    QLayout* regionToLayout(const std::string& region);
    std::string regionToString(Region region);

    _Internal_QMainWindow* _iqmainwindow;
    QGCanvas* _canvas;
    std::string _color;
    std::string _fillColor;
    std::string _font;
    int _colorInt;
    int _fillColorInt;
    double _lineWidth;
    bool _resizable;

    friend class QGInteractor;
    friend class _Internal_QMainWindow;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgwindow_h
