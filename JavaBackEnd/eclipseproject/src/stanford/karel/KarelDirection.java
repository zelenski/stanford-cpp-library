/*
 * @author Marty Stepp
 * @version 2017/04/05
 * - initial version
 */

package stanford.karel;

/**
 * An enum internally used to keep track of Karel directions.
 */
public enum KarelDirection {
	NORTH, EAST, SOUTH, WEST;
	
	/**
	 * Converts an int like KarelWorld.NORTH into a KarelDirection.
	 */
	public static KarelDirection valueOf(int dir) {
		return KarelDirection.valueOf(KarelWorld.directionName(dir));
	}
}
