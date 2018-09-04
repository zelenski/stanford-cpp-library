/*
 * File: gcontainer.h
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/08/29
 * - initial version
 */

#ifndef _gcontainer_h
#define _gcontainer_h

#include <QBoxLayout>
#include <QLayout>
#include "ginteractor.h"
#include "vector.h"

class _Internal_QContainer;   // forward declaration

/*
 * ...
 */
class GContainer : public GInteractor {
public:
    static const int MARGIN_DEFAULT = 5;
    static const int SPACING_DEFAULT = 5;

    enum Layout {
        LAYOUT_NONE,
        LAYOUT_FLOW_HORIZONTAL,
        LAYOUT_FLOW_VERTICAL,
        LAYOUT_BORDER,
        LAYOUT_GRID
    };
    enum Region {
        REGION_CENTER,
        REGION_EAST,
        REGION_NORTH,
        REGION_SOUTH,
        REGION_WEST
    };

    GContainer(Layout layout = LAYOUT_FLOW_HORIZONTAL, QWidget* parent = nullptr);
    GContainer(Layout layout, int rows, int cols, QWidget* parent = nullptr);
    virtual ~GContainer();
    virtual void add(GInteractor* interactor);
    virtual void add(GInteractor& interactor);
    virtual void addToRegion(GInteractor* interactor, Region region);
    virtual void addToRegion(GInteractor* interactor, const std::string& region = "Center");
    virtual void addToRegion(GInteractor& interactor, Region region);
    virtual void addToRegion(GInteractor& interactor, const std::string& region = "Center");
    virtual void clear();
    virtual void clearRegion(Region region);
    virtual void clearRegion(const std::string& region);
    virtual Vector<GInteractor*> getDescendents(const std::string& type = "") const;
    virtual GInteractor* getInteractor(int i) const;
    virtual const Vector<GInteractor*>& getInteractors() const;
    virtual int getInteractorCount() const;
    virtual GInteractor* getInteractorByRegion(int i, Region region) const;
    virtual GInteractor* getInteractorByRegion(int i, const std::string& region = "Center") const;
    virtual int getInteractorCountByRegion(Region region) const;
    virtual int getInteractorCountByRegion(const std::string& region = "Center") const;
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual double getMargin() const;
    virtual double getPadding() const;
    virtual double getPaddingBottom() const;
    virtual double getPaddingLeft() const;
    virtual double getPaddingRight() const;
    virtual double getPaddingTop() const;
    virtual double getSpacing() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual void insert(int index, GInteractor* interactor);
    virtual void insert(int index, GInteractor& interactor);
    virtual void insertToRegion(int index, GInteractor* interactor, Region region);
    virtual void insertToRegion(int index, GInteractor* interactor, const std::string& region = "Center");
    virtual void insertToRegion(int index, GInteractor& interactor, Region region);
    virtual void insertToRegion(int index, GInteractor& interactor, const std::string& region = "Center");
    virtual bool isEmpty() const;
    virtual void remove(GInteractor* interactor);
    virtual void remove(GInteractor& interactor);
    virtual void remove(int index);
    virtual void removeFromRegion(GInteractor* interactor, Region region);
    virtual void removeFromRegion(GInteractor* interactor, const std::string& region);
    virtual void removeFromRegion(GInteractor& interactor, Region region);
    virtual void removeFromRegion(GInteractor& interactor, const std::string& region);
    virtual void removeFromRegion(int index, Region region);
    virtual void removeFromRegion(int index, const std::string& region);
    virtual void setAlignment(HorizontalAlignment halign, VerticalAlignment valign);
    virtual void setHorizontalAlignment(HorizontalAlignment halign);
    virtual void setMargin(double px);
    virtual void setPadding(double px);
    virtual void setPadding(double topBottom, double leftRight);
    virtual void setPadding(double top, double right, double bottom, double left);
    virtual void setVerticalAlignment(VerticalAlignment valign);
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign);
    virtual void setRegionAlignment(Region region, VerticalAlignment valign);
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign);
    virtual void setRegionAlignment(const std::string& region, const std::string& align);
    virtual void setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign);
    virtual void setRegionHorizontalAlignment(Region region, HorizontalAlignment halign);
    virtual void setRegionHorizontalAlignment(const std::string& region, const std::string& halign);
    virtual void setRegionVerticalAlignment(const std::string& region, const std::string& valign);
    virtual void setRegionVerticalAlignment(Region region, VerticalAlignment valign);
    virtual void setSpacing(double px);

private:
    Q_DISABLE_COPY(GContainer)

    _Internal_QContainer* _iqcontainer;
    Vector<GInteractor*> _interactors;
    Map<Region, Vector<GInteractor*>> _interactorsByRegion;
    Layout _layout;

    friend class _Internal_QContainer;
    friend class GWindow;

    static Region stringToRegion(const std::string& regionStr);
};


// Internal class; not to be used by clients.
class _Internal_QContainer : public QWidget, public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QContainer(GContainer* gcontainer, GContainer::Layout layoutType, QWidget* parent = nullptr);
    _Internal_QContainer(GContainer* gcontainer, int rows, int cols, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    GContainer* _gcontainer;
    GContainer::Layout _layoutType;
    HorizontalAlignment _halign;
    VerticalAlignment _valign;
    int _margin;
    int _spacing;
    int _rows;
    int _cols;
    int _currentIndex;

    // border layout regions for N/S/W/E/C:
    // +------------------------+
    // |         north          |
    // |------------------------|
    // |         middle         |
    // |+----------------------+|
    // || west | center | east ||
    // |+----------------------+|
    // |------------------------|
    // |         south          |
    // +------------------------+
    // sizing/stretching rules:
    // - N/S expand horizontally
    // - W/E expand vertically
    // - C takes all remaining space
    // - each widget other than Center widget appears at its preferred ("hinted") size
    QVBoxLayout* _overallLayout;
    QHBoxLayout* _northLayout;
    QHBoxLayout* _southLayout;
    QVBoxLayout* _westLayout;
    QVBoxLayout* _eastLayout;
    QHBoxLayout* _centerLayout;
    QHBoxLayout* _middleLayout;
    Map<GContainer::Region, HorizontalAlignment> _halignMap;
    Map<GContainer::Region, VerticalAlignment> _valignMap;
    Map<GContainer::Region, bool> _regionStretchMap;

    virtual void add(QWidget* widget);
    virtual void addToRegion(QWidget* widget, GContainer::Region region);
    virtual void clear();
    virtual void clearRegion(GContainer::Region region);
    virtual void fixAlignment(QWidget* widget, GContainer::Region region = GContainer::REGION_CENTER);
    virtual HorizontalAlignment getHorizontalAlignment() const;
    virtual VerticalAlignment getVerticalAlignment() const;
    virtual GContainer::Layout getLayoutType() const;
    virtual int getMargin() const;
    virtual QLayout* getQLayout() const;
    virtual int getSpacing() const;
    virtual void insert(int i, QWidget* widget);
    virtual void insertToRegion(int i, QWidget* widget, GContainer::Region region);
    virtual bool isRegionStretch(GContainer::Region region) const;
    virtual QLayout* layoutForRegion(GContainer::Region region) const;
    virtual void remove(QWidget* widget);
    virtual void removeFromRegion(QWidget* widget, GContainer::Region region);
    virtual void remove(int i);
    virtual void removeFromRegion(int i, GContainer::Region region);
    virtual void setHorizontalAlignment(HorizontalAlignment halign);
    virtual void setLayoutType(GContainer::Layout layout);
    virtual void setMargin(int margin);
    virtual void setPadding(int padding);
    virtual void setPadding(int left, int top, int right, int bottom);
    virtual void setRegionAlignment(GContainer::Region region,
                                    HorizontalAlignment halign,
                                    VerticalAlignment valign);
    virtual void setRegionHorizontalAlignment(GContainer::Region region,
                                              HorizontalAlignment halign);
    virtual void setRegionStretch(GContainer::Region region, bool stretch = true);
    virtual void setRegionVerticalAlignment(GContainer::Region region,
                                            VerticalAlignment valign);
    virtual void setSpacing(int spacing);
    virtual void setVerticalAlignment(VerticalAlignment valign);

    friend class GContainer;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gcontainer_h
