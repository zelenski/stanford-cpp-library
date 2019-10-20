/*
 * File: gtransform.h
 * ------------------
 * This interface defines the GTransform type, which implements the
 * operations to support affine transformations.
 */

#ifndef _gtransform_h
#define _gtransform_h

#include "generic.h"
#include "gtypes.h"

/*
 * Type: GTransform
 * ----------------
 * This type represents an affine transformation.  An affine
 * transformation transforms coordinates in a way that preserves
 * any parallel lines.  In practice, affine transformations are
 * composed of rotation, scaling, and translation operations.
 */

typedef struct GTransformCDT *GTransform;

/*
 * Function: newGTransform
 * Usage: matrix = newGTransform();
 * --------------------------------
 * Returns a new identity transformation.
 */

GTransform newGTransform(void);

/*
 * Function: freeGTransform
 * Usage: freeGTransform(matrix);
 * ------------------------------
 * Frees the storage for the transformation matrix.
 */

void freeGTransform(GTransform matrix);

/*
 * Function: appendGTransform
 * Usage: appendGTransform(m1, m2);
 * --------------------------------
 * Changes transform <code>m1</code> by appending transform <code>m2</code>
 * to <code>m1</code>.
 */

void appendGTransform(GTransform m1, GTransform m2);

/*
 * Function: rotateGTransform
 * Usage: rotateGTransform(matrix, theta);
 * ---------------------------------------
 * Adds a transformation that rotates <code>matrix</code> by
 * <code>theta</code> degrees counterclockwise.
 */

void rotateGTransform(GTransform matrix, double theta);

/*
 * Function: scaleGTransform
 * Usage: scaleGTransform(matrix, sx, sy);
 * ---------------------------------------
 * Adds a transformation that scales <code>matrix</code> by
 * <code>sx</code> and <code>sy</code> relative to the coordinate axes.
 */

void scaleGTransform(GTransform matrix, double sx, double sy);

/*
 * Function: translateGTransform
 * Usage: translateGTransform(matrix, dx, dy);
 * -------------------------------------------
 * Adds a transformation that translates <code>matrix</code> by
 * <code>dx</code> and <code>dy</code> relative to the coordinate axes.
 */

void translateGTransform(GTransform matrix, double dx, double dy);

/*
 * Function: applyGTransform
 * Usage: p2 = applyGTransform(matrix, p1);
 * ----------------------------------------
 * Applies the specified transformation to the point <code>p1</code>
 * and returns the transformed point.
 */

GPoint applyGTransform(GTransform matrix, GPoint p1);

/*
 * Function: invertGTransform
 * Usage: m2 = invertGTransform(m1);
 * ---------------------------------
 * Returns a new <code>GTransform</code> value that is the inverse of
 * the transformation matrix <code>m1</code>.
 */

GTransform invertGTransform(GTransform m1);

/*
 * Function: toString
 * Usage: str = toString(matrix);
 * ------------------------------
 * Returns a string representation of the transformation matrix
 *
 *<pre>
 *     a   b
 *     c   d
 *     tx  ty
 *<pre>
 *
 * The elements of the matrix appear as a single six-element array
 * in which the elements are enclosed in curly braces and separated
 * by commas.
 */

string toStringGTransform(GTransform matrix);

#endif
