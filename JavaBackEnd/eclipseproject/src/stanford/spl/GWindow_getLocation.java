package stanford.spl;

import java.awt.Point;

import acm.util.TokenScanner;

public class GWindow_getLocation extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(")");
		Point p = new Point(0, 0);
		if (window != null) {
			p = window.getLocation();
		}
		SplPipeDecoder.writeResult("Point(" + p.x + ", " + p.y + ")");
	}
}
