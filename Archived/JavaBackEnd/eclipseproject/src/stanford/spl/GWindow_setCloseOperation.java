/*
 * @version 2016/11/24
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setCloseOperation extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int op = nextInt(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			window.setDefaultCloseOperation(op);
		}
	}
}
