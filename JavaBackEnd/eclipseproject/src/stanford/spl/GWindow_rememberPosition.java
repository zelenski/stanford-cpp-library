/*
 * @version 2017/10/05
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_rememberPosition extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			SPLWindowSettings.loadWindowLocation(localJBEWindow);
			SPLWindowSettings.saveWindowLocation(localJBEWindow);
		}
	}
}
