/*
 * @version 2017/12/18
 * - initial version
 */

package stanford.spl;

import java.awt.*;
import java.awt.event.WindowListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.*;

import stanford.cs106.io.IORuntimeException;
import stanford.cs106.io.IOUtils;

/**
 * A version of JBEWindow that can run in a 'headless' graphical environment such as a server.
 * It will not display on screen, but its pixels can be saved into an offscreen image.
 */
public class JBEHeadlessWindow implements JBEWindowInterface {
	private JavaBackEnd jbe;
	private String title;
	private String windowId;
	private JBECanvas canvas;
	private int x = 0;
	private int y = 0;
	private int width;
	private int height;
	private boolean open = true;
	
	public JBEHeadlessWindow(JavaBackEnd jbe, String windowId, String title, int width, int height) {
		this.jbe = jbe;
		this.windowId = windowId;
		this.title = title;
		this.canvas = new JBECanvas(windowId, width, height);
		setSize(width, height);
	}

	public void addToRegion(JComponent comp, String region) {
		// empty
	}
	
	public void addWindowListener(WindowListener listener) {
		// empty
	}

	public void clear() {
		canvas.clear();
	}

	public void clearCanvas() {
		canvas.clear();
	}

	public void close() {
		if (open) {
			open = false;
			jbe.notifyOfWindowClosed(this);
		}
	}

	public JBECanvas getCanvas() {
		return canvas;
	}

	public Dimension getCanvasSize() {
		return canvas.getSize();
	}
	
	public Container getContentPane() {
		// this is not quite right but oh well
		return canvas;
	}

	public Dimension getContentPaneSize() {
		return canvas.getSize();
	}
	
	public int getHeight() {
		return height;
	}

	public Point getLocation() {
		return new Point(x, y);
	}
	
	public Dimension getPreferredSize() {
		return getSize();
	}

	public Dimension getSize() {
		return new Dimension(width, height);
	}
	
	public String getTitle() {
		return title;
	}

	public int getWidth() {
		return width;
	}

	public String getWindowId() {
		return windowId;
	}

	public int getX() {
		return x;
	}
	
	public int getY() {
		return y;
	}
	
	public boolean isRepaintImmediately() {
		return canvas.getAutoRepaintFlag();
	}
	
	public boolean isResizable() {
		return false;
	}
	
	public boolean isVisible() {
		return true;
	}
	
	public void pack() {
		// empty
	}

	public void removeFromRegion(JComponent comp, String region) {
		// empty
	}
	
	public void removeWindowListener(WindowListener listener) {
		// empty
	}
	
	public void repaint() {
		canvas.repaint();
	}
	
	public void revalidate() {
		// empty
	}

	public void saveCanvasPixels(String filename) {
		// this seems to to not work in headless setting; hmm
		// canvas.save(filename);

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
	
	public void setDefaultCloseOperation(int operation) {
		// empty
	}

	public void setLocation(int x, int y) {
		this.x = x;
		this.y = y;
	}
	
	public void setLocation(Point location) {
		if (location != null) {
			x = location.x;
			y = location.y;
		}
	}
	
	public void setRegionAlignment(String region, String alignment) {
		// empty
	}

	public void setRepaintImmediately(boolean value) {
		canvas.setAutoRepaintFlag(value);
	}

	public void setResizable(boolean value) {
		// empty
	}
	
	public void setSize(Dimension size) {
		if (size != null) {
			setSize(size.width, size.height);
		}
	}
	
	public void setSize(int width, int height) {
		this.width = width;
		this.height = height;
		canvas.setSize(width, height);
	}
	
	public void setState(int state) {
		// empty
	}
	
	public void setTitle(String title) {
		this.title = title;
	}
	
	public void setVisible(boolean visible) {
		// empty
	}
	
	public void toBack() {
		// empty
	}
	
	public void toFront() {
		// empty
	}
	
	public void validate() {
		// empty
	}
}
