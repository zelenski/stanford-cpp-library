package stanford.spl;

import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GInteractor_removeChangeListener extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String interactorID = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject localGObject = paramJavaBackEnd.getGObject(interactorID);
		if (localGObject != null && localGObject instanceof GInteractor) {
			GInteractor interactor = (GInteractor) localGObject;
			if (interactor instanceof GTextField) {
				((GTextField) interactor).removeDocumentListeners();
			} else if (interactor instanceof GTextArea) {
				((GTextArea) interactor).removeDocumentListeners();
			} else if (interactor instanceof GSlider) {
				((GSlider) interactor).removeChangeListeners();
			}
			// allow change listeners only on text-editing components
		}
	}
}
