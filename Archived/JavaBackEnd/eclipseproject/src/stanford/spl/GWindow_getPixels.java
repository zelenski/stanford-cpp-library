package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_getPixels extends JBESwingCommand {
	// gbufferedimage.load("foobar.png");
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		if (window == null) {
			return;
		}
		String base64px = window.getCanvas().getPixelsAsString();
		SplPipeDecoder.writeResult(base64px);
	}
}
