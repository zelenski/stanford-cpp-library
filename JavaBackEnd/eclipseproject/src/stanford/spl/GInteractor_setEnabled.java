package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GInteractor_setEnabled extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String bool = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject.setAntiAliasing(bool.equals("true"));
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			((GInteractor) localGObject).setEnabled(bool.equals("true"));
		}
	}
}
