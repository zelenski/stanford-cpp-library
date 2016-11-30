/*
 * @version 2016/11/24
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setCloseOperation extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		int op = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.setDefaultCloseOperation(op);
		}
	}
}
