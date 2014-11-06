package stanford.spl;

import java.awt.Dimension;

import acm.util.TokenScanner;

class GWindow_getCanvasSize extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		Dimension dim = new Dimension(0, 0);
		if (localJBEWindow != null) {
			dim = localJBEWindow.isShowing() ? localJBEWindow.getCanvas().getPreferredSize() : localJBEWindow.getCanvas().getSize();
		}
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
