package stanford.spl;

import javax.swing.*;

import stanford.spl.GButton;
import stanford.spl.GCheckBox;
import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setTextPosition extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int horizontal = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int vertical = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GButton) {
			JButton component = (JButton) ((GButton) localGObject).getInteractor();
			component.setHorizontalTextPosition(horizontal);
			component.setVerticalTextPosition(vertical);
		} else if (localGObject != null && localGObject instanceof GCheckBox) {
			JCheckBox component = (JCheckBox) ((GCheckBox) localGObject).getInteractor();
			component.setHorizontalTextPosition(horizontal);
			component.setVerticalTextPosition(vertical);
		} else if (localGObject != null && localGObject instanceof GRadioButton) {
			JRadioButton component = (JRadioButton) ((GRadioButton) localGObject).getInteractor();
			component.setHorizontalTextPosition(horizontal);
			component.setVerticalTextPosition(vertical);
		} else {
			// unsupported; do nothing
		}
	}
}
