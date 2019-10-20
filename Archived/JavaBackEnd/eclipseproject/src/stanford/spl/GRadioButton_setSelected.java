package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GRadioButton_setSelected extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean selected = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GRadioButton) {
			((GRadioButton) localGObject).setSelected(selected);
		}
	}
}
