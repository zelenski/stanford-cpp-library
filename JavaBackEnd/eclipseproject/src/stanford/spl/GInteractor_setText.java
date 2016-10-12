package stanford.spl;

import javax.swing.*;

import acm.graphics.*;
import acm.util.TokenScanner;

public class GInteractor_setText extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String text = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GButton) {
			JButton component = (JButton) ((GButton) localGObject).getInteractor();
			component.setText(text);
		} else if (localGObject != null && localGObject instanceof GCheckBox) {
			JCheckBox component = (JCheckBox) ((GCheckBox) localGObject).getInteractor();
			component.setText(text);
		} else if (localGObject != null && localGObject instanceof GRadioButton) {
			JRadioButton component = (JRadioButton) ((GRadioButton) localGObject).getInteractor();
			component.setText(text);
		} else {
			// unsupported; do nothing
		}
	}
}
