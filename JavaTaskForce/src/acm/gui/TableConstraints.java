/*
 * @(#)TableConstraints.java   1.99.1 08/12/08
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
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.gui;

import acm.util.*;
import java.awt.*;
import java.util.*;

/* Class: TableConstraints */
/**
 * This class specifies a set of constraints appropriate to a
 * <code>TableLayout</code> or <code>GridBagLayout</code>.  It has
 * the following advantages over the <code>GridBagConstraints</code>
 * class on which it is based:
 *
 * <p><ul>
 * <li>The constraints can be specified as an easily readable string
 *     instead of by initializing the individual fields.
 * <li>The class includes new <code>width</code> and <code>height</code>
 *     fields that can be used with <code>TableLayout</code> to specify
 *     minimum heights and widths for rows and columns.
 * <li>The class provides accessor methods (<code>getGridX</code>
 *     and so forth) so that clients can operate as if the fields
 *     are private.
 * <li>The class includes a <code>toString</code> method that
 *     displays nondefault values of the fields in a readable way.
 * </ul>
 *
 * <p>To create a <code>TableConstraints</code> object, use the
 * constructor with a string argument to set the fields of the
 * underlying <code>GridBagConstraints</code> object.  For example,
 * suppose you wanted to achieve the effect of the traditional code
 *
 * <p><pre><code>
 * &nbsp;    GridBagConstraints gbc = new GridBagConstraints();
 * &nbsp;    gbc.gridx = 2;
 * &nbsp;    gbc.gridy = 3;
 * &nbsp;    gbc.fill = GridBagConstraints.BOTH;
 * </code></pre>
 *
 * <p>Using <code>TableConstraints</code>, you can do all of this
 * with the constructor, as follows:
 *
 * <p><pre><code>
 * &nbsp;    new TableConstraints("gridx=2 gridy=3 fill=BOTH");
 * </code></pre>
 */
public class TableConstraints extends GridBagConstraints {

/* Field: width */
/**
 * Specifies the desired width of this cell.  The width of a column
 * is taken to be the maximum of the specified cell widths.  If this
 * field has its default value of 0, the width is taken from the
 * preferred size of the component.
 */
	public int width;

/* Field: height */
/**
 * Specifies the desired height of this cell.  The height of a row
 * is taken to be the maximum of the specified cell heights.  If this
 * field has its default value of 0, the height is taken from the
 * preferred size of the component.
 */
	public int height;

/* Constructor: TableConstraints() */
/**
 * Creates a new <code>TableConstraints</code> object with default
 * values for each of the fields.
 *
 * @usage TableConstraints constraints = new TableConstraints();
 */
	public TableConstraints() {
		this("");
	}

/* Constructor: TableConstraints(str) */
/**
 * Creates a new <code>TableConstraints</code> object whose components
 * are initialized according from the specified string.  Each field is
 * initialized by specifying a binding in the form
 *
 * <p><pre><code>
 * &nbsp;    <font face="serif;times"><i>key</i></font>=<font face="serif;times"><i>value</i></font>
 * </code></pre>
 *
 * <p>where <i>key</i> is the name of one of the public fields
 * in the <code>TableConstraints</code> class and <i>value</i> is the
 * corresponding value, which can be expressed either as an integer
 * or as one of the constant names appropriate to that field.  For
 * example, the string
 *
 * <p><pre><code>
 * &nbsp;    "width=20 fill=BOTH"
 * </code></pre>
 *
 * <p>would create a <code>TableConstraints</code> object whose
 * <code>width</code> field was set to 20 and whose <code>fill</code>
 * field was set the the constant <code>GridBagConstraints.BOTH</code>.
 *
 * <p>As a special case, the four elements of the <code>insets</code>
 * field can be set using the key names <code>left</code>, <code>right</code>,
 * <code>top</code>, and <code>bottom</code>.  Also, because the names
 * are more likely to indicate their purposes to novices, the HTML names
 * <code>rowspan</code> and <code>colspan</code> can be used in place
 * of <code>gridwidth</code> and <code>gridheight</code>.
 *
 * @usage TableConstraints constraints = new TableConstraints(str);
 * @param str The constraint string as a series of key/value pairs
 */
	public TableConstraints(String str) {
		this(new OptionTable(str.toLowerCase(), LEGAL_KEYS).getMap());
	}

/* Constructor: TableConstraints(map) */
/**
 * Creates a new <code>TableConstraints</code> object whose components
 * are the key/value pairs in the map.
 *
 * @usage TableConstraints constraints = new TableConstraints(map);
 * @param map A map containing the key/value pairs
 * @noshow
 */
	public TableConstraints(Map<String,String> map) {
		OptionTable constraintTable = new OptionTable(map);
		gridx = parseXYConstraint(constraintTable.getOption("gridx"));
		gridy = parseXYConstraint(constraintTable.getOption("gridy"));
		String rowSpan = constraintTable.getOption("gridwidth");
		if (rowSpan == null) rowSpan = constraintTable.getOption("rowspan");
		String colSpan = constraintTable.getOption("gridheight");
		if (colSpan == null) colSpan = constraintTable.getOption("colspan");
		gridwidth = parseSpanConstraint(rowSpan);
		gridheight = parseSpanConstraint(colSpan);
		fill = parseFillConstraint(constraintTable.getOption("fill"));
		anchor = parseAnchorConstraint(constraintTable.getOption("anchor"));
		ipadx = constraintTable.getIntOption("ipadx", 0);
		ipady = constraintTable.getIntOption("ipady", 0);
		weightx = constraintTable.getDoubleOption("weightx", 0.0);
		weighty = constraintTable.getDoubleOption("weighty", 0.0);
		insets.left = constraintTable.getIntOption("left", 0);
		insets.right = constraintTable.getIntOption("right", 0);
		insets.top = constraintTable.getIntOption("top", 0);
		insets.bottom = constraintTable.getIntOption("bottom", 0);
		width = constraintTable.getIntOption("width", -1);
		if (width == -1) {
			width = 0;
		} else {
			width += insets.left + insets.right;
		}
		height = constraintTable.getIntOption("height", -1);
		if (height == -1) {
			height = 0;
		} else {
			height += insets.top + insets.bottom;
		}
		if (gridwidth != 1 && width != 0) {
			throw new ErrorException("TableConstraints: Cannot specify both width and gridwidth");
		}
		if (gridheight != 1 && height != 0) {
			throw new ErrorException("TableConstraints: Cannot specify both height and gridheight");
		}
	}

/* Constructor: TableConstraints(gbc) */
/**
 * Creates a new <code>TableConstraints</code> object whose fields match those
 * of the specified <code>GridBagConstraints</code> object.  Clients will not
 * ordinarily need to call this version of the constructor.
 *
 * @usage TableConstraints constraints = new TableConstraints(gbc);
 * @param gbc The <code>GridBagConstraints</code> object to copy
 * @noshow
 */
	public TableConstraints(GridBagConstraints gbc) {
		gridx = gbc.gridx;
		gridy = gbc.gridy;
		gridwidth = gbc.gridwidth;
		gridheight = gbc.gridheight;
		fill = gbc.fill;
		anchor = gbc.anchor;
		ipadx = gbc.ipadx;
		ipady = gbc.ipady;
		weightx = gbc.weightx;
		weighty = gbc.weighty;
		insets.left = gbc.insets.left;
		insets.right = gbc.insets.right;
		insets.top = gbc.insets.top;
		insets.bottom = gbc.insets.bottom;
		if (gbc instanceof TableConstraints) {
			TableConstraints tc = (TableConstraints) gbc;
			width = tc.width;
			height = tc.height;
		}
	}

/* Method: getAnchor() */
/**
 * Returns the <code>anchor</code> field from the constraint.
 *
 * @usage int anchor = constraint.getAnchor();
 * @return The <code>anchor</code> field from the constraint
 */
	public int getAnchor() {
		return anchor;
	}

/* Method: getFill() */
/**
 * Returns the <code>fill</code> field from the constraint.
 *
 * @usage int fill = constraint.getFill();
 * @return The <code>fill</code> field from the constraint
 */
	public int getFill() {
		return fill;
	}

/* Method: getGridX() */
/**
 * Returns the <code>gridx</code> field from the constraint.
 *
 * @usage int gridx = constraint.getGridX();
 * @return The <code>gridx</code> field from the constraint
 */
	public int getGridX() {
		return gridx;
	}

/* Method: getGridY() */
/**
 * Returns the <code>gridy</code> field from the constraint.
 *
 * @usage int gridy = constraint.getGridY();
 * @return The <code>gridy</code> field from the constraint
 */
	public int getGridY() {
		return gridy;
	}

/* Method: getGridWidth() */
/**
 * Returns the <code>gridwidth</code> field from the constraint.
 *
 * @usage int gridwidth = constraint.getGridWidth();
 * @return The <code>gridwidth</code> field from the constraint
 */
	public int getGridWidth() {
		return gridwidth;
	}

/* Method: getGridHeight() */
/**
 * Returns the <code>gridheight</code> field from the constraint.
 *
 * @usage int gridheight = constraint.getGridHeight();
 * @return The <code>gridheight</code> field from the constraint
 */
	public int getGridHeight() {
		return gridheight;
	}

/* Method: getIPadX() */
/**
 * Returns the <code>ipadx</code> field from the constraint.
 *
 * @usage int ipadx = constraint.getIPadX();
 * @return The <code>ipadx</code> field from the constraint
 */
	public int getIPadX() {
		return ipadx;
	}

/* Method: getIPadY() */
/**
 * Returns the <code>ipady</code> field from the constraint.
 *
 * @usage int ipady = constraint.getIPadY();
 * @return The <code>ipady</code> field from the constraint
 */
	public int getIPadY() {
		return ipady;
	}

/* Method: getInsets() */
/**
 * Returns the <code>insets</code> field from the constraint.
 *
 * @usage Insets insets = constraint.getInsets();
 * @return The <code>insets</code> field from the constraint
 */
	public Insets getInsets() {
		return insets;
	}

/* Method: getWeightX() */
/**
 * Returns the <code>weightx</code> field from the constraint.
 *
 * @usage double weightx = constraint.getWeightX();
 * @return The <code>weightx</code> field from the constraint
 */
	public double getWeightX() {
		return weightx;
	}

/* Method: getWeightY() */
/**
 * Returns the <code>weighty</code> field from the constraint.
 *
 * @usage double weighty = constraint.getWeightY();
 * @return The <code>weighty</code> field from the constraint
 */
	public double getWeightY() {
		return weighty;
	}

/* Method: getWidth() */
/**
 * Returns the <code>width</code> field from the constraint.
 *
 * @usage int width = constraint.getWidth();
 * @return The <code>width</code> field from the constraint
 */
	public int getWidth() {
		return width;
	}

/* Method: getHeight() */
/**
 * Returns the <code>height</code> field from the constraint.
 *
 * @usage int height = constraint.getHeight();
 * @return The <code>height</code> field from the constraint
 */
	public int getHeight() {
		return height;
	}

/* Override method: toString() */
/**
 * Converts the constraint into a readable string.
 *
 * @usage String str = constraint.toString();
 * @return A readable string version of the constraint
 * @noshow
 */
	public String toString() {
		String str = getClass().getName();
		str += "[gridx=" + gridx + ",gridy=" + gridy;
		switch (fill) {
		  case VERTICAL: str += ",fill=VERTICAL"; break;
		  case HORIZONTAL: str += ",fill=HORIZONTAL"; break;
		  case BOTH: str += ",fill=BOTH"; break;
		}
		switch (anchor) {
		  case NORTH: str += ",anchor=NORTH"; break;
		  case SOUTH: str += ",anchor=SOUTH"; break;
		  case EAST: str += ",anchor=EAST"; break;
		  case WEST: str += ",anchor=WEST"; break;
		  case NORTHEAST: str += ",anchor=NORTHEAST"; break;
		  case NORTHWEST: str += ",anchor=NORTHWEST"; break;
		  case SOUTHEAST: str += ",anchor=SOUTHEAST"; break;
		  case SOUTHWEST: str += ",anchor=SOUTHWEST"; break;
		  case MY_PAGE_START: str += ",anchor=PAGE_START"; break;
		  case MY_PAGE_END: str += ",anchor=PAGE_END"; break;
		  case MY_LINE_START: str += ",anchor=LINE_START"; break;
		  case MY_LINE_END: str += ",anchor=LINE_END"; break;
		  case MY_FIRST_LINE_START: str += ",anchor=FIRST_LINE_START"; break;
		  case MY_FIRST_LINE_END: str += ",anchor=FIRST_LINE_END"; break;
		  case MY_LAST_LINE_START: str += ",anchor=LAST_LINE_START"; break;
		  case MY_LAST_LINE_END: str += ",anchor=LAST_LINE_END"; break;
		}
		if (gridwidth != 1) str += ",gridwidth=" + gridwidth;
		if (gridheight != 1) str += ",gridheight=" + gridheight;
		if (ipadx != 0) str += ",ipadx=" + ipadx;
		if (ipady != 0) str += ",ipady=" + ipady;
		if (insets.left != 0) str += ",left=" + insets.left;
		if (insets.right != 0) str += ",right=" + insets.right;
		if (insets.top != 0) str += ",top=" + insets.top;
		if (insets.bottom != 0) str += ",bottom=" + insets.bottom;
		if (width != 0) str += ",width=" + width;
		if (height != 0) str += ",height=" + height;
		str += "]";
		return str;
	}

/* Private method: parseXYConstraint(str) */
/**
 * Reads the <code>gridx</code> and <code>gridy</code> values.
 */
	private int parseXYConstraint(String str) {
		if (str == null) return RELATIVE;
		if (str.equals("relative")) return RELATIVE;
		try {
			return Integer.decode(str).intValue();
		} catch (NumberFormatException ex) {
			throw new ErrorException("TableConstraints: Illegal grid coordinate");
		}
	}

/* Private method: parseSpanConstraint(str) */
/**
 * Reads the <code>gridwidth</code> and <code>gridheight</code> values.
 */
	private int parseSpanConstraint(String str) {
		if (str == null) return 1;
		if (str.equals("relative")) return RELATIVE;
		if (str.equals("remainder")) return REMAINDER;
		try {
			return Integer.decode(str).intValue();
		} catch (NumberFormatException ex) {
			throw new ErrorException("TableConstraints: Illegal span constraint");
		}
	}

/* Private method: parseAnchorConstraint(str) */
/**
 * Reads the value of an <code>anchor</code> constraint.
 */
	private int parseAnchorConstraint(String str) {
		if (str == null) return CENTER;
		if (str.equals("center")) return CENTER;
		if (str.equals("north")) return NORTH;
		if (str.equals("south")) return SOUTH;
		if (str.equals("east")) return EAST;
		if (str.equals("west")) return WEST;
		if (str.equals("northeast") || str.equals("ne")) return NORTHEAST;
		if (str.equals("northwest") || str.equals("nw")) return NORTHWEST;
		if (str.equals("southeast") || str.equals("se")) return SOUTHEAST;
		if (str.equals("southwest") || str.equals("sw")) return SOUTHWEST;
		if (str.equals("page_start")) return MY_PAGE_START;
		if (str.equals("page_end")) return MY_PAGE_END;
		if (str.equals("line_start")) return MY_LINE_START;
		if (str.equals("line_end")) return MY_LINE_END;
		if (str.equals("first_line_start")) return MY_FIRST_LINE_START;
		if (str.equals("first_line_end")) return MY_FIRST_LINE_END;
		if (str.equals("last_line_start")) return MY_LAST_LINE_START;
		if (str.equals("last_line_end")) return MY_LAST_LINE_END;
		throw new ErrorException("TableConstraints: Illegal anchor specification");
	}

/* Private method: parseFillConstraint(str) */
/**
 * Reads the value of an <code>fill</code> constraint.
 */
	private int parseFillConstraint(String str) {
		if (str == null || str.equals("none")) return NONE;
		if (str.equals("horizontal")) return HORIZONTAL;
		if (str.equals("vertical")) return VERTICAL;
		if (str.equals("both")) return BOTH;
		throw new ErrorException("TableConstraints: Illegal fill specification");
	}

/* Constants that allow this file to be compiled with old libraries */
	private static final int MY_PAGE_START = 19;
	private static final int MY_PAGE_END = 20;
	private static final int MY_LINE_START = 21;
	private static final int MY_LINE_END = 22;
	private static final int MY_FIRST_LINE_START = 23;
	private static final int MY_FIRST_LINE_END = 24;
	private static final int MY_LAST_LINE_START = 25;
	private static final int MY_LAST_LINE_END = 26;

/* Array of legal keys */
	protected static final String[] LEGAL_KEYS = {
		"anchor",
		"bottom",
		"colspan",
		"fill",
		"gridwidth",
		"gridheight",
		"gridx",
		"gridy",
		"height",
		"ipadx",
		"ipady",
		"left",
		"right",
		"rowspan",
		"top",
		"weightx",
		"weighty",
		"width"
	};

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
