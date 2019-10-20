/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/04/26
 * - initial version; moved out of Program.java into its own file
 */

package acm.program;

import java.awt.Button;
import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.Method;

import acm.util.ErrorException;

/* Package class: DefaultActionListener */
/**
 * This class is the default action listener added to buttons.
 */
class DefaultActionListener implements ActionListener {

	public DefaultActionListener() {
		/* Empty */
	}

	public void actionPerformed(ActionEvent e) {
		Component comp = (Component) e.getSource();
		Program program = findProgram(comp);
		if (program != null && countActionListeners(comp) > 1) {
			try {
				Class<?>[] types = { java.awt.event.ActionListener.class };
				Object[] args = { this };
				Method removeActionListener = comp.getClass().getMethod("removeActionListener", types);
				removeActionListener.invoke(comp, args);
				return;
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
		String message = "No ActionListener is attached";
		if (comp instanceof Button) {
			message += " to button " + ((Button) comp).getLabel();
		} else {
			try {
				Method getText = comp.getClass().getMethod("getText", new Class[0]);
				message += " to button " + (String) getText.invoke(comp, new Object[0]);
			} catch (Exception ex) {
				throw new ErrorException(ex);
			}
		}
		if (program == null) {
			throw new ErrorException(message);
		} else {
			program.getDialog().showErrorMessage(message);
		}
	}

	protected static int countActionListeners(Component comp) {
		try {
			Method getActionListeners = comp.getClass().getMethod("getActionListeners", new Class[0]);
			ActionListener[] listeners = (ActionListener[]) getActionListeners.invoke(comp, new Object[0]);
			return listeners.length;
		} catch (Exception ex) {
			return -1;
		}
	}

	private Program findProgram(Component comp) {
		if (comp instanceof Program) {
			return (Program) comp;
		} else if (comp != null) {
			return findProgram(comp.getParent());
		} else {
			return null;
		}
	}
}
