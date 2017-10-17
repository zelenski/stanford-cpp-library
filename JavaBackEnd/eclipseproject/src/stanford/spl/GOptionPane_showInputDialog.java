/*
 * @version 2017/10/12
 * - modified to accept optional initial value
 */

package stanford.spl;

import acm.util.TokenScanner;

import javax.swing.*;

public class GOptionPane_showInputDialog extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String message = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String title = SplPipeDecoder.readAndDecode(paramTokenScanner);
		if (title.isEmpty()) {
			title = null;
		}
		paramTokenScanner.verifyToken(",");
		String initialValue = SplPipeDecoder.readAndDecode(paramTokenScanner);
		if (initialValue.isEmpty()) {
			initialValue = null;
		}
		paramTokenScanner.verifyToken(")");
		
		Object input;
		if (initialValue != null && !initialValue.isEmpty()) {
			input = JOptionPane.showInputDialog(paramJavaBackEnd.getJBEConsoleFrame(),
				message, title,
				JOptionPane.DEFAULT_OPTION,
				/* icon */ null,
				/* selection values */ null,
				/* initially selected */ initialValue);
		} else {
			input = JOptionPane.showInputDialog(paramJavaBackEnd.getJBEConsoleFrame(),
					message, title,
					JOptionPane.DEFAULT_OPTION);
		}
		
		String inputStr; 
		if (input == null) {
			inputStr = "";
		} else {
			inputStr = String.valueOf(input);
		}
		SplPipeDecoder.encodeAndWrite(inputStr);
	}
}
