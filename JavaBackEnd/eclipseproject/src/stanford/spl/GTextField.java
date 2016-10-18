package stanford.spl;

import java.awt.event.ActionListener;
import acm.gui.JPlaceholderTextField;;

public class GTextField extends GInteractor {
	public GTextField(int paramInt, ActionListener paramActionListener) {
		super(new JPlaceholderTextField(paramInt));
		JPlaceholderTextField localJTextField = (JPlaceholderTextField) getInteractor();
		if (paramActionListener != null) {
			localJTextField.addActionListener(paramActionListener);
		}
	}
}
