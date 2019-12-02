/*
 * File: gspacer.h
 * ---------------
 *
 * A GSpacer is just an empty blob of space that helps you pad layouts.
 *
 * @author Marty Stepp
 * @version 2018/10/06
 * - initial version
 */


#ifndef _gspacer_h
#define _gspacer_h

#include <string>

#include "ginteractor.h"

class _Internal_QSpacer;
/**
 * A GSpacer is just an empty blob of space that helps you pad layouts.
 */
class GSpacer : public GInteractor {
public:
    GSpacer(double width, double height, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the scroll pane.
     */
    ~GSpacer() override;

    /* @inherit */
    _Internal_QWidget* getInternalWidget() const override;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    QWidget* getWidget() const override;

private:
    Q_DISABLE_COPY(GSpacer)

    _Internal_QSpacer* _iqspacer;

    friend class _Internal_QSpacer;
};


/**
 * Internal class; not to be used by clients.
 * @private
 */
class _Internal_QSpacer : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QSpacer(GSpacer* gspacer, double width, double height, QWidget* parent = nullptr);
    QSize sizeHint() const override;

// private:
    // GSpacer* _gspacer;
};

#endif // _gspacer_h
