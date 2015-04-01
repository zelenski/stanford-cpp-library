/*
 * File: KarelControlPanel.java
 * ----------------------------
 * This class implements Karel's control panel.
 * - 2015/03/31: Commented out unused constants.
 * - 2015/03/31: Changed to use Swing graphical components.
 * - 2015/03/31: Changed speed bar to allow for slower speeds (500ms and 1000ms delay).
 * - 2015/03/31: Added mnemonics on main control panel buttons.
 */

package stanford.karel;

import acm.program.*;
import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;

class KarelControlPanel extends CardPanel implements KarelWorldMonitor,
		ActionListener, AdjustmentListener, ChangeListener {

	public KarelControlPanel(KarelProgram program) {
		this.program = program;
		// setBackground(Color.WHITE);
		world = program.getWorld();
		editor = createEditor();
		resizer = createResizer();
		editorPanel = createEditorPanel();
		add("editor", editorPanel);
		resizePanel = createResizePanel();
		add("resize", resizePanel);
		buttonPanel = createButtonPanel();
		add("buttons", buttonPanel);
		setView("buttons");
		setSpeed(0);
	}

	public KarelWorld getWorld() {
		return world;
	}

	public KarelProgram getProgram() {
		return program;
	}

	public KarelWorldEditor getEditor() {
		return editor;
	}

	public KarelResizer getResizer() {
		return resizer;
	}

	public Dimension getPreferredSize() {
		return new Dimension(250, 1);
	}

	/*
	 * Method: startWorldEdit Usage: startWorldEdit(); ------------------------
	 * This action is invoked at the beginning of an editing session.
	 */

	public void startWorldEdit() {
	}

	/* Method: endWorldEdit */
	/**
	 * This action is invoked at the end of an editing session.
	 */

	public void endWorldEdit() {
	}

	/* Method: wallAction */
	/**
	 * This action is invoked when the mouse is clicked on a wall, which is the
	 * wall in the indicated direction from the Karel coordinates given by pt.
	 */

	public void wallAction(Point pt, int dir) {
		editor.wallAction(pt, dir);
	}

	/* Method: cornerAction */
	/**
	 * This action is invoked when the mouse is clicked on a corner, which is
	 * the wall in the indicated direction from the given point.
	 */

	public void cornerAction(Point pt) {
		editor.cornerAction(pt);
	}

	/* Method: trace */
	/**
	 * This action is invoked when karel executes an instruction
	 */

	public void trace() {
		double delay;
		if (speed == -2.0) {
			delay = 1000;
		} else if (speed == -1.0) {
			delay = 500;
		} else {
			delay = SLOW_DELAY + Math.sqrt(speed)
					* (FAST_DELAY - SLOW_DELAY);
		}
		// System.out.println("speed = " + speed + ", delay = " + delay);
		if (speed < 0.98)
			JTFTools.pause(delay);
	}

	/* Method: setSpeed */
	/**
	 * This method is invoked when a world map file needs to set the simulation
	 * speed (between 0 and 100).
	 */

	public void setSpeed(double speed) {
		this.speed = speed;
		int value = (int) Math.round(SPEED_SCALE_FACTOR * speed);   // 0-100 range (=> 200-0ms delay)
		speedBar.setValue(value + 20);
	}

	/* Method: getSpeed */
	/**
	 * This method is invoked when the KarelWorld class needs to get the
	 * simulation speed.
	 */

	public double getSpeed() {
		return speed;
	}

	protected KarelWorldEditor createEditor() {
		return new KarelWorldEditor(getWorld());
	}

	protected JComponent createEditorPanel() {
		VPanel vbox = new VPanel();
		vbox.add("", editor);
		saveWorldButton = new JButton("Save World");
		saveWorldButton.setMnemonic('S');
		saveWorldButton.addActionListener(this);
		vbox.add("/width:" + BUTTON_WIDTH + "/height:" + BUTTON_HEIGHT
				+ "/top:" + BUTTON_SEP, saveWorldButton);
		dontSaveButton = new JButton("Don't Save");
		dontSaveButton.setMnemonic('D');
		dontSaveButton.addActionListener(this);
		vbox.add("/width:" + BUTTON_WIDTH + "/height:" + BUTTON_HEIGHT
				+ "/top:" + BUTTON_SEP, dontSaveButton);
		return vbox;
	}

	protected JComponent createButtonPanel() {
		VPanel vPanel = new VPanel();
		// vPanel.setBackground(Color.WHITE);
		startButton = new JButton("Start Program");
		startButton.setMnemonic('S');
		startButton.addActionListener(this);
		vPanel.add("/center/width:100/height:18", startButton);
		loadWorldButton = new JButton("Load World");
		loadWorldButton.setMnemonic('L');
		loadWorldButton.addActionListener(this);
		vPanel.add("/center/width:100/height:18/top:8", loadWorldButton);
		newWorldButton = new JButton("New World");
		newWorldButton.setMnemonic('N');
		newWorldButton.addActionListener(this);
		vPanel.add("/center/width:100/height:18/top:8", newWorldButton);
		editWorldButton = new JButton("Edit World");
		editWorldButton.setMnemonic('E');
		editWorldButton.addActionListener(this);
		vPanel.add("/center/width:100/height:18/top:8", editWorldButton);
		HPanel hPanel = new HPanel();
		speedBar = new JSlider(JSlider.HORIZONTAL);
		speedBar.addChangeListener(this);
		speedBar.setMaximum(0);
		speedBar.setMaximum(120);
		speedBar.setValue(20);
		speedBar.setMajorTickSpacing(60);
		speedBar.setMinorTickSpacing(10);
		speedBar.setSnapToTicks(true);
		// speedBar.setPaintLabels(true);
		speedBar.setPaintTicks(true);
		speedBar.setPaintTrack(true);
		
		hPanel.add("/center", new Label("Slow "));
		hPanel.add("/center/width:100", speedBar);
		hPanel.add("/center", new Label(" Fast"));
		vPanel.add("/center/top:8", hPanel);
		return vPanel;
	}

	protected JComponent createResizePanel() {
		VPanel vbox = new VPanel();
		cancelButton = new JButton("Cancel");
		cancelButton.addActionListener(this);
		okButton = new JButton("OK");
		okButton.addActionListener(this);
		vbox.add("", getResizer());
		vbox.add("/center/width:" + SMALL_BUTTON_WIDTH + "/space:" + GAP,
				okButton);
		vbox.add("/center/width:" + SMALL_BUTTON_WIDTH + "/space:" + GAP,
				cancelButton);
		return vbox;
	}

	protected KarelResizer createResizer() {
		return new KarelResizer();
	}
	
//	private double convertSpeed(double speed) {
//		double result = ((200 - SLOW_DELAY) / (FAST_DELAY - SLOW_DELAY));
//		return result * result;
//	}

	/* ActionListener interface */

	public void actionPerformed(ActionEvent e) {
		Component source = (Component) e.getSource();
		if (source == startButton) {
			program.setStatus("Running ...");
			program.signalStarted();
		} else if (source == loadWorldButton) {
			FileDialog dialog = new LoadWorldDialog(world);
			dialog.setVisible(true);
			String fileName = dialog.getFile();
			if (fileName != null) {
				world.load(dialog.getDirectory() + "/" + fileName);
			}
		} else if (source == newWorldButton) {
			setView("resize");
		} else if (source == editWorldButton) {
			world.setEditMode(true);
			editor.initEditorCanvas();
			setView("editor");
		} else if (source == saveWorldButton) {
			if (world.getPathname() == null) {
				FileDialog dialog = new NewWorldDialog(world);
				dialog.setVisible(true);
				String fileName = dialog.getFile();
				if (fileName != null) {
					world.setPathName(dialog.getDirectory() + "/" + fileName);
				}
			}
			world.save();
			world.setEditMode(false);
			setView("buttons");
		} else if (source == dontSaveButton) {
			world.setEditMode(false);
			setView("buttons");
		} else if (source == cancelButton) {
			setView("buttons");
		} else if (source == okButton) {
			FileDialog dialog = new NewWorldDialog(world);
			dialog.setVisible(true);
			String fileName = dialog.getFile();
			if (fileName == null) {
				setView("buttons");
			} else {
				world.init(resizer.getColumns(), resizer.getRows());
				world.setPathName(dialog.getDirectory() + "/" + fileName);
				world.setEditMode(true);
				if (world.getKarelCount() == 1) {
					Karel karel = world.getKarel();
					karel.setLocation(1, 1);
					karel.setDirection(KarelWorld.EAST);
					karel.setBeepersInBag(KarelWorld.INFINITE);
					world.repaint();
				}
				editor.initEditorCanvas();
				setView("editor");
			}
		}
	}

	/* AdjustmentListener interface */

	public void adjustmentValueChanged(AdjustmentEvent e) {
		Component source = (Component) e.getSource();
		if (source == speedBar) {
			updateSpeed();
		}
	}

	public void stateChanged(ChangeEvent e) {
		Component source = (Component) e.getSource();
		if (source == speedBar) {
			updateSpeed();
		}
	}
	
	private void updateSpeed() {
		int value = speedBar.getValue();
		if (value == 0) {
			speed = -2.0;
		} else if (value == 10) {
			speed = -1.0;
		} else {
			speed = (value - 20) / SPEED_SCALE_FACTOR;   // from 0-100, div by 100; means 0-1
		}
	}

	/* Private state */

	private static final double SPEED_SCALE_FACTOR = 100.0;
	private static final int BUTTON_WIDTH = 100;
	private static final int BUTTON_HEIGHT = 18;
	private static final int BUTTON_SEP = 8;
	// private static final int SPEEDBAR_WIDTH = 100;
	// private static final int MAX_WIDTH = 50;
	// private static final int MAX_HEIGHT = 50;
	private static final int SMALL_BUTTON_WIDTH = 60;
	// private static final int MARGIN = 8;
	private static final int GAP = 5;
	private static final double SLOW_DELAY = 200.0;
	private static final double FAST_DELAY = 0.0;

	private KarelProgram program;
	private KarelWorld world;
	private KarelWorldEditor editor;
	private KarelResizer resizer;
	private JComponent buttonPanel;
	private JComponent editorPanel;
	private JComponent resizePanel;
	private JButton startButton;
	private JButton loadWorldButton;
	private JButton newWorldButton;
	private JButton editWorldButton;
	private JButton saveWorldButton;
	private JButton dontSaveButton;
	private JButton okButton;
	private JButton cancelButton;
	private JSlider speedBar;
	private double speed;

}

class LoadWorldDialog extends FileDialog implements FilenameFilter {

	public LoadWorldDialog(KarelWorld world) {
		super(JTFTools.getEnclosingFrame(world), "Load World");
		setDirectory(KarelProgram.getWorldDirectory());
		setFilenameFilter(this);
	}

	public boolean accept(File dir, String name) {
		return (name.endsWith(".w"));
	}
}

class NewWorldDialog extends FileDialog {
	public NewWorldDialog(KarelWorld world) {
		super(JTFTools.getEnclosingFrame(world), "New World", FileDialog.SAVE);
		setDirectory(KarelProgram.getWorldDirectory());
	}
}

class KarelResizer extends Panel implements AdjustmentListener {
	public KarelResizer() {
		resizeCanvas = new ResizeCanvas();
		resizeCanvas.setDimension(10, 10);
		widthScrollbar = new Scrollbar(Scrollbar.HORIZONTAL);
		widthScrollbar.setValues(10 - 1, 1, 0, MAX_WIDTH);
		widthScrollbar.addAdjustmentListener(this);
		heightScrollbar = new Scrollbar(Scrollbar.VERTICAL);
		heightScrollbar.setValues(MAX_HEIGHT - 10, 1, 0, MAX_HEIGHT);
		heightScrollbar.addAdjustmentListener(this);
		setLayout(new ResizeLayout());
		add("canvas", resizeCanvas);
		add("hbar", widthScrollbar);
		add("vbar", heightScrollbar);
	}

	public int getColumns() {
		return widthScrollbar.getValue() + 1;
	}

	public int getRows() {
		return MAX_HEIGHT - heightScrollbar.getValue();
	}

	/* AdjustmentListener interface */

	public void adjustmentValueChanged(AdjustmentEvent e) {
		resizeCanvas.setDimension(getColumns(), getRows());
		resizeCanvas.repaint();
	}

	/* Private instance variables */

	private static final int MAX_WIDTH = 50;
	private static final int MAX_HEIGHT = 50;

	private ResizeCanvas resizeCanvas;
	private Scrollbar widthScrollbar;
	private Scrollbar heightScrollbar;

}

class ResizeLayout implements LayoutManager {

	public void addLayoutComponent(String constraints, Component comp) {
		if (constraints.equals("canvas"))
			canvas = comp;
		if (constraints.equals("hbar"))
			hbar = comp;
		if (constraints.equals("vbar"))
			vbar = comp;
	}

	public void removeLayoutComponent(Component comp) {
		/* Empty */
	}

	public Dimension preferredLayoutSize(Container parent) {
		return minimumLayoutSize(parent);
	}

	public Dimension minimumLayoutSize(Container parent) {
		synchronized (parent.getTreeLock()) {
			Dimension csize = canvas.getPreferredSize();
			int hsize = hbar.getPreferredSize().height;
			int vsize = vbar.getPreferredSize().width;
			return new Dimension(csize.width + vsize + 1, csize.height + hsize
					+ 1);
		}
	}

	public void layoutContainer(Container parent) {
		synchronized (parent.getTreeLock()) {
			Dimension psize = parent.getSize();
			Insets insets = parent.getInsets();
			int x = insets.left;
			int y = insets.top;
			int width = psize.width - insets.left - insets.right;
			int height = psize.height - insets.top - insets.bottom;
			int hsize = hbar.getPreferredSize().height;
			int vsize = vbar.getPreferredSize().width;
			canvas.setBounds(x, y, width - vsize - 1, height - hsize - 1);
			hbar.setBounds(x, y + height - vsize, width - vsize - 1, hsize);
			vbar.setBounds(x + width - hsize, y, vsize, height - hsize - 1);
		}
	}

	private Component canvas;
	private Component hbar;
	private Component vbar;
}

class ResizeCanvas extends Canvas {

	public void setDimension(int width, int height) {
		this.cols = width;
		this.rows = height;
	}

	public Dimension getPreferredSize() {
		return new Dimension(SIZE, SIZE);
	}

	public void paint(Graphics g) {
		g.setColor(Color.BLACK);
		g.drawRect(0, 0, SIZE - 1, SIZE - 1);
		int sqSize = (SIZE - 2) / Math.max(rows, cols);
		int x = (SIZE - sqSize * (cols - 1)) / 2;
		for (int ix = 1; ix <= cols; ix++) {
			int y = (SIZE + sqSize * (rows - 1)) / 2;
			for (int iy = 1; iy <= rows; iy++) {
				drawCornerMarker(g, x, y);
				y -= sqSize;
			}
			x += sqSize;
		}
		String str = cols + "x" + rows;
		g.setFont(FONT);
		FontMetrics fm = g.getFontMetrics();
		int width = fm.stringWidth(str) + 6;
		int height = fm.getHeight() + 2;
		g.setColor(Color.WHITE);
		g.fillRect((SIZE - width) / 2, (SIZE - height) / 2, width, height);
		g.setColor(Color.BLACK);
		g.drawString(str, (SIZE - fm.stringWidth(str)) / 2,
				(SIZE + fm.getAscent()) / 2);
	}

	private void drawCornerMarker(Graphics g, int x, int y) {
		if (sqSize < KarelWorld.CROSS_THRESHOLD) {
			g.drawLine(x, y, x, y);
		} else {
			g.drawLine(x - 1, y, x + 1, y);
			g.drawLine(x, y - 1, x, y + 1);
		}
	}

	private static final int SIZE = 102;
	private static final Font FONT = new Font("Helvetica", Font.PLAIN, 12);

	private int rows;
	private int cols;
	private int sqSize;

}
