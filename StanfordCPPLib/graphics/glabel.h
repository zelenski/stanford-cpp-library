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

// forward declaration
class GLabel;

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
    virtual _Internal_QWidget* getInternalWidget() const;
    virtual std::string getLabel() const;
    virtual std::string getText() const;
    virtual GInteractor::TextPosition getTextPosition() const;
    virtual std::string getType() const;
    virtual QWidget* getWidget() const;
    virtual void setIcon(const std::string& filename, bool retainIconSize = true);
    virtual void setLabel(const std::string& text);
    virtual void setText(const std::string& text);
    virtual void setTextPosition(GInteractor::TextPosition position);

private:
    _Internal_QLabel* _iqlabel;

    friend class _Internal_QImageLabel;
};

// alias GTextLabel for GLabel for backward compatibility
typedef GLabel GTextLabel;

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _glabel_h
