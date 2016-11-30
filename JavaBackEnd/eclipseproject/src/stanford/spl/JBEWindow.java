/*
 * This is the class that represents the C++ lib GWindow class.
 * 
 * @version 2016/10/12
 * - added getContentPaneSize method
 * @version 2016/10/07
 * - added getCanvasSize method (fix GWindow_getCanvasSize JBE command)
 * @version 2016/07/30
 * - fixed constructor with false visibility (don't pop up window)
 * - added saveCanvasPixels method for saving graphical output to a file
 */

package stanford.spl;

import acm.gui.TableLayout;
import stanford.cs106.io.*;
import stanford.cs106.util.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.util.*;
import javax.imageio.*;
import javax.swing.*;

public class JBEWindow extends JFrame {
	private static final long serialVersionUID = 0L;
	private static Set<String> IMAGE_FILE_TYPES = CollectionUtils.asSet(
			"gif", "jpg", "jpeg", "png"
	);
	
	private JavaBackEnd jbe;
	private JBECanvas canvas;
	private String windowId;
	private JPanel northPanel;
	private JPanel eastPanel;
	private JPanel southPanel;
	private JPanel westPanel;

	public JBEWindow(JavaBackEnd paramJavaBackEnd, String paramString1,
			String paramString2, int width, int height) {
		super(paramString2);
		this.jbe = paramJavaBackEnd;
		this.windowId = paramString1;
		setLayout(new BorderLayout());
		this.canvas = new JBECanvas(paramString1, width, height);
		addWindowListener(this.jbe);
		this.canvas.addComponentListener(this.jbe);
		this.canvas.addMouseListener(this.jbe);
		this.canvas.addMouseMotionListener(this.jbe);
		this.canvas.addKeyListener(this.jbe);
		add(this.canvas, "Center");
		createSidePanels();
	}

	public JBECanvas getCanvas() {
		return this.canvas;
	}
	
	public Dimension getCanvasSize() {
		if (canvas == null) {
			return new Dimension(0, 0);
		}
		
		Dimension pref = canvas.getPreferredSize();
		Dimension actual = canvas.getSize();
		if (actual == null) {
			return pref;
		} else if (pref == null) {
			return actual;
		} else {
			if (actual.width <= 0 && actual.height <= 0) {
				return pref;
			} else { //if (pref.width <= 0 && pref.height <= 0) {
				return actual;
			}
//			else {
//				if (actual.width < pref.width || actual.height < pref.height) {
//					return actual;
//				} else {
//					
//				}
//			}
		}
	}

	public Dimension getContentPaneSize() {
		if (getContentPane() == null) {
			return new Dimension(0, 0);
		}
		
		Dimension pref = getContentPane().getPreferredSize();
		Dimension actual = getContentPane().getSize();
		if (actual == null) {
			return pref;
		} else if (pref == null) {
			return actual;
		} else {
			if (actual.width <= 0 && actual.height <= 0) {
				return pref;
			} else { //if (pref.width <= 0 && pref.height <= 0) {
				return actual;
			}
		}
	}

	public String getWindowId() {
		return this.windowId;
	}

	// JL: SwingUtilities.invokeLater
	public void close() {
		WindowEvent localWindowEvent = new WindowEvent(this, 201);
		processWindowEvent(localWindowEvent);
	}

	// JL: SwingUtilities.invokeLater
	public void clear() {
		this.canvas.clear();
		int i = 0;
		if (this.northPanel != null) {
			this.northPanel.removeAll();
			i = 1;
		}
		if (this.eastPanel != null) {
			this.eastPanel.removeAll();
			i = 1;
		}
		if (this.southPanel != null) {
			this.southPanel.removeAll();
			i = 1;
		}
		if (this.westPanel != null) {
			this.westPanel.removeAll();
			i = 1;
		}
		if (i != 0) {
			validate();
		}
	}

	public void clearCanvas() {
		this.canvas.clear();
		repaint();
	}
	
	// JL: SwingUtilities.invokeLater
	public void addToRegion(JComponent paramJComponent, String paramString) {
		Container localJPanel = null;
		if (paramString.equalsIgnoreCase("NORTH")) {
			localJPanel = this.northPanel;
		} else if (paramString.equalsIgnoreCase("EAST")) {
			localJPanel = this.eastPanel;
		} else if (paramString.equalsIgnoreCase("SOUTH")) {
			localJPanel = this.southPanel;
		} else if (paramString.equalsIgnoreCase("WEST")) {
			localJPanel = this.westPanel;
		} else if (paramString.equalsIgnoreCase("CENTER")) {
			remove(this.canvas);
			localJPanel = this.getContentPane();
		}
		
		if (localJPanel != null) {
			localJPanel.add(paramJComponent);
			validate();
		}
	}

	// JL: SwingUtilities.invokeLater
	public void removeFromRegion(JComponent paramJComponent, String paramString) {
		JPanel localJPanel = null;
		if (paramString.equalsIgnoreCase("NORTH")) {
			localJPanel = this.northPanel;
		} else if (paramString.equalsIgnoreCase("EAST")) {
			localJPanel = this.eastPanel;
		} else if (paramString.equalsIgnoreCase("SOUTH")) {
			localJPanel = this.southPanel;
		} else if (paramString.equalsIgnoreCase("WEST")) {
			localJPanel = this.westPanel;
		} else if (paramString.equalsIgnoreCase("CENTER")) {
			remove(paramJComponent);
			add(this.canvas);
			validate();
			return;
		}
		if (localJPanel != null) {
			localJPanel.remove(paramJComponent);
			localJPanel.validate();
			validate();
		}
	}
	
	/**
	 * 
	 * @param filename must be an image type like PNG, JPG, or GIF 
	 */
	public void saveCanvasPixels(String filename) {
		// dump canvas into a BufferedImage
		BufferedImage img = canvas.toImage();
		
		// save it
		File file = new File(filename);
		try {
			String extension = IOUtils.getExtension(file).toLowerCase();
			if (!IMAGE_FILE_TYPES.contains(extension)) {
				extension = "png";   // default
			}
			ImageIO.write(img, extension, file);
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}

	// JL: SwingUtilities.invokeLater
	public void setRegionAlignment(String paramString1, String paramString2) {
		JPanel localJPanel = null;
		if (paramString1.equalsIgnoreCase("NORTH")) {
			localJPanel = this.northPanel;
		} else if (paramString1.equalsIgnoreCase("EAST")) {
			localJPanel = this.eastPanel;
		} else if (paramString1.equalsIgnoreCase("SOUTH")) {
			localJPanel = this.southPanel;
		} else if (paramString1.equalsIgnoreCase("WEST")) {
			localJPanel = this.westPanel;
		}
		int i = 10;
		if (paramString2.equalsIgnoreCase("LEFT")) {
			i = 11;
		} else if (paramString2.equalsIgnoreCase("RIGHT")) {
			i = 12;
		}
		if (localJPanel != null) {
			((TableLayout) localJPanel.getLayout()).setHorizontalAlignment(i);
		}
	}
	
	private void createSidePanels() {
		this.northPanel = new JPanel();
		this.southPanel = new JPanel();
		this.eastPanel = new JPanel();
		this.westPanel = new JPanel();
		this.northPanel.setLayout(new TableLayout(1, 0, 5, 5));
		this.southPanel.setLayout(new TableLayout(1, 0, 5, 5));
		this.westPanel.setLayout(new TableLayout(0, 1, 5, 5));
		this.eastPanel.setLayout(new TableLayout(0, 1, 5, 5));
		add(this.northPanel, "North");
		add(this.southPanel, "South");
		add(this.eastPanel, "East");
		add(this.westPanel, "West");
	}
}
