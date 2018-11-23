/*
 * @version 2017/07/21
 * - added add() overloads
 */

package stanford.karel;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Point;
import java.io.*;
import java.util.*;
import acm.util.ErrorException;
import stanford.cs106.io.IORuntimeException;

/**
 * A version of a Karel superclass that doesn't crash when run
 * in headless (non-graphical) mode.
 */
public class HeadlessKarel extends AbstractKarel {
	private static final String KAREL_WORLD_FILENAME = "stanford.karel.world";
	private static final String KAREL_WORLD_SAVE_FILENAME = "stanford.karel.save";
	
	private static boolean exitEnabled = false;
	private boolean trackLocationsVisited = false;
	private Set<Point> locationsVisited = new LinkedHashSet<Point>();
	
	private String saveFile;
	private String worldFile;

	/**
	 * Exits the program.
	 * Saves to a file first if needed.
	 */
	public final void exit() {
		String outFileName = getSaveFile();
		if (outFileName != null && getWorld() != null) {
			getWorld().saveToImage(outFileName);
		}
		
		if (exitEnabled) {
			try {
				System.exit(0);
			} catch (Exception e) {
				// empty
			}
		}
	}
	
	public final Set<Point> getLocationsVisited() {
		return Collections.unmodifiableSet(locationsVisited);
	}
	
	protected final String getSaveFile() {
		if (saveFile == null) {
			// fall back to a default passed through a System property
			return System.getProperty(KAREL_WORLD_SAVE_FILENAME);
		} else {
			return saveFile;
		}
	}
	
	protected final String getWorldFile() {
		if (worldFile == null) {
			// fall back to a default (same name as the class), e.g. "worlds/HelloKarel.w",
			// or use a file passed through a System property
			String defaultFileName = KarelProgram.WORLDS_DIRECTORY + File.separator
					+ getClass().getSimpleName() + KarelProgram.WORLD_EXTENSION;
			String filename = System.getProperty(KAREL_WORLD_FILENAME, defaultFileName);
			worldFile = filename;
		}
		return worldFile;
	}
	
	public final boolean hasVisited(int x, int y) {
		return hasVisited(new Point(x, y));
	}
	
	public final boolean hasVisited(Point pt) {
		return locationsVisited.contains(pt);
	}
	
	/**
	 * Initializes Karel.
	 * You cannot override this method; we need it to be called as-is.
	 */
	public final void init() {
		// load initial world
		String worldFile = getWorldFile();

		KarelWorld world = new KarelWorld();
		world.add(this);
		setWorld(world);
		world.init(10, 10);   // temporary size
		
		try {
			world.load(new InputStreamReader(new FileInputStream(worldFile)));
			world.setToDefaultSize();

			if (trackLocationsVisited) {
				locationsVisited.add(getLocation());
			}
		} catch (IOException ioe) {
			throw new ErrorException("Unable to load Karel world from " + worldFile, ioe);
		}
	}
	
	public final void move() {
		super.move();
		if (trackLocationsVisited) {
			locationsVisited.add(getLocation());
		}
	}
	
	public void run() {
		// empty
	}
	
	public static void setExitEnabled(boolean enabled) {
		exitEnabled = enabled;
	}
	
	public final void setSaveFile(String filename) {
		saveFile = filename;
	}
	
	public final void setTrackLocationsVisited(boolean track) {
		trackLocationsVisited = track;
	}
	
	public final void setWorldFile(String filename) {
		if (!new File(filename).exists()) {
			throw new IORuntimeException("Karel world file not found: " + filename);
		}
		worldFile = filename;
	}
	
	public final void start() {
		init();
		run();
		exit();
	}

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

	public void addActionListeners() {
		// empty
	}

	public void addMouseListeners() {
		// empty
	}

	public String getTitle() {
		return "";
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

	public void setBackground(Color color) {
		// empty
	}

	public void setFont(Font font) {
		// empty
	}

	public void setForeground(Color color) {
		// empty
	}

	public void setTitle(String title) {
		// empty
	}
}
