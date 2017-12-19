package stanford.spl;

import java.awt.Dimension;

import acm.util.TokenScanner;

public class GWindow_getSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		Dimension size = new Dimension(0, 0);
		if (window != null) {
			size = window.getSize();
		}
		SplPipeDecoder.writeResult("GDimension(" + size.width + ", " + size.height + ")");
	}
}
