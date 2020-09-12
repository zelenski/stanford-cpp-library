/*
 * File: gobjects.h
 * ----------------
 * This file exports a hierarchy of graphical shapes based on
 * the model developed for the ACM Java Graphics.
 * <include src="pictures/ClassHierarchies/GObjectHierarchy-h.html">
 *
 * @author Marty Stepp
 * @version 2019/05/05
 * - added predictable GLine point ordering
 * @version 2019/04/23
 * - bug fix for loading GImage from file on Windows related to istream change
 * @version 2019/03/07
 * - added support for loading a GImage directly from istream (htiek)
 * @version 2018/09/14
 * - added opacity support
 * - added GCanvas-to-GImage conversion support
 * @version 2018/09/08
 * - added doc comments for new documentation generation
 * @version 2018/08/23
 * - renamed to gobjects.h to replace Java version
 * @version 2018/06/30
 * - initial version
 */


#ifndef _gobjects_h
#define _gobjects_h

#include <initializer_list>
#include <iostream>
#include <QFont>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include "gtypes.h"
#include "vector.h"

class GCanvas;
class GCompound;
class GDiffImage;

/**
 * This class is the common superclass of all graphical objects that can
 * be displayed on a graphical window.  The class <code>GObject</code>
 * itself is an <b><i>abstract class</i></b>, which means that you are not
 * allowed to construct a <code>GObject</code> directly but must instead
 * construct one of the concrete subclasses.
 * <include src="pictures/ClassHierarchies/GObjectHierarchy.html">
 *
 * Most methods used for graphics take a pointer to a <code>GObject</code>
 * rather than the <code>GObject</code> itself.  Applications that use
 * <code>GObject</code> pointers therefore use the arrow operator
 * (<code>-&gt;</code>) to apply methods to the object pointer.
 * For examples illustrating the use of the <code>GObject</code> class, see
 * the descriptions of the individual subclasses.
 */
class GObject {
public:
    /**
     * Styles that can be used for the outline around various shapes.
     * Call setLineStyle on a GObject and pass one of these values.
     */
    enum LineStyle {
        LINE_NONE,
        LINE_SOLID,
        LINE_DASH,
        LINE_DOT,
        LINE_DASH_DOT,
        LINE_DASH_DOT_DOT
    };

    /**
     * Frees the storage for the object.
     */
    virtual ~GObject();

    /**
     * Returns <code>true</code> if the specified point is inside the object.
     */
    virtual bool contains(double x, double y) const;

    /**
     * Returns <code>true</code> if the specified point is inside the object.
     */
    virtual bool contains(const GPoint& pt) const;

    /**
     * Draws this object onto the given surface.
     * Each GObject subclass must override this method.
     * @private
     */
    virtual void draw(QPainter* painter) = 0;

    /**
     * Returns the x/y coordinates of the bottom/right corner of the object.
     */
    virtual GPoint getBottomRightLocation() const;

    /**
     * Returns the <i>y</i>-coordinate of the bottom of the object.
     * Equivalent to the top y-coordinate plus the object's height.
     */
    virtual double getBottomY() const;

    /**
     * Returns the bounding box of this object, which is defined to be the
     * smallest rectangle that covers everything drawn by the figure.  The
     * coordinates of this rectangle do not necessarily match the location
     * returned by <code>getLocation</code>.  Given a <code>GText</code>
     * object, for example, <code>getLocation</code> returns the coordinates
     * of the point on the baseline at which the string begins; the
     * <code>getBounds</code> method, by contrast, returns a rectangle that
     * covers the entire window area occupied by the string.
     */
    virtual GRectangle getBounds() const;

    /**
     * Returns the x/y-coordinates of the center of the object.
     * Equivalent to the top/left plus half the object's size.
     */
    virtual GPoint getCenterLocation() const;

    /**
     * Returns the <i>x</i>-coordinate of the center of the object.
     * Equivalent to the top/left plus half the object's width.
     */
    virtual double getCenterX() const;

    /**
     * Returns the <i>y</i>-coordinate of the center of the object.
     * Equivalent to the top/left plus half the object's height.
     */
    virtual double getCenterY() const;

    /**
     * Returns the color used to display this object.  This color is
     * always returned as a string in the form <code>"#rrggbb"</code>,
     * where <code>rr</code>, <code>gg</code>, and <code>bb</code> are
     * the red, green, and blue components of the color, expressed as
     * two-digit hexadecimal values.
     */
    virtual std::string getColor() const;

    /**
     * Returns the color used to display the filled region of this object.
     * If none has been set, returns the empty string.
     */
    virtual std::string getFillColor() const;

    /**
     * Returns the height of this object, which is the same as the height
     * of its bounding box.
     */
    virtual double getHeight() const;

    /**
     * Returns the object's style such as solid or dashed.
     */
    virtual LineStyle getLineStyle() const;

    /**
     * Returns the width of the line used to draw this object.
     * @return default 1
     */
    virtual double getLineWidth() const;

    /**
     * Returns the location of the top-left corner of object.
     */
    virtual GPoint getLocation() const;

    /**
     * Returns how opaque (non-transparent) this object will appear from 0.0
     * (completely transparent) to 1.0 (completely opaque, default).
     */
    virtual double getOpacity() const;

    /**
     * Returns a pointer to the <code>GCompound</code> that contains this
     * object.  Every <code>GWindow</code> is initialized to contain a single
     * <code>GCompound</code> that is aligned with the window.  Adding
     * objects to the window adds them to that <code>GCompound</code>,
     * which means that every object you add to the window has a parent.
     * Calling <code>getParent</code> on the top-level <code>GCompound</code>
     * returns <code>nullptr</code>.
     */
    virtual GCompound* getParent() const;

    /**
     * Returns the <i>x</i>-coordinate of the right side of the object.
     * Equivalent to the left x-coordinate plus the object's width.
     */
    virtual double getRightX() const;

    /**
     * Returns the size of the object as a <code>GDimension</code>.
     */
    virtual GDimension getSize() const;

    /**
     * Returns the type of the object as a string, such as
     * <code>"GOval"</code> or <code>"GRect"</code>.
     * Each GObject subtype must override this method.
     */
    virtual std::string getType() const = 0;

    /**
     * Returns the width of this object, which is equal to the width of
     * the bounding box.
     */
    virtual double getWidth() const;

    /**
     * Returns the leftmost <i>x</i>-coordinate of the object.
     */
    virtual double getX() const;

    /**
     * Returns the topmost <i>y</i>-coordinate of the object.
     */
    virtual double getY() const;

    /**
     * Returns whether we should globally anti-alias graphical objects.
     * On by default.
     */
    static bool isAntiAliasing();

    /**
     * Returns <code>true</code> if the object is filled with color.
     */
    virtual bool isFilled() const;

    /**
     * Returns <code>true</code> if this object has been transformed
     * by calling methods such as rotate() or scale() on it.
     * Certain operations (such as setSize) cannot be performed after
     * a graphical object has been transformed.
     */
    virtual bool isTransformed() const;

    /**
     * Returns <code>true</code> if this object is visible on screen.
     */
    virtual bool isVisible() const;

    /**
     * Moves the object on the screen using the displacements
     * <code>dx</code> and <code>dy</code>.
     */
    virtual void move(double dx, double dy);

    /**
     * Instructs the object to redraw itself on screen.
     */
    virtual void repaint();

    /**
     * Undoes any previous scale/rotate transformations on this object.
     */
    virtual void resetTransform();

    /**
     * Transforms the object by rotating it <code>theta</code> degrees
     * counterclockwise around its origin.
     * After calling this method on a graphical object, <code>isTransformed</code>
     * will return <code>true</code> for that object unless you subsequently
     * call <code>resetTransform</code> on it.
     */
    virtual void rotate(double theta);

    /**
     * Scales the object by the specified scale factor.  This form
     * scales the object by <code>sf</code> in both dimensions, so that
     * invoking <code>gobj->scale(2);</code> doubles the size of the object.
     * After calling this method on a graphical object, <code>isTransformed</code>
     * will return <code>true</code> for that object unless you subsequently
     * call <code>resetTransform</code> on it.
     */
    virtual void scale(double sf);

    /**
     * Scales the object by the specified scale factors.  For example,
     * <code>gobj->scale(2, 2);</code> doubles the size of the object.
     * This form applies independent scale factors to the <i>x</i> and <i>y</i>
     * dimensions.
     * After calling this method on a graphical object, <code>isTransformed</code>
     * will return <code>true</code> for that object unless you subsequently
     * call <code>resetTransform</code> on it.
     */
    virtual void scale(double sx, double sy);

    /**
     * Moves this object one step toward the back in the <i>z</i> dimension.
     * If it was already at the back of the stack, nothing happens.
     */
    void sendBackward();

    /**
     * Moves this object one step toward the front in the <i>z</i> dimension.
     * If it was already at the front of the stack, nothing happens.
     */
    void sendForward();

    /**
     * Moves this object to the back of the display in the <i>z</i> dimension.
     * By moving it to the back, this object will appear to be behind the other
     * graphical objects on the display and may be obscured by other objects
     * in front.
     */
    void sendToBack();

    /**
     * Moves this object to the front of the display in the <i>z</i> dimension.
     * By moving it to the front, this object will appear to be on top of the
     * other graphical objects on the display and may hide any objects that
     * are further back.
     */
    void sendToFront();

    /**
     * Globally turns on/off the anti-aliasing feature that smooths out the
     * edges of onscreen shapes.  On by default.
     * Does not repaint any onscreen objects when called; you must do this yourself.
     */
    static void setAntiAliasing(bool value);

    /**
     * Changes the bounds of this object to the specified values.
     */
    virtual void setBounds(double x, double y, double width, double height);

    /**
     * Changes the bounds of this object to the specified rectangle.
     */
    virtual void setBounds(const GRectangle& size);

    /**
     * Sets the location of the bottom y-coordinate of this object.
     */
    virtual void setBottomY(double y);

    /**
     * Sets the location of the rightmost x-coordinate of this object.
     */
    virtual void setRightX(double x);

    /**
     * Sets the location of the bottom/right of this object.
     */
    virtual void setBottomRightLocation(double x, double y);

    /**
     * Sets the location of the bottom/right of this object.
     */
    virtual void setBottomRightLocation(const GPoint& pt);

    /**
     * Sets the x-coordinate of the center of this object.
     */
    virtual void setCenterX(double x);

    /**
     * Sets the y-coordinate of the center of this object.
     */
    virtual void setCenterY(double y);

    /**
     * Sets the location of the center of this object.
     */
    virtual void setCenterLocation(double x, double y);

    /**
     * Sets the location of the center of this object.
     */
    virtual void setCenterLocation(const GPoint& pt);

    /**
     * Sets the color used to display this object.
     * See gcolor.h for more detail about how to specify colors.
     *
     * Equivalent to setForeground.
     *
     * @param r redness from 0-255
     * @param g greenness from 0-255
     * @param b blueness from 0-255
     */
    virtual void setColor(int r, int g, int b);

    /**
     * Sets the color used to display this object.
     * See gcolor.h for more detail about how to specify colors.
     *
     * Equivalent to setForeground.
     *
     * @param rgb an RGB integer value such as 0x7700ff
     */
    virtual void setColor(int rgb);

    /**
     * Sets the color used to display this object.
     * See gcolor.h for more detail about how to specify colors.
     *
     * Equivalent to setForeground.
     *
     * @param color color string such as "#7700ff" or "purple"
     */
    virtual void setColor(const std::string& color);

    /**
     * Sets the color used to display the filled region of this object, if any.
     * As a side effect, sets this object to be filled (setFilled(true)).
     * See gcolor.h for more detail about how to specify colors.
     * If an empty string is passed, sets filled to false.
     *
     * @param r redness from 0-255
     * @param g greenness from 0-255
     * @param b blueness from 0-255
     */
    virtual void setFillColor(int r, int g, int b);

    /**
     * Sets the color used to display the filled region of this object, if any.
     * As a side effect, sets this object to be filled (setFilled(true)).
     * See gcolor.h for more detail about how to specify colors.
     *
     * @param rgb an RGB integer value such as 0x7700ff
     */
    virtual void setFillColor(int rgb);

    /**
     * Sets the color used to display the filled region of this object, if any.
     * As a side effect, sets this object to be filled (setFilled(true)).
     * See gcolor.h for more detail about how to specify colors.
     * If an empty string is passed, sets filled to false.
     *
     * @param color color string such as "#7700ff" or "purple"
     */
    virtual void setFillColor(const std::string& color);

    /**
     * Sets the fill status for the object, where <code>false</code> is
     * outlined and <code>true</code> is filled.
     */
    virtual void setFilled(bool flag);

    /**
     * Changes the font used to display the object as specified by
     * the given Qt font.
     * See gfont.h for more detail about how to specify fonts.
     */
    virtual void setFont(const QFont& font);

    /**
     * Changes the font used to display the object as specified by
     * the string <code>font</code>, which has the following format:
     *
     * <pre>
     * "family-style-size"
     * </pre>
     *
     * where both <code>style</code> and <code>size</code> are optional.
     * If any of these elements are missing or specified as an asterisk,
     * the existing value is retained.
     * See gfont.h for more detail about how to specify fonts.
     */
    virtual void setFont(const std::string& font);

    /**
     * Sets the color used to display this object.
     * See gcolor.h for more detail about how to specify colors.
     *
     * Equivalent to setColor.
     *
     * @param r redness from 0-255
     * @param g greenness from 0-255
     * @param b blueness from 0-255
     */
    virtual void setForeground(int r, int g, int b);

    /**
     * Sets the color used to display this object.
     * See gcolor.h for more detail about how to specify colors.
     *
     * Equivalent to setColor.
     *
     * @param rgb an RGB integer value such as 0x7700ff
     */
    virtual void setForeground(int rgb);

    /**
     * Sets the color used to display this object.
     * See gcolor.h for more detail about how to specify colors.
     *
     * Equivalent to setColor.
     *
     * @param color color string such as "#7700ff" or "purple"
     */
    virtual void setForeground(const std::string& color);

    /**
     * Changes the height of this object to the specified height
     * without changing its width.
     */
    virtual void setHeight(double height);

    /**
     * Sets the object's style such as solid (GObject::LINE_SOLID) or dashed
     * (GObject::LINE_DASH).
     */
    virtual void setLineStyle(LineStyle lineStyle);

    /**
     * Sets the width of the line used to draw this object.
     * The default line width is 1.
     */
    virtual void setLineWidth(double lineWidth);

    /**
     * Sets the location of the top-left corner of this object to the
     * specified coordinates.
     */
    virtual void setLocation(double x, double y);

    /**
     * Sets the location of the top-left corner of this object to the
     * specified point.
     */
    virtual void setLocation(const GPoint& pt);

    /**
     * Sets how opaque (non-transparent) this object will appear from 0.0
     * (completely transparent) to 1.0 (completely opaque, default).
     * @throw ErrorException if opacity is out of range [0.0, 1.0]
     */
    virtual void setOpacity(double opacity);

    /**
     * Changes the size of this object to the specified width and height.
     */
    virtual void setSize(double width, double height);

    /**
     * Changes the size of this object to the specified width and height.
     */
    virtual void setSize(const GDimension& size);

    /**
     * Sets whether this object is visible.
     * Graphical objects are initially visible when created.
     */
    virtual void setVisible(bool flag);

    /**
     * Changes the width of this object to the specified width
     * without changing its height.
     */
    virtual void setWidth(double width);

    /**
     * Sets the x location of the left side of this object.
     */
    virtual void setX(double x);

    /**
     * Sets the y location of the top of this object.
     */
    virtual void setY(double y);

    /**
     * Returns a printable representation of the object.
     */
    virtual std::string toString() const;

// Private section
private:
    // forbid assignment between objects
    const GObject& operator =(const GObject&) {
        return *this;
    }

    // forbid copy construction
    GObject(const GObject&) {
        // empty
    }

    // whether to anti-alias graphical objects; default true
    static bool _sAntiAliasing;

    /* Instance variables */
protected:
    double _x;                       // the x coordinate of the origin
    double _y;                       // the y coordinate of the origin
    double _width;                   // the width of the bounding rectangle
    double _height;                  // the height of the bounding rectangle
    double _lineWidth;               // the width of the line in pixels
    double _opacity;                 // 0.0 (transparent) - 1.0 (opaque, default)
    LineStyle _lineStyle;            // line style such as solid or dashed
    std::string _color;              // the color of the object
    int _colorInt;
    std::string _fillColor;          // color used to fill the object
    int _fillColorInt;
    std::string _font;               // the font string of the label
    bool _fillFlag;                  // indicates whether the object is filled
    bool _visible;                   // indicates if object is visible
    bool _transformed;               // indicates if object is transformed
    GCompound* _parent;             // pointer to the parent
    QPen _pen;                       // for outlines
    QBrush _brush;                   // for filling
    QTransform _transform;           // for transformations (rotate, scale)

protected:
    /**
     * Constructs a graphical object at the given location.
     * @private
     */
    GObject(double x = 0, double y = 0, double width = 0, double height = 0);

    /**
     * Initializes the Qt brush and pen to draw this object with
     * the given painter.
     * @private
     */
    virtual void initializeBrushAndPen(QPainter* painter = nullptr);

    /**
     * Converts our line style enums into Qt pen styles for drawing.
     * @private
     */
    static Qt::PenStyle toQtPenStyle(LineStyle lineStyle);

    /**
     * Returns a string containing any extra unique information about this
     * type of graphical object.
     */
    virtual std::string toStringExtra() const;

    friend class GArc;
    friend class GCompound;
    friend class GImage;
    friend class GInteractor;
    friend class GLine;
    friend class GOval;
    friend class GPolygon;
    friend class GRect;
    friend class GRoundRect;
    friend class GText;
};

/**
 * This graphical object subclass represents an elliptical arc.  The
 * arc is specified by the following parameters:
 *
 * <p><ul>
 *   <li>The coordinates of the bounding rectangle (<code>x</code>,
 *       <code>y</code>, <code>width</code>, <code>height</code>)
 *   <li>The angle at which the arc starts (<code>start</code>)
 *   <li>The number of degrees that the arc covers (<code>sweep</code>)
 * </ul>
 *
 * <p>All angles in a <code>GArc</code> description are measured in
 * degrees moving counterclockwise from the +<i>x</i> axis.  Negative
 * values for either <code>start</code> or <code>sweep</code> indicate
 * motion in a clockwise direction.
 * <include src="pictures/GObjectDiagrams/GArcGeometry.html">
 */
class GArc : public GObject {
public:
    /**
     * Creates a new <code>GArc</code> object consisting of an elliptical arc.
     * This form creates a <code>GArc</code> whose origin is the point (0, 0).
     */
    GArc(double width = 0, double height = 0, double start = 0, double sweep = 0);

    /**
     * Creates a new <code>GArc</code> object consisting of an elliptical arc.
     * This form creates a <code>GArc</code> whose origin is the point
     * (<code>x</code>, <code>y</code>).
     */
    GArc(double x, double y, double width, double height, double start, double sweep);

    /* @inherit */
    bool contains(double x, double y) const override;

    /**
     * Draws this arc on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    GRectangle getBounds() const override;

    /**
     * Returns the point at which the arc ends.
     */
    virtual GPoint getEndPoint() const;

    /**
     * Returns the boundaries of the rectangle used to frame the arc.
     */
    virtual GRectangle getFrameRectangle() const;

    /**
     * Returns the starting angle for this arc in degrees.
     */
    virtual double getStartAngle() const;

    /**
     * Returns the point at which the arc starts.
     */
    virtual GPoint getStartPoint() const;

    /**
     * Returns the sweep angle for this arc in degrees.
     */
    virtual double getSweepAngle() const;

    /* @inherit */
    std::string getType() const override;

    /**
     * Changes the boundaries of the rectangle used to frame the arc.
     */
    virtual void setFrameRectangle(const GRectangle& rect);

    /**
     * Changes the boundaries of the rectangle used to frame the arc.
     */
    virtual void setFrameRectangle(double x, double y, double width, double height);

    /**
     * Sets the starting angle for this arc in degrees.
     */
    virtual void setStartAngle(double start);

    /**
     * Sets the sweep angle for this arc in degrees.
     */
    virtual void setSweepAngle(double start);

    /* @inherit */
    std::string toStringExtra() const override;

private:
    virtual bool containsAngle(double theta) const;
    virtual GPoint getArcPoint(double theta) const;

    /* Instance variables */
    double _start;                   /* Starting angle of the arc       */
    double _sweep;                   /* How many degrees the arc runs   */
};

/**
 * This graphical object subclass consists of a collection of other
 * graphical objects.  Once assembled, the internal objects can be manipulated
 * as a unit.  The compound keeps track of its own position, and all items
 * within it are drawn relative to that location.
 */
class GCompound : public GObject {
public:
    /**
     * Creates a compound with no internal components.
     */
    GCompound();

    /**
     * Adds a new graphical object to the compound, if that object was not
     * already present in the compound.
     * If the object is already stored in this compound, has no effect.
     * @throw ErrorException if the object is null
     */
    virtual void add(GObject* gobj);

    /**
     * Adds a new graphical object to the compound, if that object was not
     * already present in the compound.
     * This form moves the object to the point (<code>x</code>, <code>y</code>) first.
     * If the object is already stored in this compound, has no effect.
     * @throw ErrorException if the object is null
     */
    virtual void add(GObject* gobj, double x, double y);

    /**
     * Adds a new graphical object to the compound.
     */
    virtual void add(GObject& gobj);

    /**
     * Adds a new graphical object to the compound, if that object was not
     * already present in the compound.
     * This form moves the object to the point (<code>x</code>, <code>y</code>) first.
     * If the object is already stored in this compound, has no effect.
     */
    virtual void add(GObject& gobj, double x, double y);

    /**
     * Removes all graphical objects from the compound.
     * Equivalent to removeAll.
     */
    virtual void clear();

    /**
     * Repaints the compound only if it needs to be repainted
     * (if any of its contents have changed).
     */
    virtual void conditionalRepaint();

    /**
     * Repaints the given rectangular region of the compound only if it needs
     * to be repainted (if any of its contents have changed).
     */
    virtual void conditionalRepaintRegion(int x, int y, int width, int height);

    /**
     * Repaints the given rectangular region of the compound only if it needs
     * to be repainted (if any of its contents have changed).
     */
    virtual void conditionalRepaintRegion(const GRectangle& bounds);

    /* @inherit */
   bool contains(double x, double y) const override;

    /**
     * Draws all objects stored in this compound using the given painter pen.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    GRectangle getBounds() const override;

    /**
     * Returns a pointer to the graphical object at the specified index,
     * numbering from back to front in the <i>z</i> dimension.
     * @throw ErrorException if the index is out of range
     */
    virtual GObject* getElement(int index) const;

    /**
     * Returns a pointer to the first graphical object that contains the given
     * (x, y) point, or a null pointer if no object in this compound touches it.
     */
    virtual GObject* getElementAt(double x, double y) const;

    /**
     * Returns the number of graphical objects stored in the compound.
     */
    virtual int getElementCount() const;

    /* @inherit */
    std::string getType() const override;

    /**
     * Returns the Qt widget associated with this compound, or a null pointer
     * if this compound is not associated with any widget.
     * Initially a compound is not associated with any widget until the
     * setWidget method is called.
     * Clients generally do not need to use this method.
     * @private
     */
    virtual QWidget* getWidget() const;

    /**
     * Returns whether the compound automatically repaints itself when its
     * contents change.
     */
    virtual bool isAutoRepaint() const;

    /**
     * Returns true if the compound does not contain any graphical objects.
     */
    virtual bool isEmpty() const;

    /**
     * Removes the specified object from the compound.
     * @throw ErrorException if the object is null
     */
    virtual void remove(GObject* gobj);

    /**
     * Removes the specified object from the compound.
     */
    virtual void remove(GObject& gobj);

    /**
     * Removes all graphical objects from the compound.
     * Equivalent to clear.
     */
    virtual void removeAll();

    /**
     * Instructs the compound to redraw all of its graphical objects.
     */
    void repaint() override;

    /**
     * Instructs the compound to redraw the given rectangular region within itself,
     * including any graphical objects that touch that region.
     */
    virtual void repaintRegion(int x, int y, int width, int height);

    /**
     * Instructs the compound to redraw the given rectangular region within itself,
     * including any graphical objects that touch that region.
     */
    virtual void repaintRegion(const GRectangle& bounds);

    /**
     * Sets whether the compound automatically repaints itself when its
     * contents change.
     */
    virtual void setAutoRepaint(bool autoRepaint);

    /**
     * Sets the Qt widget associated with this compound, or a null pointer
     * if this compound is not associated with any widget.
     * Initially a compound is not associated with any widget until this
     * Widget method is called.
     * Clients generally do not need to use this method.
     * @private
     */
    virtual void setWidget(QWidget* widget);

    /* @inherit */
    std::string toString() const override;

private:
    // methods to move an object in the z-ordering
    void sendBackward(GObject* gobj);
    void sendForward(GObject* gobj);
    void sendToBack(GObject* gobj);
    void sendToFront(GObject* gobj);
    virtual int findGObject(GObject* gobj) const;
    virtual void removeAt(int index);

    // instance variables
    Vector<GObject*> _contents;
    QWidget* _widget = nullptr;    // widget containing this compound
    bool _autoRepaint;   // automatically repaint on any change; default true

    friend class GObject;
};

/**
 * This graphical object subclass represents an image from a file.
 */
class GImage : public GObject {
public:
    /**
     * Constructs a new image by loading the image from the specified file.
     * By default, the upper left corner of the image appears at the origin,
     * but you can pass coordinates to move it to the point
     * (<code>x</code>, <code>y</code>).
     * @throw ErrorException if the given file is not found or cannot be loaded
     *        as a valid image file
     */
    GImage(const std::string& filename = "", double x = 0, double y = 0);

    /**
     * Constructs a new image by loading the image from the specified input stream.
     * By default, the upper left corner of the image appears at the origin,
     * but you can pass coordinates to move it to the point
     * (<code>x</code>, <code>y</code>).
     * @throw ErrorException if the given file is not found or cannot be loaded
     *        as a valid image file
     */
    GImage(std::istream& source, double x = 0, double y = 0);

    /**
     * Creates a blank GImage of the given width and height.
     * Called by GCanvas when converting to an image.
     */
    GImage(double width, double height);

    /**
     * Frees memory allocated internally by the image.
     */
    virtual ~GImage();



    /**
     * Draws this image on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /**
     * Returns the file name used to load the image,
     * as was passed to the constructor.
     */
    virtual std::string getFileName() const;

    /**
     * Returns the color of the pixel at the given x/y location as an RGB integer.
     * @throw ErrorException if x/y is out of range
     */
    virtual int getPixel(int x, int y) const;

    /* @inherit */
    std::string getType() const override;

    /**
     * Sets the pixel at the given x/y location to the given color,
     * represented as an RGB integer.
     * @throw ErrorException if x/y is out of range
     */
    virtual void setPixel(int x, int y, int rgb);

    /* @inherit */
    std::string toStringExtra() const override;

protected:
    /**
     * Creates a GImage wrapping the given Qt image.
     * Called by GCanvas when converting canvas to an image.
     */
    GImage(QImage* qimage);

    /**
     * Returns the inner Qt QImage object being wrapped.
     */
    QImage* getQImage() const;

private:
    /**
     * Reads the image's pixel contents from the given file.
     * @return true if loaded successfully and false if the load failed
     */
    bool load(const std::string& filename);

    /**
     * Reads the image's pixel contents from the given stream.
     * @return true if loaded successfully and false if the load failed
     */
    bool loadFromStream(std::istream& input);

    std::string _filename;
    QImage* _qimage;

    friend class GCanvas;
    friend class GDiffImage;
};

/**
 * This graphical object subclass represents a line segment.
 */
class GLine : public GObject {
public:
    /**
     * Constructs a line segment from its endpoints.
     * The point (<code>x0</code>,&nbsp;<code>y0</code>) defines the start of
     * the line and the point (<code>x1</code>,&nbsp;<code>y1</code>) defines
     * the end.
     */
    GLine(double x0 = 0, double y0 = 0, double x1 = 0, double y1 = 0, LineStyle lineStyle = LINE_SOLID);

    /**
     * Constructs a line segment from its endpoints.
     * The point <code>p0</code> defines the start of the line and
     * the point <code>p1</code> defines the end.
     */
    GLine(const GPoint& p0, const GPoint& p1);

    /* @inherit */
    bool contains(double x, double y) const override;

    /**
     * Draws this line on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    GRectangle getBounds() const override;

    /**
     * Returns the point at which the line ends.
     */
    virtual GPoint getEndPoint() const;

    /**
     * Returns the x-coordinate of the point at which the line ends.
     */
    virtual double getEndX() const;

    /**
     * Returns the y-coordinate of the point at which the line ends.
     */
    virtual double getEndY() const;

    /* @inherit */
    double getHeight() const override;

    /**
     * Returns the point at which the line starts.
     * Equivalent to getLocation.
     */
    virtual GPoint getStartPoint() const;

    /**
     * Returns the x-coordinate of the point at which the line starts.
     * Equivalent to getX.
     */
    virtual double getStartX() const;

    /**
     * Returns the y-coordinate of the point at which the line starts.
     * Equivalent to getY.
     */
    virtual double getStartY() const;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    double getWidth() const override;

    /**
     * Sets the end point in the line to (<code>x1</code>,&nbsp;<code>y1</code>),
     * leaving the start point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    virtual void setEndPoint(double x1, double y1);

    /**
     * Sets the end point in the line to p,
     * leaving the start point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    virtual void setEndPoint(const GPoint& p);

    /**
     * Sets this line's two end points to (x0, y0) and (x1, y1).
     * The points are rearranged into x-major, y-minor order; that is, if
     * x1 < x0 or (x1 == x0 and y1 < y0), the points are swapped.
     * This is done to improve consistency when drawing lines.
     */
    virtual void setPoints(double x0, double y0, double x1, double y1);

    /**
     * Sets this line's two end points to p0 and p1.
     * The points are rearranged into x-major, y-minor order; that is, if
     * x1 < x0 or (x1 == x0 and y1 < y0), the points are swapped.
     * This is done to improve consistency when drawing lines.
     */
    virtual void setPoints(const GPoint& p0, const GPoint& p1);

    /**
     * Sets the initial point in the line to (<code>x0</code>,&nbsp;<code>y0</code>),
     * leaving the end point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    virtual void setStartPoint(double x0, double y0);

    /**
     * Sets the initial point in the line to p,
     * leaving the end point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    virtual void setStartPoint(const GPoint& p);

    /* @inherit */
    std::string toStringExtra() const override;

protected:
    /* Instance variables */
    double _dx;   // the x displacement of the line
    double _dy;   // the y displacement of the line
};

/**
 * This graphical object subclass represents an oval inscribed in
 * a rectangular box.
 */
class GOval : public GObject {
public:
    /**
     * Constructs a new oval inscribed in the specified rectangle.
     * By default, the oval is positioned at the origin, but you can pass
     * coordinates <code>x</code> and <code>y</code>.
     */
    GOval(double x = 0, double y = 0, double width = 0, double height = 0);

    /* @inherit */
   bool contains(double x, double y) const override;

    /**
     * Draws this oval on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    std::string getType() const override;
};

/**
 * This graphical object subclass represents a polygon bounded by
 * line segments.  The <code>GPolygon</code> constructor creates an
 * empty polygon.  To complete the figure, you need to add vertices
 * to the polygon using the methods <code>addVertex</code>,
 * <code>addEdge</code>, and <code>addPolarEdge</code>.
 */
class GPolygon : public GObject {
public:
    /**
     * Constructs a new empty polygon at the origin.
     */
    GPolygon();

    /**
     * Constructs a new polygon with the given vertex coordinates.
     */
    GPolygon(std::initializer_list<double> coords);
    GPolygon(std::initializer_list<GPoint> points);

    /**
     * Adds an edge to the polygon whose components are given by the displacements
     * <code>dx</code> and <code>dy</code> from the last vertex.
     */
    virtual void addEdge(double dx, double dy);

    /**
     * Adds an edge to the polygon where the displacements from the last vertex
     * are specified as the x/y values of the given point.
     */
    virtual void addEdge(const GPoint& pt);

    /**
     * Adds multiple edges to the polygon whose components are given by the
     * displacements <code>dx</code> and <code>dy</code> from the last vertex.
     */
    virtual void addEdges(std::initializer_list<double> coords);

    /**
     * Adds multiple edges to the polygon whose components are given by the
     * displacements <code>dx</code> and <code>dy</code> from the last vertex.
     */
    virtual void addEdges(std::initializer_list<GPoint> points);

    /**
     * Adds an edge to the polygon specified in polar coordinates.
     * The length of the edge is given by <code>r</code>, and the edge extends
     * in direction <code>theta</code>, measured in degrees counterclockwise
     * from the +x axis.
     */
    virtual void addPolarEdge(double r, double theta);

    /**
     * Adds a vertex at (<code>x</code>, <code>y</code>) relative to the polygon
     * origin.
     */
    virtual void addVertex(double x, double y);

    /**
     * Adds a vertex at the given (<code>x</code>, <code>y</code>) point
     * relative to the polygon origin.
     */
    virtual void addVertex(const GPoint& pt);

    /**
     * Adds multiple edges to the polygon whose components are given by the
     * coordinates <code>dx</code> and <code>dy</code> relative to the polygon
     * origin.
     */
    virtual void addVertexes(std::initializer_list<double> coords);

    /**
     * Adds multiple edges to the polygon whose components are given by the
     * coordinates <code>dx</code> and <code>dy</code> relative to the polygon
     * origin.
     */
    virtual void addVertexes(std::initializer_list<GPoint> points);

    /**
     * Removes all vertexes from the polygon.
     */
    virtual void clear();

    /* @inherit */
    bool contains(double x, double y) const override;

    /**
     * Draws this polygon on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    GRectangle getBounds() const override;

    /* @inherit */
    double getHeight() const override;

    /* @inherit */
    std::string getType() const override;

    /**
     * Returns the vertex at the given 0-based index in this polygon.
     * @throw ErrorException if the index is out of bounds.
     */
    virtual GPoint getVertex(int i) const;

    /**
     * Returns the number of vertexes in this polygon.
     */
    virtual int getVertexCount() const;

    /**
     * Returns a vector of the points in the polygon.
     */
    virtual Vector<GPoint> getVertices() const;

    /* @inherit */
    double getWidth() const override;

    /**
     * Sets the vertex at the given 0-based index in this polygon to the
     * given coordinates.
     * @throw ErrorException if the index is out of bounds.
     */
    virtual void setVertex(int i, GPoint point);

    /* @inherit */
    std::string toStringExtra() const override;

private:
    /* Instance variables */
    QVector<QPointF> _vertices;   // the vertices of the polygon
    double _cx;                   // the most recent x coordinate
    double _cy;                   // the most recent y coordinate
};

/**
 * A GRect is a graphical object whose appearance consists of
 * a rectangular box.
 */
class GRect : public GObject {
public:
    /**
     * Constructs a rectangle with the specified width and height.  The first
     * form is positioned at the origin; the second at the coordinates
     * given by <code>x</code> and <code>y</code>.
     */
    GRect(double x = 0, double y = 0, double width = 0, double height = 0);

    /**
     * Draws this rectangle on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    std::string getType() const override;
};

/**
 * A GRoundRect represents a graphical object whose appearance consists
 * of a rectangular box with rounded corners.
 */
class GRoundRect : public GRect {
public:
    /**
     * The default diameter of corners on rounded rectangles if none is
     * supplied to the constructor.
     */
    static const double DEFAULT_CORNER;

    /**
     * Constructs a new rectangle with the specified width and height,
     * located at (0, 0). The <code>corner</code> parameter specifies the
     * diameter of the arc forming the corner.
     */
    GRoundRect(double width = 0, double height = 0, double corner = DEFAULT_CORNER);

    /**
     * Constructs a new rectangle with the specified width and height,
     * with its top/left corner at (x, y).  The <code>corner</code> parameter
     * specifies the diameter of the arc forming the corner.
     */
    GRoundRect(double x, double y, double width, double height, double corner = DEFAULT_CORNER);

    /**
     * Returns <code>true</code> if the specified point is inside the object.
     */
    bool contains(double x, double y) const override;

    /**
     * Draws this rectangle on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /**
     * Returns the diameter of the arc forming the corner of this rounded
     * rectangle.
     */
    virtual double getCorner() const;

    /* @inherit */
    std::string getType() const override;

    /**
     * Sets the diameter of the arc forming the corner of this rounded
     * rectangle.
     */
    virtual void setCorner(double corner);

    /* @inherit */
    std::string toStringExtra() const override;

protected:
    double _corner;
};

/**
 * This graphical object subclass represents a text string.
 *
 * Controlling the appearance and positioning of a <code>GText</code>
 * depends on understanding the following terms:
 *
 * <ul>
 *   <li>The <b><i>baseline</i></b> is the horizontal line on which the
 *       characters rest.
 *   <li>The <b><i>origin</i></b> is the point on the baseline at which
 *       the label begins.
 *   <li>The <b><i>height</i></b> is the distance that separate two
 *       successive lines.
 *   <li>The <b><i>ascent</i></b> is the maximum distance a character
 *       in this font extends above the baseline.
 *   <li>The <b><i>descent</i></b> is the maximum distance a character
 *       in this font extends below the baseline.
 * </ul>
 */
class GText : public GObject {
public:
    /**
     * The default font used in text labels if none is provided.
     */
    static const std::string DEFAULT_FONT;

    /**
     * Creates a <code>GText</code> object containing the specified string.
     * By default, the baseline of the first character appears at the origin;
     * the second form automatically resets the location of the
     * <code>GText</code> to the point (<code>x</code>, <code>y</code>).
     */
    GText(const std::string& str = "", double x = 0, double y = 0);

    /**
     * Draws this text label on screen using the given Qt painter.
     * @private
     */
    void draw(QPainter* painter) override;

    /* @inherit */
    GRectangle getBounds() const override;

    /**
     * Returns the current font for the label.
     */
    virtual std::string getFont() const;

    /**
     * Returns the maximum distance strings in this font extend above
     * the baseline.
     */
    virtual double getFontAscent() const;

    /**
     * Returns the maximum distance strings in this font descend below
     * the baseline.
     */
    virtual double getFontDescent() const;

    /**
     * Returns the string displayed by this object.
     * Equivalent to getLabel.
     */
    virtual std::string getLabel() const;

    /**
     * Returns the string displayed by this object.
     * Equivalent to getLabel.
     */
    virtual std::string getText() const;

    /* @inherit */
    std::string getType() const override;

    /* @inherit */
    void setFont(const QFont& font) override;

    /* @inherit */
    void setFont(const std::string& font) override;

    /**
     * Changes the string stored within the text label, so that
     * a new text string appears on the display.
     * Equivalent to setText.
     */
    virtual void setLabel(const std::string& str);

    /**
     * Changes the string stored within the text label, so that
     * a new text string appears on the display.
     * Equivalent to setText.
     */
    virtual void setText(const std::string& str);

    /* @inherit */
    std::string toStringExtra() const override;

private:
    /* Instance variables */
    std::string _text;   // the string displayed by the label
    QFont _qfont;

    // update width and height when font or text changes
    void updateSize();
};

/**
 * Prints the given graphical object to an output stream.
 */
std::ostream& operator <<(std::ostream& out, const GObject& obj);

#endif // _gobjects_h
