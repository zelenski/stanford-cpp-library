/*
 * @version 2016/10/08
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_toBack extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindowInterface localJBEWindow = paramJavaBackEnd.getWindowInterface(str1);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.toBack();
		}
	}
}
