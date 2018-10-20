/*
 * File: gtypes.h
 * --------------
 * This file defines classes for representing points, dimensions, and
 * rectangles.
 *
 * @version 2018/09/09
 * - added doc comments for new documentation generation
 * @version 2018/07/14
 * - initial version, based on gtypes.h
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
// signal that GUI system is in use (so it will be initialized)
#define SPL_QT_GUI_IN_USE 1
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _gtypes_h
#define _gtypes_h

#include <functional>
#include <iostream>
#include <Qt>
#include <string>

/**
 * An alias for a function wrapper around a void function with no parameters
 * and no return.
 */
typedef std::function<void()> GThunk;

/**
 * An alias for a function wrapper around a function with no parameters
 * and an int return (such as main()).
 */
typedef std::function<int()> GThunkInt;

class Point;

/**
 * This class contains real-valued width and height fields.
 * It is used to indicate the size of a graphical object.
 */
class GDimension {
public:
    /**
     * Creates a <code>GDimension</code> object with the specified
     * <code>width</code> and <code>height</code> coordinates.  If the
     * coordinates are not supplied, the default constructor sets these
     * fields to 0.
     */
    GDimension(double width = 0, double height = 0);

    /**
     * Returns the width component of the <code>GDimension</code> object.
     */
    double getWidth() const;

    /**
     * Returns the height component of the <code>GDimension</code> object.
     */
    double getHeight() const;

    /**
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

/**
 * Writes the GDimension to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const GDimension& dim);

/**
 * Compares two GDimension objects for equality.
 */
bool operator ==(const GDimension& d1, const GDimension& d2);

/**
 * Compares two GDimension objects for inequality.
 */
bool operator !=(const GDimension& d1, const GDimension& d2);

/**
 * Relational operators that compare two GDimension objects by width and
 * then by height.
 */
bool operator <(const GDimension& d1, const GDimension& d2);

/**
 * Relational operators that compare two GDimension objects by width and
 * then by height.
 */
bool operator <=(const GDimension& d1, const GDimension& d2);

/**
 * Relational operators that compare two GDimension objects by width and
 * then by height.
 */
bool operator >(const GDimension& d1, const GDimension& d2);

/**
 * Relational operators that compare two GDimension objects by width and
 * then by height.
 */
bool operator >=(const GDimension& d1, const GDimension& d2);

/**
 * Multiplies the width and height of the given GDimension object by the given
 * scale factor and returns the scaled dimension object.
 */
GDimension operator *(const GDimension& d, double scale);

/**
 * Hashing function for GDimension objects.
 */
int hashCode(const GDimension& dim);

/**
 * The supported kinds of horizontal alignment of a widget or
 * onscreen object.
 */
enum HorizontalAlignment {
    ALIGN_CENTER,
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_HORIZONTAL_STRETCH
};

/**
 * The supported kinds of vertical alignment of a widget or
 * onscreen object.
 */
enum VerticalAlignment {
    ALIGN_MIDDLE,
    ALIGN_TOP,
    ALIGN_BOTTOM,
    ALIGN_VERTICAL_STRETCH
};

/**
 * Constants for alignments and icon positions.
 * Retained for backward compatibility; new code should not use this enum.
 *
 */
enum SwingConstants {
    SWING_CENTER,
    SWING_TOP,
    SWING_LEFT,
    SWING_BOTTOM,
    SWING_RIGHT
};
// Note: Must keep in sync with GInteractor::TextPosition.

/**
 * Converts an alignment value into a string such as "Center" or "Left".
 */
std::string toString(HorizontalAlignment alignment);

/**
 * Converts an alignment value into a string such as "Middle" or "Top".
 */
std::string toString(VerticalAlignment alignment);

/**
 * Converts a string such as "Center" or "Left" into an alignment value.
 */
HorizontalAlignment toHorizontalAlignment(const std::string& alignmentStr);

/**
 * Converts our alignment values into Qt alignment constants.
 */
Qt::Alignment toQtAlignment(HorizontalAlignment alignment);

/**
 * Converts our alignment values into Qt alignment constants.
 */
Qt::Alignment toQtAlignment(VerticalAlignment alignment);

/**
 * Converts a string such as "Middle" or "Top" into an alignment value.
 */
VerticalAlignment toVerticalAlignment(const std::string& alignmentStr);

/**
 * This class contains real-valued x and y fields.
 * It is used to represent a location on the graphics plane.
 */
class GPoint {
public:
    /**
     * Creates a <code>GPoint</code> object with the specified <code>x</code>
     * and <code>y</code> coordinates.  If the coordinates are not supplied,
     * the default constructor sets these fields to 0.
     */
    GPoint(double x = 0, double y = 0);

    /**
     * Creates a <code>GPoint</code> object with the same <code>x</code>
     * and <code>y</code> coordinates as the given point.
     */
    GPoint(const Point& point);

    /**
     * Returns the x component of the point.
     */
    double getX() const;

    /**
     * Returns the y component of the point.
     */
    double getY() const;

    /**
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

/**
 * Writes the given point to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const GPoint& pt);

/**
 * Compares two GPoint objects for equality.
 */
bool operator ==(const GPoint& p1, const GPoint& p2);

/**
 * Compares two GPoint objects for inequality.
 */
bool operator !=(const GPoint& p1, const GPoint& p2);

/**
 * Relational operators that compare points by x-coordinate and then
 * by y-coordinate.
 */
bool operator <(const GPoint& p1, const GPoint& p2);

/**
 * Relational operators that compare points by x-coordinate and then
 * by y-coordinate.
 */
bool operator <=(const GPoint& p1, const GPoint& p2);

/**
 * Relational operators that compare points by x-coordinate and then
 * by y-coordinate.
 */
bool operator >(const GPoint& p1, const GPoint& p2);

/**
 * Relational operators that compare points by x-coordinate and then
 * by y-coordinate.
 */
bool operator >=(const GPoint& p1, const GPoint& p2);

/**
 * Multiplies the x and y coordinates of the given point by the given scale
 * factor and returns the scaled point.
 */
GPoint operator *(const GPoint& p, double scale);

/**
 * Hashing function for GPoint objects.
 */
int hashCode(const GPoint& pt);

/**
 * This class contains real-valued x, y, width, and height fields.
 * It is used to represent the bounding box of a graphical object.
 */
class GRectangle {
public:
    /**
     * Creates a <code>GRectangle</code> object with the specified position
     * and size. If these parameters are not supplied, the constructor sets
     * these fields to 0.
     */
    GRectangle(double x = 0, double y = 0, double width = 0, double height = 0);

    /**
     * Creates a <code>GRectangle</code> object with the specified position
     * and size. If these parameters are not supplied, the constructor sets
     * these fields to 0.
     */
    GRectangle(double x, double y, const GDimension& size);

    /**
     * Creates a <code>GRectangle</code> object with the specified position
     * and size. If these parameters are not supplied, the constructor sets
     * these fields to 0.
     */
    GRectangle(const GPoint& p, double width = 0, double height = 0);

    /**
     * Creates a <code>GRectangle</code> object with the specified position
     * and size. If these parameters are not supplied, the constructor sets
     * these fields to 0.
     */
    GRectangle(const GPoint& p, const GDimension& size);

    /**
     * Returns a new rectangle with its boundaries shifted outward by the given
     * amount on all 4 sides.
     * e.g. a 10x10 rectangle at position (55, 42) enlarged by 1 will become
     *      a 12x12 rectangle at position (54, 41).
     */
    GRectangle enlargedBy(double amount);

    /**
     * Returns the x component of the rectangle.
     */
    double getX() const;

    /**
     * Returns the y component of the rectangle.
     */
    double getY() const;

    /**
     * Returns the width component of the rectangle.
     */
    double getWidth() const;

    /**
     * Returns the height component of the rectangle.
     */
    double getHeight() const;

    /**
     * Returns <code>true</code> if the rectangle is empty, meaning that it
     * has a width and height that are both 0 or negative.
     */
    bool isEmpty() const;

    /**
     * Returns <code>true</code> if the rectangle contains the given point,
     * which may be specified either as a point or as distinct coordinates.
     */
    bool contains(const GPoint& pt) const;
    bool contains(double x, double y) const;

    /**
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

/**
 * Writes the given rectangle to the given output stream.
 */
std::ostream& operator <<(std::ostream& os, const GRectangle& rect);

/**
 * Compares two rectangles for equality.
 */
bool operator ==(const GRectangle& r1, const GRectangle& r2);

/**
 * Compares two rectangles for inequality.
 */
bool operator !=(const GRectangle& r1, const GRectangle& r2);

/**
 * Relational operators that compare rectangles by x, y, then width, then height.
 */
bool operator <(const GRectangle& r1, const GRectangle& r2);

/**
 * Relational operators that compare rectangles by x, y, then width, then height.
 */
bool operator <=(const GRectangle& r1, const GRectangle& r2);

/**
 * Relational operators that compare rectangles by x, y, then width, then height.
 */
bool operator >(const GRectangle& r1, const GRectangle& r2);

/**
 * Relational operators that compare rectangles by x, y, then width, then height.
 */
bool operator >=(const GRectangle& r1, const GRectangle& r2);

/**
 * Hashing function for GRectangle objects.
 */
int hashCode(const GRectangle& r);

#endif // _gtypes_h
