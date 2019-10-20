/*
 * @author Marty Stepp
 * @version 2017/10/14
 * - use GraphicsUtils for antialiasing
 * @version 2016/10/21
 * - added getValueAsInt/Double, valueIsInt/Double
 * @version 2016/10/15
 * - initial version
 * 
 * Based on source code taken from:
 * http://stackoverflow.com/questions/16213836/java-swing-jtextfield-set-placeholder
 */

package acm.gui;

import java.awt.*;
import javax.swing.*;
import javax.swing.text.Document;

import acm.graphics.GraphicsUtils;
import stanford.cs106.util.StringUtils;

public class JPlaceholderTextField extends JTextField {
	private String placeholder;
	private Color placeholderColor;

	public JPlaceholderTextField() {
		// empty
	}

	public JPlaceholderTextField(final Document pDoc, final String pText, final int pColumns) {
		super(pDoc, pText, pColumns);
	}

	public JPlaceholderTextField(final int pColumns) {
		super(pColumns);
	}

	public JPlaceholderTextField(final String pText) {
		super(pText);
	}

	public JPlaceholderTextField(final String pText, final int pColumns) {
		super(pText, pColumns);
	}

	public String getPlaceholder() {
		return placeholder;
	}
	
	public double getValueAsDouble() {
		return Double.parseDouble(getText());
	}
	
	public int getValueAsInt() {
		return Integer.parseInt(getText());
	}
	
	public int getValueAsInteger() {
		return Integer.parseInt(getText());
	}

	@Override
	protected void paintComponent(final Graphics g) {
		super.paintComponent(g);

		if (placeholder == null || placeholder.length() == 0 || getText().length() > 0) {
			return;
		}
		
		if (placeholderColor == null) {
			placeholderColor = getDisabledTextColor().darker();
		}

		GraphicsUtils.setAntialiasing(g, true);
		g.setColor(placeholderColor);
		g.drawString(placeholder, getInsets().left, g.getFontMetrics().getMaxAscent() + getInsets().top);
	}

	public void setPlaceholder(final String s) {
		placeholder = s;
	}
	
	public void setValue(double value) {
		setText(String.valueOf(value));
	}
	
	public void setValue(int value) {
		setText(String.valueOf(value));
	}
	
	public void setValue(long value) {
		setText(String.valueOf(value));
	}
	
	public boolean valueIsDouble() {
		return StringUtils.stringIsDouble(getText());
	}
	
	public boolean valueIsInt() {
		return StringUtils.stringIsInteger(getText());
	}
	
	public boolean valueIsInteger() {
		return StringUtils.stringIsInteger(getText());
	}
}
