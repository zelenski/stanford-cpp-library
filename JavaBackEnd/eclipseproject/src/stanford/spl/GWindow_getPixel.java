package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_getPixel extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		int rgb = window.getCanvas().getRGB(x, y);
		SplPipeDecoder.writeResult(rgb);
	}
}
