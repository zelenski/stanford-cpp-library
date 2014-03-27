/*
 * File: point.h
 * -------------
 * This file exports a class representing an integer-valued <i>x</i>-<i>y</i>
 * pair.
 */

#ifndef _point_h
#define _point_h

#include <string>

/*
 * Class: Point
 * ------------
 * This class represents an <i>x</i>-<i>y</i> coordinate point on a
 * two-dimensional integer grid.  If you need to work with real-valued points,
 * you should use the <a href="gtypes.html"><code>gtypes.h</code></a>
 * interface instead.
 */
class Point {
public:
    /*
     * Constructor: Point
     * Usage: Point origin;
     *        Point pt(x, y);
     * ----------------------
     * Creates a <code>Point</code> object with the specified x and y coordinates.
     * If the coordinates are not supplied, the default constructor sets these
     * fields to 0.
     */
    Point();
    Point(int x, int y);

    /*
     * Method: getX
     * Usage: int x = pt.getX();
     * -------------------------
     * Returns the <i>x</i>-coordinate of the point.
     */
    int getX() const;

    /*
     * Method: getY
     * Usage: int y = pt.getY();
     * -------------------------
     * Returns the <i>y</i>-coordinate of the point.
     */
    int getY() const;

    /*
     * Method: toString
     * Usage: string str = pt.toString();
     * ----------------------------------
     * Returns a string representation of the <code>Point</code> in the form
     * <code>"(x, y)"</code>.
     */
    std::string toString() const;

    /*
     * Friend operator: ==
     * Usage: if (p1 == p2) ...
     * ------------------------
     * Returns <code>true</code> if <code>p1</code> and <code>p2</code>
     * are the same point.
     */
    bool operator==(const Point & p2) const;

    /*
     * Friend operator: !=
     * Usage: if (p1 != p2) ...
     * ------------------------
     * Returns <code>true</code> if <code>p1</code> and <code>p2</code>
     * are different.
     */
    bool operator!=(const Point & p2) const;

    /* Private section */

    /**********************************************************************/
    /* Note: Everything below this point in the file is logically part    */
    /* of the implementation and should not be of interest to clients.    */
    /**********************************************************************/

private:
    /* Instance variables */
    int x;                         /* The x-coordinate of the point */
    int y;                         /* The y-coordinate of the point */
};

/*
 * Operator: <<
 * Usage: cout << pt;
 * ------------------
 * Overloads the <code>&lt;&lt;</code> operator so that it is able
 * to display <code>Point</code> values.
 */
std::ostream & operator<<(std::ostream & os, const Point & pt);

// hash code function for Point objects
int hashCode(const Point & pt);

#endif
