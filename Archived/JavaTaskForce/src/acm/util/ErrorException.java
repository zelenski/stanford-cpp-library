/*
 * @(#)ErrorException.java   1.99.1 08/12/08
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

/* Class: ErrorException */
/**
 * This class allows errors to be reported in a consistent way.
 * Clients typically call
 *
 * <p><pre><code>
 * &nbsp;    throw new ErrorException(msg);
 * </code></pre>
 *
 * <p>Because <code>ErrorException</code> is a subclass of <code>RuntimeException</code>,
 * this exception need not be declared in <code>throws</code> clauses.
 */
public class ErrorException extends RuntimeException {

/* Constructor: ErrorException(msg) */
/**
 * Creates an <code>ErrorException</code> with the specified message.
 *
 * @usage throw new ErrorException(msg);
 * @param msg The error message to be reported
 */
	public ErrorException(String msg) {
		super(msg);
	}

/* Constructor: ErrorException(ex) */
/**
 * Creates an <code>ErrorException</code> using an existing exception.
 *
 * @usage throw new ErrorException(ex);
 * @param ex The exception to be reported
 */
	public ErrorException(Exception ex) {
		super(ex.getClass().getName() + ": " + ex.getMessage());
	}
}
