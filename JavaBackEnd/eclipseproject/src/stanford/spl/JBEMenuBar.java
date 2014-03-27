package stanford.spl;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;

import acm.io.IOConsole;
import acm.program.ProgramMenuBar;

import javax.swing.*;

public class JBEMenuBar extends ProgramMenuBar implements ActionListener {
	private static final long serialVersionUID = 1L;
	
	private JMenuItem saveItem;
	private JMenuItem quitItem;
	private JMenuItem aboutItem;
	
	private KeyStroke CTRL_S;
	private KeyStroke COMMAND_S;
	private KeyStroke CTRL_Q;
	private KeyStroke CTRL_W;
	private KeyStroke COMMAND_Q;
	private KeyStroke COMMAND_W;
	private KeyStroke ALT_F4;
	private KeyStroke F1;
	private JavaBackEnd javaBackEnd;

	public JBEMenuBar(JavaBackEnd paramJavaBackEnd, IOConsole paramIOConsole) {
		super(new JBEDummyProgram(paramJavaBackEnd));
		getProgram().setConsole(paramIOConsole);
		this.javaBackEnd = paramJavaBackEnd;
	}

	protected void addMenus() {
		CTRL_S = KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.CTRL_DOWN_MASK);
		COMMAND_S = KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.META_DOWN_MASK);
		CTRL_Q = KeyStroke.getKeyStroke(KeyEvent.VK_Q, KeyEvent.CTRL_DOWN_MASK);
		CTRL_W = KeyStroke.getKeyStroke(KeyEvent.VK_W, KeyEvent.CTRL_DOWN_MASK);
		COMMAND_Q = KeyStroke.getKeyStroke(KeyEvent.VK_Q, KeyEvent.META_DOWN_MASK);
		COMMAND_W = KeyStroke.getKeyStroke(KeyEvent.VK_W, KeyEvent.META_DOWN_MASK);
		ALT_F4 = KeyStroke.getKeyStroke(KeyEvent.VK_F4, KeyEvent.ALT_DOWN_MASK);
		F1 = KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0);
		super.addMenus();
		addHelpMenu();
	}

	protected void addFileMenuItems(JMenu fileMenu) {
//		saveItem = new JMenuItem("Save As");
		saveItem = createStandardItem("Save As");
		saveItem.setMnemonic('S');
		saveItem.setAccelerator(CTRL_S);
		fileMenu.add(saveItem);
		
//		quitItem = new JMenuItem("Quit");
//		quitItem.setMnemonic('Q');
		quitItem = createStandardItem("Quit");
		quitItem.setMnemonic('Q');;
		// quitItem.setAccelerator(CTRL_W);
		quitItem.setAccelerator(ALT_F4);
		fileMenu.add(quitItem);
	}

	protected void addHelpMenu() {
		JMenu helpMenu = new JMenu("Help");
		helpMenu.setMnemonic('H');
		
//		aboutItem = new JMenuItem("About");
//		aboutItem.setMnemonic('A');
		// aboutItem = createProgramItem("About", 'A');
		aboutItem = createProgramItem("About");
		aboutItem.addActionListener(this);
		aboutItem.setMnemonic('A');
		aboutItem.setAccelerator(F1);
		setAccelerator(aboutItem, KeyEvent.VK_F1);
		helpMenu.add(aboutItem);
		
		add(helpMenu);
	}
	
	public void actionPerformed(ActionEvent event) {
		if (event.getActionCommand().equals("About")) {
			JOptionPane.showMessageDialog(
					/* parent component */
					getProgram().getConsole(),
					
					/* message */
					"Stanford C++ Library version " + javaBackEnd.getCppVersion() + "\n"
					+ "Java Back-End (spl.jar) version " + javaBackEnd.getJbeVersion() + "\n\n"
					+ "Libraries written by Eric Roberts,\n"
					+ "with assistance from Julie Zelenski, Keith Schwarz, et al.\n"
					+ "Libraries currently unofficially maintained by Marty Stepp.",
					
					/* title */
					"About Stanford C++ Library",
					
					/* type */
					JOptionPane.INFORMATION_MESSAGE
			);
		}
	}
	
	@Override
	public boolean fireAccelerator(KeyEvent event) {
		KeyStroke ks = KeyStroke.getKeyStrokeForEvent(event);
		if (ks.equals(CTRL_S) || ks.equals(COMMAND_S)) {
			saveItem.doClick(0);
		} else if (ks.equals(CTRL_Q) || ks.equals(CTRL_W)
				|| ks.equals(COMMAND_Q) || ks.equals(COMMAND_W)
				|| ks.equals(ALT_F4)) {
			quitItem.doClick(0);
		} else if (ks.equals(F1)) {
			aboutItem.doClick(0);
		}
		return super.fireAccelerator(event);
	}
}
