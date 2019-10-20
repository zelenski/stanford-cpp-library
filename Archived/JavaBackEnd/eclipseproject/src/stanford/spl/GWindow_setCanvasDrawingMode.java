package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setCanvasDrawingMode extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int mode = nextInt(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			JBECanvas canvas = window.getCanvas();
			canvas.setDrawingMode(mode);
		}
	}
}
