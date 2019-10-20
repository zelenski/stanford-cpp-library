package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setVisible extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		boolean bool = scanner.nextToken().equals("true");
		scanner.verifyToken(")");
		if (window != null) {
			window.setVisible(bool);
		}
	}
}
