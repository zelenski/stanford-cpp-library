package stanford.spl;

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;

public class SPLWindowSettings {
	public static final String SETTINGS_FILENAME = "spl-window-settings.sav";
	private static Properties props = new Properties();
	
	public static void saveWindowLocation(final Window window) {
		window.addComponentListener(new SPLComponentAdapter());
	}
	
	public static void forgetWindowLocation(final Window window) {
		for (ComponentListener listener : window.getComponentListeners()) {
			if (listener instanceof SPLComponentAdapter) {
				window.removeComponentListener(listener);
				String title = getTitle(window);
				props.remove(title + "-x");
				props.remove(title + "-y");
				props.remove(title + "-w");
				props.remove(title + "-h");
			}
		}
	}
	
	public static void loadWindowLocation(Window window) {
		synchronized (props) {
			try {
				// String tempDir = System.getProperty("java.io.tmpdir");
				String tempDir = System.getProperty("user.dir");
				String settingsFile = tempDir + "/" + SETTINGS_FILENAME;
				InputStream input = new FileInputStream(settingsFile);
				props.load(input);
			}
			catch (IOException ioe) {
				// empty
			}
			catch (Exception e) {
				// empty
			}
		}
		String title = getTitle(window); 
		if (props.containsKey(title + "-x") && props.containsKey(title + "-y")) {
			int x = Integer.parseInt(props.getProperty(title + "-x"));
			int y = Integer.parseInt(props.getProperty(title + "-y"));
			window.setLocation(x, y);
		}
		if (props.containsKey(title + "-w") && props.containsKey(title + "-h")) {
			int w = Integer.parseInt(props.getProperty(title + "-w"));
			int h = Integer.parseInt(props.getProperty(title + "-h"));
			window.setSize(w, h);
		}
	}
	
	private static String getTitle(Window window) {
		if (window instanceof Frame) {
			return ((Frame) window).getTitle();
		} else if (window instanceof Dialog) {
			return ((Dialog) window).getTitle();
		} else {
			return "???";
		}
	}
	
	private static class SPLComponentAdapter extends ComponentAdapter {
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
					// String tempDir = System.getProperty("java.io.tmpdir");
					String tempDir = System.getProperty("user.dir");
					String settingsFile = tempDir + "/" + SETTINGS_FILENAME;
					OutputStream out = new FileOutputStream(settingsFile);
					props.store(out, "Stanford C++ library window location settings");
					out.close();
				}
				catch (IOException ioe) {}
				catch (Exception e) {}
			}
		}
	}
}