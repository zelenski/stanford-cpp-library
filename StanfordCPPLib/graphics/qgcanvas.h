/*
 * File: qgcanvas.h
 * ----------------
 *
 * @version 2018/06/30
 * - initial version
 */

#ifndef _qgcanvas_h
#define _qgcanvas_h

#include <string>
#include <QWindow>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QWheelEvent>
#include <QWidget>
#include "qgdrawingsurface.h"
#include "qgevent.h"
#include "qginteractor.h"
#include "qgobjects.h"

// forward declaration
class QGCanvas;

// Internal class; not to be used by clients.
class _Internal_QCanvas : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCanvas(QGCanvas* qgcanvas, QWidget* parent = nullptr);

    virtual void enterEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void leaveEvent(QEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;
    virtual void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:
    QGCanvas* _qgcanvas;

    friend class QGCanvas;
};

/*
 * ...
 */
class QGCanvas : public virtual QGInteractor, public virtual QGDrawingSurface {
public:
    QGCanvas(QWidget* _parent = nullptr);
    virtual ~QGCanvas();

    // QGCompound methods
    virtual void add(QGObject* gobj);
    virtual void add(QGObject* gobj, double x, double y);
    virtual void add(QGObject& gobj);
    virtual void add(QGObject& gobj, double x, double y);
    virtual void clear() Q_DECL_OVERRIDE;
    virtual void clearObjects();
    virtual void clearPixels();
    virtual bool contains(double x, double y) const;
    virtual void draw(QGObject* gobj) Q_DECL_OVERRIDE;
    virtual void draw(QPainter* painter) Q_DECL_OVERRIDE;
    virtual void flatten();
    virtual std::string getBackground() const Q_DECL_OVERRIDE;
    virtual int getBackgroundInt() const Q_DECL_OVERRIDE;
    virtual QGObject* getElement(int index) const;
    virtual QGObject* getElementAt(double x, double y) const;
    virtual int getElementCount() const;
    virtual int getPixel(double x, double y) const Q_DECL_OVERRIDE;
    virtual int getPixelARGB(double x, double y) const Q_DECL_OVERRIDE;
    virtual Grid<int> getPixels() const Q_DECL_OVERRIDE;
    virtual Grid<int> getPixelsARGB() const Q_DECL_OVERRIDE;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isAutoRepaint() const;
    virtual void remove(QGObject* gobj);
    virtual void remove(QGObject& gobj);
    virtual void removeAll();
    virtual void removeClickHandler();
    virtual void removeKeyHandler();
    virtual void removeMouseHandler();
    virtual void repaint() Q_DECL_OVERRIDE;
    virtual void repaintRegion(int x, int y, int width, int height) Q_DECL_OVERRIDE;
    virtual void setAutoRepaint(bool autoRepaint) Q_DECL_OVERRIDE;
    virtual void setBackground(int color) Q_DECL_OVERRIDE;
    virtual void setBackground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setClickHandler(QGEventHandler func);
    virtual void setClickHandler(QGEventHandlerVoid func);
    virtual void setColor(int color) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;
    virtual void setKeyHandler(QGEventHandler func);
    virtual void setKeyHandler(QGEventHandlerVoid func);
    virtual void setMouseHandler(QGEventHandler func);
    virtual void setMouseHandler(QGEventHandlerVoid func);
    virtual void setPixel(double x, double y, int rgb) Q_DECL_OVERRIDE;
    virtual void setPixel(double x, double y, int r, int g, int b) Q_DECL_OVERRIDE;
    virtual void setPixelARGB(double x, double y, int argb) Q_DECL_OVERRIDE;
    virtual void setPixelARGB(double x, double y, int a, int r, int g, int b) Q_DECL_OVERRIDE;
    virtual void setPixels(const Grid<int>& pixels) Q_DECL_OVERRIDE;
    virtual void setPixelsARGB(const Grid<int>& pixelsARGB) Q_DECL_OVERRIDE;

private:
    _Internal_QCanvas* _iqcanvas;
    QGCompound _qgcompound;
    QImage* _backgroundImage;

    friend class _Internal_QCanvas;

    void ensureBackgroundImage();
    void ensureBackgroundImageConstHack() const;
    void notifyOfResize(double width, double height);
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcanvas_h
