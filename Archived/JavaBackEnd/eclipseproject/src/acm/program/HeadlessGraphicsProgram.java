/*
 * @version 2017/07/21
 * - added add() overloads
 * @version 2017/06/10
 * - added setOpaque/isOpaque
 * - set default background color to WHITE
 * @version 2017/05/02
 * - added fakeMousePress/Moved/Dragged
 * @version 2017/04/27
 * - initial version; added to support graphical problems in CodeStepByStep
 */

package acm.program;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.util.*;
import acm.graphics.*;
import acm.util.*;

public class HeadlessGraphicsProgram implements GraphicsProgramInterface {
	// copied from Program.java
	private static final int DEFAULT_HEIGHT = 492;
	private static final int DEFAULT_WIDTH = 754;
	private static final String GRAPHICAL_OUTPUT_SAVE_FILENAME_PROPERTY = "graphicsprogram.save";
	private static boolean antialiasDefault = false;
	private static boolean exitEnabled = false;
	private static String saveFile;
	
	private String title = "GraphicsProgram";
	private int windowWidth;
	private int windowHeight;
	private GCanvasInterface gc;

	public static String getSaveFile() {
		if (saveFile == null) {
			// fall back to a default passed through a System property
			return System.getProperty(GRAPHICAL_OUTPUT_SAVE_FILENAME_PROPERTY);
		} else {
			return saveFile;
		}
	}
	
	public static void setAntiAliasDefault(boolean antialias) {
		antialiasDefault = antialias;
	}
	
	public static void setSaveFile(String filename) {
		saveFile = filename;
	}
	
	// instance initializer
	{
		gc = new GCanvas();
		gc.setAntiAliasing(antialiasDefault);
		gc.setOpaque(true);
		setSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
		setTitle(getClass().getName());
		setBackground(Color.WHITE);
		gc.setBackground(Color.WHITE);
	}
	
	public final void main(String[] args) {
		init();
		run();

		// who knows why, but I need a short delay here or
		// else the graphical data will intermittently not save  :-/
		try {
			Thread.sleep(600);
		} catch (InterruptedException ie) {
			// empty
		}

		save();
		exit();
	}

	// ProgramInterface methods

	public final void exit() {
		if (exitEnabled) {
			try {
				Thread.sleep(50);
				System.exit(0);
			} catch (Exception e) {
				// empty
			}
		}
	}

	public void init() {
		// empty
	}

	public void run() {
		// empty; override me!
	}
	
	public final void save() {
		String outFileName = getSaveFile();
		if (outFileName != null) {
			BufferedImage img = toImage();
			MediaTools.saveImage(img, outFileName);
		}
	}

	public void setParameterTable(Map<String, String> table) {
		// empty
	}

	public void setStartupObject(Object obj) {
		// empty
	}

	public void start() {
		// empty
	}

	public void start(String[] args) {
		// empty
	}
	
	private MouseEvent fakeMouseEvent(int x, int y) {
		Component component = this.getGCanvas();
		if (component == null) {
			component = new javax.swing.JPanel();
		}
		return new MouseEvent(
				/* source */ component,
				/* id */ 0,
				/* when */ System.currentTimeMillis(),
				/* modifiers */ 0,
				x,
				y,
				/* clickCount */ 1,
				/* popupTrigger */ false);
	}
	
	public void fakeMouseClicked(int x, int y) {
		MouseEvent event = fakeMouseEvent(x, y);
		mousePressed(event);
		mouseReleased(event);
		mouseClicked(event);
	}
	
	public void fakeMousePressed(int x, int y) {
		MouseEvent event = fakeMouseEvent(x, y);
		mousePressed(event);
	}
	
	public void fakeMouseReleased(int x, int y) {
		MouseEvent event = fakeMouseEvent(x, y);
		mouseReleased(event);
	}
	
	public void fakeMouseMoved(int x, int y) {
		MouseEvent event = fakeMouseEvent(x, y);
		mouseMoved(event);
	}
	
	public void fakeMouseDragged(int x, int y) {
		MouseEvent event = fakeMouseEvent(x, y);
		mouseDragged(event);
	}
	
	public void fakeMouseDragSequence(int... coords) {
		if (coords.length == 0 || coords.length % 2 != 0) {
			throw new IllegalArgumentException("must pass an even number of x/y coordinates");
		}
		mousePressed(fakeMouseEvent(coords[0], coords[1]));
		for (int i = 0; i < coords.length - 1; i += 2) {
			mouseDragged(fakeMouseEvent(coords[i], coords[i + 1]));
		}
		mouseReleased(fakeMouseEvent(coords[coords.length - 2], coords[coords.length - 1]));
	}
	
	// GraphicsProgramInterface methods

	public Component add(Component comp) {
		// empty
		return comp;
	}

	public void add(Component comp, Object constraints) {
		// empty
	}
	
	public void add(Component comp, String region, Object constraints) {
		// empty
	}

	public void add(GObject gobj) {
		gc.add(gobj);
	}

	public void add(GObject gobj, double x, double y) {
		gc.add(gobj, x, y);
	}

	public void add(GObject gobj, GPoint pt) {
		gc.add(gobj, pt);
	}

	public void add(Component comp, double x, double y) {
		// empty
	}

	public void add(Component comp, GPoint pt) {
		// empty
	}

	public void addActionListeners() {
		// empty
	}
	
	public void clear() {
		gc.clear();
	}
	
	public Color getBackground() {
		return gc.getBackground();
	}

	public int getElementCount() {
		return gc.getElementCount();
	}

	public <T extends GObject> T getElement(int index) {
		return gc.getElement(index);
	}

	public <T extends GObject> T getElementAt(double x, double y) {
		return gc.getElementAt(x, y);
	}

	public <T extends GObject> T getElementAt(double... coords) {
		return gc.getElementAt(coords);
	}

	public <T extends GObject> T getElementAt(GPoint pt) {
		return gc.getElementAt(pt);
	}
	
	public Font getFont() {
		return gc.getFont();
	}

	public boolean hasElementAt(double x, double y) {
		return gc.hasElementAt(x, y);
	}

	public boolean hasElementAt(double... coords) {
		return gc.hasElementAt(coords);
	}
	
	public boolean isAntiAliasing() {
		return gc.isAntiAliasing();
	}

	public Iterator<GObject> iterator() {
		return gc.iterator();
	}

	public Iterator<GObject> iterator(int direction) {
		return gc.iterator(direction);
	}

	public void mouseClicked(MouseEvent e) {
		// empty
	}

	public void mousePressed(MouseEvent e) {
		// empty
	}

	public void mouseReleased(MouseEvent e) {
		// empty
	}

	public void mouseEntered(MouseEvent e) {
		// empty
	}

	public void mouseExited(MouseEvent e) {
		// empty
	}

	public void mouseDragged(MouseEvent e) {
		// empty
	}

	public void mouseMoved(MouseEvent e) {
		// empty
	}
	
	public void remove(double x, double y) {
		gc.remove(x, y);
	}

	public void remove(GPoint pt) {
		gc.remove(pt);
	}

	public void remove(GObject gobj) {
		gc.remove(gobj);
	}

	public void removeAll() {
		gc.clear();
	}

	public void removeAll(double x, double y) {
		gc.removeAll(x, y);
	}

	public void removeAll(double... coords) {
		gc.removeAll(coords);
	}

	public void removeAll(GPoint pt) {
		gc.removeAll(pt);
	}

	public void repaint() {
		gc.repaint();
	}
	
	public void setAntiAliasing(boolean antialias) {
		gc.setAntiAliasing(antialias);
	}

	public void setBackground(Color bg) {
		gc.setBackground(bg);
	}

	public void setShowPixelInfo(boolean show) {
		// empty
	}

	public BufferedImage toImage() {
		return gc.toImage();
	}

	public void addKeyListeners() {
		// empty
	}

	public void addKeyListeners(KeyListener listener) {
		// empty
	}

	public void addMouseListeners() {
		// empty
	}

	public void addMouseListeners(EventListener listener) {
		// empty
	}

	public void clearCanvas() {
		gc.clear();
	}

	public int getHeight() {
		if (gc.getHeight() > 0) {
			return gc.getHeight();
		} else {
			return windowHeight;
		}
	}
	
	public Dimension getSize() {
		if (gc.getSize().width > 0 && gc.getSize().height > 0) {
			return gc.getSize();
		} else {
			return new Dimension(windowWidth, windowHeight);
		}
	}
	
	public int getWidth() {
		if (gc.getWidth() > 0) {
			return gc.getWidth();
		} else {
			return windowWidth;
		}
	}

	public double getCanvasHeight() {
		return gc.getHeight();
	}

	public Dimension getCanvasSize() {
		return gc.getSize();
	}

	public double getCanvasWidth() {
		return gc.getWidth();
	}

	public GCanvas getGCanvas() {
		// gc not guaranteed to be a GCanvas
		if (gc instanceof GCanvas) {
			return (GCanvas) gc;
		} else {
			return null;
		}
	}

	public boolean hasElementAt(GPoint point) {
		return gc.hasElementAt(point);
	}
	
	public boolean isOpaque() {
		return gc.isOpaque();
	}
	
	public void pause(double milliseconds) {
		// don't actually pause! do nothing
	}

	public void removeAllComponents() {
		gc.clear();
	}

	public void setCanvasHeight(double height) {
		windowHeight = (int) height;
		gc.setSize(new Dimension(gc.getSize().width, (int) height));
	}

	public void setCanvasSize(double width, double height) {
		windowWidth = (int) width;
		windowHeight = (int) height;
		gc.setSize(new Dimension((int) width, (int) height));
	}

	public void setCanvasWidth(double width) {
		windowWidth = (int) width;
		gc.setSize(new Dimension((int) width, gc.getSize().height));
	}
	
	public void setFont(Font font) {
		gc.setFont(font);
	}

	public void setForeground(Color color) {
		// empty
	}

	public void setOpaque(boolean opaque) {
		gc.setOpaque(opaque);
	}
	
	public void setSize(Dimension size) {
		windowWidth = (int) size.width;
		windowHeight = (int) size.height;
		gc.setSize(size);
	}

	public void setSize(double width, double height) {
		windowWidth = (int) width;
		windowHeight = (int) height;
		gc.setSize(new Dimension((int) width, (int) height));
	}

	public void setWindowSize(double width, double height) {
		setSize(width, height);
	}

	public void waitForClick() {
		// empty
	}

	public String getTitle() {
		return title;
	}

	public void print(Object value) {
		System.out.print(value);
	}

	public void print(String value) {
		System.out.print(value);
	}

	public void printf(String format, Object... args) {
		System.out.printf(format, args);
	}

	public void println() {
		System.out.println();
	}

	public void println(Object value) {
		System.out.println(value);
	}

	public void println(String value) {
		System.out.println(value);
	}

	public void setTitle(String title) {
		this.title = title;
	}
}
