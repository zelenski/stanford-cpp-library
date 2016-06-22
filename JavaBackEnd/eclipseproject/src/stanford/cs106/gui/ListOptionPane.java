package stanford.cs106.gui;

import javax.swing.JFrame;

import acm.gui.*;

/**
 * This class is here only for backward compatibility.
 * @author stepp
 *
 */
public class ListOptionPane extends JListOptionPane {
	public ListOptionPane(JFrame frame, Iterable<String> items) {
		super(frame, items);
	}
}
