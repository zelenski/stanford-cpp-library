/*
 * @version 2016/10/25
 * - initial version
 */

package stanford.spl;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setAccelerator extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String accelerator = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		boolean success = false;
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject instanceof GInteractor) {
			success = ((GInteractor) localGObject).setAccelerator(accelerator);
		}
		
		if (!success) {
			SplPipeDecoder.writeError("unable to set accelerator for interactor: " + interactorID);
		}
	}
}
