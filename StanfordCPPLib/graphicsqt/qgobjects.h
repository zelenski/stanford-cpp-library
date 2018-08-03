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

#ifdef SPL_QT_GUI
#ifndef _qgobjects_h
#define _qgobjects_h

#include <initializer_list>
#include <iostream>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include "qgtypes.h"
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
    enum LineStyle {
        LINE_NONE,
        LINE_SOLID,
        LINE_DASH,
        LINE_DOT,
        LINE_DASH_DOT,
        LINE_DASH_DOT_DOT
    };

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
    virtual bool contains(const QGPoint& pt) const;

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
    virtual double getBottomY() const;
    virtual QGPoint getBottomRightLocation() const;

    /*
     * Method: getBounds
     * Usage: QGRectangle rect = gobj->getBounds();
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
    virtual QGRectangle getBounds() const;

    /*
     * Method: getCenterX/Y
     * Usage: double centerX = gobj->getCenterX();
     *        double centerY = gobj->getCenterY();
     *        QGPoint center = gobj->getCenterLocation();
     * -------------------------------------------------
     * Returns the <i>x</i>-coordinate of the center of the object.
     * Equivalent to the top/left plus half the object's size.
     */
    virtual QGPoint getCenterLocation() const;
    virtual double getCenterX() const;
    virtual double getCenterY() const;

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
    virtual std::string getColor() const;

    /*
     * Method: getFillColor
     * Usage: string color = gobj->getFillColor();
     * -------------------------------------------
     * Returns the color used to display the filled region of this object.  If
     * none has been set, <code>getFillColor</code> returns the empty string.
     */
    virtual std::string getFillColor() const;

    /*
     * Method: getHeight
     * Usage: double height = gobj->getHeight();
     * -----------------------------------------
     * Returns the height of this object, which is defined to be the height
     * of the bounding box.
     */
    virtual double getHeight() const;

    /*
     * Method: getLineStyle
     * Usage: QGObject::LineStyle lineStyle = gobj->getLineStyle();
     * ------------------------------------------------------------
     * Returns the object's style such as solid or dashed.
     */
    virtual LineStyle getLineStyle() const;

    /*
     * Method: getLocation
     * Usage: QGPoint pt = gobj->getLocation();
     * ---------------------------------------
     * Returns the location of the top-left corner of object as a <code>QGPoint</code>.
     */
    virtual QGPoint getLocation() const;

    /*
     * Method: getLineWidth
     * Usage: double lineWidth = gobj->getLineWidth();
     * -----------------------------------------------
     * Returns the width of the line used to draw this object.
     */
    virtual double getLineWidth() const;

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
    virtual QGCompound* getParent() const;

    /*
     * Method: getRightX
     * Usage: double rightX = gobj->getRightX();
     * -----------------------------------------
     * Returns the <i>x</i>-coordinate of the right side of the object.
     * Equivalent to the left x-coordinate plus the object's width.
     */
    virtual double getRightX() const;

    /*
     * Method: getSize
     * Usage: QQGDimension size = gobj->getSize();
     * -----------------------------------------
     * Returns the size of the object as a <code>QQGDimension</code>.
     */
    virtual QGDimension getSize() const;

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
    virtual double getWidth() const;

    /*
     * Method: getX
     * Usage: double x = gobj->getX();
     * -------------------------------
     * Returns the leftmost <i>x</i>-coordinate of the object.
     */
    virtual double getX() const;

    /*
     * Method: getY
     * Usage: double y = gobj->getY();
     * -------------------------------
     * Returns the topmost <i>y</i>-coordinate of the object.
     */
    virtual double getY() const;

    /*
     * Returns whether we should globally anti-alias graphical objects.
     * On by default.
     */
    static bool isAntiAliasing();

    /*
     * Method: isFilled
     * Usage: if (gobj->isFilled()) ...
     * --------------------------------
     * Returns <code>true</code> if the object is filled.
     */
    virtual bool isFilled() const;

    /*
     * Method: isVisible
     * Usage: if (gobj->isVisible()) ...
     * ---------------------------------
     * Returns <code>true</code> if this object is visible.
     */
    virtual bool isVisible() const;

    /*
     * Method: move
     * Usage: gobj->move(dx, dy);
     * --------------------------
     * Moves the object on the screen using the displacements
     * <code>dx</code> and <code>dy</code>.
     */
    virtual void move(double dx, double dy);

    /*
     * ...
     */
    virtual void repaint();

    /*
     * Undoes any previous scale/rotate transformations on this object.
     */
    virtual void resetTransform();

    /*
     * Method: rotate
     * Usage: gobj->rotate(theta);
     * ---------------------------
     * Transforms the object by rotating it <code>theta</code> degrees
     * counterclockwise around its origin.
     */
    virtual void rotate(double theta);

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
    virtual void scale(double sf);
    virtual void scale(double sx, double sy);

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
     * Does not repaint any onscreen objects when called; you must do this yourself.
     */
    static void setAntiAliasing(bool value);

    /*
     * Method: setBounds
     * Usage: gobj->setBounds(rect);
     *        gobj->setBounds(x, y, width, height);
     * --------------------------------------------
     * Changes the bounds of this object to the specified values.
     */
    virtual void setBounds(double x, double y, double width, double height);
    virtual void setBounds(const QGRectangle& size);

    /*
     * Method: setCenter/X/Y
     * Usage: gobj->setBottomY(y);
     *        gobj->setRightX(x);
     *        gobj->setBottomRightLocation(x, y);
     *        gobj->setBottomRightLocation(pt);
     * ------------------------------------------
     * Sets the location of the bottom/right of this object.
     */
    virtual void setBottomY(double y);
    virtual void setRightX(double x);
    virtual void setBottomRightLocation(double x, double y);
    virtual void setBottomRightLocation(const QGPoint& pt);

    /*
     * Method: setCenter/X/Y
     * Usage: gobj->setCenterX(x);
     *        gobj->setCenterY(y);
     *        gobj->setCenterLocation(x, y);
     *        gobj->setCenterLocation(pt);
     * -------------------------------------
     * Sets the location of the center of this object.
     */
    virtual void setCenterX(double x);
    virtual void setCenterY(double y);
    virtual void setCenterLocation(double x, double y);
    virtual void setCenterLocation(const QGPoint& pt);

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
    virtual void setColor(int r, int g, int b);
    virtual void setColor(int rgb);
    virtual void setColor(const std::string& color);

    /*
     * Method: setFillColor
     * Usage: gobj->setFillColor(color);
     * ---------------------------------
     * Sets the color used to display the filled region of this object, if any.
     * As a side effect, sets this object to be filled (setFilled(true)).
     * If an empty string is passed, sets filled to false.
     */
    virtual void setFillColor(int r, int g, int b);
    virtual void setFillColor(int rgb);
    virtual void setFillColor(const std::string& color);

    /*
     * Method: setFilled
     * Usage: gobj->setFilled(flag);
     * -----------------------------
     * Sets the fill status for the object, where <code>false</code> is
     * outlined and <code>true</code> is filled.
     */
    virtual void setFilled(bool flag);

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
     * Method: setLineStyle
     * Usage: gobj->setLineStyle(QGObject::LINE_DASH);
     * ---------------------------------------------------
     * Sets the object's style such as solid or dashed.
     */
    virtual void setLineStyle(LineStyle lineStyle);

    /*
     * Method: setLineWidth
     * Usage: gobj->setLineWidth(lineWidth);
     * -------------------------------------
     * Sets the width of the line used to draw this object.
     */
    virtual void setLineWidth(double lineWidth);

    /*
     * Method: setLocation
     * Usage: gobj->setLocation(pt);
     *        gobj->setLocation(x, y);
     * -------------------------------
     * Sets the location of the top-left corner of this object to the specified point.
     */
    virtual void setLocation(double x, double y);
    virtual void setLocation(const QGPoint& pt);

    /*
     * Method: setSize
     * Usage: rect->setSize(size);
     *        rect->setSize(width, height);
     * ------------------------------------
     * Changes the size of this rectangle to the specified width and height.
     */
    virtual void setSize(double width, double height);
    virtual void setSize(const QGDimension& size);

    /*
     * Method: setVisible
     * Usage: gobj->setVisible(flag);
     * ------------------------------
     * Sets whether this object is visible.
     */
    virtual void setVisible(bool flag);

    /*
     * Method: setX/Y
     * Usage: gobj->setX(x);
     *        gobj->setY(Y);
     * ---------------------
     * Sets the x/y location of the top/left of this object.
     */
    virtual void setX(double x);
    virtual void setY(double y);

    /*
     * Method: toString
     * Usage: gobj->toString();
     * ------------------------
     * Returns a printable representation of the object.
     */
    virtual std::string toString() const;

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

    // whether to anti-alias graphical objects; default true
    static bool _sAntiAliasing;

    /* Instance variables */
protected:
    double _x;                       // the x coordinate of the origin
    double _y;                       // the y coordinate of the origin
    double _width;                   // the width of the bounding rectangle
    double _height;                  // the height of the bounding rectangle
    double _lineWidth;               // the width of the line in pixels
    LineStyle _lineStyle;            // line style such as solid or dashed
    std::string _color;              // the color of the object
    int _colorInt;
    std::string _fillColor;          // color used to fill the object
    int _fillColorInt;
    std::string _font;               // the font string of the label
    bool _fillFlag;                  // indicates whether the object is filled
    bool _visible;                   // indicates if object is visible
    bool _transformed;               // indicates if object is transformed
    QGCompound* _parent;             // pointer to the parent
    QPen _pen;                       // for outlines
    QBrush _brush;                   // for filling
    QTransform _transform;           // for transformations (rotate, scale)

protected:
    QGObject(double x = 0, double y = 0, double width = 0, double height = 0);

    virtual void initializeBrushAndPen(QPainter* painter = nullptr);
    static Qt::PenStyle toQtPenStyle(LineStyle lineStyle);
    virtual std::string toStringExtra() const;

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
     * Usage: QGArc* arc = new QGArc(width, height, start, sweep);
     *        QGArc* arc = new QGArc(x, y, width, height, start, sweep);
     * ---------------------------------------------------------------
     * Creates a new <code>QGArc</code> object consisting of an elliptical arc.
     * The first form creates a <code>QGArc</code> whose origin is the point
     * (0,&nbsp;0); the second form positions the <code>QGArc</code> at the
     * point (<code>x</code>, <code>y</code>).
     * <include src="pictures/QGObjectDiagrams/QGArcExamples.html">
     */
    QGArc(double width = 0, double height = 0, double start = 0, double sweep = 0);
    QGArc(double x, double y, double width, double height, double start, double sweep);

    virtual void draw(QPainter* painter);

    /*
     * Method: getEndPoint
     * Usage: QGPoint pt = arc->getEndPoint();
     * --------------------------------------
     * Returns the point at which the arc ends.
     */
    virtual QGPoint getEndPoint() const;

    /*
     * Method: getFrameRectangle
     * Usage: QGRectangle rect = arc->getFrameRectangle();
     * --------------------------------------------------
     * Returns the boundaries of the rectangle used to frame the arc.
     */
    virtual QGRectangle getFrameRectangle() const;

    /*
     * Method: getStartAngle
     * Usage: double angle = arc->getStartAngle();
     * -------------------------------------------
     * Returns the starting angle for this <code>QGArc</code> object.
     */
    virtual double getStartAngle() const;

    /*
     * Method: getStartPoint
     * Usage: QGPoint pt = arc->getStartPoint();
     * ----------------------------------------
     * Returns the point at which the arc starts.
     */
    virtual QGPoint getStartPoint() const;

    /*
     * Method: getSweepAngle
     * Usage: double angle = arc->getSweepAngle();
     * -------------------------------------------
     * Returns the sweep angle for this <code>QGArc</code> object.
     */
    virtual double getSweepAngle() const;

    /*
     * Method: setFrameRectangle
     * Usage: arc->setFrameRectangle(rect);
     *        arc->setFrameRectangle(x, y, width, height);
     * ---------------------------------------------------
     * Changes the boundaries of the rectangle used to frame the arc.
     */
    virtual void setFrameRectangle(const QGRectangle& rect);
    virtual void setFrameRectangle(double x, double y, double width, double height);

    /*
     * Method: setStartAngle
     * Usage: arc->setStartAngle(start);
     * ---------------------------------
     * Sets the starting angle for this <code>QGArc</code> object.
     */
    virtual void setStartAngle(double start);

    /*
     * Method: setSweepAngle
     * Usage: arc->setSweepAngle(start);
     * ---------------------------------
     * Sets the sweep angle for this <code>QGArc</code> object.
     */
    virtual void setSweepAngle(double start);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual QGRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    virtual bool containsAngle(double theta) const;
    virtual QGPoint getArcPoint(double theta) const;

    /* Instance variables */
    double _start;                   /* Starting angle of the arc       */
    double _sweep;                   /* How many degrees the arc runs   */
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
     * Usage: QGCompound* comp = new QGCompound();
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
    virtual void add(QGObject* gobj);
    virtual void add(QGObject* gobj, double x, double y);
    virtual void add(QGObject& gobj);
    virtual void add(QGObject& gobj, double x, double y);

    /*
     * Method: clear
     * Usage: comp->clear();
     * ---------------------
     * Removes all graphical objects from the <code>QGCompound</code>.
     * Equivalent to removeAll.
     */
    virtual void clear();

    /*
     * ...
     */
    virtual void conditionalRepaint();
    virtual void conditionalRepaintRegion(int x, int y, int width, int height);
    virtual void conditionalRepaintRegion(const QGRectangle& bounds);

    /*
     * Draws all objects stored in this compound using the given painter pen.
     */
    virtual void draw(QPainter* painter);

    /*
     * Method: getElement
     * Usage: QGObject* gobj = comp->getElement(index);
     * -----------------------------------------------
     * Returns a pointer to the graphical object at the specified index,
     * numbering from back to front in the the <i>z</i> dimension.
     */
    virtual QGObject* getElement(int index) const;

    /*
     * Method: getElementAt
     * Usage: QGObject* gobj = comp->getElementAt(x, y);
     * ------------------------------------------------
     * Returns a pointer to the first graphical object that contains the given
     * (x, y) point, or a null pointer if no object in this compound touches it.
     */
    virtual QGObject* getElementAt(double x, double y) const;

    /*
     * Method: getElementCount
     * Usage: int n = comp->getElementCount();
     * ---------------------------------------
     * Returns the number of graphical objects stored in the
     * <code>QGCompound</code>.
     */
    virtual int getElementCount() const;

    virtual QWidget* getWidget() const;

    virtual bool isAutoRepaint() const;

    virtual bool isEmpty() const;

    /*
     * Method: remove
     * Usage: comp->remove(gobj);
     * --------------------------
     * Removes the specified object from the <code>QGCompound</code>.
     */
    virtual void remove(QGObject* gobj);
    virtual void remove(QGObject& gobj);

    /*
     * Method: removeAll
     * Usage: comp->removeAll();
     * -------------------------
     * Removes all graphical objects from the <code>QGCompound</code>.
     * Equivalent to clear.
     */
    virtual void removeAll();

    virtual void repaint();
    virtual void repaintRegion(int x, int y, int width, int height);
    virtual void repaintRegion(const QGRectangle& bounds);

    virtual void setAutoRepaint(bool autoRepaint);

    virtual void setWidget(QWidget* widget);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual QGRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toString() const;

private:
    void sendBackward(QGObject* gobj);
    void sendForward(QGObject* gobj);
    void sendToBack(QGObject* gobj);
    void sendToFront(QGObject* gobj);
    virtual int findQGObject(QGObject* gobj) const;
    virtual void removeAt(int index);

    /* Instance variables */
    Vector<QGObject*> _contents;
    QWidget* _widget;    // widget containing this compound
    bool _autoRepaint;   // automatically repaint on any change; default true

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
 *       QGImage* tree = new QGImage("StanfordTree.png");
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
    QGImage(const std::string& filename = "", double x = 0, double y = 0);
    virtual ~QGImage();

    virtual void draw(QPainter* painter);

    virtual std::string getFileName() const;

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    std::string _filename;
    QImage* _qimage;
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
     * --------------------------------------------------
     * Constructs a line segment from its endpoints.  The point
     * (<code>x0</code>,&nbsp;<code>y0</code>) defines the start of the
     * line and the point (<code>x1</code>,&nbsp;<code>y1</code>) defines
     * the end.
     */
    QGLine(double x0 = 0, double y0 = 0, double x1 = 0, double y1 = 0, LineStyle lineStyle = LINE_SOLID);

    /*
     * Constructor: QGLine
     * Usage: QGLine* gline = new QGLine(p0, p1);
     * ----------------------------------------
     * Constructs a line segment from its endpoints.  The point
     * <code>p0</code> defines the start of the line and the point
     * <code>p1</code> defines the end.
     */
    QGLine(const QGPoint& p0, const QGPoint& p1);

    virtual void draw(QPainter* painter);

    /*
     * Method: getEndPoint
     * Usage: QGPoint pt = line->getEndPoint();
     * ---------------------------------------
     * Returns the point at which the line ends.
     */
    virtual QGPoint getEndPoint() const;

    /*
     * Method: getStartPoint
     * Usage: QGPoint pt = line->getStartPoint();
     * -----------------------------------------
     * Returns the point at which the line starts.
     */
    virtual QGPoint getStartPoint() const;

    /*
     * Method: setEndPoint
     * Usage: line->setEndPoint(x, y);
     * -------------------------------
     * Sets the end point in the line to (<code>x</code>,&nbsp;<code>y</code>),
     * leaving the start point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    virtual void setEndPoint(double x, double y);

    /*
     * Method: setStartPoint
     * Usage: line->setStartPoint(x, y);
     * ---------------------------------
     * Sets the initial point in the line to (<code>x</code>,&nbsp;<code>y</code>),
     * leaving the end point unchanged.  This method is therefore different from
     * <code>setLocation</code>, which moves both components of the line segment.
     */
    virtual void setStartPoint(double x, double y);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual QGRectangle getBounds() const;
    virtual double getHeight() const;
    virtual std::string getType() const;
    virtual double getWidth() const;
    virtual std::string toStringExtra() const;

protected:
    /* Instance variables */
    double _dx;   // the x displacement of the line
    double _dy;   // the y displacement of the line
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
 *       QGOval* oval = new QGOval(gw.getWidth(), gw.getHeight());
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
     * Usage: QGOval* oval = new QGOval(width, height);
     *        QGOval* oval = new QGOval(x, y, width, height);
     * ----------------------------------------------------
     * Constructs a new oval inscribed in the specified rectangle.  The
     * first form is positioned at the origin; the second at the coordinates
     * given by <code>x</code> and <code>y</code>.
     */
    QGOval(double x = 0, double y = 0, double width = 0, double height = 0);

    virtual void draw(QPainter* painter);

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual std::string getType() const;
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
    QGPolygon(std::initializer_list<QGPoint> points);

    /*
     * Method: addEdge
     * Usage: poly->addEdge(dx, dy);
     * -----------------------------
     * Adds an edge to the polygon whose components are given by the displacements
     * <code>dx</code> and <code>dy</code> from the last vertex.
     */
    virtual void addEdge(double dx, double dy);
    virtual void addEdge(const QGPoint& pt);

    /*
     * Method: addEdges
     * Usage: poly->addEdges({dx1, dy1, dx2, dy2, ..., dxN, dyN});
     * -----------------------------------------------------------
     * Adds multiple edges to the polygon whose components are given by the
     * displacements <code>dx</code> and <code>dy</code> from the last vertex.
     */
    virtual void addEdges(std::initializer_list<double> coords);
    virtual void addEdges(std::initializer_list<QGPoint> points);

    /*
     * Method: addPolarEdge
     * Usage: poly->addPolarEdge(r, theta);
     * ------------------------------------
     * Adds an edge to the polygon specified in polar coordinates.  The length
     * of the edge is given by <code>r</code>, and the edge extends in
     * direction <code>theta</code>, measured in degrees counterclockwise
     * from the +x axis.
     */
    virtual void addPolarEdge(double r, double theta);

    /*
     * Method: addVertex
     * Usage: poly->addVertex(x, y);
     * -----------------------------
     * Adds a vertex at (<code>x</code>, <code>y</code>) relative to the polygon
     * origin.
     */
    virtual void addVertex(double x, double y);
    virtual void addVertex(const QGPoint& pt);

    /*
     * Method: addVertexes
     * Usage: poly->addVertexes({x1, y1, x2, y2, ..., xN, yN});
     * -----------------------------------------------------------
     * Adds multiple edges to the polygon whose components are given by the
     * coordinates <code>dx</code> and <code>dy</code> relative to the polygon origin.
     */
    virtual void addVertexes(std::initializer_list<double> coords);
    virtual void addVertexes(std::initializer_list<QGPoint> points);

    virtual void draw(QPainter* painter);

    /*
     * Method: getVertices
     * Usage: Vector<QGPoint> vec = poly->getVertices();
     * ------------------------------------------------
     * Returns a vector of the points in the polygon.
     */
    virtual Vector<QGPoint> getVertices() const;

    /* Prototypes for the virtual methods */
    virtual bool contains(double x, double y) const;
    virtual QGRectangle getBounds() const;
    virtual double getHeight() const;
    virtual std::string getType() const;
    virtual double getWidth() const;
    virtual std::string toStringExtra() const;

private:
    /* Instance variables */
    QVector<QPointF> _vertices;   // the vertices of the polygon
    double _cx;                   // the most recent x coordinate
    double _cy;                   // the most recent y coordinate
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
 *       QGRect* rect = new QGRect(0, 0, 200, 100);
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
     * Usage: QGRect* rect = new QGRect(width, height);
     *        QGRect* rect = new QGRect(x, y, width, height);
     * ----------------------------------------------------
     * Constructs a rectangle with the specified width and height.  The first
     * form is positioned at the origin; the second at the coordinates
     * given by <code>x</code> and <code>y</code>.
     */
    QGRect(double x = 0, double y = 0, double width = 0, double height = 0);

    virtual void draw(QPainter* painter);

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
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
     * Usage: QGRoundRect* rect = new QGRoundRect(width, height);
     *        QGRoundRect* rect = new QGRoundRect(width, height, corner);
     *        QGRoundRect* rect = new QGRoundRect(x, y, width, height);
     *        QGRoundRect* rect = new QGRoundRect(x, y, width, height, corner);
     * ------------------------------------------------------------------------
     * Constructs a new rectangle with the specified width and height.  If
     * the <code>x</code> and <code>y</code> parameters are specified, they
     * are used to specify the origin.  The <code>corner</code> parameter
     * specifies the diameter of the arc forming the corner.
     */
    QGRoundRect(double width = 0, double height = 0);
    QGRoundRect(double width, double height, double corner);
    QGRoundRect(double x, double y, double width, double height);
    QGRoundRect(double x, double y, double width, double height, double corner);

    /*
     * Method: contains
     * Usage: if (grrect->contains(pt)) ...
     *        if (grrect->contains(x, y)) ...
     * --------------------------------------
     * Returns <code>true</code> if the specified point is inside the object.
     */
    virtual bool contains(double x, double y) const;

    virtual void draw(QPainter* painter);

    virtual double getCorner() const;

    virtual void setCorner(double corner);

    /* Prototypes for the virtual methods */
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

protected:
    double _corner;
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
     * Usage: QGString* label = new QGString(str);
     *        QGString* label = new QGString(str, x, y);
     * -------------------------------------------------
     * Creates a <code>QGString</code> object containing the specified string.
     * By default, the baseline of the first character appears at the origin;
     * the second form automatically resets the location of the
     * <code>QGString</code> to the point (<code>x</code>, <code>y</code>).
     */
    QGString(const std::string& str = "", double x = 0, double y = 0);

    virtual void draw(QPainter* painter);

    /*
     * Method: getFont
     * Usage: string font = label->getFont();
     * --------------------------------------
     * Returns the current font for the <code>QGString</code>.
     */
    virtual std::string getFont() const;

    /*
     * Method: getFontAscent
     * Usage: double ascent = label->getFontAscent();
     * ----------------------------------------------
     * Returns the maximum distance strings in this font extend above
     * the baseline.
     */
    virtual double getFontAscent() const;

    /*
     * Method: getFontDescent
     * Usage: double descent = label->getFontDescent();
     * ------------------------------------------------
     * Returns the maximum distance strings in this font descend below
     * the baseline.
     */
    virtual double getFontDescent() const;

    /*
     * Method: getLabel
     * Usage: string str = label->getLabel();
     * --------------------------------------
     * Returns the string displayed by this object.
     */
    virtual std::string getLabel() const;
    virtual std::string getText() const;

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
    virtual void setLabel(const std::string& str);
    virtual void setText(const std::string& str);

    /* Prototypes for the virtual methods */
    virtual QGRectangle getBounds() const;
    virtual std::string getType() const;
    virtual std::string toStringExtra() const;

private:
    /* Instance variables */
    std::string _text;   // the string displayed by the label
    QFont _qfont;

    // update width and height when font or text changes
    void updateSize();
};

std::ostream& operator <<(std::ostream& out, const QGObject& obj);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgobjects_h
#endif // SPL_QT_GUI
