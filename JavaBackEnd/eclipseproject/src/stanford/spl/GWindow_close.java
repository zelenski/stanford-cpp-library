package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_close extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.removeWindowListener(paramJavaBackEnd);
			paramJavaBackEnd.deleteWindow(str);
			localJBEWindow.close();
		}
	}
}
