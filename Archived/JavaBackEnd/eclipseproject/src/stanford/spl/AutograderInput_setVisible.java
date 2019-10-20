package stanford.spl;

import acm.util.TokenScanner;

public class AutograderInput_setVisible extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		boolean value = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		AutograderInput autograderInput = AutograderInput.getInstance(paramJavaBackEnd);
		autograderInput.setVisible(value);
	}
}
