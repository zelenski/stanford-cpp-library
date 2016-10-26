/*
 * @version 2016/10/25
 * - initial version
 */

package stanford.spl;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setMnemonic extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int mnemonic = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean success = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject instanceof GInteractor) {
			success = ((GInteractor) localGObject).setMnemonic(mnemonic);
		}
		
		if (!success) {
			SplPipeDecoder.writeError("unable to set mnemonic for interactor: " + interactorID);
		}
	}
}
