/*
 * @version 2016/05/10
 * - fixed bug with drawing at wrong location (paint vs paint2d)
 * @version 2016/05/05
 * - resynched with eroberts source
 * - alphabetized members
 * @version 2015/04/29
 * - added setText method to mimick less-standard setLabel
 */


/*
 * @(#)GLabel.java   1.99.1 08/12/08
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

package acm.graphics;

import acm.util.*;
import java.awt.*;
import java.awt.geom.*;

/**
 * The <code>GLabel</code> class is a graphical object whose appearance
 * consists of a text string.
 */
public class GLabel extends GObject {
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */
	static final long serialVersionUID = 1L;
	/**
	 * The default font used to display strings.  You can change the font by invoking
	 * the <a href="#setFont(Font)"><code>setFont</code></a> method.
	 */
	public static final Font DEFAULT_FONT = new Font("Default", Font.PLAIN, 12);

	private static final Component DUMMY_COMPONENT = MediaTools.getImageObserver();

	/* Private instance variables */
	private String label;
	private Font labelFont;

	/**
	 * Creates a new <code>GLabel</code> object initialized to contain the specified string.
	 *
	 * @param str The initial contents of the <code>GLabel</code>
	 * @usage GLabel glabel = new GLabel(str);
	 */
	public GLabel(String str) {
		this(str, 0, 0);
	}

	/**
	 * Creates a new <code>GLabel</code> object with its baseline origin at the
	 * specified position.
	 *
	 * @param str The initial contents of the <code>GLabel</code>
	 * @param x   The x-coordinate of the label origin
	 * @param y   The y-coordinate of the baseline for the label
	 * @usage GLabel glabel = new GLabel(str, x, y);
	 */
	public GLabel(String str, double x, double y) {
		label = str;
		setFont(DEFAULT_FONT);
		setLocation(x, y);
	}

	/**
	 * Returns true if this label contains the given x/y point.
	 *
	 * @param x the x-coordinate to check
	 * @param y the y-coordinate to check
	 * @return true if this label contains the given x/y point
	 */
	public boolean contains(double x, double y) {
		FontMetrics fm = getFontMetrics();
		Object obj = new java.awt.geom.Rectangle2D.Double(0.0,
				-fm.getAscent(), fm.stringWidth(label),
				fm.getHeight());
		AffineTransform affinetransform = getMatrix();
		if (affinetransform != null) {
			obj = affinetransform.createTransformedShape(((Shape) (obj)));
		}
		return ((Shape) (obj)).contains(x - getX(), y - getY());
	}

	/**
	 * Returns the distance this string extends above the baseline.
	 *
	 * @return The ascent of this string in pixels
	 * @usage double ascent = glabel.getAscent();
	 */
	public double getAscent() {
		return getFontMetrics().getAscent();
	}

	/**
	 * Returns a <code>GRectangle</code> that specifies the bounding box for the string.
	 *
	 * @return The bounding box for this object
	 * @usage GRectangle bounds = glabel.getBounds();
	 */
	public GRectangle getBounds() {
		FontMetrics fm = getFontMetrics();
		Object obj = new java.awt.geom.Rectangle2D.Double(0.0,
				-fm.getAscent(), fm.stringWidth(label),
				fm.getHeight());
		AffineTransform affinetransform = getMatrix();
		if (affinetransform != null) {
			obj = affinetransform.createTransformedShape(((Shape) (obj)));
		}
		java.awt.Rectangle rectangle = ((Shape) (obj)).getBounds();
		GRectangle bounds = new GRectangle(getX() + rectangle.getX(), getY()
				+ rectangle.getY(), rectangle.getWidth(), rectangle.getHeight());
		return bounds;
	}

	/**
	 * Returns the distance this string descends below the baseline.
	 *
	 * @return The descent of this string in pixels
	 * @usage double descent = glabel.getDescent();
	 */
	public double getDescent() {
		return getFontMetrics().getDescent();
	}

	/**
	 * Returns the font in which the <code>GLabel</code> is displayed.
	 *
	 * @return The font in use by this object
	 * @usage Font font = glabel.getFont();
	 */
	public Font getFont() {
		return labelFont;
	}

	/**
	 * Returns a <code>FontMetrics</code> object describing the dimensions of this string.
	 *
	 * @return A <code>FontMetrics</code> object describing the dimensions of this string
	 * @usage FontMetrics fm = glabel.getFontMetrics();
	 * @noshow
	 */
	public FontMetrics getFontMetrics() {
		Component comp = getComponent();
		if (comp == null) {
			comp = DUMMY_COMPONENT;
		}
		return comp.getFontMetrics(labelFont);
	}

	/**
	 * Returns the height of this string, as it appears on the display.
	 *
	 * @return The height of this string
	 * @usage double height = glabel.getHeight();
	 */
	public double getHeight() {
		return getFontMetrics().getHeight();
	}

	/**
	 * Returns the string displayed by this object.
	 * Equivalent to getText.
	 *
	 * @return The string displayed by this object
	 * @usage String str = glabel.getLabel();
	 */
	public String getLabel() {
		return label;
	}

	/**
	 * Returns the string displayed by this object.
	 * Equivalent to getLabel.
	 *
	 * @return The string displayed by this object
	 * @usage String str = glabel.getLabel();
	 */
	public String getText() {
		return getLabel();
	}

	/**
	 * Returns the width of this string, as it appears on the display.
	 *
	 * @return The width of this object
	 * @usage double width = glabel.getWidth();
	 */
	public double getWidth() {
		return getFontMetrics().stringWidth(label);
	}

	/**
	 * Implements the <code>paint2d</code> operation for this graphical object.  This method
	 * is not called directly by clients.
	 *
	 * @noshow
	 */
	public void paint2d(Graphics2D g) {
		g.setFont(labelFont);
		g.drawString(label, 0, 0);
	}

	/**
	 * Returns a string indicating the parameters of this object.
	 *
	 * @noshow
	 */
	public String paramString() {
		return super.paramString() + ", string=\"" + label + "\"";
	}

	/**
	 * Sets the bottom y-coordinate of this object to the given y value.
	 * Since labels use y-positioning relative to their bottom, not the top,
	 * this is equivalent to calling setY().
	 *
	 * @param bottomY The new bottom y-coordinate for the object
	 * @usage gobj.setBottomY(y);
	 */
	@Override
	public void setBottomY(double bottomY) {
		// we have to override this method because labels use y-positioning
		// relative to the bottom, not the top
		setLocation(getX(), bottomY);
	}

	/**
	 * Sets the central x/y-coordinates of this object to the given x/y values.
	 *
	 * @param centerX The new central x-coordinate for the object
	 * @param centerY The new central y-coordinate for the object
	 * @usage gobj.setCenterLocation(x, y);
	 */
	@Override
	public void setCenterLocation(double centerX, double centerY) {
		// we have to override this method because labels use y-positioning
		// relative to the bottom, not the top
		setLocation(centerX - getWidth() / 2, centerY + getHeight() / 2);
	}

	/**
	 * Sets the central y-coordinate of this object to the given y value.
	 *
	 * @param centerY The new central y-coordinate for the object
	 * @usage gobj.setCenterY(y);
	 */
	@Override
	public void setCenterY(double centerY) {
		// we have to override this method because labels use y-positioning
		// relative to the bottom, not the top
		setLocation(getX(), centerY + getHeight() / 2);
	}

	/**
	 * Changes the font used to display the <code>GLabel</code>.  This call will
	 * usually change the size of the displayed object and will therefore affect
	 * the result of calls to <a href="GObject.html#getSize()"><code>getSize</code></a>
	 * and <a href="GObject.html#getBounds()"><code>getBounds</code></a>.
	 *
	 * @param font A <code>Font</code> object indicating the new font
	 * @usage glabel.setFont(font);
	 */
	public void setFont(Font font) {
		labelFont = JTFTools.getStandardFont(font);
		repaint();
	}

	/**
	 * Changes the font used to display the <code>GLabel</code> as specified by
	 * the string <code>str</code>, which is interpreted in the style of
	 * <code>Font.decode</code>.  The usual format of the font string is
	 * <p>
	 * <p><pre><code>
	 * &nbsp;    <font face="serif;times"><i>family</i></font>-<font face="serif;times"><i>style</i></font>-<font face="serif;times"><i>size</i></font>
	 * <p>
	 * <p>where both <i>style</i> and <i>size</i> are optional.  If any of these
	 * parts are specified as an asterisk, the existing value is retained.
	 * <p>
	 * @usage glabel.setFont(str);
	 * @param str A <code>String</code> specifying the new font
	 */
	public void setFont(String str) {
		setFont(JTFTools.decodeFont(str, getFont()));
	}

	/**
	 * Changes the string stored within the <code>GLabel</code> object, so that
	 * a new text string appears on the display.
	 * Equivalent to setText.
	 *
	 * @param str The new string to display
	 * @usage glabel.setLabel(str);
	 */
	public void setLabel(String str) {
		label = str;
		repaint();
	}

	/**
	 * Changes the string stored within the <code>GLabel</code> object, so that
	 * a new text string appears on the display.
	 * Equivalent to setLabel.
	 *
	 * @param str The new string to display
	 * @usage glabel.setText(str);
	 */
	public void setText(String str) {
		setLabel(str);
	}

	/**
	 * Sets the top y-coordinate of this object to the given y value.
	 * Since labels use y-positioning relative to their bottom, not the top, this
	 * is equivalent to calling setY() and shifting downward by the label's height.
	 *
	 * @param bottomY The new top y-coordinate for the object
	 * @usage gobj.setBottomY(y);
	 */
	public void setTopY(double topY) {
		// we have to override this method because labels use y-positioning
		// relative to the bottom, not the top
		setLocation(getX(), topY + getHeight());
	}
}
