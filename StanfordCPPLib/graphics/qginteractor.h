/*
 * File: qginteractor.h
 * --------------------
 *
 * @version 2018/06/25
 * - initial version
 */

#ifndef _qginteractor_h
#define _qginteractor_h

#include <functional>
#include <string>
#include <QObject>
#include <QWidget>
#include "gtypes.h"
#include "map.h"
#include "qgevent.h"

class QGWindow;   // forward declaration

/*
 * ...
 */
class QGInteractor {
public:
    enum TextPosition {
        TEXT_BESIDE_ICON,
        TEXT_UNDER_ICON,
        TEXT_ONLY
    };

    QGInteractor();
    virtual std::string getAccelerator() const;
    virtual std::string getActionCommand() const;
    virtual GRectangle getBounds() const;
    virtual std::string getFont() const;
    virtual double getHeight() const;
    virtual std::string getIcon() const;
    virtual char getMnemonic() const;
    virtual std::string getType() const = 0;
    virtual QWidget* getWidget() const = 0;
    virtual double getWidth() const;
    virtual double getX() const;
    virtual double getY() const;
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
    virtual void setForeground(int rgb);
    virtual void setForeground(const std::string& color);
    virtual void setFont(const std::string& font);
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);
    virtual void setMnemonic(char mnemonic);
    virtual void setSize(double width, double height);
    virtual void setSize(const GDimension& size);
    virtual void setTooltip(const std::string& tooltipText);
    virtual void setVisible(bool visible);
    virtual std::string toString() const;

protected:
    virtual void clearEventHandlers();
    virtual void ensureThreadSafety();
    virtual void fireEvent(QGEvent& event);
    virtual QWidget* getInternalParent(QWidget* parent) const;
    virtual bool hasEventHandler(const std::string& eventName) const;
    virtual std::string normalizeAccelerator(const std::string& accelerator) const;
    virtual void setEventHandler(const std::string& eventName, QGEventHandler func);
    virtual void setEventHandler(const std::string& eventName, QGEventHandlerVoid func);

    Map<std::string, QGEvent::EventHandlerWrapper> _eventMap;
    std::string _actionCommand;
    std::string _icon;

    friend class QGWindow;
};

class _Internal_QWidget {
    // empty
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qginteractor_h
