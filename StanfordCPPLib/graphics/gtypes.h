/*
 * File: gtypes.h
 * --------------
 * This file defines classes for representing points, dimensions, and
 * rectangles.
 *
 * @version 2018/07/14
 * - initial version, based on gtypes.h
 */

#ifndef _gtypes_h
#define _gtypes_h

#include <functional>
#include <iostream>
#include <Qt>
#include <string>

// function pointer types (no params / no return)
typedef std::function<void()> GThunk;
typedef std::function<int()> GThunkInt;

/*
 * Class: GDimension
 * -----------------
 * This class contains real-valued width and height fields.
 * It is used to indicate the size of a graphical object.
 */
class GDimension {
public:
    /*
     * Constructor: GDimension
     * Usage: GDimension empty;
     *        GDimension dim(width, height);
     * -------------------------------------
     * Creates a <code>GDimension</code> object with the specified
     * <code>width</code> and <code>height</code> coordinates.  If the
     * coordinates are not supplied, the default constructor sets these
     * fields to 0.
     */
    GDimension(double width = 0, double height = 0);

    /*
     * Method: getWidth
     * Usage: double width = dim.getWidth();
     * -------------------------------------
     * Returns the width component of the <code>GDimension</code> object.
     */
    double getWidth() const;

    /*
     * Method: getHeight
     * Usage: double height = dim.getHeight();
     * ---------------------------------------
     * Returns the height component of the <code>GDimension</code> object.
     */
    double getHeight() const;

    /*
     * Method: toString
     * Usage: string str = dim.toString();
     * -----------------------------------
     * Converts the <code>GDimension</code> to a string in the form
     * <code>"(</code><i>width</i><code>,</code>&nbsp;<i>height</i><code>)"</code>.
     */
    std::string toString() const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in this class is logically part  */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/
private:
    /* Instance variables */
    double _width;                   /* The width of the GDimension       */
    double _height;                  /* The height of the GDimension      */

    /* Friend declarations */
    friend std::ostream& operator <<(std::ostream& os, const GDimension& dim);
    friend bool operator ==(const GDimension& d1, const GDimension& d2);
    friend bool operator !=(const GDimension& d1, const GDimension& d2);
    friend bool operator <(const GDimension& d1, const GDimension& d2);
    friend bool operator <=(const GDimension& d1, const GDimension& d2);
    friend bool operator >(const GDimension& d1, const GDimension& d2);
    friend bool operator >=(const GDimension& d1, const GDimension& d2);
    friend GDimension operator *(const GDimension& d, double scale);
    friend int hashCode(const GDimension& dim);
};

std::ostream& operator <<(std::ostream& os, const GDimension& dim);
bool operator ==(const GDimension& d1, const GDimension& d2);
bool operator !=(const GDimension& d1, const GDimension& d2);
bool operator <(const GDimension& d1, const GDimension& d2);
bool operator <=(const GDimension& d1, const GDimension& d2);
bool operator >(const GDimension& d1, const GDimension& d2);
bool operator >=(const GDimension& d1, const GDimension& d2);
GDimension operator *(const GDimension& d, double scale);
int hashCode(const GDimension& dim);

/*
 * The three supported kinds of horizontal alignment of a widget or
 * onscreen object.
 */
enum HorizontalAlignment {
    ALIGN_CENTER,
    ALIGN_LEFT,
    ALIGN_RIGHT
};

/*
 * The three supported kinds of vertical alignment of a widget or
 * onscreen object.
 */
enum VerticalAlignment {
    ALIGN_MIDDLE,
    ALIGN_TOP,
    ALIGN_BOTTOM
};

/*
 * Constants for alignments and icon positions.
 * Retained for backward compatibility; new code should not use this enum.
 * To dev: Keep in sync with GInteractor::TextPosition.
 */
enum SwingConstants {
    SWING_CENTER,
    SWING_TOP,
    SWING_LEFT,
    SWING_BOTTOM,
    SWING_RIGHT
};

std::string toString(HorizontalAlignment alignment);
std::string toString(VerticalAlignment alignment);
HorizontalAlignment toHorizontalAlignment(const std::string& alignmentStr);
Qt::Alignment toQtAlignment(HorizontalAlignment alignment);
Qt::Alignment toQtAlignment(VerticalAlignment alignment);
VerticalAlignment toVerticalAlignment(const std::string& alignmentStr);

/*
 * Class: GPoint
 * -------------
 * This class contains real-valued x and y fields.
 * It is used to represent a location on the graphics plane.
 */
class GPoint {
public:
    /*
     * Constructor: GPoint
     * Usage: GPoint origin;
     *        GPoint pt(x, y);
     * -----------------------
     * Creates a <code>GPoint</code> object with the specified <code>x</code>
     * and <code>y</code> coordinates.  If the coordinates are not supplied,
     * the default constructor sets these fields to 0.
     */
    GPoint(double x = 0, double y = 0);

    /*
     * Method: getX
     * Usage: double x = pt.getX();
     * ----------------------------
     * Returns the x component of the point.
     */
    double getX() const;

    /*
     * Method: getY
     * Usage: double y = pt.getY();
     * ----------------------------
     * Returns the y component of the point.
     */
    double getY() const;

    /*
     * Method: toString
     * Usage: string str = pt.toString();
     * ----------------------------------
     * Converts the <code>GPoint</code> to a string in the form
     * <code>"(</code><i>x</i><code>,</code>&nbsp;<i>y</i><code>)"</code>.
     */
    std::string toString() const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in this class is logically part  */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/
private:
    /* Instance variables */
    double _x;                       /* The x-coordinate of the point */
    double _y;                       /* The y-coordinate of the point */

    /* Friend declarations */
    friend std::ostream& operator <<(std::ostream& out, const GPoint& p);
    friend bool operator ==(const GPoint& p1, const GPoint& p2);
    friend bool operator !=(const GPoint& p1, const GPoint& p2);
    friend bool operator <(const GPoint& p1, const GPoint& p2);
    friend bool operator <=(const GPoint& p1, const GPoint& p2);
    friend bool operator >(const GPoint& p1, const GPoint& p2);
    friend bool operator >=(const GPoint& p1, const GPoint& p2);
    friend GPoint operator *(const GPoint& p, double scale);
    friend int hashCode(const GPoint& pt);
};

std::ostream& operator <<(std::ostream& os, const GPoint& pt);
bool operator ==(const GPoint& p1, const GPoint& p2);
bool operator !=(const GPoint& p1, const GPoint& p2);
bool operator <(const GPoint& p1, const GPoint& p2);
bool operator <=(const GPoint& p1, const GPoint& p2);
bool operator >(const GPoint& p1, const GPoint& p2);
bool operator >=(const GPoint& p1, const GPoint& p2);
GPoint operator *(const GPoint& p, double scale);
int hashCode(const GPoint& pt);

/*
 * Class: GRectangle
 * -----------------
 * This type contains real-valued x, y, width, and height fields.
 * It is used to represent the bounding box of a graphical object.
 */
class GRectangle {
public:
    /*
     * Constructor: GRectangle
     * Usage: GRectangle empty;
     *        GRectangle r(x, y, width, height);
     * -----------------------------------------
     * Creates a <code>GRectangle</code> object with the specified components.
     * If these parameters are not supplied, the default constructor sets
     * these fields to 0.
     */
    GRectangle(double x = 0, double y = 0, double width = 0, double height = 0);
    GRectangle(double x, double y, const GDimension& size);
    GRectangle(const GPoint& p, double width, double height);
    GRectangle(const GPoint& p, const GDimension& size);

    /*
     * Shifts the boundaries outward by the given amount on all 4 sides.
     * e.g. a 10x10 rectangle at position (55, 42) enlarged by 1 will become
     *      a 12x12 rectangle at position (54, 41).
     */
    GRectangle enlargedBy(double amount);

    /*
     * Method: getX
     * Usage: double x = r.getX();
     * ---------------------------
     * Returns the x component of the rectangle.
     */
    double getX() const;

    /*
     * Method: getY
     * Usage: double y = pt.getY();
     * ----------------------------
     * Returns the y component of the rectangle.
     */
    double getY() const;

    /*
     * Method: getWidth
     * Usage: double width = r.getWidth();
     * -----------------------------------
     * Returns the width component of the rectangle.
     */
    double getWidth() const;

    /*
     * Method: getHeight
     * Usage: double height = pt.getHeight();
     * --------------------------------------
     * Returns the height component of the rectangle.
     */
    double getHeight() const;

    /*
     * Method: isEmpty
     * Usage: if (r.isEmpty()) ...
     * ---------------------------
     * Returns <code>true</code> if the rectangle is empty.
     */
    bool isEmpty() const;

    /*
     * Method: contains
     * Usage: if (r.contains(pt)) ...
     *        if (r.contains(x, y)) ...
     * --------------------------------
     * Returns <code>true</code> if the rectangle contains the given point,
     * which may be specified either as a point or as distinct coordinates.
     */
    bool contains(const GPoint& pt) const;
    bool contains(double _x, double _y) const;

    /*
     * Method: toString
     * Usage: string str = r.toString();
     * ---------------------------------
     * Converts the <code>GRectangle</code> to a string in the form
     * <code>"(</code><i>x</i><code>,</code>&nbsp;<i>y</i><code>,</code>
     * <i>width</i><code>,</code>&nbsp;<i>height</i><code>)"</code>.
     */
    std::string toString() const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in this class is logically part  */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/
private:
    /* Instance variables */
    double _x;        /* The x-coordinate of the rectangle */
    double _y;        /* The y-coordinate of the rectangle */
    double _width;    /* The width of the rectangle        */
    double _height;   /* The height of the rectangle       */

    /* Friend declarations */
    friend std::ostream& operator <<(std::ostream& os, const GRectangle& rect);
    friend bool operator ==(const GRectangle& r1, const GRectangle& r2);
    friend bool operator !=(const GRectangle& r1, const GRectangle& r2);
    friend bool operator <(const GRectangle& r1, const GRectangle& r2);
    friend bool operator <=(const GRectangle& r1, const GRectangle& r2);
    friend bool operator >(const GRectangle& r1, const GRectangle& r2);
    friend bool operator >=(const GRectangle& r1, const GRectangle& r2);
    friend int hashCode(const GRectangle& r);
};

std::ostream& operator <<(std::ostream& os, const GRectangle& rect);
bool operator ==(const GRectangle& r1, const GRectangle& r2);
bool operator !=(const GRectangle& r1, const GRectangle& r2);
bool operator <(const GRectangle& r1, const GRectangle& r2);
bool operator <=(const GRectangle& r1, const GRectangle& r2);
bool operator >(const GRectangle& r1, const GRectangle& r2);
bool operator >=(const GRectangle& r1, const GRectangle& r2);
int hashCode(const GRectangle& r);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _gtypes_h
