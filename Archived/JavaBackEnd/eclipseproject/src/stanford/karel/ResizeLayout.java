package stanford.karel;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Insets;
import java.awt.LayoutManager;

class ResizeLayout implements LayoutManager {
	private Component canvas;
	private Component hbar;
	private Component vbar;

	public void addLayoutComponent(String constraints, Component comp) {
		if (constraints.equals("canvas"))
			canvas = comp;
		if (constraints.equals("hbar"))
			hbar = comp;
		if (constraints.equals("vbar"))
			vbar = comp;
	}

	public void removeLayoutComponent(Component comp) {
		/* Empty */
	}

	public Dimension preferredLayoutSize(Container parent) {
		return minimumLayoutSize(parent);
	}

	public Dimension minimumLayoutSize(Container parent) {
		synchronized (parent.getTreeLock()) {
			Dimension csize = canvas.getPreferredSize();
			int hsize = hbar.getPreferredSize().height;
			int vsize = vbar.getPreferredSize().width;
			return new Dimension(csize.width + vsize + 1, csize.height + hsize
					+ 1);
		}
	}

	public void layoutContainer(Container parent) {
		synchronized (parent.getTreeLock()) {
			Dimension psize = parent.getSize();
			Insets insets = parent.getInsets();
			int x = insets.left;
			int y = insets.top;
			int width = psize.width - insets.left - insets.right;
			int height = psize.height - insets.top - insets.bottom;
			int hsize = hbar.getPreferredSize().height;
			int vsize = vbar.getPreferredSize().width;
			canvas.setBounds(x, y, width - vsize - 1, height - hsize - 1);
			hbar.setBounds(x, y + height - vsize, width - vsize - 1, hsize);
			vbar.setBounds(x + width - hsize, y, vsize, height - hsize - 1);
		}
	}
}
