package stanford.spl;

import acm.util.TokenScanner;

import javax.swing.*;

public class GOptionPane_showConfirmDialog extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String message = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String title = SplPipeDecoder.readAndDecode(paramTokenScanner);
		if (title.isEmpty()) {
			title = null;
		}
		paramTokenScanner.verifyToken(",");
		paramTokenScanner.scanNumbers();
		int type = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		int choice = JOptionPane.showConfirmDialog(paramJavaBackEnd.getJBEConsoleFrame(), message, title, type);
		SplPipeDecoder.writeResult(choice);
	}
}
