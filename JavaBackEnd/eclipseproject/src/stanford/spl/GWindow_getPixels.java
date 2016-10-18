package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_getPixels extends JBESwingCommand {
	// gbufferedimage.load("foobar.png");
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		
		if (localJBEWindow == null) {
			return;
		}
		
		String base64px = localJBEWindow.getCanvas().getPixelsAsString();
		SplPipeDecoder.writeResult(base64px);
	}
}
