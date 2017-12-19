/*
 * @version 2017/12/18
 * - initial version
 */

package stanford.spl;

import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import stanford.cs106.util.*;

/**
 * Common parent of JBEWindow and JBEHeadlessWindow.
 */
public interface JBEWindowInterface {
	public static final Set<String> IMAGE_FILE_TYPES = CollectionUtils.asSet(
			"gif", "jpg", "jpeg", "png"
	);
	
	public void addToRegion(JComponent comp, String region);
	public void addWindowListener(WindowListener listener);
	public void clear();
	public void clearCanvas();
	public void close();
	public JBECanvas getCanvas();
	public Dimension getCanvasSize();
	public Container getContentPane();
	public Dimension getContentPaneSize();
	public int getHeight();
	public Point getLocation();
	public Dimension getPreferredSize();
	public Dimension getSize();
	public String getTitle();
	public int getWidth();
	public String getWindowId();
	public int getX();
	public int getY();
	public boolean isRepaintImmediately();
	public boolean isResizable();
	public boolean isVisible();
	public void pack();
	public void removeFromRegion(JComponent comp, String region);
	public void removeWindowListener(WindowListener listener);
	public void repaint();
	public void revalidate();
	public void saveCanvasPixels(String filename);
	public void setDefaultCloseOperation(int operation);
	public void setLocation(int x, int y);
	public void setLocation(Point location);
	public void setRegionAlignment(String region, String alignment);
	public void setRepaintImmediately(boolean value);
	public void setResizable(boolean value);
	public void setState(int state);
	public void setSize(Dimension size);
	public void setSize(int width, int height);
	public void setTitle(String title);
	public void setVisible(boolean visible);
	public void toBack();
	public void toFront();
	public void validate();
}
