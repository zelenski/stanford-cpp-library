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


#ifndef _glabel_h
#define _glabel_h

#include <string>
#include <QLabel>

#include "ginteractor.h"
#include "gobjects.h"

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
    ~GLabel() override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

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
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

    /**
     * Returns whether the label should wrap if its text is too long.
     * Default false.
     */
    virtual bool isWordWrap() const;

    /* @inherit */
    void setBounds(double x, double y, double width, double height) override;

    /* @inherit */
    void setBounds(const GRectangle& size) override;

    /* @inherit */
    void setColor(int rgb) override;

    /* @inherit */
    void setColor(const std::string& color) override;

    /* @inherit */
    void setFont(const QFont& font) override;

    /* @inherit */
    void setFont(const std::string& font) override;

    /* @inherit */
    void setForeground(int rgb) override;

    /* @inherit */
    void setForeground(const std::string& color) override;

    /* @inherit */
    void setHeight(double height) override;

    /* @inherit */
    void setIcon(const QIcon& icon) override;

    /* @inherit */
    void setIcon(const QPixmap& icon) override;

    /* @inherit */
    void setIcon(const std::string& filename, bool retainIconSize = true) override;

    /**
     * Sets the text on the label to be the given text.
     * Equivalent to setText.
     */
    virtual void setLabel(const std::string& text);

    /* @inherit */
    void setLocation(double x, double y) override;

    /* @inherit */
    void setSize(double width, double height) override;

    /* @inherit */
    void setSize(const GDimension& size) override;

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
    void setVisible(bool visible) override;

    /* @inherit */
    void setWidth(double width) override;

    /**
     * Sets whether the label should wrap if its text is too long.
     * Default false.
     */
    virtual void setWordWrap(bool wrap);

    /* @inherit */
    void setX(double x) override;

    /* @inherit */
    void setY(double y) override;

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
    void detach() override;
    QSize sizeHint() const override;

signals:
    void clicked();
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    GLabel* _glabel;

    friend class GLabel;
};

#endif // _glabel_h
