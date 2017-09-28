package stanford.spl;

import acm.util.TokenScanner;
import javax.swing.*;

public class JBEConsole_getTitle extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		JFrame frame = jbe.getJBEConsoleFrame();
		String title = frame == null ? "null" : frame.getTitle();
		jbe.println("result:" + title);
	}
}
