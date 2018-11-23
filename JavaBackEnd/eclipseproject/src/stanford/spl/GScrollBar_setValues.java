/*
 * @version 2018/07/16
 * - initial version
 */

package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GScrollBar_setValues extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		int value = nextInt(scanner);
		scanner.verifyToken(",");
		int extent = nextInt(scanner);
		scanner.verifyToken(",");
		int min = nextInt(scanner);
		scanner.verifyToken(",");
		int max = nextInt(scanner);
		scanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GScrollBar) {
			GScrollBar scrollBar = (GScrollBar) gobj;
			scrollBar.setValues(value, extent, min, max);
			SplPipeDecoder.writeOK();
		} else {
			SplPipeDecoder.writeError("Not a scroll bar: " + id);
		}
	}
}