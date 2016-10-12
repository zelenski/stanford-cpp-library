package stanford.spl;

import acm.util.TokenScanner;

public class GTextArea_create extends JBESwingCommand {
	// gTextArea = new GTextArea(width, height);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int width = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int height = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GTextArea area = new GTextArea(width, height);
		jbe.defineGObject(id, area);	       
	}
}