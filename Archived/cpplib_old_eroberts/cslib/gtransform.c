/*
 * File: gtransform.c
 * ------------------
 * This file implements the gtransform.h interface.
 */

#include <math.h>
#include <simpio.h>
#include "generic.h"
#include "gmath.h"
#include "gtransform.h"
#include "gtypes.h"
#include "strbuf.h"
#include "strlib.h"
#include "unittest.h"

/*
 * Constant: EPSILON
 * -----------------
 * Values whose magnitude is less than EPSILON are displayed as 0.
 */

#define EPSILON 1.0E-10

/*
 * Type: GTransformCDT
 * -------------------
 * This type defines the concrete structure for the GTransform type.
 * Mathematically, the fields of this structure represent the following
 * 3x3 matrix:
 *
 *      a   b   0
 *      c   d   0
 *      tx  ty  1
 */

struct GTransformCDT {
   double a, b;
   double c, d;
   double tx, ty;
};

/* Exported entries */

GTransform newGTransform(void) {
   GTransform matrix;

   matrix = newBlock(GTransform);
   matrix->a = 1;
   matrix->b = 0;
   matrix->c = 0;
   matrix->d = 1;
   matrix->tx = 0;
   matrix->ty = 0;
   return matrix;
}

void freeGTransform(GTransform matrix) {
   freeBlock(matrix);
}

void appendGTransform(GTransform m1, GTransform m2) {
   double a, b, c, d, tx, ty;

   a = m1->a * m2->a + m1->c * m2->b;
   b = m1->b * m2->a + m1->d * m2->b;
   c = m1->a * m2->c + m1->c * m2->d;
   d = m1->b * m2->c + m1->d * m2->d;
   tx = m1->a * m2->tx + m1->c * m2->ty + m1->tx;
   ty = m1->b * m2->tx + m1->d * m2->ty + m1->ty;
   m1->a = a;
   m1->b = b;
   m1->c = c;
   m1->d = d;
   m1->tx = tx;
   m1->ty = ty;
}

void rotateGTransform(GTransform matrix, double theta) {
   struct GTransformCDT m2;
   m2.a = cosDegrees(theta);
   m2.b = -sinDegrees(theta);
   m2.c = -m2.b;
   m2.d = m2.a;
   m2.tx = 0;
   m2.ty = 0;
   appendGTransform(matrix, &m2);
}

void scaleGTransform(GTransform matrix, double sx, double sy) {
   struct GTransformCDT m2;
   m2.a = sx;
   m2.b = 0;
   m2.c = 0;
   m2.d = sy;
   m2.tx = 0;
   m2.ty = 0;
   appendGTransform(matrix, &m2);
}

void translateGTransform(GTransform matrix, double dx, double dy) {
   struct GTransformCDT m2;
   m2.a = 1;
   m2.b = 0;
   m2.c = 0;
   m2.d = 1;
   m2.tx = dx;
   m2.ty = dy;
   appendGTransform(matrix, &m2);
}

GPoint applyGTransform(GTransform matrix, GPoint p1) {
   return createGPoint(matrix->a * p1.x + matrix->c * p1.y + matrix->tx,
                       matrix->b * p1.x + matrix->d * p1.y + matrix->ty);
}

GTransform invertGTransform(GTransform m1) {
   GTransform m2;
   double det;

   det = m1->a * m1->d - m1->c * m1->b;
   if (det == 0) error("invertGTransform: Uninvertible matrix");
   m2 = newBlock(GTransform);
   m2->a = m1->d / det;
   m2->b = -m1->b / det;
   m2->c = -m1->c / det;
   m2->d = m1->a / det;
   m2->tx = -(m1->d * m1->tx - m1->c * m1->ty) / det;
   m2->ty = (m1->b * m1->tx - m1->a * m1->ty) / det;
   return m2;
}

string toStringGTransform(GTransform matrix) {
   StringBuffer sb;
   string result;

   sb = newStringBuffer();
   sbprintf(sb, "{%g, %g, %g, %g, %g, %g}",
                fabs(matrix->a) < EPSILON ? 0 : matrix->a,
                fabs(matrix->b) < EPSILON ? 0 : matrix->b,
                fabs(matrix->c) < EPSILON ? 0 : matrix->c,
                fabs(matrix->d) < EPSILON ? 0 : matrix->d,
                fabs(matrix->tx) < EPSILON ? 0 : matrix->tx,
                fabs(matrix->ty) < EPSILON ? 0 : matrix->ty);
   result = copyString(getString(sb));
   freeStringBuffer(sb);
   return result;
}

/**********************************************************************/
/* Unit test for the gtransform module                                */
/**********************************************************************/

#ifndef _NOTEST_

/* Unit test */

static void testTranslate(void);
static void testRotate(void);
static void testScale(void);
static void testInverse(void);

void testGTransformModule(void) {
   testTranslate();
   testRotate();
   testScale();
   testInverse();
}

void testTranslate(void) {
   GTransform m;
   GPoint p1, p2;

   trace(m = newGTransform());
   test(toString(m), "{1, 0, 0, 1, 0, 0}");
   trace(translateGTransform(m, 300, 100));
   test(toString(m), "{1, 0, 0, 1, 300, 100}");
   p1 = createGPoint(75, 50);
   p2 = applyGTransform(m, p1);
   test(toString(p2), "(375, 150)");
}

void testRotate(void) {
   GTransform m;

   trace(m = newGTransform());
   test(toString(m), "{1, 0, 0, 1, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{0.707107, -0.707107, 0.707107, 0.707107, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{0, -1, 1, 0, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{-0.707107, -0.707107, 0.707107, -0.707107, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{-1, 0, 0, -1, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{-0.707107, 0.707107, -0.707107, -0.707107, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{0, 1, -1, 0, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{0.707107, 0.707107, -0.707107, 0.707107, 0, 0}");
   trace(rotate(m, 45));
   test(toString(m), "{1, 0, 0, 1, 0, 0}");
}

void testScale(void) {
   GTransform m;
   GPoint p1, p2;

   trace(m = newGTransform());
   test(toString(m), "{1, 0, 0, 1, 0, 0}");
   trace(scale(m, 2, 0.5));
   test(toString(m), "{2, 0, 0, 0.5, 0, 0}");
   p1 = createGPoint(75, 50);
   p2 = applyGTransform(m, p1);
   test(toString(p2), "(150, 25)");
}

void testInverse(void) {
   GTransform m1, m2, m3;

   trace(m1 = newGTransform());
   trace(rotate(m1, 90));
   test(toString(m1), "{0, -1, 1, 0, 0, 0}");
   trace(m2 = invertGTransform(m1));
   test(toString(m2), "{0, 1, -1, 0, 0, 0}");
   trace(scale(m1, 2, 3));
   trace(translateGTransform(m1, 50, -25));
   test(toString(m1), "{0, -2, 3, 0, -75, -100}");
   trace(m2 = invertGTransform(m1));
   test(toString(m2), "{0, 0.333333, -0.5, 0, -50, 25}");
   trace(appendGTransform(m2, m1));
   test(toString(m2), "{1, 0, 0, 1, 0, 0}");
}

#endif
