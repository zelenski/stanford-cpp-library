/*
 * File: qgslider.h
 * ----------------
 *
 * @version 2018/06/29
 * - initial version
 */

#ifndef _qgslider_h
#define _qgslider_h

#include <string>
#include <QSlider>
#include <QWidget>
#include "qginteractor.h"

// forward declaration
class QGSlider;

// Internal class; not to be used by clients.
class _Q_Internal_Slider : public QSlider {
    Q_OBJECT

public:
    _Q_Internal_Slider(QGSlider* slider, QWidget* parent = nullptr);

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
    int getMajorTickSpacing() const;
    int getMinorTickSpacing() const;
    bool getPaintLabels() const;
    bool getPaintTicks() const;
    bool getSnapToTicks() const;
    virtual std::string getType() const;
    int getValue() const;
    virtual QWidget* getWidget() const;
    void setChangeHandler(std::function<void()> func);
    void setMajorTickSpacing(int value);
    void setMinorTickSpacing(int value);
    void setPaintLabels(bool value);
    void setPaintTicks(bool value);
    void setSnapToTicks(bool value);
    void setValue(int value);

private:
    _Q_Internal_Slider _slider;

    friend class _Q_Internal_Slider;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgslider_h
