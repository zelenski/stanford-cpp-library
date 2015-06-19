package stanford.spl;

import java.awt.Frame;

import acm.util.TokenScanner;

public class JBEConsole_setVisible extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		boolean value = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		Frame consoleFrame = jbe.getJBEConsoleFrame();
		if (consoleFrame != null) {
			consoleFrame.setVisible(value);
		}
	}
}
