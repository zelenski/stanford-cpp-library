package stanford.spl.pipecommand;

import stanford.spl.JBECommand;
import stanford.spl.JBEWindow;
import stanford.spl.JavaBackEnd;
import acm.util.TokenScanner;

public class GWindow_setSize extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		int w = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int h = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			localJBEWindow.setSize(w, h);
		}
	}
}
