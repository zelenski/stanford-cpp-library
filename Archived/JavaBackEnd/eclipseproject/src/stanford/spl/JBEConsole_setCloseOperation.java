package stanford.spl;

import acm.util.TokenScanner;

public class JBEConsole_setCloseOperation extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		int op = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		paramJavaBackEnd.setConsoleCloseOperation(op);
	}
}
