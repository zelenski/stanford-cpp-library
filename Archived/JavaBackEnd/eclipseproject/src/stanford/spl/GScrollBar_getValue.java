/*
 * @version 2018/07/16
 * - initial version
 */

package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GScrollBar_getValue extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GScrollBar) {
			GScrollBar scrollBar = (GScrollBar) gobj;
			int value = scrollBar.getValue();
			SplPipeDecoder.writeResult(value);
		} else {
			SplPipeDecoder.writeError("Not a scroll bar: " + id);
		}
	}
}
