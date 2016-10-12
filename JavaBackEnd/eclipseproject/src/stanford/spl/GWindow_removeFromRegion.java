package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GWindow_removeFromRegion extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner,
			JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String str2 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String str3 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		GObject localGObject = paramJavaBackEnd.getGObject(str2);
		if ((localJBEWindow != null) && (localGObject != null)) {
			localJBEWindow.removeFromRegion(
					paramJavaBackEnd.getInteractor(localGObject), str3);
		}
	}
}
