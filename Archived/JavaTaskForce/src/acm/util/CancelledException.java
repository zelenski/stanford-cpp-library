/*
 * @(#)CancelledException.java   1.99.1 08/12/08
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

package acm.util;

/* Class: CancelledException */
/**
 * This class allows a dialog to signal clients that it has been
 * cancelled.  Because <code>CancelledException</code> is a subclass
 * of <code>RuntimeException</code>, this exception need not be declared
 * in <code>throws</code> clauses.
 */
public class CancelledException extends RuntimeException {

/* Constructor: CancelledException() */
/**
 * Creates an <code>CancelledException</code>.
 *
 * @usage throw new CancelledException();
 */
	public CancelledException() {
		/* Empty */
	}
}
