/*
 * @version 2018/01/23
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class AutograderUnitTest_runTestsInSeparateThreads extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(jbe, /* isStyleCheck */ false);
		boolean runInThreads = gui.runTestsInSeparateThreads();
		SplPipeDecoder.writeResult(runInThreads);
	}
}
