/*
 * @version 2017/07/21
 * - added add() overloads
 * @version 2017/04/27
 * - added directional constants e.g. CENTER
 * - added pause()
 * - added print(), printf(), println()
 */

package acm.program;

import java.awt.*;
import java.util.*;

/**
 * This interface contains all methods that Program classes are required to implement.
 */
public interface ProgramInterface {
	/** Constant specifying the center of the container */
	public static final String CENTER = BorderLayout.CENTER;

	/** Constant specifying the east edge of the container */
	public static final String EAST = BorderLayout.EAST;

	/** Constant specifying the north edge of the container */
	public static final String NORTH = BorderLayout.NORTH;

	/** Constant specifying the south edge of the container */
	public static final String SOUTH = BorderLayout.SOUTH;

	/** Constant specifying the west edge of the container */
	public static final String WEST = BorderLayout.WEST;

	public Component add(Component comp);
	public void add(Component comp, Object constraints);
	public void add(Component comp, String region, Object constraints);
	public void addActionListeners();
	public void exit();
	public String getTitle();
	public void init();
	public void pause(double milliseconds);
	public void print(Object value);
	public void print(String value);
	public void printf(String format, Object... args);
	public void println();
	public void println(Object value);
	public void println(String value);
	public void run();
	public void setBackground(Color color);
	public void setFont(Font font);
	public void setForeground(Color color);
	public void setParameterTable(Map<String, String> table);
	public void setStartupObject(Object obj);
	public void setTitle(String title);
	public void start();
	public void start(String[] args);
}
