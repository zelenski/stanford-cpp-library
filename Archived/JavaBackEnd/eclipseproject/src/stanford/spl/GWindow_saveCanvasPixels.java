/*
 * Given a window and a file name, dumps the pixels of that window's canvas to that image file.
 * @version 2016/10/09
 * - now writes ack of "ok" on finish
 * @version 2016/07/30
 * - initial version
 */

package stanford.spl;

import acm.util.*;
import stanford.cs106.io.IORuntimeException;

public class GWindow_saveCanvasPixels extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String windowID = nextString(paramTokenScanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowID);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (window == null) {
			SplPipeDecoder.writeError("window not found");
		} else {
			try {
				window.saveCanvasPixels(filename);
				SplPipeDecoder.writeOK();
			} catch (IORuntimeException ioe) {
				SplPipeDecoder.writeError(ioe);
			}
		}
	}
}
