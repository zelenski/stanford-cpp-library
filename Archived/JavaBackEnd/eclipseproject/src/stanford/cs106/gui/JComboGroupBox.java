/*
 * @version 2016/05/26
 */

package stanford.cs106.gui;

import java.awt.*;
import javax.swing.*;

/**
 * A JComboBox that allows group tags, like the HTML optgroup tag.
 */
@SuppressWarnings({ "unchecked", "rawtypes" })
public class JComboGroupBox extends JComboBox {

	public JComboGroupBox() {
		setModel(new ExtendedComboBoxModel());
		setRenderer(new ExtendedListCellRenderer());
	}

	public void addDelimiter(String text) {
		this.addItem(new Delimiter(text));
	}
	
	public void addItem(String item) {
		super.addItem(item);
	}
	
	public String getItemText(int index) {
		return getItemAt(index).toString();
	}
	
	public boolean isDelimiter(int index) {
		return getItemAt(index) instanceof Delimiter;
	}

	private static class ExtendedComboBoxModel extends DefaultComboBoxModel {
		@Override
		public void setSelectedItem(Object anObject) {
			if (!(anObject instanceof Delimiter)) {
				super.setSelectedItem(anObject);
			} else {
				int index = getIndexOf(anObject);
				if (index < getSize()) {
					setSelectedItem(getElementAt(index + 1));
				}
			}
		}

	}

	private static class ExtendedListCellRenderer extends DefaultListCellRenderer {

		@Override
		public Component getListCellRendererComponent(JList list, Object value, int index, boolean isSelected,
				boolean cellHasFocus) {
			if (!(value instanceof Delimiter)) {
				return super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
			} else {
				JLabel label = new JLabel(value.toString());
				Font f = label.getFont();
				label.setFont(f.deriveFont(f.getStyle() | Font.BOLD | Font.ITALIC));
				Dimension size = label.getPreferredSize();
				size.height += 5;
				label.setPreferredSize(size);
				return label;
			}
		}
	}

	private static class Delimiter {
		private String text;

		private Delimiter(String text) {
			this.text = text;
		}

		@Override
		public String toString() {
			return text.toString();
		}
	}
}
