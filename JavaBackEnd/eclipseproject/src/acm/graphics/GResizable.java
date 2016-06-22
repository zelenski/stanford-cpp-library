/*
 * @(#)GResizable.java   1.99.1 08/12/08
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

/* Interface: GResizable */
/**
 * Specifies the characteristics of a graphical object that supports the
 * <code>setSize</code> and <code>setBounds</code> methods.
 */
public interface GResizable {

/* Method: setSize(width, height) */
/**
 * Changes the size of this object to the specified width and height.
 *
 * @usage gobj.setSize(width, height);
 * @param width The new width of the object
 * @param height The new height of the object
 */
	public void setSize(double width, double height);

/* Method: setSize(size) */
/**
 * Changes the size of this object as specified by the <code>GDimension</code>
 * object.
 *
 * @usage gobj.setSize(size);
 * @param size A <code>GDimension</code> object specifying the new size
 */
	public void setSize(GDimension size);

/* Method: setBounds(x, y, width, height) */
/**
 * Changes the bounds of this object to the specified values.
 *
 * @usage gobj.setBounds(x, y, width, height);
 * @param x The new x-coordinate for the object
 * @param y The new y-coordinate for the object
 * @param width The new width of the object
 * @param height The new height of the object
 */
	public void setBounds(double x, double y, double width, double height);

/* Method: setBounds(bounds) */
/**
 * Changes the bounds of this object to the values from the specified
 * <code>GRectangle</code>.
 *
 * @usage gobj.setBounds(bounds);
 * @param bounds A <code>GRectangle</code> specifying the new bounds
 */
	public void setBounds(GRectangle bounds);

}
