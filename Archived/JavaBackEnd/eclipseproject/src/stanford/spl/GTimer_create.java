package stanford.spl;

import acm.util.TokenScanner;

public class GTimer_create extends JBECommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		double ms = nextDouble(scanner);
		scanner.verifyToken(")");
		jbe.createTimer(id, ms);
	}
}
