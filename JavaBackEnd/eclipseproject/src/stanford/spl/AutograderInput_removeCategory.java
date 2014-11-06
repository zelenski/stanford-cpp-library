package stanford.spl;

import acm.util.TokenScanner;

class AutograderInput_removeCategory extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String name = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		AutograderInput autograderInput = AutograderInput.getInstance();
		autograderInput.removeCategory(name);
	}
}
