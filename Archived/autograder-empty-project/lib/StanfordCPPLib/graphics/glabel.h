/*
 * File: glabel.h
 * --------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - moved some event-handling code to GInteractor superclass
 * @version 2019/04/22
 * - added setIcon with QIcon and QPixmap
 * @version 2018/10/04
 * - added get/setWordWrap
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/09/03
 * - added addActionListener methods for clickable labels
 * @version 2018/08/23
 * - renamed to glabel.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _glabel_h
#define _glabel_h

#include <string>
#include <QWindow>
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QSize>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "gobjects.h"
#undef INTERNAL_INCLUDE

// forward declaration
class _Internal_QLabel;
class GWindow;

/**
 * A GLabel represents a text string.
 * A label can contain text and/or an image icon.
 *
 * GLabels can be made clickable by setting an action, click, or double-click
 * listener, but generally if you want a clickable interactor with text on it,
 * you may prefer a GButton.
 */
class GLabel : public GInteractor {
public:
    /**
     * Creates a label with the specified text label and optional icon.
     */
    GLabel(const std::string& text = "", const std::string& iconFileName = "", QWidget* parent = nullptr);

    /**
     * Creates a label with the specified text label and icon.
     */
    GLabel(const std::string& text, const QIcon& icon, QWidget* parent = nullptr);

    /**
     * Creates a label with the specified text label and icon.
     */
    GLabel(const std::string& text, const QPixmap& icon, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the label.
     */
    virtual ~GLabel() Q_DECL_OVERRIDE;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the string displayed by the label.
     * Equivalent to getText.
     */
    virtual std::string getLabel() const;

    /**
     * Returns the string displayed by the label.
     * Equivalent to getLabel.
     */
    virtual std::string getText() const;

    /**
     * Returns the label's text position relative to its icon.
     * The default is TEXT_BESIDE_ICON, but it can be changed to TEXT_UNDER_ICON
     * by calling the setTextPosition method.
     */
    virtual GInteractor::TextPosition getTextPosition() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns whether the label should wrap if its text is too long.
     * Default false.
     */
    virtual bool isWordWrap() const;

    /* @inherit */
    virtual void setBounds(double x, double y, double width, double height) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setBounds(const GRectangle& size) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setColor(int rgb) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setColor(const std::string& color) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setFont(const QFont& font) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setFont(const std::string& font) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setForeground(int rgb) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setForeground(const std::string& color) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setHeight(double height) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setIcon(const QIcon& icon) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setIcon(const QPixmap& icon) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setIcon(const std::string& filename, bool retainIconSize = true) Q_DECL_OVERRIDE;

    /**
     * Sets the text on the label to be the given text.
     * Equivalent to setText.
     */
    virtual void setLabel(const std::string& text);

    /* @inherit */
    virtual void setLocation(double x, double y) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setSize(double width, double height) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setSize(const GDimension& size) Q_DECL_OVERRIDE;

    /**
     * Sets the text on the label to be the given text.
     * Equivalent to setLabel.
     */
    virtual void setText(const std::string& text);

    /**
     * Sets the label's text position relative to its icon.
     * The default is TEXT_BESIDE_ICON, but it can be changed to TEXT_UNDER_ICON.
     */
    virtual void setTextPosition(GInteractor::TextPosition position);

    /* @inherit */
    virtual void setVisible(bool visible) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setWidth(double width) Q_DECL_OVERRIDE;

    /**
     * Sets whether the label should wrap if its text is too long.
     * Default false.
     */
    virtual void setWordWrap(bool wrap);

    /* @inherit */
    virtual void setX(double x) Q_DECL_OVERRIDE;

    /* @inherit */
    virtual void setY(double y) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(GLabel)
    _Internal_QLabel* _iqlabel;
    GText* _gtext;

    void ensureGText();
    GText* getGText() const;
    bool hasGText() const;

    friend class _Internal_QLabel;
    friend class GWindow;
};

// alias GTextLabel for GLabel for backward compatibility
typedef GLabel GTextLabel;


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QLabel : public QLabel, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QLabel(GLabel* glabel, QWidget* parent = nullptr);
    virtual void detach() Q_DECL_OVERRIDE;
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

signals:
    void clicked();
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private:
    GLabel* _glabel;

    friend class GLabel;
};

#endif // _glabel_h
