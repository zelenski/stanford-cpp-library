package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setPixel extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int x = nextInt(scanner);
		scanner.verifyToken(",");
		int y = nextInt(scanner);
		scanner.verifyToken(",");
		int rgb = nextInt(scanner);
		scanner.verifyToken(",");
		boolean repaint = nextBoolean(scanner);
		scanner.verifyToken(")");
		
		window.getCanvas().setRGB(x, y, rgb, repaint);
	}
}
