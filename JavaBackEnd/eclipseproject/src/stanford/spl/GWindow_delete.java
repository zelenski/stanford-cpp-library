package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_delete extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		scanner.verifyToken(")");
		jbe.deleteWindow(windowId);
	}
}
