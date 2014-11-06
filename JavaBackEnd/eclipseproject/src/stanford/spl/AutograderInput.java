package stanford.spl;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
// import javax.swing.border.*;

public class AutograderInput extends Observable implements ActionListener {
	private static final int MIN_WIDTH = 75;
	private static AutograderInput instance;   // singleton
	
	public static synchronized AutograderInput getInstance() {
		if (instance == null) {
			instance = new AutograderInput();
		}
		return instance;
	}
	
	private JFrame frame;
	// private GridLayout layout;
	private JPanel currentCategory;
	private Map<String, JComponent> components;
	private Set<JLabel> allLabels = new LinkedHashSet<JLabel>();
	
	public AutograderInput() {
		frame = new JFrame();
		frame.setTitle("Autograder Input Panel");
		frame.setVisible(false);
		Box box = Box.createVerticalBox();
		frame.setContentPane(box);
		// layout = new GridLayout(0, 1, 5, 5);
		// frame.setLayout(layout);
		components = new HashMap<String, JComponent>();
		SPLWindowSettings.loadWindowLocation(frame);
		SPLWindowSettings.saveWindowLocation(frame);
	}
	
	public void addBlank() {
		addButton("");
	}
	
	public void addButton(String text) {
		addButton(text, text);
	}
	
	public void addButton(String text, String input) {
		if (currentCategory == null) {
			FlowLayout layout = new FlowLayout(FlowLayout.LEFT, /* hgap */ 1, /* vgap */ 2);
			currentCategory = new JPanel(layout);
			// frame.add(Box.createVerticalGlue());
			frame.add(currentCategory);
		}
		
		if (text.length() == 0) {
			currentCategory.add(new JPanel());   // blank slot
		} else {
			if (!input.endsWith("\n")) {
				input += "\n";
			}
			JButton button = new JButton(text);
			
			// shrink font so more stuff can fit
			shrinkFont(button);
			
			button.setActionCommand(input);
			button.addActionListener(this);
			currentCategory.add(button);
			components.put(text, button);
		}
		checkVisibility();
	}
	
	public void addCategory(String name) {
		currentCategory = new JPanel(new FlowLayout(FlowLayout.LEFT));
		// TitledBorder border = BorderFactory.createTitledBorder(name);
		// currentCategory.setBorder(border);
		
		JLabel label = new JLabel(name + (name != null && !name.isEmpty() ? ": " : ""));
		label.setFont(label.getFont().deriveFont(Font.BOLD));
		label.setHorizontalAlignment(JLabel.RIGHT);
		shrinkFont(label);
		allLabels.add(label);
		currentCategory.add(label);
		
		frame.add(currentCategory);
		components.put(name, currentCategory);
		checkVisibility();
	}
	
	public void actionPerformed(ActionEvent event) {
		setChanged();
		notifyObservers(event.getActionCommand());
	}
	
	public boolean isEmpty() {
		return buttonCount() == 0;
	}
	
	public int buttonCount() {
		return frame.getComponentCount();
	}
	
	public int getColumns() {
		// return layout.getColumns();
		return 1;
	}
	
	public void removeButton(String text) {
//		for (Component comp : frame.getComponents()) {
//			if (!(comp instanceof JButton)) continue;
//			JButton button = (JButton) comp;
//			if (button.getText().equals(text)) {
//				frame.remove(button);
//				break;
//			}
//		}
		JComponent button = components.get(text);
		if (button != null) {
			components.remove(text);
			frame.remove(button);
			checkVisibility();
		}
	}
	
	public void removeCategory(String name) {
		JComponent category = components.get(name);
		if (category != null) {
			components.remove(name);
			frame.remove(category);
			checkVisibility();
		}
	}
	
	public void setColumns(int columns) {
		// layout.setColumns(columns);
		checkVisibility();
	}
	
	public void setVisible(boolean value) {
		frame.setVisible(value);
	}
	
	private void checkVisibility() {
		frame.validate();
		frame.pack();
		
		// normalize size of all left-side labels
		int maxLabelWidth = 0;
		for (JLabel label : allLabels) {
			maxLabelWidth = Math.max(maxLabelWidth, label.getPreferredSize().width);
		}
		for (JLabel label : allLabels) {
			Dimension size = label.getPreferredSize();
			label.setPreferredSize(new Dimension(maxLabelWidth, size.height));
		}
		frame.validate();
		frame.pack();
		
		if (frame.getWidth() < MIN_WIDTH) {
			frame.setSize(MIN_WIDTH, frame.getHeight());
		}
		frame.setVisible(!isEmpty());
	}
	
	private void shrinkFont(JComponent button) {
		shrinkFont(button, 1);
	}
	
	private void shrinkFont(JComponent button, int amount) {
		Font font = button.getFont();
		font = font.deriveFont((float) (font.getSize() - amount));
		button.setFont(font);
	}
}
