/*
 * @author Marty Stepp
 * @version 2016/11/24
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.gui.ClipboardUtils;

public class Clipboard_get extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		String text = ClipboardUtils.get();
		SplPipeDecoder.writeResult(text);
	}
}
