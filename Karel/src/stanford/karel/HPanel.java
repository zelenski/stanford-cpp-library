/*
 * This file implements a CSPanel subclass that is useful for
 * creating a horizontal assembly of components.  The details
 * of its operation are described in the HVLayout manager,
 * which is common to both HPanels and VPanels.
 * - 2015/03/31: Changed to use Swing graphical components.
 */

package stanford.karel;

import java.awt.*;
import javax.swing.*;

class HPanel extends JPanel {
	public HPanel() {
		setLayout(new HVLayout(HVLayout.HORIZONTAL));
		setOpaque(false);
	}

	public Component add(Component comp) {
		return add("", comp);
	}

	public Component add(String constraint) {
		return add(constraint, null);
	}

	public Component add(String constraint, Component comp) {
		if (comp == null)
			comp = new EmptyCanvas();
		return super.add(constraint, comp);
	}
}
