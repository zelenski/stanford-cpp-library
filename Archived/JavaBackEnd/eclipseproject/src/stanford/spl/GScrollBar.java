/*
 * @version 2018/07/16
 * - initial version
 */

package stanford.spl;

import java.awt.Dimension;
import java.awt.event.*;
import javax.swing.*;
import stanford.cs106.gui.GuiUtils;

public class GScrollBar extends GInteractor {
	public GScrollBar() {
		super(new JScrollBar());
	}
	
	public GScrollBar(int orientation) {
		super(new JScrollBar(orientation));
	}
	
	public GScrollBar(int orientation, int value, int extent, int min, int max) {
		super(makeScrollBar(orientation, value, extent, min, max));
		updatePreferredSize(getInteractor());
		Dimension prefSize = getInteractor().getPreferredSize();
		setSize(prefSize.width, prefSize.height);
	}
	
	public void addAdjustmentListener(AdjustmentListener listener) {
		getInteractor().addAdjustmentListener(listener);
	}
	
	public JScrollBar getInteractor() {
		return (JScrollBar) super.getInteractor();
	}
	
	public int getValue() {
		return getInteractor().getValue();
	}
	
	public void setValues(int value, int extent, int min, int max) {
		getInteractor().setValues(value, extent, min, max);
		updatePreferredSize(getInteractor());
	}
	
	private static JScrollBar makeScrollBar(int orientation, int value, int extent, int min, int max) {
		final JScrollBar scrollBar = new JScrollBar(orientation, value, extent, min, max) {
			@Override
			public Dimension getPreferredSize() {
				int range = getMaximum() - getMinimum() + 1;
				if (getOrientation() == JScrollBar.HORIZONTAL) {
					return new Dimension(range, super.getPreferredSize().height);
				} else {
					// vertical
					return new Dimension(super.getPreferredSize().width, range);
				}
			}
			
			@Override
			public Dimension getMinimumSize() {
				return getPreferredSize();
			}
//			
//			@Override
//			public void setSize(int width, int height) {
//				JOptionPane.showMessageDialog(null, "setSize(" + width + "x" + height + "):\n" + ExceptionUtils.stackTraceToString());
//			}
//			
//			@Override
//			public void setSize(Dimension size) {
//				JOptionPane.showMessageDialog(null, "setSize(" + size + "):\n" + ExceptionUtils.stackTraceToString());
//			}
		};
		scrollBar.setBackground(java.awt.Color.RED);
		scrollBar.addMouseWheelListener(new MouseWheelListener() {
			public void mouseWheelMoved(MouseWheelEvent event) {
				int rotation = event.getWheelRotation();
				if (rotation > 0) {
					// scroll down
					scrollBar.setValue(scrollBar.getValue() + event.getScrollAmount());
				} else if (rotation < 0) {
					scrollBar.setValue(scrollBar.getValue() - event.getScrollAmount());
				}
			}
		});
		updatePreferredSize(scrollBar);
		return scrollBar;
	}
	
	private static void updatePreferredSize(JScrollBar scrollBar) {
		int range = scrollBar.getMaximum() - scrollBar.getMinimum() + 1;
		if (scrollBar.getOrientation() == JScrollBar.HORIZONTAL) {
			GuiUtils.setPreferredWidth(scrollBar, range);
		} else {
			// vertical
			GuiUtils.setPreferredHeight(scrollBar, range);
		}
		scrollBar.setMinimumSize(scrollBar.getPreferredSize());
		scrollBar.setSize(scrollBar.getPreferredSize());
		scrollBar.updateUI();
		scrollBar.validate();
		scrollBar.revalidate();
		scrollBar.repaint();
	}
}
