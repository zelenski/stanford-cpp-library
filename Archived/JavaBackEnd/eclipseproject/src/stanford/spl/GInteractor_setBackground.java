package stanford.spl;

import java.awt.Color;

import acm.graphics.GObject;
import acm.util.JTFTools;
import acm.util.TokenScanner;

public class GInteractor_setBackground extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String colorStr = nextString(paramTokenScanner);
		Color color = JTFTools.decodeColor(colorStr);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			((GInteractor) localGObject).setBackground(color);
		}
	}
}
