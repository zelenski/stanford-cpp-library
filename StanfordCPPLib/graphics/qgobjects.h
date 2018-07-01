/*
 * File: qgobjects.h
 * -----------------
 * This file exports a hierarchy of graphical shapes based on
 * the model developed for the ACM Java Graphics.
 * <include src="pictures/ClassHierarchies/QGObjectHierarchy-h.html">
 *
 * @version 2018/06/30
 * - initial version
 */

#ifndef _qgobjects_h
#define _qgobjects_h

#include <initializer_list>
#include <iostream>
#include <QPainter>
#include <QPen>
#include "gtypes.h"
#include "vector.h"

// forward declaration of class
class QGCompound;

/*
 * Class: QGObject
 * ---------------
 * This class is the common superclass of all graphical objects that can
 * be displayed on a graphical window.  The class <code>QGObject</code>
 * itself is an <b><i>abstract class</i></b>, which means that you are not
 * allowed to construct a <code>QGObject</code> directly but must instead
 * construct one of the concrete subclasses.
 * <include src="pictures/ClassHierarchies/QGObjectHierarchy.html">
 *
 * <p>Most methods used for graphics take a pointer to a <code>QGObject</code>
 * rather than the <code>QGObject</code> itself.  Applications that use
 * <code>QGObject</code> pointers therefore use the arrow operator
 * (<code>-&gt;</code>) to apply methods to the object pointer.
 * For examples illustrating the use of the <code>QGObject</code> class, see
 * the descriptions of the individual subclasses.
 */
class QGObject {
public:
    /*
     * Destructor: ~QGObject
     * --------------------
     * Frees the storage for the object.
     */
    virtual ~QGObject();

    /*
     * Method: contains
     * Usage: if (gobj->contains(pt)) ...
     *        if (gobj->contains(x, y)) ...
     * ------------------------------------
     * Returns <code>true</code> if the specified point is inside the object.
     */
    virtual bool contains(double x, double y) const;
    bool contains(const GPoint& pt) const;

    /*
     * Draws this object onto the given surface.
     */
    virtual void draw(QPainter* painter) = 0;

    /*
     * Method: getBottomY
     * Usage: double bottomY = gobj->getBottomY();
     * -------------------------------------------
     * Returns the <i>y</i>-coordinate of the bottom of the object.
     * Equivalent to the top y-coordinate plus the object's height.
     */
    double getBottomY() const;
    GPoint getBottomRightLocation() const;

    /*
     * Method: getBounds
     * Usage: GRectangle rect = gobj->getBounds();
     * -------------------------------------------
     * Returns the bounding box of this object, which is defined to be the
     * smallest rectangle that covers everything drawn by the figure.  The
     * coordinates of this rectangle do not necessarily match the location
     * returned by <code>getLocation</code>.  Given a <code>QGString</code>
     * object, for example, <code>getLocation</code> returns the coordinates
     * of the point on the baseline at which the string begins; the
     * <code>getBounds</code> method, by contrast, returns a rectangle that
     * covers the entire window area occupied by the string.
     */
    virtual GRectangle getBounds() const = 0;

    /*
     * Method: getCenterX/Y
     * Usage: double centerX = gobj->getCenterX();
     *        double centerY = gobj->getCenterY();
     *        GPoint center = gobj->getCenterLocation();
     * -------------------------------------------------
     * Returns the <i>x</i>-coordinate of the center of the object.
     * Equivalent to the top/left plus half the object's size.
     */
    GPoint getCenterLocation() const;
    double getCenterX() const;
    double getCenterY() const;

    /*
     * Method: getColor
     * Usage: string color = gobj->getColor();
     * ---------------------------------------
     * Returns the color used to display this object.  This color is
     * always returned as a string in the form <code>"#rrggbb"</code>,
     * where <code>rr</code>, <code>gg</code>, and <code>bb</code> are
     * the red, green, and blue components of the color, expressed as
     * two-digit hexadecimal values.
     */
    std::string getColor() const;

    /*
     * Method: getFillColor
     * Usage: string color = gobj->getFillColor();
     * -------------------------------------------
     * Returns the color used to display the filled region of this object.  If
     * none has been set, <code>getFillColor</code> returns the empty string.
     */
    std::string getFillColor() const;

    /*
     * Method: getHeight
     * Usage: double height = gobj->getHeight();
     * -----------------------------------------
     * Returns the height of this object, which is defined to be the height
     * of the bounding box.
     */
    double getHeight() const;

    /*
     * Method: getLocation
     * Usage: GPoint pt = gobj->getLocation();
     * ---------------------------------------
     * Returns the location of the top-left corner of object as a <code>GPoint</code>.
     */
    GPoint getLocation() const;

    /*
     * Method: getLineWidth
     * Usage: double lineWidth = gobj->getLineWidth();
     * -----------------------------------------------
     * Returns the width of the line used to draw this object.
     */
    double getLineWidth() const;

    /*
     * Method: getParent
     * Usage: QGCompound *parent = gobj->getParent();
     * ---------------------------------------------
     * Returns a pointer to the <code>QGCompound</code> that contains this
     * object.  Every <code>QGWindow</code> is initialized to contain a single
     * <code>QGCompound</code> that is aligned with the window.  Adding
     * objects to the window adds them to that <code>QGCompound</code>,
     * which means that every object you add to the window has a parent.
     * Calling <code>getParent</code> on the top-level <code>QGCompound</code>
     * returns <code>nullptr</code>.
     */
    QGCompound* getParent() const;

    /*
     * Method: getRightX
     * Usage: double rightX = gobj->getRightX();
     * -----------------------------------------
     * Returns the <i>x</i>-coordinate of the right side of the object.
     * Equivalent to the left x-coordinate plus the object's width.
     */
    double getRightX() const;

    /*
     * Method: getSize
     * Usage: GDimension size = gobj->getSize();
     * -----------------------------------------
     * Returns the size of the object as a <code>GDimension</code>.
     */
    GDimension getSize() const;

    /*
     * Method: getType
     * Usage: string type = gobj->getType();
     * -------------------------------------
     * Returns the concrete type of the object as a string, as in
     * <code>"QGOval"</code> or <code>"QGRect"</code>.
     */
    virtual std::string getType() const = 0;

    /*
     * Method: getWidth
     * Usage: double width = gobj->getWidth();
     * ---------------------------------------
     * Returns the width of this object, which is defined to be the width of
     * the bounding box.
     */
    double getWidth() const;

    /*
     * Method: getX
     * Usage: double x = gobj->getX();
     * -------------------------------
     * Returns the leftmost <i>x</i>-coordinate of the object.
     */
    double getX() const;

    /*
     * Method: getY
     * Usage: double y = gobj->getY();
     * -------------------------------
     * Returns the topmost <i>y</i>-coordinate of the object.
     */
    double getY() const;

    /*
     * Method: isFilled
     * Usage: if (gobj->isFilled()) ...
     * --------------------------------
     * Returns <code>true</code> if the object is filled.
     */
    bool isFilled() const;

    /*
     * Method: isVisible
     * Usage: if (gobj->isVisible()) ...
     * ---------------------------------
     * Returns <code>true</code> if this object is visible.
     */
    bool isVisible() const;

    /*
     * Method: move
     * Usage: gobj->move(dx, dy);
     * --------------------------
     * Moves the object on the screen using the displacements
     * <code>dx</code> and <code>dy</code>.
     */
    void move(double dx, double dy);

    /*
     * Method: rotate
     * Usage: gobj->rotate(theta);
     * ---------------------------
     * Transforms the object by rotating it <code>theta</code> degrees
     * counterclockwise around its origin.
     */
    void rotate(double theta);

    /*
     * Method: scale
     * Usage: gobj->scale(sf);
     *        gobj->scale(sx, sy);
     * ---------------------------
     * Scales the object by the specified scale factors.  Most clients will use
     * the first form, which scales the object by <code>sf</code> in both
     * dimensions, so that invoking <code>gobj->scale(2)</code> doubles the
     * size of the object.  The second form applies independent scale factors
     * to the <i>x</i> and <i>y</i> dimensions.
     */
    void scale(double sf);
    void scale(double sx, double sy);

    /*
     * Method: sendBackward
     * Usage: gobj->sendBackward();
     * ----------------------------
     * Moves this object one step toward the back in the <i>z</i> dimension.
     * If it was already at the back of the stack, nothing happens.
     */
    void sendBackward();

    /*
     * Method: sendForward
     * Usage: gobj->sendForward();
     * ---------------------------
     * Moves this object one step toward the front in the <i>z</i> dimension.
     * If it was already at the front of the stack, nothing happens.
     */
    void sendForward();

    /*
     * Method: sendToBack
     * Usage: gobj->sendToBack();
     * --------------------------
     * Moves this object to the back of the display in the <i>z</i> dimension.
     * By moving it to the back, this object will appear to be behind the other
     * graphical objects on the display and may be obscured by other objects
     * in front.
     */
    void sendToBack();

    /*
     * Method: sendToFront
     * Usage: gobj->sendToFront();
     * ---------------------------
     * Moves this object to the front of the display in the <i>z</i> dimension.
     * By moving it to the front, this object will appear to be on top of the
     * other graphical objects on the display and may hide any objects that
     * are further back.
     */
    void sendToFront();

    /*
     * Globally turns on/off the anti-aliasing feature that smooths out the
     * edges of onscreen shapes.  On by default.
     */
    static void setAntiAliasing(bool value);

    /*
     * Method: setCenter/X/Y
     * Usage: gobj->setBottomY(y);
     *        gobj->setRightX(x);
     *        gobj->setBottomRightLocation(x, y);
     *        gobj->setBottomRightLocation(pt);
     * ------------------------------------------
     * Sets the location of the bottom/right of this object.
     */
    void setBottomY(double y);
    void setRightX(double x);
    void setBottomRightLocation(double x, double y);
    void setBottomRightLocation(const GPoint& pt);

    /*
     * Method: setCenter/X/Y
     * Usage: gobj->setCenterX(x);
     *        gobj->setCenterY(y);
     *        gobj->setCenterLocation(x, y);
     *        gobj->setCenterLocation(pt);
     * -------------------------------------
     * Sets the location of the center of this object.
     */
    void setCenterX(double x);
    void setCenterY(double y);
    void setCenterLocation(double x, double y);
    void setCenterLocation(const GPoint& pt);

    /*
     * Method: setColor
     * Usage: gobj->setColor(color);
     * -----------------------------
     * Sets the color used to display this object.  The <code>color</code>
     * string is usually one of the predefined color names:
     *
     *    <code>BLACK</code>,
     *    <code>BLUE</code>,
     *    <code>CYAN</code>,
     *    <code>DARK_GRAY</code>,
     *    <code>GRAY</code>,
     *    <code>GREEN</code>,
     *    <code>LIGHT_GRAY</code>,
     *    <code>MAGENTA</code>,
     *    <code>ORANGE</code>,
     *    <code>PINK</code>,
     *    <code>RED</code>,
     *    <code>WHITE</code>, and
     *    <code>YELLOW</code>.
     *
     * The case of the individual letters in the color name is ignored, as
     * are spaces and underscores, so that the color <code>DARK_GRAY</code>
     * can be written as <code>"Dark Gray"</code>.
     *
     * <p>The color can also be specified as a string in the form
     * <code>"#rrggbb"</code> where <code>rr</code>, <code>gg</code>, and
     * <code>bb</code> are pairs of hexadecimal digits indicating the
     * red, green, and blue components of the color, respectively.
     */
    void setColor(int r, int g, int b);
    void setColor(int rgb);
    void setColor(const std::string& color);

    /*
     * Method: setFillColor
     * Usage: gobj->setFillColor(color);
     * ---------------------------------
     * Sets the color used to display the filled region of this object, if any.
     * As a side effect, sets this object to be filled (setFilled(true)).
     * If an empty string is passed, sets filled to false.
     */
    void setFillColor(int r, int g, int b);
    void setFillColor(int rgb);
    void setFillColor(const std::string& color);

    /*
     * Method: setFilled
     * Usage: gobj->setFilled(flag);
     * -----------------------------
     * Sets the fill status for the object, where <code>false</code> is
     * outlined and <code>true</code> is filled.
     */
    void setFilled(bool flag);

    /*
     * Method: setFont
     * Usage: gobj->setFont(font);
     * ---------------------------
     * Changes the font used to display the object as specified by
     * the string <code>font</code>, which has the following format:
     *
     *<pre>
     *    family-style-size
     *</pre>
     *
     * where both <code>style</code> and <code>size</code> are optional.
     * If any of these elements are missing or specified as an asterisk,
     * the existing value is retained.
     */
    virtual void setFont(const std::string& font);

    /*
     * Method: setLineWidth
     * Usage: gobj->setLineWidth(lineWidth);
     * -------------------------------------
     * Sets the width of the line used to draw this object.
     */
    void setLineWidth(double lineWidth);

    /*
     * Method: setLocation
     * Usage: gobj->setLocation(pt);
     *        gobj->setLocation(x, y);
     * -------------------------------
     * Sets the location of the top-left corner of this object to the specified point.
     */
    void setLocation(double x, double y);
    void setLocation(const GPoint& pt);

    /*
     * Method: setVisible
     * Usage: gobj->setVisible(flag);
     * ------------------------------
     * Sets whether this object is visible.
     */
    void setVisible(bool flag);

    /*
     * Method: setX/Y
     * Usage: gobj->setX(x);
     *        gobj->setY(Y);
     * ---------------------
     * Sets the x/y location of the top/left of this object.
     */
    void setX(double x);
    void setY(double y);

    /*
     * Method: toString
     * Usage: gobj->toString();
     * ------------------------
     * Returns a printable representation of the object.
     */
    virtual std::string toString() const;
    virtual std::string toStringExtra() const;

    /* Private section */
private:
    // forbid assignment between objects
    const QGObject& operator =(const QGObject&) {
        return *this;
    }

    // forbid copy construction
    QGObject(const QGObject&) {
        // empty
    }

    /* Instance variables */
protected:
    double _x;                       // The x coordinate of the origin
    double _y;                       // The y coordinate of the origin
    double _lineWidth;               // The width of the line in pixels
    std::string _color;              // The color of the object
    int _colorInt;
    std::string _fillColor;          // Color used to fill the object
    int _fillColorInt;
    std::string _font;               // The font string of the label
    bool _fillFlag;                  // Indicates whether the object is filled
    bool _visible;                   // Indicates if object is visible
    bool _transformed;               // Indicates if object is transformed
    QGCompound* _parent;             // Pointer to the parent
    QPen _pen;                       // for outlines
    QBrush _brush;                   // for filling

protected:
    QGObject();

    void initializeBrushAndPen(QPainter* painter = nullptr);

    friend class QG3DRect;
    friend class QGArc;
    friend class QGCompound;
    friend class QGImage;
    friend class QGInteractor;
    friend class QGLine;
    friend class QGOval;
    friend class QGPolygon;
    friend class QGRect;
    friend class QGRoundRect;
    friend class QGString;
};

/*
 * Class: QGRect
 * -------------
 * This class represents a graphical object whose appearance consists of
 * a rectangular box.  For example, the following code adds a filled, red
 * <nobr>200<font class=sansserif size=-1>x</font>100</nobr> rectangle
 * at the upper left corner of the graphics window:
 *
 *<pre>
 *    int main() {
 *       QGWindow gw;
 *       cout << "This program draws a red rectangle at (0, 0)." << endl;
 *       QGRect *rect = new QGRect(0, 0, 200, 100);
 *       rect->setFilled(true);
 *       rect->setColor("RED");
 *       gw.add(rect);
 *       return 0;
 *    }
 *</pre>
 */
class QGRect : public QGObject {
public:
    /*
     * Constructor: QGRect
     * Usage: QGRect *rect = new QGRect(width, height);
     *        QGRect *rect = new QGRect(x, y, width, height);
     * ----------------------------------------------------
     * Constructs a rectangle with the specified width and height.  The first
     * form is positioned at the origin; the second at the coordinates
     * given by <code>x</code> and <code>y</code>.
     */
    QGRect();
    QGRect(double width, double height);
    QGRect(double x, double y, double width, double height);

    /*
     * Destructor: ~QGRect
     * ------------------
     * Frees any resources maintained by this object.
     */
    virtual ~QGRect();

    void draw(QPainter* painter);

    /*
     * Method: isFilled
     * Usage: if (rect->isFilled()) ...
     * --------------------------------
     * Returns <code>true</code> if the rectangle is filled.
     */
    bool isFilled() const;

    /*
     * Method: setBounds
     * Usage: rect->setBounds(rect);
     *        rect->setBounds(x, y, width, height);
     * --------------------------------------------
     * Changes the bounds of this rectangle to the specified values.
     */
    void setBounds(double x, double y, double width, double height);
    void setBounds(const GRectangle& size);

    /*
     * Method: setFilled
     * Usage: rect->setFilled(flag);
     * -----------------------------
     * Sets the fill status for the rectangle, where <code>false</code> is
     * outlined and <code>true</code> is filled.
     */
    void setFilled(bool flag);

    /*
     * Method: setSize
     * Usage: rect->setSize(size);
     *        rect->setSize(width, height);
     * ------------------------------------
     * Changes the size of this rectangle to the specified width and height.
     */
    void setSize(double width, double height);
    void setSize(const GDimension& size);

    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

protected:
    /* Instance variables */
    double width;           /* The width of the rectangle              */
    double height;          /* The height of the rectangle             */

    /* Protected methods */
    virtual void createQGRect(double width, double height);
};

/*
 * Class: QGRoundRect
 * ------------------
 * This class represents a graphical object whose appearance consists
 * of a rectangular box with rounded corners.
 */
class QGRoundRect : public QGRect {
public:
    /*
     * Constructor: QGRoundRect
     * Usage: QGRoundRect *rect = new QGRoundRect(width, height);
     *        QGRoundRect *rect = new QGRoundRect(width, height, corner);
     *        QGRoundRect *rect = new QGRoundRect(x, y, width, height);
     *        QGRoundRect *rect = new QGRoundRect(x, y, width, height, corner);
     * ----------------------------------------------------------------------
     * Constructs a new rectangle with the specified width and height.  If
     * the <code>x</code> and <code>y</code> parameters are specified, they
     * are used to specify the origin.  The <code>corner</code> parameter
     * specifies the diameter of the arc forming the corner.
     */
    QGRoundRect(double width, double height);
    QGRoundRect(double width, double height, double corner);
    QGRoundRect(double x, double y, double width, double height);
    QGRoundRect(double x, double y, double width, double height, double corner);

    /*
     * Destructor: ~QGRoundRect
     * -----------------------
     * Frees any resources maintained by this object.
     */
    virtual ~QGRoundRect();

    /*
     * Method: contains
     * Usage: if (grrect->contains(pt)) ...
     *        if (grrect->contains(x, y)) ...
     * --------------------------------------
     * Returns <code>true</code> if the specified point is inside the object.
     */
    virtual bool contains(double x, double y) const;

    void draw(QPainter* painter);

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

protected:
    double corner;

    /* Protected methods */
    void createQGRoundRect(double width, double height, double corner);
};

/*
 * Class: QG3DRect
 * ---------------
 * This graphical object subclass represents a rectangular box that can
 * be raised or lowered.
 */
class QG3DRect : public QGRect {
public:
    /*
     * Constructor: QG3DRect
     * Usage: QG3DRect *rect = new QG3DRect(width, height);
     *        QG3DRect *rect = new QG3DRect(width, height, raised);
     *        QG3DRect *rect = new QG3DRect(x, y, width, height);
     *        QG3DRect *rect = new QG3DRect(x, y, width, height, raised);
     * ----------------------------------------------------------------
     * Constructs a new 3D rectangle with the specified width and height.  If
     * the <code>x</code> and <code>y</code> parameters are specified, they
     * are used to specify the origin.  The <code>raised</code> parameter
     * determines whether the rectangle should be drawn with highlights that
     * suggest that it is raised about the background.
     */
    QG3DRect(double width, double height);
    QG3DRect(double width, double height, bool raised);
    QG3DRect(double x, double y, double width, double height);
    QG3DRect(double x, double y, double width, double height, bool raised);

    /*
     * Destructor: ~QG3DRect
     * --------------------
     * Frees any resources maintained by this object.
     */
    virtual ~QG3DRect();

    void draw(QPainter* painter);

    /*
     * Method: isRaised
     * Usage: if (rect->isRaised()) ...
     * --------------------------------
     * Returns <code>true</code> if this object appears raised.
     */
    bool isRaised() const;

    /*
     * Method: setRaised
     * Usage: rect->setRaised(raised);
     * -------------------------------
     * Indicates whether this object appears raised.
     */
    void setRaised(bool raised);

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

protected:
    bool raised;

    /* Protected methods */
    void createQG3DRect(double width, double height, bool raised);
};

/*
 * Class: QGOval
 * -------------
 * This graphical object subclass represents an oval inscribed in
 * a rectangular box.  For example, the following code displays a
 * filled green oval inscribed in the graphics window:
 *
 *<pre>
 *    int main() {
 *       QGWindow gw;
 *       cout << "This program draws a green oval filling the window." << endl;
 *       QGOval *oval = new QGOval(gw.getWidth(), gw.getHeight());
 *       oval->setFilled(true);
 *       oval->setColor("GREEN");
 *       gw.add(oval);
 *       return 0;
 *    }
 *</pre>
 */
class QGOval : public QGObject {
public:
    /*
     * Constructor: QGOval
     * Usage: QGOval *oval = new QGOval(width, height);
     *        QGOval *oval = new QGOval(x, y, width, height);
     * ----------------------------------------------------
     * Constructs a new oval inscribed in the specified rectangle.  The
     * first form is positioned at the origin; the second at the coordinates
     * given by <code>x</code> and <code>y</code>.
     */
    QGOval(double width, double height);
    QGOval(double x, double y, double width, double height);

    /*
     * Destructor: ~QGOval
     * ------------------
     * Frees any resources maintained by this object.
     */
    virtual ~QGOval();

    void draw(QPainter* painter);

    /*
     * Method: setSize
     * Usage: oval->setSize(size);
     *        oval->setSize(width, height);
     * ------------------------------------
     * Changes the size of the bounding rectangle to the specified width
     * and height.
     */
    void setSize(double width, double height);
    void setSize(const GDimension& size);

    /*
     * Method: setBounds
     * Usage: oval->setBounds(rect);
     *        oval->setBounds(x, y, width, height);
     * --------------------------------------------
     * Changes the bounds of the oval to the specified values.
     */
    void setBounds(double x, double y, double width, double height);
    void setBounds(const GRectangle& size);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

protected:
    /* Instance variables */
    double width;           /* The width of the bounding rectangle     */
    double height;          /* The height of the bounding rectangle    */

    /* Protected methods */
    void createQGOval(double width, double height);
};

/*
 * Class: QGArc
 * ------------
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
 * <p>All angles in a <code>QGArc</code> description are measured in
 * degrees moving counterclockwise from the +<i>x</i> axis.  Negative
 * values for either <code>start</code> or <code>sweep</code> indicate
 * motion in a clockwise direction.
 * <include src="pictures/QGObjectDiagrams/QGArcGeometry.html">
 */
class QGArc : public QGObject {
public:
    /*
     * Constructor: QGArc
     * Usage: QGArc *arc = new QGArc(width, height, start, sweep);
     *        QGArc *arc = new QGArc(x, y, width, height, start, sweep);
     * ---------------------------------------------------------------
     * Creates a new <code>QGArc</code> object consisting of an elliptical arc.
     * The first form creates a <code>QGArc</code> whose origin is the point
     * (0,&nbsp;0); the second form positions the <code>QGArc</code> at the
     * point (<code>x</code>, <code>y</code>).
     * <include src="pictures/QGObjectDiagrams/QGArcExamples.html">
     */
    QGArc(double width, double height, double start, double sweep);
    QGArc(double x, double y, double width, double height,
          double start, double sweep);

    void draw(QPainter* painter);

    /*
     * Method: getEndPoint
     * Usage: GPoint pt = arc->getEndPoint();
     * --------------------------------------
     * Returns the point at which the arc ends.
     */
    GPoint getEndPoint() const;

    /*
     * Method: getFrameRectangle
     * Usage: GRectangle rect = arc->getFrameRectangle();
     * --------------------------------------------------
     * Returns the boundaries of the rectangle used to frame the arc.
     */
    GRectangle getFrameRectangle() const;

    /*
     * Method: getStartAngle
     * Usage: double angle = arc->getStartAngle();
     * -------------------------------------------
     * Returns the starting angle for this <code>QGArc</code> object.
     */
    double getStartAngle() const;

    /*
     * Method: getStartPoint
     * Usage: GPoint pt = arc->getStartPoint();
     * ----------------------------------------
     * Returns the point at which the arc starts.
     */
    GPoint getStartPoint() const;

    /*
     * Method: getSweepAngle
     * Usage: double angle = arc->getSweepAngle();
     * -------------------------------------------
     * Returns the sweep angle for this <code>QGArc</code> object.
     */
    double getSweepAngle() const;

    /*
     * Method: setFrameRectangle
     * Usage: arc->setFrameRectangle(rect);
     *        arc->setFrameRectangle(x, y, width, height);
     * ---------------------------------------------------
     * Changes the boundaries of the rectangle used to frame the arc.
     */
    void setFrameRectangle(const GRectangle& rect);
    void setFrameRectangle(double x, double y, double width, double height);

    /*
     * Method: setStartAngle
     * Usage: arc->setStartAngle(start);
     * ---------------------------------
     * Sets the starting angle for this <code>QGArc</code> object.
     */
    void setStartAngle(double start);

    /*
     * Method: setSweepAngle
     * Usage: arc->setSweepAngle(start);
     * ---------------------------------
     * Sets the sweep angle for this <code>QGArc</code> object.
     */
    void setSweepAngle(double start);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    bool containsAngle(double theta) const;
    void createQGArc(double width, double height, double start, double sweep);
    GPoint getArcPoint(double theta) const;

    /* Instance variables */
    double start;                   /* Starting angle of the arc       */
    double sweep;                   /* How many degrees the arc runs   */
    double frameWidth;              /* The width of the bounding box   */
    double frameHeight;             /* The height of the bounding box  */
};

/*
 * Class: QGCompound
 * -----------------
 * This graphical object subclass consists of a collection
 * of other graphical objects.  Once assembled, the internal objects
 * can be manipulated as a unit.  The <code>QGCompound</code> keeps
 * track of its own position, and all items within it are drawn
 * relative to that location.
 */
class QGCompound : public QGObject {
public:
    /*
     * Constructor: QGCompound
     * Usage: QGCompound *comp = new QGCompound();
     * -----------------------------------------
     * Creates a <code>QGCompound</code> object with no internal components.
     */
    QGCompound();

    /*
     * Method: add
     * Usage: comp->add(gobj);
     *        comp->add(gobj, x, y);
     * -----------------------------
     * Adds a new graphical object to the <code>QGCompound</code>.  The second
     * form moves the object to the point (<code>x</code>, <code>y</code>) first.
     */
    void add(QGObject* gobj);
    void add(QGObject* gobj, double x, double y);

    /*
     * Method: clear
     * Usage: comp->clear();
     * ---------------------
     * Removes all graphical objects from the <code>QGCompound</code>.
     * Equivalent to removeAll.
     */
    void clear();

    /*
     * Draws all objects stored in this compound using the given painter pen.
     */
    void draw(QPainter* painter);

    /*
     * Method: getElement
     * Usage: QGObject* gobj = comp->getElement(index);
     * -----------------------------------------------
     * Returns a pointer to the graphical object at the specified index,
     * numbering from back to front in the the <i>z</i> dimension.
     */
    QGObject* getElement(int index) const;

    /*
     * Method: getElementAt
     * Usage: QGObject* gobj = comp->getElementAt(x, y);
     * ------------------------------------------------
     * Returns a pointer to the first graphical object that contains the given
     * (x, y) point, or a null pointer if no object in this compound touches it.
     */
    QGObject* getElementAt(double x, double y) const;

    /*
     * Method: getElementCount
     * Usage: int n = comp->getElementCount();
     * ---------------------------------------
     * Returns the number of graphical objects stored in the
     * <code>QGCompound</code>.
     */
    int getElementCount() const;

    /*
     * Method: remove
     * Usage: comp->remove(gobj);
     * --------------------------
     * Removes the specified object from the <code>QGCompound</code>.
     */
    void remove(QGObject* gobj);

    /*
     * Method: removeAll
     * Usage: comp->removeAll();
     * -------------------------
     * Removes all graphical objects from the <code>QGCompound</code>.
     * Equivalent to clear.
     */
    void removeAll();

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toString() const;

private:
    void sendBackward(QGObject* gobj);
    void sendForward(QGObject* gobj);
    void sendToBack(QGObject* gobj);
    void sendToFront(QGObject* gobj);
    int findQGObject(QGObject* gobj) const;
    void removeAt(int index);

    /* Instance variables */
    Vector<QGObject*> contents;

    /* Friend declarations */
    friend class QGObject;
};

/*
 * Class: QGImage
 * --------------
 * This graphical object subclass represents an image from a file.
 * For example, the following code displays a <code>QGImage</code>
 * containing the Stanford tree at the center of the window, assuming
 * that the image file <code>StanfordTree.png</code> exists, either in
 * the current directory or an <code>images</code> subdirectory:
 *
 *<pre>
 *    int main() {
 *       QGWindow gw;
 *       cout << "This program draws the Stanford tree." << endl;
 *       QGImage *tree = new QGImage("StanfordTree.png");
 *       double x = (gw.getWidth() - tree->getWidth()) / 2;
 *       double y = (gw.getHeight() - tree->getHeight()) / 2;
 *       gw.add(tree, x, y);
 *       return 0;
 *    }
 *</pre>
 */
class QGImage : public QGObject {
public:
    /*
     * Constructor: QGImage
     * Usage: QGImage* image = new QGImage(filename);
     *        QGImage* image = new QGImage(filename, x, y);
     * --------------------------------------------------
     * Constructs a new image by loading the image from the specified
     * file, which is either in the current directory or a subdirectory named
     * <code>images</code>.  By default, the upper left corner of the image
     * appears at the origin; the second form automatically sets the location
     * to the point (<code>x</code>, <code>y</code>).
     */
    QGImage(const std::string& filename);
    QGImage(const std::string& filename, double x, double y);

    void draw(QPainter* painter);

    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    std::string filename;
    double width;
    double height;

    void createQGImage(const std::string& filename);
};

/*
 * Class: QGString
 * ---------------
 * This graphical object subclass represents a text string.  For
 * example, the following code adds a <code>QGString</code> containing
 * the string <code>"hello, world"</code> to the center of the window:
 *
 *<pre>
 *    int main() {
 *       QGWindow gw;
 *       cout << "This program draws the 'hello, world' message." << endl;
 *       QGString *label = new QGString("hello, world");
 *       label->setFont("SansSerif-18");
 *       double x = (gw.getWidth() - label->getWidth()) / 2;
 *       double y = (gw.getHeight() + label->getFontAscent()) / 2;
 *       gw.add(label, x, y);
 *       return 0;
 *    }
 *</pre>
 *
 * Controlling the appearance and positioning of a <code>QGString</code>
 * depends on understanding the following terms:
 *
 * <p><ul>
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
 * <include src="pictures/QGObjectDiagrams/QGStringGeometry.html">
 */
class QGString : public QGObject {
public:
    /*
     * Constructor: QGString
     * Usage: QGString *label = new QGString(str);
     *        QGString *label = new QGString(str, x, y);
     * ---------------------------------------------
     * Creates a <code>QGString</code> object containing the specified string.
     * By default, the baseline of the first character appears at the origin;
     * the second form automatically resets the location of the
     * <code>QGString</code> to the point (<code>x</code>, <code>y</code>).
     */
    QGString(const std::string& str = "");
    QGString(const std::string& str, double x, double y);

    void draw(QPainter* painter);

    /*
     * Method: getFont
     * Usage: string font = label->getFont();
     * --------------------------------------
     * Returns the current font for the <code>QGString</code>.
     */
    std::string getFont() const;

    /*
     * Method: getFontAscent
     * Usage: double ascent = label->getFontAscent();
     * ----------------------------------------------
     * Returns the maximum distance strings in this font extend above
     * the baseline.
     */
    double getFontAscent() const;

    /*
     * Method: getFontDescent
     * Usage: double descent = label->getFontDescent();
     * ------------------------------------------------
     * Returns the maximum distance strings in this font descend below
     * the baseline.
     */
    double getFontDescent() const;

    /*
     * Method: getLabel
     * Usage: string str = label->getLabel();
     * --------------------------------------
     * Returns the string displayed by this object.
     */
    std::string getLabel() const;
    std::string getText() const;

    /*
     * Method: setFont
     * Usage: gobj->setFont(font);
     * ---------------------------
     * Changes the font used to display the object as specified by
     * the string <code>font</code>, which has the following format:
     *
     *<pre>
     *    family-style-size
     *</pre>
     *
     * where both <code>style</code> and <code>size</code> are optional.
     * If any of these elements are missing or specified as an asterisk,
     * the existing value is retained.
     */
    /* override */
    virtual void setFont(const std::string& font);

    /*
     * Method: setLabel
     * Usage: label->setLabel(str);
     * ----------------------------
     * Changes the string stored within the <code>QGString</code> object, so that
     * a new text string appears on the display.
     */
    void setLabel(const std::string& str);
    void setText(const std::string& str);

    /* Prototypes for the virtual methods */
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    /* Instance variables */
    std::string str;                /* The string displayed by the label */
    double width;                   /* Width of the bounding box         */
    double height;                  /* Height of the bounding box        */
    double ascent;                  /* Font ascent                       */
    double descent;                 /* Font descent                      */

    void createQGString(const std::string& str);
};

/*
 * Class: QGLine
 * -------------
 * This graphical object subclass represents a line segment.  For
 * example, the following code adds lines that mark the diagonals
 * of the graphics window:
 *
 *<pre>
 *    int main() {
 *       QGWindow gw;
 *       cout << "This program draws the diagonals on the window." << endl;
 *       gw.add(new QGLine(0, 0, gw.getWidth(), gw.getHeight()));
 *       gw.add(new QGLine(0, gw.getHeight(), gw.getWidth(), 0));
 *       return 0;
 *    }
 *</pre>
 */
class QGLine : public QGObject {
public:
    /*
     * Constructor: QGLine
     * Usage: QGLine* gline = new QGLine(x0, y0, x1, y1);
     * ------------------------------------------------
     * Constructs a line segment from its endpoints.  The point
     * (<code>x0</code>,&nbsp;<code>y0</code>) defines the start of the
     * line and the point (<code>x1</code>,&nbsp;<code>y1</code>) defines
     * the end.
     */
    QGLine(double x0, double y0, double x1, double y1);

    /*
     * Constructor: QGLine
     * Usage: QGLine* gline = new QGLine(p0, p1);
     * ----------------------------------------
     * Constructs a line segment from its endpoints.  The point
     * <code>p0</code> defines the start of the line and the point
     * <code>p1</code> defines the end.
     */
    QGLine(const GPoint& p0, const GPoint& p1);

    void draw(QPainter* painter);

    /*
     * Method: getEndPoint
     * Usage: GPoint pt = line->getEndPoint();
     * ---------------------------------------
     * Returns the point at which the line ends.
     */
    GPoint getEndPoint() const;

    /*
     * Method: getStartPoint
     * Usage: GPoint pt = line->getStartPoint();
     * -----------------------------------------
     * Returns the point at which the line starts.
     */
    GPoint getStartPoint() const;

    /*
     * Method: setEndPoint
     * Usage: line->setEndPoint(x, y);
     * -------------------------------
     * Sets the end point in the line to (<code>x</code>,&nbsp;<code>y</code>),
     * leaving the start point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    void setEndPoint(double x, double y);

    /*
     * Method: setStartPoint
     * Usage: line->setStartPoint(x, y);
     * ---------------------------------
     * Sets the initial point in the line to (<code>x</code>,&nbsp;<code>y</code>),
     * leaving the end point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    void setStartPoint(double x, double y);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

protected:
    /* Instance variables */
    double dx;                   /* The x displacement of the line */
    double dy;                   /* The y displacement of the line */
};

/*
 * Class: QGPolygon
 * ----------------
 * This graphical object subclass represents a polygon bounded by
 * line segments.  The <code>QGPolygon</code> constructor creates an
 * empty polygon.  To complete the figure, you need to add vertices
 * to the polygon using the methods
 * <a href="#Method:addVertex"><code>addVertex</code></a>,
 * <a href="#Method:addEdge"><code>addEdge</code></a>,
 * and <a href="#Method:addPolarEdge"><code>addPolarEdge</code></a>.
 * As an example, the following code adds a filled red octagon to
 * the center of the window:
 *
 *<pre>
 *    int main() {
 *       QGWindow gw;
 *       cout << "This program draws a red octagon." << endl;
 *       double edge = 75;
 *       QGPolygon* stopSign = new QGPolygon();
 *       stopSign->addVertex(-edge / 2, edge / 2 + edge / sqrt(2.0));
 *       for (int i = 0; i < 8; i++) {
 *          stopSign->addPolarEdge(edge, 45 * i);
 *       }
 *       stopSign->setFilled(true);
 *       stopSign->setColor("RED");
 *       gw.add(stopSign, gw.getWidth() / 2, gw.getHeight() / 2);
 *       return 0;
 *    }
 *</pre>
 * <include src="pictures/QGObjectDiagrams/StopSign.html">
 */
class QGPolygon : public QGObject {
public:
    /*
     * Constructor: QGPolygon
     * Usage: QGPolygon* poly = new QGPolygon();
     * ---------------------------------------
     * Constructs a new empty polygon at the origin.
     */
    QGPolygon();

    /*
     * Constructor: QGPolygon
     * Usage: QGPolygon* poly = new QGPolygon({x1, y1, x2, y2, ..., xN, yN});
     * --------------------------------------------------------------------
     * Constructs a new polygon with the given vertex coordinates at the origin.
     */
    QGPolygon(std::initializer_list<double> coords);
    QGPolygon(std::initializer_list<GPoint> points);

    /*
     * Method: addEdge
     * Usage: poly->addEdge(dx, dy);
     * -----------------------------
     * Adds an edge to the polygon whose components are given by the displacements
     * <code>dx</code> and <code>dy</code> from the last vertex.
     */
    void addEdge(double dx, double dy);
    void addEdge(const GPoint& pt);

    /*
     * Method: addEdges
     * Usage: poly->addEdges({dx1, dy1, dx2, dy2, ..., dxN, dyN});
     * -----------------------------------------------------------
     * Adds multiple edges to the polygon whose components are given by the
     * displacements <code>dx</code> and <code>dy</code> from the last vertex.
     */
    void addEdges(std::initializer_list<double> coords);
    void addEdges(std::initializer_list<GPoint> points);

    /*
     * Method: addPolarEdge
     * Usage: poly->addPolarEdge(r, theta);
     * ------------------------------------
     * Adds an edge to the polygon specified in polar coordinates.  The length
     * of the edge is given by <code>r</code>, and the edge extends in
     * direction <code>theta</code>, measured in degrees counterclockwise
     * from the +x axis.
     */
    void addPolarEdge(double r, double theta);

    /*
     * Method: addVertex
     * Usage: poly->addVertex(x, y);
     * -----------------------------
     * Adds a vertex at (<code>x</code>, <code>y</code>) relative to the polygon
     * origin.
     */
    void addVertex(double x, double y);
    void addVertex(const GPoint& pt);

    /*
     * Method: addVertexes
     * Usage: poly->addVertexes({x1, y1, x2, y2, ..., xN, yN});
     * -----------------------------------------------------------
     * Adds multiple edges to the polygon whose components are given by the
     * coordinates <code>dx</code> and <code>dy</code> relative to the polygon origin.
     */
    void addVertexes(std::initializer_list<double> coords);
    void addVertexes(std::initializer_list<GPoint> points);

    void draw(QPainter* painter);

    /*
     * Method: getVertices
     * Usage: Vector<GPoint> vec = poly->getVertices();
     * ------------------------------------------------
     * Returns a vector of the points in the polygon.
     */
    Vector<GPoint> getVertices() const;

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual GRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    /* Instance variables */
    Vector<GPoint> vertices;        /* The vertices of the polygon   */
    double cx;                      /* The most recent x coordinate  */
    double cy;                      /* The most recent y coordinate  */
};

std::ostream& operator <<(std::ostream& out, const QGObject& obj);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgobjects_h
