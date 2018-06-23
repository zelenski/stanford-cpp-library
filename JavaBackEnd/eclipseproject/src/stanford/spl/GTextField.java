/*
 * @version 2018/06/23
 * - added document listener functionality
 */

package stanford.spl;

import java.awt.event.ActionListener;
import java.util.*;
import javax.swing.event.*;
import acm.gui.JPlaceholderTextField;

public class GTextField extends GInteractor {
	private List<DocumentListener> documentListeners;
	
	public GTextField(int paramInt, ActionListener paramActionListener) {
		super(new JPlaceholderTextField(paramInt));
		JPlaceholderTextField localJTextField = (JPlaceholderTextField) getInteractor();
		if (paramActionListener != null) {
			localJTextField.addActionListener(paramActionListener);
		}
	}
	
	public void addDocumentListener(DocumentListener listener) {
		JPlaceholderTextField textField = (JPlaceholderTextField) getInteractor();
		textField.getDocument().addDocumentListener(listener);
		if (documentListeners == null) {
			documentListeners = new ArrayList<DocumentListener>();
		}
		documentListeners.add(listener);
	}
	
	public boolean hasDocumentListener() {
		return documentListeners != null && !documentListeners.isEmpty();
	}
	
	public void removeDocumentListener(DocumentListener listener) {
		JPlaceholderTextField textField = (JPlaceholderTextField) getInteractor();
		textField.getDocument().removeDocumentListener(listener);
		if (documentListeners != null) {
			documentListeners.remove(listener);
		}
	}
	
	public void removeDocumentListeners() {
		JPlaceholderTextField textField = (JPlaceholderTextField) getInteractor();
		if (documentListeners != null) {
			for (DocumentListener listener : documentListeners) {
				textField.getDocument().removeDocumentListener(listener);
			}
		}
	}
}
