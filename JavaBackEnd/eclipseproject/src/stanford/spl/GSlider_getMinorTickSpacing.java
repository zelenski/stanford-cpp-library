package stanford.spl.pipecommand;

import stanford.spl.GSlider;
import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GSlider_getMinorTickSpacing extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		int result = -1;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GSlider) {
			result = ((GSlider) localGObject).getMinorTickSpacing();
		}
		SplPipeDecoder.writeResult(result);
	}
}
