package stanford.spl.pipecommand;

import stanford.spl.AutograderUnitTestGUI;
import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import acm.util.TokenScanner;

public class AutograderUnitTest_clearTestResults extends JBECommand {

	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		boolean isStyleCheck = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(paramJavaBackEnd, isStyleCheck);
		gui.clearTestResults();
	}
}
