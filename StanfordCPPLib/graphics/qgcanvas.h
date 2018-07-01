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
class QGCanvas : public QGInteractor, public QGCompound {
public:
    QGCanvas(QWidget* _parent = nullptr);
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;

private:
    _Q_Internal_Canvas _canvas;

    friend class _Q_Internal_Canvas;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgcanvas_h
