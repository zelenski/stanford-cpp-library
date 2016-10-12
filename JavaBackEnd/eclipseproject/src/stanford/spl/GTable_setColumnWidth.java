package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTable_setColumnWidth extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int column = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int width = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTable) {
			GTable table = (GTable) localGObject;
			table.setColumnWidth(column, width);
		}
	}
}
