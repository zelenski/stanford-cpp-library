package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setLocationSaved extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		boolean value = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			if (value) {
				SPLWindowSettings.loadWindowLocation(localJBEWindow);
				SPLWindowSettings.saveWindowLocation(localJBEWindow);
			} else {
				SPLWindowSettings.forgetWindowLocation(localJBEWindow);
			}
		}
	}
}
