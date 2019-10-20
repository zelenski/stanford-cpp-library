package stanford.spl;

import java.awt.Frame;

import acm.util.TokenScanner;

public class GWindow_toFront extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindowInterface localJBEWindow = paramJavaBackEnd.getWindowInterface(str1);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.setState(Frame.NORMAL);
			localJBEWindow.toFront();
		}
	}
}
