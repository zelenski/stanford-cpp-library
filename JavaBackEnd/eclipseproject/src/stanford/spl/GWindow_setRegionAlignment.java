package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setRegionAlignment extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		scanner.verifyToken(",");
		String str2 = nextString(scanner);
		scanner.verifyToken(",");
		String str3 = nextString(scanner);
		scanner.verifyToken(")");
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		if (window != null) {
			window.setRegionAlignment(str2, str3);
		}
	}
}
