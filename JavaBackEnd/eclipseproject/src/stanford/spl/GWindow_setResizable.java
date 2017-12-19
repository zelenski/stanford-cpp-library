package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setResizable extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		boolean value = nextBoolean(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			window.setResizable(value);
		}
	}
}
