package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GObject_setAntialiasing extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String bool = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject.setAntiAliasing(bool.equals("true"));
	}
}
