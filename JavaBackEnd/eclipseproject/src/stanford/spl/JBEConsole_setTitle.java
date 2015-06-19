package stanford.spl;

import acm.util.TokenScanner;

public class JBEConsole_setTitle extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.scanNumbers();
		paramTokenScanner.verifyToken("(");
		String title = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		jbe.consoleSetTitle(title);
	}
}
