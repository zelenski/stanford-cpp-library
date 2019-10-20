package stanford.spl;

import java.io.IOException;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GFormattedPane_setPage extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		String url = SplPipeDecoder.readAndDecode(scanner);
		scanner.verifyToken(")");
		
		GObject gobj = jbe.getGObject(id);
		if (gobj != null && gobj instanceof GFormattedPane) {
			GFormattedPane pane = (GFormattedPane) gobj;
			try {
				pane.setPage(url);
				SplPipeDecoder.writeOK();
			} catch (IOException ioe) {
				SplPipeDecoder.writeError(ioe);
			}
		} else {
			SplPipeDecoder.writeError("Not a formatted pane: " + id);
		}
	}
}