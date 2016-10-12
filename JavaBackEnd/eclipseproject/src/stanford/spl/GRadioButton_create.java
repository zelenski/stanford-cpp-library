/*
 * 
 * @author Marty Stepp
 * @version 2015/06/20
 */

package stanford.spl;

import acm.util.*;

public class GRadioButton_create extends JBESwingCommand {
	// grb = new GRadioButton(label, group);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String label = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String group = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GRadioButton grb = new GRadioButton(label, group);
		paramJavaBackEnd.defineGObject(id, grb);
		paramJavaBackEnd.defineSource(grb.getInteractor(), id);
	}
}
