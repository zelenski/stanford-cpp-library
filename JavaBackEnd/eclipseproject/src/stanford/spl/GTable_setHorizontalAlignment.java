package stanford.spl;

import javax.swing.JLabel;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GTable_setHorizontalAlignment extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String alignment = nextString(paramTokenScanner).toUpperCase();
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GTable) {
			GTable table = (GTable) localGObject;
			if (alignment.equals("LEFT")) {
				table.setHorizontalAlignment(JLabel.LEFT);
			} else if (alignment.equals("CENTER")) {
				table.setHorizontalAlignment(JLabel.CENTER);
			} else if (alignment.equals("RIGHT")) {
				table.setHorizontalAlignment(JLabel.RIGHT);
			}
		}
	}
}
