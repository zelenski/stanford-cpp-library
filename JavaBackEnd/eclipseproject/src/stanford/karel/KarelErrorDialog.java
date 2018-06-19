package stanford.karel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

import javax.swing.JButton;
import javax.swing.JDialog;

import acm.util.JTFTools;

/* Private class: KarelErrorDialog */
/**
 * The <code>KarelErrorDialog</code> class is used to display error messages in
 * Karel's world.
 */
class KarelErrorDialog extends JDialog implements WindowListener,
		ActionListener, KeyListener {

	private Frame parent;

	public KarelErrorDialog(KarelProgram program) {
		super(JTFTools.getEnclosingFrame(program.getWorld()), true);
		parent = JTFTools.getEnclosingFrame(program.getWorld());
		setLayout(new BorderLayout(10, 10));
		init();
	}

	public KarelErrorDialog(KarelWorld world) {
		super(JTFTools.getEnclosingFrame(world), true);
		parent = JTFTools.getEnclosingFrame(world);
		setLayout(new BorderLayout(10, 10));
		init();
	}

	private void init() {
		setSize(DIALOG_WIDTH, DIALOG_HEIGHT);
		setFont(DIALOG_FONT);
		setBackground(DIALOG_BGCOLOR);
		setResizable(false);
		addWindowListener(this);
		setLayout(new BorderLayout());
		HPanel hbox = new HPanel();
		hbox.setBackground(DIALOG_BGCOLOR);
		VPanel vbox = new VPanel();
		vbox.setBackground(DIALOG_BGCOLOR);
		bugIcon = new KarelBugIcon();
		okButton = new JButton("OK");
		okButton.addActionListener(this);
		okButton.setMnemonic('O');
		errorDisplay = new KarelErrorCanvas(this);
		errorDisplay.setFont(DIALOG_FONT);
		hbox.add("/width:" + LOGO_WIDTH + "/height:" + LOGO_HEIGHT, bugIcon);
		hbox.add("/stretch:both", errorDisplay);
		vbox.add("/stretch:both", hbox);
		vbox.add("/top:3/bottom:3/width:" + BUTTON_WIDTH + "/center", okButton);
		add(BorderLayout.CENTER, vbox);
		
		okButton.addKeyListener(this);
		errorDisplay.addKeyListener(this);
		addKeyListener(this);
		
		validate();
		pack();
		
		// center with respect to parent
		int centerX = (int) ((parent.getX() + parent.getWidth()  / 2) - DIALOG_WIDTH  / 2);
		int centerY = (int) ((parent.getY() + parent.getHeight() / 2) - DIALOG_HEIGHT / 2);
		setLocation(centerX, centerY);
		
		okButton.requestFocus();
	}

	public void error(String msg) {
		error(msg, "");
	}
	
	public void error(String msg, String stackTrace) {
		errorDisplay.setText(msg);
		errorDisplay.setStackTrace(stackTrace);
		setVisible(true);
	}
	
	public void error(Throwable throwable) {
		String errorMessage = throwable.getMessage();
		if (errorMessage == null || errorMessage.isEmpty()) {
			errorMessage = "";
			if (throwable instanceof StackOverflowError) {
				errorMessage = "Infinite method calls.";
			}
		}
		while (!errorMessage.endsWith("\n\n")) {
			errorMessage += "\n";
		}
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		PrintStream out = new PrintStream(bytes);
		throwable.printStackTrace(out);
		String stackTrace = bytes.toString();
		error(errorMessage, stackTrace);
	}

	/* WindowListener interface */

	public void windowClosing(WindowEvent e) {
		setVisible(false);
	}

	public void windowOpened(WindowEvent e) {
		// empty
	}

	public void windowClosed(WindowEvent e) {
		// empty
	}

	public void windowIconified(WindowEvent e) {
		// empty
	}

	public void windowDeiconified(WindowEvent e) {
		// empty
	}

	public void windowActivated(WindowEvent e) {
		// empty
	}

	public void windowDeactivated(WindowEvent e) {
		// empty
	}

	/* ActionListener interface */

	public void actionPerformed(ActionEvent e) {
		Component source = (Component) e.getSource();
		if (source == okButton) {
			windowClosing(null);
		}
	}

	/* Private constants */

	private static final int DIALOG_WIDTH = 630;
	private static final int DIALOG_HEIGHT = 370;
	private static final int LOGO_WIDTH = 100;
	private static final int LOGO_HEIGHT = 100;
	private static final int BUTTON_WIDTH = 60;
	private static final Font DIALOG_FONT = new Font("SansSerif", Font.PLAIN, 12);
	public static final Color DIALOG_BGCOLOR = new Color(255, 128, 128);   // Color.RED;

	/* Private state */

	private KarelBugIcon bugIcon;
	private JButton okButton;
	private KarelErrorCanvas errorDisplay;

	/* KeyListener interface */
	
	public void keyPressed(KeyEvent event) {
		int code = event.getKeyCode();
		if (code == KeyEvent.VK_ESCAPE || code == KeyEvent.VK_ENTER || code == KeyEvent.VK_SPACE) {
			okButton.doClick();
		}
	}

	public void keyReleased(KeyEvent event) {
		// empty
	}

	public void keyTyped(KeyEvent event) {
		// empty
	}

}
