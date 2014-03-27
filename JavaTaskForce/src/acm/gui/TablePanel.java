/*
 * @(#)TablePanel.java   1.99.1 08/12/08
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

import javax.swing.*;

/* Class: TablePanel */
/**
 * This class represents a simple <code>JPanel</code> that uses
 * <a href="TableLayout.html"><code>TableLayout</code></a> as its layout manager.
 * The purpose of <code>TablePanel</code> is to support tabular component
 * structures without the complexity of the <code>GridBagLayout</code> class.
 */
public class TablePanel extends JPanel {

/** Do not resize component */
	public static final int NONE = TableLayout.NONE;

/** Resize component in horizontal direction only */
	public static final int HORIZONTAL = TableLayout.HORIZONTAL;

/** Resize component in vertical direction only */
	public static final int VERTICAL = TableLayout.VERTICAL;

/** Resize component in both directions */
	public static final int BOTH = TableLayout.BOTH;

/** Center table in the container */
	public static final int CENTER = TableLayout.CENTER;

/** Align table horizontally at the left of its container */
	public static final int LEFT = TableLayout.LEFT;

/** Align table horizontally at the right of its container */
	public static final int RIGHT = TableLayout.RIGHT;

/** Align table vertically at the top of its container */
	public static final int TOP = TableLayout.TOP;

/** Align table vertically at the bottom of its container */
	public static final int BOTTOM = TableLayout.BOTTOM;

/** Expand table to fill its container */
	public static final int FILL = TableLayout.FILL;

/* Package private constructor: TablePanel() */
/**
 * Creates a new <code>TablePanel</code> without assigning a layout manager.
 * This constructor is used only within the <code>acm.gui</code> package.
 */
	TablePanel() {
		/* Empty */
	}

/* Constructor: TablePanel(rows, columns) */
/**
 * Creates a new <code>TablePanel</code> whose layout manager supports the
 * specified number of rows and columns.
 *
 * @usage TablePanel panel = new TablePanel(rows, columns);
 * @param rows The number of rows, or 0 for no limit
 * @param columns The number of columns, or 0 for no limit
 */
	public TablePanel(int rows, int columns) {
		this(rows, columns, 0, 0);
	}

/* Constructor: TablePanel(rows, columns, hgap, vgap) */
/**
 * Creates a new <code>TablePanel</code> with the  specified number
 * of rows and columns and the supplied values for the horizontal and
 * vertical gap.
 *
 * @usage TablePanel panel = new TablePanel(rows, columns, hgap, vgap);
 * @param rows The number of rows, or 0 for no limit
 * @param columns The number of columns, or 0 for no limit
 * @param hgap The gap between columns
 * @param vgap The gap between rows
 */
	public TablePanel(int rows, int columns, int hgap, int vgap) {
		setLayout(new TableLayout(rows, columns, hgap, vgap));
	}

/* Method: setHorizontalAlignment(align) */
/**
 * Sets the horizontal alignment for the table.  The legal values
 * are <code>CENTER</code>, <code>LEFT</code>, <code>RIGHT</code>, and
 * <code>FILL</code>.
 *
 * @usage layout.setHorizontalAlignment(align);
 * @param align The horizontal alignment for the table
 */
	public void setHorizontalAlignment(int align) {
		((TableLayout) getLayout()).setHorizontalAlignment(align);
	}

/* Method: getHorizontalAlignment() */
/**
 * Returns the horizontal alignment for the table.
 *
 * @usage align = layout.getHorizontalAlignment();
 * @return The horizontal alignment for the table
 */
	public int getHorizontalAlignment() {
		return ((TableLayout) getLayout()).getHorizontalAlignment();
	}

/* Method: setVerticalAlignment(align) */
/**
 * Sets the vertical alignment for the table.  The legal values
 * are <code>CENTER</code>, <code>TOP</code>, <code>BOTTOM</code>, and
 * <code>FILL</code>.
 *
 * @usage layout.setVerticalAlignment(align);
 * @param align The vertical alignment for the table
 */
	public void setVerticalAlignment(int align) {
		((TableLayout) getLayout()).setVerticalAlignment(align);
	}

/* Method: getVerticalAlignment() */
/**
 * Returns the vertical alignment for the table.
 *
 * @usage align = layout.getVerticalAlignment();
 * @return The vertical alignment for the table
 */
	public int getVerticalAlignment() {
		return ((TableLayout) getLayout()).getVerticalAlignment();
	}

/* Method: setDefaultFill(fill) */
/**
 * Sets the default fill parameter for components in the table.  The legal values
 * are <code>NONE</code>, <code>HORIZONTAL</code>, <code>VERTICAL</code>, and
 * <code>BOTH</code>.
 *
 * @usage layout.setDefaultFill(fill);
 * @param fill The default fill parameter for components in the table
 */
	public void setDefaultFill(int fill) {
		((TableLayout) getLayout()).setDefaultFill(fill);
	}

/* Method: getDefaultFill() */
/**
 * Returns the default fill parameter for components in the table.
 *
 * @usage fill = layout.getDefaultFill();
 * @return The default fill parameter for components in the table
 */
	public int getDefaultFill() {
		return ((TableLayout) getLayout()).getDefaultFill();
	}

/* Method: setHgap(pixels) */
/**
 * Sets the horizontal gap between components.
 * @param pixels The gap between components in pixels
 */
	public void setHgap(int pixels) {
		((TableLayout) getLayout()).setHgap(pixels);
	}

/* Method: getHgap() */
/**
 * Returns the horizontal gap between components.
 * @return The horizontal gap between components
 */
	public int getHgap() {
		return ((TableLayout) getLayout()).getHgap();
	}

/* Method: setVgap(pixels) */
/**
 * Sets the vertical gap between components.
 * @param pixels The gap between components in pixels
 */
	public void setVgap(int pixels) {
		((TableLayout) getLayout()).setVgap(pixels);
	}

/* Method: getVgap() */
/**
 * Returns the vertical gap between components.
 * @return The vertical gap between components
 */
	public int getVgap() {
		return ((TableLayout) getLayout()).getVgap();
	}

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
