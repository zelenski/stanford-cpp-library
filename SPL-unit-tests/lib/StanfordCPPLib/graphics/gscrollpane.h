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

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gscrollpane_h
#define _gscrollpane_h

#include <QScrollArea>
#include <QWidget>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#undef INTERNAL_INCLUDE

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
    virtual ~GScrollPane() Q_DECL_OVERRIDE;

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
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

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
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

// private:
    // GScrollPane* _gscrollpane;
};

#endif // _gscrollpane_h
