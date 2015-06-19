package stanford.spl.pipecommand;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GObject_setAntialiasing extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String bool = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject.setAntiAliasing(bool.equals("true"));
	}
}
