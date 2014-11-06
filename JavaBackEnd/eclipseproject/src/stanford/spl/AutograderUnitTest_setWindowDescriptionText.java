package stanford.spl;

import acm.util.TokenScanner;

class AutograderUnitTest_setWindowDescriptionText extends JBECommand {

	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String description = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean isStyleCheck = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(isStyleCheck);
		gui.setDescription(description);
	}
}
