/*
 * @version 2016/10/16
 * - moved most of logic into GInteractor class
 * - support for GRadioButton and GLabel icons
 */

package stanford.spl;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setIcon extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject instanceof GInteractor) {
			((GInteractor) localGObject).setIcon(filename);
		}
	}
}
