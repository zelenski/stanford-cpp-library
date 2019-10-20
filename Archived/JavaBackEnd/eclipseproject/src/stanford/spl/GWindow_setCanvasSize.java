package stanford.spl;

import java.awt.Dimension;

import acm.util.TokenScanner;

public class GWindow_setCanvasSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int w = nextInt(scanner);
		scanner.verifyToken(",");
		int h = nextInt(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			window.getCanvas().setPreferredSize(new Dimension(w, h));
			window.getCanvas().validate();
			window.getCanvas().setSize(new Dimension(w, h));
			window.getContentPane().validate();
			window.validate();
			if (window.isVisible()) {
				window.pack();
			}
		}
	}
}
