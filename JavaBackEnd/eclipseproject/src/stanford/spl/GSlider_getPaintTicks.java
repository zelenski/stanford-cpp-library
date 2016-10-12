package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GSlider_getPaintTicks extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean result = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GSlider) {
			result = ((GSlider) localGObject).getPaintTicks();
		}
		SplPipeDecoder.writeResult(result);
	}
}
