package stanford.spl;

import acm.util.TokenScanner;

public class AutograderUnitTest_isChecked extends JBESwingCommand {

	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String testName = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(paramJavaBackEnd);
		boolean checked = gui.isChecked(testName);
		SplPipeDecoder.writeResult(checked);
	}
}
