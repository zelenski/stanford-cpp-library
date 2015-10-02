/*
 * This class implements Karel's control panel.
 * 
 * @author Marty Stepp (based on Eric Roberts version)
 * @version 2015/06/20
 * - fixed bugs in Reset button
 * @version 2015/04/02
 * - enables/disables buttons when program is running
 * @version 2015/03/31
 * - commented out unused constants
 * - changed to use Swing graphical components
 * - changed speed bar to allow for slower speeds (500ms and 1000ms delay)
 * - added mnemonics on main control panel buttons
 */

package stanford.karel;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.FileDialog;
import java.awt.Label;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.util.Observable;
import java.util.Observer;
import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import acm.util.JTFTools;

public class KarelControlPanel extends CardPanel implements KarelWorldMonitor,
		ActionListener, AdjustmentListener, ChangeListener, Observer {

	/* Private state */
	private static final double SPEED_SCALE_FACTOR = 100.0;
	private static final int BUTTON_WIDTH = 100;
	private static final int BUTTON_HEIGHT = 18;
	private static final int BUTTON_SEP = 8;
	private static final int SMALL_BUTTON_WIDTH = 60;
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
	private JButton resetButton;
	private JButton loadWorldButton;
	private JButton newWorldButton;
	private JButton editWorldButton;
	private JButton saveWorldButton;
	private JButton dontSaveButton;
	private JButton okButton;
	private JButton cancelButton;
	private JSlider speedBar;
	private double speed;

	public KarelControlPanel(KarelProgram program) {
		this.program = program;
		program.StateEvent.add(this);
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
	 * This action is invoked at the beginning of an editing session.
	 */
	public void startWorldEdit() {
	}

	/**
	 * This action is invoked at the end of an editing session.
	 */
	public void endWorldEdit() {
	}

	/**
	 * This action is invoked when the mouse is clicked on a wall, which is the
	 * wall in the indicated direction from the Karel coordinates given by pt.
	 */
	public void wallAction(Point pt, int dir) {
		editor.wallAction(pt, dir);
	}

	/**
	 * This action is invoked when the mouse is clicked on a corner, which is
	 * the wall in the indicated direction from the given point.
	 */
	public void cornerAction(Point pt) {
		editor.cornerAction(pt);
	}

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
			delay = SLOW_DELAY + Math.sqrt(speed) * (FAST_DELAY - SLOW_DELAY);
		}
		if (speed < 0.98)
			JTFTools.pause(delay);
	}

	/**
	 * This method is invoked when a world map file needs to set the simulation
	 * speed (between 0 and 100).
	 */
	public void setSpeed(double speed) {
		this.speed = speed;
		// 0-100 range (=> 200-0ms delay)
		int value = (int) Math.round(SPEED_SCALE_FACTOR * speed);
		speedBar.setValue(value + 20);
	}

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
		startButton = new JButton("Start Program");
		startButton.setMnemonic('S');
		startButton.addActionListener(this);
		resetButton = new JButton("Reset Program");
		resetButton.setMnemonic('R');
		resetButton.addActionListener(this);
		loadWorldButton = new JButton("Load World");
		loadWorldButton.setMnemonic('L');
		loadWorldButton.addActionListener(this);
		newWorldButton = new JButton("New World");
		newWorldButton.setMnemonic('N');
		newWorldButton.addActionListener(this);
		editWorldButton = new JButton("Edit World");
		editWorldButton.setMnemonic('E');
		editWorldButton.addActionListener(this);

		JButton[] buttons = { startButton, /* resetButton, */ loadWorldButton,
				newWorldButton, editWorldButton };
		int maxWidth = 0;
		for (JButton button : buttons) {
			maxWidth = Math.max(maxWidth, button.getPreferredSize().width);
		}
		for (JButton button : buttons) {
			button.setPreferredSize(new Dimension(maxWidth, button
					.getPreferredSize().height));
			button.setMinimumSize(new Dimension(maxWidth, button
					.getPreferredSize().height));
		}
		vPanel.add("/center/width:100/height:18", startButton);
		vPanel.add("/center/width:100/height:18/top:8", resetButton);
		vPanel.add("/center/width:100/height:18/top:8", loadWorldButton);
		vPanel.add("/center/width:100/height:18/top:8", newWorldButton);
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

	/* ActionListener interface */
	public void actionPerformed(ActionEvent e) {
		Component source = (Component) e.getSource();
		if (source == startButton) {
			program.setStatus("Running ...");
			program.signalStarted();
		} else if (source == resetButton) {
			String filePath = world.getPathName();
			if (filePath == null) {
				program.loadInitialWorld();
			} else {
				world.load(filePath);
			}
			program.setStatus("Reset to initial state.");
		} else if (source == loadWorldButton) {
			FileDialog dialog = new LoadWorldDialog(world);
			dialog.setVisible(true);
			String fileName = dialog.getFile();
			if (fileName != null) {
				world.load(dialog.getDirectory() + "/" + fileName);
				program.setStatus("Loaded world from " + fileName + ".");
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
					program.setStatus("Saved world to " + fileName + ".");
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
		doEnabling();
	}

	private void doEnabling() {
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

	public void update(Observable obs, Object arg) {
		boolean running = arg == KarelProgram.State.RUNNING;
		startButton.setEnabled(!running);
		resetButton.setEnabled(!running);
		loadWorldButton.setEnabled(!running);
		newWorldButton.setEnabled(!running);
		editWorldButton.setEnabled(!running);
	}

	private void updateSpeed() {
		int value = speedBar.getValue();
		if (value == 0) {
			speed = -2.0;
		} else if (value == 10) {
			speed = -1.0;
		} else {
			// from 0-100, div by 100; means 0-1
			speed = (value - 20) / SPEED_SCALE_FACTOR;
		}
	}
}
