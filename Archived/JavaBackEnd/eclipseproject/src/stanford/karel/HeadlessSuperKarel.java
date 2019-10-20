/*
 * This class implements an extension of the basic Karel class that supports
 * more operations.
 * @version 2017/04/04
 * - made a copy of SuperKarel.java
 */

package stanford.karel;

import java.awt.*;

/**
 * Extended Karel class . . .
 */
public class HeadlessSuperKarel extends HeadlessKarel {
	public static final Color BLACK = Color.BLACK;
	public static final Color BLUE = Color.BLUE;
	public static final Color CYAN = Color.CYAN;
	public static final Color DARK_GRAY = Color.DARK_GRAY;
	public static final Color GRAY = Color.GRAY;
	public static final Color GREEN = Color.GREEN;
	public static final Color LIGHT_GRAY = Color.LIGHT_GRAY;
	public static final Color MAGENTA = Color.MAGENTA;
	public static final Color ORANGE = Color.ORANGE;
	public static final Color PINK = Color.PINK;
	public static final Color RED = Color.RED;
	public static final Color WHITE = Color.WHITE;
	public static final Color YELLOW = Color.YELLOW;

	/* Constructor */

	public HeadlessSuperKarel() {
		/* Empty */
	}

	public void run() {
		/* Empty */
	}

	public void turnRight() {
		checkWorld("turnRight");
		setDirection(KarelWorld.rightFrom(getDirection()));
		getWorld().trace();
	}

	public void turnAround() {
		checkWorld("turnAround");
		setDirection(KarelWorld.oppositeDirection(getDirection()));
		getWorld().trace();
	}

	public void paintCorner(Color color) {
		KarelWorld world = getWorld();
		Point pt = getLocation();
		checkWorld("paintCorner");
		world.setCornerColor(pt.x, pt.y, color);
		world.trace();
	}

	public void paintCorner(int rgb) {
		paintCorner(new Color(rgb));
	}

	public void paintCorner(double red, double blue, double green) {
		paintCorner(new Color((float) red, (float) blue, (float) green));
	}

	public Color newColor(int rgb) {
		return new Color(rgb);
	}

	public boolean random() {
		checkWorld("random");
		return random(0.5);
	}

	public boolean random(double p) {
		checkWorld("random");
		return Math.random() < p;
	}

	public boolean cornerColorIs(Color color) {
		KarelWorld world = getWorld();
		Point pt = getLocation();
		checkWorld("cornerColorIs");
		if (color == null) {
			return world.getCornerColor(pt.x, pt.y) == null;
		} else {
			return (color.equals(world.getCornerColor(pt.x, pt.y)));
		}
	}

	public boolean cornerColorIs(int rgb) {
		return cornerColorIs(new Color(rgb));
	}

	public boolean cornerColorIs(double red, double blue, double green) {
		Color color = new Color((float) red, (float) blue, (float) green);
		return cornerColorIs(color);
	}

	public void pause(double milliseconds) {
		KarelWorld world = getWorld();
		boolean repaintFlag = true;
		KarelWorldMonitor monitor = world.getMonitor();
		if (monitor != null && monitor.getSpeed() > 0.98) {
			repaintFlag = false;
		}
		world.setRepaintFlag(true);
		world.repaint();
		world.setRepaintFlag(repaintFlag);
		try {
			int millis = (int) milliseconds;
			int nanos = (int) Math.round((milliseconds - millis) * 1000000);
			Thread.sleep(millis, nanos);
		} catch (InterruptedException ex) {
			/* Empty */
		}
	}
	
	/**
	 * Causes Karel to pop up a speech bubble with the given text in it.
	 */
	public void say(String text) {
		// TODO
	}
}
