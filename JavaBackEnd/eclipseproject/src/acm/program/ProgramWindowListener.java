/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 * - alphabetized methods
 */

package acm.program;

import java.awt.*;
import java.awt.event.*;

/* Package class: ProgramWindowListener */
/**
 * This class implements a simple window listener for programs whose only
 * function is to exit if the window is closed.
 */
class ProgramWindowListener implements WindowListener {
	/* Private instance variables */
	private Program program;

	public ProgramWindowListener(Program owner) {
		program = owner;
	}

	/* WindowListener interface */
	public void windowClosing(WindowEvent e) {
		((Component) e.getSource()).setVisible(false);
		program.exit();
	}

	public void windowActivated(WindowEvent e) {
		// empty
	}
	
	public void windowClosed(WindowEvent e) {
		// empty
	}
	
	public void windowDeactivated(WindowEvent e) {
		// empty
	}
	
	public void windowDeiconified(WindowEvent e) {
		// empty
	}
	
	public void windowIconified(WindowEvent e) {
		// empty
	}
	
	public void windowOpened(WindowEvent e) {
		// empty
	}
}
