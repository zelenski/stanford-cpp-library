package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GRadioButton_isSelected extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean result = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GRadioButton) {
			result = ((GRadioButton) localGObject).isSelected();
		}
		SplPipeDecoder.writeResult(result);
	}
}
