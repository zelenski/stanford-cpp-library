package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTable_setEventEnabled extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int eventType = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean enabled = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTable) {
			GTable table = (GTable) localGObject;
			table.setEventEnabled(eventType, enabled);
		}
	}
}
