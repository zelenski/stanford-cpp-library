/*
 * @version 2017/10/05
 * - initial version
 */

package stanford.spl;

import java.awt.*;
import acm.util.TokenScanner;

public class GWindow_rememberPosition extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		if (window != null && window instanceof Window) {
			SPLWindowSettings.loadWindowLocation((Window) window);
			SPLWindowSettings.saveWindowLocation((Window) window);
		}
	}
}
