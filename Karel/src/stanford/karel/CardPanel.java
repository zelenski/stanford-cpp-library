/*
 * This class defines a simple card layout panel that hides
 * the layout manager.
 * - 2015/03/31: Changed to use Swing graphical components.
 */

package stanford.karel;

import java.awt.*;
import javax.swing.*;

class CardPanel extends JPanel {
	private CardLayout layout;
	private String currentView;

	/*
	 * This constructor creates a panel instance with a card layout that can be
	 * manipulated directly from the panel object by calling setView.
	 */
	public CardPanel() {
		layout = new CardLayout();
		setLayout(layout);
		setOpaque(false);
	}

	/*
	 * These methods allow clients to set and get the name of the currently
	 * displayed view.
	 */
	public void setView(String name) {
		validate();
		currentView = name;
		layout.show(this, name);
		repaint();
	}

	public String getView() {
		return currentView;
	}
}
