package stanford.spl;

import stanford.spl.GTextField;
import acm.graphics.GObject;
import acm.gui.*;
import acm.util.TokenScanner;

public class GTextField_setPlaceholder extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String placeholder = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTextField) {
			JPlaceholderTextField interactor = (JPlaceholderTextField) ((GTextField) localGObject).getInteractor();
			interactor.setPlaceholder(placeholder);
		}
	}
}
