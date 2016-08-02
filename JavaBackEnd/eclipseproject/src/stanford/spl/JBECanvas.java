/*
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
import java.awt.geom.*;
import java.awt.image.BufferedImage;

public class JBECanvas extends GCanvas {
	private static final long serialVersionUID = 1L;
	private String windowId;
	private TopCompound topCompound;
	private Graphics2D osg;
	private Image offscreenImage;
	private int preferredWidth;
	private int preferredHeight;

	public JBECanvas(String paramString, int paramInt1, int paramInt2) {
		this.windowId = paramString;
		setBackground(Color.WHITE);
		setForeground(Color.BLACK);
		setOpaque(false);
		setLayout(null);
		this.preferredWidth = paramInt1;
		this.preferredHeight = paramInt2;
	}

	public Graphics2D getOSG() {
		return this.osg;
	}

	protected void setTopCompound(TopCompound paramTopCompound) {
		this.topCompound = paramTopCompound;
		paramTopCompound.setCanvas(this);
		paramTopCompound.setParent(this);
	}

	protected TopCompound getTopCompound() {
		return this.topCompound;
	}

	protected void initOffscreenImage() {
		Dimension localDimension = Toolkit.getDefaultToolkit().getScreenSize();
		this.offscreenImage = createImage(localDimension.width,
				localDimension.height);
		this.osg = ((Graphics2D) this.offscreenImage.getGraphics());
		this.osg.setColor(getBackground());
		this.osg.fillRect(0, 0, localDimension.width, localDimension.height);
		this.osg.setColor(getForeground());
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

	public void clear() {
		Dimension localDimension = Toolkit.getDefaultToolkit().getScreenSize();
		this.osg.setColor(Color.WHITE);
		this.osg.fillRect(0, 0, localDimension.width, localDimension.height);
		this.osg.setColor(Color.BLACK);
		if (topCompound != null) {
			topCompound.removeAll();
		}
		repaint();
	}

	public void draw(Shape paramShape) {
		this.osg.draw(paramShape);
		repaint();
	}

	public void fill(Shape paramShape) {
		this.osg.fill(paramShape);
		repaint();
	}

	public void drawRect(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4) {
		Rectangle2D.Double localDouble = new Rectangle2D.Double(paramDouble1,
				paramDouble2, paramDouble3, paramDouble4);
		this.osg.draw(localDouble);
		repaint();
	}

	public void fillRect(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4) {
		Rectangle2D.Double localDouble = new Rectangle2D.Double(paramDouble1,
				paramDouble2, paramDouble3, paramDouble4);
		this.osg.fill(localDouble);
		this.osg.draw(localDouble);
		repaint();
	}

	public void drawOval(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4) {
		Ellipse2D.Double localDouble = new Ellipse2D.Double(paramDouble1,
				paramDouble2, paramDouble3, paramDouble4);
		this.osg.draw(localDouble);
		repaint();
	}

	public void fillOval(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4) {
		Ellipse2D.Double localDouble = new Ellipse2D.Double(paramDouble1,
				paramDouble2, paramDouble3, paramDouble4);
		this.osg.fill(localDouble);
		this.osg.draw(localDouble);
		repaint();
	}

	public void drawArc(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4, double paramDouble5,
			double paramDouble6) {
		Arc2D.Double localDouble = new Arc2D.Double(paramDouble1, paramDouble2,
				paramDouble3, paramDouble4, paramDouble5, paramDouble6, 0);

		this.osg.draw(localDouble);
		repaint();
	}

	public void fillArc(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4, double paramDouble5,
			double paramDouble6) {
		Arc2D.Double localDouble = new Arc2D.Double(paramDouble1, paramDouble2,
				paramDouble3, paramDouble4, paramDouble5, paramDouble6, 2);

		this.osg.fill(localDouble);
		this.osg.draw(localDouble);
		repaint();
	}

	public void drawLine(double paramDouble1, double paramDouble2,
			double paramDouble3, double paramDouble4) {
		Line2D.Double localDouble = new Line2D.Double(paramDouble1,
				paramDouble2, paramDouble3, paramDouble4);
		this.osg.draw(localDouble);
		repaint();
	}

	public void setColor(int paramInt) {
		this.osg.setColor(new Color(paramInt));
	}

	public void paint(Graphics paramGraphics) {
		paramGraphics.drawImage(this.offscreenImage, 0, 0, this);
		this.topCompound.paint(paramGraphics);
		super.paint(paramGraphics);
	}
	
	/**
	 * Returns the pixel contents of this canvas as a BufferedImage.
	 */
	public BufferedImage toImage() {
		// dump canvas into a BufferedImage
		BufferedImage img = new BufferedImage(preferredWidth, preferredHeight,
				BufferedImage.TYPE_INT_ARGB);
		paint(img.getGraphics());
		return img;
	}
}
