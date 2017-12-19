package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GWindow_removeFromRegion extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		scanner.verifyToken(",");
		String str2 = nextString(scanner);
		scanner.verifyToken(",");
		String str3 = nextString(scanner);
		scanner.verifyToken(")");
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		GObject localGObject = jbe.getGObject(str2);
		if ((window != null) && (localGObject != null)) {
			window.removeFromRegion(
					jbe.getInteractor(localGObject), str3);
		}
	}
}
