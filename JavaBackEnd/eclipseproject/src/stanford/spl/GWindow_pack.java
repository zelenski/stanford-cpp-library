package stanford.spl;

import acm.util.TokenScanner;

public class GWindow_pack extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(")");
		if (localJBEWindow != null) {
			// BUGFIX: some kind of race condition on packing windows; slight delay helps
			try {
				Thread.sleep(50);
			} catch (InterruptedException ie) {
				// empty
			}
			localJBEWindow.pack();
		}
	}
}
