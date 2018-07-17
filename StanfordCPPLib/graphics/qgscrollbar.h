/*
 * File: qgscrollbar.h
 * -------------------
 *
 * @version 2018/07/16
 * - initial version
 */

#ifndef _qgscrollbar_h
#define _qgscrollbar_h

#include <string>
#include <QScrollBar>
#include <QSize>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGScrollBar;

// Internal class; not to be used by clients.
class _Internal_QScrollBar : public QScrollBar, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QScrollBar(QGScrollBar* qgscrollbar, Qt::Orientation orientation, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleValueChange(int value);

private:
    QGScrollBar* _qgscrollbar;
};

/*
 * Class: QGScrollBar
 * ------------------
 * A QGScrollBar represents a horizontal or vertical scroll bar that can be
 * dragged by the user.  The bar does not inherently cause any other interactor
 * to scroll itself.  If you want the bar to cause any effect, you must wait
 * for scroll events and respond to them.
 */
class QGScrollBar : public QGInteractor {
public:
    /*
     * The two valid orientations of scrollbars.
     * Since these relate to a JScrollBar in the Java back-end, their values
     * must match those of the scrollbar orientations as defined in Java Swing
     * in the JScrollBar class. See also:
     * https://docs.oracle.com/javase/7/docs/api/constant-values.html#javax.swing.SwingConstants.HORIZONTAL
     */
    enum Orientation {
        HORIZONTAL = 0,
        VERTICAL   = 1
    };

    QGScrollBar(Orientation orientation = VERTICAL, int value = 0, int extent = 10, int min = 0, int max = 100, QWidget* parent = nullptr);
    virtual ~QGScrollBar();

    /*
     * Returns the scroll bar's extent, meaning the amount of its range that is
     * currently in view.
     */
    virtual int getExtent() const;

    virtual _Internal_QWidget* getInternalWidget() const;

    /*
     * Returns the maximum value of the scroll bar.
     */
    virtual int getMax() const;

    /*
     * Returns the minimum allowed value of the scroll bar.
     */
    virtual int getMin() const;

    /*
     * Returns the orientation of the scroll bar, either HORIZONTAL or VERTICAL.
     */
    virtual Orientation getOrientation() const;

    virtual std::string getType() const;

    /*
     * Returns the current value of the scroll bar.
     */
    virtual int getValue() const;

    virtual QWidget* getWidget() const;

    virtual void removeValueChangeHandler();

    /*
     * Sets the scroll bar's extent, meaning the amount of its range that is
     * currently in view.
     */
    virtual void setExtent(int extent);

    /*
     * Sets the maximum value of the scroll bar.
     */
    virtual void setMax(int max);

    /*
     * Sets the minimum allowed value of the scroll bar.
     */
    virtual void setMin(int min);

    /*
     * Sets all of the relevant state of the scroll bar.
     */
    virtual void setState(int value, int extent, int min, int max);

    /*
     * Sets the current value of the scroll bar.
     */
    virtual void setValue(int value);

    virtual void setValueChangeHandler(QGEventHandler func);
    virtual void setValueChangeHandler(QGEventHandlerVoid func);

private:
    _Internal_QScrollBar* _iqscrollbar;

    friend class _Internal_QScrollBar;

    void updateSize();
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgscrollbar_h
