package stanford.spl;

import java.awt.Frame;

import acm.util.TokenScanner;

class GWindow_minimize extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.setState(Frame.ICONIFIED);
		}
	}
}
