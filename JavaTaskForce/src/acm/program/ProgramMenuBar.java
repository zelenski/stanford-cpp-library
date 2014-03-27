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
import javax.swing.event.*;

/* Class: ProgramMenuBar */
/**
 * This class standardizes the menu bars used in the ACM program package.
 * The fundamental principles behind the design of this package are:
 *
 * <p><ol>
 * <li>The most common menu operations (including, for example, the standard
 *     editing operations <b>cut</b>, <b>copy</b>, and <b>paste</b>) should
 *     always be available and require no action on the part of the programmer.
 * <p>
 * <li>It should be easy to extend the menu bar without interfering with the
 *     standard operations.
 * <p>
 * <li>Menu bars should work in a familiar way on each of the major platforms.
 *     In particular, Macintosh users expect there to be a single menu bar
 *     at the top of the screen rather than a menu bar in each window.
 * </ol>
 *
 * <p>These goals turn out to be difficult to achieve simultaneously.  In
 * particular, supporting both Macintosh-style and Windows-style menu bars
 * requires creating a parallel <code>MenuBar</code> structure behind the
 * underlying <code>JMenuBar</code>, which accounts for much of the complexity
 * in this implementation.
 *
 * <p><b>Using the <code>ProgramMenuBar</code> class</b>
 *
 * The <code>ProgramMenuBar</code> class supports two distinct disciplines
 * for listening for menu actions:
 *
 * <p><ul>
 * <li><b>Focused items</b> correspond to actions that are relevant only to the
 * component with the keyboard focus (such as <b>Cut</b>, <b>Copy</b>, and
 * <b>Paste</b>).  Clients create focused items by calling
 * <a href="#createFocusedItem(String)"><code>createFocusedItem</code></a>.
 * Activating a focused item passes an action event to the listener set by calling
 * <a href="#setFocusedListener(ActionListener)"><code>setFocusedListener</code></a>,
 * which should be called whenever a component interested in responding to menu
 * actions gains the keyboard focus.
 * <p>
 * <li><b>Program items</b> correspond to actions that are relevant throughout
 * the lifetime of the program (such as <b>Quit</b> and <b>Print</b>).
 * Clients create program items by calling
 * <a href="#createProgramItem(String)"><code>createProgramItem</code></a>.
 * Activating a program item passes an action event to the <code>menuAction</code>
 * method in the <code>Program</code> object that created the menu bar.
 * </ul>
 */
public class ProgramMenuBar extends JMenuBar implements Iterable<JMenuItem> {

/* Constant: SHIFT */
/**
 * Constant indicating that an accelerator key requires the SHIFT modifier.
 */
	public static final int SHIFT = 0x20000;

/* Constructor: ProgramMenuBar(program) */
/**
 * Creates an empty <code>ProgramMenuBar</code>.
 *
 * @usage ProgramMenuBar mbar = new ProgramMenuBar(owner);
 * @param owner The <code>Program</code> that owns this menu bar.
 */
	public ProgramMenuBar(Program owner) {
		program = owner;
		menuBarListener = new ProgramMenuBarListener(this);
		focusedListener = null;
		accelerators = new HashMap<KeyStroke,JMenuItem>();
		focusedItems = new HashSet<JMenuItem>();
		macMenuBarFlag = true;
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
 * <code>ProgramMenuBar</code> class.   The menu item is identified
 * by its action command.
 *
 * @usage JMenuItem item = mbar.createStandardItem(action);
 * @param action The action command identifying the menu item to be created
 */
	public JMenuItem createStandardItem(String action) {
		JMenuItem item = null;
		if (action.equals("Quit")) {
			item = createProgramItem(action);
			if (Platform.isMac()) {
				setAccelerator(item, 'Q');
			} else {
				item.setName("Exit");
			}
		} else if (action.equals("Cut")) {
			item = createFocusedItem(action, 'X');
			if (!Platform.isMac()) item.setName("Cut (x)");
		} else if (action.equals("Copy")) {
			item = createFocusedItem(action, 'C');
			if (!Platform.isMac()) item.setName("Copy (c)");
		} else if (action.equals("Paste")) {
			item = createFocusedItem(action, 'V');
			if (!Platform.isMac()) item.setName("Paste (v)");
		} else if (action.equals("Select All")) {
			item = createFocusedItem(action, 'A');
		} else if (action.equals("Save")) {
			item = createFocusedItem(action, 'S');
		} else if (action.equals("Save As")) {
			item = createFocusedItem(action);
		} else if (action.equals("Print")) {
			item = createProgramItem(action, 'P');
			item.setName("Print...");
		} else if (action.equals("Print Console")) {
			item = createProgramItem(action);
		} else if (action.equals("Script")) {
			item = createProgramItem(action);
			item.setName("Script...");
		} else if (action.equals("Export Applet")) {
			item = createProgramItem(action);
			item.setName("Export Applet...");
		} else if (action.equals("Submit Project")) {
			item = createProgramItem(action);
			item.setName("Submit Project...");
		} else {
			throw new ErrorException("Illegal standard menu item: " + action);
		}
		return item;
	}

/* Method: createProgramItem(action) */
/**
 * Creates a program menu item with the specified action command.  The
 * initial item has the same label as the action command, but clients
 * can change this name by calling <code>setName</code> on the item.
 *
 * @usage JMenuItem item = createProgramItem(action);
 * @param action The action command generated by this menu item
 */
	public JMenuItem createProgramItem(String action) {
		JMenuItem item = new JMenuItem(action);
		item.setActionCommand(action);
		item.addActionListener(menuBarListener);
		return item;
	}

/* Method: createProgramItem(action, key) */
/**
 * Creates a program menu item with the specified action command and accelerator key.
 *
 * @usage JMenuItem item = createProgramItem(action, key);
 * @param action The action command generated by this menu item
 * @param key The integer value of the keystroke accelerator
 */
	public JMenuItem createProgramItem(String action, int key) {
		JMenuItem item = createProgramItem(action);
		setAccelerator(item, key);
		return item;
	}

/* Method: createFocusedItem(action) */
/**
 * Creates a focused menu item with the specified action command.
 *
 * @usage JMenuItem item = createFocusedItem(action);
 * @param action The action command generated by this menu item
 */
	public JMenuItem createFocusedItem(String action) {
		JMenuItem item = createProgramItem(action);
		focusedItems.add(item);
		return item;
	}

/* Method: createFocusedItem(action, key) */
/**
 * Creates a focused menu item with the specified action command and accelerator key.
 *
 * @usage JMenuItem item = createFocusedItem(action, key);
 * @param action The action command generated by this menu item
 * @param key The integer value of the keystroke accelerator
 */
	public JMenuItem createFocusedItem(String action, int key) {
		JMenuItem item = createFocusedItem(action);
		setAccelerator(item, key);
		return item;
	}

/* Method: isFocusedItem(item) */
/**
 * Returns <code>true</code> if the item is a focused item.
 *
 * @usage if (mbar.isFocusedItem(item)) . . .
 * @param item A menu item installed in the menu bar
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
 * @param item The menu item triggered by this accelerator
 * @param key The integer value of the keystroke accelerator
 */
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
 * Enables or disables any menu items that generate the specified action command.
 *
 * @usage mbar.setEnabled(action, flag);
 * @param action The action command triggered by the menu item
 * @param flag <code>true</code> to enable the item, <code>false</code> to disable it
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
 * @param comp A descendant of the frame in which the menu is to be installed
 */
	public void install(Component comp) {
		Component contentPane = program.getContentPane();
		while (comp != null && !(comp instanceof JFrame)) {
			comp = comp.getParent();
			if (comp == contentPane && program.isAppletMode()) {
				if (!Platform.isMac() || !macMenuBarFlag) {
					program.setJMenuBar(this);
				}
				return;
			}
		}
		if (comp == null) return;
		JFrame frame = (JFrame) comp;
		if (Platform.isMac() && macMenuBarFlag) {
			if (oldStyleMenuBar == null) {
				oldStyleMenuBar = createOldStyleMenuBar();
			}
			frame.setMenuBar(oldStyleMenuBar);
		} else {
			frame.setJMenuBar(this);
			frame.validate();
		}
	}

/* Method: setMacMenuBarFlag(flag) */
/**
 * Sets a flag indicating whether applications running on the Macintosh
 * should use standard Mac menus.  The default is <code>true</code>.
 * Setting this value to <code>false</code> means that Mac programs
 * use the same in-window <code>JMenuBar</code> approach used on other
 * platforms.
 *
 * @usage setMacMenuBarFlag(flag);
 * @param flag <code>true</code> to use Mac menu style; <code>false</code> otherwise
 */
	public void setMacMenuBarFlag(boolean flag) {
		macMenuBarFlag = flag;
	}

/* Method: getMacMenuBarFlag() */
/**
 * Retrieves the setting of the Mac menu bar flag.
 *
 * @usage boolean flag = getMacMenuBarFlag();
 * @return <code>true</code> if Mac menu style is supported; <code>false</code> otherwise
 */
	public boolean getMacMenuBarFlag() {
		return macMenuBarFlag;
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
 * Triggers the accelerator associated with the keystroke implied by the key event.
 * This method returns <code>true</code> if such an accelerator exists.
 */
	public boolean fireAccelerator(KeyEvent e) {
		KeyStroke stroke = KeyStroke.getKeyStrokeForEvent(e);
		JMenuItem item = accelerators.get(stroke);
		if (item != null) {
			item.doClick(0);
			return true;
		}
		return false;
	}

/* Method: setFocusedListener(listener) */
/**
 * Registers a listener that responds while the caller holds the keyboard
 * focus.  The caller should register its listener when it acquires the
 * keyboard focus and set it to <code>null</code> when it loses it.
 *
 * @usage setFocusedListener(listener);
 * @param listener An <code>ActionListener</code> that responds to focused items
 */
	public void setFocusedListener(ActionListener listener) {
		focusedListener = listener;
	}

/* Method: iterator() */
/**
 * Returns an iterator that enumerates the individual menu items under the
 * control of the menu bar.
 *
 * @usage Iterator<JMenuItem> iterator = mbar.iterator();
 * return An iterator that enumerates the menu items
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
 * Adds menus to the menu bar.  Subclasses that wish to change the composition
 * of the menu bar beyond the default <code>File</code> and <code>Edit</code>
 * menus should override this method with one that adds the desired menus.
 */
	protected void addMenus() {
		addFileMenu();
		addEditMenu();
	}

/* Protected method: addFileMenu() */
/**
 * Installs the <code>File</code> menu.
 *
 * @usage mbar.addFileMenu();
 */
	protected void addFileMenu() {
		JMenu fileMenu = new JMenu("File");
		fileMenu.setMnemonic('F');
		addFileMenuItems(fileMenu);
		add(fileMenu);
	}

/* Protected method: addEditMenu() */
/**
 * Installs the <code>Edit</code> menu.
 *
 * @usage mbar.addEditMenu();
 */
	protected void addEditMenu() {
		JMenu editMenu = new JMenu("Edit");
		editMenu.setMnemonic('E');
		addEditMenuItems(editMenu);
		add(editMenu);
	}

/* Protected method: addFileMenuItems(menu) */
/**
 * Adds the standard <code>File</code> items to the specified menu.  Subclasses
 * can override this method to change the list of items.
 *
 * @usage mbar.addFileMenuItems(menu);
 * @param menu The menu to which the <code>File</code> items are added
 */
	protected void addFileMenuItems(JMenu menu) {
		menu.add(createStandardItem("Save"));
		menu.add(createStandardItem("Save As"));
		menu.addSeparator();
		menu.add(createStandardItem("Print"));
		menu.add(createStandardItem("Print Console"));
		menu.add(createStandardItem("Script"));
		menu.addSeparator();
		menu.add(createStandardItem("Export Applet"));
		menu.add(createStandardItem("Submit Project"));
		menu.addSeparator();
		menu.add(createStandardItem("Quit"));
	}

/* Protected method: addEditMenuItems(menu) */
/**
 * Adds the standard <code>Edit</code> items to the specified menu.  Subclasses
 * can override this method to change the list of items.
 *
 * @usage mbar.addEditMenuItems(menu);
 * @param menu The menu to which the <code>Edit</code> items are added
 */
	protected void addEditMenuItems(JMenu menu) {
		menu.add(createStandardItem("Cut"));
		menu.add(createStandardItem("Copy"));
		menu.add(createStandardItem("Paste"));
		menu.add(createStandardItem("Select All"));
	}

/* Private method: addItemToList(itemList, item) */
/**
 * Adds the specified menu item to the list.  If <code>item</code> is itself
 * a menu, this method expands the item recursively.
 *
 * @usage mbar.addItemToList(itemList, item);
 * @param itemList The <code>ArrayList</code> to which items are added
 * @param item The item to be added
 */
	private void addItemToList(ArrayList<JMenuItem> itemList, JMenuItem item) {
		if (item == null) return;
		if (item instanceof JMenu) {
			JMenu menu = (JMenu) item;
			for (int i = 0; i < menu.getItemCount(); i++) {
				addItemToList(itemList, menu.getItem(i));
			}
		} else {
			itemList.add(item);
		}
	}

/* Private method: createOldStyleMenuBar */
/**
 * Creates a <code>MenuBar</code> that has the same effect as the
 * specified <code>JMenuBar</code>.
 *
 * @usage MenuBar oldMenuBar = mbar.createOldStyleMenuBar();
 * @return A <code>MenuBar</code> whose actions are paired with the original
 */
	private MenuBar createOldStyleMenuBar() {
		MenuBar mbar = new MenuBar();
		int nMenus = getMenuCount();
		for (int i = 0; i < nMenus; i++) {
			mbar.add(createOldStyleMenu(getMenu(i)));
		}
		return mbar;
	}

/* Private method: createOldStyleMenu */
/**
 * Creates a <code>Menu</code> that has the same effect as the
 * specified <code>JMenu</code>.
 */
	private Menu createOldStyleMenu(JMenu jmenu) {
		Menu menu = new Menu(jmenu.getText());
		int nItems = jmenu.getItemCount();
		for (int i = 0; i < nItems; i++) {
			menu.add(createOldStyleMenuItem(jmenu.getItem(i)));
		}
		return menu;
	}

/* Private method: createOldStyleMenuItem */
/**
 * Creates a <code>MenuItem</code> that has the same effect as the
 * specified <code>JMenuItem</code>.
 */
	private MenuItem createOldStyleMenuItem(Object jitem) {
		if (jitem == null) {
			return new MenuItem("-");
		} else if (jitem instanceof JMenu) {
			return createOldStyleMenu((JMenu) jitem);
		} else if (jitem instanceof JCheckBoxMenuItem) {
			return new OldStyleCheckBoxMenuItem((JCheckBoxMenuItem) jitem);
		} else if (jitem instanceof JMenuItem) {
			return new OldStyleMenuItem((JMenuItem) jitem);
		}
		throw new ErrorException("Unsupported menu item type");
	}

/* Private method: setEnabled(menu, action, flag) */
/**
 * Updates the enabled state of everything in the menu that has the specified action.
 */
	private void setEnabled(JMenu item, String action, boolean flag) {
		JMenu menu = item;
		int nItems = menu.getItemCount();
		for (int i = 0; i < nItems; i++) {
			JMenuItem subItem = menu.getItem(i);
			if (subItem != null) setEnabled(subItem, action, flag);
		}
	}

/* Private method: setEnabled(item, action, flag) */
/**
 * Updates the enabled state of the menu item if it has the specified action.
 */
	private void setEnabled(JMenuItem item, String action, boolean flag) {
		if (action.equals(item.getActionCommand())) item.setEnabled(flag);
	}

/* Private instance variables */
	private Program program;
	private ActionListener menuBarListener;
	private ActionListener focusedListener;
	private HashMap<KeyStroke,JMenuItem> accelerators;
	private HashSet<JMenuItem> focusedItems;
	private MenuBar oldStyleMenuBar;
	private boolean macMenuBarFlag;

}

/* Package class: ProgramMenuBarListener */
/**
 * This class implements the listener for the standard menu items that
 * forwards their action back to the program.
 */
class ProgramMenuBarListener implements ActionListener {

/* Constructor: ProgramMenuBarListener(mbar) */
/**
 * Creates a new listener for the standard menu items that will be added to this
 * menu bar.
 */
	public ProgramMenuBarListener(ProgramMenuBar mbar) {
		menuBar = mbar;
	}

/* Method: actionPerformed(e) */
/**
 * Responds to an action event in the corresponding menu.   The effect of an
 * action event is to forward the action command back to the program.
 */
	public void actionPerformed(ActionEvent e) {
		menuBar.fireActionListeners(e);
	}

/* Private instance variables */
	private ProgramMenuBar menuBar;

}

/* Package class: OldStyleMenuItem */
/**
 * This class represents a standard Macintosh <code>MenuItem</code> that listens to
 * a <code>JMenuItem</code> and tracks its changes.
 */
class OldStyleMenuItem extends MenuItem implements ActionListener, ChangeListener {

/* Constructor: OldStyleMenuItem(jitem) */
/**
 * Creates a new <code>MenuItem</code> that tracks the changes in the specified
 * <code>JMenuItem</code>.
 */
	public OldStyleMenuItem(JMenuItem jitem) {
		super(jitem.getText());
		twin = jitem;
		addActionListener(this);
		twin.addChangeListener(this);
		setEnabled(twin.isEnabled());
		KeyStroke accelerator = twin.getAccelerator();
		if (accelerator != null) setShortcut(createShortcut(accelerator));
	}

/* Method: actionPerformed(e) */
/**
 * Responds to an action event in the Mac menu and forwards it along to
 * the actual <code>JMenuItem</code> that the client has created.
 */
	public void actionPerformed(ActionEvent e) {
		if (e != e) /* Avoid the unused parameter warning */;
		twin.doClick(0);
	}

/* Method: stateChanged(e) */
/**
 * Monitors the state of the <code>JMenuItem</code> and replicates changes
 * in the enabled state.
 */
	public void stateChanged(ChangeEvent e) {
		setEnabled(twin.isEnabled());
	}

/* Private method: createShortcut(accelerator) */
/**
 * Creates an old-style menu shortcut from the new-style accelerator.
 */
	private MenuShortcut createShortcut(KeyStroke accelerator) {
		boolean isShifted = (accelerator.getModifiers() & Event.SHIFT_MASK) != 0;
		return new MenuShortcut(accelerator.getKeyCode(), isShifted);
	}

/* Private instance variables */
	private JMenuItem twin;
}

/* Package class: OldStyleCheckBoxMenuItem */
/**
 * This class represents a standard Macintosh <code>CheckBoxMenuItem</code> that
 * listens to a <code>JCheckBoxMenuItem</code> and tracks its changes.
 */
class OldStyleCheckBoxMenuItem extends CheckboxMenuItem implements ActionListener, ChangeListener {

/* Constructor: OldStyleCheckBoxMenuItem(jitem) */
/**
 * Creates a new <code>CheckBoxMenuItem</code> that tracks the changes in the specified
 * <code>JCheckBoxMenuItem</code>.
 */
	public OldStyleCheckBoxMenuItem(JCheckBoxMenuItem jitem) {
		super(jitem.getText());
		twin = jitem;
		addActionListener(this);
		twin.addChangeListener(this);
		setState(twin.getState());
		setEnabled(twin.isEnabled());
		KeyStroke accelerator = twin.getAccelerator();
		if (accelerator != null) setShortcut(createShortcut(accelerator));
	}

/* Method: actionPerformed(e) */
/**
 * Responds to an action event in the Mac menu and forwards it along to
 * the actual <code>JMenuItem</code> that the client has created.
 */
	public void actionPerformed(ActionEvent e) {
		if (e != e) /* Avoid the unused parameter warning */;
		twin.doClick(0);
	}

/* Method: stateChanged(e) */
/**
 * Monitors the state of the <code>JMenuItem</code> and replicates changes
 * in the enabled state.
 */
	public void stateChanged(ChangeEvent e) {
		setState(twin.getState());
		setEnabled(twin.isEnabled());
	}

/* Private method: createShortcut(accelerator) */
/**
 * Creates an old-style menu shortcut from the new-style accelerator.
 */
	private MenuShortcut createShortcut(KeyStroke accelerator) {
		boolean isShifted = (accelerator.getModifiers() & Event.SHIFT_MASK) != 0;
		return new MenuShortcut(accelerator.getKeyCode(), isShifted);
	}

/* Private instance variables */
	private JCheckBoxMenuItem twin;
}
