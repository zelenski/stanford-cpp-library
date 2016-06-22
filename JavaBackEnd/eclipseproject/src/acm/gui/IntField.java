/*
 * @(#)IntField.java   1.99.1 08/12/08
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

/* Class: IntField */
/**
 * This class implements a simple interactor that accepts an integer value.
 */
public class IntField extends JTextField {

/* Constructor: IntField() */
/**
 * Creates a new field object for entering a <code>int</code> value.
 * The contents of the field are initially blank.
 *
 * @usage IntField field = new IntField();
 */
	public IntField() {
		this("", Integer.MIN_VALUE, Integer.MAX_VALUE);
	}

/* Constructor: IntField(value) */
/**
 * Creates a new field object for entering a <code>int</code> value.
 * The contents of the field initially contain the specified value.
 *
 * @usage IntField field = new IntField(value);
 * @param value The value to store in the field
 */
	public IntField(int value) {
		this("" + value, Integer.MIN_VALUE, Integer.MAX_VALUE);
	}

/* Constructor: IntField(low, high) */
/**
 * Creates a new field object for entering a <code>int</code> value,
 * which is constrained to be within the specified range. The contents
 * of the field are initially blank.
 *
 * @usage IntField field = new IntField(low, high);
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 */
	public IntField(int low, int high) {
		this("", low, high);
	}

/* Constructor: IntField(value, low, high) */
/**
 * Creates a new field object for entering a <code>int</code> value,
 * which is constrained to be within the specified range. The contents
 * of the field initially contain the specified value.
 *
 * @usage IntField field = new IntField(value, low, high);
 * @param value The value to store in the field
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 */
	public IntField(int value, int low, int high) {
		this("" + value, low, high);
	}

/* Private constructor: IntField(str, low, high) */
/**
 * Creates a new field object for entering a <code>int</code> value,
 * which is constrained to be within the specified range. The contents
 * of the field initially contain the specified string.
 */
	private IntField(String str, int low, int high) {
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
 * Returns the value of this field as a <code>int</code>.  If this value is either
 * not a legal numeric value or is outside the specified range, this method will
 * either pop up a dialog allowing the user to reenter the value or throw an
 * <code>ErrorException</code> depending on the state of the <code>exceptionOnError</code>
 * flag.
 *
 * @usage int n = field.getValue();
 * @return The value stored in the field as a <code>int</code>
 */
	public int getValue() {
		String text = getText().trim();
		if (text.length() == 0) {
			if (minValue <= 0 && maxValue >= 0) return 0;
			return minValue;
		}
		String msg = null;
		int value = 0;
		while (true) {
			try {
				value = parser.parse(text).intValue();
				if (value >= minValue && value <= maxValue) break;
				msg = "Value is outside the specified range";
			} catch (ParseException ex) {
				msg = "Illegal integer format";
			}
			if (exceptionOnError) {
				throw new ErrorException(msg);
			} else {
				String prompt = "Enter an integer";
				if (minValue != Integer.MIN_VALUE) {
					if (maxValue != Integer.MAX_VALUE) {
						prompt += " between " + minValue + " and " + maxValue;
					} else {
						prompt += " greater than " + minValue;
					}
				} else {
					if (maxValue != Integer.MAX_VALUE) {
						prompt += " less than " + maxValue;
					}
				}
				if (dialog == null) dialog = new IODialog(this);
				value = dialog.readInt(prompt, minValue, maxValue);
				break;
			}
		}
		setValue(value);
		return value;
	}

/* Method: setValue(n) */
/**
 * Sets the value of a field.
 *
 * @usage field.setValue(n);
 * @param n The value to be stored in the field
 */
	public void setValue(int n) {
		setText((formatter == null) ? defaultFormat(n) : formatter.format(n));
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
 * class.
 *
 * @usage field.setFormat(format);
 * @param format The format to use for the field
 */
	public void setFormat(String format) {
		String contents = getText().trim();
		int value = 0;
		if (contents.length() != 0) {
			try {
				value = parser.parse(contents).intValue();
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

/* Private method: defaultFormat(n) */
/**
 * This method formats the number if the formatter is <code>null</code>.
 * The only special case that applies occurs if the output is larger than
 * the field, in which case the entire display is replaced by number signs.
 */
	private String defaultFormat(int n) {
		String str = "" + n;
		int availableSpace = getSize().width - 2 * PIXEL_MARGIN;
		FontMetrics fm = getFontMetrics(getFont());
		if (fm.stringWidth(str) > availableSpace) {
			str = "";
			while (fm.stringWidth(str + "#") <= availableSpace) {
				str += "#";
			}
		}
		return str;
	}

/* Private constants */
	private static final int MINIMUM_WIDTH = 60;
	private static final int MINIMUM_HEIGHT = 22;
	private static final int PIXEL_MARGIN = 2;

/* Private instance variables */
	private boolean exceptionOnError;
	private int minValue;
	private int maxValue;
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
