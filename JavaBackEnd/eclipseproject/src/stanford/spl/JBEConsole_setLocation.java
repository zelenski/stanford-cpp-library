package stanford.spl;

import java.awt.Point;

import acm.util.TokenScanner;

public class JBEConsole_setLocation extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.scanNumbers();
		paramTokenScanner.verifyToken("(");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		// BUGBUG: It appears that TokenScanner can't handle negative numbers
		//         and doesn't think they are int tokens.  Ugh.
		if ((x == -1 && y == -1) || (x == 999999 && y == 999999)) {
			Point center = GWindow_setLocation.getCenter(
					jbe.getJBEConsoleWidth(), jbe.getJBEConsoleHeight());
			x = center.x;
			y = center.y;
		}
		jbe.setConsoleLocation(x, y);
	}
}
