package stanford.spl;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setTooltip extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String tooltipText = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			GInteractor ginteractor = (GInteractor) localGObject;
			ginteractor.setTooltip(tooltipText);
		}
	}
}
