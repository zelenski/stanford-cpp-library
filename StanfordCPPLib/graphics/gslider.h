/*
 * File: gslider.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to gslider.h to replace Java version
 * @version 2018/06/29
 * - initial version
 */

#ifndef _gslider_h
#define _gslider_h

#include <string>
#include <QSize>
#include <QSlider>
#include <QWidget>
#include "ginteractor.h"

// forward declaration
class GSlider;

// Internal class; not to be used by clients.
class _Internal_QSlider : public QSlider, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSlider(GSlider* qgslider, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleChange(int value);

private:
    GSlider* _gslider;
};

/*
 * ...
 */
class GSlider : public GInteractor {
public:
    static const int DEFAULT_MIN_VALUE;
    static const int DEFAULT_MAX_VALUE;
    static const int DEFAULT_INITIAL_VALUE;

    GSlider(int min = 0, int max = 100, int value = 50, QWidget* parent = nullptr);
    virtual ~GSlider();
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual int getMajorTickSpacing() const;
    virtual int getMinorTickSpacing() const;
    virtual bool getPaintLabels() const;
    virtual bool getPaintTicks() const;
    virtual bool getSnapToTicks() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual int getValue() const;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual void removeActionListener();
    virtual void setActionListener(GEventListener func);
    virtual void setActionListener(GEventListenerVoid func);
    virtual void setMajorTickSpacing(int value);
    virtual void setMinorTickSpacing(int value);
    virtual void setPaintLabels(bool value);
    virtual void setPaintTicks(bool value);
    virtual void setSnapToTicks(bool value);
    virtual void setValue(int value);

private:
    _Internal_QSlider* _iqslider;

    friend class _Internal_QSlider;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gslider_h
