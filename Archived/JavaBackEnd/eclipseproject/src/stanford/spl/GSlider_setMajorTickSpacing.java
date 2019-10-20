package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GSlider_setMajorTickSpacing extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int value = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GSlider) {
			((GSlider) localGObject).setMajorTickSpacing(value);
		}
	}
}
