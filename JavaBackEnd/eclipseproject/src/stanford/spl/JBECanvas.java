/*
 * @version 2016/10/25
 * - bug fix for paint() -> paintComponent() (prevents overdraw on shapes)
 * @version 2016/08/01
 * - added toImage method (to facilitate image diffing and autograders)
 * @version 2016/07/20
 * - bug fix with clear()  (wasn't working)
 * @version ...
 * - I brought this in so I could overwrite/fix setPreferredSize
 */

package stanford.spl;

import acm.graphics.*;
import java.awt.*;

public class JBECanvas extends GCanvas {
	private static final long serialVersionUID = 1L;
	private String windowId;
	private TopCompound topCompound;
	private int preferredWidth;
	private int preferredHeight;

	public JBECanvas(String windowId, int width, int height) {
		this.windowId = windowId;
		this.preferredWidth = width;
		this.preferredHeight = height;
	}

	protected void setTopCompound(TopCompound top) {
		this.topCompound = top;
		top.setCanvas(this);
		top.setParent(this);
	}

	protected TopCompound getTopCompound() {
		return this.topCompound;
	}

	public String getWindowId() {
		return this.windowId;
	}

	public Dimension getPreferredSize() {
		return new Dimension(this.preferredWidth, this.preferredHeight);
	}
	
	public void setPreferredSize(Dimension d) {
		this.preferredWidth = d.width;
		this.preferredHeight = d.height;
	}

	@Override
	public void clear() {
		if (topCompound != null) {
			topCompound.removeAll();
		}
		super.clear();
		conditionalRepaint();
	}

//	public void paint(Graphics paramGraphics) {
//		super.paint(paramGraphics);
//		this.topCompound.paint(paramGraphics);
//	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		this.topCompound.paint(g);
	}
}
