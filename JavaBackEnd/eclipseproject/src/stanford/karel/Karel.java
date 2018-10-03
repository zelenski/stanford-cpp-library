/*
 * This class implements the basic Karel-the-Robot object.
 * 
 * @version 2018/10/02
 * - added echo functionality for screen readers
 * - alphabetized methods
 * @version 2015/03/31
 * - fixed warning by closing resources
 * - commented out unused constant INFINITE
 * - changed to OS system look and feel
 */

package stanford.karel;

import acm.graphics.GObject;
import acm.program.*;
import acm.util.*;
import stanford.cs106.gui.GuiUtils;
import stanford.cs106.reflect.ClassUtils;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.*;
import java.util.*;
import java.util.zip.*;

/**
 * The <code>Karel</code> class represents the simplest possible Karel the Robot
 * object. Each instance of the <code>Karel</code> class represents an
 * individual robot that can move inside a world laid out as a grid of streets
 * and avenues. The other characteristics of Karel's world include
 * <b><i>walls,</i></b> which separate individual corners, and
 * <b><i>beepers,</i></b> which are described by Karel's creator as
 * "plastic cones which emit a quiet beeping noise." As it is shipped from the
 * factory, Karel can execute only four operations:
 * 
 * <dl>
 * <dt><code><a href="#move()">move</a>()</code></dt>
 * <dd>Moves this Karel forward one block. Karel cannot move forward if there is
 * a wall blocking the way.</dd>
 * <p>
 * <dt><code><a href="#turnLeft()">turnLeft</a>()</code></dt>
 * <dd>Rotates this Karel 90 degrees to the left (counterclockwise).</dd>
 * <p>
 * <dt><code><a href="#pickBeeper()">pickBeeper</a>()</code></dt>
 * <dd>Picks up one beeper from the current corner and stores the beeper in this
 * Karel's beeper bag, which can hold an infinite number of beepers. Karel can
 * execute a <code>pickBeeper</code> instruction only if there is a beeper on
 * the current corner.</dd>
 * <p>
 * <dt><code><a href="#putBeeper()">putBeeper</a>()</code></dt>
 * <dd>Takes one beeper from this Karel's beeper bag and deposits it on the
 * current corner. Karel can execute a <code>pickBeeper</code> instruction only
 * if there is a beeper in its bag.</dd>
 * <p>
 * </dl>
 * 
 * <p>
 * Karel programs are executed by defining a new subclass that extends
 * <code>Karel</code> and provides it with a <code>run</code> method that
 * defines its operation. For example, the following Karel subclass represents a
 * Karel program that moves forward, picks up a beeper from the square, and then
 * moves forward one more time:
 * 
 * <p>
 * <table cellspacing=0 cellpadding=0>
 * <tr>
 * <td width=36>&nbsp;</td>
 * <td>
 * <table cellspacing=0 cellpadding=0>
 * <tr>
 * <td>
 * 
 * <pre>
 * <code>
 * public class SimpleKarelExample extends Karel {
 * &nbsp;&nbsp;&nbsp;public void run() {
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;move();
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;putBeeper();
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;move();
 * &nbsp;&nbsp;&nbsp;}
 * }
 * </code>
 * </pre>
 * 
 * </td>
 * </tr>
 * </table>
 * </td>
 * </tr>
 * </table>
 */
public class Karel extends Program implements KarelInterface, Runnable {
	/* Private constants */
	private static final int NORTH = KarelWorld.NORTH;
	private static final int EAST = KarelWorld.EAST;
	private static final int SOUTH = KarelWorld.SOUTH;
	private static final int WEST = KarelWorld.WEST;
	// private static final int INFINITE = KarelWorld.INFINITE;

	/* List of jar files to skip */
	private static final String[] SKIP_JARS = { "acm.jar", "karel.jar", };

	/* Private instance variables */
	private KarelWorld world;
	private int x;
	private int y;
	private int dir;
	private int beepers;
	private boolean echo;   // for blind accessibility

	/* Constructor */
	public Karel() {
		x = 1;
		y = 1;
		dir = EAST;
		world = null;
		echo = false;
	}

	public boolean beepersInBag() {
		checkWorld("beepersInBag");
		return getBeepersInBag() > 0;
	}

	public boolean beepersPresent() {
		checkWorld("beepersPresent");
		return world.getBeepersOnCorner(x, y) > 0;
	}

	protected void checkWorld(String caller) {
		if (world == null) {
			throwErrorException(caller
					+ ": Karel is not living in a world");
		}
	}

	public boolean facingEast() {
		checkWorld("facingEast");
		return dir == EAST;
	}

	public boolean facingNorth() {
		checkWorld("facingNorth");
		return dir == NORTH;
	}

	public boolean facingSouth() {
		checkWorld("facingSouth");
		return dir == SOUTH;
	}

	public boolean facingWest() {
		checkWorld("facingWest");
		return dir == WEST;
	}

	public boolean frontIsBlocked() {
		checkWorld("frontIsBlocked");
		return world.checkWall(x, y, dir);
	}

	public boolean frontIsClear() {
		checkWorld("frontIsClear");
		return !world.checkWall(x, y, dir);
	}

	public int getBeepersInBag() {
		return (beepers);
	}

	public int getDirection() {
		return dir;
	}

	public Point getLocation() {
		return new Point(x, y);
	}

	public KarelWorld getWorld() {
		return world;
	}

	public boolean leftIsBlocked() {
		checkWorld("leftIsBlocked");
		return world.checkWall(x, y, KarelWorld.leftFrom(dir));
	}

	public boolean leftIsClear() {
		checkWorld("leftIsClear");
		return !world.checkWall(x, y, KarelWorld.leftFrom(dir));
	}

	protected void maybeEcho(String text) {
		if (echo) {
			System.err.flush();
			pause(10);
			System.out.println(text);
			System.out.flush();
		}
	}
	
	protected void maybeEchoError(String text) {
		if (echo) {
			System.out.println();
			System.out.flush();
			pause(10);
			System.err.println(text);
			System.err.flush();
		}
	}
	
	public void maybeEchoCurrentState() {
		Point loc = getLocation();
		int nCorner = world.getBeepersOnCorner(loc);
		Color color = world.getCornerColor(loc);
		maybeEcho("  Karel is at " + loc.x + ", " + loc.y
				+ (nCorner > 0 ? ", " + nCorner + " beeper" + (nCorner == 1 ? "" : "s") : "")
				+ (color != null ? ", color " + GObject.colorNameFriendly(color) : ""));
	}
	
	public void maybeEchoFinalState() {
		// world size
		maybeEcho("Final state:");

		Point loc = getLocation();
		int nCorner = world.getBeepersOnCorner(loc);
		Color color = world.getCornerColor(loc);
		maybeEcho("  Karel ends at location " + loc.x + ", " + loc.y
				+ (nCorner > 0 ? ", which contains " + nCorner + " beeper" + (nCorner == 1 ? "" : "s") : "")
				+ (color != null ? ", color " + GObject.colorNameFriendly(color) : ""));
		
		int nBag = getBeepersInBag();
		if (nBag != KarelWorld.INFINITE) {
			maybeEcho("  Beepers in bag:    " + nBag);
		}
	}
	
	public void maybeEchoInitialState() {
		// world size
		maybeEcho("Karel is ready! Initial state:");
		maybeEcho("  Karel is in a world of size "
				+ world.getColumns() + " by " + world.getRows()); 

		Point loc = getLocation();
		int nCorner = world.getBeepersOnCorner(loc);
		Color color = world.getCornerColor(loc);
		maybeEcho("  Karel begins at location " + loc.x + ", " + loc.y
				+ (nCorner > 0 ? ", which contains " + nCorner + " beeper" + (nCorner == 1 ? "" : "s") : "")
				+ (color != null ? ", color " + GObject.colorNameFriendly(color) : ""));
		
		int nBag = getBeepersInBag();
		maybeEcho("  Beepers in bag: " + (nBag == KarelWorld.INFINITE ? "infinite" : String.valueOf(nBag)));
		maybeEcho("Press Enter to run.");
	}
	
	@Override
	public boolean menuAction(ActionEvent event) {
		return super.menuAction(event);
	}

	public void move() {
		maybeEcho("move");
		checkWorld("move");
		if (world.checkWall(x, y, dir)) {
			throwErrorException("Karel is blocked");
		}
		Point loc = KarelWorld.adjacentPoint(x, y, dir);
		setLocation(loc);
		world.trace();
		maybeEchoCurrentState();
	}

	public boolean noBeepersPresent() {
		checkWorld("noBeepersPresent");
		return world.getBeepersOnCorner(x, y) == 0;
	}

	public boolean noBeepersInBag() {
		checkWorld("noBeepersInBag");
		return getBeepersInBag() == 0;
	}

	public boolean notFacingEast() {
		checkWorld("notFacingEast");
		return dir != EAST;
	}

	public boolean notFacingNorth() {
		checkWorld("notFacingNorth");
		return dir != NORTH;
	}

	public boolean notFacingSouth() {
		checkWorld("notFacingSouth");
		return dir != SOUTH;
	}

	public boolean notFacingWest() {
		checkWorld("notFacingWest");
		return dir != WEST;
	}

	public void pickBeeper() {
		maybeEcho("pick beeper");
		checkWorld("pickBeeper");
		int nCorner = world.getBeepersOnCorner(x, y);
		if (nCorner < 1) {
			throwErrorException("pickBeeper: No beepers on this corner");
		}
		nCorner = KarelWorld.adjustBeepers(nCorner, -1);
		world.setBeepersOnCorner(x, y, nCorner);
		
		int nBag = getBeepersInBag();
		nBag = KarelWorld.adjustBeepers(nBag, +1);
		setBeepersInBag(nBag);
		world.trace();
		
		if (nCorner > 0) {
			maybeEcho("  Beepers still on corner: " + nCorner);
		}
		if (nBag != KarelWorld.INFINITE) {
			maybeEcho("  Beepers in bag: " + nBag);
		}
	}

	public void putBeeper() {
		maybeEcho("put beeper");
		checkWorld("putBeeper");
		int nBag = getBeepersInBag();
		if (nBag < 1) {
			throwErrorException("putBeeper: No beepers in bag");
		}
		nBag = KarelWorld.adjustBeepers(nBag, -1);
		setBeepersInBag(nBag);
		
		int nCorner = world.getBeepersOnCorner(x, y);
		nCorner = KarelWorld.adjustBeepers(nCorner, +1);
		world.setBeepersOnCorner(x, y, nCorner);
		world.trace();
		
		maybeEcho("  Beepers on corner: " + nCorner);
		if (nBag != KarelWorld.INFINITE) {
			maybeEcho("  Beepers in bag:    " + nBag);
		}
	}

	public boolean rightIsBlocked() {
		checkWorld("rightIsBlocked");
		return world.checkWall(x, y, KarelWorld.rightFrom(dir));
	}

	public boolean rightIsClear() {
		checkWorld("rightIsClear");
		return !world.checkWall(x, y, KarelWorld.rightFrom(dir));
	}

	public void run() {
		/* Empty */
	}
	
	public void setEcho() {
		setEcho(true);
	}
	
	public void setEcho(boolean echo) {
		this.echo = echo;
	}
	
	public void setLocation(int x, int y) {
		if (world != null) {
			if (world.outOfBounds(x, y)) {
				throwErrorException("setLocation: Out of bounds");
			}
			Karel occupant = world.getKarelOnSquare(x, y);
			if (occupant == this)
				return;
			if (occupant != null) {
				throwErrorException("setLocation: Square is occupied");
			}
		}
		int x0 = this.x;
		int y0 = this.y;
		this.x = x;
		this.y = y;
		if (world != null) {
			world.updateCorner(x, y);
			world.updateCorner(x0, y0);
		}
	}

	public void setLocation(Point pt) {
		setLocation(pt.x, pt.y);
	}

	public void setDirection(int dir) {
		this.dir = dir;
		if (world != null)
			world.updateCorner(x, y);
	}

	public void setBeepersInBag(int nBeepers) {
		beepers = nBeepers;
	}

	public void setWorld(KarelWorld world) {
		this.world = world;
	}

	public void setDisplayOneFlag(boolean flag) {
		world.setDisplayOneFlag(flag);
	}

	/**
	 * Starts a <code>KarelProgram</code> containing this Karel instance,
	 * passing it the specified arguments.
	 */
	public void start(String[] args) {
		KarelProgram program = new KarelProgram();
		program.setStartupObject(this);
		program.start(args);
	}
	
	protected void throwErrorException(String message) {
		ErrorException ex = new ErrorException(message);
		if (echo) {
			String errorEchoText = "Error: " + message + "\n";
			for (StackTraceElement stack : ex.getStackTrace()) {
				String clazz = stack.getClassName();
				if (clazz.startsWith("acm.")
						|| clazz.startsWith("stanford.")
						|| clazz.startsWith("java.")
						|| clazz.startsWith("javax.")
						|| clazz.startsWith("javazoom.")
						|| clazz.startsWith("com.")
						|| clazz.startsWith("sun.")) {
					// don't print library classes
					continue;
				}
				errorEchoText += "  " + stack.getFileName() + ", line " + stack.getLineNumber()
						+ ", " + ClassUtils.stripPackages(stack.getClassName()) + "." + stack.getMethodName() + "\n";
			}
			maybeEchoError(errorEchoText);
		}
		throw ex;
	}

	public void turnLeft() {
		maybeEcho("turn left");
		checkWorld("turnLeft");
		int newdir = KarelWorld.leftFrom(dir);
		setDirection(newdir);
		world.trace();
		maybeEcho("  Karel is facing " + KarelWorld.directionName(newdir).toLowerCase());
	}


	private static String getMainClassName() {
		String result = null;
		String classpath = System.getProperty("java.class.path");
		if (classpath == null)
			classpath = System.getProperty("user.dir");
		if (classpath == null)
			return null;
		StringTokenizer tokenizer = new StringTokenizer(classpath, ":;");
		while (tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			File file = new File(token);
			String[] candidates = null;
			if (file.isDirectory()) {
				candidates = file.list();
			} else if (token.endsWith(".jar") && !nameAppears(token, SKIP_JARS)) {
				try {
					ZipFile zf = new ZipFile(file);
					ArrayList<String> list = new ArrayList<String>();
					Enumeration<?> entries = zf.entries();
					while (entries.hasMoreElements()) {
						list.add(((ZipEntry) entries.nextElement()).getName());
					}
					candidates = new String[list.size()];
					for (int i = 0; i < candidates.length; i++) {
						candidates[i] = list.get(i);
					}
					zf.close();
				} catch (IOException ex) {
					candidates = null;
				}
			}
			if (candidates != null) {
				for (int i = 0; i < candidates.length; i++) {
					String fileName = candidates[i];
					if (fileName.endsWith(".class")) {
						String className = fileName.substring(0,
								fileName.lastIndexOf(".class"));
						if (className.indexOf("/") == -1
								&& JTFTools.checkIfLoaded(className)) {
							try {
								Class<?> karelClass = Class
										.forName("stanford.karel.Karel");
								Class<?> c = Class.forName(className);
								if (karelClass.isAssignableFrom(c)) {
									result = className;
								}
							} catch (Exception ex) {
								/* Empty */
							}
						}
					}
				}
			}
		}
		return result;
	}

	public static void main(String[] args) {
		GuiUtils.setSystemLookAndFeel();
		ArrayList<String> argList = new ArrayList<String>();
		boolean codeFlag = false;
		for (int i = 0; i < args.length; i++) {
			argList.add(args[i]);
			if (args[i].startsWith("code="))
				codeFlag = true;
		}
		if (!codeFlag) {
			String className = getMainClassName();
			if (className.endsWith(".class")) {
				className = className.substring(0, className.length() - 6);
			}
			className = className.replace('/', '.');
			argList.add("code=" + className);
		}
		argList.add("program=stanford.karel.KarelProgram");
		String[] argArray = new String[argList.size()];
		for (int i = 0; i < argArray.length; i++) {
			argArray[i] = argList.get(i);
		}
		KarelProgram.main(argArray);
	}
	
	private static boolean nameAppears(String name, String[] array) {
		for (int i = 0; i < array.length; i++) {
			if (array[i].equals(name))
				return true;
		}
		return false;
	}
}
