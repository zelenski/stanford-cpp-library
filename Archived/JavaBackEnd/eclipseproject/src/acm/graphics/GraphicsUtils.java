/*
 * @version 2017/10/14
 * - initial version
 */

package acm.graphics;

import java.awt.*;

/**
 * A static class with utility methods related to graphics.
 */
public final class GraphicsUtils {
	private GraphicsUtils() {
		// empty
	}
	
	public static void printGraphicsDebugInfo(Graphics g) {
		System.err.println("GraphicsUtils: g =" + g
				+ "\nis Graphics2D? " + (g instanceof Graphics2D)
				+ "\nantialias = " + ((Graphics2D) g).getRenderingHint(RenderingHints.KEY_ANTIALIASING));
	}
	
	public static void setAntialiasingDefault(Component comp) {
		if (comp != null) {
			setAntialiasingDefault(comp.getGraphics());
		}
	}
	
	public static void setAntialiasing(Component comp, boolean antialiasing) {
		if (comp != null) {
			setAntialiasing(comp.getGraphics(), antialiasing);
		}
	}
	
	public static void setAntialiasingDefault(Graphics g) {
		setAntialiasing(g, GObject.isAntiAliasing());
	}
	
	public static void setAntialiasing(Graphics g, boolean antialiasing) {
		if (!(g instanceof Graphics2D)) {
			return;
		}
		Graphics2D g2 = (Graphics2D) g;
		if (antialiasing) {
			g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		} else {
			g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
			g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_OFF);
			g2.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_SPEED);
		}
	}

	public static void setAntialiasingDefault(Image image) {
		if (image != null) {
			setAntialiasingDefault(image.getGraphics());
		}
	}
	
	public static void setAntialiasing(Image image, boolean antialiasing) {
		if (image != null) {
			setAntialiasing(image.getGraphics(), antialiasing);
		}
	}
}
