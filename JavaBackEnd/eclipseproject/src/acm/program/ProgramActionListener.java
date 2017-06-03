/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 */

package acm.program;

import java.awt.event.*;

/* Package class: ProgramActionListener */
/**
 * This class listens for global action events in the menu bar.  These events are
 * passed back to the program through the protected <code>globalMenuAction<code>
 * method.
 */
class ProgramActionListener implements ActionListener {
	/* Private instance variables */
	private Program program;

	public ProgramActionListener(Program owner) {
		program = owner;
	}

	/* ActionListener interface */
	public void actionPerformed(ActionEvent e) {
		program.menuAction(e);
	}
}
