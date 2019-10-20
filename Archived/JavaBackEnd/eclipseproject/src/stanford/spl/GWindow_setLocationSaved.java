package stanford.spl;

import java.awt.*;
import acm.util.TokenScanner;

public class GWindow_setLocationSaved extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		boolean value = nextBoolean(scanner);
		scanner.verifyToken(")");
		if (window != null && window instanceof Window) {
			if (value) {
				SPLWindowSettings.loadWindowLocation((Window) window);
				SPLWindowSettings.saveWindowLocation((Window) window);
			} else {
				SPLWindowSettings.forgetWindowLocation((Window) window);
			}
		}
	}
}
