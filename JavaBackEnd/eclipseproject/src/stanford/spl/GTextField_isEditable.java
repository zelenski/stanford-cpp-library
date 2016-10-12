package stanford.spl;

import javax.swing.JTextField;

import stanford.spl.GTextField;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTextField_isEditable extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean result = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTextField) {
			JTextField interactor = (JTextField) ((GTextField) localGObject).getInteractor();
			result = interactor.isEditable();
		}
		SplPipeDecoder.writeResult(result);
	}
}
