package stanford.spl.pipecommand;

import java.awt.Dimension;

import stanford.spl.JBECommand;
import stanford.spl.JBEWindow;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

import acm.util.TokenScanner;

public class GWindow_getSize extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		Dimension dim = new Dimension(0, 0);
		if (localJBEWindow != null) {
			dim = localJBEWindow.getSize();
		}
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
