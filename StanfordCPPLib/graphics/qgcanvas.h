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
#include <QWidget>
#include "qginteractor.h"
#include "qgobjects.h"

// forward declaration
class QGCanvas;

// Internal class; not to be used by clients.
class _Q_Internal_Canvas : public QWidget {
    Q_OBJECT

public:
    _Q_Internal_Canvas(QGCanvas* canvas, QWidget* parent = nullptr);

    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QGCanvas* _canvas;

    friend class QGCanvas;
};

/*
 * ...
 */
class QGCanvas : public virtual QGInteractor {
public:
    QGCanvas(QWidget* _parent = nullptr);
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;

    // QGCompound methods
    void add(QGObject* gobj);
    void add(QGObject* gobj, double x, double y);
    void clear();
    void draw(QPainter* painter);
    virtual bool contains(double x, double y) const;
    QGObject* getElement(int index) const;
    QGObject* getElementAt(double x, double y) const;
    int getElementCount() const;
    bool isAutoRepaint() const;
    void remove(QGObject* gobj);
    void removeAll();
    void repaint();
    void setAutoRepaint(bool autoRepaint);

private:
    _Q_Internal_Canvas _canvas;
    QGCompound _compound;

    friend class _Q_Internal_Canvas;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcanvas_h
