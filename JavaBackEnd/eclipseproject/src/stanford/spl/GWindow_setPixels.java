package stanford.spl;

import acm.util.*;
import stanford.cs106.io.IORuntimeException;

public class GWindow_setPixels extends JBESwingCommand {
	// gbufferedimage.load("foobar.png");
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		String base64 = nextBase64(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (localJBEWindow == null) {
			return;
		}
		
		try {
			localJBEWindow.getCanvas().setPixelsFromString(base64);
		} catch (IORuntimeException ioe) {
			ioe.printStackTrace();
		}
	}
}
