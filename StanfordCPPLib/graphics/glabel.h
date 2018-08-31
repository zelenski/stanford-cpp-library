/*
 * File: glabel.h
 * --------------
 *
 * @author Marty Stepp
 * @version 2018/08/23
 * - renamed to glabel.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#ifndef _glabel_h
#define _glabel_h

#include <string>
#include <QLabel>
#include <QSize>
#include <QWidget>
#include "ginteractor.h"
#include "gobjects.h"

// forward declaration
class GLabel;
class GWindow;

// Internal class; not to be used by clients.
class _Internal_QLabel : public QLabel, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QLabel(GLabel* qglabel, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    GLabel* _glabel;
};

/*
 * ...
 */
class GLabel : public GInteractor {
public:
    GLabel(const std::string& text = "", const std::string& iconFileName = "", QWidget* parent = nullptr);
    virtual ~GLabel();
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual std::string getLabel() const;
    virtual std::string getText() const;
    virtual GInteractor::TextPosition getTextPosition() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual void setBounds(double x, double y, double width, double height) Q_DECL_OVERRIDE;
    virtual void setBounds(const GRectangle& size) Q_DECL_OVERRIDE;
    virtual void setColor(int rgb) Q_DECL_OVERRIDE;
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;
    virtual void setForeground(int rgb) Q_DECL_OVERRIDE;
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;
    virtual void setHeight(double height) Q_DECL_OVERRIDE;
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);
    virtual void setLabel(const std::string& text);
    virtual void setLocation(double x, double y) Q_DECL_OVERRIDE;
    virtual void setSize(double width, double height) Q_DECL_OVERRIDE;
    virtual void setSize(const GDimension& size) Q_DECL_OVERRIDE;
    virtual void setText(const std::string& text);
    virtual void setTextPosition(GInteractor::TextPosition position);
    virtual void setVisible(bool visible) Q_DECL_OVERRIDE;
    virtual void setWidth(double width) Q_DECL_OVERRIDE;
    virtual void setX(double x) Q_DECL_OVERRIDE;
    virtual void setY(double y) Q_DECL_OVERRIDE;

private:
    _Internal_QLabel* _iqlabel;
    GText* _gtext;

    void ensureGText();
    GText* getGText() const;
    bool hasGText() const;

    friend class _Internal_QImageLabel;
    friend class GWindow;
};

// alias GTextLabel for GLabel for backward compatibility
typedef GLabel GTextLabel;

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _glabel_h
