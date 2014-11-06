package stanford.cs106.gui;

/*
 * Convenience methods for constructing various graphical components and
 * remembering the location of windows across multiple runs of a program.
 * 
 * author: Marty Stepp
 * version: 2014/05/04
 */

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;

public class GuiUtils2 {
	private GuiUtils2() {
		// empty; factory class shouldn't be instantiated
	}
	
	public static final String SETTINGS_FILENAME = "autograder-window-settings.sav";
	private static Properties props = new Properties();
	private static String tempDir = System.getProperty("java.io.tmpdir");
	
	public static JButton addButton(ActionListener listener, String text, char mnemonic, Container container) {
		JButton button = new JButton(text);
		if (mnemonic != '\0' && mnemonic != ' ') {
			button.setMnemonic(mnemonic);
		}
		button.addActionListener(listener);
		container.add(button);
		return button;
	}
	
	public static JButton addButton(ActionListener listener, String text, String actionCommand, char mnemonic, Container container) {
		JButton button = new JButton(text);
		button.setActionCommand(actionCommand);
		if (mnemonic != '\0' && mnemonic != ' ') {
			button.setMnemonic(mnemonic);
		}
		button.addActionListener(listener);
		container.add(button);
		return button;
	}
	
	public static JButton createButton(ActionListener listener, String text, char mnemonic) {
		JButton button = new JButton(text);
		if (mnemonic != '\0' && mnemonic != ' ') {
			button.setMnemonic(mnemonic);
		}
		button.addActionListener(listener);
		return button;
	}
	
	public static JButton createButton(ActionListener listener, String text, String actionCommand, char mnemonic) {
		JButton button = new JButton(text);
		button.setActionCommand(actionCommand);
		if (mnemonic != '\0' && mnemonic != ' ') {
			button.setMnemonic(mnemonic);
		}
		button.addActionListener(listener);
		return button;
	}
	
	public static JLabel createLabel(String text, int width) {
		return createLabel(text, width, /* rightAligned */ false);
	}
	
	public static JLabel createLabel(String text, int width, boolean rightAligned) {
		JLabel label = new JLabel(text);
		Dimension size = label.getPreferredSize();
		if (size.width < width) {
			size.width = width;
		}
		if (rightAligned) {
			label.setHorizontalAlignment(SwingConstants.RIGHT);
		}
		label.setPreferredSize(size);
		return label;
	}
	
	public static void rememberWindowLocation(final Frame window) {
		window.addComponentListener(new WindowSettingsComponentAdapter());
		loadWindowLocation(window);
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
	
	public static void loadWindowLocation(Frame window) {
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
