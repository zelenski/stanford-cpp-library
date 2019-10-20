/*
 * @author Marty Stepp
 * @version 2016/11/26
 * - initial version
 */

package stanford.spl;

import acm.graphics.GObject;
import acm.util.*;

public class GTable_setRowAlignment extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String interactorID = nextString(scanner);
		scanner.verifyToken(",");
		int row = nextInt(scanner);
		scanner.verifyToken(",");
		int alignment = nextInt(scanner);
		scanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(interactorID);
		if (gobj != null && gobj instanceof GTable) {
			GTable table = (GTable) gobj;
			table.setRowAlignment(row, alignment);
		}
	}
}
