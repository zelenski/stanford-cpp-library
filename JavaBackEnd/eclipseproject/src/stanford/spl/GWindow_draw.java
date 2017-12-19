package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;
import java.awt.Graphics2D;

public class GWindow_draw extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		windowId = nextString(scanner);
		GObject obj = jbe.getGObject(windowId);
		scanner.verifyToken(")");
		if ((window != null) && (obj != null) && (obj.isVisible())) {
			JBECanvas canvas = window.getCanvas();
			Graphics2D g = canvas.getOSG();
			g.setColor(obj.getColor());
			obj.paint(g);
			canvas.repaint();
		}
	}
}
