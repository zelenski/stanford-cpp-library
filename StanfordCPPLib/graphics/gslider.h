/*
 * File: gslider.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - added key events
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gslider.h to replace Java version
 * @version 2018/06/29
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gslider_h
#define _gslider_h

#include <string>
#include <QSize>
#include <QSlider>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

class _Internal_QSlider;

/**
 * This interactor subclass represents an onscreen slider.
 * Dragging the slider control generates action events.
 */
class GSlider : public GInteractor {
public:
    /**
     * The two valid orientations of sliders.
     */
    enum Orientation {
        HORIZONTAL = 0,
        VERTICAL   = 1
    };

    /**
     * Default minimum value for a slider (0).
     */
    static const int DEFAULT_MIN_VALUE;

    /**
     * Default maximum value for a slider (100).
     */
    static const int DEFAULT_MAX_VALUE;

    /**
     * Default initial value for a slider (50).
     */
    static const int DEFAULT_INITIAL_VALUE;

    /**
     * Creates a new horizontal slider with the given value range.
     * @throw ErrorException if min > max or value is not between min and max
     */
    GSlider(int min = 0, int max = 100, int value = 50, QWidget* parent = nullptr);

    /**
     * Creates a new horizontal or vertical slider with the given value range.
     * @throw ErrorException if min > max or value is not between min and max
     */
    GSlider(Orientation orientation, int min = 0, int max = 100, int value = 50, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the slider.
     */
    virtual ~GSlider() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the number of pixels of spacing between ticks on the slider.
     * @private
     */
    virtual int getMajorTickSpacing() const;

    /**
     * Returns the maximum allowed value of the slider.
     */
    virtual int getMax() const;

    /**
     * Returns the minimum allowed value of the slider.
     */
    virtual int getMin() const;

    /**
     * Returns the number of pixels of spacing between small ticks on the slider.
     * @private
     */
    virtual int getMinorTickSpacing() const;

    /**
     * Returns the orientation of the slider, either HORIZONTAL or VERTICAL.
     */
    virtual Orientation getOrientation() const;

    /**
     * Returns whether the slider paints text labels at regular increments.
     * Default false.
     * @private
     */
    virtual bool getPaintLabels() const;

    /**
     * Returns whether the slider paints tick marks at regular increments.
     * @private
     */
    virtual bool getPaintTicks() const;

    /**
     * Returns whether the slider's cursor will be constrained to stop only at
     * its tick marks.
     * @private
     */
    virtual bool getSnapToTicks() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /**
     * Returns the slider's current value.
     */
    virtual int getValue() const;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Sets the number of pixels of spacing between ticks on the slider.
     * @private
     */
    virtual void setMajorTickSpacing(int value);

    /**
     * Sets the maximum allowed value of the slider.
     * @throw ErrorException if min > max
     */
    virtual void setMax(int max);

    /**
     * Sets the minimum allowed value of the slider.
     * @throw ErrorException if min > max
     */
    virtual void setMin(int min);

    /**
     * Sets the number of pixels of spacing between small ticks on the slider.
     * @private
     */
    virtual void setMinorTickSpacing(int value);

    /**
     * Sets whether the slider paints text labels at regular increments.
     * Default false.
     * @private
     */
    virtual void setPaintLabels(bool value);

    /**
     * Sets whether the slider paints tick marks at regular increments.
     * @private
     */
    virtual void setPaintTicks(bool value);

    /**
     * Sets the min-max range of the slider.
     * @throw ErrorException if min > max or value is not between min and max
     */
    virtual void setRange(int min, int max);

    /**
     * Sets whether the slider's cursor will be constrained to stop only at
     * its tick marks.
     * @private
     */
    virtual void setSnapToTicks(bool value);

    /**
     * Sets all of the relevant state of the slider.
     * @throw ErrorException if min > max or value is not between min and max
     */
    virtual void setState(int min, int max, int value);

    /**
     * Sets the current value of the slider.
     * @throw ErrorException if value is not between min and max
     */
    virtual void setValue(int value);

protected:
    /**
     * @private
     */
    virtual std::string getActionEventType() const Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(GSlider)

    _Internal_QSlider* _iqslider;

    friend class _Internal_QSlider;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QSlider : public QSlider, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSlider(GSlider* qgslider, Qt::Orientation orientation = Qt::Horizontal, QWidget* parent = nullptr);
    virtual void detach() Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleChange(int value);

private:
    GSlider* _gslider;

    friend class GSlider;
};

#endif // _gslider_h
