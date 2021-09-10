/*
 * File: gbutton.h
 * ---------------
 *
 * @author Marty Stepp
 * @version 2019/04/23
 * - moved some event-handling code to GInteractor superclass
 * @version 2019/04/22
 * - added setIcon with QIcon and QPixmap
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/09/04
 * - added double-click event support
 * @version 2018/08/23
 * - renamed to gbutton.h to replace Java version
 * @version 2018/06/25
 * - initial version
 */


#ifndef _gbutton_h
#define _gbutton_h

#include <string>
#include <QToolButton>

#include "ginteractor.h"

class _Internal_QPushButton;

/**
 * This interactor subclass represents an onscreen button.
 * You can listen for clicks on a button using the setActionListener method,
 * passing the function you want to call on each click.
 */
class GButton : public GInteractor {
public:
    /**
     * Creates a button with the specified text label and optional icon.
     */
    GButton(const std::string& text = "", const std::string& iconFileName = "", QWidget* parent = nullptr);

    /**
     * Creates a button with the specified text label and icon.
     */
    GButton(const std::string& text, const QIcon& icon, QWidget* parent = nullptr);

    /**
     * Creates a button with the specified text label and icon.
     */
    GButton(const std::string& text, const QPixmap& icon, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the button.
     */
    ~GButton() override;

    /* @inherit */
    std::string getAccelerator() const override;

    /* @inherit */
    std::string getActionCommand() const override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /**
     * Returns the button's text.
     * @return the text
     */
    virtual std::string getText() const;

    /**
     * Returns the button's text position relative to its icon.
     * The default is TEXT_BESIDE_ICON, but it can be changed to TEXT_UNDER_ICON
     * by calling the setTextPosition method.
     */
    virtual GInteractor::TextPosition getTextPosition() const;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

    /* @inherit */
    void setAccelerator(const std::string& accelerator) override;

    /* @inherit */
    void setIcon(const QIcon& icon) override;

    /* @inherit */
    void setIcon(const QPixmap& icon) override;

    /* @inherit */
    void setIcon(const std::string& filename, bool retainIconSize = true) override;

    /**
     * Sets the text on the button to be the given text.
     */
    virtual void setText(const std::string& text);

    /**
     * Sets the button's text position relative to its icon.
     * The default is TEXT_BESIDE_ICON, but it can be changed to TEXT_UNDER_ICON.
     */
    virtual void setTextPosition(GInteractor::TextPosition position);

private:
    Q_DISABLE_COPY(GButton)
    _Internal_QPushButton* _iqpushbutton;

    friend class _Internal_QPushButton;
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QPushButton : public QToolButton, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QPushButton(GButton* button, QWidget* parent = nullptr);
    void detach() override;
    QSize sizeHint() const override;

signals:
    void doubleClicked();

public slots:
    void handleClick();

protected:
    void mouseDoubleClickEvent(QMouseEvent* e) override;

private:
    GButton* _gbutton;

    friend class GButton;
};

#endif // _gbutton_h
