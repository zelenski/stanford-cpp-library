package stanford.spl;

import javax.swing.JTextField;

import stanford.spl.GTextField;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTextField_setEditable extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean editable = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTextField) {
			JTextField interactor = (JTextField) ((GTextField) localGObject).getInteractor();
			interactor.setEditable(editable);
		}
	}
}
