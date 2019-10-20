/*
 * @(#)GScalable.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

package acm.graphics;

/* Interface: GScalable */
/**
 * Specifies the characteristics of a graphical object that supports the
 * <code>scale</code> method.
 */
public interface GScalable {

/* Method: scale(sx, sy) */
/**
 * Scales the object on the screen by the scale factors <code>sx</code> and <code>sy</code>.
 *
 * @usage gobj.scale(sx, sy);
 * @param sx The factor used to scale all coordinates in the x direction
 * @param sy The factor used to scale all coordinates in the y direction
 */
	public void scale(double sx, double sy);

/* Method: scale(sf) */
/**
 * Scales the object on the screen by the scale factor <code>sf</code>, which applies
 * in both dimensions.
 *
 * @usage gobj.scale(sf);
 * @param sf The factor used to scale all coordinates in both dimensions
 */
	public void scale(double sf);

}
