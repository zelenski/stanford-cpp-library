package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GSlider_getMajorTickSpacing extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		int result = -1;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GSlider) {
			result = ((GSlider) localGObject).getMajorTickSpacing();
		}
		SplPipeDecoder.writeResult(result);
	}
}
