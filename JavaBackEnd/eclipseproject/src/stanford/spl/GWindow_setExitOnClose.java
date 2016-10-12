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
	
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		JBEWindow localJBEWindow = paramJavaBackEnd.getWindow(str1);
		paramTokenScanner.verifyToken(",");
		String bool = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (bool.equals("true")) {
			windowsToMonitor.add(str1);
		} else {
			windowsToMonitor.remove(str1);
		}
		
		if (localJBEWindow != null) {
			// won't turn off if you re-call it and set it to false; oh well
			localJBEWindow.addWindowListener(this);
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
