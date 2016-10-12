package stanford.spl;

import java.awt.Dimension;

import acm.util.TokenScanner;

public class GWindow_getCanvasSize extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		Dimension dim = new Dimension(0, 0);
		if (localJBEWindow != null) {
			dim = localJBEWindow.getCanvasSize();
		}
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
