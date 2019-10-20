package stanford.spl;

import acm.util.TokenScanner;
import java.awt.Dimension;
import java.awt.Toolkit;

public class GWindow_getScreenWidth extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
		SplPipeDecoder.writeResult(size.width);
	}
}
