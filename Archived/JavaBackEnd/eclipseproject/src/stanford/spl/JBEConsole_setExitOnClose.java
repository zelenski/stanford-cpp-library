package stanford.spl;

import acm.util.TokenScanner;

public class JBEConsole_setExitOnClose extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		boolean bool = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		paramJavaBackEnd.setExitOnConsoleClose(bool);
	}
}
