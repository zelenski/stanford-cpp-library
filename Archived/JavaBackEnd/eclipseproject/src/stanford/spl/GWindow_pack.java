package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_pack extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		if (window != null) {
			// BUGFIX: some kind of race condition on packing windows; slight delay helps
			try {
				Thread.sleep(50);
			} catch (InterruptedException ie) {
				// empty
			}
			window.pack();
		}
	}
}
