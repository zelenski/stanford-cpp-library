package stanford.spl;

import acm.util.TokenScanner;
import javax.swing.*;

public class JBEConsole_getTitle extends JBECommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		JFrame frame = jbe.getJBEConsoleFrame();
		String title = frame == null ? "null" : frame.getTitle();
		SplPipeDecoder.writeResult(title);
	}
}
