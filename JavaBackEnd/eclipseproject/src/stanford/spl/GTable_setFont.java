package stanford.spl;

import acm.graphics.GObject;
import acm.util.JTFTools;
import acm.util.TokenScanner;
import java.awt.Font;

public class GTable_setFont extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String fontStr = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTable) {
			GTable table = (GTable) localGObject;
			Font font = JTFTools.decodeFont(fontStr);
			table.setFont(font);
		}
	}
}
