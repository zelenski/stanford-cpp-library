package stanford.spl.pipecommand;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import acm.util.TokenScanner;

public class JBEConsole_setCloseOperation extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		int op = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		paramJavaBackEnd.setConsoleCloseOperation(op);
	}
}
