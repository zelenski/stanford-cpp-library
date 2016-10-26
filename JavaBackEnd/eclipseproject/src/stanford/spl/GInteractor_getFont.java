/*
 * @version 2016/10/25
 * - initial version
 */

package stanford.spl;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_getFont extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject instanceof GInteractor) {
			String fontString = ((GInteractor) localGObject).getFontString();
			SplPipeDecoder.writeResult(fontString);
		} else {
			SplPipeDecoder.writeError("interactor not found or invalid: " + interactorID);
		}
	}
}
