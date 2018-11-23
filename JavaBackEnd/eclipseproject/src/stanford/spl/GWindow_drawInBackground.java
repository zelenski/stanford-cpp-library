package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

import java.awt.Graphics2D;

/*
 * same as GWindow_draw but doesn't repaint for speed
 */
public class GWindow_drawInBackground extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		String objectId = nextString(scanner);
		GObject obj = jbe.getGObject(objectId);
		scanner.verifyToken(")");
		if ((window != null) && (obj != null) && (obj.isVisible())) {
			JBECanvas canvas = window.getCanvas();
			canvas.setAutoRepaintFlag(false);
			Graphics2D g = canvas.getOSG();
			g.setColor(obj.getColor());
			obj.paint(g);
		}
	}
}
