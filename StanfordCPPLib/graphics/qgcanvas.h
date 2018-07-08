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
#include <QMouseEvent>
#include <QWidget>
#include "qginteractor.h"
#include "qgobjects.h"

// forward declaration
class QGCanvas;

// Internal class; not to be used by clients.
class _Internal_QCanvas : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QCanvas(QGCanvas* qgcanvas, QWidget* parent = nullptr);

    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QGCanvas* _qgcanvas;
    void fireQGEvent(QEvent* event, QGEvent::EventType eventType, const std::string& eventName);
    void fireQGEvent(QMouseEvent* event, QGEvent::EventType eventType, const std::string& eventName);

    friend class QGCanvas;
};

/*
 * ...
 */
class QGCanvas : public virtual QGInteractor {
public:
    QGCanvas(QWidget* _parent = nullptr);
    virtual ~QGCanvas();

    // QGCompound methods
    virtual void add(QGObject* gobj);
    virtual void add(QGObject* gobj, double x, double y);
    virtual void add(QGObject& gobj);
    virtual void add(QGObject& gobj, double x, double y);
    virtual void clear();
    virtual void draw(QPainter* painter);
    virtual bool contains(double x, double y) const;
    virtual QGObject* getElement(int index) const;
    virtual QGObject* getElementAt(double x, double y) const;
    virtual int getElementCount() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual bool isAutoRepaint() const;
    virtual void remove(QGObject* gobj);
    virtual void remove(QGObject& gobj);
    virtual void removeAll();
    virtual void repaint();
    virtual void setAutoRepaint(bool autoRepaint);

private:
    _Internal_QCanvas* _iqcanvas;
    QGCompound _qgcompound;

    friend class _Internal_QCanvas;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcanvas_h
