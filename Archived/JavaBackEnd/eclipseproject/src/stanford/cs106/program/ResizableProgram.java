/*
 * CS 106A
 *
 * This instructor-provided file represents a program that generates an event
 * when it is resized.
 *
 * Author : Marty Stepp
 * Version: Tue 2014/05/11
 * 
 * This file and its contents are copyright (C) Stanford University and Marty Stepp,
 * licensed under Creative Commons Attribution 2.5 License.  All rights reserved.
 */

package stanford.cs106.program;
import acm.program.*;
import java.awt.event.*;

public class ResizableProgram extends Program implements ComponentListener {
	/*
	 * Constructs the program and adds the resizing listener.
	 */
	public ResizableProgram() {
		addComponentListener(this);
	}
	
	/* Required methods of ComponentListener interface. */
	public void componentHidden(ComponentEvent e) {
		// empty
	}

	/* Required methods of ComponentListener interface. */
	public void componentMoved(ComponentEvent e) {
		// empty
	}

	/* Required methods of ComponentListener interface. */
	public void componentResized(ComponentEvent e) {
		// empty
	}

	/* Required methods of ComponentListener interface. */
	public void componentShown(ComponentEvent e) {
		// empty
	}
}
