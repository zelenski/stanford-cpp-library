package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_setPixel extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean repaint = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		window.getCanvas().setRGB(x, y, rgb, repaint);
	}
}
