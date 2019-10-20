package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GFormattedPane_setContentType extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		String contentType = SplPipeDecoder.readAndDecode(scanner);
		scanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GFormattedPane) {
			GFormattedPane pane = (GFormattedPane) gobj;
			pane.setContentType(contentType);
			SplPipeDecoder.writeOK();
		} else {
			SplPipeDecoder.writeError("Not a formatted pane: " + id);
		}
	}
}