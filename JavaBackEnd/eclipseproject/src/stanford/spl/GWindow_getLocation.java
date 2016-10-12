package stanford.spl;

import java.awt.Point;

import acm.util.TokenScanner;

public class GWindow_getLocation extends JBESwingCommand {
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
