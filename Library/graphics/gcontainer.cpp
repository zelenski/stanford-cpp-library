/*
 * File: gcontainer.cpp
 * --------------------
 *
 * @author Marty Stepp
 * @version 2019/02/02
 * - destructor now stops event processing
 * @version 2018/11/27
 * - added code to set a widget visible after adding/inserting it to flow container
 *   (needed to see widgets added to container after window is showing on screen)
 * @version 2018/09/19
 * - added contains, regionContains methods
 * - added argument checking with require.h
 * @version 2018/09/05
 * - thread safety improvements
 * - added setContainer logic to child interactors
 * @version 2018/08/29
 * - initial version
 */

#include "gcontainer.h"
#include "error.h"
#include "glabel.h"
#include "glayout.h"
#include "gthread.h"
#include "require.h"
#include "strlib.h"

// margin  - around container, but outside of its background color area (like CSS)
// padding - around container, but within its background color area (like CSS)
// spacing - between neighboring widgets in container

const int GContainer::MARGIN_DEFAULT = 5;
const int GContainer::SPACING_DEFAULT = 8;

// JDZ: list of interactors seems to be used more as set than indexed/vector
// this is a custom for use within this module as stop gap

template <typename ValueType>
void removeValue(Vector<ValueType> &v, const ValueType& value) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == value) {
            v.remove(i);
            return;
        }
    }
}
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
    _iqcontainer->detach();
    _iqcontainer = nullptr;
}

void GContainer::add(GInteractor* interactor) {
    require::nonNull(interactor, "GContainer::add");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(this);
    _interactors.add(interactor);

    GThread::runOnQtGuiThread([this, widget]() {
        widget->setParent(_iqcontainer);
        _iqcontainer->add(widget);
    });
}

void GContainer::add(GInteractor& interactor) {
    add(&interactor);
}

void GContainer::addToGrid(GInteractor* interactor, int row, int col, int rowspan, int colspan) {
    require::nonNull(interactor, "GContainer::addToGrid");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    GThread::runOnQtGuiThread([this, widget, row, col, rowspan, colspan]() {
        _iqcontainer->addToGrid(widget, row, col, rowspan, colspan);
    });
}

void GContainer::addToGrid(GInteractor& interactor, int row, int col, int rowspan, int colspan) {
    addToGrid(&interactor, row, col, rowspan, colspan);
}

void GContainer::addToRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GContainer::addToRegion");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    // special case: if center, remove all other widgets in that region first
    if (region == REGION_CENTER) {
        clearRegion(region);
    }

    interactor->setContainer(this);
    _interactors.add(interactor);
    _interactorsByRegion[region].add(interactor);

    GThread::runOnQtGuiThread([this, widget, region]() {
        _iqcontainer->addToRegion(widget, region);
    });
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
    for (GInteractor* interactor : _interactors) {
        interactor->setContainer(nullptr);
    }
    _interactors.clear();
    _interactorsByRegion.clear();

    GThread::runOnQtGuiThread([this]() {
        _iqcontainer->clear();
    });
}

void GContainer::clearRegion(Region region) {
    for (GInteractor* interactor : _interactorsByRegion[region]) {
        interactor->setContainer(nullptr);
        interactor->setVisible(false);
        removeValue(_interactors, interactor);
    }
    _interactorsByRegion.remove(region);

    GThread::runOnQtGuiThread([this, region]() {
        _iqcontainer->clearRegion(region);
    });
}

void GContainer::clearRegion(const std::string& region) {
    clearRegion(stringToRegion(region));
}

bool GContainer::contains(GInteractor* interactor) const {
    if (!interactor) {
        return false;
    } else {
        return _iqcontainer->contains(interactor->getWidget());
    }
}

bool GContainer::contains(GInteractor& interactor) const {
    return contains(&interactor);
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

GContainer::Layout GContainer::getLayout() const {
    return _layout;
}

double GContainer::getMargin() const {
    return _iqcontainer->getMargin();
}

double GContainer::getPadding() const {
    QMargins m = _iqcontainer->contentsMargins();
    return (m.left() + m.top() + m.right() + m.bottom()) / 4.0;
}

double GContainer::getPaddingBottom() const {
    return _iqcontainer->contentsMargins().bottom();
}

double GContainer::getPaddingLeft() const {
    return _iqcontainer->contentsMargins().left();
}

double GContainer::getPaddingRight() const {
    return _iqcontainer->contentsMargins().right();
}

double GContainer::getPaddingTop() const {
    return _iqcontainer->contentsMargins().top();
}

GDimension GContainer::getPreferredSize() const {
    QSize qsize = _iqcontainer->getPreferredSize();
    return GDimension(qsize.width(), qsize.height());
}

double GContainer::getRegionHeight(Region region) const {
    return getRegionSize(region).height;
}

double GContainer::getRegionHeight(const std::string& region) const {
    return getRegionHeight(stringToRegion(region));
}

GDimension GContainer::getRegionSize(Region region) const {
    QLayout* layout = _iqcontainer->layoutForRegion(region);
    if (!layout) {
        return GDimension();
    } else {
        QRect geom = layout->geometry();
        return GDimension(geom.width(), geom.height());
    }
}

GDimension GContainer::getRegionSize(const std::string& region) const {
    return getRegionSize(stringToRegion(region));
}

double GContainer::getRegionWidth(Region region) const {
    return getRegionSize(region).width;
}

double GContainer::getRegionWidth(const std::string& region) const {
    return getRegionWidth(stringToRegion(region));
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
    require::nonNull(interactor, "GContainer::insert");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(this);
    _interactors.insert(index, interactor);

    GThread::runOnQtGuiThread([this, index, widget]() {
        _iqcontainer->insert(index, widget);
    });
}

void GContainer::insert(int index, GInteractor& interactor) {
    insert(index, &interactor);
}

void GContainer::insertToRegion(int index, GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GContainer::insertToRegion");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(this);
    _interactors.add(interactor);
    _interactorsByRegion[region].insert(index, interactor);

    GThread::runOnQtGuiThread([this, index, widget, region]() {
        _iqcontainer->insertToRegion(index, widget, region);
    });
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

bool GContainer::regionContains(GInteractor* interactor, GContainer::Region region) const {
    if (!interactor) {
        return false;
    } else {
        return _iqcontainer->regionContains(interactor->getWidget(), region);
    }
}

bool GContainer::regionContains(GInteractor* interactor, const std::string& region) const {
    return regionContains(interactor, stringToRegion(region));
}

bool GContainer::regionContains(GInteractor& interactor, GContainer::Region region) const {
    return regionContains(&interactor, region);
}

bool GContainer::regionContains(GInteractor& interactor, const std::string& region) const {
    return regionContains(interactor, stringToRegion(region));
}


void GContainer::remove(GInteractor* interactor) {
    require::nonNull(interactor, "GContainer::remove");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(nullptr);
    removeValue(_interactors, interactor);

    GThread::runOnQtGuiThread([this, widget]() {
        _iqcontainer->remove(widget);
    });
}

void GContainer::remove(GInteractor& interactor) {
    remove(&interactor);
}

void GContainer::remove(int index) {
    GInteractor* interactor = _interactors[index];
    interactor->setContainer(nullptr);
    _interactors.remove(index);

    GThread::runOnQtGuiThread([this, index]() {
        _iqcontainer->remove(index);
    });
}

void GContainer::removeFromRegion(GInteractor* interactor, Region region) {
    require::nonNull(interactor, "GContainer::removeFromRegion");
    QWidget* widget = interactor->getWidget();
    if (!widget) {
        return;
    }

    interactor->setContainer(nullptr);
    removeValue(_interactors, interactor);
    removeValue(_interactorsByRegion[region], interactor);

    GThread::runOnQtGuiThread([this, widget, region]() {
        _iqcontainer->removeFromRegion(widget, region);
    });
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
    GInteractor* interactor = _interactorsByRegion[region][index];
    interactor->setContainer(nullptr);
    removeValue(_interactors, interactor);
    _interactorsByRegion[region].remove(index);

    GThread::runOnQtGuiThread([this, index, region]() {
        _iqcontainer->removeFromRegion(index, region);
    });
}

void GContainer::removeFromRegion(int index, const std::string& region) {
    removeFromRegion(index, stringToRegion(region));
}

void GContainer::setAlignment(HorizontalAlignment halign, VerticalAlignment valign) {
    setHorizontalAlignment(halign);
    setVerticalAlignment(valign);
}

void GContainer::setHorizontalAlignment(HorizontalAlignment halign) {
    GThread::runOnQtGuiThread([this, halign]() {
        _iqcontainer->setHorizontalAlignment(halign);
    });
}

void GContainer::setMargin(double px) {
    GThread::runOnQtGuiThread([this, px]() {
        _iqcontainer->setMargin((int) px);
    });
}

void GContainer::setPadding(double px) {
    setPadding(px, px, px, px);
}

void GContainer::setPadding(double topBottom, double leftRight) {
    setPadding(topBottom, leftRight, topBottom, leftRight);
}

void GContainer::setPadding(double top, double right, double bottom, double left) {
    GThread::runOnQtGuiThread([this, top, right, bottom, left]() {
        _iqcontainer->setPadding((int) top, (int) right, (int) bottom, (int) left);
    });
}

void GContainer::setRegionAlignment(Region region, HorizontalAlignment halign) {
    setRegionHorizontalAlignment(region, halign);
}

void GContainer::setRegionAlignment(Region region, VerticalAlignment valign) {
    setRegionVerticalAlignment(region, valign);
}

void GContainer::setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, region, halign, valign]() {
        _iqcontainer->setRegionAlignment(region, halign, valign);
    });
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
    GThread::runOnQtGuiThread([this, region, halign]() {
        _iqcontainer->setRegionHorizontalAlignment(region, halign);
    });
}

void GContainer::setRegionHorizontalAlignment(const std::string& region, const std::string& halign) {
    setRegionHorizontalAlignment(stringToRegion(region), toHorizontalAlignment(halign));
}

void GContainer::setRegionVerticalAlignment(Region region, VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, region, valign]() {
        _iqcontainer->setRegionVerticalAlignment(region, valign);
    });
}

void GContainer::setRegionVerticalAlignment(const std::string& region, const std::string& valign) {
    setRegionVerticalAlignment(stringToRegion(region), toVerticalAlignment(valign));
}

void GContainer::setSpacing(double px) {
    GThread::runOnQtGuiThread([this, px]() {
        _iqcontainer->setSpacing((int) px);
    });
}

void GContainer::setVerticalAlignment(VerticalAlignment valign) {
    GThread::runOnQtGuiThread([this, valign]() {
        _iqcontainer->setVerticalAlignment(valign);
    });
}

GContainer::Region GContainer::stringToRegion(const std::string& regionStr) {
    std::string regionLC = toLowerCase(trim(regionStr));
    if (stringContains(regionLC, "north") || stringContains(regionLC, "top")) {
        return REGION_NORTH;
    } else if (stringContains(regionLC, "south") || stringContains(regionLC, "bottom")) {
        return REGION_SOUTH;
    } else if (stringContains(regionLC, "west") || stringContains(regionLC, "left")) {
        return REGION_WEST;
    } else if (stringContains(regionLC, "east") || stringContains(regionLC, "right")) {
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
          // _rows(0),
          _cols(0),
          _currentIndex(-1),
          _overallLayout(nullptr),
          _northLayout(nullptr),
          _southLayout(nullptr),
          _westLayout(nullptr),
          _eastLayout(nullptr),
          _centerLayout(nullptr),
          _middleLayout(nullptr) {
    require::nonNull(gcontainer, "_Internal_QContainer::constructor");
    setLayoutType(layoutType);
    if (layout()) {
        setSpacing(GContainer::SPACING_DEFAULT);
    }
}

_Internal_QContainer::_Internal_QContainer(GContainer* gcontainer, int /*rows*/, int cols, QWidget* parent)
        : QWidget(parent),
          _gcontainer(gcontainer),
          _layoutType(GContainer::LAYOUT_NONE),
          _halign(ALIGN_CENTER),
          _valign(ALIGN_MIDDLE),
          _margin(GContainer::MARGIN_DEFAULT),
          _spacing(GContainer::SPACING_DEFAULT),
          // _rows(rows),
          _cols(cols),
          _currentIndex(-1),
          _overallLayout(nullptr),
          _northLayout(nullptr),
          _southLayout(nullptr),
          _westLayout(nullptr),
          _eastLayout(nullptr),
          _centerLayout(nullptr),
          _middleLayout(nullptr) {
    setObjectName(QString::fromStdString("_Internal_QContainer_" + std::to_string(gcontainer->getID())));
    setLayoutType(GContainer::LAYOUT_GRID);
    if (layout()) {
        setMargin(GContainer::MARGIN_DEFAULT);
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
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        case GContainer::LAYOUT_FLOW_VERTICAL: {
            // add to end of the list of widgets in this region
            QBoxLayout* boxLayout = (QBoxLayout*) getQLayout();
            boxLayout->insertWidget(/* index */ boxLayout->count() - 1, widget);
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
        default: {
            error("GContainer::add: no layout has been set");
            break;
        }
    }
}

void _Internal_QContainer::addToGrid(QWidget* widget, int row, int col, int rowspan, int colspan) {
    if (_layoutType == GContainer::LAYOUT_GRID) {
        QGridLayout* gridLayout = (QGridLayout*) getQLayout();
        gridLayout->addWidget(widget, row, col, rowspan, colspan);
        _currentIndex = row * _cols + col;   // approximate
        widget->setVisible(true);
        GLayout::forceUpdate(this);
    } else {
        add(widget);
    }
}

void _Internal_QContainer::addToRegion(QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QBoxLayout* layout = (QBoxLayout*) layoutForRegion(region);
        if (region == GContainer::REGION_CENTER) {
            // center holds at most one widget
            GLayout::clearLayout(layout);

            // http://doc.qt.io/qt-5/qsizepolicy.html
            QSizePolicy sizePolicy;
            sizePolicy.setHorizontalPolicy(QSizePolicy::Ignored);
            sizePolicy.setVerticalPolicy(QSizePolicy::Ignored);
            sizePolicy.setHorizontalStretch(999);
            sizePolicy.setVerticalStretch(999);
            widget->setSizePolicy(sizePolicy);

            layout->addWidget(widget, /* stretch */ 999);
        } else {
            // add to end of the list of widgets in this region
            widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            layout->insertWidget(/* index */ layout->count() - 1, widget, /* stretch */ 0);
        }
        widget->setVisible(true);

        // set alignment of widget
        fixAlignment(widget, region);
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
        GLayout::forceUpdate(this);
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
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
        GLayout::forceUpdate(this);
    } else {
        clear();
    }
}

bool _Internal_QContainer::contains(QWidget* widget) const {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        return GLayout::contains(_northLayout, widget)
                || GLayout::contains(_southLayout, widget)
                || GLayout::contains(_westLayout, widget)
                || GLayout::contains(_eastLayout, widget)
                || GLayout::contains(_centerLayout, widget);
    } else {
        return GLayout::contains(getQLayout(), widget);
    }
}

void _Internal_QContainer::detach() {
    // TODO: nullify/delete regions
    _gcontainer = nullptr;
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

void _Internal_QContainer::fixMargin(QLayout* layout, bool hasStretch) {
    if (!hasStretch) {
        return;
    }
    int emptyCount = hasStretch ? 2 : 0;
    if (layout->count() <= emptyCount) {
        layout->setContentsMargins(0,0,0,0);
    } else {
        auto m = getMargin();
        layout->setContentsMargins(m,m,m,m);
    }
    GLayout::invalidateLayout(layout);
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

QSize _Internal_QContainer::getPreferredSize() const {
    // make sure the layout has calculated everybody's position/size
    // GLayout::forceUpdate((QWidget*) this);

    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QSize north = GLayout::getProperSize(_northLayout);
        QSize south = GLayout::getProperSize(_southLayout);
        QSize west = GLayout::getProperSize(_westLayout);
        QSize east = GLayout::getProperSize(_eastLayout);

        QSize center(0, 0);
        if (!_centerLayout->isEmpty()) {
            QWidget* centerWidget = _centerLayout->itemAt(0)->widget();
            center = GLayout::getPreferredSize(centerWidget);
        }

        int preferredWidth = center.width() + west.width() + east.width();
        int preferredHeight = center.height() + north.height() + south.height();
        return QSize(preferredWidth, preferredHeight);
    } else {
        return layout()->sizeHint();
    }
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
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
        case GContainer::LAYOUT_FLOW_HORIZONTAL:
        case GContainer::LAYOUT_FLOW_VERTICAL:
        default: {
            // index is off by 1 because of 'stretch' widgets at start/end
            QBoxLayout* boxLayout = (QBoxLayout*) getQLayout();
            boxLayout->insertWidget(/* index */ i - 1, widget);
            widget->setVisible(true);
            GLayout::forceUpdate(this);
            break;
        }
    }
}

void _Internal_QContainer::insertToRegion(int i, QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        if (region == GContainer::REGION_CENTER) {
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
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);

        GLayout::forceUpdate(this);
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

bool _Internal_QContainer::regionContains(QWidget* widget, GContainer::Region region) const {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        return GLayout::contains(layout, widget);
    } else {
        return contains(widget);
    }
}

void _Internal_QContainer::remove(QWidget* widget) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        removeFromRegion(widget, GContainer::REGION_CENTER);
        removeFromRegion(widget, GContainer::REGION_NORTH);
        removeFromRegion(widget, GContainer::REGION_SOUTH);
        removeFromRegion(widget, GContainer::REGION_WEST);
        removeFromRegion(widget, GContainer::REGION_EAST);
    } else if (layout()) {
        layout()->removeWidget(widget);
        GLayout::forceUpdate(this);
    }
}

void _Internal_QContainer::removeFromRegion(QWidget* widget, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        if (GLayout::contains(layout, widget)) {
            widget->setVisible(false);
            layout->removeWidget(widget);
            layout->update();
            fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
            GLayout::forceUpdate(this);
        }
    } else {
        remove(widget);
    }
}

void _Internal_QContainer::remove(int i) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        removeFromRegion(i, GContainer::REGION_CENTER);
    } else {
        QWidget* widget = layout()->itemAt(i)->widget();
        if (widget) {
            widget->setVisible(false);
        }
        // add +1 to the index to account for the 'stretch' at start
        layout()->removeItem(layout()->itemAt(i + 1));
        layout()->update();
        GLayout::forceUpdate(this);
    }
}

void _Internal_QContainer::removeFromRegion(int i, GContainer::Region region) {
    if (_layoutType == GContainer::LAYOUT_BORDER) {
        QLayout* layout = layoutForRegion(region);
        QWidget* widget = layout->itemAt(i)->widget();
        widget->setVisible(false);
        if (layout == _centerLayout) {
            layout->removeItem(layout->itemAt(i));
        } else {
            // add +1 to the index to account for the 'stretch' at start
            layout->removeItem(layout->itemAt(i + 1));
        }
        layout->update();
        fixMargin(layout, /* hasStretch */ region != GContainer::REGION_CENTER);
        GLayout::forceUpdate(this);
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
        _northLayout->setContentsMargins(left, top, right, bottom);
        _southLayout->setContentsMargins(left, top, right, bottom);
        _westLayout->setContentsMargins(left, top, right, bottom);
        _eastLayout->setContentsMargins(left, top, right, bottom);
        // _centerLayout->setContentsMargins(left, top, right, bottom);
        // _overallLayout->setContentsMargins(left, top, right, bottom);
        // _middleLayout->setContentsMargins(left, top, right, bottom);
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
        case GContainer::LAYOUT_BORDER: {
            // - set align of ALL regions in align map
            setRegionHorizontalAlignment(GContainer::REGION_CENTER, halign);
            setRegionHorizontalAlignment(GContainer::REGION_NORTH, halign);
            setRegionHorizontalAlignment(GContainer::REGION_SOUTH, halign);
            setRegionHorizontalAlignment(GContainer::REGION_EAST, halign);
            setRegionHorizontalAlignment(GContainer::REGION_WEST, halign);
            break;
        }
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
            _overallLayout->setObjectName(QString::fromStdString("_overallLayout_" + std::to_string(_gcontainer->getID())));
            _northLayout   = new QHBoxLayout;
            _northLayout->setObjectName(QString::fromStdString("_northLayout_" + std::to_string(_gcontainer->getID())));
            _southLayout   = new QHBoxLayout;
            _southLayout->setObjectName(QString::fromStdString("_southLayout_" + std::to_string(_gcontainer->getID())));
            _westLayout    = new QVBoxLayout;
            _westLayout->setObjectName(QString::fromStdString("_westLayout_" + std::to_string(_gcontainer->getID())));
            _eastLayout    = new QVBoxLayout;
            _eastLayout->setObjectName(QString::fromStdString("_eastLayout_" + std::to_string(_gcontainer->getID())));
            _centerLayout  = new QHBoxLayout;
            _centerLayout->setObjectName(QString::fromStdString("_centerLayout_" + std::to_string(_gcontainer->getID())));
            _middleLayout  = new QHBoxLayout;
            _middleLayout->setObjectName(QString::fromStdString("_middleLayout_" + std::to_string(_gcontainer->getID())));

            // squish margins/padding
            _overallLayout->setSpacing(0);
            _northLayout->setSpacing(getSpacing());
            _southLayout->setSpacing(getSpacing());
            _westLayout->setSpacing(getSpacing());
            _eastLayout->setSpacing(getSpacing());
            _centerLayout->setSpacing(0);
            _middleLayout->setSpacing(0);

            _overallLayout->setContentsMargins(0, 0, 0, 0);
            _northLayout->setContentsMargins(0, 0, 0, 0);
            _southLayout->setContentsMargins(0, 0, 0, 0);
            _westLayout->setContentsMargins(0, 0, 0, 0);
            _eastLayout->setContentsMargins(0, 0, 0, 0);
            _centerLayout->setContentsMargins(0, 0, 0, 0);
            _middleLayout->setContentsMargins(0, 0, 0, 0);

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

            // fake empty central widget as placeholder until center widget added
            _centerLayout->addStretch(999);

            _overallLayout->addLayout(_northLayout, /* stretch */ 0);
            _middleLayout->addLayout(_westLayout, /* stretch */ 0);
            _middleLayout->addLayout(_centerLayout, /* stretch */ 99);
            _middleLayout->addLayout(_eastLayout, /* stretch */ 0);
            _overallLayout->addLayout(_middleLayout, /* stretch */ 99);
            _overallLayout->addLayout(_southLayout, /* stretch */ 0);
            setLayout(_overallLayout);
            if (layout()) {
                layout()->setSpacing(0);
                layout()->setContentsMargins(0,0,0,0);
            }
            setRegionAlignment(GContainer::REGION_NORTH, ALIGN_CENTER, ALIGN_MIDDLE);
            setRegionAlignment(GContainer::REGION_SOUTH, ALIGN_CENTER, ALIGN_MIDDLE);
            setRegionAlignment(GContainer::REGION_WEST, ALIGN_LEFT, ALIGN_MIDDLE);
            setRegionAlignment(GContainer::REGION_EAST, ALIGN_LEFT, ALIGN_MIDDLE);
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
            auto m = getMargin();
            qlayout->setContentsMargins(m,m,m,m);
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
            auto m = getMargin();
            qlayout->setContentsMargins(m,m,m,m);
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
            fixMargin(_northLayout, /* hasStretch */ true);
            fixMargin(_southLayout, /* hasStretch */ true);
            fixMargin(_westLayout, /* hasStretch */ true);
            fixMargin(_eastLayout, /* hasStretch */ true);
            // fixMargin(_centerLayout, /* hasStretch */ false);
            // _centerLayout->setMargin(margin);
        } else {
            layout()->setContentsMargins(margin,margin,margin,margin);
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
    _halignMap[region] = halign;
    if (_layoutType != GContainer::LAYOUT_BORDER) {
        setHorizontalAlignment(halign);
        return;
    }

    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

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
}

void _Internal_QContainer::setRegionStretch(GContainer::Region region, bool stretch) {
    _regionStretchMap[region] = stretch;

    // stretch / de-stretch any widgets in that region

}

void _Internal_QContainer::setRegionVerticalAlignment(GContainer::Region region,
                                                      VerticalAlignment valign) {
    _valignMap[region] = valign;
    if (_layoutType != GContainer::LAYOUT_BORDER) {
        setVerticalAlignment(valign);
        return;
    }

    QLayout* layout = layoutForRegion(region);
    if (!layout) {
        return;
    }

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
        case GContainer::LAYOUT_BORDER: {
            // - set align of ALL regions in align map
            setRegionVerticalAlignment(GContainer::REGION_CENTER, valign);
            setRegionVerticalAlignment(GContainer::REGION_NORTH, valign);
            setRegionVerticalAlignment(GContainer::REGION_SOUTH, valign);
            setRegionVerticalAlignment(GContainer::REGION_EAST, valign);
            setRegionVerticalAlignment(GContainer::REGION_WEST, valign);
            break;
        }
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


