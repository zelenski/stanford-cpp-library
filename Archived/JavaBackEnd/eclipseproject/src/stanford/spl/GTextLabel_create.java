/*
 * @version 2017/10/12
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GTextLabel_create extends JBESwingCommand {
	// gTextArea = new GTextArea(width, height);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String text = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GTextLabel label = new GTextLabel(text);
		jbe.defineGObject(id, label);	       
	}
}