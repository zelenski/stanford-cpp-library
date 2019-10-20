/*
 * @version 2016/10/25
 * - initial version
 */

package stanford.spl;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setFont extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String font = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject instanceof GInteractor) {
			((GInteractor) localGObject).setFont(font);
		}
	}
}
