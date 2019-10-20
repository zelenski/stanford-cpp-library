/*
 * @version 2016/10/07
 * - added setLabel, setText overrides to fix broken C++ GLabel behavior
 */

package stanford.spl;

import acm.graphics.GLabel;
import java.awt.Color;
import java.awt.Font;
import javax.swing.JComponent;
import javax.swing.JLabel;

public class JBELabel extends GLabel {
	private JLabel jlabel;

	public JBELabel(String paramString) {
		super(paramString);
		this.jlabel = null;
	}

	public JComponent getInteractor() {
		if (this.jlabel == null) {
			this.jlabel = new JLabel(getLabel());
			this.jlabel.setFont(getFont());
			this.jlabel.setForeground(getColor());
		}
		return this.jlabel;
	}

	// JL: SwingUtilities.invokeLater
	public void setFont(Font paramFont) {
		super.setFont(paramFont);
		getInteractor();
		if (this.jlabel != null) {
			this.jlabel.setFont(paramFont);
		}
	}

	// JL: SwingUtilities.invokeLater
	public void setColor(Color paramColor) {
		super.setColor(paramColor);
		getInteractor();
		if (this.jlabel != null) {
			this.jlabel.setForeground(paramColor);
		}
	}
	
	// JL: SwingUtilities.invokeLater
	public void setLabel(String label) {
		super.setLabel(label);
		getInteractor();
		if (this.jlabel != null) {
			this.jlabel.setText(label);
		}
	}
	
	public void setText(String label) {
		setLabel(label);
	}
}
