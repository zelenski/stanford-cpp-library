package stanford.spl.pipecommand;

import acm.util.TokenScanner;

import javax.swing.*;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

public class GOptionPane_showMessageDialog extends JBECommand {
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
		JOptionPane.showMessageDialog(paramJavaBackEnd.getJBEConsoleFrame(), message, title, type);
		
		// useless "ok" result for C++ lib to throw away, to make dialog modal
		SplPipeDecoder.writeResult("ok");
	}
}
