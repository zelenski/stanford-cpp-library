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
		paramTokenScanner.verifyToken(")");
		String input = JOptionPane.showInputDialog(paramJavaBackEnd.getJBEConsoleFrame(), message, title,
				JOptionPane.DEFAULT_OPTION);
		if (input == null) {
			input = "";
		}
		SplPipeDecoder.encodeAndWrite(input);
	}
}
