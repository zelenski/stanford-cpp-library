package stanford.spl.pipecommand;

import javax.swing.*;

import stanford.spl.GButton;
import stanford.spl.GCheckBox;
import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setIcon extends JBECommand {
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
