package stanford.spl.pipecommand;

import javax.swing.JTextField;

import stanford.spl.GTextField;
import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTextField_isEditable extends JBECommand {
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
