/*
 * @author Marty Stepp
 * @version 2016/11/24
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.gui.ClipboardUtils;

public class Clipboard_set extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String text = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (text != null) {
			ClipboardUtils.copy(text);
		}
	}
}
