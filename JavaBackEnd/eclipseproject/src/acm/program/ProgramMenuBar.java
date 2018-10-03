/*
 * @author Marty Stepp (current maintainer)
 * @version 2017/10/31
 * - added Ctrl-number hotkeys to automatically load input script and compare output
 * @version 2017/04/25
 * - added Save / Save As options to GraphicsProgram
 * - added Anti-alias checkbox to GraphicsProgram
 * - added Compare Output option to GraphicsProgram
 * - added internal image icons to menu items
 * @version 2016/11/03
 * - added Load Input Script option for console programs
 * @version 2016/06/21
 * - bug fix where Compare Output option wasn't showing in C++ console programs
 * @version 2015/10/13
 * - new menu feature "Check for Updates..." => LibraryUpdater.java
 * @version 2015/06/19
 * - fixed bug where some hotkeys (PgUp, PgDown) were not working in SPL C++ JBEDummyProgram
 * @version 2015/05/21
 * - fixed bug where Edit copy/paste options were disabled in SPL C++ JBEDummyProgram
 * @version 2015/05/14
 * - removed save, print options from GraphicsProgram menu bar
 * @version 2015/05/12
 * - added Ctrl-Home, Ctrl-End, PgUp, PgDown hotkeys to scroll around in console
 */

/*
 * @(#)ProgramMenuBar.java   1.99.1 08/12/08
 */

// ************************************************************************
// * Copyright (c) 2008 by the Association for Computing Machinery        *
// *                                                                      *
// * The Java Task Force seeks to impose few restrictions on the use of   *
// * these packages so that users have as much freedom as possible to     *
// * use this software in constructive ways and can make the benefits of  *
// * that work available to others.  In view of the legal complexities    *
// * of software development, however, it is essential for the ACM to     *
// * maintain its copyright to guard against attempts by others to        *
// * claim ownership rights.  The full text of the JTF Software License   *
// * is available at the following URL:                                   *
// *                                                                      *
// *          http://www.acm.org/jtf/jtf-software-license.pdf             *
// *                                                                      *
// ************************************************************************

// REVISION HISTORY
//
// -- V2.0 --
// Feature enhancement 2-Mar-07 (ESR)
//   1. Added menu options to implement the "Export Applet" and
//      "Submit Project" items.
//
// Feature enhancement 21-May-08 (ESR)
//   1. Significant redesign of package to support easier extensions.
//   2. Added program argument to the constructor.

package acm.program;

import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;

import stanford.cs106.gui.GuiUtils;
import stanford.cs106.io.ResourceUtils;
import stanford.karel.*;

/* Class: ProgramMenuBar */
/**
 * This class standardizes the menu bars used in the ACM program package. The
 * fundamental principles behind the design of this package are:
 *
 * <p>
 * <ol>
 * <li>The most common menu operations (including, for example, the standard
 * editing operations <b>cut</b>, <b>copy</b>, and <b>paste</b>) should always
 * be available and require no action on the part of the programmer.
 * <p>
 * <li>It should be easy to extend the menu bar without interfering with the
 * standard operations.
 * <p>
 * <li>Menu bars should work in a familiar way on each of the major platforms.
 * In particular, Macintosh users expect there to be a single menu bar at the
 * top of the screen rather than a menu bar in each window.
 * </ol>
 *
 * <p>
 * These goals turn out to be difficult to achieve simultaneously. In
 * particular, supporting both Macintosh-style and Windows-style menu bars
 * requires creating a parallel <code>MenuBar</code> structure behind the
 * underlying <code>JMenuBar</code>, which accounts for much of the complexity
 * in this implementation.
 *
 * <p>
 * <b>Using the <code>ProgramMenuBar</code> class</b>
 *
 * The <code>ProgramMenuBar</code> class supports two distinct disciplines for
 * listening for menu actions:
 *
 * <p>
 * <ul>
 * <li><b>Focused items</b> correspond to actions that are relevant only to the
 * component with the keyboard focus (such as <b>Cut</b>, <b>Copy</b>, and
 * <b>Paste</b>). Clients create focused items by calling
 * <a href="#createFocusedItem(String)"><code>createFocusedItem</code></a>.
 * Activating a focused item passes an action event to the listener set by
 * calling <a href="#setFocusedListener(ActionListener)">
 * <code>setFocusedListener</code></a>, which should be called whenever a
 * component interested in responding to menu actions gains the keyboard focus.
 * <p>
 * <li><b>Program items</b> correspond to actions that are relevant throughout
 * the lifetime of the program (such as <b>Quit</b> and <b>Print</b>). Clients
 * create program items by calling <a href="#createProgramItem(String)">
 * <code>createProgramItem</code></a>. Activating a program item passes an
 * action event to the <code>menuAction</code> method in the
 * <code>Program</code> object that created the menu bar.
 * </ul>
 */
public class ProgramMenuBar extends JMenuBar implements Iterable<JMenuItem> {

	// constants for menu bar item text
	public static final String MENU_ITEM_TEXT_ABOUT = "About...";
	public static final String MENU_ITEM_TEXT_ANTI_ALIASING = "Anti-aliasing";
	public static final String MENU_ITEM_TEXT_BACKGROUND_COLOR = "Background Color...";
	public static final String MENU_ITEM_TEXT_CHECK_FOR_UPDATES = "Check for Updates";
	public static final String MENU_ITEM_TEXT_CLEAR_CONSOLE = "Clear Console";
	public static final String MENU_ITEM_TEXT_COPY = "Copy";
	public static final String MENU_ITEM_TEXT_CUT = "Cut";
	public static final String MENU_ITEM_TEXT_EXPORT_APPLET = "Export Applet";
	public static final String MENU_ITEM_TEXT_FONT = "Font...";
	public static final String MENU_ITEM_TEXT_FOREGROUND_COLOR = "Text Color...";
	public static final String MENU_ITEM_TEXT_PASTE = "Paste";
	public static final String MENU_ITEM_TEXT_PRINT = "Print";
	public static final String MENU_ITEM_TEXT_PRINT_CONSOLE = "Print Console";
	public static final String MENU_ITEM_TEXT_QUIT = "Quit";
	public static final String MENU_ITEM_TEXT_SAVE = "Save";
	public static final String MENU_ITEM_TEXT_SAVE_AS = "Save As...";
	public static final String MENU_ITEM_TEXT_SCRIPT = "Script";
	public static final String MENU_ITEM_TEXT_SELECT_ALL = "Select All";
	public static final String MENU_ITEM_TEXT_SHOW_PIXEL_GRID = "Show Pixel Grid";
	public static final String MENU_ITEM_TEXT_SHOW_PIXEL_INFO = "Show Pixel Info";
	public static final String MENU_ITEM_TEXT_SUBMIT_PROJECT = "Submit Project";

	// constants for menu bar item text (specific to Karel programs)
	public static final String MENU_ITEM_TEXT_INTERACTIVE = "Interactive Mode";
	public static final String MENU_ITEM_TEXT_MSKAREL = "Ms. Karel";

	// constants for menu bar item text (specific to Console programs)
	public static final String MENU_ITEM_TEXT_COMPARE_OUTPUT = "Compare Output...";
	public static final String MENU_ITEM_TEXT_LOAD_INPUT_SCRIPT = "Load Input Script...";

	/* Constant: SHIFT */
	/**
	 * Constant indicating that an accelerator key requires the SHIFT modifier.
	 */
	public static final int SHIFT = 0x20000;

	// key commands for navigating around in the console window
	protected KeyStroke ALT_F4;
	protected KeyStroke COMMAND_A;
	protected KeyStroke COMMAND_B;
	protected KeyStroke COMMAND_C;
	protected KeyStroke COMMAND_END;
	protected KeyStroke COMMAND_EQUALS;
	protected KeyStroke COMMAND_HOME;
	protected KeyStroke COMMAND_I;
	protected KeyStroke COMMAND_L;
	protected KeyStroke COMMAND_MINUS;
	protected KeyStroke COMMAND_SHIFT_MINUS;
	protected KeyStroke COMMAND_P;
	protected KeyStroke COMMAND_PLUS;
	protected KeyStroke COMMAND_SHIFT_PLUS;
	protected KeyStroke COMMAND_Q;
	protected KeyStroke COMMAND_S;
	protected KeyStroke COMMAND_V;
	protected KeyStroke COMMAND_W;
	protected KeyStroke COMMAND_X;
	protected KeyStroke CTRL_1;
	protected KeyStroke CTRL_2;
	protected KeyStroke CTRL_3;
	protected KeyStroke CTRL_4;
	protected KeyStroke CTRL_5;
	protected KeyStroke CTRL_6;
	protected KeyStroke CTRL_7;
	protected KeyStroke CTRL_8;
	protected KeyStroke CTRL_9;
	protected KeyStroke CTRL_0;
	protected KeyStroke CTRL_A;
	protected KeyStroke CTRL_B;
	protected KeyStroke CTRL_C;
	protected KeyStroke CTRL_END;
	protected KeyStroke CTRL_EQUALS;
	protected KeyStroke CTRL_HOME;
	protected KeyStroke CTRL_I;
	protected KeyStroke CTRL_L;
	protected KeyStroke CTRL_MINUS;
	protected KeyStroke CTRL_SHIFT_MINUS;
	protected KeyStroke CTRL_P;
	protected KeyStroke CTRL_Q;
	protected KeyStroke CTRL_PLUS;
	protected KeyStroke CTRL_SHIFT_PLUS;
	protected KeyStroke CTRL_S;
	protected KeyStroke CTRL_V;
	protected KeyStroke CTRL_W;
	protected KeyStroke CTRL_X;
	protected KeyStroke DOWN_ARROW;
	protected KeyStroke F1;
	protected KeyStroke PGDN;
	protected KeyStroke PGUP;
	protected KeyStroke UP_ARROW;

	/* Private fields (instance variables) */
	private Program program;
	private /* ActionListener */ ProgramMenuBarListener menuBarListener;
	private ActionListener focusedListener;
	private HashMap<KeyStroke, JMenuItem> accelerators;
	private HashSet<JMenuItem> focusedItems;

	/* Constructor: ProgramMenuBar(program) */
	/**
	 * Creates an empty <code>ProgramMenuBar</code>.
	 *
	 * @usage ProgramMenuBar mbar = new ProgramMenuBar(owner);
	 * @param owner
	 *            The <code>Program</code> that owns this menu bar.
	 */
	public ProgramMenuBar(Program owner) {
		program = owner;
		menuBarListener = new ProgramMenuBarListener(this);
		focusedListener = null;
		accelerators = new HashMap<KeyStroke, JMenuItem>();
		focusedItems = new HashSet<JMenuItem>();

		ALT_F4 = KeyStroke.getKeyStroke(KeyEvent.VK_F4, KeyEvent.ALT_DOWN_MASK);
		CTRL_1 = KeyStroke.getKeyStroke(KeyEvent.VK_1, KeyEvent.CTRL_DOWN_MASK);
		CTRL_2 = KeyStroke.getKeyStroke(KeyEvent.VK_2, KeyEvent.CTRL_DOWN_MASK);
		CTRL_3 = KeyStroke.getKeyStroke(KeyEvent.VK_3, KeyEvent.CTRL_DOWN_MASK);
		CTRL_4 = KeyStroke.getKeyStroke(KeyEvent.VK_4, KeyEvent.CTRL_DOWN_MASK);
		CTRL_5 = KeyStroke.getKeyStroke(KeyEvent.VK_5, KeyEvent.CTRL_DOWN_MASK);
		CTRL_6 = KeyStroke.getKeyStroke(KeyEvent.VK_6, KeyEvent.CTRL_DOWN_MASK);
		CTRL_7 = KeyStroke.getKeyStroke(KeyEvent.VK_7, KeyEvent.CTRL_DOWN_MASK);
		CTRL_8 = KeyStroke.getKeyStroke(KeyEvent.VK_8, KeyEvent.CTRL_DOWN_MASK);
		CTRL_9 = KeyStroke.getKeyStroke(KeyEvent.VK_9, KeyEvent.CTRL_DOWN_MASK);
		CTRL_0 = KeyStroke.getKeyStroke(KeyEvent.VK_0, KeyEvent.CTRL_DOWN_MASK);
		COMMAND_A = KeyStroke.getKeyStroke(KeyEvent.VK_A, KeyEvent.META_DOWN_MASK);
		COMMAND_B = KeyStroke.getKeyStroke(KeyEvent.VK_B, KeyEvent.META_DOWN_MASK);
		COMMAND_C = KeyStroke.getKeyStroke(KeyEvent.VK_C, KeyEvent.META_DOWN_MASK);
		COMMAND_END = KeyStroke.getKeyStroke(KeyEvent.VK_END, KeyEvent.META_DOWN_MASK);
		COMMAND_EQUALS = KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS, KeyEvent.META_DOWN_MASK);
		COMMAND_HOME = KeyStroke.getKeyStroke(KeyEvent.VK_HOME, KeyEvent.META_DOWN_MASK);
		COMMAND_I = KeyStroke.getKeyStroke(KeyEvent.VK_I, KeyEvent.META_DOWN_MASK);
		COMMAND_L = KeyStroke.getKeyStroke(KeyEvent.VK_L, KeyEvent.META_DOWN_MASK);
		COMMAND_MINUS = KeyStroke.getKeyStroke(KeyEvent.VK_MINUS, KeyEvent.META_DOWN_MASK);
		COMMAND_SHIFT_MINUS = KeyStroke.getKeyStroke(KeyEvent.VK_MINUS, KeyEvent.META_DOWN_MASK | KeyEvent.SHIFT_DOWN_MASK);
		COMMAND_P = KeyStroke.getKeyStroke(KeyEvent.VK_P, KeyEvent.META_DOWN_MASK);
		COMMAND_PLUS = KeyStroke.getKeyStroke(KeyEvent.VK_PLUS, KeyEvent.META_DOWN_MASK);
		COMMAND_SHIFT_PLUS = KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS, KeyEvent.META_DOWN_MASK | KeyEvent.SHIFT_DOWN_MASK);
		COMMAND_Q = KeyStroke.getKeyStroke(KeyEvent.VK_Q, KeyEvent.META_DOWN_MASK);
		COMMAND_S = KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.META_DOWN_MASK);
		COMMAND_V = KeyStroke.getKeyStroke(KeyEvent.VK_V, KeyEvent.META_DOWN_MASK);
		COMMAND_W = KeyStroke.getKeyStroke(KeyEvent.VK_W, KeyEvent.META_DOWN_MASK);
		CTRL_A = KeyStroke.getKeyStroke(KeyEvent.VK_A, KeyEvent.CTRL_DOWN_MASK);
		CTRL_B = KeyStroke.getKeyStroke(KeyEvent.VK_B, KeyEvent.CTRL_DOWN_MASK);
		CTRL_C = KeyStroke.getKeyStroke(KeyEvent.VK_C, KeyEvent.CTRL_DOWN_MASK);
		CTRL_END = KeyStroke.getKeyStroke(KeyEvent.VK_END, KeyEvent.CTRL_DOWN_MASK);
		CTRL_EQUALS = KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS, KeyEvent.CTRL_DOWN_MASK);
		CTRL_HOME = KeyStroke.getKeyStroke(KeyEvent.VK_HOME, KeyEvent.CTRL_DOWN_MASK);
		CTRL_I = KeyStroke.getKeyStroke(KeyEvent.VK_I, KeyEvent.CTRL_DOWN_MASK);
		CTRL_L = KeyStroke.getKeyStroke(KeyEvent.VK_L, KeyEvent.CTRL_DOWN_MASK);
		CTRL_MINUS = KeyStroke.getKeyStroke(KeyEvent.VK_MINUS, KeyEvent.CTRL_DOWN_MASK);
		CTRL_SHIFT_MINUS = KeyStroke.getKeyStroke(KeyEvent.VK_MINUS, KeyEvent.CTRL_DOWN_MASK | KeyEvent.SHIFT_DOWN_MASK);
		CTRL_P = KeyStroke.getKeyStroke(KeyEvent.VK_P, KeyEvent.CTRL_DOWN_MASK);
		CTRL_PLUS = KeyStroke.getKeyStroke(KeyEvent.VK_PLUS, KeyEvent.CTRL_DOWN_MASK);
		CTRL_SHIFT_PLUS = KeyStroke.getKeyStroke(KeyEvent.VK_EQUALS, KeyEvent.CTRL_DOWN_MASK | KeyEvent.SHIFT_DOWN_MASK);
		CTRL_Q = KeyStroke.getKeyStroke(KeyEvent.VK_Q, KeyEvent.CTRL_DOWN_MASK);
		CTRL_S = KeyStroke.getKeyStroke(KeyEvent.VK_S, KeyEvent.CTRL_DOWN_MASK);
		CTRL_V = KeyStroke.getKeyStroke(KeyEvent.VK_V, KeyEvent.CTRL_DOWN_MASK);
		CTRL_W = KeyStroke.getKeyStroke(KeyEvent.VK_W, KeyEvent.CTRL_DOWN_MASK);
		DOWN_ARROW = KeyStroke.getKeyStroke(KeyEvent.VK_DOWN, 0);
		F1 = KeyStroke.getKeyStroke(KeyEvent.VK_F1, 0);
		PGDN = KeyStroke.getKeyStroke(KeyEvent.VK_PAGE_DOWN, 0);
		PGUP = KeyStroke.getKeyStroke(KeyEvent.VK_PAGE_UP, 0);
		UP_ARROW = KeyStroke.getKeyStroke(KeyEvent.VK_UP, 0);

		addMenus();
	}

	/* Method: getProgram() */
	/**
	 * Returns the <code>Program</code> object associated with this menu.
	 *
	 * @usage Program program = mbar.getProgram();
	 * @return The program associated with this menu bar
	 */
	public Program getProgram() {
		return program;
	}

	/* Method: createStandardItem(action) */
	/**
	 * Creates one of the standard menu items implemented by the
	 * <code>ProgramMenuBar</code> class. The menu item is identified by its
	 * action command.
	 *
	 * @usage JMenuItem item = mbar.createStandardItem(action);
	 * @param action
	 *            The action command identifying the menu item to be created
	 */
	public JMenuItem createStandardItem(String action, int mnemonic) {
		return createStandardItem(action, mnemonic, /* keystroke */ null);
	}

	public JMenuItem createStandardItem(String action, int mnemonic, KeyStroke keystroke) {
		boolean mac = Platform.isMac();
		action = action.intern();
		JMenuItem item = null;
		if (action.equals(MENU_ITEM_TEXT_QUIT)) {
			item = createProgramItem(action, 'Q', mac ? COMMAND_Q : ALT_F4);
			if (mac) {
				setAccelerator(item, 'Q');
			} else {
				item.setName("Exit");
			}
		} else if (action.equals(MENU_ITEM_TEXT_CUT)) {
			item = createFocusedItem(action, 'X', mac ? COMMAND_X : CTRL_X);
			item.setMnemonic('t');
			if (!mac)
				item.setName("Cut (x)");
		} else if (action.equals(MENU_ITEM_TEXT_COPY)) {
			item = createFocusedItem(action, 'C', mac ? COMMAND_C : CTRL_C);
			if (!mac)
				item.setName("Copy (c)");
		} else if (action.equals(MENU_ITEM_TEXT_PASTE)) {
			item = createFocusedItem(action, 'V', mac ? COMMAND_V : CTRL_V);
			item.setMnemonic('P');
			if (!mac)
				item.setName("Paste (v)");
		} else if (action.equals(MENU_ITEM_TEXT_COMPARE_OUTPUT)) {
			item = createProgramItem(action, 'C');
		} else if (action.equals(MENU_ITEM_TEXT_CLEAR_CONSOLE)) {
			item = createProgramItem(action, 'L', mac ? COMMAND_L : CTRL_L);
		} else if (action.equals(MENU_ITEM_TEXT_LOAD_INPUT_SCRIPT)) {
			item = createProgramItem(action, 'I', mac ? COMMAND_I : CTRL_I);
		} else if (action.equals(MENU_ITEM_TEXT_SELECT_ALL)) {
			item = createFocusedItem(action, 'A', mac ? COMMAND_A : CTRL_A);
		} else if (action.equals(MENU_ITEM_TEXT_SAVE)) {
			item = createFocusedItem(action, 'S', mac ? COMMAND_S : CTRL_S);
		} else if (action.equals(MENU_ITEM_TEXT_SAVE_AS)) {
			item = createFocusedItem(action, 'A');
		} else if (action.equals(MENU_ITEM_TEXT_PRINT)) {
			item = createProgramItem(action, 'P', mac ? COMMAND_P : CTRL_P);
			item.setName(MENU_ITEM_TEXT_PRINT);
		} else if (action.equals(MENU_ITEM_TEXT_PRINT_CONSOLE)) {
			item = createProgramItem(action);
		} else if (action.equals(MENU_ITEM_TEXT_SCRIPT)) {
			item = createProgramItem(action);
			item.setName("Script...");
		} else if (action.equals(MENU_ITEM_TEXT_EXPORT_APPLET)) {
			item = createProgramItem(action);
			item.setName("Export Applet...");
		} else if (action.equals(MENU_ITEM_TEXT_SUBMIT_PROJECT)) {
			item = createProgramItem(action);
			item.setName("Submit Project...");
		} else {
			throw new ErrorException("Illegal standard menu item: " + action);
		}
		if (keystroke != null) {
			item.setAccelerator(keystroke);
		}
		
		tryToSetIcon(item);
		return item;
	}
	
	/**
	 * Tries to look up an image icon in the spl.jar and set it on the given menu item.
	 */
	static void tryToSetIcon(JMenuItem item) {
		// try to find an icon for this menu item
		String filename = "res/icons/" + item.getText().toLowerCase()
				.replace("...", "")
				.replaceAll("[^a-zA-Z0-9_-]+", "_")
				+ ".gif";
		if (ResourceUtils.fileExists(filename)) {
			java.net.URL url = ResourceUtils.filenameToURL(filename);
			Image img = MediaTools.loadImage(url);
			if (img != null) {
				item.setIcon(new ImageIcon(img));
			} else {
				// fall back to blank 16x16 icon
				filename = "res/icons/blank.gif";
				url = ResourceUtils.filenameToURL(filename);
				img = MediaTools.loadImage(url);
				if (img != null) {
					item.setIcon(new ImageIcon(img));
				}
			}
		}
	}

	/* Method: createProgramItem(action) */
	/**
	 * Creates a program menu item with the specified action command. The
	 * initial item has the same label as the action command, but clients can
	 * change this name by calling <code>setName</code> on the item.
	 *
	 * @usage JMenuItem item = createProgramItem(action);
	 * @param action
	 *            The action command generated by this menu item
	 */
	public JMenuItem createProgramItem(String action) {
		JMenuItem item = new JMenuItem(action);
		item.setActionCommand(action);
		item.addActionListener(menuBarListener);
		tryToSetIcon(item);
		return item;
	}

	/* Method: createProgramItem(action, key) */
	/**
	 * Creates a program menu item with the specified action command and
	 * accelerator key.
	 *
	 * @usage JMenuItem item = createProgramItem(action, key);
	 * @param action
	 *            The action command generated by this menu item
	 * @param key
	 *            The integer value of the keystroke accelerator
	 */
	public JMenuItem createProgramItem(String action, int key, KeyStroke keystroke) {
		JMenuItem item = createProgramItem(action, key);
		item.setAccelerator(keystroke);
		item.setMnemonic(key);
		tryToSetIcon(item);
		return item;
	}

	public JMenuItem createProgramItem(String action, int key) {
		JMenuItem item = createProgramItem(action);
		setAccelerator(item, key);
		item.setMnemonic(key);
		tryToSetIcon(item);
		return item;
	}

	/* Method: createFocusedItem(action) */
	/**
	 * Creates a focused menu item with the specified action command.
	 *
	 * @usage JMenuItem item = createFocusedItem(action);
	 * @param action
	 *            The action command generated by this menu item
	 */
	public JMenuItem createFocusedItem(String action) {
		JMenuItem item = createProgramItem(action);
		focusedItems.add(item);
		tryToSetIcon(item);
		return item;
	}

	/* Method: createFocusedItem(action, key) */
	/**
	 * Creates a focused menu item with the specified action command and
	 * accelerator key.
	 *
	 * @usage JMenuItem item = createFocusedItem(action, key);
	 * @param action
	 *            The action command generated by this menu item
	 * @param key
	 *            The integer value of the keystroke accelerator
	 */
	public JMenuItem createFocusedItem(String action, int key) {
		return createFocusedItem(action, key, /* keystroke */ null, /* shouldSetAccelerator */ false);
	}

	public JMenuItem createFocusedItem(String action, int key, KeyStroke keystroke) {
		return createFocusedItem(action, key, keystroke, /* shouldSetAccelerator */ true);
	}

	public JMenuItem createFocusedItem(String action, int key, KeyStroke keystroke, boolean shouldSetAccelerator) {
		JMenuItem item = createFocusedItem(action);
		if (keystroke != null) {
			item.setAccelerator(keystroke);
			accelerators.put(keystroke, item);
		} else if (shouldSetAccelerator && key != ' ' && key != '\0') {
			setAccelerator(item, key);
		}
		item.setMnemonic(key);
		tryToSetIcon(item);
		return item;
	}

	@SuppressWarnings("unchecked")
	public <T extends JMenuItem> T getMenuItem(String text) {
		for (int i = 0; i < getMenuCount(); i++) {
			JMenu menu = getMenu(i);
			if (menu == null) {
				continue;
			}
			for (int j = 0; j < menu.getItemCount(); j++) {
				JMenuItem item = menu.getItem(j);
				String itemText = item.getText();
				if (itemText != null && itemText.equals(text)) {
					return (T) item;
				}
			}
		}
		return null;
	}

	/* Method: isFocusedItem(item) */
	/**
	 * Returns <code>true</code> if the item is a focused item.
	 *
	 * @usage if (mbar.isFocusedItem(item)) . . .
	 * @param item
	 *            A menu item installed in the menu bar
	 * @return <code>true</code> if the item is a program item
	 */
	public boolean isFocusedItem(JMenuItem item) {
		return focusedItems.contains(item);
	}

	/* Method: setAccelerator(item, key) */
	/**
	 * Sets the accelerator for the item as appropriate to the operating system
	 * conventions.
	 *
	 * @usage mbar.setAccelerator(item, key);
	 * @param item
	 *            The menu item triggered by this accelerator
	 * @param key
	 *            The integer value of the keystroke accelerator
	 */
	@SuppressWarnings("deprecation")
	public void setAccelerator(JMenuItem item, int key) {
		int mask = (Platform.isMac()) ? KeyEvent.META_MASK : KeyEvent.CTRL_MASK;
		if (key > 0x10000) {
			key -= SHIFT;
			mask |= KeyEvent.SHIFT_MASK;
		}
		KeyStroke stroke = KeyStroke.getKeyStroke((char) key, mask);
		accelerators.put(stroke, item);
		if (Platform.isMac()) {
			item.setAccelerator(stroke);
		} else {
			item.setMnemonic(key);
		}
	}

	/* Method: setEnabled(action, flag) */
	/**
	 * Enables or disables any menu items that generate the specified action
	 * command.
	 *
	 * @usage mbar.setEnabled(action, flag);
	 * @param action
	 *            The action command triggered by the menu item
	 * @param flag
	 *            <code>true</code> to enable the item, <code>false</code> to
	 *            disable it
	 */
	public void setEnabled(String action, boolean flag) {
		int nMenus = getMenuCount();
		for (int i = 0; i < nMenus; i++) {
			setEnabled(getMenu(i), action, flag);
		}
	}

	/* Method: install(comp) */
	/**
	 * Installs the menu bar in the <code>JFrame</code> or <code>Program</code>
	 * object enclosing the component <code>comp</code>.
	 *
	 * @usage mbar.install(comp);
	 * @param comp
	 *            A descendant of the frame in which the menu is to be installed
	 */
	public void install(Component comp) {
		@SuppressWarnings("deprecation")
		Component contentPane = program.getContentPane();
		while (comp != null && !(comp instanceof JFrame)) {
			comp = comp.getParent();
			if (comp == contentPane && program.isAppletMode()) {
				program.setJMenuBar(this);
				return;
			}
		}
		if (comp == null)
			return;
		JFrame frame = (JFrame) comp;
		frame.setJMenuBar(this);
		frame.validate();
	}

	/* Method: fireActionListeners(e) */
	/**
	 * Fires the action listeners responsible for handling the specified event.
	 * The process of choosing the appropriate handlers takes into account
	 * whether the action command is designated as program or focused.
	 */
	public void fireActionListeners(ActionEvent e) {
		if (focusedListener != null && focusedItems.contains(e.getSource())) {
			focusedListener.actionPerformed(e);
		} else {
			program.menuAction(e);
		}
	}

	/* Method: fireAccelerator(e) */
	/**
	 * Triggers the accelerator associated with the keystroke implied by the key
	 * event. This method returns <code>true</code> if such an accelerator
	 * exists.
	 */
	public boolean fireAccelerator(KeyEvent e) {
		KeyStroke stroke = KeyStroke.getKeyStrokeForEvent(e);
		JMenuItem item = accelerators.get(stroke);
		if (item != null) {
			item.doClick(0);
			return true;
		}

		//////
		if (getProgram() instanceof AbstractConsoleProgram) {
			final AbstractConsoleProgram consoleProgram = (AbstractConsoleProgram) getProgram();
			if (stroke.equals(CTRL_HOME) || stroke.equals(COMMAND_HOME)) {
				consoleProgram.scrollToTop();
				return true;
			} else if (stroke.equals(CTRL_END) || stroke.equals(COMMAND_END)) {
				consoleProgram.scrollToBottom();
				return true;
			} else if (stroke.equals(PGUP)) {
				consoleProgram.scrollPageUp();
				return true;
			} else if (stroke.equals(CTRL_PLUS) || stroke.equals(COMMAND_PLUS) || stroke.equals(CTRL_EQUALS)
					|| stroke.equals(COMMAND_EQUALS) || stroke.equals(CTRL_SHIFT_PLUS) || stroke.equals(COMMAND_SHIFT_PLUS)) {
				consoleProgram.fontEnlarge();
				return true;
			} else if (stroke.equals(CTRL_MINUS) || stroke.equals(COMMAND_MINUS) || stroke.equals(CTRL_SHIFT_MINUS) || stroke.equals(COMMAND_SHIFT_MINUS)) {
				consoleProgram.fontShrink();
				return true;
			} else if (stroke.equals(CTRL_1)
					|| stroke.equals(CTRL_2)
					|| stroke.equals(CTRL_3)
					|| stroke.equals(CTRL_4)
					|| stroke.equals(CTRL_5)
					|| stroke.equals(CTRL_6)
					|| stroke.equals(CTRL_7)
					|| stroke.equals(CTRL_8)
					|| stroke.equals(CTRL_9)
					|| stroke.equals(CTRL_0)) {
				// auto-load input script and compare output
				final int num = stroke.getKeyCode() - '0';   // 0-9
				if (consoleProgram.loadInputScript(num)) {
					new Thread(new Runnable() {
						public void run() {
							// wait up to 30 sec for program to terminate
							long startTime = System.currentTimeMillis();
							long elapsed = 0;
							final long ELAPSED_MAX = 30000;
							
							while (elapsed < ELAPSED_MAX && consoleProgram.isRunning()) {
								consoleProgram.pause(100);
								elapsed = System.currentTimeMillis() - startTime;
							}
							
							if (elapsed < ELAPSED_MAX) {
								consoleProgram.compareOutput(num);
							}
						}
					}).start();
				}
				return true;
			} else if (stroke.equals(CTRL_B) || stroke.equals(COMMAND_B)) {
				consoleProgram.fontToggleBold();
				return true;
			} else if (stroke.equals(UP_ARROW)) {
				consoleProgram.historyUp();
				return true;
			} else if (stroke.equals(DOWN_ARROW)) {
				consoleProgram.historyDown();
				return true;
			} else if (stroke.equals(PGDN)) {
				consoleProgram.scrollPageDown();
				return true;
			} else if (stroke.equals(CTRL_W) || stroke.equals(COMMAND_W)) {
				consoleProgram.exit();
				return true;
			}
		}

		return false;
	}

	/* Method: setFocusedListener(listener) */
	/**
	 * Registers a listener that responds while the caller holds the keyboard
	 * focus. The caller should register its listener when it acquires the
	 * keyboard focus and set it to <code>null</code> when it loses it.
	 *
	 * @usage setFocusedListener(listener);
	 * @param listener
	 *            An <code>ActionListener</code> that responds to focused items
	 */
	public void setFocusedListener(ActionListener listener) {
		focusedListener = listener;
	}

	/* Method: iterator() */
	/**
	 * Returns an iterator that enumerates the individual menu items under the
	 * control of the menu bar.
	 *
	 * @usage Iterator<JMenuItem> iterator = mbar.iterator(); return An iterator
	 *        that enumerates the menu items
	 */
	public Iterator<JMenuItem> iterator() {
		ArrayList<JMenuItem> itemList = new ArrayList<JMenuItem>();
		for (int i = 0; i < getMenuCount(); i++) {
			addItemToList(itemList, getMenu(i));
		}
		return itemList.iterator();
	}

	/* Protected methods */

	/* Protected method: addMenus() */
	/**
	 * Adds menus to the menu bar. Subclasses that wish to change the
	 * composition of the menu bar beyond the default <code>File</code> and
	 * <code>Edit</code> menus should override this method with one that adds
	 * the desired menus.
	 */
	protected void addMenus() {
		addFileMenu();
		Program program = getProgram();
		if (program instanceof AbstractConsoleProgram) {
			addEditMenu();
		}
		if (program instanceof AbstractConsoleProgram) {
			addConsoleOptionsMenu();
		} else if (program instanceof Karel || program instanceof KarelProgram) {
			addKarelOptionsMenu();
		} else {
			addGraphicsOptionsMenu();
		}
		addHelpMenu();
	}

	/* Protected method: addFileMenu() */
	/**
	 * Installs the <code>File</code> menu.
	 *
	 * @usage mbar.addFileMenu();
	 */
	protected void addFileMenu() {
		JMenu fileMenu = GuiUtils.createMenu("File", this);
		addFileMenuItems(fileMenu);
	}

	/* Protected method: addEditMenu() */
	/**
	 * Installs the <code>Edit</code> menu.
	 *
	 * @usage mbar.addEditMenu();
	 */
	protected void addEditMenu() {
		JMenu editMenu = GuiUtils.createMenu("Edit", this);
		addEditMenuItems(editMenu);
	}

	/**
	 * Installs the <code>Options</code> menu for console programs.
	 *
	 * @usage mbar.addConsoleOptionsMenu();
	 */
	protected void addConsoleOptionsMenu() {
		JMenu optionsMenu = GuiUtils.createMenu("Options", this);
		tryToSetIcon(GuiUtils.createMenuItem(MENU_ITEM_TEXT_FONT, menuBarListener, optionsMenu));
		tryToSetIcon(GuiUtils.createMenuItem(MENU_ITEM_TEXT_BACKGROUND_COLOR, menuBarListener, optionsMenu));
		tryToSetIcon(GuiUtils.createMenuItem(MENU_ITEM_TEXT_FOREGROUND_COLOR, menuBarListener, optionsMenu));
	}

	/**
	 * Installs the <code>Options</code> menu for graphics programs.
	 *
	 * @usage mbar.addGraphicsOptionsMenu();
	 */
	protected void addGraphicsOptionsMenu() {
		JMenu optionsMenu = GuiUtils.createMenu("Options", this);
		JCheckBoxMenuItem antialiasItem = GuiUtils.createCheckBoxMenuItem(MENU_ITEM_TEXT_ANTI_ALIASING, /* checked */ true, menuBarListener, optionsMenu);
		antialiasItem.setToolTipText("Enable / disable smoothing of edges and pixels in drawn shapes.");
		JCheckBoxMenuItem showPixelInfoItem = GuiUtils.createCheckBoxMenuItem(MENU_ITEM_TEXT_SHOW_PIXEL_INFO, /* checked */ false, menuBarListener, optionsMenu);
		showPixelInfoItem.setToolTipText("Enable / disable display of pixel x/y/color info for debugging.");
		JCheckBoxMenuItem showPixelGridItem = GuiUtils.createCheckBoxMenuItem(MENU_ITEM_TEXT_SHOW_PIXEL_GRID, /* checked */ false, menuBarListener, optionsMenu);
		showPixelGridItem.setToolTipText("Enable / disable display of grid lines on canvas for debugging.");
	}

	/* Protected method: addKarelOptionsMenu() */
	/**
	 * Installs the <code>Options</code> menu for Karel programs.
	 *
	 * @usage mbar.addKarelOptionsMenu();
	 */
	protected void addKarelOptionsMenu() {
		JMenu optionsMenu = GuiUtils.createMenu("Options", this);
		GuiUtils.createCheckBoxMenuItem(MENU_ITEM_TEXT_MSKAREL, menuBarListener, optionsMenu);
		GuiUtils.createCheckBoxMenuItem(MENU_ITEM_TEXT_INTERACTIVE, menuBarListener, optionsMenu);
	}

	protected void addHelpMenu() {
		JMenu helpMenu = GuiUtils.createMenu("Help", this);

		JMenuItem aboutItem = GuiUtils.createMenuItem(MENU_ITEM_TEXT_ABOUT, menuBarListener, helpMenu);
		aboutItem.setAccelerator(F1);
		accelerators.put(F1, aboutItem);
		tryToSetIcon(aboutItem);

		JMenuItem checkForUpdatesItem = GuiUtils.createMenuItem(MENU_ITEM_TEXT_CHECK_FOR_UPDATES, menuBarListener, helpMenu);
		tryToSetIcon(checkForUpdatesItem);
	}

	/* Protected method: addFileMenuItems(menu) */
	/**
	 * Adds the standard <code>File</code> items to the specified menu.
	 * Subclasses can override this method to change the list of items.
	 *
	 * @usage mbar.addFileMenuItems(menu);
	 * @param menu
	 *            The menu to which the <code>File</code> items are added
	 */
	protected void addFileMenuItems(JMenu menu) {
		boolean isConsole = getProgram() instanceof AbstractConsoleProgram;
		// boolean isGraphics = getProgram() instanceof GraphicsProgram;
		
		menu.add(createStandardItem(MENU_ITEM_TEXT_SAVE, 'S'));
		menu.add(createStandardItem(MENU_ITEM_TEXT_SAVE_AS, 'A'));
		menu.addSeparator();
		
		menu.add(createStandardItem(MENU_ITEM_TEXT_PRINT, 'P'));
		// menu.add(createStandardItem("Print Console"));
		// menu.add(createStandardItem("Script"));
		menu.addSeparator();

		if (isConsole) {
			tryToSetIcon(GuiUtils.createMenuItem(MENU_ITEM_TEXT_LOAD_INPUT_SCRIPT, menuBarListener, menu));
		}

		tryToSetIcon(GuiUtils.createMenuItem(MENU_ITEM_TEXT_COMPARE_OUTPUT, menuBarListener, menu));
		menu.addSeparator();

		// BUGBUG: turning off Export and Submit features;
		// they seem to not always work any more anyway and are rarely used
		// menu.add(createStandardItem("Export Applet"));
		// menu.add(createStandardItem("Submit Project"));
		// menu.addSeparator();
		
		menu.add(createStandardItem(MENU_ITEM_TEXT_QUIT, 'Q'));
	}

	/* Protected method: addEditMenuItems(menu) */
	/**
	 * Adds the standard <code>Edit</code> items to the specified menu.
	 * Subclasses can override this method to change the list of items.
	 *
	 * @usage mbar.addEditMenuItems(menu);
	 * @param menu
	 *            The menu to which the <code>Edit</code> items are added
	 */
	protected void addEditMenuItems(JMenu menu) {
		menu.add(createStandardItem(MENU_ITEM_TEXT_CUT, 'C'));
		menu.add(createStandardItem(MENU_ITEM_TEXT_COPY, 'o'));
		menu.add(createStandardItem(MENU_ITEM_TEXT_PASTE, 'P'));
		menu.add(createStandardItem(MENU_ITEM_TEXT_SELECT_ALL, 'A'));
		menu.addSeparator();
		menu.add(createStandardItem(MENU_ITEM_TEXT_CLEAR_CONSOLE, 'l'));
	}

	/* Private method: addItemToList(itemList, item) */
	/**
	 * Adds the specified menu item to the list. If <code>item</code> is itself
	 * a menu, this method expands the item recursively.
	 *
	 * @usage mbar.addItemToList(itemList, item);
	 * @param itemList
	 *            The <code>ArrayList</code> to which items are added
	 * @param item
	 *            The item to be added
	 */
	private void addItemToList(ArrayList<JMenuItem> itemList, JMenuItem item) {
		if (item == null)
			return;
		if (item instanceof JMenu) {
			JMenu menu = (JMenu) item;
			for (int i = 0; i < menu.getItemCount(); i++) {
				addItemToList(itemList, menu.getItem(i));
			}
		} else {
			itemList.add(item);
		}
	}

	/* Private method: setEnabled(menu, action, flag) */
	/**
	 * Updates the enabled state of everything in the menu that has the
	 * specified action.
	 */
	private void setEnabled(JMenu item, String action, boolean flag) {
		JMenu menu = item;
		int nItems = menu.getItemCount();
		for (int i = 0; i < nItems; i++) {
			JMenuItem subItem = menu.getItem(i);
			if (subItem != null)
				setEnabled(subItem, action, flag);
		}
	}

	/* Private method: setEnabled(item, action, flag) */
	/**
	 * Updates the enabled state of the menu item if it has the specified
	 * action.
	 */
	private void setEnabled(JMenuItem item, String action, boolean flag) {
		if (action.equals(item.getActionCommand()))
			item.setEnabled(flag);
	}

	/* Package class: ProgramMenuBarListener */
	/**
	 * This class implements the listener for the standard menu items that forwards
	 * their action back to the program.
	 */
	private static class ProgramMenuBarListener implements ActionListener {
		/* Private instance variables */
		private ProgramMenuBar menuBar;

		/* Constructor: ProgramMenuBarListener(mbar) */
		/**
		 * Creates a new listener for the standard menu items that will be added to
		 * this menu bar.
		 */
		public ProgramMenuBarListener(ProgramMenuBar mbar) {
			menuBar = mbar;
		}

		/* Method: actionPerformed(e) */
		/**
		 * Responds to an action event in the corresponding menu. The effect of an
		 * action event is to forward the action command back to the program.
		 */
		public void actionPerformed(ActionEvent e) {
			menuBar.fireActionListeners(e);
		}
	}
}
