package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GInteractor_isEnabled extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean result = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			result = ((GInteractor) localGObject).isEnabled();
		}
		SplPipeDecoder.writeResult(result);
	}
}
