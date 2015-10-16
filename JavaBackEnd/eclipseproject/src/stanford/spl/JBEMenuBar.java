/*
 * @version 2015/10/15
 * - tiny edit: added a colon after 'version' in Help About message
 * @version 2014/10/22
 * - added Ctrl-Home, Ctrl-End, PgUp, PgDown hotkeys to scroll around in console
 */

package stanford.spl;

import java.awt.event.*;

import acm.io.*;
import acm.program.*;

public class JBEMenuBar extends ProgramMenuBar implements ActionListener {
	private static final long serialVersionUID = 1L;
	
	private JavaBackEnd javaBackEnd;

	public JBEMenuBar(JavaBackEnd paramJavaBackEnd, IOConsole paramIOConsole) {
		super(new JBEDummyProgram(paramJavaBackEnd));
		getProgram().setConsole(paramIOConsole);
		this.javaBackEnd = paramJavaBackEnd;
	}

	protected String getAboutMessage() {
		return "Stanford C++ Library version: " + javaBackEnd.getCppVersion() + "\n"
				+ super.getAboutMessage();
	}
}
