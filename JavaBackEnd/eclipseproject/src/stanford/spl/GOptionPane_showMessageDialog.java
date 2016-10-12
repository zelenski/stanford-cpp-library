package stanford.spl;

import acm.util.TokenScanner;

import javax.swing.*;

public class GOptionPane_showMessageDialog extends JBESwingCommand {
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
