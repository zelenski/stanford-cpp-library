/*
 * This file implements the KarelProgram class.
 * 
 * @author Marty Stepp (based on Eric Roberts version)
 * @version 2015/04/05
 * - added "Options" menu bar for Ms. Karel mode
 * @version 2015/04/01
 * - improved scrollbar appearance in error dialogs
 * - added "About" menu bar
 * @version 2015/03/31
 * - commented out unused field 'program'
 * - changed to use Swing graphical components
 * - improved error dialog to display program stack trace
 * - added setStatus method to display program state
 */

package stanford.karel;

import acm.program.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

/**
 * This class is a subclass of <code><a href="Program.html">Program</a></code>
 * that runs a Karel program.
 */
public class KarelProgram extends Program {
	/** Directory where *.w Karel world files are found. */
	public static final String WORLDS_DIRECTORY = "worlds";
	
	/** File extension for Karel worlds. */
	public static final String WORLD_EXTENSION = ".w";
	
	public static final int NORTH = KarelWorld.NORTH;
	public static final int EAST = KarelWorld.EAST;
	public static final int SOUTH = KarelWorld.SOUTH;
	public static final int WEST = KarelWorld.WEST;

	public static final int INFINITE = KarelWorld.INFINITE;

	public static final int SIMPLE = KarelWorld.SIMPLE;
	public static final int FANCY = KarelWorld.FANCY;

	public static final Color BLACK = Color.BLACK;
	public static final Color BLUE = Color.BLUE;
	public static final Color CYAN = Color.CYAN;
	public static final Color DARK_GRAY = Color.DARK_GRAY;
	public static final Color GRAY = Color.GRAY;
	public static final Color GREEN = Color.GREEN;
	private static final Color DARK_YELLOW = Color.YELLOW.darker().darker().darker();
	private static final Color DARK_GREEN = Color.GREEN.darker().darker();
	private static final Color DARK_RED = Color.RED.darker();
	// private static final Color DARK_BLUE = Color.BLUE.darker();
	public static final Color LIGHT_GRAY = Color.LIGHT_GRAY;
	public static final Color MAGENTA = Color.MAGENTA;
	public static final Color ORANGE = Color.ORANGE;
	public static final Color PINK = Color.PINK;
	public static final Color RED = Color.RED;
	public static final Color WHITE = Color.WHITE;
	public static final Color YELLOW = Color.YELLOW;
	
	private static final String CONFIG_FILE_NAME = "karel-settings.txt";

	/* Private state */
	private KarelWorld world;
	private KarelControlPanel controlPanel;
	private KarelErrorDialog errorDialog;
	private JLabel statusLabel;
	private JCheckBoxMenuItem msKarelItem;
	private boolean started;
	
	public static enum State { RUNNING, STOPPED, ERROR };
	public Event<State> StateEvent = new Event<State>("StateEvent");

	/**
	 * Creates a new Karel program.
	 */
	public KarelProgram() {
		world = createWorld();
		world.setRepaintFlag(false);
		world.setDisplayFlag(false);
		world.init(10, 10);
		JPanel panel = new JPanel();
		panel.setLayout(new BorderLayout());
		panel.add(BorderLayout.CENTER, world);
		controlPanel = new KarelControlPanel(this);
		world.setMonitor(controlPanel);
		panel.add(BorderLayout.WEST, controlPanel);
		add(panel);
		
		// set up status label
		statusLabel = new JLabel("Welcome to Karel!");
		statusLabel.setFont(statusLabel.getFont().deriveFont(Font.BOLD));
		add(statusLabel, BorderLayout.SOUTH);

		validate();
	}

	/**
	 * Contains the code to be executed for each specific program subclass. If
	 * you are defining your own <code>KarelProgram</code> class, you need to
	 * override the definition of <code>main</code> so that it contains the code
	 * for your application.
	 */

	public void main() {
		/* Empty */
	}
	
	public void setStatus(String status) {
		if (status.contains("Running")) {
			statusLabel.setForeground(DARK_YELLOW);
		} else if (status.contains("due to an error")) {
			statusLabel.setForeground(DARK_RED);
		} else if (status.contains("Finished")) {
			statusLabel.setForeground(DARK_GREEN);
		} else {
			// put back to default label color
			statusLabel.setForeground(new JLabel().getForeground());
		}
		statusLabel.setText(status);
	}

	/**
	 * Returns the <code>KarelWorld</code> object in which Karel lives.
	 * 
	 * @return The <code>KarelWorld</code> object in which Karel lives
	 */
	public KarelWorld getWorld() {
		return world;
	}

	/**
	 * Returns the default directory in which Karel's worlds live.
	 * 
	 * @return The directory in which Karel's worlds lives
	 */
	public static String getWorldDirectory() {
		String dir = System.getProperty("user.dir");
		if (new File(dir, WORLDS_DIRECTORY).isDirectory()) {
			dir += File.separator + WORLDS_DIRECTORY;
		}
		return dir;
	}

	/**
	 * Creates the <code>KarelWorld</code> in which Karel lives. Subclasses can
	 * override this method to create their own <code>KarelWorld</code> types.
	 * 
	 * @return The <code>World</code> object in which Karel lives
	 * @noshow student
	 */
	protected KarelWorld createWorld() {
		return new KarelWorld();
	}

	/**
	 * Checks to see whether this program has started.
	 * 
	 * @noshow student
	 */
	protected boolean isStarted() {
		if (world == null || !super.isStarted())
			return false;
		Dimension size = world.getSize();
		return (size != null) && (size.width != 0) && (size.height != 0);
	}

	protected Karel getKarel() {
		return world.getKarel();
	}

	public void add(Karel karel) {
		add(karel, 1, 1, KarelWorld.EAST, KarelWorld.INFINITE);
	}

	public void add(Karel karel, int x, int y, int dir, int nBeepers) {
		karel.setLocation(x, y);
		karel.setDirection(dir);
		karel.setBeepersInBag(nBeepers);
		world.add(karel);
	}

	public void start(String[] args) {
		super.start(args);
	}

	protected void setStartupObject(Object obj) {
		super.setStartupObject(obj);
	}
	
	protected void augmentMenuBar() {
		// try to set up JMenuBar
		try {
			ProgramMenuBar bar = getConsole().getMenuBar();
			bar.removeAll();
			
			JMenu fileMenu = new JMenu("File");
			fileMenu.setMnemonic('F');
			JMenuItem quitItem = new JMenuItem("Quit");
			quitItem.setMnemonic('Q');
			quitItem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent event) {
					stop();
					destroy();
					setVisible(false);
					try {
						System.exit(0);
					} catch (Exception e) {
						// empty
					}
				}
			});
			fileMenu.add(quitItem);
			
			JMenu optionsMenu = new JMenu("Options");
			optionsMenu.setMnemonic('O');
			msKarelItem = new JCheckBoxMenuItem("Ms. Karel");
			msKarelItem.setMnemonic('M');
			msKarelItem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent event) {
					world.setMsKarel(msKarelItem.isSelected());
					saveConfiguration();
				}
			});
			optionsMenu.add(msKarelItem);
			
			JMenu helpMenu = new JMenu("Help");
			helpMenu.setMnemonic('H');
			JMenuItem aboutItem = new JMenuItem("About Karel...");
			aboutItem.setActionCommand("About Karel...");
			aboutItem.setMnemonic('A');
			aboutItem.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent event) {
					JOptionPane.showMessageDialog(KarelProgram.this, Version.ABOUT_MESSAGE);
				}
			});
			helpMenu.add(aboutItem);
			
			bar.add(fileMenu);
			bar.add(optionsMenu);
			bar.add(helpMenu);
		} catch (Exception e) {
			// empty
		}
		
	}
	
	protected void loadConfiguration(Properties props) {
		try {
			String tmpDir = System.getProperty("java.io.tmpdir");
			if (tmpDir != null) {
				File configFile = new File(tmpDir, CONFIG_FILE_NAME);
				if (!configFile.exists()) {
					return;
				}
				FileInputStream input = new FileInputStream(configFile);
				props.load(input);
				input.close();
				
				if (props.containsKey("mskarel")) {
					boolean msKarel = Boolean.parseBoolean(String.valueOf(props.get("mskarel")));
					msKarelItem.setSelected(msKarel);
					world.setMsKarel(msKarel);
				}
			}
		} catch (Exception ex) {
			// ex.printStackTrace();
			// empty
		}
	}
	
	protected void saveConfiguration(Properties props) {
		try {
			String tmpDir = System.getProperty("java.io.tmpdir");
			if (tmpDir != null) {
				File configFile = new File(tmpDir, CONFIG_FILE_NAME);
				props.put("mskarel", String.valueOf(msKarelItem.isSelected()));
				FileOutputStream out = new FileOutputStream(configFile);
				props.store(out, "Karel configuration file");
				out.close();
			}
		} catch (Exception ex) {
			// ex.printStackTrace();
			// empty
		}
	}
	
	public void loadInitialWorld() {
		String karelClass = getParameter("karel");
		if (karelClass == null) {
			Karel karel = (Karel) getStartupObject();
			karelClass = karel.getClass().getName();
			karelClass = karelClass.substring(karelClass.lastIndexOf(".") + 1);
		}

		String worldName = getParameter("world");
		if (worldName == null)
			worldName = karelClass;
		try {
			URL url = new URL(getCodeBase(), WORLDS_DIRECTORY + File.separator + worldName + WORLD_EXTENSION);
			URLConnection connection = url.openConnection();
			world.load(new InputStreamReader(connection.getInputStream()));
		} catch (Exception ex) {
			/* Ignore this error */
		}
	}

	protected void startRun() {
		augmentMenuBar();
		if (!isAppletMode()) {
			loadConfiguration();
		}
		
		Karel karel = null;
		String karelClass = getParameter("karel");
		if (karelClass == null) {
			karel = (Karel) getStartupObject();
			karelClass = karel.getClass().getName();
			karelClass = karelClass.substring(karelClass.lastIndexOf(".") + 1);
		} else {
			try {
				karel = (Karel) Class.forName(karelClass).newInstance();
			} catch (Exception ex) {
				System.out.println("Exception: " + ex);
			}
		}
		if (karel != null) {
			world.add(karel);
			setTitle(karelClass);
			loadInitialWorld();
		}
		world.setRepaintFlag(true);
		world.setDisplayFlag(true);
		world.repaint();
		while (true) {
			started = false;
			synchronized (this) {
				while (!started) {
					try {
						wait();
					} catch (InterruptedException ex) {
						/* Empty */
					}
				}
			}
			
			Throwable throwable = null;
			try {
				if (karel == null) {
					main();
				} else {
					karel.run();
					setStatus("Finished running.");
				}
				world.setRepaintFlag(true);
				world.repaint();
			} catch (Exception ex) {
				throwable = ex;
			} catch (StackOverflowError err) {
				throwable = err;
			}
			
			if (throwable == null) {
				StateEvent.fire(State.STOPPED);
			} else {
				setStatus("Halted due to an error.");
				StateEvent.fire(State.ERROR);
				if (errorDialog == null) {
					errorDialog = new KarelErrorDialog(this);
				}
				String errorMessage = throwable.getMessage();
				if (errorMessage == null || errorMessage.isEmpty()) {
					errorMessage = "";
					if (throwable instanceof StackOverflowError) {
						errorMessage = "Infinite method calls.";
					}
				}
				while (!errorMessage.endsWith("\n\n")) {
					errorMessage += "\n";
				}
				ByteArrayOutputStream bytes = new ByteArrayOutputStream();
				PrintStream out = new PrintStream(bytes);
				throwable.printStackTrace(out);
				String stackTrace = bytes.toString();
				errorDialog.error(errorMessage, stackTrace);
			}
		}
	}

	void signalStarted() {
		synchronized (this) {
			StateEvent.fire(State.RUNNING);
			started = true;
			notifyAll();
		}
	}
}
