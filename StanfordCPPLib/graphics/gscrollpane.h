/*
 * File: gscrollpane.h
 * -------------------
 *
 * @author Marty Stepp
 * @version 2018/09/01
 * - initial version
 */

#ifndef _gscrollpane_h
#define _gscrollpane_h

#include <QScrollArea>
#include <QWidget>
#include "ginteractor.h"

// forward declaration
class _Internal_QScrollArea;

/*
 * ...
 */
class GScrollPane : public GInteractor {
public:
    enum ScrollBarPolicy {
        SCROLLBAR_AS_NEEDED,
        SCROLLBAR_ALWAYS,
        SCROLLBAR_NEVER
    };

    GScrollPane(GInteractor* interactor, QWidget* parent = nullptr);
    virtual ~GScrollPane();
    virtual ScrollBarPolicy getHorizontalScrollBarPolicy() const;
    virtual ScrollBarPolicy getVerticalScrollBarPolicy() const;
    virtual GInteractor* getInteractor() const;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual bool isInteractorStretch() const;
    virtual void setHorizontalScrollBarPolicy(ScrollBarPolicy policy);
    virtual void setScrollBarPolicy(ScrollBarPolicy policy);
    virtual void setVerticalScrollBarPolicy(ScrollBarPolicy policy);
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

// Internal class; not to be used by clients.
class _Internal_QScrollArea : public QScrollArea, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QScrollArea(GScrollPane* gscrollpane, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    GScrollPane* _gscrollpane;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gscrollpane_h
