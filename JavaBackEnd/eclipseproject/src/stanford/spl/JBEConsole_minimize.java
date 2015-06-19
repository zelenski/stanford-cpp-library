package stanford.spl;

import acm.util.TokenScanner;

public class JBEConsole_minimize extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.scanNumbers();
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		jbe.consoleMinimize();
	}
}
