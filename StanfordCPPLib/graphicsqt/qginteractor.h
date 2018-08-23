/*
 * File: qginteractor.h
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifdef SPL_QT_GUI
#ifndef _qginteractor_h
#define _qginteractor_h

#include <functional>
#include <string>
#include <QObject>
#include <QWidget>
#include "map.h"
#include "qgevent.h"
#include "qgobservable.h"
#include "qgtypes.h"

class QGWindow;   // forward declaration

class _Internal_QWidget {
public:
    _Internal_QWidget();
    virtual QSize getMinimumSize() const;
    virtual bool hasMinimumSize() const;
    virtual QSize getPreferredSize() const;
    virtual bool hasPreferredSize() const;
    virtual void setMinimumSize(double width, double height);
    virtual void setMinimumSize(const QSize& size);
    virtual void setPreferredSize(double width, double height);
    virtual void setPreferredSize(const QSize& size);

private:
    QGDimension _minimumSize;
    QGDimension _preferredSize;
};

/*
 * ...
 */
class QGInteractor : public QGObservable {
public:
    enum TextPosition {
        TEXT_BESIDE_ICON,
        TEXT_UNDER_ICON,
        TEXT_ONLY
    };

    QGInteractor();
    virtual ~QGInteractor();
    virtual bool eventsEnabled() const Q_DECL_OVERRIDE;
    virtual std::string getAccelerator() const;
    virtual std::string getActionCommand() const;
    virtual std::string getBackground() const;
    virtual int getBackgroundInt() const;
    virtual QGRectangle getBounds() const;
    virtual std::string getColor() const;
    virtual int getColorInt() const;
    virtual std::string getFont() const;
    virtual std::string getForeground() const;
    virtual int getForegroundInt() const;
    virtual double getHeight() const;
    virtual std::string getIcon() const;
    virtual _Internal_QWidget* getInternalWidget() const = 0;
    virtual QGPoint getLocation() const;
    virtual double getMinimumHeight() const;
    virtual QGDimension getMinimumSize() const;
    virtual double getMinimumWidth() const;
    virtual char getMnemonic() const;
    virtual double getPreferredHeight() const;
    virtual QGDimension getPreferredSize() const;
    virtual double getPreferredWidth() const;
    virtual QGDimension getSize() const;
    virtual QWidget* getWidget() const = 0;
    virtual double getWidth() const;
    virtual double getX() const;
    virtual double getY() const;
    virtual bool inBounds(double x, double y) const;
    virtual bool inBounds(int x, int y) const;
    virtual bool isEnabled() const;
    virtual bool isVisible() const;
    virtual void requestFocus();
    virtual void setActionCommand(const std::string& actionCommand);
    virtual void setAccelerator(const std::string& accelerator);
    virtual void setBackground(int rgb);
    virtual void setBackground(const std::string& color);
    virtual void setBounds(double x, double y, double width, double height);
    virtual void setBounds(const QGRectangle& size);
    virtual void setColor(int rgb);
    virtual void setColor(const std::string& color);
    virtual void setEnabled(bool value);
    virtual void setForeground(int rgb);
    virtual void setForeground(const std::string& color);
    virtual void setFont(const std::string& font);
    virtual void setHeight(double height);
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);
    virtual void setLocation(double x, double y);
    virtual void setMinimumSize(double width, double height);
    virtual void setMinimumSize(const QGDimension& size);
    virtual void setMnemonic(char mnemonic);
    virtual void setPreferredHeight(double height);
    virtual void setPreferredSize(double width, double height);
    virtual void setPreferredSize(const QGDimension& size);
    virtual void setPreferredWidth(double width);
    virtual void setSize(double width, double height);
    virtual void setSize(const QGDimension& size);
    virtual void setTooltip(const std::string& tooltipText);
    virtual void setVisible(bool visible);
    virtual void setWidth(double width);
    virtual void setX(double x);
    virtual void setY(double y);

protected:
    virtual QWidget* getInternalParent(QWidget* parent) const;
    virtual std::string normalizeAccelerator(const std::string& accelerator) const;

    std::string _actionCommand;
    std::string _icon;

    friend class QGWindow;
};

/*
 * A generic simple QGInteractor that wraps any Qt QWidget you pass it.
 * This is meant to make the library extensible and allow you to wrap other
 * Qt widgets that were not built into the original library.
 */
template <typename T>
class QGGenericInteractor : public QGInteractor {
public:
    QGGenericInteractor(T* widget)
            : _widget(widget) {
        _iqwidget = new _Internal_QWidget();   // dummy
    }

    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE {
        return _iqwidget;
    }

    virtual std::string getType() const Q_DECL_OVERRIDE {
        std::string typeName = typeid(T).name();
        return std::string("QGGenericInteractor<") + typeName + ">";
    }

    virtual QWidget* getWidget() const Q_DECL_OVERRIDE {
        return _widget;
    }

private:
    _Internal_QWidget* _iqwidget;
    T* _widget;
};


#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qginteractor_h
#endif // SPL_QT_GUI
