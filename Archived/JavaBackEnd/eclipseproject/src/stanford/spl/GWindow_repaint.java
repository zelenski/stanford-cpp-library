package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_repaint extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		if (window != null) {
			window.getCanvas().repaint();
		}
	}
}
