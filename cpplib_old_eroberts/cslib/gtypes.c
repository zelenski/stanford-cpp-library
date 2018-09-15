/*
 * File: gtypes.c
 * --------------
 * This file implements the gtypes.h interface.
 */

#include "cslib.h"
#include "generic.h"
#include "gtypes.h"

GPoint createGPoint(double x, double y) {
   GPoint pt;

   pt.x = x;
   pt.y = y;
   return pt;
}

double getXGPoint(GPoint pt) {
   return pt.x;
}

double getYGPoint(GPoint pt) {
   return pt.y;
}

GDimension createGDimension(double width, double height) {
   GDimension dim;

   dim.width = width;
   dim.height = height;
   return dim;
}

double getWidthGDimension(GDimension dim) {
   return dim.width;
}

double getHeightGDimension(GDimension dim) {
   return dim.height;
}

GRectangle createGRectangle(double x, double y, double width, double height) {
   GRectangle r;

   r.x = x;
   r.y = y;
   r.width = width;
   r.height = height;
   return r;
}

double getXGRectangle(GRectangle r) {
   return r.x;
}

double getYGRectangle(GRectangle r) {
   return r.y;
}

double getWidthGRectangle(GRectangle r) {
   return r.width;
}

double getHeightGRectangle(GRectangle r) {
   return r.height;
}

bool isEmptyGRectangle(GRectangle r) {
   return r.width <= 0 || r.height <= 0;
}

bool containsGRectangle(GRectangle r, GPoint pt) {
   return pt.x >= r.x && pt.y >= r.y && pt.x < r.x + r.width
                                     && pt.y < r.y + r.height;
}

/**********************************************************************/
/* Unit test for the gtypes module                                    */
/**********************************************************************/

#ifndef _NOTEST_

#include "unittest.h"

/* Unit test */

void testGTypesModule(void) {
   GPoint origin, pt;
   GDimension dim;
   GRectangle r;

   trace(origin = createGPoint(0, 0));
   trace(pt = createGPoint(2, 3));
   trace(dim = createGDimension(3, 1));
   trace(r = createGRectangle(1, 2, 3, 4));
   test(getX(origin), 0.0);
   test(getY(origin), 0.0);
   test(getX(pt), 2.0);
   test(getY(pt), 3.0);
   test(getWidth(dim), 3.0);
   test(getHeight(dim), 1.0);
   test(getX(r), 1.0);
   test(getY(r), 2.0);
   test(getWidth(r), 3.0);
   test(getHeight(r), 4.0);
   test(isEmpty(r), false);
   test(contains(r, origin), false);
   test(contains(r, pt), true);
}

#endif
