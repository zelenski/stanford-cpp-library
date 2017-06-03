/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 */

package acm.program;

import java.awt.*;
import java.awt.event.*;

/* Package class: ProgramContentPaneLayout */
/**
 * This layout manager is identical to the BorderLayout manager except that
 * it also resizes the Program object itself to correspond to the available
 * space.  This class is required for JDK 1.6 systems, when it became
 * necessary to separate the content pane of the JApplet from the program's
 * JApplet itself.
 */
class ProgramContentPaneLayout extends BorderLayout {
	private Program myProgram;

	public ProgramContentPaneLayout(Program program) {
		myProgram = program;
	}

	public void layoutContainer(Container parent) {
		super.layoutContainer(parent);
		if (!myProgram.isAncestorOf(parent)) {
			Dimension psize = parent.getSize();
			Insets insets = parent.getInsets();
			int x = insets.left;
			int y = insets.top;
			int width = psize.width - insets.left - insets.right;
			int height = psize.height - insets.top - insets.bottom;
			myProgram.setBounds(x, y, width, height);
			ComponentEvent e = new ComponentEvent(myProgram, ComponentEvent.COMPONENT_RESIZED);
			Toolkit.getDefaultToolkit().getSystemEventQueue().postEvent(e);
		}
	}
}
