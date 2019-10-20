/*
 * @(#)VPanel.java   1.99.1 08/12/08
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

// REVISION HISTORY
//
// -- V2.0 --
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.gui;

/* Class: VPanel */
/**
 * This class represents a vertical <a href="TablePanel.html"><code>TablePanel</code></a>
 * with a single column.  It is used along with <a href="HPanel.html"><code>HPanel</code></a>
 * to assemble hierarchical collections of panels that can be partitioned into rows
 * and columns.
 */
public class VPanel extends TablePanel {

/* Constructor: VPanel() */
/**
 * Creates a new <code>VPanel</code>.  A <code>VPanel</code> uses a
 * <code>TableLayout</code> manager with a single column.
 *
 * @usage VPanel vbox = new VPanel();
 */
	public VPanel() {
		this(0, 0);
	}

/* Constructor: VPanel(hgap, vgap) */
/**
 * Creates a new <code>VPanel</code> with the specified <code>hgap</code>
 * and <code>vgap</code> parameters, which specify the padding between
 * elements.  For a <code>VPanel</code>, the <code>hgap</code> is used
 * to assign space to the left and right of the column.
 *
 * @usage VPanel vbox = new VPanel(hgap, vgap);
 * @param hgap The gap between columns
 * @param vgap The gap between rows
 */
	public VPanel(int hgap, int vgap) {
		TableLayout layout = new TableLayout(0, 1, hgap, vgap);
		layout.setHorizontalAlignment(TableLayout.FILL);
		layout.setVerticalAlignment(TableLayout.FILL);
		setLayout(layout);
	}

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
