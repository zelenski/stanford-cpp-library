/*
 * File: gscrollpane.cpp
 * ---------------------
 *
 * @author Marty Stepp
 * @version 2018/09/01
 * - initial version
 */

#include "gscrollpane.h"
#include "glayout.h"
#include "gthread.h"

GScrollPane::GScrollPane(GInteractor* interactor, QWidget* parent)
        : _iqscrollarea(nullptr),
          _interactor(interactor) {
    GThread::runOnQtGuiThread([this, parent]() {
        _iqscrollarea = new _Internal_QScrollArea(this, getInternalParent(parent));
    });
    setInteractor(interactor);
    setInteractorStretch(true);
    setVisible(false);   // all widgets are not shown until added to a window
}

GScrollPane::~GScrollPane() {
    // TODO: delete _iqscrollarea;
    _iqscrollarea = nullptr;
}

GInteractor* GScrollPane::getInteractor() const {
    return _interactor;
}

_Internal_QWidget* GScrollPane::getInternalWidget() const {
    return _iqscrollarea;
}

std::string GScrollPane::getType() const {
    return "GScrollPane";
}

QWidget* GScrollPane::getWidget() const {
    return static_cast<QWidget*>(_iqscrollarea);
}

void GScrollPane::setInteractor(GInteractor* interactor) {
    GThread::runOnQtGuiThread([this, interactor]() {
        _iqscrollarea->setWidget(interactor->getWidget());
        GLayout::forceUpdate(_iqscrollarea);
    });
}

void GScrollPane::setInteractorStretch(bool stretch) {
    GThread::runOnQtGuiThread([this, stretch]() {
        _iqscrollarea->setWidgetResizable(stretch);
        GLayout::forceUpdate(_iqscrollarea);
    });
}


_Internal_QScrollArea::_Internal_QScrollArea(GScrollPane* gscrollpane, QWidget* parent)
        : QScrollArea(parent),
          _gscrollpane(gscrollpane) {
    // empty
}

QSize _Internal_QScrollArea::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QScrollArea::sizeHint();
    }
}
