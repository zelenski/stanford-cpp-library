package stanford.spl;

import java.awt.Dimension;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.Window;

//import javax.swing.JOptionPane;

import acm.util.TokenScanner;

public class GWindow_setLocation extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			// BUGBUG: It appears that TokenScanner can't handle negative numbers
			//         and doesn't think they are int tokens.  Ugh.
			if ((x == -1 && y == -1) || (x == 999999 && y == 999999)) {
				Point center = getCenter(localJBEWindow);
				x = center.x;
				y = center.y;
			}
			localJBEWindow.setLocation(x, y);
		}
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
//		JOptionPane.showMessageDialog(null,
//				"screen size: " + screen.width + " x " + screen.height + "\n"
//				+ "window size: " + windowWidth + " x " + windowHeight + "\n"
//				+ "center: (x=" + centerX + ", y=" + centerY + ")");
		return new Point(centerX, centerY);
	}
}
