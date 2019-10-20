package stanford.spl;

import acm.util.TokenScanner;

public class AutograderInput_setColumns extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		int columns = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		AutograderInput autograderInput = AutograderInput.getInstance(paramJavaBackEnd);
		autograderInput.setColumns(columns);
	}
}
