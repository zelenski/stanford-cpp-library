package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int w = nextInt(scanner);
		scanner.verifyToken(",");
		int h = nextInt(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			window.setSize(w, h);
		}
	}
}
