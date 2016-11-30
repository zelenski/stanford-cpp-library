package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTable_setEditorValue extends JBESwingCommand {
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
			table.setEditorValue(row, col, value);
		}
	}
}
