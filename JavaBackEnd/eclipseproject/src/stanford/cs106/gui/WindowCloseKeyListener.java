package stanford.cs106.gui;

import java.awt.*;
import java.awt.event.*;

/**
 * Makes a given window close itself when you press Escape or Ctrl-W.
 */
public class WindowCloseKeyListener implements KeyListener {
	public static void add(Window window) {
		new WindowCloseKeyListener(window);
	}
	
	private Window window;
	
	public WindowCloseKeyListener(Window window) {
		this.window = window;
		GuiUtils.addKeyListenerRecursive(window, this);
	}
	
	public void keyPressed(KeyEvent e) {
		boolean ctrl = e.isControlDown() || e.isMetaDown();
		if (e.getKeyCode() == KeyEvent.VK_ESCAPE || 
				(ctrl && e.getKeyCode() == KeyEvent.VK_W)) {
			window.setVisible(false);
			// window.dispose();
		}
	}
	
	public void keyReleased(KeyEvent e) {
		// empty
	}
	
	public void keyTyped(KeyEvent e) {
		// empty
	}
}
