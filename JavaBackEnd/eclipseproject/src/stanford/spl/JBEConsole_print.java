package stanford.spl;

import acm.util.TokenScanner;

public class JBEConsole_print extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str = nextString(paramTokenScanner);
		String token = paramTokenScanner.nextToken();
		boolean isStderr = false;
		if (token.equals(",")) {
			isStderr = nextBoolean(paramTokenScanner);
		}
		paramJavaBackEnd.putConsole(str, isStderr);
	}
}
