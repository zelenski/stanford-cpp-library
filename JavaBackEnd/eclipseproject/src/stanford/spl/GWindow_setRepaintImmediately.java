/*
 * @version 2017/10/12
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setRepaintImmediately extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		boolean value = nextBoolean(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			window.setRepaintImmediately(value);
		}
	}
}
