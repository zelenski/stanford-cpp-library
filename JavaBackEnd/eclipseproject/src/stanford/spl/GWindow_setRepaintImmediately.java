/*
 * @version 2017/10/12
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setRepaintImmediately extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		boolean value = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.setRepaintImmediately(value);
		}
	}
}
