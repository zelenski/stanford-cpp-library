package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_exitGraphics extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		System.exit(0);
	}
}
