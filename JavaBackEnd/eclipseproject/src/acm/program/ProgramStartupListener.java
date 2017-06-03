/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 * - alphabetized methods
 */

package acm.program;

import java.awt.event.*;
import acm.util.JTFTools;

/* Package class: ProgramStartupListener */
/**
 * This class implements a component listener that supports the program
 * startup logic.
 */
class ProgramStartupListener implements ComponentListener {
	/* Private constants */
	private static final int STARTUP_DELAY = 1000;
	private static final int STARTUP_CYCLE = 300;

	/* ComponentListener interface */
	public void componentHidden(ComponentEvent e) {
		// empty
	}
	
	public void componentMoved(ComponentEvent e) {
		// empty
	}

	public void componentResized(ComponentEvent e) {
		componentShown(e);
	}

	public synchronized void componentShown(ComponentEvent e) {
		notifyAll();
	}

	/* Method: waitForStartup(program) */
	/**
	 * Waits until the specified program has started.
	 */
	public synchronized void waitForStartup(Program program) {
		JTFTools.pause(STARTUP_DELAY);
		while (!program.isStarted()) {
			try {
				wait(STARTUP_CYCLE);
			} catch (InterruptedException ex) {
				/* Empty */
			}
		}
	}
}
