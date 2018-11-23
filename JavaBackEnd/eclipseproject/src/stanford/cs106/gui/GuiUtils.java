/*
 * This class contains utility functions related to GUIs.
 *
 * @author Marty Stepp
 * @version 2018/07/08
 * - added ensureMinimumPreferred*
 * @version 2017/10/12
 * - added createLabel overloads with icon
 * @version 2016/11/26
 * - added getAncestor
 * @version 2016/05/26
 * - added createJComboGroupBox, setPreferred* methods
 * @version 2016/05/01
 * - added create*MenuItem methods
 * @version 2014/05/26
 * - added centerWindowWithin method
 * @version 2014/05/22
 * - added methods for creating radio buttons, sliders, etc.
 * @version 2014/06/05
 * - original version
 * 
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */


// TO DO: This file's documentation is insufficient due to time constraints.
// Improve documentation in future quarters for better style and maintainability.

package stanford.cs106.gui;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.ChangeListener;
import javax.swing.filechooser.FileFilter;
import acm.program.Program;
import stanford.cs106.io.*;
import stanford.spl.*;

public class GuiUtils {
	public static final String SETTINGS_FILENAME = "autograder-window-settings.sav";
	private static Properties props = new Properties();
	private static String tempDir = System.getProperty("java.io.tmpdir");
	
	public static void addKeyListenerRecursive(Component component, KeyListener listener) {
		if (component.isFocusable() || component instanceof Window) {
			component.addKeyListener(listener);
		}
		if (component instanceof Container) {
			Container container = (Container) component;
			for (Component subcomponent : container.getComponents()) {
				addKeyListenerRecursive(subcomponent, listener);
			}
		}
	}
	
	/**
	 * Sets up the given program so that its componentResized method will be
	 * called whenever the window resizes.
	 * The given program must implement the ResizeListener interface.
	 * @param program the program to listen to
	 * @throws ClassCastException if the given program does not implement the
	 *                            ResizeListener interface.
	 */
	public static void addResizeListener(Program program) {
		if (!(program instanceof ResizeListener)) {
			throw new ClassCastException("Your program class must implement the ResizeListener interface.");
		}
		final ResizeListener listener = (ResizeListener) program;
		program.addComponentListener(new ComponentAdapter() {
			public void componentResized(ComponentEvent event) {
				listener.componentResized(event);
			}
		});
	}
	
	public static void centerWindow(Window window) {
		if (window != null) {
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
			window.setLocation(screenSize.width/2 - window.getWidth()/2,
					screenSize.height/2 - window.getHeight()/2);
		}
	}

	public static void centerWindowWithin(Window window, Component parent) {
		if (window != null && parent != null) {
			window.setLocation(
					parent.getX() + (parent.getWidth() - window.getWidth()) / 2,
					parent.getY() + (parent.getHeight() - window.getHeight()) / 2);
		}
	}

	public static JButton createButton(String text, char mnemonic, ActionListener listener) {
		return createButton(text, null, null, mnemonic, listener, /* container */ null);
	}
	
	public static JButton createButton(String text, char mnemonic, ActionListener listener, Container container) {
		return createButton(text, null, null, mnemonic, listener, container);
	}
	
	public static JButton createButton(String text, String actionCommand, char mnemonic, ActionListener listener) {
		return createButton(text, actionCommand, null, mnemonic, listener, /* container */ null);
	}
	
	public static JButton createButton(String text, String actionCommand, char mnemonic, ActionListener listener, Container container) {
		return createButton(text, actionCommand, null, mnemonic, listener, container);
	}
	
	public static JButton createButton(String text, String actionCommand, String icon, char mnemonic, ActionListener listener) {
		return createButton(text, actionCommand, icon, mnemonic, listener, /* container */ null);
	}
		
	public static JButton createButton(String text, String actionCommand, String icon, char mnemonic, ActionListener listener, Container container) {
		JButton button = new JButton(text);
		if (actionCommand == null || actionCommand.isEmpty()) {
			actionCommand = text;
		}
		button.setActionCommand(actionCommand);
		
		if (icon != null && icon.length() > 0) {
			try {
				if (ResourceUtils.fileExists(icon)) {
					button.setIcon(new ImageIcon(ResourceUtils.filenameToURL(icon)));
				}
			} catch (Exception e) {
				try {
					button.setIcon(new ImageIcon(ResourceUtils.filenameToURL(icon)));
				} catch (IORuntimeException ioe) {
					// empty
				}
			}
		}
		
		if (mnemonic != '\0' && mnemonic != ' ') {
			button.setMnemonic(mnemonic);
		}
		
		if (listener != null) {
			button.addActionListener(listener);
		}
		
		if (container != null) {
			container.add(button);
		}
		
		return button;
	}
	
	public static ButtonGroup createButtonGroup(ActionListener listener, String... items) {
		ButtonGroup group = new ButtonGroup();
		String selected = null;
		for (String item : items) {
			JRadioButton jrb = new JRadioButton(item);
			if (selected == null) {
				jrb.setSelected(true);
				selected = item;
			}
			if (listener != null) {
				jrb.addActionListener(listener);
			}
			group.add(jrb);
		}
		return group;
	}
	
	public static JCheckBox createCheckBox(String actionCommand, ActionListener listener) {
		return createCheckBox(actionCommand, /* checked */ false, listener);
	}
	
	public static JCheckBox createCheckBox(String text, String actionCommand, char mnemonic, ActionListener listener) {
		return createCheckBox(text, actionCommand, mnemonic, /* checked */ false, listener);
	}
	
	public static JCheckBox createCheckBox(String actionCommand, boolean checked, ActionListener listener) {
		char mnemonic = (actionCommand != null && !actionCommand.isEmpty() ? actionCommand.charAt(0) : '\0');
		return createCheckBox(actionCommand, mnemonic, checked, listener);
	}
	
	public static JCheckBox createCheckBox(String actionCommand, char mnemonic, boolean checked) {
		return createCheckBox(actionCommand, mnemonic, checked, /* listener */ null);
	}
	
	public static JCheckBox createCheckBox(String actionCommand, char mnemonic, boolean checked, ActionListener listener) {
		return createCheckBox(actionCommand, mnemonic, checked, listener, /* container */ null);
	}
	
	public static JCheckBox createCheckBox(String text, String actionCommand, char mnemonic, boolean checked, ActionListener listener) {
		return createCheckBox(text, actionCommand, mnemonic, checked, listener, /* container */ null);
	}
	
	public static JCheckBox createCheckBox(String actionCommand, char mnemonic, ActionListener listener, Container container) {
		return createCheckBox(actionCommand, mnemonic, /* checked */ false, listener, container);
	}
	
	public static JCheckBox createCheckBox(String actionCommand, char mnemonic, boolean checked, ActionListener listener, Container container) {
		return createCheckBox(/* text */ actionCommand, actionCommand, mnemonic, checked, listener, container);
	}
	
	public static JCheckBox createCheckBox(String text, String actionCommand, char mnemonic, boolean checked, ActionListener listener, Container container) {
		JCheckBox box = new JCheckBox(actionCommand);
		box.setSelected(checked);
		if (mnemonic != '\0' && mnemonic != ' ') {
			box.setMnemonic(mnemonic);
		}
		if (listener != null) {
			box.addActionListener(listener);
		}
		if (container != null) {
			container.add(box);
		}
		return box;
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static JComboBox createComboBox(String actionCommand, ActionListener listener, String... items) {
		JComboBox box = new JComboBox(items);
		box.setEditable(false);
		if (listener != null) {
			box.addActionListener(listener);
			box.setActionCommand(actionCommand);
		}
		return box;
	}
	
	public static JComboGroupBox createComboGroupBox(String actionCommand, ActionListener listener, String... items) {
		JComboGroupBox box = new JComboGroupBox();
		for (String item : items) {
			if (item.startsWith("GROUP: ")) {
				box.addDelimiter(item.substring(7));
			} else if (item.startsWith("* ")) {
				box.addDelimiter(item.substring(2));
			} else if (item.startsWith("- ")) {
				box.addDelimiter(item.substring(2));
			} else {
				box.addItem(item);
			}
		}
		box.setEditable(false);
		if (listener != null) {
			box.addActionListener(listener);
			box.setActionCommand(actionCommand);
		}
		return box;
	}
	
	public static JMenu createMenu(String text, JMenuBar bar) {
		return createMenu(text, /* mnemonic */ text.charAt(0), bar);
	}
	
	public static JMenu createMenu(String text, char mnemonic, JMenuBar bar) {
		JMenu menu = new JMenu(text);
		if (mnemonic != '\0') {
			menu.setMnemonic(mnemonic);
		}
		if (bar != null) {
			bar.add(menu);
		}
		return menu;
	}
	
	public static JMenuItem createMenuItem(String text, ActionListener listener, JMenu menu) {
		return createMenuItem(text, /* mnemonic */ text.charAt(0), listener, menu);
	}
	
	public static JMenuItem createMenuItem(String text, char mnemonic, ActionListener listener, JMenu menu) {
		JMenuItem item = new JMenuItem(text);
		if (mnemonic != '\0') {
			item.setMnemonic(mnemonic);
		}
		if (listener != null) {
			item.addActionListener(listener);
		}
		if (menu != null) {
			menu.add(item);
		}
		return item;
	}
	
	public static JCheckBoxMenuItem createCheckBoxMenuItem(String text, ActionListener listener, JMenu menu) {
		return createCheckBoxMenuItem(text, /* mnemonic */ text.charAt(0), /* checked */ false, listener, menu);
	}
	
	public static JCheckBoxMenuItem createCheckBoxMenuItem(String text, boolean checked, ActionListener listener, JMenu menu) {
		return createCheckBoxMenuItem(text, /* mnemonic */ text.charAt(0), checked, listener, menu);
	}
	
	public static JCheckBoxMenuItem createCheckBoxMenuItem(String text, char mnemonic, boolean checked, ActionListener listener, JMenu menu) {
		JCheckBoxMenuItem item = new JCheckBoxMenuItem(text);
		if (mnemonic != '\0') {
			item.setMnemonic(mnemonic);
		}
		item.setSelected(checked);
		if (listener != null) {
			item.addActionListener(listener);
		}
		if (menu != null) {
			menu.add(item);
		}
		return item;
	}
	
	/*
	 * adds a JPanel to the layout
	 */
	public static JPanel createPanel(Component... components) {
		return createPanel(new FlowLayout(FlowLayout.CENTER), components);
	}
	
	/*
	 * adds a JPanel to the layout
	 */
	public static JPanel createPanel(LayoutManager layout, Component... components) {
		JPanel panel = new JPanel(layout);
		for (Component comp : components) {
			panel.add(comp);
		}
		return panel;
	}
	

	/* Helper method to create a JRadioButton with the given properties. */
	public static JRadioButton createRadioButton(String text, char mnemonic,
			boolean selected, ButtonGroup group, ActionListener listen,
			Container panel) {
		JRadioButton button = new JRadioButton(text, selected);
		if (mnemonic != '\0') {
			button.setMnemonic(mnemonic);
		}
		button.addActionListener(listen);
		if (panel != null) {
			panel.add(button);
		}
		if (group != null) {
			group.add(button);
		}
		return button;
	}
	
	/* Helper method to create a JSlider with the given properties. */
	public static JSlider createSlider(int min, int max, int initial,
			int majorTick, int minorTick, ChangeListener listen, Container panel) {
		JSlider slider = new JSlider(min, max, initial);
		slider.setMajorTickSpacing(majorTick);
		slider.setMinorTickSpacing(minorTick);
		slider.setSnapToTicks(true);
		slider.setPaintTicks(true);
		// slider.setPaintLabels(true);
		slider.addChangeListener(listen);
		if (panel != null) {
			panel.add(slider);
		}
		return slider;
	}
	
	public static void ensureMinimumPreferredHeight(JComponent comp, int height) {
		Dimension size = comp.getPreferredSize();
		size.height = Math.max(size.height, height);
		comp.setPreferredSize(size);
	}
	
	public static void ensureMinimumPreferredSize(JComponent comp, int width, int height) {
		Dimension size = comp.getPreferredSize();
		size.width  = Math.max(size.width,  width);
		size.height = Math.max(size.height, height);
		comp.setPreferredSize(size);
	}
	
	public static void ensureMinimumPreferredWidth(JComponent comp, int width) {
		Dimension size = comp.getPreferredSize();
		size.width = Math.max(size.width, width);
		comp.setPreferredSize(size);
	}
	
	public static FileFilter getExtensionFileFilter(String description, String... extensions) {
		ExtensionFileFilter filter = new ExtensionFileFilter(extensions);
		filter.setDescription(description);
		return filter;
	}
	
	public static JLabel createLabel(String text, String icon) {
		return createLabel(text, icon, /* width */ 0, /* rightAligned */ false);
	}
	
	public static JLabel createLabel(String text, int width) {
		return createLabel(text, /* icon */ null, width, /* rightAligned */ false);
	}
	
	public static JLabel createLabel(String text, int width, boolean rightAligned) {
		return createLabel(text, /* icon */ null, width, rightAligned);
	}
	
	public static JLabel createLabel(String text, String icon, int width) {
		return createLabel(text, icon, width, /* rightAligned */ false);
	}
	
	public static JLabel createLabel(String text, String icon, int width, boolean rightAligned) {
		JLabel label = new JLabel(text);
		Dimension size = label.getPreferredSize();
		if (width > 0 && size.width < width) {
			size.width = width;
		}
		if (rightAligned) {
			label.setHorizontalAlignment(SwingConstants.RIGHT);
		}
		if (width > 0) {
			label.setPreferredSize(size);
		}
		if (icon != null && icon.length() > 0) {
			try {
				if (ResourceUtils.fileExists(icon)) {
					label.setIcon(new ImageIcon(ResourceUtils.filenameToURL(icon)));
				}
			} catch (Exception e) {
				try {
					label.setIcon(new ImageIcon(ResourceUtils.filenameToURL(icon)));
				} catch (IORuntimeException ioe) {
					// empty
				}
			}
		}
		return label;
	}
	
	/**
	 * Looks upward at parent containers of the given component until it finds one of the given type or
	 * any subclass of the given type, and returns it.  If none found, returns null.
	 */
	@SuppressWarnings("unchecked")
	public static <C extends Component> C getAncestor(Component comp, Class<C> type) {
		if (comp == null) {
			return null;
		}
		while (true) {
			comp = comp.getParent();
			if (comp == null) {
				break;
			}
			if (type.isAssignableFrom(comp.getClass())) {
				return (C) comp;
			}
		}
		return null;
	}
	
	/**
	 * Returns first found descendent of given type within the given container; null if not found.
	 */
	public static <JC extends JComponent> JC getDescendent(Container container, Class<JC> type) {
		Set<JC> descendents = getDescendents(container, type);
		if (descendents.isEmpty()) {
			return null;
		} else {
			for (JC jc : descendents) {
				return jc;
			}
			return null;
		}
	}
	
	@SuppressWarnings("unchecked")
	public static <JC extends JComponent> Set<JC> getDescendents(Container container, Class<JC> type) {
		if (container == null) {
			return null;
		}
		Set<JC> results = new LinkedHashSet<JC>();
		for (Component component : container.getComponents()) {
			if (component instanceof JComponent) {
				if (type == null || type == component.getClass() || type.isAssignableFrom(component.getClass())) {
					results.add((JC) component);
				}
			}
			if (component instanceof Container) {
				Set<JC> sub = getDescendents((Container) component, type);
				results.addAll(sub);
			}
		}
		
		if (container instanceof Program && results.isEmpty()) {
			Set<JC> sub = getDescendents(((Program) container).getWindow(), type);
			results.addAll(sub);
		}
		return results;
	}
	
	public static void errorDialog(String text) {
		errorDialog(/* comp */ null, text);
	}
	
	public static void errorDialog(Component comp, String text) {
		JOptionPane.showMessageDialog(comp, text, "Error", JOptionPane.ERROR_MESSAGE);
	}
		
	public static void errorDialog(Component comp, String text, Throwable th) {
		JOptionPane.showMessageDialog(comp, text + ": " + th.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
	}
		
	public static void errorDialog(Throwable th) {
		errorDialog(/* comp */ null, th);
	}
	
	public static void errorDialog(Component comp, Throwable th) {
		JOptionPane.showMessageDialog(comp, th.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
	}
		
	public static Icon extractOptionPaneIcon(String text) {
		JOptionPane opt = new JOptionPane("message", JOptionPane.INFORMATION_MESSAGE);
		return extractHelper(opt, text);
	}
	
	public static void forgetWindowLocation(final Frame window) {
		for (ComponentListener listener : window.getComponentListeners()) {
			if (listener instanceof WindowSettingsComponentAdapter) {
				window.removeComponentListener(listener);
				String title = window.getTitle();
				props.remove(title + "-x");
				props.remove(title + "-y");
				props.remove(title + "-w");
				props.remove(title + "-h");
			}
		}
	}
	
	public static void growFont(JComponent button) {
		growFont(button, 1);
	}
	
	public static void growFont(JComponent button, int amount) {
		Font font = button.getFont();
		font = font.deriveFont((float) (font.getSize() + amount));
		button.setFont(font);
	}
	
	public static String htmlLabelText(String text) {
		return htmlLabelText(text, /* props */ Collections.emptyMap());
	}
	
	private static String toPxUnit(Object ostr) {
		String str = String.valueOf(ostr);
		if (!str.endsWith("px") && !str.endsWith("pt") && !str.endsWith("em")) {
			str += "px";
		}
		return str;
	}
	
	public static String htmlLabelText(String text, Map<?, ?> props) {
		String bodyStyle = "";
		if (props.containsKey("width")) {
			Object width = toPxUnit(props.get("width"));
			bodyStyle += " width: " + width + ";";
		}
		if (props.containsKey("max-width")) {
			Object maxWidth = toPxUnit(props.get("max-width"));
			bodyStyle += " max-width: " + maxWidth + ";";
		}
		if (props.containsKey("height")) {
			Object height = toPxUnit(props.get("height"));
			bodyStyle += " height: " + height + ";";
		}
		if (props.containsKey("max-height")) {
			Object maxHeight = toPxUnit(props.get("max-height"));
			bodyStyle += " max-height: " + maxHeight + ";";
		}
		if (props.containsKey("monospace") || props.containsKey("monospaced")) {
			bodyStyle += " font-family: monospaced;";
		} else if (props.containsKey("font-family")) {
			Object fontFamily = props.get("font-family");
			bodyStyle += " font-family: \"" + fontFamily + "\";";
		} else if (props.containsKey("font")) {
			Font font = (Font) props.get("font");
			bodyStyle += " font-family: \"" + font.getName() + "\";";
			bodyStyle += " font-size: " + font.getSize() + "pt;";
			if (font.isBold()) {
				bodyStyle += " font-weight: bold;";
			}
			if (font.isItalic()) {
				bodyStyle += " font-style: italic;";
			}
		}
		String htmlText = "<html><body style='" + bodyStyle.trim() + "'>";   // close body tag
		
		String before = "";
		String after = "";
		if (props.containsKey("font-color")) {
			Object color = props.get("font-color");
			before += "<font color=\"" + color + "\">";
			after = "</font>" + after;
		}
		if (props.containsKey("center")) {
			before += "<center>";
			after = "</center>" + after;
		}
		htmlText += before + text + after;
		
		htmlText += "</body></html>";
		return htmlText;
	}
	
	public static void loadWindowLocation(Frame window) {
		if (window == null) {
			return;
		}
		synchronized (props) {
			try {
				String settingsFile = tempDir + "/" + SETTINGS_FILENAME;
				if (new File(settingsFile).exists()) {
					InputStream input = new FileInputStream(settingsFile);
					props.load(input);
				}
			} catch (IOException ioe) {
				System.err.println("I/O error trying to load window settings: " + ioe);
			} catch (Exception e) {
				System.err.println("Error trying to save window settings: " + e);
			}
		}
		String title = window.getTitle(); 
		if (props.containsKey(title + "-x") && props.containsKey(title + "-y")) {
			int x = Integer.parseInt(props.getProperty(title + "-x"));
			int y = Integer.parseInt(props.getProperty(title + "-y"));
			window.setLocation(x, y);
			// System.out.println("Loaded location of window \"" + window.getTitle() + "\".");
		}
		if (props.containsKey(title + "-w") && props.containsKey(title + "-h")) {
			int w = Integer.parseInt(props.getProperty(title + "-w"));
			int h = Integer.parseInt(props.getProperty(title + "-h"));
			window.setSize(w, h);
			// System.out.println("Loaded size of window \"" + window.getTitle() + "\".");
		}
	}
	
	public static void pad(JComponent component, int w, int h) {
		Dimension size = component.getPreferredSize();
		size.width += w;
		size.height += h;
		component.setPreferredSize(size);
	}
	
	public static void printComponentHierarchy(Component comp) {
		printComponentHierarchy(comp, /* indent */ "");
	}
	
	private static void printComponentHierarchy(Component comp, String indent) {
		if (comp == null) {
			return;
		}
		
		Dimension size = comp.getSize();
		Dimension prefSize = comp.getPreferredSize();
		System.out.println(indent
				+ comp.getClass().getName()
				+ "@" + comp.hashCode()
				+ ", size=" + size.width + "x" + size.height
				+ ", psize=" + prefSize.width + "x" + prefSize.height
				+ ", vis?" + comp.isVisible()
				+ ", show?" + comp.isShowing()
		);
		if (comp instanceof Container) {
			Container cont = (Container) comp;
			for (int i = 0; i < cont.getComponentCount(); i++) {
				printComponentHierarchy(cont.getComponent(i), indent + "  ");
			}
		}
	}
	
	public static void rememberWindowLocation(final Frame window) {
		if (window != null) {
			window.addComponentListener(new WindowSettingsComponentAdapter());
			loadWindowLocation(window);
		}
	}
	
	public static void setSystemLookAndFeel() {
		try {
			String lnf = UIManager.getSystemLookAndFeelClassName();
			if (lnf == null || lnf.contains("MetalLookAndFeel")) {
				// workaround because system L&F seems to fail on Linux boxes
				UIManager.setLookAndFeel("com.sun.java.swing.plaf.gtk.GTKLookAndFeel");
			} else {
				UIManager.setLookAndFeel(lnf);
			}
			
			GSlider.setSystemLookAndFeelProperties();
			GTable.setSystemLookAndFeelProperties();
			System.setProperty("apple.laf.useScreenMenuBar", "true");
		} catch (Exception e) {
			// empty
		}
	}
	
	public static void setPreferredWidth(JComponent comp, int width) {
		Dimension size = comp.getPreferredSize();
		size.width = width;
		comp.setPreferredSize(size);
	}
	
	public static void setPreferredHeight(JComponent comp, int height) {
		Dimension size = comp.getPreferredSize();
		size.height = height;
		comp.setPreferredSize(size);
	}
	
	public static void shiftPreferredSize(JComponent comp, int dw, int dh) {
		Dimension size = comp.getPreferredSize();
		size.width += dw;
		size.height += dh;
		comp.setPreferredSize(size);
	}
	
	public static void shrinkFont(JComponent button) {
		shrinkFont(button, 1);
	}
	
	public static void shrinkFont(JComponent button, int amount) {
		Font font = button.getFont();
		font = font.deriveFont((float) (font.getSize() - amount));
		button.setFont(font);
	}

	public static void heighten(JComponent component, int px) {
		pad(component, 0, px);
	}
	
	public static void widen(JComponent component, int px) {
		pad(component, px, 0);
	}
	
	private GuiUtils() {
		// empty
	}
	
	private static Icon extractHelper(Component comp, String text) {
		if (comp instanceof JButton) {
			JButton button = (JButton) comp;
			String buttonText = String.valueOf(button.getText());
			if (buttonText.toUpperCase().contains(text.toUpperCase())) {
				return button.getIcon();
			}
		} else if (comp instanceof JLabel) {
			JLabel label = (JLabel) comp;
			String labelText = String.valueOf(label.getText());
			if (labelText.toUpperCase().contains(text.toUpperCase())) {
				return label.getIcon();
			}
		} else if (comp instanceof Container) {
			for (Component subcomp : ((Container) comp).getComponents()) {
				Icon icon = extractHelper(subcomp, text);
				if (icon != null) {
					return icon;
				}
			}
		}
		return null;
	}
	
	private static class WindowSettingsComponentAdapter extends ComponentAdapter {
		public void componentMoved(ComponentEvent event) {
			Component component = event.getComponent();
			if (!(component instanceof Frame)) return;
			Frame window = (Frame) component; 
			int x = window.getX();
			int y = window.getY();
			int w = window.getWidth();
			int h = window.getHeight();
			synchronized (props) {
				props.setProperty(window.getTitle() + "-x", String.valueOf(x));
				props.setProperty(window.getTitle() + "-y", String.valueOf(y));
				props.setProperty(window.getTitle() + "-w", String.valueOf(w));
				props.setProperty(window.getTitle() + "-h", String.valueOf(h));
				try {
					// String tempDir = System.getProperty("user.dir");
					String settingsFile = tempDir + "/" + SETTINGS_FILENAME;
					OutputStream out = new FileOutputStream(settingsFile);
					props.store(out, "Stanford Autograder window location settings");
					out.close();
					// System.out.println("Saved size/location of window \"" + window.getTitle() + "\".");
				} catch (IOException ioe) {
					System.err.println("I/O error trying to save window settings: " + ioe);
				} catch (Exception e) {
					System.err.println("Error trying to save window settings: " + e);
				}
			}
		}
	}
}
