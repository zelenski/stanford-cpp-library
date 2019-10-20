/*
 * @version 2017/06/10
 * - added setOpaque/isOpaque
 */

package acm.graphics;

import java.awt.*;
import java.awt.image.*;
import java.util.*;

public interface GCanvasInterface {
	public void add(GObject gobj);
	public void add(GObject gobj, double x, double y);
	public void add(GObject gobj, GPoint pt);
	public void add(Component comp, double x, double y);
	public void add(Component comp, GPoint pt);
	public void clear();
	public Color getBackground();
	public int getElementCount();
	public Font getFont();
	public <T extends GObject> T getElement(int index);
	public <T extends GObject> T getElementAt(double x, double y);
	public <T extends GObject> T getElementAt(double... coords);
	public <T extends GObject> T getElementAt(GPoint pt);
	public int getHeight();
	public Dimension getSize();
	public int getWidth();
	public boolean hasElementAt(double x, double y);
	public boolean hasElementAt(double... coords);
	public boolean hasElementAt(GPoint pt);
	public boolean isAntiAliasing();
	public boolean isOpaque();
	public Iterator<GObject> iterator();
	public Iterator<GObject> iterator(int direction);
	public void remove(double x, double y);
	public void remove(GPoint pt);
	public void remove(GObject gobj);
	public void removeAll();
	public void removeAll(double x, double y);
	public void removeAll(double... coords);
	public void removeAll(GPoint pt);
	public void repaint();
	public void setAntiAliasing(boolean antialias);
	public void setBackground(Color bg);
	public void setFont(Font font);
	// public void setHeight(int height);
	public void setOpaque(boolean opaque);
	public void setSize(Dimension size);
	// public void setWidth(int width);
	public void setShowPixelInfo(boolean show);
	public BufferedImage toImage();
}
