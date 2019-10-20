/*
 * @version 2018/07/16
 * - initial version
 */

package stanford.spl;

import acm.util.TokenScanner;

public class GScrollBar_create extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		int orientation = nextInt(scanner);
		scanner.verifyToken(",");
		int value = nextInt(scanner);
		scanner.verifyToken(",");
		int extent = nextInt(scanner);
		scanner.verifyToken(",");
		int min = nextInt(scanner);
		scanner.verifyToken(",");
		int max = nextInt(scanner);
		scanner.verifyToken(")");
		
		GScrollBar scrollBar = new GScrollBar(orientation, value, extent, min, max);
		jbe.defineGObject(id, scrollBar);
		scrollBar.addAdjustmentListener(jbe.createAdjustmentListener(scrollBar));
		
		// TODO: getInteractor here returns the JScrollPane; should it be instead linked to the JEditorPane?
		jbe.defineSource(scrollBar.getInteractor(), id);
	}
}