/*
 * File: qgtypes.h
 * ---------------
 * This file defines classes for representing points, dimensions, and
 * rectangles.
 *
 * @version 2018/07/14
 * - initial version, based on gtypes.h
 */

#ifndef _qgtypes_h
#define _qgtypes_h

#include <iostream>
#include <string>

/*
 * Class: QGDimension
 * ------------------
 * This class contains real-valued width and height fields.
 * It is used to indicate the size of a graphical object.
 */
class QGDimension {
public:
    /*
     * Constructor: QGDimension
     * Usage: QGDimension empty;
     *        QGDimension dim(width, height);
     * --------------------------------------
     * Creates a <code>QGDimension</code> object with the specified
     * <code>width</code> and <code>height</code> coordinates.  If the
     * coordinates are not supplied, the default constructor sets these
     * fields to 0.
     */
    QGDimension(double width = 0, double height = 0);

    /*
     * Method: getWidth
     * Usage: double width = dim.getWidth();
     * -------------------------------------
     * Returns the width component of the <code>QGDimension</code> object.
     */
    double getWidth() const;

    /*
     * Method: getHeight
     * Usage: double height = dim.getHeight();
     * ---------------------------------------
     * Returns the height component of the <code>QGDimension</code> object.
     */
    double getHeight() const;

    /*
     * Method: toString
     * Usage: string str = dim.toString();
     * -----------------------------------
     * Converts the <code>QGDimension</code> to a string in the form
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
    double _width;                   /* The width of the QGDimension       */
    double _height;                  /* The height of the QGDimension      */

    /* Friend declarations */
    friend std::ostream& operator <<(std::ostream& os, const QGDimension& dim);
    friend bool operator ==(const QGDimension& d1, const QGDimension& d2);
    friend bool operator !=(const QGDimension& d1, const QGDimension& d2);
    friend bool operator <(const QGDimension& d1, const QGDimension& d2);
    friend bool operator <=(const QGDimension& d1, const QGDimension& d2);
    friend bool operator >(const QGDimension& d1, const QGDimension& d2);
    friend bool operator >=(const QGDimension& d1, const QGDimension& d2);
    friend QGDimension operator *(const QGDimension& d, double scale);
    friend int hashCode(const QGDimension& dim);
};

std::ostream& operator <<(std::ostream& os, const QGDimension& dim);
bool operator ==(const QGDimension& d1, const QGDimension& d2);
bool operator !=(const QGDimension& d1, const QGDimension& d2);
bool operator <(const QGDimension& d1, const QGDimension& d2);
bool operator <=(const QGDimension& d1, const QGDimension& d2);
bool operator >(const QGDimension& d1, const QGDimension& d2);
bool operator >=(const QGDimension& d1, const QGDimension& d2);
QGDimension operator *(const QGDimension& d, double scale);
int hashCode(const QGDimension& dim);

/*
 * Class: QGPoint
 * --------------
 * This class contains real-valued x and y fields.
 * It is used to represent a location on the graphics plane.
 */
class QGPoint {
public:
    /*
     * Constructor: QGPoint
     * Usage: QGPoint origin;
     *        QGPoint pt(x, y);
     * ------------------------
     * Creates a <code>QGPoint</code> object with the specified <code>x</code>
     * and <code>y</code> coordinates.  If the coordinates are not supplied,
     * the default constructor sets these fields to 0.
     */
    QGPoint(double x = 0, double y = 0);

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
     * Converts the <code>QGPoint</code> to a string in the form
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
    friend std::ostream& operator <<(std::ostream& out, const QGPoint& p);
    friend bool operator ==(const QGPoint& p1, const QGPoint& p2);
    friend bool operator !=(const QGPoint& p1, const QGPoint& p2);
    friend bool operator <(const QGPoint& p1, const QGPoint& p2);
    friend bool operator <=(const QGPoint& p1, const QGPoint& p2);
    friend bool operator >(const QGPoint& p1, const QGPoint& p2);
    friend bool operator >=(const QGPoint& p1, const QGPoint& p2);
    friend QGPoint operator *(const QGPoint& p, double scale);
    friend int hashCode(const QGPoint& pt);
};

std::ostream& operator <<(std::ostream& os, const QGPoint& pt);
bool operator ==(const QGPoint& p1, const QGPoint& p2);
bool operator !=(const QGPoint& p1, const QGPoint& p2);
bool operator <(const QGPoint& p1, const QGPoint& p2);
bool operator <=(const QGPoint& p1, const QGPoint& p2);
bool operator >(const QGPoint& p1, const QGPoint& p2);
bool operator >=(const QGPoint& p1, const QGPoint& p2);
QGPoint operator *(const QGPoint& p, double scale);
int hashCode(const QGPoint& pt);

/*
 * Class: QGRectangle
 * ------------------
 * This type contains real-valued x, y, width, and height fields.
 * It is used to represent the bounding box of a graphical object.
 */
class QGRectangle {
public:
    /*
     * Constructor: QGRectangle
     * Usage: QGRectangle empty;
     *        QGRectangle r(x, y, width, height);
     * ------------------------------------------
     * Creates a <code>QGRectangle</code> object with the specified components.
     * If these parameters are not supplied, the default constructor sets
     * these fields to 0.
     */
    QGRectangle(double x = 0, double y = 0, double width = 0, double height = 0);
    QGRectangle(double x, double y, const QGDimension& size);
    QGRectangle(const QGPoint& p, double width, double height);
    QGRectangle(const QGPoint& p, const QGDimension& size);

    /*
     * Shifts the boundaries outward by the given amount on all 4 sides.
     * e.g. a 10x10 rectangle at position (55, 42) enlarged by 1 will become
     *      a 12x12 rectangle at position (54, 41).
     */
    QGRectangle enlargedBy(double amount);

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
    bool contains(const QGPoint& pt) const;
    bool contains(double _x, double _y) const;

    /*
     * Method: toString
     * Usage: string str = r.toString();
     * ---------------------------------
     * Converts the <code>QGRectangle</code> to a string in the form
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
    friend std::ostream& operator <<(std::ostream& os, const QGRectangle& rect);
    friend bool operator ==(const QGRectangle& r1, const QGRectangle& r2);
    friend bool operator !=(const QGRectangle& r1, const QGRectangle& r2);
    friend bool operator <(const QGRectangle& r1, const QGRectangle& r2);
    friend bool operator <=(const QGRectangle& r1, const QGRectangle& r2);
    friend bool operator >(const QGRectangle& r1, const QGRectangle& r2);
    friend bool operator >=(const QGRectangle& r1, const QGRectangle& r2);
    friend int hashCode(const QGRectangle& r);
};

std::ostream& operator <<(std::ostream& os, const QGRectangle& rect);
bool operator ==(const QGRectangle& r1, const QGRectangle& r2);
bool operator !=(const QGRectangle& r1, const QGRectangle& r2);
bool operator <(const QGRectangle& r1, const QGRectangle& r2);
bool operator <=(const QGRectangle& r1, const QGRectangle& r2);
bool operator >(const QGRectangle& r1, const QGRectangle& r2);
bool operator >=(const QGRectangle& r1, const QGRectangle& r2);
int hashCode(const QGRectangle& r);

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#endif // _qgtypes_h
