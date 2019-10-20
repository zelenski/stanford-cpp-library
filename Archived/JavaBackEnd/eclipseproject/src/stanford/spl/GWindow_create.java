package stanford.spl;

import stanford.spl.TopCompound;
import acm.util.TokenScanner;

public class GWindow_create extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		scanner.verifyToken(",");
		int width = nextInt(scanner);
		scanner.verifyToken(",");
		int height = nextInt(scanner);
		scanner.verifyToken(",");
		String str2 = nextString(scanner);
		scanner.verifyToken(",");
		boolean visible = nextBoolean(scanner);
		scanner.verifyToken(")");
		
		// creates an object of type JBEWindow
		jbe.createWindow(windowId, width, height, (TopCompound) jbe.getGObject(str2), visible);
		SplPipeDecoder.writeOK();
	}
}