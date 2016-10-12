/*
 * GTextArea.java
 * 
 * @author Jeff Lutgen
 */

/*************************************************************************/
/* Stanford Portable Library (adapted from the ACM graphics library)     */
/* Copyright (C) 2014 by Eric Roberts <eroberts@cs.stanford.edu>         */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*************************************************************************/

package stanford.spl;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JScrollPane;
import javax.swing.JTextArea;

/**
 * The <code>GTextArea</code> class is a graphical object whose appearance
 * consists of a rectangular area that can display text.
 */

public class GTextArea extends GInteractor {
	/**
	 * The default font used to display text.  You can change the font by
	 * invoking the <a href="#setFont(Font)"><code>setFont</code></a> method.
	 */
	public static final Font DEFAULT_FONT = new Font("Monospaced", Font.PLAIN, 11);
	
	public static final Color DEFAULT_BG_COLOR = new Color(240, 240, 240); 

	/* Serial version UID */
	/**
	 * The serialization code for this class.  This value should be incremented
	 * whenever you change the structure of this class in an incompatible way,
	 * typically by adding a new instance variable.
	 */

	static final long serialVersionUID = 21L;

	/* Private instance variables */
	
	private JTextArea textArea;
	private JScrollPane scrollPane;



	/**
	 * Creates a new <code>GTextArea</code> object with its anchor point at the
	 * specified position.
	 *
	 * @param width The width of this <code>GTextArea</code> in pixels
	 * @param height The height of this <code>GTextArea</code> in pixels
	 */

	public GTextArea(double width, double height) {
		super (new JScrollPane());
		textArea = new JTextArea();
		scrollPane = (JScrollPane) getInteractor();
		scrollPane.setViewportView(textArea);
		setFont(DEFAULT_FONT);
		setSize(width, height); 
	}

	/**
	 * Sets the background color of this text area.
	 *
	 * @param color The background color to be used for this text area
	 */

	public void setBackgroundColor(Color color) {
		textArea.setBackground(color);
	}
	   
	/**
	 * Changes the font used in the <code>GTextArea</code>.
	 *
	 * @param font A <code>Font</code> object indicating the new font
	 */

	public void setFont(Font font) {
		textArea.setFont(font);
	}

	/**
	 * Changes the font used to display the <code>GTextArea</code> as specified by
	 * the string <code>str</code>, which is interpreted in the style of
	 * <code>Font.decode</code>.  The usual format of the font string is
	 *
	 *<pre>
	 *    family-style-size
	 *</pre>
	 *
	 * where both <code>style</code> and <code>size</code> are optional.
	 * If any of these parts are specified as an asterisk, the existing
	 * value is retained.
	 *
	 * @param str A <code>String</code> specifying the new font
	 */

	public void setFont(String str) {
		textArea.setFont(Font.decode(str));
	}

	/**
	 * Returns the font used by this <code>GTextArea</code>.
	 *
	 * @return The font in use by this object
	 */

	public Font getFont() {
		return textArea.getFont();
	}

	/**
	 * Changes the text displayed by this <code>GTextArea</code> object.
	 *
	 * @param str The new string to display
	 */

	public void setText(String str) {
		textArea.setText(str);
	}

	/**
	 * Returns the text displayed by this object, encoded in Base64.
	 *
	 * @return The text displayed by this object
	 */

	public String getText() {
		return Base64.encodeBytes(textArea.getText().getBytes());
	}

	/**
	 * Sets whether this text area is editable.
	 *
	 * @param flag true for editable
	 */

	public void setEditable(boolean flag) {
		textArea.setEditable(flag);
	}
	
	/**
	 * Checks to see whether a point is inside the object.
	 *
	 * @param x The x-coordinate of the point being tested
	 * @param y The y-coordinate of the point being tested
	 * @return <code>true</code> if the point (<code>x</code>,&nbsp;<code>y</code>)
	 *         is inside the object, and <code>false</code> otherwise
	 */

	@Override
	public boolean contains(double x, double y) {
		return textArea.contains((int) (x - getX()), (int) (y - getY()));
	}
}
