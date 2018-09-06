/*
 * File: ginteractor.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/09/05
 * - added getContainer and is/setVisible logic
 * @version 2018/08/23
 * - renamed to ginteractor.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#ifndef _ginteractor_h
#define _ginteractor_h

#include <functional>
#include <string>
#include <QObject>
#include <QWidget>
#include "map.h"
#include "gevent.h"
#include "gobservable.h"
#include "gtypes.h"

class GContainer;
class GWindow;
class _Internal_QWidget;

/*
 * ...
 */
class GInteractor : public GObservable {
public:
    enum TextPosition {
        TEXT_BESIDE_ICON,
        TEXT_UNDER_ICON,
        TEXT_ONLY
    };

    GInteractor();
    virtual ~GInteractor();
    virtual void addActionListener() Q_DECL_DEPRECATED;   // does nothing
    virtual bool eventsEnabled() const Q_DECL_OVERRIDE;
    virtual std::string getAccelerator() const;
    virtual std::string getActionCommand() const;
    virtual std::string getBackground() const;
    virtual int getBackgroundInt() const;
    virtual GRectangle getBounds() const;
    virtual std::string getColor() const;
    virtual int getColorInt() const;
    virtual GContainer* getContainer() const;
    virtual std::string getFont() const;
    virtual std::string getForeground() const;
    virtual int getForegroundInt() const;
    virtual double getHeight() const;
    virtual int getID() const;
    virtual std::string getIcon() const;
    virtual _Internal_QWidget* getInternalWidget() const = 0;
    virtual GPoint getLocation() const;
    virtual double getMinimumHeight() const;
    virtual GDimension getMinimumSize() const;
    virtual double getMinimumWidth() const;
    virtual char getMnemonic() const;
    virtual std::string getName() const;
    virtual double getPreferredHeight() const;
    virtual GDimension getPreferredSize() const;
    virtual double getPreferredWidth() const;
    virtual GDimension getSize() const;
    virtual std::string getType() const = 0;
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
    virtual void setBounds(const GRectangle& size);
    virtual void setColor(int rgb);
    virtual void setColor(const std::string& color);
    virtual void setEnabled(bool value);
    virtual void setFont(const QFont& font);
    virtual void setFont(const std::string& font);
    virtual void setForeground(int rgb);
    virtual void setForeground(const std::string& color);
    virtual void setHeight(double height);
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);
    virtual void setLocation(double x, double y);
    virtual void setMinimumSize(double width, double height);
    virtual void setMinimumSize(const GDimension& size);
    virtual void setMnemonic(char mnemonic) Q_DECL_DEPRECATED;
    virtual void setName(const std::string& name);
    virtual void setPreferredHeight(double height);
    virtual void setPreferredSize(double width, double height);
    virtual void setPreferredSize(const GDimension& size);
    virtual void setPreferredWidth(double width);
    virtual void setSize(double width, double height);
    virtual void setSize(const GDimension& size);
    virtual void setTooltip(const std::string& tooltipText);
    virtual void setVisible(bool visible);
    virtual void setWidth(double width);
    virtual void setX(double x);
    virtual void setY(double y);

protected:
    std::string _actionCommand;
    std::string _icon;
    std::string _name;
    int _id;
    GContainer* _container;

    static QWidget* getInternalParent(QWidget* parent);
    static std::string normalizeAccelerator(const std::string& accelerator);
    virtual std::string getDefaultInteractorName() const;
    virtual void setContainer(GContainer* container);

    friend class GContainer;
    friend class GWindow;
    friend class _Internal_QWidget;

private:
    static int _interactorCount;
};


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
    GDimension _minimumSize;
    GDimension _preferredSize;

    friend class GInteractor;
};

/*
 * A generic simple GInteractor that wraps any Qt QWidget you pass it.
 * This is meant to make the library extensible and allow you to wrap other
 * Qt widgets that were not built into the original library.
 */
template <typename T>
class GGenericInteractor : public GInteractor {
public:
    GGenericInteractor(T* widget)
            : _widget(widget) {
        _iqwidget = new _Internal_QWidget();   // dummy
    }

    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE {
        return _iqwidget;
    }

    virtual std::string getType() const Q_DECL_OVERRIDE {
        std::string typeName = typeid(T).name();
        return std::string("GGenericInteractor<") + typeName + ">";
    }

    virtual QWidget* getWidget() const Q_DECL_OVERRIDE {
        return _widget;
    }

private:
    _Internal_QWidget* _iqwidget;
    T* _widget;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _ginteractor_h
