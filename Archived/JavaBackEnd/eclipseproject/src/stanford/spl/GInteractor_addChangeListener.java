/*
 * @version 2018/06/23
 * - initial version
 */

package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GInteractor_addChangeListener extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			GInteractor interactor = (GInteractor) localGObject;
			if (interactor instanceof GTextField) {
				GTextField textField = ((GTextField) interactor);
				if (!textField.hasDocumentListener()) {
					textField.addDocumentListener(paramJavaBackEnd.createDocumentListener(interactor));
				}
			} else if (interactor instanceof GTextArea) {
				GTextArea textArea = ((GTextArea) interactor);
				if (!textArea.hasDocumentListener()) {
					textArea.addDocumentListener(paramJavaBackEnd.createDocumentListener(interactor));
				}
			} else if (interactor instanceof GSlider) {
				GSlider slider = ((GSlider) interactor);
				if (!slider.hasChangeListener()) {
					slider.addChangeListener(paramJavaBackEnd);
				}
			}
			// allow change listeners only on text-editing components
		}
	}
}
