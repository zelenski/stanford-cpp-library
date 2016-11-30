/*
 * @author Marty Stepp
 * @version 2016/11/24
 * - initial version
 */

package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTable_set extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int row = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int col = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String value = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTable) {
			GTable table = (GTable) localGObject;
			
			// We pass 'false' as notifyBackEnd because this event came *from* the back-end;
			// only need to tell back-end when the user initiates setting a cell from front-end in Java.
			table.set(row, col, value, /* notifyBackEnd */ false);
		}
	}
}
