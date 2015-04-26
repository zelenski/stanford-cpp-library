package stanford.spl.pipecommand;

import java.awt.Point;

import stanford.spl.JBECommand;
import stanford.spl.JBEWindow;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

import acm.util.TokenScanner;

public class GWindow_getLocation extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		Point p = new Point(0, 0);
		if (localJBEWindow != null) {
			p = localJBEWindow.getLocation();
		}
		SplPipeDecoder.writeResult("Point(" + p.x + ", " + p.y + ")");
	}
}
