package stanford.spl.pipecommand;

import stanford.spl.AutograderUnitTestGUI;
import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;
import acm.util.TokenScanner;

public class AutograderUnitTest_setWindowDescriptionText extends JBECommand {

	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String description = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean isStyleCheck = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(paramJavaBackEnd, isStyleCheck);
		gui.setDescription(description);
	}
}
