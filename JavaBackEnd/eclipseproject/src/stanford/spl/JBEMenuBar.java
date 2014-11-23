/*
 * @version 2014/10/22
 * - added Ctrl-Home, Ctrl-End, PgUp, PgDown hotkeys to scroll around in console
 */

package stanford.spl;

import java.awt.event.*;

import acm.io.*;
import acm.program.*;

import javax.swing.*;

public class JBEMenuBar extends ProgramMenuBar implements ActionListener {
	private static final long serialVersionUID = 1L;
	
	private JMenuItem saveItem;
	private JMenuItem quitItem;
	private JMenuItem aboutItem;
	private JMenuItem clearItem;
	
	private KeyStroke CTRL_S;
	private KeyStroke COMMAND_S;
	private KeyStroke CTRL_Q;
	private KeyStroke CTRL_W;
	private KeyStroke COMMAND_Q;
	private KeyStroke COMMAND_W;
	private KeyStroke ALT_F4;
	private KeyStroke F1;
	private KeyStroke PGUP;
	private KeyStroke PGDN;
	private KeyStroke CTRL_HOME;
	private KeyStroke CTRL_END;
	private KeyStroke COMMAND_HOME;
	private KeyStroke COMMAND_END;
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
		PGUP = KeyStroke.getKeyStroke(KeyEvent.VK_PAGE_UP, 0);
		PGDN = KeyStroke.getKeyStroke(KeyEvent.VK_PAGE_DOWN, 0);
		CTRL_HOME = KeyStroke.getKeyStroke(KeyEvent.VK_HOME, KeyEvent.CTRL_DOWN_MASK);
		CTRL_END = KeyStroke.getKeyStroke(KeyEvent.VK_END, KeyEvent.CTRL_DOWN_MASK);
		COMMAND_HOME = KeyStroke.getKeyStroke(KeyEvent.VK_HOME, KeyEvent.META_DOWN_MASK);
		COMMAND_END = KeyStroke.getKeyStroke(KeyEvent.VK_END, KeyEvent.META_DOWN_MASK);
		super.addMenus();
		addHelpMenu();
	}

	protected void addFileMenuItems(JMenu fileMenu) {
		saveItem = createStandardItem("Save As");
		saveItem.setMnemonic('S');
		saveItem.setAccelerator(CTRL_S);
		fileMenu.add(saveItem);
		
		quitItem = createProgramItem("Quit");
		quitItem.setMnemonic('Q');
		// quitItem.setAccelerator(CTRL_W);
		quitItem.setAccelerator(ALT_F4);
		fileMenu.add(quitItem);
	}
	
	protected void addEditMenuItems(JMenu jmenu) {
		super.addEditMenuItems(jmenu);
		jmenu.addSeparator();
		
		clearItem = createProgramItem("Clear Console");
		jmenu.add(clearItem);
		clearItem.addActionListener(this);
	}

	protected void addHelpMenu() {
		JMenu helpMenu = new JMenu("Help");
		helpMenu.setMnemonic('H');
		
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
					+ "Libraries originally written by Eric Roberts,\n"
					+ "with assistance from Julie Zelenski, Keith Schwarz, et al.\n"
					+ "This version of the library is unofficially maintained by Marty Stepp.",
					
					/* title */
					"About Stanford C++ Library",
					
					/* type */
					JOptionPane.INFORMATION_MESSAGE
			);
		} else if (event.getActionCommand().equals("Clear Console")) {
			getProgram().getConsole().clear();
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
		} else if (ks.equals(CTRL_HOME) || ks.equals(COMMAND_HOME)) {
			JScrollPane scroll = getScrollPane();
			if (scroll != null && scroll.getVerticalScrollBar() != null) {
				scroll.getVerticalScrollBar().setValue(scroll.getVerticalScrollBar().getMinimum());
			}
		} else if (ks.equals(CTRL_END) || ks.equals(COMMAND_END)) {
			JScrollPane scroll = getScrollPane();
			if (scroll != null && scroll.getVerticalScrollBar() != null) {
				scroll.getVerticalScrollBar().setValue(scroll.getVerticalScrollBar().getMaximum());
			}
		} else if (ks.equals(PGUP)) {
			JScrollPane scroll = getScrollPane();
			if (scroll != null && scroll.getVerticalScrollBar() != null) {
				int value = scroll.getVerticalScrollBar().getValue();
				int height = scroll.getHeight();
				int min = scroll.getVerticalScrollBar().getMinimum();
				scroll.getVerticalScrollBar().setValue(Math.max(min, value - height));
			}
		} else if (ks.equals(PGDN)) {
			JScrollPane scroll = getScrollPane();
			if (scroll != null && scroll.getVerticalScrollBar() != null) {
				int value = scroll.getVerticalScrollBar().getValue();
				int height = scroll.getHeight();
				int max = scroll.getVerticalScrollBar().getMaximum();
				scroll.getVerticalScrollBar().setValue(Math.min(max, value + height));
			}
		}
		return super.fireAccelerator(event);
	}
	
	private JScrollPane getScrollPane() {
		IOConsole console = getProgram().getConsole();
		StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
		return model.getScrollPane();
	}
}
