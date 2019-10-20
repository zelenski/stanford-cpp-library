/*
 * @version 2016/10/12
 * - JL's getBounds fix for paint2d()
 */

package acm.graphics;

// import java.awt.Graphics;
import java.awt.Graphics2D;

public class G3DRect extends GRect {
	static final long serialVersionUID = 21L;

	private boolean isRaised;

	public G3DRect(double d, double d1) {
		this(0.0D, 0.0D, d, d1, false);
	}

	public G3DRect(double d, double d1, double d2, double d3) {
		this(d, d1, d2, d3, false);
	}

	public G3DRect(double d, double d1, double d2, double d3, boolean flag) {
		super(d, d1, d2, d3);
		isRaised = flag;
	}

//	public void paint(Graphics g) {
//		if (!isVisible())
//			return;
//		Graphics2D graphics2d = createTransformedGraphics(g);
//		if (isFilled()) {
//			graphics2d.setColor(getFillColor());
//			graphics2d.fill3DRect(0, 0, GMath.round(getFrameWidth()), GMath.round(getFrameHeight()), isRaised);
//			graphics2d.setColor(getColor());
//		}
//		graphics2d.draw3DRect(0, 0, GMath.round(getFrameWidth()), GMath.round(getFrameHeight()), isRaised);
//		graphics2d.dispose();
//	}

	/**
	 * Implements the <code>paint2d</code> operation for this graphical object.
	 * This method is not called directly by clients.
	 * @noshow
	 */
	protected void paint2d(Graphics2D g) {
		if (isFilled()) {
			g.setColor(getFillColor());
			g.fill3DRect(0, 0, GMath.round(getFrameWidth()), GMath.round(getFrameHeight()), isRaised);
			g.setColor(getColor());
		} else {
			g.draw3DRect(0, 0, GMath.round(getFrameWidth()), GMath.round(getFrameHeight()), isRaised);
		}
	}

	public void setRaised(boolean flag) {
		isRaised = flag;
	}

	public boolean isRaised() {
		return isRaised;
	}
}
