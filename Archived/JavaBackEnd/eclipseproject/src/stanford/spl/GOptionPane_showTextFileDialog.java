package stanford.spl;

import acm.util.TokenScanner;

public class GOptionPane_showTextFileDialog extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String text = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String title = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rows = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int cols = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		TextFileDialog.showDialog(paramJavaBackEnd.getJBEConsoleFrame(), title, text, rows, cols);
		
		// useless "ok" result for C++ lib to throw away, to make dialog modal
		SplPipeDecoder.writeResult("ok");
	}
}
