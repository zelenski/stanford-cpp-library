package stanford.spl;

import acm.util.TokenScanner;

public class AutograderInput_removeCategory extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String name = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		AutograderInput autograderInput = AutograderInput.getInstance(paramJavaBackEnd);
		autograderInput.removeCategory(name);
	}
}
