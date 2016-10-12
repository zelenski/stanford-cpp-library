/*
 * @version 2016/05/01
 * - moved "version" logic to centralized Version.java
 * @version 2015/10/15
 * - tiny edit: added a colon after 'version' in Help About message
 * @version 2014/10/22
 * - added Ctrl-Home, Ctrl-End, PgUp, PgDown hotkeys to scroll around in console
 */

package stanford.spl;

import acm.io.*;
import acm.program.*;

public class JBEMenuBar extends ProgramMenuBar {
	private static final long serialVersionUID = 1L;
	
	public JBEMenuBar(JavaBackEnd paramJavaBackEnd, IOConsole paramIOConsole) {
		super(new JBEDummyProgram(paramJavaBackEnd));
		getProgram().setConsole(paramIOConsole);
	}
}
