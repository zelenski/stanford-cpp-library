package stanford.karel;

import java.awt.*;

/**
 * An interface to represent Karel objects.
 * Used to present a common supertype between Karel and HeadlessKarel.
 */
public interface KarelInterface {
	public int getDirection();
	public int getBeepersInBag();
	public KarelWorld getWorld();
	public Point getLocation();
	public void pause(double ms);
	public void setBeepersInBag(int beepers);
	public void setDirection(int dir);
	public void setLocation(int x, int y);
	public void setLocation(Point pt);
	public void setWorld(KarelWorld world);

	public void run();

	public void move();
	public void turnLeft();
	public void pickBeeper();
	public void putBeeper();
	public boolean frontIsClear();
	public boolean frontIsBlocked();
	public boolean leftIsClear();
	public boolean leftIsBlocked();
	public boolean rightIsClear();
	public boolean rightIsBlocked();
	public boolean beepersPresent();
	public boolean noBeepersPresent();
	public boolean beepersInBag();
	public boolean noBeepersInBag();
	public boolean facingNorth();
	public boolean facingEast();
	public boolean facingSouth();
	public boolean facingWest();
	public boolean notFacingNorth();
	public boolean notFacingEast();
	public boolean notFacingSouth();
	public boolean notFacingWest();
}
