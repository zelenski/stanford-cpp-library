/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 */

package acm.program;

import java.awt.*;
import javax.swing.*;

/* Package class: ProgramFrame */
/**
 * This frame represents the visible component that encloses the program.
 */
class ProgramFrame extends JFrame {
	public ProgramFrame(String title) {
		super(title);
	}

	public void update(Graphics g) {
		paint(g);
	}
}
