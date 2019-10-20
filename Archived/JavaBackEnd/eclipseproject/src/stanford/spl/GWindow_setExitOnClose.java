package stanford.spl;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.HashSet;
import java.util.Set;

import javax.swing.Timer;

import acm.util.TokenScanner;

public class GWindow_setExitOnClose extends JBESwingCommand implements ActionListener, WindowListener {
	private Set<String> windowsToMonitor = new HashSet<String>();
	
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String windowId = nextString(scanner);
		JBEWindowInterface window = jbe.getWindowInterface(windowId);
		scanner.verifyToken(",");
		String bool = nextString(scanner);
		scanner.verifyToken(")");
		if (bool.equals("true")) {
			windowsToMonitor.add(windowId);
		} else {
			windowsToMonitor.remove(windowId);
		}
		
		if (window != null) {
			// won't turn off if you re-call it and set it to false; oh well
			window.addWindowListener(this);
		}
	}

	public void windowClosing(WindowEvent e) {
		if (!(e.getSource() instanceof JBEWindow)) { return; }
		JBEWindow window = (JBEWindow) e.getSource();
		String id = window.getWindowId();
		if (windowsToMonitor.contains(id)) {
			// shut down after a delay to let any messages print out
			Timer tim = new Timer(500, this);
			tim.start();
		}
	}
	
	public void actionPerformed(ActionEvent e) {
		System.exit(0);
	}
	
	public void windowActivated(WindowEvent arg0) {}
	public void windowClosed(WindowEvent e) {}
	public void windowDeactivated(WindowEvent e) {}
	public void windowDeiconified(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}
	public void windowOpened(WindowEvent e) {}
}
