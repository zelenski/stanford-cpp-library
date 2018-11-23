/*
 * CS 106A
 *
 * An interface for simpler listening to resize events.
 *
 * Author : Marty Stepp
 * Version: Tue 2014/06/05
 * 
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

package stanford.cs106.gui;

import java.awt.event.*;

public interface ResizeListener {
	public void componentResized(ComponentEvent event);
}
