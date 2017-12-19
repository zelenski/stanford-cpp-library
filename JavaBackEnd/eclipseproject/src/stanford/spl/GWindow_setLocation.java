package stanford.spl;

import java.awt.Dimension;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.Window;

//import javax.swing.JOptionPane;

import acm.util.TokenScanner;

public class GWindow_setLocation extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int x = nextInt(scanner);
		scanner.verifyToken(",");
		int y = nextInt(scanner);
		scanner.verifyToken(")");
		if (window != null) {
			// BUGBUG: It appears that TokenScanner can't handle negative numbers
			//         and doesn't think they are int tokens.  Ugh.
			if ((x == -1 && y == -1) || (x == 999999 && y == 999999)) {
				Point center = getCenter(window);
				x = center.x;
				y = center.y;
			}
			window.setLocation(x, y);
		}
	}
	
	public static Point getCenter(JBEWindowInterface window) {
		return getCenter(window.getWidth(), window.getHeight());
	}
	
	public static Point getCenter(Window window) {
		return getCenter(window.getWidth(), window.getHeight());
	}
	
	public static Point getCenter(Dimension windowDimensions) {
		return getCenter(windowDimensions.width, windowDimensions.height);
	}
	
	public static Point getCenter(int windowWidth, int windowHeight) {
		Dimension screen = Toolkit.getDefaultToolkit().getScreenSize();
		int centerX = Math.max(0, (screen.width - windowWidth) / 2);
		int centerY = Math.max(0, (screen.height - windowHeight) / 2);
		return new Point(centerX, centerY);
	}
}
