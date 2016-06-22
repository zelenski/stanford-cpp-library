/*
 * @version 2016/05/19
 * - initial version; for 16sp CS 106A
 */

package acm.gui;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;
import javax.swing.*;

public class JButtonGroup extends JPanel implements Iterable<JRadioButton> {
	private ButtonGroup group;
	private List<JRadioButton> buttons;
	private boolean vertical = false;
	
	// instance initializer
	{
		clear();
	}
	
	public JButtonGroup() {
		// empty
	}
	
	public JButtonGroup(String... items) {
		addAll(/* listener */ null, items);
	}
	
	public JButtonGroup(ActionListener listener, String... items) {
		addAll(listener, items);
	}
	
	public void addActionListener(ActionListener listener) {
		for (JRadioButton button : this) {
			boolean shouldAdd = true;
			for (ActionListener listener2 : button.getActionListeners()) {
				if (listener2 == listener) {
					shouldAdd = false;
					break;
				}
			}
			if (shouldAdd) {
				button.addActionListener(listener);
			}
		}
	}
	
	public ActionListener[] getActionListeners() {
		LinkedHashSet<ActionListener> list = new LinkedHashSet<ActionListener>();
		for (JRadioButton button : this) {
			for (ActionListener listener : button.getActionListeners()) {
				list.add(listener);
			}
		}
		return list.toArray(new ActionListener[0]);
	}
	
	public void removeActionListener(ActionListener listener) {
		for (JRadioButton button : this) {
			button.removeActionListener(listener);
		}
	}
	
	public void addAll(ActionListener listener, String... items) {
		for (String item : items) {
			AbstractButton jrb = add(item);
			if (listener != null) {
				jrb.addActionListener(listener);
			}
		}
	}
	
	@Override
	public Component add(Component comp) {
		if (comp instanceof JRadioButton) {
			add((JRadioButton) comp);
			return comp;
		} else {
			return super.add(comp);
		}
	}
	
	@Override
	public Component add(Component comp, int index) {
		return add(comp);
	}
	
	@Override
	public Component add(String name, Component comp) {
		return add(comp);
	}
	
	public void add(JRadioButton button) {
		if (isEmpty()) {
			button.setSelected(true);
		}
		group.add(button);
		super.add(button);
	}
	
	public AbstractButton add(String text) {
		JRadioButton jrb = new JRadioButton(text);
		add(jrb);
		return jrb;
	}
	
	public void clear() {
		super.removeAll();
		group = new ButtonGroup();
		buttons = new ArrayList<JRadioButton>();
	}
	
	public int getButtonCount() {
		return group.getButtonCount();
	}
	
	public JRadioButton getSelectedButton() {
		for (JRadioButton button : buttons) {
			if (button.isSelected()) {
				return button;
			}
		}
		return null;   // should not be reached
	}
	
	public String getSelectedText() {
		JRadioButton button = getSelectedButton();
		if (button == null) {
			return null;
		} else {
			return button.getText();
		}
	}
	
	public boolean isEmpty() {
		return getButtonCount() == 0;
	}
	
	public boolean isSelected(String text) {
		String selectedText = getSelectedText();
		return selectedText != null && selectedText.equals(text);
	}
	
	public boolean isVertical() {
		return vertical;
	}
	
	public Iterator<JRadioButton> iterator() {
		return Collections.unmodifiableList(buttons).iterator();
	}
	
	@Override
	public void remove(Component comp) {
		if (comp instanceof JRadioButton) {
			remove((JRadioButton) comp);
		} else {
			super.remove(comp);
		}
	}
	
	public void remove(JRadioButton button) {
		for (int i = 0; i < buttons.size(); i++) {
			JRadioButton jrb = buttons.get(i);
			if (button == jrb) {
				buttons.remove(i);
				group.remove(jrb);
				break;
			}
		}
	}
	
	public void remove(String text) {
		for (int i = 0; i < buttons.size(); i++) {
			JRadioButton button = buttons.get(i);
			if (button.getText().equals(text)) {
				buttons.remove(i);
				group.remove(button);
				break;
			}
		}
	}
	
	@Override
	public void removeAll() {
		clear();
	}
	
	public void setVertical(boolean vertical) {
		if (this.vertical != vertical) {
			if (vertical) {
				setLayout(new BoxLayout(this, BoxLayout.Y_AXIS));
			} else {
				setLayout(new FlowLayout());
			}
		}
		this.vertical = vertical;
	}
}
