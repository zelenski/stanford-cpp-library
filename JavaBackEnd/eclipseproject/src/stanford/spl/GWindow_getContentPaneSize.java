package stanford.spl;

import java.awt.Dimension;

import acm.util.TokenScanner;

public class GWindow_getContentPaneSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		Dimension dim = new Dimension(0, 0);
		if (window != null) {
			dim = window.getContentPaneSize();
		}
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
