/*
 * @version 2016/12/01
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class AutograderUnitTest_catchExceptions extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		AutograderUnitTestGUI gui = AutograderUnitTestGUI.getInstance(jbe, /* isStyleCheck */ false);
		boolean catching = gui.catchExceptions();
		SplPipeDecoder.writeResult(catching);
	}
}
