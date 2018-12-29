/*
 * File: point.h
 * -------------
 * This file exports a class representing an integer-valued <i>x</i>-<i>y</i>
 * pair.
 *
 * @version 2018/11/22
 * - added headless mode support
 * @version 2018/09/25
 * - added doc comments for new documentation generation
 */

#include "private/init.h"   // ensure that Stanford C++ lib is initialized

#ifndef INTERNAL_INCLUDE
#include "private/initstudent.h"   // insert necessary included code by student
#endif // INTERNAL_INCLUDE

#ifndef _point_h
#define _point_h

#include <string>

#ifndef SPL_HEADLESS_MODE
class GPoint;   // forward declaration
#endif // SPL_HEADLESS_MODE

/**
 * This class represents an <i>x</i>-<i>y</i> coordinate point on a
 * two-dimensional integer grid.  If you need to work with real-valued points,
 * you should use the <a href="gtypes.html"><code>gtypes.h</code></a>
 * interface instead.
 */
class Point {
public:
    /**
     * Creates a <code>Point</code> object at (0, 0).
     */
    Point();

    /**
     * Creates a <code>Point</code> object with the specified x and y coordinates.
     */
    Point(int x, int y);

#ifndef SPL_HEADLESS_MODE
    /**
     * Creates a <code>Point</code> object with the same x and y coordinates
     * as the given other point.
     */
    Point(const GPoint& point);
#endif // SPL_HEADLESS_MODE

    /**
     * Returns the <i>x</i>-coordinate of the point.
     */
    int getX() const;

    /**
     * Returns the <i>y</i>-coordinate of the point.
     */
    int getY() const;

    /**
     * Returns a string representation of the <code>Point</code> in the form
     * <code>"(x, y)"</code>.
     */
    std::string toString() const;

    /**
     * Returns <code>true</code> if <code>p1</code> and <code>p2</code>
     * are the same point.
     */
    bool operator ==(const Point& p2) const;

    /**
     * Returns <code>true</code> if <code>p1</code> and <code>p2</code>
     * are different.
     */
    bool operator !=(const Point& p2) const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    // instance variables
    int x;                         // the x-coordinate of the point
    int y;                         // the y-coordinate of the point
};

/**
 * Overloads the <code>&lt;&lt;</code> operator so that it is able
 * to display <code>Point</code> values.
 */
std::ostream& operator <<(std::ostream& os, const Point& pt);

/**
 * Hash code function for Point objects.
 */
int hashCode(const Point& pt);

#endif // _point_h
