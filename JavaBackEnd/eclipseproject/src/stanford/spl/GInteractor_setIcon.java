package stanford.spl;

import javax.swing.*;
import acm.graphics.*;
import acm.util.TokenScanner;

class GInteractor_setIcon extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GButton) {
			JButton component = (JButton) ((GButton) localGObject).getInteractor();
			component.setIcon(new ImageIcon(filename));
		} else if (localGObject != null && localGObject instanceof GCheckBox) {
			JCheckBox component = (JCheckBox) ((GCheckBox) localGObject).getInteractor();
			component.setIcon(new ImageIcon(filename));
		} else {
			// unsupported; do nothing
		}
	}
}
