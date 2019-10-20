/*
 * This file implements a Panel subclass that is useful for
 * creating a vertical assembly of components.  The details
 * of its operation are described in the HVLayout manager,
 * which is common to both HPanels and VPanels.
 * - 2015/03/31: Changed to use Swing graphical components.
 */

package stanford.karel;

import java.awt.*;
import javax.swing.*;

class VPanel extends JPanel {
	public VPanel() {
		setLayout(new HVLayout(HVLayout.VERTICAL));
		setOpaque(false);
	}

	public Component add(String constraint) {
		return add(constraint, null);
	}

	public Component add(Component comp) {
		return add("", comp);
	}

	public Component add(String constraint, Component comp) {
		if (comp == null)
			comp = new EmptyCanvas();
		return super.add(constraint, comp);
	}
}
