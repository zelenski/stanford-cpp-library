package stanford.spl;

import java.awt.Frame;

import acm.util.TokenScanner;

public class JBEConsole_setLocationSaved extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		boolean value = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		Frame consoleFrame = jbe.getJBEConsoleFrame();
		if (consoleFrame != null) {
			if (value) {
				SPLWindowSettings.loadWindowLocation(consoleFrame);
				SPLWindowSettings.saveWindowLocation(consoleFrame);
			} else {
				SPLWindowSettings.forgetWindowLocation(consoleFrame);
			}
		}
	}
}
