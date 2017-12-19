package stanford.spl;

import acm.util.*;
import stanford.cs106.io.IORuntimeException;

public class GWindow_setPixels extends JBESwingCommand {
	// gbufferedimage.load("foobar.png");
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		String base64 = nextBase64(scanner);
		scanner.verifyToken(")");
		if (window == null) {
			return;
		}
		
		try {
			window.getCanvas().setPixelsFromString(base64);
		} catch (IORuntimeException ioe) {
			SplPipeDecoder.writeError(ioe);
		}
	}
}
