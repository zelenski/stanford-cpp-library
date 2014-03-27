package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

class GInteractor_isEnabled extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean result = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			result = ((GInteractor) localGObject).isEnabled();
		}
		System.out.println("result:" + result);
		System.out.flush();
	}
}
