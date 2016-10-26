package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GInteractor_removeActionListener extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			GInteractor interactor = (GInteractor) localGObject;
			if (interactor instanceof GButton) {
				return;   // disallow add/remove of action listeners on buttons (always on)
			}
			interactor.removeActionListener(paramJavaBackEnd);
		}
	}
}
