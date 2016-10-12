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
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String windowID = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(windowID);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (window == null) {
			SplPipeDecoder.writeResult("error:window not found");
		} else {
			try {
				window.saveCanvasPixels(filename);
				SplPipeDecoder.writeResult("ok");
			} catch (IORuntimeException ioe) {
				SplPipeDecoder.writeResult("error:" + ioe.getMessage());
			}
		}
	}
}
