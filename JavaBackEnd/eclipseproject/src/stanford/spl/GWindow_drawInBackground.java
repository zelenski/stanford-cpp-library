package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

import java.awt.Graphics2D;

/*
 * same as GWindow_draw but doesn't repaint for speed
 */
public class GWindow_drawInBackground extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str);
		paramTokenScanner.verifyToken(",");
		str = nextString(paramTokenScanner);
		GObject localGObject = paramJavaBackEnd.getGObject(str);
		paramTokenScanner.verifyToken(")");
		if ((localJBEWindow != null) && (localGObject != null) && (localGObject.isVisible())) {
			JBECanvas localJBECanvas = localJBEWindow.getCanvas();
			localJBECanvas.setAutoRepaintFlag(false);
			Graphics2D localGraphics2D = localJBECanvas.getOSG();
			localGraphics2D.setColor(localGObject.getColor());
			localGObject.paint(localGraphics2D);
			// localJBECanvas.repaint();
		}
	}
}
