/*
 * This class implements the basic Karel-the-Robot object.
 * - 2015/03/31: Fixed warning by closing resources.
 * - 2015/03/31: Commented out unused constant INFINITE.
 * - 2015/03/31: Changed to OS system look and feel.
 */

package stanford.karel;

import acm.program.*;
import acm.util.*;
import stanford.cs106.gui.GuiUtils;

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

	/* Constructor */
	public Karel() {
		x = 1;
		y = 1;
		dir = EAST;
		world = null;
	}

	public void run() {
		/* Empty */
	}

	public void move() {
		checkWorld("move");
		if (world.checkWall(x, y, dir)) {
			throw new ErrorException("Karel is blocked");
		}
		setLocation(KarelWorld.adjacentPoint(x, y, dir));
		world.trace();
	}

	public void turnLeft() {
		checkWorld("turnLeft");
		setDirection(KarelWorld.leftFrom(dir));
		world.trace();
	}

	public void pickBeeper() {
		checkWorld("pickBeeper");
		int nb = world.getBeepersOnCorner(x, y);
		if (nb < 1) {
			throw new ErrorException("pickBeeper: No beepers on this corner");
		}
		world.setBeepersOnCorner(x, y, KarelWorld.adjustBeepers(nb, -1));
		setBeepersInBag(KarelWorld.adjustBeepers(getBeepersInBag(), +1));
		world.trace();
	}

	public void putBeeper() {
		checkWorld("putBeeper");
		int nBag = getBeepersInBag();
		if (nBag < 1) {
			throw new ErrorException("putBeeper: No beepers in bag");
		}
		int nCorner = world.getBeepersOnCorner(x, y);
		world.setBeepersOnCorner(x, y, KarelWorld.adjustBeepers(nCorner, +1));
		setBeepersInBag(KarelWorld.adjustBeepers(nBag, -1));
		world.trace();
	}

	public boolean frontIsClear() {
		checkWorld("frontIsClear");
		return !world.checkWall(x, y, dir);
	}

	public boolean frontIsBlocked() {
		checkWorld("frontIsBlocked");
		return world.checkWall(x, y, dir);
	}

	public boolean leftIsClear() {
		checkWorld("leftIsClear");
		return !world.checkWall(x, y, KarelWorld.leftFrom(dir));
	}

	public boolean leftIsBlocked() {
		checkWorld("leftIsBlocked");
		return world.checkWall(x, y, KarelWorld.leftFrom(dir));
	}

	public boolean rightIsClear() {
		checkWorld("rightIsClear");
		return !world.checkWall(x, y, KarelWorld.rightFrom(dir));
	}

	public boolean rightIsBlocked() {
		checkWorld("rightIsBlocked");
		return world.checkWall(x, y, KarelWorld.rightFrom(dir));
	}

	public boolean beepersPresent() {
		checkWorld("beepersPresent");
		return world.getBeepersOnCorner(x, y) > 0;
	}

	public boolean noBeepersPresent() {
		checkWorld("noBeepersPresent");
		return world.getBeepersOnCorner(x, y) == 0;
	}

	public boolean beepersInBag() {
		checkWorld("beepersInBag");
		return getBeepersInBag() > 0;
	}

	public boolean noBeepersInBag() {
		checkWorld("noBeepersInBag");
		return getBeepersInBag() == 0;
	}

	public boolean facingNorth() {
		checkWorld("facingNorth");
		return dir == NORTH;
	}

	public boolean facingEast() {
		checkWorld("facingEast");
		return dir == EAST;
	}

	public boolean facingSouth() {
		checkWorld("facingSouth");
		return dir == SOUTH;
	}

	public boolean facingWest() {
		checkWorld("facingWest");
		return dir == WEST;
	}

	public boolean notFacingNorth() {
		checkWorld("notFacingNorth");
		return dir != NORTH;
	}

	public boolean notFacingEast() {
		checkWorld("notFacingEast");
		return dir != EAST;
	}

	public boolean notFacingSouth() {
		checkWorld("notFacingSouth");
		return dir != SOUTH;
	}

	public boolean notFacingWest() {
		checkWorld("notFacingWest");
		return dir != WEST;
	}

	/* Entry points for program operation */

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
	
	@Override
	public boolean menuAction(ActionEvent event) {
		return super.menuAction(event);
	}

	/* Protected method: start(args) */
	/**
	 * Starts a <code>KarelProgram</code> containing this Karel instance,
	 * passing it the specified arguments.
	 */

	public void start(String[] args) {
		KarelProgram program = new KarelProgram();
		program.setStartupObject(this);
		program.start(args);
	}

	/* Protected methods */

	public Point getLocation() {
		return new Point(x, y);
	}

	public void setLocation(Point pt) {
		setLocation(pt.x, pt.y);
	}

	public void setLocation(int x, int y) {
		if (world != null) {
			if (world.outOfBounds(x, y)) {
				throw new ErrorException("setLocation: Out of bounds");
			}
			Karel occupant = world.getKarelOnSquare(x, y);
			if (occupant == this)
				return;
			if (occupant != null) {
				throw new ErrorException("setLocation: Square is occupied");
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

	public int getDirection() {
		return dir;
	}

	public void setDirection(int dir) {
		this.dir = dir;
		if (world != null)
			world.updateCorner(x, y);
	}

	public int getBeepersInBag() {
		return (beepers);
	}

	public void setBeepersInBag(int nBeepers) {
		beepers = nBeepers;
	}

	public KarelWorld getWorld() {
		return world;
	}

	public void setWorld(KarelWorld world) {
		this.world = world;
	}

	protected void checkWorld(String caller) {
		if (world == null) {
			throw new ErrorException(caller
					+ ": Karel is not living in a world");
		}
	}

	public void setDisplayOneFlag(boolean flag) {
		world.setDisplayOneFlag(flag);
	}

	/* Private methods */

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

	private static boolean nameAppears(String name, String[] array) {
		for (int i = 0; i < array.length; i++) {
			if (array[i].equals(name))
				return true;
		}
		return false;
	}
}
