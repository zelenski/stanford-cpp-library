/*
 * File: gcontainer.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2018/08/29
 * - initial version
 */

#include "gcontainer.h"
#include "error.h"
#include "glabel.h"
#include "glayout.h"
#include "gthread.h"
#include "strlib.h"

GContainer::GContainer(Layout layout, QWidget* parent)
        : _iqcontainer(nullptr),
          _layout(layout) {
    GThread::runOnQtGuiThread([this, layout, parent]() {
        _iqcontainer = new _Internal_QContainer(this, layout, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GContainer::GContainer(Layout /*layout*/, int rows, int cols, QWidget* parent)
        : _iqcontainer(nullptr),
          _layout(LAYOUT_GRID) {
    GThread::runOnQtGuiThread([this, rows, cols, parent]() {
        _iqcontainer = new _Internal_QContainer(this, rows, cols, getInternalParent(parent));
    });
    setVisible(false);   // all widgets are not shown until added to a window
}

GContainer::~GContainer() {
    // TODO: delete _iqcontainer;
    _iqcontainer = nullptr;
}

void GContainer::add(GInteractor* interactor) {
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }
    GThread::runOnQtGuiThread([this, widget]() {
        widget->setParent(_iqcontainer);
        _iqcontainer->add(widget);
    });
    _interactors.add(interactor);
}

void GContainer::add(GInteractor& interactor) {
    add(&interactor);
}

void GContainer::addToRegion(GInteractor* interactor, Region region) {
    _iqcontainer->addToRegion(interactor->getWidget(), region);
    _interactors.add(interactor);
    _interactorsByRegion[region].add(interactor);
}

void GContainer::addToRegion(GInteractor* interactor, const std::string& region) {
    addToRegion(interactor, stringToRegion(region));
}

void GContainer::addToRegion(GInteractor& interactor, Region region) {
    addToRegion(&interactor, region);
}

void GContainer::addToRegion(GInteractor& interactor, const std::string& region) {
    addToRegion(&interactor, region);
}

void GContainer::clear() {
    _iqcontainer->clear();
    _interactors.clear();
    _interactorsByRegion.clear();
}

void GContainer::clearRegion(Region region) {
    _iqcontainer->clearRegion(region);
    for (GInteractor* interactor : _interactorsByRegion[region]) {
        _interactors.removeValue(interactor);
    }
    _interactorsByRegion.remove(region);
}

void GContainer::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

Vector<GInteractor*> GContainer::getDescendents(const std::string& type) const {
    Vector<GInteractor*> result;
    for (GInteractor* interactor : _interactors) {
        // pre-order traversal; add parent and then visit children
        if (type.empty() || type == "*" || equalsIgnoreCase(type, interactor->getType())) {
            result.add(interactor);
        }

        if (equalsIgnoreCase(interactor->getType(), "GContainer")) {
            // recursively get all descendents of this child
            GContainer* subcontainer = static_cast<GContainer*>(interactor);
            Vector<GInteractor*> descendents = subcontainer->getDescendents(type);
            result.addAll(descendents);
        }
    }
    return result;
}

GInteractor* GContainer::getInteractor(int i) const {
    return _interactors[i];
}

const Vector<GInteractor*>& GContainer::getInteractors() const {
    return _interactors;
}

int GContainer::getInteractorCount() const {
    return _interactors.size();
}

GInteractor* GContainer::getInteractorByRegion(int i, Region region) const {
    return _interactorsByRegion[region][i];
}

GInteractor* GContainer::getInteractorByRegion(int i, const std::string& region) const {
    return getInteractorByRegion(i, stringToRegion(region));
}

int GContainer::getInteractorCountByRegion(Region region) const {
    return _interactorsByRegion[region].size();
}

int GContainer::getInteractorCountByRegion(const std::string& region) const {
    return getInteractorCountByRegion(stringToRegion(region));
}


_Internal_QWidget* GContainer::getInternalWidget() const {
    return _iqcontainer;
}

double GContainer::getMargin() const {
    return _iqcontainer->getMargin();
}

double GContainer::getPadding() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return (left + top + right + bottom) / 4.0;
}

double GContainer::getPaddingBottom() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return bottom;
}

double GContainer::getPaddingLeft() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return left;
}

double GContainer::getPaddingRight() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return right;
}

double GContainer::getPaddingTop() const {
    int left, right, top, bottom;
    _iqcontainer->getContentsMargins(&left, &top, &right, &bottom);
    return top;
}

double GContainer::getSpacing() const {
    return _iqcontainer->getSpacing();
}

std::string GContainer::getType() const {
    return "GContainer";
}

QWidget* GContainer::getWidget() const {
    return static_cast<QWidget*>(_iqcontainer);
}

void GContainer::insert(int index, GInteractor* interactor) {
    _iqcontainer->insert(index, interactor->getWidget());
    _interactors.insert(index, interactor);
}

void GContainer::insert(int index, GInteractor& interactor) {
    insert(index, &interactor);
}

void GContainer::insertToRegion(int index, GInteractor* interactor, Region region) {
    _iqcontainer->insertToRegion(index, interactor->getWidget(), region);
    _interactors.add(interactor);
    _interactorsByRegion[region].insert(index, interactor);
}

void GContainer::insertToRegion(int index, GInteractor* interactor, const std::string& region) {
    insertToRegion(index, interactor, stringToRegion(region));
}

void GContainer::insertToRegion(int index, GInteractor& interactor, Region region) {
    insertToRegion(index, &interactor, region);
}

void GContainer::insertToRegion(int index, GInteractor& interactor, const std::string& region) {
    insertToRegion(index, &interactor, stringToRegion(region));
}

bool GContainer::isEmpty() const {
    return getInteractorCount() == 0;
}

void GContainer::remove(GInteractor* interactor) {
    _iqcontainer->remove(interactor->getWidget());
    _interactors.removeValue(interactor);
}

void GContainer::remove(GInteractor& interactor) {
    remove(&interactor);
}

void GContainer::remove(int index) {
    _iqcontainer->remove(index);
    _interactors.remove(index);
}

void GContainer::removeFromRegion(GInteractor* interactor, Region region) {
    _iqcontainer->removeFromRegion(interactor->getWidget(), region);
    _interactors.removeValue(interactor);
    _interactorsByRegion[region].removeValue(interactor);
}

void GContainer::removeFromRegion(GInteractor* interactor, const std::string& region) {
    removeFromRegion(interactor, stringToRegion(region));
}

void GContainer::removeFromRegion(GInteractor& interactor, Region region) {
    removeFromRegion(&interactor, region);
}

void GContainer::removeFromRegion(GInteractor& interactor, const std::string& region) {
    removeFromRegion(&interactor, region);
}

void GContainer::removeFromRegion(int index, Region region) {
    _iqcontainer->removeFromRegion(index, region);
    GInteractor* interactor = _interactorsByRegion[region][index];
    _interactors.removeValue(interactor);
    _interactorsByRegion[region].remove(index);
}

void GContainer::removeFromRegion(int index, const std::string& region) {
    removeFromRegion(index, stringToRegion(region));
}

void GContainer::setAlignment(HorizontalAlignment halign, VerticalAlignment valign) {
    setHorizontalAlignment(halign);
    setVerticalAlignment(valign);
}

void GContainer::setHorizontalAlignment(HorizontalAlignment halign) {
    _iqcontainer->setHorizontalAlignment(halign);
}

void GContainer::setMargin(double px) {
    _iqcontainer->setMargin((int) px);
}

void GContainer::setPadding(double px) {
    _iqcontainer->setPadding((int) px, (int) px, (int) px, (int) px);
}

void GContainer::setPadding(double topBottom, double leftRight) {
    _iqcontainer->setPadding((int) topBottom, (int) leftRight, (int) topBottom, (int) leftRight);
}

void GContainer::setPadding(double top, double right, double bottom, double left) {
    _iqcontainer->setPadding((int) top, (int) right, (int) bottom, (int) left);
}

void GContainer::setVerticalAlignment(VerticalAlignment valign) {
    _iqcontainer->setVerticalAlignment(valign);
}

void GContainer::setRegionAlignment(Region region, HorizontalAlignment halign) {
    _iqcontainer->setRegionHorizontalAlignment(region, halign);
}

void GContainer::setRegionAlignment(Region region, VerticalAlignment valign) {
    _iqcontainer->setRegionVerticalAlignment(region, valign);
}

void GContainer::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    _iqcontainer->setRegionAlignment(region, halign, valign);

}

void GContainer::setRegionAlignment(const std::string& region, const std::string& align) {
    HorizontalAlignment halignment = toHorizontalAlignment(align);
    VerticalAlignment valignment = toVerticalAlignment(align);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void GContainer::setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign) {
    HorizontalAlignment halignment = toHorizontalAlignment(halign);
    VerticalAlignment valignment = toVerticalAlignment(valign);
    setRegionAlignment(stringToRegion(region), halignment, valignment);
}

void GContainer::setRegionHorizontalAlignment(Region region, HorizontalAlignment halign) {
    _iqcontainer->setRegionHorizontalAlignment(region, halign);
}

void GContainer::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    _iqcontainer->setRegionHorizontalAlignment(stringToRegion(region), toHorizontalAlignment(halign));
}

void GContainer::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    _iqcontainer->setRegionVerticalAlignment(stringToRegion(region), toVerticalAlignment(valign));
}

void GContainer::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    _iqcontainer->setRegionVerticalAlignment(region, valign);
}

void GContainer::setSpacing(double px) {
    _iqcontainer->setSpacing((int) px);
}

GContainer::Region GContainer::stringToRegion(const std::string& regionStr) {
    std::string regionLC = toLowerCase(trim(regionStr));
    if (stringContains(regionLC, "north")) {
        return REGION_NORTH;
    } else if (stringContains(regionLC, "south")) {
        return REGION_SOUTH;
    } else if (stringContains(regionLC, "west")) {
        return REGION_WEST;
    } else if (stringContains(regionLC, "east")) {
        return REGION_EAST;
    } else {
        return REGION_CENTER;
    }
}



_Internal_QContainer::_Internal_QContainer(GContainer* gcontainer, GContainer::Layout layoutType, QWidget* parent)
        : QWidget(parent),
          _gcontainer(gcontainer),
          _layoutType(GContainer::LAYOUT_NONE),
          _halign(ALIGN_CENTER),
          _valign(ALIGN_MIDDLE),
          _margin(GContainer::MARGIN_DEFAULT),
          _spacing(GContainer::SPACING_DEFAULT),
          _rows(0),
          _cols(0),
          _currentIndex(-1),
          _overallLayout(nullptr),
          _northLayout(nullptr),
          _southLayout(nullptr),
          _westLayout(nullptr),
          _eastLayout(nullptr),
          _centerLayout(nullptr),
          _middleLayout(nullptr) {
    setLayoutType(layoutType);
    if (layout()) {
        setSpacing(GContainer::SPACING_DEFAULT);
    }
}

_Internal_QContainer::_Internal_QContainer(GContainer* gcontainer, int rows, int cols, QWidget* parent)
        : QWidget(parent),
          _gcontainer(gcontainer),
          _layoutType(GContainer::LAYOUT_NONE),
          _halign(ALIGN_CENTER),
          _valign(ALIGN_MIDDLE),
          _margin(GContainer::MARGIN_DEFAULT),
          _spacing(GContainer::SPACING_DEFAULT),
          _rows(rows),
          _cols(cols),
          _currentIndex(-1),
          _overallLayout(nullptr),
          _northLayout(nullptr),
          _southLayout(nullptr),
          _westLayout(nullptr),
          _eastLayout(nullptr),
          _centerLayout(nullptr),
          _middleLayout(nullptr) {
    setLayoutType(GContainer::LAYOUT_GRID);
    if (layout()) {
        setSpacing(GContainer::SPACING_DEFAULT);
    }
}

void _Internal_QContainer::add(QWidget* widget) {
    switch (_layoutType) {
        case GContainer::LAYOUT_BORDER: {
            addToRegion(widget, GContainer::REGION_CENTER);
            break;
        }
        case GContainer::LAYOUT_GRID: {
            _currentIndex++;
            int row = _cols <= 0 ? 0 : _currentIndex / _cols;
            int col = _cols <= 0 ? 0 : _currentIndex % _cols;
            QGridLayout* gridLayout = (QGridLayout*) getQLayout();
            gridLayout->addWidget(widget, row, col);
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // add to end of the list of widgets in this region
            QBoxLayout* boxLayout = (QBoxLayout*) getQLayout();
            boxLayout->insertWidget(/* index */ boxLayout->count() - 1, widget);
            break;
        }
        default: {
            error("GContainer::add: no layout has been set");
            break;
        }
    }
}

void _Internal_QContainer::addToRegion(QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        GThread::runOnQtGuiThread([this, region, widget, layout]() {
            if (layout == _centerLayout) {
                // center holds at most one widget
                GLayout::clearLayout(layout);
                layout->addWidget(widget);
                widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            } else {
                // add to end of the list of widgets in this region
                ((QBoxLayout*) layout)->insertWidget(/* index */ layout->count() - 1, widget);
                widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            }
            widget->setVisible(true);

            // set alignment of widget
            fixAlignment(widget, region);

            // fixMargins();
            GLayout::forceUpdate(this);
        });
    } else {
        add(widget);
    }
}

void _Internal_QContainer::clear() {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        clearRegion(GContainer::REGION_NORTH);
        clearRegion(GContainer::REGION_SOUTH);
        clearRegion(GContainer::REGION_WEST);
        clearRegion(GContainer::REGION_EAST);
        clearRegion(GContainer::REGION_CENTER);
    } else {
        GLayout::clearLayout(layout());
        _currentIndex = -1;
        setHorizontalAlignment(getHorizontalAlignment());
        setVerticalAlignment(getVerticalAlignment());
    }
}

void _Internal_QContainer::clearRegion(GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        GLayout::clearLayout(layout);
        GLayout::forceUpdate(this);
    } else {
        clear();
    }
}

void _Internal_QContainer::fixAlignment(QWidget* widget, GContainer::Region region) {
    // needs to be run on GUI thread
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        if (_halignMap.containsKey(region) && _valignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_halignMap[region]) | toQtAlignment(_valignMap[region]));
        } else if (_halignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_halignMap[region]));
        } else if (_valignMap.containsKey(region)) {
            layout->setAlignment(widget, toQtAlignment(_valignMap[region]));
        }
    } else {
        layout()->setAlignment(widget, toQtAlignment(_halign) | toQtAlignment(_valign));
    }
}

HorizontalAlignment _Internal_QContainer::getHorizontalAlignment() const {
    return _halign;
}

GContainer::Layout _Internal_QContainer::getLayoutType() const {
    return _layoutType;
}

int _Internal_QContainer::getMargin() const {
    return _margin;
}

QLayout* _Internal_QContainer::getQLayout() const {
    return layout();
}

int _Internal_QContainer::getSpacing() const {
    return _spacing;
}

VerticalAlignment _Internal_QContainer::getVerticalAlignment() const {
    return _valign;
}

void _Internal_QContainer::insert(int i, QWidget* widget) {
    switch (_layoutType) {
        case GContainer::LAYOUT_BORDER: {
            insertToRegion(i, widget, GContainer::REGION_CENTER);
            break;
        }
        case GContainer::LAYOUT_GRID: {
            int row = _cols <= 0 ? 0 : i / _cols;
            int col = _cols <= 0 ? 0 : i % _cols;
            QGridLayout* gridLayout = (QGridLayout*) getQLayout();
            gridLayout->addWidget(widget, row, col);
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        case GContainer::LAYOUT_FLOW_VERTICAL:
        default: {
            // index is off by 1 because of 'stretch' widgets at start/end
            QBoxLayout* boxLayout = (QBoxLayout*) getQLayout();
            boxLayout->insertWidget(/* index */ i - 1, widget);
            break;
        }
    }
}

void _Internal_QContainer::insertToRegion(int i, QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        GThread::runOnQtGuiThread([this, i, widget, region, layout]() {
            if (layout == _centerLayout) {
                // center holds at most one widget
                GLayout::clearLayout(layout);
                layout->addWidget(widget);
                widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            } else {
                // index is off by 1 because of 'stretch' widgets at start/end
                ((QBoxLayout*) layout)->insertWidget(/* index */ i + 1, widget);
                widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            }
            widget->setVisible(true);

            // set alignment of widget
            fixAlignment(widget, region);

            // fixMargins();
            GLayout::forceUpdate(this);
        });
    } else {
        insert(i, widget);
    }
}

bool _Internal_QContainer::isRegionStretch(GContainer::Region region) const {
    return _regionStretchMap[region];
}

QLayout* _Internal_QContainer::layoutForRegion(GContainer::Region region) const {
    if (region == GContainer::REGION_NORTH) {
        return _northLayout;
    } else if (region == GContainer::REGION_SOUTH) {
        return _southLayout;
    } else if (region == GContainer::REGION_WEST) {
        return _westLayout;
    } else if (region == GContainer::REGION_EAST) {
        return _eastLayout;
    } else {
        return _centerLayout;
    }
}

void _Internal_QContainer::remove(QWidget* widget) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(GContainer::REGION_CENTER);
        layout->removeWidget(widget);
        layout->update();
        // fixMargins();
        GLayout::forceUpdate(this);
    } else if (layout()) {
        layout()->removeWidget(widget);
    }
}

void _Internal_QContainer::removeFromRegion(QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        GThread::runOnQtGuiThread([this, widget, layout]() {
            widget->setVisible(false);
            layout->removeWidget(widget);
            layout->update();
            // fixMargins();
            GLayout::forceUpdate(this);
        });
    } else {
        remove(widget);
    }
}

void _Internal_QContainer::remove(int i) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        removeFromRegion(i, GContainer::REGION_CENTER);
    } else {
        GThread::runOnQtGuiThread([this, i]() {
            QWidget* widget = layout()->itemAt(i)->widget();
            if (widget) {
                widget->setVisible(false);
            }
            // add +1 to the index to account for the 'stretch' at start
            layout()->removeItem(layout()->itemAt(i + 1));
            layout()->update();
            // fixMargins();
            GLayout::forceUpdate(this);
        });
    }
}

void _Internal_QContainer::removeFromRegion(int i, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        GThread::runOnQtGuiThread([this, i, layout]() {
            QWidget* widget = layout->itemAt(i)->widget();
            widget->setVisible(false);
            if (layout == _centerLayout) {
                layout->removeItem(layout->itemAt(i));
            } else {
                // add +1 to the index to account for the 'stretch' at start
                layout->removeItem(layout->itemAt(i + 1));
            }
            layout->update();
            // fixMargins();
            GLayout::forceUpdate(this);
        });
    } else {
        remove(i);
    }
}

void _Internal_QContainer::setPadding(int padding) {
    setPadding(padding, padding, padding, padding);
}

void _Internal_QContainer::setPadding(int top, int right, int bottom, int left) {
    QWidget::setContentsMargins(left, top, right, bottom);   // call super
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        _overallLayout->setContentsMargins(left, top, right, bottom);
        _northLayout->setContentsMargins(left, top, right, bottom);
        _southLayout->setContentsMargins(left, top, right, bottom);
        _westLayout->setContentsMargins(left, top, right, bottom);
        _eastLayout->setContentsMargins(left, top, right, bottom);
        _middleLayout->setContentsMargins(left, top, right, bottom);
        _centerLayout->setContentsMargins(left, top, right, bottom);
    }
    GLayout::forceUpdate(this);
}

void _Internal_QContainer::setHorizontalAlignment(HorizontalAlignment halign) {
    switch (_layoutType) {
        case GContainer::LAYOUT_FLOW_HORIZONTAL: {
            // to align "left", limit first stretch;
            // to align "right", limit last stretch
            QHBoxLayout* layout = (QHBoxLayout*) getQLayout();
            if (layout->count() >= 2) {
                layout->removeItem(layout->itemAt(0));
                layout->removeItem(layout->itemAt(layout->count() - 1));
            }
            if (halign == ALIGN_LEFT) {
                layout->insertStretch(0, /* stretch */ 0);
                layout->addStretch(/* stretch */ 99);
            } else if (halign == ALIGN_RIGHT) {
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 0);
            } else {   // halign == ALIGN_CENTER
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 99);
            }
            break;
        }
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // set each widget's horizontal alignment individually
            QVBoxLayout* layout = (QVBoxLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(halign);
            for (int i = 1; i < layout->count() - 1; i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (widget) {
                    layout->setAlignment(widget, qtAlign);
                }
            }
            break;
        }
        case GContainer::LAYOUT_GRID: {
            // set each widget's horizontal alignment individually
            QGridLayout* layout = (QGridLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(halign);
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (widget) {
                    layout->setAlignment(widget, qtAlign);
                }
            }
            break;
        }
        // TODO: border
        // - set h/valign of ALL regions in align map
        default: {
            break;
        }
    }
}

void _Internal_QContainer::setLayoutType(GContainer::Layout layoutType) {
    if (layoutType == _layoutType) {
        return;   // don't do work if it's the same layout
    }

    if (_layoutType == GContainer::LAYOUT_BORDER
            && layoutType != GContainer::LAYOUT_BORDER) {
        // get rid of the now-unneeded inner layouts
        _overallLayout = nullptr;
        _northLayout   = nullptr;
        _southLayout   = nullptr;
        _westLayout    = nullptr;
        _eastLayout    = nullptr;
        _centerLayout  = nullptr;
        _middleLayout  = nullptr;
    }

    _layoutType = layoutType;
    switch (layoutType) {
        case GContainer::LAYOUT_BORDER: {
            // set up border regions
            _overallLayout = new QVBoxLayout;
            _northLayout   = new QHBoxLayout;
            _southLayout   = new QHBoxLayout;
            _westLayout    = new QVBoxLayout;
            _eastLayout    = new QVBoxLayout;
            _centerLayout  = new QHBoxLayout;
            _middleLayout  = new QHBoxLayout;

            // squish margins/padding
            _overallLayout->setSpacing(0);
            _northLayout->setSpacing(getSpacing());
            _southLayout->setSpacing(getSpacing());
            _westLayout->setSpacing(getSpacing());
            _eastLayout->setSpacing(getSpacing());
            _centerLayout->setSpacing(0);
            _middleLayout->setSpacing(0);

            _overallLayout->setMargin(0);
            _northLayout->setMargin(getMargin());
            _southLayout->setMargin(getMargin());
            _westLayout->setMargin(getMargin());
            _eastLayout->setMargin(getMargin());
            _centerLayout->setMargin(0);
            _middleLayout->setMargin(0);

            // add "stretches" at start and end of N/S/W/E regions
            // to center and un-space the actual widgets in each
            _northLayout->addStretch(99);
            _northLayout->addStretch(99);
            _southLayout->addStretch(99);
            _southLayout->addStretch(99);
            _westLayout->addStretch(99);
            _westLayout->addStretch(99);
            _eastLayout->addStretch(99);
            _eastLayout->addStretch(99);

            _overallLayout->addLayout(_northLayout, /* stretch */ 0);
            _middleLayout->addLayout(_westLayout, /* stretch */ 0);
            _middleLayout->addLayout(_centerLayout, /* stretch */ 99);
            _middleLayout->addLayout(_eastLayout, /* stretch */ 0);
            _overallLayout->addLayout(_middleLayout, /* stretch */ 99);
            _overallLayout->addLayout(_southLayout, /* stretch */ 0);
            setLayout(_overallLayout);
            if (layout()) {
                layout()->setSpacing(0);
                layout()->setMargin(0);
            }
            break;
        }
        case GContainer::LAYOUT_GRID: {
            QGridLayout* qlayout = new QGridLayout;
            if (layout()) {
                // transfer over from previous layout
                for (int i = 1; i < layout()->count() - 1; i++) {
//                    QWidget* widget = layout()->itemAt(i)->widget();
//                    qlayout->addWidget(widget);
                }
            }
            setLayout(qlayout);
            break;
        }
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            QVBoxLayout* qlayout = new QVBoxLayout;
            qlayout->setSpacing(getSpacing());
            qlayout->setMargin(getMargin());
            qlayout->addStretch(99);   // top side stretch
            if (layout()) {
                // transfer over from previous layout
                for (int i = 1; i < layout()->count() - 1; i++) {
                    QWidget* widget = layout()->itemAt(i)->widget();
                    if (widget) {
                        qlayout->addWidget(widget);
                    }
                }
            }
            qlayout->addStretch(99);   // bottom side stretch
            setLayout(qlayout);
            setHorizontalAlignment(getHorizontalAlignment());
            setVerticalAlignment(getVerticalAlignment());
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        default: {
            QHBoxLayout* qlayout = new QHBoxLayout;
            qlayout->setSpacing(getSpacing());
            qlayout->setMargin(getMargin());
            qlayout->addStretch(99);   // left side stretch
            if (layout()) {
                // transfer over from previous layout
                for (int i = 1; i < layout()->count() - 1; i++) {
                    QWidget* widget = layout()->itemAt(i)->widget();
                    qlayout->addWidget(widget);
                }
            }
            qlayout->addStretch(99);   // right side stretch
            setLayout(qlayout);
            setHorizontalAlignment(getHorizontalAlignment());
            setVerticalAlignment(getVerticalAlignment());
            break;
        }
    }
}

void _Internal_QContainer::setMargin(int margin) {
    _margin = margin;
    if (layout()) {
        if (_layoutType == GContainer::LAYOUT_BORDER) {
            _northLayout->setMargin(margin);
            _southLayout->setMargin(margin);
            _westLayout->setMargin(margin);
            _eastLayout->setMargin(margin);
            _centerLayout->setMargin(margin);
        } else {
            layout()->setMargin(margin);
        }
    }
}

void _Internal_QContainer::setRegionAlignment(GContainer::Region region,
                                              HorizontalAlignment halign,
                                              VerticalAlignment valign) {
    setRegionHorizontalAlignment(region, halign);
    setRegionVerticalAlignment(region, valign);
}

void _Internal_QContainer::setRegionHorizontalAlignment(GContainer::Region region,
                                                        HorizontalAlignment halign) {
    GThread::runOnQtGuiThread([this, region, halign]() {
        _halignMap[region] = halign;
        QLayout* layout = layoutForRegion(region);
        if (layout == _westLayout || layout == _eastLayout || layout == _centerLayout) {
            // set each widget's horizontal alignment individually
            Qt::Alignment qtAlign = toQtAlignment(halign);
            QSizePolicy::Policy hSizePolicy = ((qtAlign & Qt::AlignJustify) != 0) ? QSizePolicy::Expanding : QSizePolicy::Preferred;
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (!widget) {
                    continue;
                }
                layout->setAlignment(widget, qtAlign);
                widget->setSizePolicy(
                        /* horizontal */ hSizePolicy,
                        /* vertical */   widget->sizePolicy().verticalPolicy());
            }
        } else if (layout == _northLayout || layout == _southLayout) {
            // to align "left", limit first stretch;
            // to align "right", limit last stretch
            QHBoxLayout* boxLayout = (QHBoxLayout*) layout;
            boxLayout->removeItem(layout->itemAt(0));
            boxLayout->removeItem(layout->itemAt(layout->count() - 1));
            int beforeStretch = (halign == ALIGN_CENTER || halign == ALIGN_RIGHT)  ? 99 : 0;
            int afterStretch  = (halign == ALIGN_LEFT   || halign == ALIGN_CENTER) ? 99 : 0;
            boxLayout->insertStretch(0, beforeStretch);
            boxLayout->addStretch(afterStretch);
        }

        layout->update();
        GLayout::forceUpdate(this);
    });

}

void _Internal_QContainer::setRegionStretch(GContainer::Region region, bool stretch) {
    _regionStretchMap[region] = stretch;

    // stretch / de-stretch any widgets in that region

}

void _Internal_QContainer::setRegionVerticalAlignment(GContainer::Region region,
                                                      VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, region, valign]() {
        _valignMap[region] = valign;
        QLayout* layout = layoutForRegion(region);
        if (layout == _westLayout || layout == _eastLayout) {
            // to align "top", limit first stretch;
            // to align "bottom", limit last stretch
            QVBoxLayout* boxLayout = (QVBoxLayout*) layout;
            boxLayout->removeItem(layout->itemAt(0));
            boxLayout->removeItem(layout->itemAt(layout->count() - 1));
            int beforeStretch = (valign == ALIGN_MIDDLE || valign == ALIGN_BOTTOM) ? 99 : 0;
            int afterStretch  = (valign == ALIGN_TOP    || valign == ALIGN_MIDDLE) ? 99 : 0;
            boxLayout->insertStretch(0, beforeStretch);
            boxLayout->addStretch(afterStretch);
        } else if (layout == _northLayout || layout == _southLayout || layout == _centerLayout) {
            // set each widget's vertical alignment individually
            Qt::Alignment qtAlign = toQtAlignment(valign);
            QSizePolicy::Policy vSizePolicy = ((qtAlign & Qt::AlignJustify) != 0) ? QSizePolicy::Expanding : QSizePolicy::Preferred;
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (!widget) {
                    continue;
                }
                layout->setAlignment(widget, qtAlign);
                widget->setSizePolicy(
                        /* horizontal */ widget->sizePolicy().horizontalPolicy(),
                        /* vertical */   vSizePolicy);
            }
        }

        layout->update();
        GLayout::forceUpdate(this);
    });
}

void _Internal_QContainer::setSpacing(int spacing) {
    _spacing = spacing;
    if (layout()) {
        if (_layoutType == GContainer::LAYOUT_BORDER) {
            _northLayout->setSpacing(spacing);
            _southLayout->setSpacing(spacing);
            _westLayout->setSpacing(spacing);
            _eastLayout->setSpacing(spacing);
            _centerLayout->setSpacing(spacing);
        } else {
            layout()->setSpacing(spacing);
        }
    }
}

void _Internal_QContainer::setVerticalAlignment(VerticalAlignment valign) {
    switch (_layoutType) {
        case GContainer::LAYOUT_FLOW_HORIZONTAL: {
            // set each widget's vertical alignment individually
            QVBoxLayout* layout = (QVBoxLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(valign);
            for (int i = 1; i < layout->count() - 1; i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (!widget) {
                    continue;
                }
                layout->setAlignment(widget, qtAlign);
            }
            break;
        }
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // to align "left", limit first stretch;
            // to align "right", limit last stretch
            QVBoxLayout* layout = (QVBoxLayout*) getQLayout();
            if (layout->count() >= 2) {
                layout->removeItem(layout->itemAt(0));
                layout->removeItem(layout->itemAt(layout->count() - 1));
            }
            if (valign == ALIGN_TOP) {
                layout->insertStretch(0, /* stretch */ 0);
                layout->addStretch(/* stretch */ 99);
            } else if (valign == ALIGN_BOTTOM) {
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 0);
            } else {   // halign == ALIGN_MIDDLE
                layout->insertStretch(0, /* stretch */ 99);
                layout->addStretch(/* stretch */ 99);
            }
            break;
        }
        case GContainer::LAYOUT_GRID: {
            // set each widget's vertical alignment individually
            QGridLayout* layout = (QGridLayout*) getQLayout();
            Qt::Alignment qtAlign = toQtAlignment(valign);
            for (int i = 0; i < layout->count(); i++) {
                QWidget* widget = layout->itemAt(i)->widget();
                if (!widget) {
                    continue;
                }
                layout->setAlignment(widget, qtAlign);
            }
            break;
        }
        // TODO: border
        default: {
            break;
        }
    }
}

QSize _Internal_QContainer::sizeHint() const {
    if (hasPreferredSize()) {
        return getPreferredSize();
    } else {
        return QWidget::sizeHint();
    }
}
