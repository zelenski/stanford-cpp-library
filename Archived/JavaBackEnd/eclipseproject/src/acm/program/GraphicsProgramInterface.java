/*
 * @version 2017/07/21
 * - additional methods: hasElementAt(GPoint), etc.
 */

package acm.program;

import acm.graphics.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;

public interface GraphicsProgramInterface extends ProgramInterface, GCanvasInterface, MouseListener, MouseMotionListener {
	public void add(GObject gobj);
	public void add(GObject gobj, double x, double y);
	public void add(GObject gobj, GPoint pt);
	public void addKeyListeners();
	public void addKeyListeners(KeyListener listener);
	public void addMouseListeners();
	public void addMouseListeners(EventListener listener);
	public void clearCanvas();
	public double getCanvasHeight();
	public Dimension getCanvasSize();
	public double getCanvasWidth();
	public <T extends GObject> T getElementAt(double x, double y);
	public <T extends GObject> T getElementAt(GPoint pt);
	public boolean hasElementAt(double x, double y);
	public boolean hasElementAt(GPoint point);
	public GCanvas getGCanvas();
	public void remove(double x, double y);
	public void remove(GPoint pt);
	public void removeAll();
	public void removeAll(double x, double y);
	public void removeAll(GPoint pt);
	public void removeAllComponents();
	public void setCanvasHeight(double height);
	public void setCanvasSize(double width, double height);
	public void setCanvasWidth(double width);
	public void setSize(double width, double height);
	public void setWindowSize(double width, double height);
	public void waitForClick();
}
