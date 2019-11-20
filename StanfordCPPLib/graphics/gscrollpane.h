/*
 * File: gscrollpane.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/09/01
 * - initial version
 */


#ifndef _gscrollpane_h
#define _gscrollpane_h

#include <string>
#include <QScrollArea>

#include "ginteractor.h"

class _Internal_QScrollArea;

/**
 * A GScrollPane is a container that wraps another interactor with scroll bars.
 * It can be used to allow another interactor to be at its preferred size
 * (or some arbitrarily large size) while only occupying a smaller number of
 * onscreen pixels with vertical and/or horizontal scroll bars.
 */
class GScrollPane : public GInteractor {
public:
    /**
     * Constants to indicate whether scroll bars in each dimension should be
     * always shown, never shown, or shown only if the inner interactor's size
     * is large enough to require the scroll bar (default).
     */
    enum ScrollBarPolicy {
        SCROLLBAR_AS_NEEDED,
        SCROLLBAR_ALWAYS,
        SCROLLBAR_NEVER
    };

    /**
     * Creates a new scroll pane to scroll the given interactor.
     * @throw ErrorException if the interactor is null
     */
    GScrollPane(GInteractor* interactor, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the scroll pane.
     */
    ~GScrollPane() override;

    /**
     * Returns a constant indicating whether the horizontal scroll bar will be shown.
     */
    virtual ScrollBarPolicy getHorizontalScrollBarPolicy() const;

    /**
     * Returns a constant indicating whether the vertical scroll bar will be shown.
     */
    virtual ScrollBarPolicy getVerticalScrollBarPolicy() const;

    /**
     * Returns the inner interactor being wrapped by this scroll pane.
     */
    virtual GInteractor* getInteractor() const;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

    /**
     * Returns true if the inner interactor should stretch itself to its
     * preferred size.  Default true.
     */
    virtual bool isInteractorStretch() const;

    /**
     * Sets whether the horizontal scroll bar will be shown.
     */
    virtual void setHorizontalScrollBarPolicy(ScrollBarPolicy policy);

    /**
     * Sets whether the horizontal and vertical scroll bars will be shown.
     */
    virtual void setScrollBarPolicy(ScrollBarPolicy policy);

    /**
     * Sets whether the vertical scroll bar will be shown.
     */
    virtual void setVerticalScrollBarPolicy(ScrollBarPolicy policy);

    /**
     * Sets whether the inner interactor should stretch itself to its
     * preferred size.  Default true.
     */
    virtual void setInteractorStretch(bool stretch);

private:
    Q_DISABLE_COPY(GScrollPane)

    _Internal_QScrollArea* _iqscrollarea;
    GInteractor* _interactor;
    ScrollBarPolicy _horizontalScrollBarPolicy;
    ScrollBarPolicy _verticalScrollBarPolicy;

    friend class _Internal_QScrollArea;

    static Qt::ScrollBarPolicy toQtScrollBarPolicy(ScrollBarPolicy policy);
};

/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QScrollArea : public QScrollArea, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QScrollArea(GScrollPane* gscrollpane, QWidget* parent = nullptr);
    QSize sizeHint() const override;

// private:
    // GScrollPane* _gscrollpane;
};

#endif // _gscrollpane_h
