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
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;
    virtual double getMargin() const;
    virtual double getSpacing() const;
    virtual std::string getType() const Q_DECL_OVERRIDE;
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;
    virtual void remove(GInteractor* interactor);
    virtual void remove(GInteractor& interactor);
    virtual void removeFromRegion(GInteractor* interactor, Region region);
    virtual void removeFromRegion(GInteractor* interactor, const std::string& region);
    virtual void removeFromRegion(GInteractor& interactor, Region region);
    virtual void removeFromRegion(GInteractor& interactor, const std::string& region);
    virtual void setAlignment(HorizontalAlignment halign, VerticalAlignment valign);
    virtual void setHorizontalAlignment(HorizontalAlignment halign);
    virtual void setMargin(double px);
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
    _Internal_QContainer* _iqcontainer;
    Layout _layout;

    friend class _Internal_QContainer;
    friend class GWindow;

    static Region stringToRegion(const std::string& regionStr);
};

// Internal class; not to be used by clients.
class _Internal_QContainer : public QWidget,  public _Internal_QWidget {
    Q_OBJECT

public:
    _Internal_QContainer(GContainer* gcontainer, GContainer::Layout layoutType, QWidget* parent = nullptr);
    _Internal_QContainer(GContainer* gcontainer, int rows, int cols, QWidget* parent = nullptr);
    virtual QSize sizeHint() const Q_DECL_OVERRIDE;

private:
    GContainer* _gcontainer;
    GContainer::Layout _layout;
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

    virtual void add(QWidget* widget);
    virtual void addToRegion(QWidget* widget, GContainer::Region region);
    virtual void clear();
    virtual void clearRegion(GContainer::Region region);
    virtual HorizontalAlignment getHorizontalAlignment() const;
    virtual VerticalAlignment getVerticalAlignment() const;
    virtual GContainer::Layout getLayoutType() const;
    virtual int getMargin() const;
    virtual QLayout* getQLayout() const;
    virtual int getSpacing() const;
    virtual QLayout* layoutForRegion(GContainer::Region region) const;
    virtual void remove(QWidget* widget);
    virtual void removeFromRegion(QWidget* widget, GContainer::Region region);
    virtual void setHorizontalAlignment(HorizontalAlignment halign);
    virtual void setLayoutType(GContainer::Layout layout);
    virtual void setMargin(int margin);
    virtual void setRegionAlignment(GContainer::Region region,
                                    HorizontalAlignment halign,
                                    VerticalAlignment valign);
    virtual void setRegionHorizontalAlignment(GContainer::Region region,
                                              HorizontalAlignment halign);
    virtual void setRegionVerticalAlignment(GContainer::Region region,
                                            VerticalAlignment valign);
    virtual void setSpacing(int spacing);
    virtual void setVerticalAlignment(VerticalAlignment valign);

    friend class GContainer;
};

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gcontainer_h
