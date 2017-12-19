package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_requestFocus extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		if (window != null) {
			window.toFront();
			window.getCanvas().requestFocus();
		}
	}
}
