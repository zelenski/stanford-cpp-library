package stanford.spl;

import acm.util.TokenScanner;

import java.util.*;

import javax.swing.*;

public class GOptionPane_showOptionDialog extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String message = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String title = SplPipeDecoder.readAndDecode(paramTokenScanner);
		if (title.isEmpty()) {
			title = null;
		}
		paramTokenScanner.verifyToken(",");
		paramTokenScanner.verifyToken("{");
		
		List<String> options = new ArrayList<String>();
		while (paramTokenScanner.hasMoreTokens()) {
			String token = nextString(paramTokenScanner);
			if (token.equals("}")) {
				break;
			} else if (token.equals(",")) {
				continue;
			} else {
				options.add(SplPipeDecoder.decode(token));
			}
		}
		paramTokenScanner.verifyToken(",");
		String initiallySelected = SplPipeDecoder.readAndDecode(paramTokenScanner);
		if (initiallySelected.isEmpty()) {
			initiallySelected = null;   // tells JOptionPane not to select anything
		}
		paramTokenScanner.verifyToken(")");
		
		int result = JOptionPane.showOptionDialog(
				/* parent */ paramJavaBackEnd.getJBEConsoleFrame(),
				message,
				title,
				/* optionType */ JOptionPane.DEFAULT_OPTION,
				/* messageType */ JOptionPane.QUESTION_MESSAGE,
				/* icon */ null,
				/* Object[] options */ options.toArray(),
				initiallySelected);
		
		SplPipeDecoder.writeResult(result);
	}
}
