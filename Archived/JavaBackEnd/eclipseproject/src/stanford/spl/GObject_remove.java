/*
 * Had to pull out of acm.jar to fix bug with not revalidating compound after removal
 * @version 2011/11/18
 */

package stanford.spl;

import acm.graphics.GCompound;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GObject_remove extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner,
			JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str = nextString(paramTokenScanner);
		GObject localGObject = paramJavaBackEnd.getGObject(str);
		paramTokenScanner.verifyToken(")");
		if (localGObject != null) {
			GCompound localGCompound = (GCompound) localGObject.getParent();
			if (localGCompound != null) {
				localGCompound.remove(localGObject);
			}
			
			if (localGObject instanceof GInteractor) {
				// also remove interactor from screen, if it's there
				GInteractor ginter = (GInteractor) localGObject;
				ginter.setParent(null);
			}
		}
	}
}
