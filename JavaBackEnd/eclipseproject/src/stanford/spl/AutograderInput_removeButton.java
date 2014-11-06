package stanford.spl;

import acm.util.TokenScanner;

class AutograderInput_removeButton extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String text = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		AutograderInput autograderInput = AutograderInput.getInstance();
		autograderInput.removeButton(text);
	}
}
