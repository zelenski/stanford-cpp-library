package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_getPixel extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		int x = nextInt(scanner);
		scanner.verifyToken(",");
		int y = nextInt(scanner);
		scanner.verifyToken(")");
		
		int rgb = window.getCanvas().getRGB(x, y);
		SplPipeDecoder.writeResult(rgb);
	}
}
