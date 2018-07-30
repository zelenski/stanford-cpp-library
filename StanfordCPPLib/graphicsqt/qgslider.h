/*
 * File: qgslider.h
 * ----------------
 *
 * @version 2018/06/29
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qgslider_h
#define _qgslider_h

#include <string>
#include <QSize>
#include <QSlider>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGSlider;

// Internal class; not to be used by clients.
class _Internal_QSlider : public QSlider, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSlider(QGSlider* qgslider, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

public slots:
    void handleChange(int value);

private:
    QGSlider* _qgslider;
};

/*
 * ...
 */
class QGSlider : public QGInteractor {
public:
    static const int DEFAULT_MIN_VALUE;
    static const int DEFAULT_MAX_VALUE;
    static const int DEFAULT_INITIAL_VALUE;

    QGSlider(int min = 0, int max = 100, int value = 50, QWidget* parent = nullptr);
    virtual ~QGSlider();
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual int getMajorTickSpacing() const;
    virtual int getMinorTickSpacing() const;
    virtual bool getPaintLabels() const;
    virtual bool getPaintTicks() const;
    virtual bool getSnapToTicks() const;
    virtual std::string getType() const;
    virtual int getValue() const;
    virtual QWidget* getWidget() const;
    virtual void removeActionListener();
    virtual void setActionListener(QGEventListener func);
    virtual void setActionListener(QGEventListenerVoid func);
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

#endif // _qgslider_h
#endif // SPL_QT_GUI
