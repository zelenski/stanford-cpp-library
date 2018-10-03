/*
 * An accessible version of Karel that prints status information, making it easier
 * for a screen reader to speak Karel's progress aloud.
 * 
 * @version 2018/10/02
 * - initial version
 */

package stanford.karel;

import java.awt.Color;
import acm.graphics.GObject;

public class AccessibleKarel extends SuperKarel {
	public AccessibleKarel() {
		super();
		setEcho(true);
	}
	
	/**
	 * Prints the current state of Karel's world to the console.
	 */
	public void echoWorld() {
		maybeEcho("Current world state:");
		KarelWorld world = getWorld();
		maybeEcho("  World size is " + world.getColumns() + " by " + world.getRows());
		
		for (int x = 1; x <= world.getColumns(); x++) {
			for (int y = 1; y <= world.getRows(); y++) {
				int beepers = world.getBeepersOnCorner(x, y);
				if (beepers > 0) {
					maybeEcho("  Corner " + x + ", " + y + " has " + beepers + " beeper"
							+ (beepers > 1 ? "s" : ""));
				}
				Color color = world.getCornerColor(x, y);
				if (color != null) {
					maybeEcho("  Corner " + x + ", " + y + " has color "
							+ GObject.colorNameFriendly(color));
				}
			}
		}
	}
	
	/**
	 * Prints Karel's current state to the console.
	 */
	public void help() {
		maybeEchoCurrentState();
	}
}
