package stanford.cs106.gui;

import java.awt.*;
import java.awt.event.*;

public class WindowCloseKeyListener implements KeyListener {
	
	private Window window;
	
	public WindowCloseKeyListener(Window window) {
		this.window = window;
		addKeyListenerRecursive(window);
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
	
	private void addKeyListenerRecursive(Component component) {
		if (component.isFocusable() || component instanceof Window) {
			// javax.swing.JOptionPane.showMessageDialog(null, "attaching key listener to: " + component);
			component.addKeyListener(this);
		}
		if (component instanceof Container) {
			Container container = (Container) component;
			for (Component subcomponent : container.getComponents()) {
				addKeyListenerRecursive(subcomponent);
			}
		}
	}
}
