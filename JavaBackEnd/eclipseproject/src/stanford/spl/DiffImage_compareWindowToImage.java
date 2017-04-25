/*
 * Given a graphical window and an image file name, pops up a DiffImage window to compare the
 * window (actual output) to the image (expected output) for differences.
 * @author Marty Stepp
 * @version 2017/04/25
 * - moved guts of diffing logic to GCanvas, where it arguably should have always been
 * @version 2016/10/16
 * - added ignoreWindowSize option
 * @version 2016/07/30
 * - added descriptive text labels to the two sides of the diff
 * @version 2016/07/06
 * - initial version
 */

package stanford.spl;

import acm.util.*;
import stanford.cs106.io.IORuntimeException;

public class DiffImage_compareWindowToImage extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String windowID = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(windowID);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean ignoreWindowSize = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (window != null) {
			// convert the window canvas contents into an Image
			JBECanvas canvas = window.getCanvas();
			try {
				canvas.diff(filename, ignoreWindowSize);
				
				// useless "ok" result for C++ lib to throw away, to make dialog modal
				SplPipeDecoder.writeOK();
			} catch (IORuntimeException ioe) {
				SplPipeDecoder.writeError(ioe.getMessage());
			}
		}
		
	}
}
