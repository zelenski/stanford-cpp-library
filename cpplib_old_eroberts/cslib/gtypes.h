/*
 * File: gtypes.h
 * --------------
 * This interface defines types for representing points, dimensions, and
 * rectangles.
 */

#ifndef _gtypes_h
#define _gtypes_h

#include "generic.h"

/*
 * Type: GPoint
 * ------------
 * This type contains real-valued x and y fields.  It is used to
 * represent a location on the graphics plane.
 */

typedef struct {
   double x;
   double y;
} GPoint;

/*
 * Type: GDimension
 * ----------------
 * This type contains real-valued width and height fields.  It is used
 * to indicate the size of a graphical object.
 */

typedef struct {
   double width;
   double height;
} GDimension;

/*
 * Type: GRectangle
 * ----------------
 * This type contains real-valued x, y, width, and height fields.  It is
 * used to represent the bounding box of a graphical object.
 */

typedef struct {
   double x;
   double y;
   double width;
   double height;
} GRectangle;

/*
 * Function: createGPoint
 * Usage: pt = createGPoint(x, y);
 * -------------------------------
 * Creates a <code>GPoint</code> structure with the specified <code>x</code>
 * and <code>y</code> coordinates.
 */

GPoint createGPoint(double x, double y);

/*
 * Function: getX
 * Usage: x = getX(pt);
 * --------------------
 * Returns the x component of the <code>GPoint</code>.
 */

double getXGPoint(GPoint pt);

/*
 * Function: getY
 * Usage: y = getY(pt);
 * --------------------
 * Returns the y component of the <code>GPoint</code>.
 */

double getYGPoint(GPoint pt);

/*
 * Function: createGDimension
 * Usage: dim = createGDimension(width, height);
 * ---------------------------------------------
 * Creates a <code>GDimension</code> value with the specified
 * <code>width</code> and <code>height</code> coordinates.
 */

GDimension createGDimension(double width, double height);

/*
 * Function: getWidth
 * Usage: width = getWidth(dim);
 * -----------------------------
 * Returns the width component of the <code>GDimension</code>.
 */

double getWidthGDimension(GDimension dim);

/*
 * Function: getHeight
 * Usage: width = getHeight(dim);
 * ------------------------------
 * Returns the width component of the <code>GDimension</code>.
 */

double getHeightGDimension(GDimension dim);

/*
 * Function: createGRectangle
 * Usage: r = createGRectangle(x, y, width, height);
 * -------------------------------------------------
 * Creates a <code>GRectangle</code> value with the specified components.
 */

GRectangle createGRectangle(double x, double y, double width, double height);

/*
 * Function: getX
 * Usage: double x = getX(r);
 * --------------------------
 * Returns the x component of the rectangle.
 */

double getXGRectangle(GRectangle r);

/*
 * Function: getY
 * Usage: double y = getY(r);
 * --------------------------
 * Returns the y component of the rectangle.
 */

double getYGRectangle(GRectangle r);

/*
 * Function: getWidth
 * Usage: width = getWidth(r);
 * ---------------------------
 * Returns the width component of the <code>GRectangle</code>.
 */

double getWidthGRectangle(GRectangle r);

/*
 * Function: getHeight
 * Usage: width = getHeight(r);
 * ----------------------------
 * Returns the width component of the <code>GRectangle</code>.
 */

double getHeightGRectangle(GRectangle r);

/*
 * Function: isEmpty
 * Usage: if (isEmpty(r)) . . .
 * ----------------------------
 * Returns <code>true</code> if the rectangle is empty.
 */

bool isEmptyGRectangle(GRectangle r);

/*
 * Function: contains
 * Usage: if (contains(r, pt)) . . .
 * ---------------------------------
 * Returns <code>true</code> if the rectangle contains the given point.
 */

bool containsGRectangle(GRectangle r, GPoint pt);

#endif
