package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTable_getSelection extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTable) {
			GTable table = (GTable) localGObject;
			int row = table.getSelectedRow();
			int column = table.getSelectedColumn();
			SplPipeDecoder.writeResult(row);
			SplPipeDecoder.writeResult(column);
		}
	}
}
