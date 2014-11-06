package stanford.spl;

import acm.util.TokenScanner;

class JBEConsole_setExitOnClose extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		boolean bool = nextString(paramTokenScanner).equals("true");
		paramTokenScanner.verifyToken(")");
		paramJavaBackEnd.setExitOnConsoleClose(bool);
	}
}
