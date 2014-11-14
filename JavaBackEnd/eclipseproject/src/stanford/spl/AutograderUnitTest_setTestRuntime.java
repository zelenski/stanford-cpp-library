package stanford.spl;

import acm.util.TokenScanner;

class AutograderUnitTest_setTestRuntime extends JBECommand {

	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String testName = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int runtime = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance();
		gui.setTestRuntime(testName, runtime);
	}
}
