package stanford.spl;

import java.awt.*;

import acm.util.TokenScanner;

public class GWindow_getScreenSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
