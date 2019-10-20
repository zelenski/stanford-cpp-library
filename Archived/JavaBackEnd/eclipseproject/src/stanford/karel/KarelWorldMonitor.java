package stanford.karel;

import java.awt.Point;

interface KarelWorldMonitor {
	/**
	 * This action is invoked at the beginning of an editing session.
	 */
	public void startWorldEdit();

	/**
	 * This action is invoked at the end of an editing session.
	 */
	public void endWorldEdit();

	/**
	 * This action is invoked when the mouse is clicked on a wall, which is the
	 * wall in the indicated direction from the Karel coordinates given by pt.
	 */
	public void wallAction(Point pt, int dir);

	/**
	 * This action is invoked when the mouse is clicked on a corner, which is
	 * the wall in the indicated direction from the given point.
	 */
	public void cornerAction(Point pt);

	/**
	 * This action is invoked when karel executes an instruction.
	 */
	public void trace();

	/**
	 * This method is invoked when a world map file needs to set the simulation
	 * speed.
	 */
	public void setSpeed(double speed);

	/**
	 * This method is invoked when the KarelWorld class needs to get the
	 * simulation speed.
	 */
	public double getSpeed();
}
