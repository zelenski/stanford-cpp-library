/*
 * @version 2017/10/14
 * - added Ellipse2D constructor, parameterless constructor
 * - rename some method/ctor parameters
 * @version 2016/10/12
 * - JL's getBounds fix for contains()
 */

package acm.graphics;

import acm.util.ErrorException;
import java.awt.*;
import java.awt.geom.*;

public class GOval extends GObject implements GFillable, GResizable {
	static final long serialVersionUID = 21L;

	private double frameWidth;
	private double frameHeight;
	private boolean isFilled;
	private Color fillColor;

	public GOval() {
		this(0.0, 0.0, 0.0, 0.0);
	}

	public GOval(double width, double height) {
		this(0.0, 0.0, width, height);
	}

	public GOval(double x, double y, double width, double height) {
		frameWidth = width;
		frameHeight = height;
		setLocation(x, y);
	}
	
	public GOval(Ellipse2D oval) {
		this(oval.getX(), oval.getY(), oval.getWidth(), oval.getHeight());
	}

	public GRectangle getBounds() {
		Object obj = new java.awt.geom.Ellipse2D.Double(0.0D, 0.0D, frameWidth, frameHeight);
		AffineTransform affinetransform = getMatrix();
		if (affinetransform != null) {
			obj = affinetransform.createTransformedShape(((Shape) (obj)));
		}
		Rectangle2D rectangle = ((Shape) (obj)).getBounds2D();
		return new GRectangle(rectangle.getX() + getX(), rectangle.getY() + getY(), rectangle.getWidth(),
				rectangle.getHeight());
	}

	public boolean contains(double x, double y) {
		Object obj = null;
		AffineTransform affinetransform = getMatrix();
		if (affinetransform == null) {
			obj = new java.awt.geom.Ellipse2D.Double(0.0D, 0.0D, frameWidth, frameHeight);
		} else {
			// JL: remove getBounds() call here
			obj = affinetransform.createTransformedShape(((Shape) (obj)));
		}
		return ((Shape) (obj)).contains(x - getX(), y - getY());
	}

	protected void paint2d(Graphics2D g2) {
		java.awt.geom.Ellipse2D.Double oval = new java.awt.geom.Ellipse2D.Double(0.0D, 0.0D, frameWidth,
				frameHeight);
		if (isFilled()) {
			g2.setColor(getFillColor());
			g2.fill(oval);
			g2.setColor(getColor());
		}
		g2.draw(oval);
	}

	public void setFilled(boolean flag) {
		isFilled = flag;
		repaint();
	}

	public boolean isFilled() {
		return isFilled;
	}

	public void setFillColor(Color color) {
		fillColor = color;
		isFilled = fillColor != null;
		repaint();
	}

	public Color getFillColor() {
		return fillColor != null ? fillColor : getColor();
	}

	public void setSize(double width, double height) {
		if (getMatrix() != null) {
			throw new ErrorException("setSize: Object has been transformed");
		} else {
			frameWidth = width;
			frameHeight = height;
			repaint();
			return;
		}
	}

	public final void setSize(GDimension gdimension) {
		setSize(gdimension.getWidth(), gdimension.getHeight());
	}

	public GDimension getSize() {
		return new GDimension(frameWidth, frameHeight);
	}

	public void setBounds(double x, double y, double width, double height) {
		if (getMatrix() != null) {
			throw new ErrorException("setBounds: Object has been transformed");
		} else {
			frameWidth = width;
			frameHeight = height;
			setLocation(x, y);
			return;
		}
	}

	public final void setBounds(GRectangle grectangle) {
		if (getMatrix() != null) {
			throw new ErrorException("setBounds: Object has been transformed");
		} else {
			setBounds(grectangle.getX(), grectangle.getY(), grectangle.getWidth(), grectangle.getHeight());
			return;
		}
	}
}
