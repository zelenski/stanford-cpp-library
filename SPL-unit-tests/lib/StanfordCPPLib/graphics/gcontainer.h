/*
 * File: gcontainer.h
 * ------------------
 *
 * @author Marty Stepp
 * @version 2018/09/25
 * - added getLayout method
 * @version 2018/09/19
 * - added contains, regionContains methods
 * @version 2018/09/07
 * - added doc comments for new documentation generation
 * @version 2018/08/29
 * - initial version
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gcontainer_h
#define _gcontainer_h

#include <QBoxLayout>
#include <QLayout>

#define INTERNAL_INCLUDE 1
#include "ginteractor.h"
#define INTERNAL_INCLUDE 1
#include "vector.h"
#undef INTERNAL_INCLUDE

class _Internal_QContainer;   // forward declaration

/**
 * A GContainer is a logical grouping for interactors.
 * The container manages the position and size of the interactors inside it.
 * This class is very similar to the Java concept of a container, represented
 * in Swing by the JPanel class.
 *
 * A container has a layout that guides how it positions its interactors.
 * Supported layouts include border (put interactors in the N/S/W/E/Center),
 * grid (rows and columns of interactors), and flow (interactors go in a
 * single horizontal or vertical line).
 *
 * Containers also use a "box model" similar to the CSS box model with margins
 * and padding around each interactor, and spacing between neighboring widgets:
 *
 * <pre>
    container
 * +-------------------+
 * |      margin       |
 * |  +---border----+  |
 * |  |   padding   |  |
 * |  |   content   |  |
 * |  |   padding   |  |
 * |  +-------------+  |
 * |      margin       |
 * |                   |
 * |      spacing      |
 * |                   |
 * |      margin       |
 * |  +---border----+  |
 * |  |   padding   |  |
 * |  |   content   |  |
 * |  |   padding   |  |
 * |  +-------------+  |
 * |      margin       |
 * |       ...         |
 * +-------------------+
 * </pre>
 */
class GContainer : public GInteractor {
public:
    /**
     * Default margin around each interactor.
     */
    static const int MARGIN_DEFAULT;

    /**
     * Default spacing between neighboring interactors.
     */
    static const int SPACING_DEFAULT;

    /**
     * The various layout management styles that containers can use.
     */
    enum Layout {
        LAYOUT_NONE,
        LAYOUT_FLOW_HORIZONTAL,
        LAYOUT_FLOW_VERTICAL,
        LAYOUT_BORDER,
        LAYOUT_GRID
    };

    /**
     * The five regions of border layouts.
     * Not used by the other layout styles.
     */
    enum Region {
        REGION_CENTER,
        REGION_EAST,
        REGION_NORTH,
        REGION_SOUTH,
        REGION_WEST
    };

    /**
     * Creates a new container with the given layout.
     */
    GContainer(Layout layout = LAYOUT_FLOW_HORIZONTAL, QWidget* parent = nullptr);

    /**
     * Creates a new container with the given number of rows and columns.
     * Meant to be used for grid layouts.
     */
    GContainer(Layout layout, int rows, int cols, QWidget* parent = nullptr);

    /**
     * Frees memory allocated internally by the container.
     */
    virtual ~GContainer() Q_DECL_OVERRIDE;

    /**
     * Adds the given interactor to the end of the list of interactors in this container.
     * If the container uses a grid layout, adds the interactor to the next available
     * row/column.  If it uses a border layout, adds to the center region.
     * @throw ErrorException if the interactor is null
     */
    virtual void add(GInteractor* interactor);

    /**
     * Adds the given interactor to the end of the list of interactors in this container.
     * If the container uses a grid layout, adds the interactor to the next available
     * row/column.  If it uses a border layout, adds to the center region.
     */
    virtual void add(GInteractor& interactor);

    /**
     * Adds the given interactor to the given row and column in this container,
     * which is assumed to use a grid layout.
     * If the rowspan and/or colspan arguments are passed, the item will occupy
     * multiple rows or columns' worth of space.
     * If the container does not use a grid layout, equivalent to add().
     * @throw ErrorException if the interactor is null
     */
    virtual void addToGrid(GInteractor* interactor, int row, int col, int rowspan = 1, int colspan = 1);

    /**
     * Adds the given interactor to the given row and column in this container,
     * which is assumed to use a grid layout.
     * If the rowspan and/or colspan arguments are passed, the item will occupy
     * multiple rows or columns' worth of space.
     * If the container does not use a grid layout, equivalent to add().
     */
    virtual void addToGrid(GInteractor& interactor, int row, int col, int rowspan = 1, int colspan = 1);

    /**
     * Adds the given interactor to the given region in this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to add().
     * @throw ErrorException if the interactor is null
     */
    virtual void addToRegion(GInteractor* interactor, Region region);

    /**
     * Adds the given interactor to the given region in this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to add().
     * @throw ErrorException if the interactor is null
     */
    virtual void addToRegion(GInteractor* interactor, const std::string& region = "Center");

    /**
     * Adds the given interactor to the given region in this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to add().
     */
    virtual void addToRegion(GInteractor& interactor, Region region);

    /**
     * Adds the given interactor to the given region in this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to add().
     */
    virtual void addToRegion(GInteractor& interactor, const std::string& region = "Center");

    /**
     * Removes all interactors from this container.
     */
    virtual void clear();

    /**
     * Removes all interactors from the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, has no effect.
     */
    virtual void clearRegion(Region region);

    /**
     * Removes all interactors from the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, has no effect.
     */
    virtual void clearRegion(const std::string& region);

    /**
     * Returns true if the given interactor is found in this container.
     * Returns false if the interactor is null
     */
    virtual bool contains(GInteractor* interactor) const;

    /**
     * Returns true if the given interactor is found in this container.
     */
    virtual bool contains(GInteractor& interactor) const;

    /**
     * Returns all interactors that are children or descendents inside this
     * container.  This differs from getInteractors in that it returns not only
     * direct children but also grandchildren, etc. in any sub-containers.
     * If the type argument is passed, will return only interactors of that type
     * (e.g. "GCheckBox").  Otherwise all interactors will be returned.
     */
    virtual Vector<GInteractor*> getDescendents(const std::string& type = "") const;

    /**
     * Returns the child interactor at the given 0-based index in this container.
     * @throw ErrorException if the index is out of bounds
     */
    virtual GInteractor* getInteractor(int i) const;

    /**
     * Returns a vector of all children interactors in this container.
     */
    virtual const Vector<GInteractor*>& getInteractors() const;

    /**
     * Returns the number of children interactors in this container.
     */
    virtual int getInteractorCount() const;

    /**
     * Returns the child interactor at the given 0-based index within the given
     * region in this container, which is assumed to use a border layout.
     * If the container does not use a border layout, returns nullptr.
     * @throw ErrorException if the index is out of bounds
     */
    virtual GInteractor* getInteractorByRegion(int i, Region region) const;

    /**
     * Returns the child interactor at the given 0-based index within the given
     * region in this container, which is assumed to use a border layout.
     * If the container does not use a border layout, returns nullptr.
     * @throw ErrorException if the index is out of bounds
     */
    virtual GInteractor* getInteractorByRegion(int i, const std::string& region = "Center") const;

    /**
     * Returns the number of children interactors within the given region in
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual int getInteractorCountByRegion(Region region) const;

    /**
     * Returns the number of children interactors within the given region in
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual int getInteractorCountByRegion(const std::string& region = "Center") const;

    /* @inherit */
    virtual _Internal_QWidget* getInternalWidget() const Q_DECL_OVERRIDE;

    /**
     * Returns the type of layout used by this container.
     */
    virtual Layout getLayout() const;

    /**
     * Returns the margin around each widget in this container in pixels.
     */
    virtual double getMargin() const;

    /**
     * Returns the padding inside this container in pixels.
     */
    virtual double getPadding() const;

    /**
     * Returns the padding on the bottom side of this container in pixels.
     */
    virtual double getPaddingBottom() const;

    /**
     * Returns the padding on the left side of this container in pixels.
     */
    virtual double getPaddingLeft() const;

    /**
     * Returns the padding on the right side of this container in pixels.
     */
    virtual double getPaddingRight() const;

    /**
     * Returns the padding on the top side of this container in pixels.
     */
    virtual double getPaddingTop() const;

    /* @inherit */
    virtual GDimension getPreferredSize() const Q_DECL_OVERRIDE;

    /**
     * Returns the height in pixels of the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual double getRegionHeight(Region region) const;

    /**
     * Returns the height in pixels of the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual double getRegionHeight(const std::string& region) const;

    /**
     * Returns the width and height in pixels of the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual GDimension getRegionSize(Region region) const;

    /**
     * Returns the width and height in pixels of the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual GDimension getRegionSize(const std::string& region) const;

    /**
     * Returns the width in pixels of the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual double getRegionWidth(Region region) const;

    /**
     * Returns the width in pixels of the given region of this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, returns 0.
     */
    virtual double getRegionWidth(const std::string& region) const;

    /**
     * Returns the spacing between widgets in this container in pixels.
     */
    virtual double getSpacing() const;

    /* @inherit */
    virtual std::string getType() const Q_DECL_OVERRIDE;

    /* @inherit */
    virtual QWidget* getWidget() const Q_DECL_OVERRIDE;

    /**
     * Adds the given interactor to this container just before the given index.
     * @throw ErrorException if the index is out of bounds or interactor is null
     */
    virtual void insert(int index, GInteractor* interactor);

    /**
     * Adds the given interactor to this container just before the given index.
     * @throw ErrorException if the index is out of bounds
     */
    virtual void insert(int index, GInteractor& interactor);

    /**
     * Adds the given interactor to the given layout region within this container
     * just before the given index.  The container is assumed to use a border
     * layout; if it does not, equivalent to insert().
     * @throw ErrorException if the index is out of bounds or interactor is null
     */
    virtual void insertToRegion(int index, GInteractor* interactor, Region region);

    /**
     * Adds the given interactor to the given layout region within this container
     * just before the given index.  The container is assumed to use a border
     * layout; if it does not, equivalent to insert().
     * @throw ErrorException if the index is out of bounds or interactor is null
     */
    virtual void insertToRegion(int index, GInteractor* interactor, const std::string& region = "Center");

    /**
     * Adds the given interactor to the given layout region within this container
     * just before the given index.  The container is assumed to use a border
     * layout; if it does not, equivalent to insert().
     * @throw ErrorException if the index is out of bounds
     */
    virtual void insertToRegion(int index, GInteractor& interactor, Region region);

    /**
     * Adds the given interactor to the given layout region within this container
     * just before the given index.  The container is assumed to use a border
     * layout; if it does not, equivalent to insert().
     * @throw ErrorException if the index is out of bounds
     */
    virtual void insertToRegion(int index, GInteractor& interactor, const std::string& region = "Center");

    /**
     * Returns true if the container does not contain any interactors.
     */
    virtual bool isEmpty() const;

    /**
     * Returns true if the given interactor is found in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to contains().
     * @throw ErrorException if the interactor is null
     */
    virtual bool regionContains(GInteractor* interactor, Region region) const;

    /**
     * Returns true if the given interactor is found in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to contains().
     * @throw ErrorException if the interactor is null
     */
    virtual bool regionContains(GInteractor* interactor, const std::string& region) const;

    /**
     * Returns true if the given interactor is found in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to contains().
     */
    virtual bool regionContains(GInteractor& interactor, Region region) const;

    /**
     * Returns true if the given interactor is found in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to contains().
     */
    virtual bool regionContains(GInteractor& interactor, const std::string& region) const;

    /**
     * Removes the given interactor from this container.
     * Works for any layout.
     * If the given interactor is not found in this container, has no effect.
     * @throw ErrorException if the interactor is null
     */
    virtual void remove(GInteractor* interactor);

    /**
     * Removes the given interactor from this container.
     * Works for any layout.
     * If the given interactor is not found in this container, has no effect.
     */
    virtual void remove(GInteractor& interactor);

    /**
     * Removes the child interactor at the given 0-based index from this container.
     * Works for any layout.
     * @throw ErrorException if the index is out of bounds
     */
    virtual void remove(int index);

    /**
     * Removes the given interactor from the given region within this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to remove().
     * If the given interactor is not found in the given region, has no effect.
     * @throw ErrorException if the interactor is null
     */
    virtual void removeFromRegion(GInteractor* interactor, Region region);

    /**
     * Removes the given interactor from the given region within this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to remove().
     * If the given interactor is not found in the given region, has no effect.
     * @throw ErrorException if the interactor is null
     */
    virtual void removeFromRegion(GInteractor* interactor, const std::string& region);

    /**
     * Removes the given interactor from the given region within this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to remove().
     * If the given interactor is not found in the given region, has no effect.
     */
    virtual void removeFromRegion(GInteractor& interactor, Region region);

    /**
     * Removes the given interactor from the given region within this container,
     * which is assumed to use a border layout.
     * If the container does not use a border layout, equivalent to remove().
     * If the given interactor is not found in the given region, has no effect.
     */
    virtual void removeFromRegion(GInteractor& interactor, const std::string& region);

    /**
     * Removes the child interactor at the given 0-based index from the given
     * region of this container, which is assumed to use a border layout.
     * If the container does not use a border layout, has no effect.
     * If the given interactor is not found in the given region, has no effect.
     * @throw ErrorException if the index is out of bounds
     */
    virtual void removeFromRegion(int index, Region region);

    /**
     * Removes the child interactor at the given 0-based index from the given
     * region of this container, which is assumed to use a border layout.
     * If the container does not use a border layout, has no effect.
     * If the given interactor is not found in the given region, has no effect.
     * @throw ErrorException if the index is out of bounds
     */
    virtual void removeFromRegion(int index, const std::string& region);

    /**
     * Sets the horizontal and vertical alignment of interactors in this container.
     */
    virtual void setAlignment(HorizontalAlignment halign, VerticalAlignment valign);

    /**
     * Sets the horizontal alignment of interactors in this container.
     */
    virtual void setHorizontalAlignment(HorizontalAlignment halign);

    /**
     * Sets the margin in pixels around interactors in this container.
     */
    virtual void setMargin(double px);

    /**
     * Sets the padding on all 4 sides around widgets in this container.
     */
    virtual void setPadding(double px);

    /**
     * Sets the padding on all 4 sides around widgets in this container,
     * using different padding on the vertical vs horizontal sides.
     */
    virtual void setPadding(double topBottom, double leftRight);

    /**
     * Sets the padding on all 4 sides around widgets in this container,
     * using different padding on each of the 4 sides.
     */
    virtual void setPadding(double top, double right, double bottom, double left);

    /**
     * Sets the vertical alignment of interactors in this container.
     */
    virtual void setVerticalAlignment(VerticalAlignment valign);

    /**
     * Sets the horizontal alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign);

    /**
     * Sets the vertical alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionAlignment(Region region, VerticalAlignment valign);

    /**
     * Sets the horizontal and vertical alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionAlignment(Region region, HorizontalAlignment halign, VerticalAlignment valign);

    /**
     * Sets the horizontal and/or vertical alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionAlignment(const std::string& region, const std::string& align);

    /**
     * Sets the horizontal and vertical alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionAlignment(const std::string& region, const std::string& halign, const std::string& valign);

    /**
     * Sets the horizontal alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionHorizontalAlignment(Region region, HorizontalAlignment halign);

    /**
     * Sets the horizontal alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionHorizontalAlignment(const std::string& region, const std::string& halign);

    /**
     * Sets the vertical alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionVerticalAlignment(const std::string& region, const std::string& valign);

    /**
     * Sets the vertical alignment of interactors in the given region of
     * this container, which is assumed to use a border layout.
     * If the container does not use a border layout, sets the alignment of
     * the entire container instead.
     */
    virtual void setRegionVerticalAlignment(Region region, VerticalAlignment valign);

    /**
     * Sets the spacing between interactors in this container.
     */
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


/**
 * Internal class; not to be used by clients.
 * @private
 */
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
    // int _rows;
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
    virtual void addToGrid(QWidget* widget, int row, int col, int rowspan = 1, int colspan = 1);
    virtual void addToRegion(QWidget* widget, GContainer::Region region);
    virtual void clear();
    virtual void clearRegion(GContainer::Region region);
    virtual bool contains(QWidget* widget) const;
    virtual void detach() Q_DECL_OVERRIDE;
    virtual void fixAlignment(QWidget* widget, GContainer::Region region = GContainer::REGION_CENTER);
    virtual void fixMargin(QLayout* layout, bool hasStretch = false);
    virtual HorizontalAlignment getHorizontalAlignment() const;
    virtual VerticalAlignment getVerticalAlignment() const;
    virtual GContainer::Layout getLayoutType() const;
    virtual int getMargin() const;
    virtual QSize getPreferredSize() const Q_DECL_OVERRIDE;
    virtual QLayout* getQLayout() const;
    virtual int getSpacing() const;
    virtual void insert(int i, QWidget* widget);
    virtual void insertToRegion(int i, QWidget* widget, GContainer::Region region);
    virtual bool isRegionStretch(GContainer::Region region) const;
    virtual QLayout* layoutForRegion(GContainer::Region region) const;
    virtual bool regionContains(QWidget* widget, GContainer::Region region) const;
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

#endif // _gcontainer_h
