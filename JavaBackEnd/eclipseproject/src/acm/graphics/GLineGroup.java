/*
 * @version 2016/05/19
 * - initial version
 */

package acm.graphics;

import java.awt.*;
import java.util.*;
import java.util.List;

/**
 * A line group represents a collection of points that form line segments.
 * A line group can be drawn on the screen and looks similar to a collection
 * of GLine objects positioned one after another.
 * 
 * @author Marty Stepp
 */
public class GLineGroup extends GObject implements Iterable<GPoint> {
	private List<GPoint> points;
	
	/**
	 * Creates a new line group that does not contain any points.
	 */
	public GLineGroup() {
		points = new ArrayList<GPoint>();
	}
	
	/**
	 * Adds a point with the given coordinates to this line group.
	 */
	public void add(double x, double y) {
		add(new GPoint(x, y));
	}
	
	/**
	 * Adds the given point to this line group.
	 * @param point the point to add
	 */
	public void add(GPoint point) {
		if (point == null) {
			throw new NullPointerException();
		}
		points.add(point);
		repaint();
	}
	
	/**
	 * Removes all points from this line group.
	 */
	public void clear() {
		points.clear();
		repaint();
	}
	
	/**
	 * Returns true if any of the line segments in this line group touch the given point.
	 */
	@Override
	public boolean contains(double x, double y) {
		for (GPoint point : points) {
			if (point.equals(x, y)) {
				return true;
			}
		}
		
		for (int i = 0; i < size() - 1; i++) {
			GPoint p1 = points.get(i);
			GPoint p2 = points.get(i + 1);
			if (GLine.intersects(x, y, p1.getX(), p1.getY(), p2.getX(), p2.getY(), getLineWidth())) {
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Returns a bounding rectangle that tightly contains this line group.
	 * The rectangle's min x/y are the minimum x/y values of any point in the line group,
	 * and the rectangle's max x/y are the maximum x/y values of any point in the line group.
	 */
	@Override
	public GRectangle getBounds() {
		if (isEmpty()) {
			return new GRectangle();
		} else {
			GPoint p0 = points.get(0);
			double minX = p0.getX();
			double maxX = minX;
			double minY = p0.getY();
			double maxY = minY;
			for (int i = 1; i < size(); i++) {
				GPoint p1 = points.get(i);
				minX = Math.min(minX, p1.getX());
				maxX = Math.max(maxX, p1.getX());
				minY = Math.min(minY, p1.getY());
				maxY = Math.max(maxY, p1.getY());
			}
			return new GRectangle(minX, maxX, minY, maxY);
		}
	}
	
//	@Override
//	public double getX() {
//		return getBounds().getX();
//	}
//	
//	@Override
//	public double getY() {
//		return getBounds().getY();
//	}

	/**
	 * Returns true if there are no points in this line group.
	 */
	public boolean isEmpty() {
		return size() == 0;
	}
	
	/**
	 * Returns an Iterator for examining the points in this line group.
	 */
	public Iterator<GPoint> iterator() {
		return points.iterator();
	}
	
	/**
	 * Paints the group of lines on the screen.
	 */
	@Override
	protected void paint2d(Graphics2D g) {
		double x = 0;//getX();
		double y = 0;//getY();
		for (int i = 0; i < size() - 1; i++) {
			GPoint p1 = points.get(i);
			GPoint p2 = points.get(i + 1);
			double x0 = p1.getX() - x;
			double y0 = p1.getY() - y;
			double x1 = p2.getX() - x;
			double y1 = p2.getY() - y;
			g.drawLine(GMath.round(x0), GMath.round(y0),
					GMath.round(x1), GMath.round(y1));
		}
	}
	
	/**
	 * Returns a string indicating the parameters of this object.
	 * 
	 * @noshow
	 */
	public String paramString() {
		String tail = super.paramString();
		tail = tail.substring(tail.indexOf(')') + 1);
		return "points=" + points + ", lineWidth=" + getLineWidth() + tail;
	}

	/**
	 * Removes the given point from this line group, if it is part of the line group.
	 */
	public void remove(double x, double y) {
		remove(new GPoint(x, y));
	}
	
	/**
	 * Removes the given point from this line group, if it is part of the line group.
	 */
	public void remove(GPoint point) {
		points.remove(point);
	}
	
	/**
	 * Sets the line group's top/left corner to the given x/y pair.
	 * Initially every line group has a top/left corner at (0, 0), regardless of any points
	 * that may have been added to it.
	 */
	@Override
	public void setLocation(double x, double y) {
		double oldX = getX();
		double oldY = getY();
		double dx = x - oldX;
		double dy = y - oldY;
		for (GPoint point : this) {
			point.translate(dx, dy);
		}
		repaint();
	}

	/**
	 * Returns the number of points that have been added to the line group.
	 * @return
	 */
	public int size() {
		return points.size();
	}

	/**
	 * Shifts every point in this line group by the given amounts.
	 */
	public void translate(double dx, double dy) {
		for (GPoint point : this) {
			point.translate(dx, dy);
		}
		repaint();
	}
}
