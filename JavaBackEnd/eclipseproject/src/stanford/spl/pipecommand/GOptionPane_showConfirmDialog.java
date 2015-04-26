package stanford.spl.pipecommand;

import acm.util.TokenScanner;

import javax.swing.*;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

public class GOptionPane_showConfirmDialog extends JBECommand {
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
