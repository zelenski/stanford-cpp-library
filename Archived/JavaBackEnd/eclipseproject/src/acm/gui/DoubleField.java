/*
 * @(#)DoubleField.java   1.99.1 08/12/08
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
// Bug fix 2-Mar-07 (ESR, JTFBug 2007-004)
//   1. Fixed bug in the formatting code that allowed the display to
//      extend beyond the boundaries of the component.
//
// Bug fix 6-May-07 (ESR, JTFBug 2007-006)
//   1. Fixed various bugs in the localization of numbers.
//
// Feature enhancement 26-May-08 (ESR)
//   1. Added support for serialization.

package acm.gui;

import acm.io.*;
import acm.util.*;
import java.awt.*;
import java.text.*;
import java.util.*;
import javax.swing.*;

/* Class: DoubleField */
/**
 * This class implements a simple interactor that accepts a floating-point value.
 */
public class DoubleField extends JTextField {

/* Constructor: DoubleField() */
/**
 * Creates a new field object for entering a <code>double</code> value.
 * The contents of the field are initially blank.
 *
 * @usage DoubleField field = new DoubleField();
 */
	public DoubleField() {
		this("", Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
	}

/* Constructor: DoubleField(value) */
/**
 * Creates a new field object for entering a <code>double</code> value.
 * The contents of the field initially contain the specified value.
 *
 * @usage DoubleField field = new DoubleField(value);
 * @param value The value to store in the field
 */
	public DoubleField(double value) {
		this("" + value, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
	}

/* Constructor: DoubleField(low, high) */
/**
 * Creates a new field object for entering a <code>double</code> value,
 * which is constrained to be within the specified range. The contents
 * of the field are initially blank.
 *
 * @usage DoubleField field = new DoubleField(low, high);
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 */
	public DoubleField(double low, double high) {
		this("", low, high);
	}

/* Constructor: DoubleField(value, low, high) */
/**
 * Creates a new field object for entering a <code>double</code> value,
 * which is constrained to be within the specified range. The contents
 * of the field initially contain the specified value.
 *
 * @usage DoubleField field = new DoubleField(value, low, high);
 * @param value The value to store in the field
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 */
	public DoubleField(double value, double low, double high) {
		this("" + value, low, high);
	}

/* Private constructor: DoubleField(str, low, high) */
/**
 * Creates a new field object for entering a <code>double</code> value,
 * which is constrained to be within the specified range. The contents
 * of the field initially contain the specified string.
 */
	private DoubleField(String str, double low, double high) {
		parser = NumberFormat.getNumberInstance(Locale.US);
		setBackground(Color.WHITE);
		setHorizontalAlignment(RIGHT);
		minValue = low;
		maxValue = high;
		setText(str);
		exceptionOnError = false;
	}

/* Method: getValue() */
/**
 * Returns the value of this field as a <code>double</code>.  If this value is either
 * not a legal numeric value or is outside the specified range, this method will
 * either pop up a dialog allowing the user to reenter the value or throw an
 * <code>ErrorException</code> depending on the state of the <code>exceptionOnError</code>
 * flag.
 *
 * @usage double d = field.getValue();
 * @return The value stored in the field as a <code>double</code>
 */
	public double getValue() {
		String text = getText().trim();
		if (text.length() == 0) {
			if (minValue <= 0 && maxValue >= 0) return 0.0;
			return minValue;
		}
		String msg = null;
		double value = 0.0;
		while (true) {
			try {
				value = parser.parse(text).doubleValue();
				if (value >= minValue && value <= maxValue) break;
				msg = "Value is outside the specified range";
			} catch (ParseException ex) {
				msg = "Illegal numeric format";
			}
			if (exceptionOnError) {
				throw new ErrorException(msg);
			} else {
				String prompt = "Enter a number";
				if (minValue != Double.NEGATIVE_INFINITY) {
					if (maxValue != Double.POSITIVE_INFINITY) {
						prompt += " between " + minValue + " and " + maxValue;
					} else {
						prompt += " greater than " + minValue;
					}
				} else {
					if (maxValue != Double.POSITIVE_INFINITY) {
						prompt += " less than " + maxValue;
					}
				}
				if (dialog == null) dialog = new IODialog(this);
				value = dialog.readDouble(prompt, minValue, maxValue);
				break;
			}
		}
		setValue(value);
		return value;
	}

/* Method: setValue(d) */
/**
 * Sets the value of a field.
 *
 * @usage field.setValue(d);
 * @param d The value to be stored in the field
 */
	public void setValue(double d) {
		setText((formatter == null) ? defaultFormat(d) : formatter.format(d));
	}

/* Method: getFormat() */
/**
 * Returns the format currently in use for this field, or
 * <code>null</code> if no format has been set.
 *
 * @usage String format = field.getFormat();
 * @return The format for this field
 */
	public String getFormat() {
		return formatString;
	}

/* Method: setFormat(format) */
/**
 * Sets the format used for the field.  The structure of the
 * format string is described in the comments for the DecimalFormat
 * class.  If a format is set for the field, the format will also be
 * used to read the number to ensure that localization is correctly
 * handled.
 *
 * @usage field.setFormat(format);
 * @param format The format to use for the field
 */
	public void setFormat(String format) {
		String contents = getText().trim();
		double value = 0;
		if (contents.length() != 0) {
			try {
				value = parser.parse(contents).doubleValue();
			} catch (ParseException ex) {
				throw new ErrorException(ex);
			}
		}
		formatString = format;
		if (format == null) {
			formatter = null;
			parser = NumberFormat.getNumberInstance(Locale.US);
		} else {
			formatter = (format.length() == 0) ? new DecimalFormat() : new DecimalFormat(format);
			parser = formatter;
		}
		if (contents.length() != 0) {
			setValue(value);
		}
	}

/* Method: setExceptionOnError(flag) */
/**
 * Sets the error-handling mode of this interactor as specified by the <code>flag</code>
 * parameter.  If <code>flag</code> is <code>false</code> (which is the default),
 * calling <a href="#getValue()"><code>getValue</code></a> on this interactor displays
 * a dialog that gives the user a chance to retry after erroneous input.  If this
 * value is set to <code>true</code>, illegal input raises an
 * <a href="../util/ErrorException.html"><code>ErrorException</code></a> instead.
 *
 * @usage field.setExceptionOnError(flag);
 * @param flag <code>false</code> to retry on errors; <code>true</code> to raise an exception
 */
	public void setExceptionOnError(boolean flag) {
		exceptionOnError = flag;
	}

/* Method: getExceptionOnError() */
/**
 * Returns the state of the error-handling flag.
 *
 * @usage boolean flag = console.getExceptionOnError();
 * @return The current setting of the error-handling mode (<code>false</code> to retry
 *         on errors; <code>true</code> to raise an exception)
 */
	public boolean getExceptionOnError() {
		return exceptionOnError;
	}

/* Override method: getPreferredSize() */
/**
 * Overrides the standard definition to impose a target size.
 * @noshow
 */
	public Dimension getPreferredSize() {
		Dimension size = super.getPreferredSize();
		return new Dimension(Math.max(MINIMUM_WIDTH, size.width),
		                     Math.max(MINIMUM_HEIGHT, size.height));
	}

/* Private method: defaultFormat(d) */
/**
 * This method formats the number if the formatter is <code>null</code>.
 * The code attempts to fit the value into the field.  All the work comes
 * from having to round off the digits that are shifted out of the field.
 */
	private String defaultFormat(double d) {
		String str = "" + d;
		int availableSpace = getSize().width - 2 * PIXEL_MARGIN;
		FontMetrics fm = getFontMetrics(getFont());
		if (fm.stringWidth(str) <= availableSpace) return str;
		int eIndex = str.indexOf("E");
		String suffix = "";
		if (eIndex != -1) {
			suffix = str.substring(eIndex);
			str = str.substring(0, eIndex);
			try {
				d = parser.parse(str).doubleValue();
			} catch (ParseException ex) {
				throw new ErrorException(ex);
			}
		}
		NumberFormat standard = NumberFormat.getNumberInstance(Locale.US);
		standard.setGroupingUsed(false);
		String head = str.substring(0, str.indexOf('.') + 1);
		int fractionSpace = availableSpace - fm.stringWidth(head + suffix);
		if (fractionSpace > 0) {
			int fractionDigits = fractionSpace / fm.stringWidth("0");
			standard.setMaximumFractionDigits(fractionDigits);
			return standard.format(d) + suffix;
		}
		str = "";
		while (fm.stringWidth(str + "#") <= availableSpace) {
			str += "#";
		}
		return str;
	}

/* Private constants */
	private static final int MINIMUM_WIDTH = 60;
	private static final int MINIMUM_HEIGHT = 22;
	private static final int PIXEL_MARGIN = 2;

/* Private instance variables */
	private boolean exceptionOnError;
	private double minValue;
	private double maxValue;
	private String formatString;
	private DecimalFormat formatter;
	private NumberFormat parser;
	private IODialog dialog;

/* Serial version UID */
/**
 * The serialization code for this class.  This value should be incremented
 * whenever you change the structure of this class in an incompatible way,
 * typically by adding a new instance variable.
 */
	static final long serialVersionUID = 1L;
}
