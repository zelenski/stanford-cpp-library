/*
 * @version 2017/10/22
 * - added alertError methods 
 * @version 2017/08/04
 * - added system property to disable load/save of configuration 
 * @version 2017/07/21
 * - added add() overloads
 * @version 2017/07/14
 * - disabled console error messages when unable to save/load config data
 * @version 2017/05/31
 * - made Program class implement various EventListener sub-interface types
 * @version 2017/05/30
 * - changes to N/S/E/W/C panel opacity and root pane enabled (for setBackground calls)
 * @version 2017/05/22
 * - added JS-style alert, confirm, and prompt methods
 * @version 2017/05/21
 * - added setCenter/Bottom/RightX/Y
 * @version 2017/04/26
 * - added setShowPixelInfo, setShowPixelGrid
 * - alphabetized methods (phew!)
 * - moved other classes out into their own files
 * @version 2017/04/25
 * - added menu bar actions to save/diff/print program's graphical output
 * @version 2016/11/13
 * - added fileSize(directory, filename)
 * @version 2016/11/03
 * - added removeFromRegion
 * @version 2016/10/21
 * - added center, pack, setWidth, setHeight, setLocation, setX, setY
 * - fixed printf bug (was mistakenly inserting \n line break at end)
 * - fixed bug in fileExistsInJar with paths that end with "/"
 * @version 2016/10/02
 * - added getYesOrNo
 * @version 2016/07/14
 * - bug fix for fileExistsInsideJAR, openFileFromJAR on Windows
 * @version 2016/05/22
 * - slight refactor of outermost exception-handling code
 *   (let exceptions bubble out to aid student debugging)
 * @version 2016/04/28
 * - bug fix in openFileFromDisk (was ignoring directory passed)
 * @version 2016/04/16
 * - added some file-reading methods to help with programs packed into JARs/applets
 *   e.g. fileExists, openFileFromJAR
 * @version 2015/10/13
 * - added animation-related methods (animation, setAnimationDelay, startAnimation, etc.)
 * - style fix: moved fields/constants to top of class as the good lord intended
 */

/*
 * @(#)Program.java   1.99.1 08/12/08
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
// Bug fix 30-Jan-07 (ESR, JTFBug 2007-002)
//   1. Change default in getCommandLine to behave as if Linux.
//   2. If environment is headless, substitute a CommandLineProgram.
//
// Feature enhancement 2-Mar-07 (ESR)
//   1. Added menu option to export the program as an applet.
//   2. Added menu option to submit the program via email.
//
// Bug fix 8-May-07 (ESR, JTFBug 2007-007)
//   1. Fixed significant bug introduced by JDK 1.6 in which it is no
//      longer possible to display a JApplet as a component.
//
// Code cleanup 28-May-07 (ESR)
//   1. Added generic type tags.
//   2. Rewrote code for handling command-line arguments.
//   3. Repackaged the ButtonLike interface.
//   4. Added setInputModel, setOutputModel, and setDialog for symmetry
//
// Bug fix 10-Sep-07 (ESR, JTFBug 2007-012)
//   1. Fixed deadlock bug arising from change to thread handling in JDK 1.6.
//
// Code cleanup 10-May-08 (ESR)
//   1. Changed code to account for introduction of CommandLineProgram class.
//
// Bug fix 21-May-08 (ESR, JTFBug 2008-002)
//   1. Fixed the logic for isAppletMode.
//
// Code cleanup 21-May-08 (ESR)
//   1. Changed factory method for the menu bar to pass the program.
//   2. Redesigned other code features to account for redesign of the
//      ProgramMenuBar class.
//
// Bug fix 10-Jun-08 (ESR, JTFBug 2008-003)
//   1. Fixed serious bug caused by overriding the definitions of
//      getWidth and getHeight (which was, in retrospect, a poor design).
//      To avoid requiring changes to client code, the implementation now
//      checks to see whether these methods have been called from inside
//      the java package hierarchy and, if so, maintains their traditional
//      semantics.

package acm.program;

import acm.graphics.*;
import acm.gui.*;
import acm.io.*;
import acm.util.*;
import stanford.cs106.gui.*;
import stanford.cs106.io.*;
import stanford.cs106.util.ExceptionUtils;
import stanford.cs106.util.StringUtils;
import stanford.spl.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

/* Class: Program */
/**
 * This class is the superclass for all executable
 * programs in the <code>acm.program</code> package.  Its principal
 * role is to unify the concepts of applets and applications in a single
 * class, although it also provides applications with many other useful
 * facilities not traditionally available in applications.
 *
 * <p>In many programming environments, objects that are specific instances
 * of a <code>Program</code> subclass will run automatically without any
 * special action on your part.  For maximum portability, you might want
 * to define a static <code>main</code> method as described in the comments
 * for the standard implementation of <a href="#main(String[])"><code>main</code></a>.
 */
@SuppressWarnings("deprecation")
public abstract class Program
		extends JApplet
		implements ActionListener,
		AdjustmentListener,
		ChangeListener,
		ComponentListener,
		ContainerListener,
		DocumentListener,
		FocusListener,
		IOModel,
		ItemListener,
		KeyListener,
		ListDataListener,
		ListSelectionListener,
		MouseListener,
		MouseInputListener,
		MouseMotionListener,
		MouseWheelListener,
		PopupMenuListener,
		ProgramInterface,
		Runnable,
		TextListener,
		WindowFocusListener,
		WindowListener,
		WindowStateListener {

	/* Private constants */
	private static final int DEFAULT_HEIGHT = 492;
	private static final int DEFAULT_WIDTH = 754;
	private static final int DEFAULT_X = 16;
	private static final int DEFAULT_Y = 40;
	private static final int PRINT_MARGIN = 36;

	/* file where Stanford lib settings (e.g. window size/font) are saved on disk */
	protected static final String CONFIG_FILE_NAME = "spl-jar-settings.txt";
	
	/* system property to disable/enable saving/loading configuration */
	private static final String CONFIG_PROPERTY = "spl.save.settings";
	
	// messages to display when re-prompting on bad user input
	private static final String GETYESORNO_DEFAULT_REPROMPT = "Please type a word that starts with 'Y' or 'N'.";

	/* Private fields */
	private AppletStarter appletStarter;
	private AppletStub appletStub;
	private ArrayList<Object> finalizers;
	private boolean animatedMode = false;
	private boolean appletMode;
	private boolean exitOnClose = true;
	private boolean initFinished;
	private boolean kill = false;
	private boolean pauseTickFlag = false;
	private boolean runFinished = false;
	private boolean shown;
	private boolean started;
	private Component eastBorder;
	private Component northBorder;
	private Component southBorder;
	private Component westBorder;
	private double pauseScaleFactor = 1.0;
	private File lastSaveFile;
	private int animationDelay = 1000 / 50;   // 50 FPS default
	private IOConsole myConsole;
	private IODialog myDialog;
	private IOModel inputModel;
	private IOModel outputModel;
	private javax.swing.Timer animationTimer = null;
	private JFrame programFrame;
	private JPanel centerPanel;
	private JPanel eastPanel;
	private JPanel northPanel;
	private JPanel southPanel;
	private JPanel westPanel;
	private Map<String, String> parameterTable;
	private Object startupObject;
	private ProgramMenuBar myMenuBar;
	private Rectangle programBounds;
	private String myTitle;
	protected Set<GObject> invisibleObjects = new HashSet<GObject>();

	/** Required method of ListDataListener interface. */
	public void	contentsChanged(ListDataEvent e) {
		// empty
	}

	/** Required method of ListDataListener interface. */
	public void	intervalAdded(ListDataEvent e) {
		// empty
	}

	/** Required method of ListDataListener interface. */
	public void	intervalRemoved(ListDataEvent e) {
		// empty
	}
	
	/** Required method of MenuListener interface. */
	public void menuCanceled(MenuEvent e) {
		// empty
	}
	
	/** Required method of MenuListener interface. */
	public void menuDeselected(MenuEvent e) {
		// empty
	}
	
	/** Required method of MenuListener interface. */
	public void menuSelected(MenuEvent e) {
		// empty
	}
	
	/** Required method of PopupMenuListener interface. */
	public void popupMenuCanceled(PopupMenuEvent e) {
		// empty
	}
	
	/** Required method of PopupMenuListener interface. */
	public void popupMenuWillBecomeInvisible(PopupMenuEvent e) {
		// empty
	}
	
	/** Required method of PopupMenuListener interface. */
	public void popupMenuWillBecomeVisible(PopupMenuEvent e) {
		// empty
	}
	
	/** Required method of ListSelectionListener interface. */
	public void valueChanged(ListSelectionEvent e) {
		// empty
	}


	
	/* Default constructor: Program */
	/**
	 * This code initializes the program data structures.
	 */
	protected Program() {
		GuiUtils.setSystemLookAndFeel();
		JTFTools.registerApplet(this);
		appletMode = checkForAppletMode();
		shown = false;
		parameterTable = null;
		finalizers = new ArrayList<Object>();
		myTitle = getClass().getName();
		myTitle = myTitle.substring(myTitle.lastIndexOf(".") + 1);
		appletStub = new ProgramAppletStub(this);
		setAppletStub(appletStub);
		initContentPane(getContentPane());
		setVisible(false);
		setConsole(createConsole());
		myDialog = createDialogIO();
		myDialog.setAssociatedConsole(myConsole);
		myMenuBar = createMenuBar();
		myConsole.setMenuBar(myMenuBar);
		addComponentListener(this);
	}

	/* Method: actionPerformed (implements ActionListener) */
	/**
	 * Called when a component (typically a button) is activated.
	 */
	public void actionPerformed(ActionEvent e) {
		// empty
	}

	/* Method: add(comp, region, constraints) */
	/**
	 * Adds the component to the specified border region with the indicated
	 * constraints object.
	 *
	 * @usage add(comp, region, constraints);
	 * @param comp The component to be added
	 * @param region The region of the window (<code>NORTH</code>, <code>SOUTH</code>,
	 *               <code>EAST</code>, <code>WEST</code>, or <code>CENTER</code>)
	 * @param constraints The constraints object
	 * @noshow
	 * @return the component passed
	 */
	public void add(Component comp, String region, Object constraints) {
		if (region.equalsIgnoreCase(NORTH)) {
			northPanel.add(comp, constraints);
		} else if (region.equalsIgnoreCase(SOUTH)) {
			southPanel.add(comp, constraints);
		} else if (region.equalsIgnoreCase(WEST)) {
			westPanel.add(comp, constraints);
		} else if (region.equalsIgnoreCase(EAST)) {
			eastPanel.add(comp, constraints);
		} else if (region.equalsIgnoreCase(CENTER)) {
			centerPanel.add(comp, constraints);
		} else {
			throw new ErrorException("add: Illegal region " + region);
		}
	}

	/**
	 * Adds the program as an <code>ActionListener</code> to every button in
	 * the structure that does not have a listener already.
	 *
	 * @usage addActionListeners();
	 */
	public void addActionListeners() {
		addActionListeners(this);
	}

	/**
	 * Adds the specified listener to every button in
	 * the structure that does not have a listener already.
	 *
	 * @usage addActionListeners(listener);
	 * @param listener The <code>ActionListener</code> to be added
	 */
	public void addActionListeners(ActionListener listener) {
		addActionListeners(getContentPane(), listener, /* buttonsOnly */ true);
	}

	/**
	 * Recursively adds the specified listener as an <code>ActionListener</code> to
	 * every button in the hierarchy.  Reflection is used because there are many
	 * possible classes of button-like objects.
	 */
	private void addActionListeners(Component comp, ActionListener listener, boolean buttonsOnly) {
		if (!buttonsOnly || isButton(comp)) {
			if (!hasActionListener(comp)) {
				try {
					Class<?>[] types = { java.awt.event.ActionListener.class };
					Object[] args = { listener };
					Method addActionListener = comp.getClass().getMethod("addActionListener", types);
					if (addActionListener != null) {
						addActionListener.invoke(comp, args);
					}
				} catch (Exception ex) {
					throw new ErrorException(ex);
				}
			}
		} else if (comp instanceof Container) {
			Container container = (Container) comp;
			int nComponents = container.getComponentCount();
			for (int i = 0; i < nComponents; i++) {
				addActionListeners(container.getComponent(i), listener, buttonsOnly);
			}
		}
	}

	/**
	 * Adds the program as an <code>ActionListener</code> to every button in
	 * the structure that does not have a listener already.
	 * Also adds action listeners to other components that have action commands.
	 *
	 * @usage addActionListeners();
	 */
	public void addActionListenersToButtonsOnly() {
		addActionListeners(getContentPane(), this, /* buttonsOnly */ true);
	}

	/**
	 * Adds the program as an <code>ActionListener</code> to every button or component in
	 * the structure that does not have a listener already.
	 * Also adds action listeners to other components that have action commands.
	 *
	 * @usage addAllActionListeners();
	 */
	public void addAllActionListeners() {
		addActionListeners(getContentPane(), this, /* buttonsOnly */ false);
	}

	/* Method: addExitHook(obj) */
	/**
	 * Requests that the program call the <code>exit</code> method in the
	 * specified object before exiting.
	 *
	 * @usage program.addExitHook(obj);
	 */
	public void addExitHook(Object obj) {
		finalizers.add(obj);
	}

	/* Overridden method: addImpl(comp, constraints, index) */
	/**
	 * Adds the specified component to the content pane using the specified constraints and index.
	 */
	protected void addImpl(Component comp, Object constraints, int index) {
		if (isRootPaneCheckingEnabled()) {
			if (constraints == null) {
				centerPanel.add(comp, index);
			} else if (constraints.equals(NORTH)) {
				northPanel.add(comp, index);
			} else if (constraints.equals(SOUTH)) {
				southPanel.add(comp, index);
			} else if (constraints.equals(WEST)) {
				westPanel.add(comp, index);
			} else if (constraints.equals(EAST)) {
				eastPanel.add(comp, index);
			} else if (constraints.equals(CENTER)) {
				centerPanel.add(comp, index);
			} else {
				centerPanel.add(comp, constraints, index);
			}
			if (!shown && programFrame != null) {
				programFrame.setVisible(true);
				shown = true;
			}
		} else {
			super.addImpl(comp, constraints, index);
		}
	}
	
	/**
	 * Required empty method of AdjustmentListener interface.
	 */
	public void adjustmentValueChanged(AdjustmentEvent e) {
		// empty
	}
	
	/**
	 * Pops up a dialog box displaying a string representation of the given message.
	 * @param value the value to display (its toString() method is implicitly called)
	 */
	public void alert(Object value) {
		alert(String.valueOf(value));
	}

	/**
	 * Pops up a dialog box displaying the given message.
	 * @param message the message to display
	 */
	public void alert(String message) {
		JOptionPane.showMessageDialog(getWindow(), message);
	}

	/**
	 * Pops up a dialog box displaying the given error message.
	 * @param message the error message to display
	 */
	public void alertError(String message) {
		JOptionPane.showMessageDialog(getWindow(), "Error: " + message, /* title */ "Error", JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * Pops up a dialog box displaying the given error message.
	 * @param error the error to display
	 */
	public void alertError(Throwable error) {
		alertError(error, "An error of type " + error.getClass().getSimpleName() + " was thrown:");
	}

	/**
	 * Pops up a dialog box displaying the given error message.
	 * @param value the value to display
	 */
	public void alertError(Throwable th, String message) {
		String stackTrace = ExceptionUtils.stackTraceToString(th);
		stackTrace = StringUtils.fitToHeight(stackTrace, 10);
		JOptionPane.showMessageDialog(getWindow(),
				message + "\n"
				+ th.getMessage() + "\n\n" + stackTrace,
				/* title */ "Error", JOptionPane.ERROR_MESSAGE);
	}

	/**
	 * Override this method to indicate what to do on every frame advance
	 * in an animated program after setAnimated(true) is called.
	 */
	public void animation() {
		// empty
	}

	/**
	 * Moves this program's window to the center of the screen.
	 */
	public void center() {
		GuiUtils.centerWindow(getWindow());
	}
	
	/** Required method of DocumentListener interface. */
	public void changedUpdate(DocumentEvent e) {
		// empty
	}

	/*
	 * Override this method to perform any checking of Java compiler flags
	 * needed by this program.
	 */
	protected void checkCompilerFlags() {
		// empty; override me
	}

	/* Private method: checkForAppletMode() */
	/**
	 * Determines whether this program has been invoked as an applet by scanning
	 * the execution stack to see if the string <code>"Applet"</code> appears
	 * in any method name up the calling chain.
	 */
	private boolean checkForAppletMode() {
		StackTraceElement[] stack = new Throwable().getStackTrace();
		for (int i = 1; i < stack.length; i++) {
			if (stack[i].getMethodName().indexOf("Applet") >= 0) return true;
		}
		return false;
	}

	/**
	 * Checks whether the program has been stopped via the killMe method.
	 * Not to be called by students.
	 */
	public void checkKill() {
		if (kill) {
			throw new Error("exit -1");
		}
	}

	/* Private method: circumventFrameSizeBug(frame, size) */
	/**
	 * In some versions of the JDK, calling getInsets on a Frame fails to return
	 * the insets correctly until the frame is validated.  On those systems, it
	 * is impossible to compute the correct frame size in advance.  The workaround
	 * is to check that the size of the content pane is equal to what it was supposed
	 * to be after installing and validating it.  If it is, do nothing to avoid another
	 * validation repaint.  If not, adjust the size of the frame by however much the
	 * content pane is too small.
	 */
	private void circumventFrameSizeBug(Frame frame, Dimension size) {
		Container contentPane = getContentPane();
		Dimension actualSize = contentPane.getSize();
		if (size.equals(actualSize) || actualSize.width == 0 || actualSize.height == 0) return;
		Dimension frameSize = frame.getSize();
		frameSize.width += size.width - actualSize.width;
		frameSize.height += size.height - actualSize.height;
		frame.setSize(frameSize.width, frameSize.height);
		frame.validate();
	}

	/*
	 * Override this method to perform any checking of Java startup settings
	 * needed by this program.
	 */
	protected void checkStartupSettings() {
		// empty; override me
	}

	/** Required method of ContainerListener interface. Does nothing. */
	public void componentAdded(ContainerEvent e) {
		// empty
	}

	/** Required method of ComponentListener interface. Does nothing. */
	public void componentHidden(ComponentEvent e) {
		// empty
	}

	/** Required method of ComponentListener interface. Does nothing. */
	public void componentMoved(ComponentEvent e) {
		// empty
	}

	/** Required method of ContainerListener interface. Does nothing. */
	public void componentRemoved(ContainerEvent e) {
		// empty
	}

	/** Required method of ComponentListener interface. Does nothing. */
	public void componentResized(ComponentEvent e) {
		// empty
	}

	/** Required method of ComponentListener interface. Does nothing. */
	public void componentShown(ComponentEvent e) {
		// empty
	}

	/* Private method: computeProgramBounds() */
	/**
	 * Sets the bounds for this program as specified in the parameters.
	 */
	private Rectangle computeProgramBounds() {
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
		int width = decodeSizeParameter("WIDTH", DEFAULT_WIDTH, size.width);
		int height = decodeSizeParameter("HEIGHT", DEFAULT_HEIGHT, size.height);
		int x = decodeSizeParameter("X", (width >= size.width) ? 0 : DEFAULT_X, size.width);
		int y = decodeSizeParameter("Y", (height >= size.height) ? 0 : DEFAULT_Y, size.height);
		return new Rectangle(x, y, width, height);
	}

	/**
	 * Pops up a dialog box displaying the given message and asking the user to choose Yes or No.
	 * @param value the value to display
	 * @return true if user chose Yes, false if they chose No
	 */
	public boolean confirm(String question) {
		int result = JOptionPane.showConfirmDialog(getWindow(), question, "Confirm", JOptionPane.YES_NO_OPTION);
		return result == JOptionPane.YES_OPTION;
	}

	/* Factory method: createConsole() */
	/**
	 * Creates the console used by the <code>ConsoleProgram</code>.  Subclasses can
	 * override this method to create their own console types.
	 *
	 * @usage IOConsole console = program.createConsole();
	 * @return The console to be used by the program
	 */
	protected IOConsole createConsole() {
		return IOConsole.SYSTEM_CONSOLE;
	}

	/* Factory method: createDialogIO() */
	/**
	 * Creates the dialog used for interaction (primarily by the <code>DialogProgram</code>
	 * class).  Subclasses can override this method to create their own dialog types.
	 *
	 * @usage IODialog dialog = program.createDialogIO();
	 * @return The dialog to be used by the program
	 */
	protected IODialog createDialogIO() {
		return new IODialog(getContentPane());
	}

	/* Factory method: createMenuBar() */
	/**
	 * Creates a menu bar for use with the program.
	 *
	 * @usage ProgramMenuBar menuBar = createMenuBar();
	 * @return A menu bar for use with this <code>Program</code>
	 * @noshow
	 */
	protected ProgramMenuBar createMenuBar() {
		return new ProgramMenuBar(this);
	}

	/* Protected static method: createParameterTable(args) */
	/**
	 * Creates a hash table containing the parameters specified in the
	 * argument list.  Parameters are taken to be any argument that matches
	 * the template
	 *
	 * <p>   <i>name</i><code>=</code><i>value</i>
	 *
	 * All other arguments are collected as a tab-separated string and placed
	 * in an entry under the key <code>"ARGS"</code>.  All named parameters
	 * are converted to lower case to preserve the case-insensitive semantics
	 * of <code>getParameter</code>.
	 *
	 * @usage HashMap<String,String> ht = createParameterTable(args);
	 * @param args The array of strings passed to the application
	 * @return A <code>HashMap</code> containing the parameter bindings
	 */
	protected static Map<String,String> createParameterTable(String[] args) {
		if (args == null) return Collections.emptyMap();
		HashMap<String,String> ht = new HashMap<String,String>();
		String newArgs = "";
		for (int i = 0; i < args.length; i++) {
			String arg = args[i];
			int equals = arg.indexOf('=');
			if (equals > 0) {
				String name = arg.substring(0, equals).toLowerCase();
				String value = arg.substring(equals + 1);
				ht.put(name, value);
			} else {
				if (newArgs.length() > 0) newArgs += '\t';
				newArgs += arg;
			}
		}
		ht.put("ARGS", newArgs);
		return ht;
	}

	/* Factory method: createProgramFrame() */
	/**
	 * Creates the frame containing the program.
	 *
	 * @usage Frame frame = program.createProgramFrame();
	 * @return The newly allocated <code>Frame</code> object
	 */
	protected JFrame createProgramFrame() {
		return new ProgramFrame(getTitle());
	}

	/* Private method: decodeSizeParameter(name, value, max) */
	/**
	 * Decodes a size parameter.
	 *
	 * @usage int size = decodeSizeParameter(name, value, max);
	 * @param name The name of the parameter
	 * @param value The default value if the parameter is unspecified
	 * @param max The maximum value if the parameter is specified as a percentage
	 * @return The integer denoting the size
	 */
	private int decodeSizeParameter(String name, int value, int max) {
		String str = getParameter(name);
		if (str == null) {
			try {
				Class<?> mainClass = getClass();
				Field field = mainClass.getField("APPLICATION_" + name);
				Object obj = field.get(null);
				if (obj instanceof Integer) return ((Integer) obj).intValue();
				if (obj instanceof String) {
					str = (String) obj;
				} else {
					return value;
				}
			} catch (Exception ex) {
				return value;
			}
		}
		if (str.equals("*")) str = "100%";
		if (str.endsWith("%")) {
			int percent = Integer.parseInt(str.substring(0, str.length() - 1));
			return (int) Math.round(percent / 100.0 * max);
		} else {
			return Integer.parseInt(str);
		}
	}

	/* Overridden method: destroy() */
	/**
	 * Called when the program has been told to destroy itself.  The code here
	 * stops the main thread and any animators that have been initiated by this
	 * applet.
	 *
	 * @usage program.destroy();
	 * @noshow
	 */
	public void destroy() {
		Animator.shutdown(this);
		if (appletStarter != null) appletStarter.stop();
	}

	/* Protected method: endHook() */
	/**
	 * Performs class-specific cleanup for the program just after
	 * it finishes.
	 * @noshow
	 */
	protected void endHook() {
		// empty
	}

	/**
	 * Generates an ErrorException with the given exception as its cause.
	 */
	public void error(Exception ex) {
		throw new ErrorException(ex);
	}

	/**
	 * Generates an ErrorException with the given exception and text.
	 */
	public void error(Exception ex, String text) {
		throw new ErrorException(text, ex);
	}

	/**
	 * Generates an ErrorException with the given text as its error message.
	 */
	public void error(String text) {
		throw new ErrorException(text);
	}

	/* Method: exit() */
	/**
	 * Exits from the program.  Subclasses should override this method if they need
	 * to perform any actions before shutting down the program, such as asking the
	 * user to save any unsaved files.  Any clients that do override this method
	 * should call <code>super.exit()</code> at the end of their processing.
	 *
	 * @usage program.exit();
	 */
	public void exit() {
		try {
			if (!StringUtils.isFalsey(System.getProperty(CONFIG_PROPERTY))
					&& !StringUtils.isFalsey(System.getenv(CONFIG_PROPERTY))) {
				saveConfiguration();
			}
		} catch (SecurityException sex) {
			// empty
		}
		
		if (exitOnClose) {
			int nFinalizers = finalizers.size();
			for (int i = 0; i < nFinalizers; i++) {
				Object obj = finalizers.get(i);
				try {
					Class<?> c = obj.getClass();
					Method exit = c.getMethod("exit", new Class[0]);
					exit.invoke(obj, new Object[0]);
				} catch (Exception ex) {
					throw new ErrorException(ex);
				}
			}
			JTFTools.terminateAppletThreads(this);
			if (!appletMode) {
				System.exit(0);
			}
		}
	}

	/**
	 * Returns true if the given file exists within the current working directory.
	 * @param filename name of the file.
	 * @return true if file exists, false if not
	 */
	protected boolean fileExists(String filename) {
		return fileExistsOnDisk(filename) || fileExistsInsideJAR(filename);
	}

	/**
	 * Returns true if the given file exists within the given directory.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return true if file exists, false if not
	 */
	protected boolean fileExists(String directory, String filename) {
		return fileExistsOnDisk(directory, filename) || fileExistsInsideJAR(directory, filename);
	}

	/**
	 * Returns true if the given file exists inside the app's JAR archive within the current working directory.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return true if file exists, false if not
	 */
	protected boolean fileExistsInsideJAR(String filename) {
		return fileExistsInsideJAR(/* directory */ "", filename);
	}

	/**
	 * Returns true if the given file exists inside the app's JAR archive within the given directory.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return true if file exists, false if not
	 */
	protected boolean fileExistsInsideJAR(String directory, String filename) {
		// fallback to using internal class stream (JAR or applet)
		String filepath = "";
		if (directory != null && !directory.isEmpty()) {
			// BUGFIX: use "/" instead of File.separator because JAR URLs must use / even on Windows
			filepath += directory;
			if (!filepath.endsWith("/")) {
				filepath += "/";
			}
		}
		filepath += filename;
		InputStream stream = getClass().getResourceAsStream(filepath);
		if (stream == null) {
			return false;
		} else {
			try {
				stream.close();
			} catch (IOException ioe) {
				// empty
			}
			return true;
		}
	}

	/**
	 * Returns true if the given file exists in the current working directory.
	 * @param filename name of the file.
	 * @return true if file exists, false if not
	 */
	protected boolean fileExistsOnDisk(String filename) {
		return fileExistsOnDisk(/* directory */ "", filename);
	}

	/**
	 * Returns true if the given file exists in the given directory.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return true if file exists, false if not
	 */
	protected boolean fileExistsOnDisk(String directory, String filename) {
		if (isApplet()) {
			return false;
		} else {
			// try reading a real file
			File file = (directory == null || directory.isEmpty()) ? new File(filename) : new File(directory, filename);
			try {
				if (file.exists() && file.isFile()) {
					return true;
				} else {
					file = new File("../" + directory, filename);  // "../simple.txt"
					if (file.exists() && file.isFile()) {
						return true;
					}
				}
			} catch (SecurityException sex) {
				// running as an applet
			}
			return false;
		}
	}

	/**
	 * Returns the size of the given file in bytes in the current working directory.
	 * @param filename name of the file.
	 * @return file size in bytes
	 * @throws IORuntimeException if file is not found or cannot be read
	 */
	protected int fileSize(String filename) {
		if (!isApplet() && fileExistsOnDisk(filename)) {
			return (int) (new File(filename).length());
		} else if (fileExistsInsideJAR(filename)) {
			InputStream input = openFileFromJAR(filename);
			int size = 0;
			try {
				while (input.read() != -1) {
					size++;
				}
			} catch (IOException ioe) {
				throw new IORuntimeException(ioe);
			}
			return size;
		} else {
			throw new IORuntimeException("File not found: " + filename);
		}
	}

	/**
	 * Returns the size of the given file in bytes.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return file size in bytes
	 * @throws IORuntimeException if file is not found or cannot be read
	 */
	protected int fileSize(String directory, String filename) {
		if (!isApplet() && fileExistsOnDisk(directory, filename)) {
			return (int) (new File(directory, filename).length());
		} else if (fileExistsInsideJAR(directory, filename)) {
			InputStream input = openFileFromJAR(directory, filename);
			int size = 0;
			try {
				while (input.read() != -1) {
					size++;
				}
			} catch (IOException ioe) {
				throw new IORuntimeException(ioe);
			}
			return size;
		} else {
			throw new IORuntimeException("File not found: " + filename);
		}
	}
	
	/**
	 * Required empty method of FocusListener interface; does nothing.
	 * Invoked when a component gains the keyboard focus.
	 */
	public void	focusGained(FocusEvent e) {
		// empty; override me
	}
	
	/**
	 * Required empty method of FocusListener interface; does nothing.
	 * Invoked when a component loses the keyboard focus.
	 */
	public void	focusLost(FocusEvent e) {
		// empty; override me
	}

	/**
	 * Returns the delay between frames of animation in milliseconds (default 20).
	 */
	public int getAnimationDelay() {
		return animationDelay;
	}

	/* Protected method: getAppletStub() */
	/**
	 * Retrieves the applet stub.
	 *
	 * @usage AppletStub stub = getAppletStub();
	 * @return The applet stub
	 */
	protected AppletStub getAppletStub() {
		return appletStub;
	}

	/* Protected method: getArgumentArray() */
	/**
	 * Retrieves the array of arguments passed in from the command line, or
	 * <code>null</code> if no arguments are available.
	 *
	 * @usage String[] args = getArgumentArray();
	 * @return The array of command-line arguments
	 * @noshow
	 */
	protected String[] getArgumentArray() {
		if (parameterTable == null) return null;
		StringTokenizer tokenizer = new StringTokenizer(parameterTable.get("ARGS"), "\t", false);
		String[] args = new String[tokenizer.countTokens()];
		for (int i = 0; tokenizer.hasMoreTokens(); i++) {
			args[i] = tokenizer.nextToken();
		}
		return args;
	}

	/**
	 * Reads and returns a boolean value (<code>true</code> or <code>false</code>).
	 * The input must match one of these strings, ignoring case.  If the user
	 * types a value that is not one of these possibilities, the method ordinarily
	 * offers the user a chance to reenter the data, although this behavior
	 * can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage boolean flag = program.getBoolean();
	 * @return The value of the input interpreted as a boolean value
	 */
	public final boolean getBoolean() {
		return readBoolean();
	}

	/**
	 * Prompts the user to enter a boolean value, which is returned as
	 * the value of this method.  If the user types a value that is not a
	 * legal boolean value, the method ordinarily offers the user a chance
	 * to reenter the data, although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage boolean flag = program.readBoolean(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a boolean value
	 */
	public boolean getBoolean(String prompt) {
		return readBoolean(prompt);
	}

	/**
	 * Prompts the user to enter a boolean value, which is matched against the
	 * labels provided.  If the user enters a value that is not one of the two
	 * choices, <code>readBoolean</code> ordinarily offers the user a chance
	 * to reenter the data, although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage boolean flag = program.getBoolean(prompt, trueLabel, falseLabel);
	 * @param prompt The prompt string to display to the user
	 * @param trueLabel The string used to indicate <code>true</code>
	 * @param falseLabel The string used to indicate <code>false</code>
	 * @return The value of the input interpreted as a boolean value
	 */
	public boolean getBoolean(String prompt, String trueLabel, String falseLabel) {
		return readBoolean(prompt, trueLabel, falseLabel);
	}

	/* Protected method: getBorder(side) */
	/**
	 * Returns the component installed as a border on the specified side, which must
	 * be one of the constants from <code>BorderLayout</code> (<code>NORTH</code>,
	 * <code>SOUTH</code>, <code>EAST</code>, <code>WEST</code>).
	 *
	 * @usage getBorder(side, comp);
	 * @param side The side (<code>NORTH</code>, <code>SOUTH</code>, <code>EAST</code>,
	 *             or <code>WEST</code>)
	 * @return The component used as a border on the specified side
	 * @noshow
	 */
	protected Component getBorder(String side) {
		if (side.equals(NORTH)) return northBorder;
		if (side.equals(SOUTH)) return southBorder;
		if (side.equals(EAST)) return eastBorder;
		if (side.equals(WEST)) return westBorder;
		throw new ErrorException("Illegal border specification - " + side);
	}
	
	/**
	 * Returns the y-coordinate of the bottom of this program's window.
	 */
	public int getBottomY() {
		return getY() + getHeight();
	}

	/**
	 * Returns the x/y-coordinates of the center of this program's window.
	 */
	public GDimension getCenterLocation() {
		return new GDimension(getCenterX(), getCenterY());
	}

	/**
	 * Returns the x-coordinate of the center of this program's window.
	 */
	public int getCenterX() {
		return getX() + getWidth() / 2;
	}

	/**
	 * Returns the y-coordinate of the center of this program's window.
	 */
	public int getCenterY() {
		return getY() + getHeight() / 2;
	}

	/* Method: getCentralRegionSize() */
	/**
	 * Returns the size of the central region.  If the content pane has
	 * not been validated, this method computes its preferred size by
	 * subtracting the sizes required for the side panels from the size
	 * of the entire frame.
	 *
	 * @return The size of the central region
	 */
	public Dimension getCentralRegionSize() {
		if (centerPanel == null) return super.getSize();
		if (initFinished) return centerPanel.getSize();
		Dimension size = (programFrame == null) ? super.getSize() : programFrame.getSize();
		Insets insets = (programFrame == null) ? super.getInsets() : programFrame.getInsets();
		size.width -= westPanel.getPreferredSize().width + eastPanel.getPreferredSize().width;
		size.width -= insets.left + insets.right;
		size.height -= northPanel.getPreferredSize().height + southPanel.getPreferredSize().height;
		size.height -= insets.top + insets.bottom;
		return size;
	}

	/* Method: getConsole() */
	/**
	 * Returns the console associated with this program.
	 *
	 * @usage IOConsole console = program.getConsole();
	 * @return The <code>IOConsole</code> object used for this program
	 */
	public IOConsole getConsole() {
		return myConsole;
	}

	/* Method: getDialog() */
	/**
	 * Returns the dialog used for user interaction.
	 *
	 * @usage IODialog dialog = program.getDialog();
	 * @return The <code>IODialog</code> object used for this program
	 */
	public IODialog getDialog() {
		return myDialog;
	}

	/**
	 * Reads and returns a double-precision value from the user.  If the user
	 * types a value that is not a legal number, the method ordinarily offers
	 * the user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.getDouble();
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double getDouble() {
		return readDouble();
	}

	/**
	 * Reads and returns a double-precision value from the user, which is
	 * constrained to be within the specified inclusive range.  If the user
	 * types a value that is not a legal number, the method ordinarily offers
	 * the user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.getDouble(low, high);
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double getDouble(double low, double high) {
		return readDouble(low, high);
	}

	/**
	 * Prompts the user to enter an double-precision number, which is then
	 * returned as the value of this method.  If the user types a value that
	 * is not a legal number, the method ordinarily offers the user a chance to
	 * reenter the data,  although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.getDouble(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public final double getDouble(String prompt) {
		return readDouble(prompt);
	}

	/**
	 * Prompts the user to enter an double-precision number, which is then returned
	 * as the value of this method.  The value must be within the inclusive range
	 * between <code>low</code> and <code>high</code>.  If the user types a value
	 * that is not a legal number, the method ordinarily offers the user a chance
	 * to reenter the data,  although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage d = program.getDouble(prompt, low, high);
	 * @param prompt The prompt string to display to the user
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double getDouble(String prompt, double low, double high) {
		return readDouble(prompt, low, high);
	}

	/* Overridden method: getHeight() */
	/**
	 * Returns the height of the central region.
	 *
	 * @usage int height = getHeight();
	 * @return The height of the central region
	 * @noshow
	 */
	public int getHeight() {
		String caller = getMyCaller();
		if (caller.startsWith("java.") || caller.startsWith("javax.")) {
			return super.getHeight();
		} else {
			return getCentralRegionSize().height;
		}
	}

	/* Method: getInputModel() */
	/**
	 * Returns the <code>IOModel</code> used for program input, which will
	 * ordinarily be the console.
	 *
	 * @usage IOModel io = program.getInputModel();
	 * @return The <code>IOModel</code> used for program input
	 */
	public IOModel getInputModel() {
		return (inputModel == null) ? myConsole : inputModel;
	}

	/**
	 * Reads and returns an integer value from the user.  If the user types
	 * a value that is not a legal integer, the method ordinarily offers the
	 * user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = program.getInteger();
	 * @return The value of the input interpreted as a decimal integer
	 */
	public int getInteger() {
		return readInt();
	}

	/**
	 * Reads and returns an integer value from the user, which is constrained to
	 * be within the specified inclusive range.  If the user types a value
	 * that is not a legal integer, the method ordinarily offers the user a chance
	 * to reenter the data, although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = program.getInteger(low, high);
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a decimal integer
	 */
	public int getInteger(int low, int high) {
		return readInt(low, high);
	}

	/**
	 * Prompts the user to enter an integer, which is then returned as the value
	 * of this method.  If the user types a value that is not a legal integer,
	 * the method ordinarily offers the user a chance to reenter the data,
	 * although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = program.getInteger(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a decimal integer
	 */
	public final int getInteger(String prompt) {
		return readInt(prompt);
	}

	/**
	 * Prompts the user to enter an integer, which is then returned as the value
	 * of this method.  The value must be within the inclusive range between
	 * <code>low</code> and <code>high</code>.  If the user types a value that
	 * is not a legal integer or is outside the specified range, the method
	 * ordinarily offers the user a chance to reenter the data,
	 * although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = console.getInteger(prompt, low, high);
	 * @param prompt The prompt string to display to the user
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a decimal integer
	 */
	public int getInteger(String prompt, int low, int high) {
		return readInt(prompt, low, high);
	}

	/**
	 * Returns the Java Swing window frame that encloses this program.
	 * Will be null until start() has been called on the program.
	 * @return the window frame
	 */
	public JFrame getJFrame() {
		return programFrame;
	}

	/* Overridden method: getLayout() */
	/**
	 * Gets the layout manager for the central region of the content pane.
	 *
	 * @usage LayoutManager layout = setLayout();
	 * @return The active layout manager
	 * @noshow
	 */
	public LayoutManager getLayout() {
		if (isRootPaneCheckingEnabled()) {
			return centerPanel.getLayout();
		} else {
			return super.getLayout();
		}
	}

	/**
	 * Reads and returns a line of input from the console.  The end-of-line
	 * characters that terminate the input are not included in the returned
	 * string.
	 *
	 * @usage String str = program.getLine();
	 * @return The next line of input as a <code>String</code>
	 */
	public String getLine() {
		return readLine();
	}

	/**
	 * Prompts the user for a line of input.  The end-of-line characters
	 * that terminate the input are not included in the returned string.
	 *
	 * @usage String str = program.getLine(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The next line of input as a <code>String</code>
	 */
	public String getLine(String prompt) {
		return readLine(prompt);
	}

	/* Method: getMainThread() */
	/**
	 * Returns the thread that is running the main program.
	 *
	 * @usage Thread mainThread = getMainThread();
	 * @return The thread that is running the main program, if any
	 * @noshow
	 */
	public Thread getMainThread() {
		return (appletStarter == null) ? null : appletStarter.getMainThread();
	}

	/* Method: getMenuBar() */
	/**
	 * Returns the menu bar associated with this program.  Note that this menu bar
	 * cannot be set by clients, although it can be changed initially by overriding
	 * the <code>createMenuBar</code> factory method.
	 *
	 * @usage ProgramMenuBar mbar = getMenuBar();
	 * @return The menu bar in use for this program
	 */
	public ProgramMenuBar getMenuBar() {
		return myMenuBar;
	}

	/* Private method: getMyCaller() */
	/**
	 * Returns the name of the caller of the method that invoked <code>getMyCaller</code>.
	 */
	String getMyCaller() {
		StackTraceElement[] stack = new Throwable().getStackTrace();
		return stack[2].getClassName() + "." + stack[2].getMethodName();
	}

	/**
	 * Returns the current output color used to display output on the console.
	 * If it has never been explicitly set, returns a default of Color.BLACK.
	 */
	public Color getOutputColor() {
		IOModel model = getOutputModel();
		if (model instanceof IOConsole) {
			ConsoleModel cmodel = ((IOConsole) model).getConsoleModel();
			if (cmodel instanceof StandardConsoleModel) {
				StandardConsoleModel smodel = (StandardConsoleModel) cmodel;
				return smodel.getOutputColor();
			}
		}
		return Color.BLACK;
	}

	/* Method: getOutputModel() */
	/**
	 * Returns the <code>IOModel</code> used for program output, which will
	 * ordinarily be the console.
	 *
	 * @usage IOModel io = program.getOutputModel();
	 * @return The <code>IOModel</code> used for program output
	 */
	public IOModel getOutputModel() {
		return (outputModel == null) ? myConsole : outputModel;
	}

	/* Overridden method: getParameter(name) */
	/**
	 * Returns the parameter associated with name.
	 *
	 * @usage String value = getParameter(name);
	 * @param name The name of the parameter
	 * @return The value associated with the parameter, or <code>null</code> if none
	 * @noshow
	 */
	public String getParameter(String name) {
		String value = null;
		if (parameterTable != null) {
			value = parameterTable.get(name.toLowerCase());
		}
		if (value != null) return value;
		return super.getParameter(name);
	}

	/* Protected method: getParameterTable() */
	/**
	 * Retrieves the parameter table.
	 *
	 * @usage ParameterTable ht = getParameterTable();
	 * @return The parameter table
	 */
	protected Map<String, String> getParameterTable() {
		return parameterTable;
	}

	/* Overridden method: getPreferredSize() */
	/**
	 * Returns the preferred size of the content pane.
	 *
	 * @usage Dimension size = getPreferredSize();
	 * @return The preferred size of the content pane
	 * @noshow
	 */
	public Dimension getPreferredSize() {
		return computeProgramBounds().getSize();
	}

	/* Method: getReader() */
	/**
	 * Returns a <code>BufferedReader</code> whose input comes from the console.
	 *
	 * @usage BufferedReader rd = getReader();
	 * @return A <code>Reader</code> for use with this console
	 */
	public BufferedReader getReader() {
		return getConsole().getReader();
	}

	/**
	 * Reads and returns a double-precision value from the user.  If the user
	 * types a value that is not a legal number, the method ordinarily offers
	 * the user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.getReal();
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double getReal() {
		return readDouble();
	}

	/**
	 * Reads and returns a double-precision value from the user, which is
	 * constrained to be within the specified inclusive range.  If the user
	 * types a value that is not a legal number, the method ordinarily offers
	 * the user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.getReal(low, high);
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double getReal(double low, double high) {
		return readDouble(low, high);
	}

	/**
	 * Prompts the user to enter an double-precision number, which is then
	 * returned as the value of this method.  If the user types a value that
	 * is not a legal number, the method ordinarily offers the user a chance to
	 * reenter the data,  although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.getReal(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public final double getReal(String prompt) {
		return readDouble(prompt);
	}
	
	/**
	 * Prompts the user to enter an double-precision number, which is then returned
	 * as the value of this method.  The value must be within the inclusive range
	 * between <code>low</code> and <code>high</code>.  If the user types a value
	 * that is not a legal number, the method ordinarily offers the user a chance
	 * to reenter the data,  although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage d = program.getReal(prompt, low, high);
	 * @param prompt The prompt string to display to the user
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double getReal(String prompt, double low, double high) {
		return readDouble(prompt, low, high);
	}

	/* Method: getRegionPanel(region) */
	/**
	 * Gets the <code>JPanel</code> for the specified region.
	 *
	 * @usage JPanel panel = getRegionPanel(region);
	 * @param region The region of the window (<code>NORTH</code>, <code>SOUTH</code>,
	 *               <code>EAST</code>, <code>WEST</code>, or <code>CENTER</code>)
	 * @return The <code>JPanel</code> for that subregion
	 * @noshow
	 */
	public JPanel getRegionPanel(String region) {
		if (region.equalsIgnoreCase(NORTH)) {
			return northPanel;
		} else if (region.equalsIgnoreCase(SOUTH)) {
			return southPanel;
		} else if (region.equalsIgnoreCase(WEST)) {
			return westPanel;
		} else if (region.equalsIgnoreCase(EAST)) {
			return eastPanel;
		} else if (region.equalsIgnoreCase(CENTER)) {
			return centerPanel;
		} else {
			throw new ErrorException("getRegionPanel: Illegal region " + region);
		}
	}
	
	/**
	 * Returns the x-coordinate of the right edge of this program's window.
	 */
	public int getRightX() {
		return getX() + getWidth();
	}

	/**
	 * Returns the height of the screen in pixels.
	 */
	public int getScreenHeight() {
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
		return size.height;
	}
	
	/**
	 * Returns the width x height of the screen in pixels.
	 */
	public GDimension getScreenSize() {
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
		return new GDimension(size.width, size.height);
	}
	
	/**
	 * Returns the width of the screen in pixels.
	 */
	public int getScreenWidth() {
		Dimension size = Toolkit.getDefaultToolkit().getScreenSize();
		return size.width;
	}

	/* Protected method: getStartupObject() */
	/**
	 * Retrieves the object that was created when this program was started
	 * if that object is something other than a <code>Program</code>.  In
	 * the normal case of running a <code>Program</code> object, this method
	 * will return <code>null</code>.
	 *
	 * @usage Object obj = getStartupObject();
	 * @return The startup object
	 */
	protected Object getStartupObject() {
		return startupObject;
	}

	/* Method: getTitle() */
	/**
	 * Gets the title of this program.
	 *
	 * @usage String title = getTitle();
	 * @return The title in use for this program
	 */
	public String getTitle() {
		return myTitle;
	}

	/* Overridden method: getWidth() */
	/**
	 * Returns the width of the central region.
	 *
	 * @usage int width = getWidth();
	 * @return The width of the central region
	 * @noshow
	 */
	public int getWidth() {
		String caller = getMyCaller();
		if (caller.startsWith("java.") || caller.startsWith("javax.")) {
			return super.getWidth();
		} else {
			return getCentralRegionSize().width;
		}
	}

	/**
	 * Returns the Java Swing window that encloses this program.
	 * @return the window frame
	 */
	public Window getWindow() {
		JFrame frame = getJFrame();
		if (frame != null) {
			return frame;
		} else {
			Component comp = this;
			while (comp != null && !(comp instanceof Window)) {
				comp = comp.getParent();
			}
			return (Window) comp;
		}
	}

	/* Method: getWriter() */
	/**
	 * Returns a <code>PrintWriter</code> whose output is directed to the console.
	 *
	 * @usage PrintWriter wr = getWriter();
	 * @return A <code>PrintWriter</code> for use with this console
	 */
	public PrintWriter getWriter() {
		return getConsole().getWriter();
	}

	/**
	 * Prompts the user to answer a yes/no question and returns true if the user
	 * typed 'yes' (or anything that starts with a 'y', case-insensitively),
	 * false if the user types anything that starts with 'n', or re-prompts if
	 * the user doesn't type a 'y' or 'n' word.
	 * @return true if user types a 'y' word; false if user types an 'n' word
	 */
	public boolean getYesOrNo(String prompt) {
		while (true) {
			String answer = getLine(prompt).trim().toLowerCase();
			if (answer.startsWith("y")) {
				return true;
			} else if (answer.startsWith("n")) {
				return false;
			} else {
				getOutputModel().showErrorMessage(GETYESORNO_DEFAULT_REPROMPT);
			}
		}
	}
	
	/**
	 * Returns true if the component has at least one action listener.  The method
	 * returns false if the Java runtime is too old to determine the answer.
	 */
	private boolean hasActionListener(Component comp) {
		try {
			Method getActionListeners = comp.getClass().getMethod("getActionListeners", new Class[0]);
			ActionListener[] listeners = (ActionListener[]) getActionListeners.invoke(comp, new Object[0]);
			return (listeners.length > 0);
		} catch (Exception ex) {
			return false;
		}
	}

	/**
	 * Returns true if this program has any saved configuration stored on disk.
	 */
	public final boolean hasConfiguration() {
		try {
			String tmpDir = System.getProperty("java.io.tmpdir");
			if (tmpDir != null) {
				File configFile = new File(tmpDir, CONFIG_FILE_NAME);
				return configFile.isFile() && configFile.canRead();
			}
		} catch (Exception ex) {
			// empty
		}
		return false;
	}

	/* Method: init() */
	/**
	 * Specifies the code to be executed as startup time before the
	 * <code>run</code> method is called.  Subclasses can override this
	 * method to perform any initialization code that would ordinarily
	 * be included in an applet <code>init</code> method.  In general,
	 * subclasses will override <code>init</code> in GUI-based programs
	 * where the program simply sets up an initial state and then waits
	 * for events from the user.  The <code>run</code> method is required
	 * for applications in which there needs to be some control thread
	 * while the program runs, as in a typical animation.
	 *
	 * @usage program.init();
	 */
	public void init() {
		// empty
	}

	/* Private method: initApplicationFrame() */
	/**
	 * Creates the program frame and puts the program in it.
	 */
	private void initApplicationFrame() {
		programFrame = createProgramFrame();
		((ProgramAppletStub) appletStub).setFrame(programFrame);
		Container contents = programFrame.getContentPane();
		contents.setLayout(new BorderLayout());
		contents.add(getContentPane(), BorderLayout.CENTER);
		programFrame.addWindowListener(new ProgramWindowListener(this));
		programBounds = computeProgramBounds();
		Insets insets = programFrame.getInsets();
		int frameWidth = programBounds.width + insets.left + insets.right;
		int frameHeight = programBounds.height + insets.top + insets.bottom;
		programFrame.setBounds(programBounds.x, programBounds.y, frameWidth, frameHeight);
	}

	/* Private method: initContentPane(contentPane) */
	/**
	 * Initializes the content pane to contain its five regions.
	 */
	private void initContentPane(Container contentPane) {
		contentPane.setLayout(new ProgramContentPaneLayout(this));
		northPanel = new JPanel();
		northPanel.setOpaque(false);
		southPanel = new JPanel();
		southPanel.setOpaque(false);
		eastPanel = new JPanel();
		eastPanel.setOpaque(false);
		westPanel = new JPanel();
		westPanel.setOpaque(false);
		centerPanel = new JPanel();
		centerPanel.setOpaque(false);
		northPanel.setLayout(new TableLayout(1, 0, 5, 5));
		southPanel.setLayout(new TableLayout(1, 0, 5, 5));
		westPanel.setLayout(new TableLayout(0, 1, 5, 5));
		eastPanel.setLayout(new TableLayout(0, 1, 5, 5));
		centerPanel.setLayout(new GridLayout(1, 0));
		contentPane.add(northPanel, NORTH);
		contentPane.add(southPanel, SOUTH);
		contentPane.add(eastPanel, EAST);
		contentPane.add(westPanel, WEST);
		contentPane.add(centerPanel, CENTER);
	}

	/** Required method of DocumentListener interface. */
	public void insertUpdate(DocumentEvent e) {
		// empty
	}

	/**
	 * Returns true if the program is currently in animated mode.
	 */
	public boolean isAnimated() {
		return animatedMode;
	}

	/**
	 * Returns <code>true</code> if this program is running as an applet in a browser.
	 */
	protected boolean isApplet() {
		try {
			if (java.lang.System.getSecurityManager() != null) {
				// applets run with security managers enabled
				return true;
			} else {
				File currentDir = new File(java.lang.System.getProperty("user.dir"));
				currentDir.exists();  // calling this will trigger security exception if applet
			}
		} catch (SecurityException sex) {
			return true;
		}

		return false;
	}

	/* Method: isAppletMode() */
	/**
	 * Returns <code>true</code> if this program is running as an applet in a browser.
	 *
	 * @usage if (isAppletMode()) . . .
	 * @return <code>true</code> if this program is running as an applet, <code>false</code> otherwise
	 * @noshow
	 */
	public boolean isAppletMode() {
		return appletMode;
	}

	/**
	 * Determines whether the component is a button.
	 */
	private boolean isButton(Component comp) {
		return (comp instanceof Button || comp instanceof JButton);
	}

	/**
	 * Returns whether the program will exit when its window is closed (default true).
	 */
	public boolean isExitOnClose() {
		return this.exitOnClose;
	}

	/**
	 * Returns true if this program's init() method has been called.
	 */
	public boolean isInitialized() {
		return initFinished;
	}
	
	public boolean isRunning() {
		return isStarted() && !runFinished;
	}

	/* Protected method: isStarted() */
	/**
	 * Checks to see whether this program has started, usually by checking to see
	 * whether some pane exists.  Subclasses can override this method to ensure
	 * that their structures are visible before proceeding.
	 * @noshow
	 */
	protected boolean isStarted() {
		IOConsole console = getConsole();
		if (console == null) {
			return false;
		}
		if (console.getParent() == null) {
			return true;
		}
		Dimension size = console.getSize();
		return (console.isShowing()) && (size.width != 0) && (size.height != 0);
	}
	
	/** Required empty method of ItemListener interface. */
	public void	itemStateChanged(ItemEvent e) {
		// empty
	}

	/* Method: keyPressed (implements KeyListener) */
	/**
	 * Called when a key is pressed.
	 */
	public void keyPressed(KeyEvent e) {
		// empty
	}

	/* Method: keyReleased (implements KeyListener) */
	/**
	 * Called when a key is released.
	 */
	public void keyReleased(KeyEvent e) {
		// empty
	}

	/* Method: keyTyped (implements KeyListener) */
	/**
	 * Called when a key is typed (i.e., pressed and released).
	 */
	public void keyTyped(KeyEvent e) {
		// empty
	}

	/**
	 * Instructs the program to stop/kill itself.
	 * Not to be called by students.
	 */
	public void killMe() {
		kill = true;
	}

	/**
	 * Loads any saved configuration of this program from disk.
	 */
	public final void loadConfiguration() {
		File configFile = null;
		try {
			String tmpDir = System.getProperty("java.io.tmpdir");
			if (tmpDir != null) {
				configFile = new File(tmpDir, CONFIG_FILE_NAME);
				if (!configFile.exists()) {
					return;
				}
				Properties props = new Properties();
				FileInputStream input = new FileInputStream(configFile);
				props.load(input);
				input.close();

				loadConfiguration(props);
			}
		} catch (Exception ex) {
			// disabling this error message because some students were seeing it and becoming alarmed
			// System.err.println("error: Could not load C++ lib configuration from file \"" + configFile + "\": " + ex.getMessage());
		}
	}

	/**
	 * Loads any saved configuration of this program from disk.
	 * @param props properties to load from
	 */
	protected void loadConfiguration(Properties props) {
		// empty; override me
	}
	
	/* Method: menuAction(e) */
	/**
	 * Called whenever a program action is detected in the menu bar.
	 * Subclasses can override this method to extend the set of menu
	 * commands recognized even in the absence of a component with
	 * keyboard focus.
	 */
	public boolean menuAction(ActionEvent event) {
		String cmd = String.valueOf(event.getActionCommand()).intern();
		if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_QUIT) {
			exit();
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_SAVE) {
			// try to find a GCanvas in this program; if there is one, save its output
			GCanvas gc = GuiUtils.getDescendent(this, GCanvas.class);
			if (gc == null) {
				return false;
			}
			if (this.lastSaveFile == null) {
				this.lastSaveFile = gc.showSaveDialog();
			} else {
				gc.save(this.lastSaveFile);
			}
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_SAVE_AS) {
			// try to find a GCanvas in this program; if there is one, save its output
			GCanvas gc = GuiUtils.getDescendent(this, GCanvas.class);
			if (gc == null) {
				return false;
			}
			this.lastSaveFile = gc.showSaveDialog();
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_COMPARE_OUTPUT) {
			// try to find a GCanvas in this program; if there is one, compare its output
			GCanvas gc = GuiUtils.getDescendent(this, GCanvas.class);
			if (gc == null) {
				return false;
			}
			gc.showDiffDialog();
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_PRINT
				|| cmd.equals(ProgramMenuBar.MENU_ITEM_TEXT_PRINT + "...")) {
			Frame frame = JTFTools.getEnclosingFrame(this);
			if (frame == null) {
				return true;
			}
			PrintJob pj = frame.getToolkit().getPrintJob(frame, myTitle, null);
			if (pj == null) {
				return true;
			}
			Graphics pg = pj.getGraphics();
			pg.translate(PRINT_MARGIN, PRINT_MARGIN);
			frame.printAll(pg);
			pj.end();
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_EXPORT_APPLET
				|| cmd == ProgramMenuBar.MENU_ITEM_TEXT_SUBMIT_PROJECT) {
			JTFTools.executeExportAction(this, cmd);
			return true;
		} else if (ProgramMenuBar.MENU_ITEM_TEXT_ANTI_ALIASING.equals(cmd)) {
			JCheckBoxMenuItem item = (JCheckBoxMenuItem) event.getSource();
			GObject.setAntiAliasing(item.isSelected());
			for (GCanvas gc : GuiUtils.getDescendents(this, GCanvas.class)) {
				gc.setAntiAliasing(item.isSelected());
			}
		} else if (ProgramMenuBar.MENU_ITEM_TEXT_SHOW_PIXEL_GRID.equals(cmd)) {
			JCheckBoxMenuItem item = (JCheckBoxMenuItem) event.getSource();
			for (GCanvas gc : GuiUtils.getDescendents(this, GCanvas.class)) {
				gc.setShowPixelGrid(item.isSelected());
			}
		} else if (ProgramMenuBar.MENU_ITEM_TEXT_SHOW_PIXEL_INFO.equals(cmd)) {
			JCheckBoxMenuItem item = (JCheckBoxMenuItem) event.getSource();
			for (GCanvas gc : GuiUtils.getDescendents(this, GCanvas.class)) {
				gc.setShowPixelInfo(item.isSelected());
			}
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_ABOUT) {
			JOptionPane.showMessageDialog(
					getConsole(),                        // parent component
					Version.getAboutMessage(),           // message
					"About Stanford Library",            // title
					JOptionPane.INFORMATION_MESSAGE      // type
					);

		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_CHECK_FOR_UPDATES) {
			stanford.spl.LibraryUpdater updater = new stanford.spl.LibraryUpdater();
			updater.checkForUpdates(getJFrame());
		}

		// IOConsole handles other menu items like Cut/Copy/Paste, clear, etc.
		return getConsole().menuAction(event);
	}

	/* Method: mouseClicked (implements MouseListener) */
	/**
	 * Called when the mouse is clicked.  A call to <code>mouseClicked</code>
	 * is always preceded by both a <code>mousePressed</code> and a
	 * <code>mouseReleased</code> event for the same source.
	 */
	public void mouseClicked(MouseEvent e) {
		// empty
	}

	/* Method: mouseDragged (implements MouseMotionListener) */
	/**
	 * Called when the mouse is dragged with the button down.  Java
	 * makes several guarantees about dragging.  First, a
	 * <code>mouseDragged</code> call is always preceded by a
	 * <code>mousePressed</code> call for the same source.  If the
	 * mouse is pressed elsewhere and then enters a source with
	 * the button down, no drag event occurs.  Moreover, once the
	 * mouse button goes down in a particular source, only that
	 * source will receive mouse events until the button goes up.
	 * Those events, moreover, are reported even in the mouse
	 * travels outside the domain of the object.
	 */
	public void mouseDragged(MouseEvent e) {
		// empty
	}

	/* Method: mouseEntered (implements MouseListener) */
	/**
	 * Called when the mouse enters the source (which may be
	 * either a component or a <code>GObject</code>).
	 */
	public void mouseEntered(MouseEvent e) {
		// empty
	}

	/* Method: mouseExited (implements MouseListener) */
	/**
	 * Called when the mouse exits the source (which may be
	 * either a component or a <code>GObject</code>).
	 */
	public void mouseExited(MouseEvent e) {
		// empty
	}

	/* Method: mouseMoved (implements MouseMotionListener) */
	/**
	 * Called when the mouse is moved.
	 */
	public void mouseMoved(MouseEvent e) {
		// empty
	}

	/* Method: mousePressed (implements MouseListener) */
	/**
	 * Called when the mouse button is pressed.
	 */
	public void mousePressed(MouseEvent e) {
		// empty
	}

	/* Method: mouseReleased (implements MouseListener) */
	/**
	 * Called when the mouse button is released.
	 */
	public void mouseReleased(MouseEvent e) {
		// empty
	}

	/**
	 * Implementation of MouseWheelListener interface.
	 * This method does nothing but can be overridden.
	 */
	public void mouseWheelMoved(MouseWheelEvent e) {
		// empty
	}

	/**
	 * Opens the given file for reading.
	 * @param filename name of the file.
	 * @return stream to read file
	 * @throws IORuntimeException if file is not found
	 */
	protected InputStream openFile(String filename) {
		return openFile(/* directory */ "", filename);
	}

	/**
	 * Opens the given file from within the given directory.
	 * @param filename name of the file.
	 * @return stream to read file
	 * @throws IORuntimeException if file is not found
	 */
	protected InputStream openFile(String directory, String filename) {
		if (fileExistsOnDisk(directory, filename)) {
			return openFileFromDisk(directory, filename);
		} else if (fileExistsInsideJAR(directory, filename)) {
			return openFileFromJAR(directory, filename);
		} else {
			throw new IORuntimeException("File not found: " + filename);
		}
	}

	/**
	 * Opens the given file from within the current working directory.
	 * @param filename name of the file.
	 * @return stream to read file
	 * @throws IORuntimeException if file is not found
	 */
	protected InputStream openFileFromDisk(String filename) {
		return openFileFromDisk(/* directory */ "", filename);
	}

	/**
	 * Opens the given file from within the given directory.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return stream to read file
	 * @throws IORuntimeException if file is not found
	 */
	protected InputStream openFileFromDisk(String directory, String filename) {
		// try reading a real file first
		File file = (directory == null || directory.isEmpty()) ? new File(filename) : new File(directory, filename);
		try {
			if (file.exists()) {
				return new BufferedInputStream(new FileInputStream(file));
			} else {
				file = new File("../" + directory, filename);  // "../simple.txt"
				if (file.exists()) {
					return new BufferedInputStream(new FileInputStream(file));
				}
			}
		} catch (SecurityException sex) {
			// running as an applet
		} catch (IOException sex) {
			// file not found
		}
		throw new IORuntimeException("File not found: " + filename);
	}

	/**
	 * Opens the given file from inside the app's JAR archive.
	 * @param filename name of the file.
	 * @return stream to read file
	 * @throws IORuntimeException if file is not found
	 */
	protected InputStream openFileFromJAR(String filename) {
		return openFileFromJAR(/* directory */ "", filename);
	}

	/**
	 * Opens the given file from within the app's JAR archive in the given directory.
	 * @param directory directory to search for the file.
	 * @param filename name of the file.
	 * @return stream to read file
	 * @throws IORuntimeException if file is not found
	 */
	protected InputStream openFileFromJAR(String directory, String filename) {
		// fallback to using internal class stream (JAR or applet)
		String filepath = "";
		if (directory != null && !directory.isEmpty()) {
			// BUGFIX: use "/" instead of File.separator because JAR URLs must use / even on Windows
			filepath += directory;
			if (!filepath.endsWith("/")) {
				filepath += "/";
			}
		}
		filepath += filename;
		InputStream stream = getClass().getResourceAsStream(filepath);
		if (stream == null) {
			throw new IORuntimeException("File not found: " + filepath);
		} else {
			return new BufferedInputStream(stream);
		}
	}

	/**
	 * Resizes this program to exactly fit the preferred size of its contents.
	 */
	public void pack() {
		this.getWindow().pack();
	}

	/* Method: pause(milliseconds) */
	/**
	 * Delays the calling thread for the specified time, which is expressed in
	 * milliseconds.  Unlike <code>Thread.sleep</code>, this method never throws an
	 * exception.
	 *
	 * @usage program.pause(milliseconds);
	 * @param milliseconds The sleep time in milliseconds
	 */
	public void pause(double milliseconds) {
		// JTFTools.pause(milliseconds);

		checkKill();
		if (pauseScaleFactor == 0.0) {
			// pause indefinitely
			while (pauseScaleFactor == 0.0) {
				// if tick flag is set, break out;
				// but on next pause() call, will pause indefinitely again
				if (pauseTickFlag) {
					pauseTickFlag = false;
					break;
				}
				JTFTools.pause(250);
			}
		} else {
			JTFTools.pause((int) (milliseconds * pauseScaleFactor));
		}
	}

	/**
	 * Sets a 'pause tick' flag.
	 * Not to be called by students.
	 */
	public void pauseTick() {
		pauseTickFlag = true;
	}
	
	/**
	 * Makes sure that <code>print</code> can display a <code>boolean</code>.
	 * @noshow
	 */
	public final void print(boolean x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(boolean value, Color color) {
		print(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>print</code> can display a <code>char</code>.
	 * @noshow
	 */
	public final void print(char x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(char value, Color color) {
		print(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>print</code> can display a <code>double</code>.
	 * @noshow
	 */
	public final void print(double x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(double value, Color color) {
		print(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>print</code> can display a <code>float</code>.
	 * @noshow
	 */
	public final void print(float x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(float value, Color color) {
		print(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>print</code> can display an <code>int</code>.
	 * @noshow
	 */
	public final void print(int x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(int value, Color color) {
		print(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>print</code> can display a <code>long</code>.
	 * @noshow
	 */
	public final void print(long x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(long value, Color color) {
		print(String.valueOf(value), color);
	}
	
	/**
	 * Makes sure that <code>print</code> can display an <code>Object</code>.
	 * @noshow
	 */
	public final void print(Object x) {
		print(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void print(Object value, Color color) {
		print(String.valueOf(value), color);
	}

	/* Method: print(value) */
	/**
	 * Displays the argument value on the console, leaving the cursor at the end of
	 * the output.  The <code>print</code> method is overloaded so that
	 * <code>value</code> can be of any type.
	 *
	 * @usage program.print(value);
	 * @param value The value to be displayed
	 */
	public void print(String value) {
		getOutputModel().print(value);
	}

	/**
	 * Prints the given string in the given color.
	 * @param value The string to print.
	 * @param color The color in which to draw the text. If null, draws in black.
	 */
	public void print(String value, Color color) {
		Color old = getOutputColor();
		setOutputColor(color);
		getOutputModel().print(value);
		setOutputColor(old);
	}

	/**
	 * Writes a formatted string to this output stream using the specified format string and arguments.
	 * @param format A format string as described in Java's Format string syntax.
	 * @param args Arguments referenced by the format specifiers in the format string. If there are more arguments than format specifiers, the extra arguments are ignored. The number of arguments is variable and may be zero. The maximum number of arguments is limited by the maximum dimension of a Java array as defined by The Java™ Virtual Machine Specification. The behavior on a null argument depends on the conversion.
	 * @throws IllegalFormatException If a format string contains an illegal syntax, a format specifier that is incompatible with the given arguments, insufficient arguments given the format string, or other illegal conditions.
	 * @throws NullPointerException If the format is null
	 */
	public void printf(String format, Object... args) {
		print(String.format(format, args));
	}

	/**
	 * Writes a formatted string to this output stream using the specified format string and arguments.
	 * @param format A format string as described in Java's Format string syntax.
	 * @param args Arguments referenced by the format specifiers in the format string. If there are more arguments than format specifiers, the extra arguments are ignored. The number of arguments is variable and may be zero. The maximum number of arguments is limited by the maximum dimension of a Java array as defined by The Java™ Virtual Machine Specification. The behavior on a null argument depends on the conversion.
	 * @throws IllegalFormatException If a format string contains an illegal syntax, a format specifier that is incompatible with the given arguments, insufficient arguments given the format string, or other illegal conditions.
	 * @throws NullPointerException If the format is null
	 */
	// public void printf(String format, Color color, Object... args) {
	// 	println(String.format(format, args), color);
	// }

	/* Method: println() */
	/**
	 * Advances the console cursor to the beginning of the next line.
	 *
	 * @usage program.println();
	 */
	public void println() {
		getOutputModel().println();
	}

	/**
	 * Makes sure that <code>println</code> can display a <code>boolean</code>.
	 * @noshow
	 */
	public final void println(boolean x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(boolean value, Color color) {
		println(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>println</code> can display a <code>char</code>.
	 * @noshow
	 */
	public final void println(char x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(char value, Color color) {
		println(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>println</code> can display a <code>double</code>.
	 * @noshow
	 */
	public final void println(double x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(double value, Color color) {
		println(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>println</code> can display a <code>float</code>.
	 * @noshow
	 */
	public final void println(float x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(float value, Color color) {
		println(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>println</code> can display an <code>int</code>.
	 * @noshow
	 */
	public final void println(int x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(int value, Color color) {
		println(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>println</code> can display a <code>long</code>.
	 * @noshow
	 */
	public final void println(long x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(long value, Color color) {
		println(String.valueOf(value), color);
	}

	/**
	 * Makes sure that <code>println</code> can display an <code>Object</code>.
	 * @noshow
	 */
	public final void println(Object x) {
		println(String.valueOf(x));
	}

	/**
	 * Prints the given value in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The value to print.
	 * @param color The color in which to draw the value. If null, draws in black.
	 */
	public void println(Object value, Color color) {
		println(String.valueOf(value), color);
	}

	/* Method: println(value) */
	/**
	 * Displays the argument value on the console and then advances the cursor
	 * to the beginning of the next line.  The <code>println</code> method is
	 * overloaded so that <code>value</code> can be of any type.
	 *
	 * @usage program.println(value);
	 * @param value The value to be displayed
	 */
	public void println(String value) {
		getOutputModel().println(value);
	}

	/**
	 * Prints the given string in the given color and then advances the cursor to the beginning of the next line.
	 * @param value The string to print.
	 * @param color The color in which to draw the text. If null, draws in black.
	 */
	public void println(String value, Color color) {
		Color old = getOutputColor();
		setOutputColor(color);
		getOutputModel().println(value);
		setOutputColor(old);
	}

	/**
	 * Pops up a dialog box displaying the given message and asking the user to type a response.
	 * @param value the value to display
	 * @return the text that the user typed, or null if the user presses Cancel
	 */
	public String prompt(String prompt) {
		String result = JOptionPane.showInputDialog(getWindow(), prompt);
		return result;
	}

	/* Method: readBoolean() */
	/**
	 * Reads and returns a boolean value (<code>true</code> or <code>false</code>).
	 * The input must match one of these strings, ignoring case.  If the user
	 * types a value that is not one of these possibilities, the method ordinarily
	 * offers the user a chance to reenter the data, although this behavior
	 * can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage boolean flag = program.readBoolean();
	 * @return The value of the input interpreted as a boolean value
	 */
	public final boolean readBoolean() {
		return readBoolean(/* prompt */ null);
	}

	/* Method: readBoolean(prompt) */
	/**
	 * Prompts the user to enter a boolean value, which is returned as
	 * the value of this method.  If the user types a value that is not a
	 * legal boolean value, the method ordinarily offers the user a chance
	 * to reenter the data, although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage boolean flag = program.readBoolean(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a boolean value
	 */
	public final boolean readBoolean(String prompt) {
		return readBoolean(prompt, /* trueLabel */ "true", /* falseLabel */ "false");
	}

	/* Method: readBoolean(prompt, trueLabel, falseLabel) */
	/**
	 * Prompts the user to enter a boolean value, which is matched against the
	 * labels provided.  If the user enters a value that is not one of the two
	 * choices, <code>readBoolean</code> ordinarily offers the user a chance
	 * to reenter the data, although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage boolean flag = program.readBoolean(prompt, trueLabel, falseLabel);
	 * @param prompt The prompt string to display to the user
	 * @param trueLabel The string used to indicate <code>true</code>
	 * @param falseLabel The string used to indicate <code>false</code>
	 * @return The value of the input interpreted as a boolean value
	 */
	public boolean readBoolean(String prompt, String trueLabel, String falseLabel) {
		return getInputModel().readBoolean(prompt, trueLabel, falseLabel);
	}

	/* Method: readDouble() */
	/**
	 * Reads and returns a double-precision value from the user.  If the user
	 * types a value that is not a legal number, the method ordinarily offers
	 * the user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.readDouble();
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public final double readDouble() {
		return readDouble(/* prompt */ null, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
	}

	/* Method: readDouble(low, high) */
	/**
	 * Reads and returns a double-precision value from the user, which is
	 * constrained to be within the specified inclusive range.  If the user
	 * types a value that is not a legal number, the method ordinarily offers
	 * the user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.readDouble(low, high);
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public final double readDouble(double low, double high) {
		return readDouble(/* prompt */ null, low, high);
	}

	/* Method: readDouble(prompt) */
	/**
	 * Prompts the user to enter an double-precision number, which is then
	 * returned as the value of this method.  If the user types a value that
	 * is not a legal number, the method ordinarily offers the user a chance to
	 * reenter the data,  although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage double d = program.readDouble(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public final double readDouble(String prompt) {
		return readDouble(prompt, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
	}

	/* Method: readDouble(prompt, low, high) */
	/**
	 * Prompts the user to enter an double-precision number, which is then returned
	 * as the value of this method.  The value must be within the inclusive range
	 * between <code>low</code> and <code>high</code>.  If the user types a value
	 * that is not a legal number, the method ordinarily offers the user a chance
	 * to reenter the data,  although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage d = program.readDouble(prompt, low, high);
	 * @param prompt The prompt string to display to the user
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a <code>double</code>
	 */
	public double readDouble(String prompt, double low, double high) {
		return getInputModel().readDouble(prompt, low, high);
	}

	/* Method: readInt() */
	/**
	 * Reads and returns an integer value from the user.  If the user types
	 * a value that is not a legal integer, the method ordinarily offers the
	 * user a chance to reenter the data, although this behavior can be
	 * changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = program.readInt();
	 * @return The value of the input interpreted as a decimal integer
	 */
	public final int readInt() {
		return readInt(/* prompt */ null, Integer.MIN_VALUE, Integer.MAX_VALUE);
	}

	/* Method: readInt(low, high) */
	/**
	 * Reads and returns an integer value from the user, which is constrained to
	 * be within the specified inclusive range.  If the user types a value
	 * that is not a legal integer, the method ordinarily offers the user a chance
	 * to reenter the data, although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = program.readInt(low, high);
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a decimal integer
	 */
	public final int readInt(int low, int high) {
		return readInt(/* prompt */ null, low, high);
	}

	/* Method: readInt(prompt) */
	/**
	 * Prompts the user to enter an integer, which is then returned as the value
	 * of this method.  If the user types a value that is not a legal integer,
	 * the method ordinarily offers the user a chance to reenter the data,
	 * although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = program.readInt(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The value of the input interpreted as a decimal integer
	 */
	public final int readInt(String prompt) {
		return readInt(prompt, Integer.MIN_VALUE, Integer.MAX_VALUE);
	}

	/* Method: readInt(prompt, low, high) */
	/**
	 * Prompts the user to enter an integer, which is then returned as the value
	 * of this method.  The value must be within the inclusive range between
	 * <code>low</code> and <code>high</code>.  If the user types a value that
	 * is not a legal integer or is outside the specified range, the method
	 * ordinarily offers the user a chance to reenter the data,
	 * although this behavior can be changed using the
	 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
	 *
	 * @usage int n = console.readInt(prompt, low, high);
	 * @param prompt The prompt string to display to the user
	 * @param low The lowest value in the permitted range
	 * @param high The highest value in the permitted range
	 * @return The value of the input interpreted as a decimal integer
	 */
	public int readInt(String prompt, int low, int high) {
		return getInputModel().readInt(prompt, low, high);
	}

	/* Method: readLine() */
	/**
	 * Reads and returns a line of input from the console.  The end-of-line
	 * characters that terminate the input are not included in the returned
	 * string.
	 *
	 * @usage String str = program.readLine();
	 * @return The next line of input as a <code>String</code>
	 */
	public final String readLine() {
		return readLine(/* prompt */ null);
	}

	/* Method: readLine(prompt) */
	/**
	 * Prompts the user for a line of input.  The end-of-line characters
	 * that terminate the input are not included in the returned string.
	 *
	 * @usage String str = program.readLine(prompt);
	 * @param prompt The prompt string to display to the user
	 * @return The next line of input as a <code>String</code>
	 */
	public String readLine(String prompt) {
		return getInputModel().readLine(prompt);
	}
	
	/* Overridden method: remove(index) */
	/**
	 * Removes the component at the specified index from the central region.
	 *
	 * @usage remove(index);
	 * @param index The index position of the component to remove
	 * @noshow
	 */
	public void remove(int index) {
		if (isRootPaneCheckingEnabled()) {
			centerPanel.remove(index);
		} else {
			super.remove(index);
		}
	}

	/* Overridden method: remove(comp) */
	/**
	 * Removes the specified component from the central region.
	 *
	 * @usage remove(comp);
	 * @param comp The component to remove
	 * @noshow
	 */
	public void remove(Component comp) {
		if (isRootPaneCheckingEnabled()) {
			centerPanel.remove(comp);
		} else {
			super.remove(comp);
		}
		// invisibleObjects.remove(comp);
	}

	/* Overridden method: removeAll() */
	/**
	 * Removes all components from the central region.
	 *
	 * @usage removeAll();
	 * @noshow
	 */
	public void removeAll() {
		if (isRootPaneCheckingEnabled()) {
			centerPanel.removeAll();
		} else {
			super.removeAll();
		}
	}

	/**
	 * Removes the specified component from the specified region of this program window.
	 */
	public void removeFromRegion(Component comp, String region) {
		JPanel panel = null;
		if (region.equalsIgnoreCase("NORTH")) {
			panel = this.northPanel;
		} else if (region.equalsIgnoreCase("EAST")) {
			panel = this.eastPanel;
		} else if (region.equalsIgnoreCase("SOUTH")) {
			panel = this.southPanel;
		} else if (region.equalsIgnoreCase("WEST")) {
			panel = this.westPanel;
		} else if (region.equalsIgnoreCase("CENTER")) {
			panel = this.centerPanel;
		}
		if (panel != null) {
			panel.remove(comp);
			panel.validate();
			validate();
		}
	}

	/**
	 * Removes the JMenuBar at the top of the program window.
	 */
	public void removeMenuBar() {
		getJFrame().getJMenuBar().setEnabled(false);
		setJMenuBar(null);
		getJFrame().setJMenuBar(null);
		getJFrame().validate();
	}

	/** Required method of DocumentListener interface. */
	public void removeUpdate(DocumentEvent e) {
		// empty
	}

	/* Overridden method: repaint() */
	/**
	 * Forwards repaint to the content pane.
	 *
	 * @usage repaint();
	 * @noshow
	 */
	public void repaint() {
		if (isRootPaneCheckingEnabled()) getContentPane().repaint();
		super.repaint();
	}

	/* Method: run() */
	/**
	 * Specifies the code to be executed as the program runs.
	 * The <code>run</code> method is required for applications that have
	 * a thread of control that runs even in the absence of user actions,
	 * such as a program that uses console interation or that involves
	 * animation.  GUI-based programs that operate by setting up an initial
	 * configuration and then wait for user events usually do not specify a
	 * <code>run</code> method and supply a new definition for <code>init</code>
	 * instead.
	 */
	public void run() {
		// empty; override me
	}

	/* Protected method: runHook() */
	/**
	 * Calls the run method in the program.  Subclasses can override this
	 * method to transfer control somewhere else.
	 */
	protected void runHook() {
		run();
	}

	/**
	 * Saves configuration of this program to disk.
	 */
	public final void saveConfiguration() {
		File configFile = null;
		try {
			String tmpDir = System.getProperty("java.io.tmpdir");
			if (tmpDir != null) {
				configFile = new File(tmpDir, CONFIG_FILE_NAME);
				Properties props = new Properties();
				saveConfiguration(props);
				FileOutputStream out = new FileOutputStream(configFile);
				props.store(out, "spl.jar configuration file");
				out.close();
			}
		} catch (Exception ex) {
			// disabling this error message because some students were seeing it and becoming alarmed
			// System.err.println("error: Could not save C++ lib configuration to file \"" + configFile + "\": " + ex.getMessage());
		}
	}

	/**
	 * Saves configuration of this program to disk in the given properties collection.
	 */
	protected void saveConfiguration(Properties props) {
		// empty; override me
	}

	/**
	 * Sets the program to be in animated (true) or non-animated (false) mode.
	 */
	public synchronized void setAnimated(boolean value) {
		animatedMode = value;
		if (animatedMode) {
			// start timer, if not already started
			if (animationTimer == null) {
				animationTimer = new javax.swing.Timer(animationDelay, new ActionListener() {
					public void actionPerformed(ActionEvent event) {
						animation();
					}
				});
				animationTimer.start();
			}
		} else {
			// stop timer, if already started
			if (animationTimer != null) {
				animationTimer.stop();
				animationTimer = null;
			}
		}
	}

	/**
	 * Sets the number of milliseconds to delay between frames of animation.
	 * If animation is already in progress, the delay change will take effect immediately.
	 * @param delayMS delay in milliseconds (must be a positive integer)
	 */
	public synchronized void setAnimationDelay(int delayMS) {
		if (delayMS <= 0) {
			throw new IllegalArgumentException("invalid delay of " + delayMS + "; must be a positive integer");
		}
		if (delayMS != animationDelay) {
			animationDelay = delayMS;
			if (animationTimer != null) {
				animationTimer.setDelay(delayMS);
			}
		}
	}

	/* Protected method: setAppletStub(stub) */
	/**
	 * Sets the applet stub for this program in a way that makes it possible for
	 * clients to retrieve it.
	 *
	 * @usage setAppletStub(stub);
	 * @param stub The applet stub
	 */
	protected void setAppletStub(AppletStub stub) {
		appletStub = stub;
		setStub(stub);
	}

	/* Overridden method: setBackground(color) */
	/**
	 * Sets the background for the central region of the content pane.
	 *
	 * @usage setBackground(color);
	 * @param color The new background color
	 * @noshow
	 */
	public void setBackground(Color color) {
		if (isRootPaneCheckingEnabled()) {
			northPanel.setBackground(color);
			southPanel.setBackground(color);
			westPanel.setBackground(color);
			eastPanel.setBackground(color);
			centerPanel.setBackground(color);
			getContentPane().setBackground(color);
		}
		super.setBackground(color);
	}

	/**
	 * Sets the y location of the bottom of this program.
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setBottomY(double y) {
		setLocation(getX(), y - getHeight());
	}

	/**
	 * Sets the x location of the horizontal center of this program.
	 * @param x the x location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setCenterX(double x) {
		setLocation(x - getWidth() / 2, getY());
	}

	/**
	 * Sets the y location of the vertical center of this program.
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setCenterY(double y) {
		setLocation(getX(), y - getHeight() / 2);
	}

	/**
	 * Sets the y location of the horizontal and vertical center of this program.
	 * @param x the x location as a pixel offset (will be rounded down to nearest integer)
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setCenterLocation(double x, double y) {
		setLocation(x - getWidth() / 2, y - getHeight() / 2);
	}

	/* Method: setConsole(console) */
	/**
	 * Sets the console associated with this program.
	 *
	 * @usage program.setConsole(console);
	 * @param console The <code>IOConsole</code> object used for this program
	 */
	public void setConsole(IOConsole console) {
		myConsole = console;
	}

	/* Method: setDialog(dialog) */
	/**
	 * Sets the dialog associated with this program.
	 *
	 * @usage program.setDialog(dialog);
	 * @param dialog The <code>IODialog</code> object used for this program
	 */
	public void setDialog(IODialog dialog) {
		myDialog = dialog;
	}

	/**
	 * Sets whether the program will exit when its window is closed (default true).
	 */
	public void setExitOnClose(boolean exitOnClose) {
		this.exitOnClose = exitOnClose;
	}

	/**
	 * Sets the animation delay properly to produce the given number of frames
	 * per second of animation.
	 * Equivalent to calling setAnimationDelay(1000 / fps).
	 * Special case: If 0 fps is passed, stops the animation.
	 * Note that calling this method does not start the animation running.
	 * If animation is already in progress, the FPS change will take effect immediately.
	 * @param fps frames per second (must be a non-negative integer)
	 */
	public void setFramesPerSecond(int fps) {
		if (fps < 0) {
			throw new IllegalArgumentException("invalid FPS of " + fps + "; must be non-negative");
		} else if (fps == 0) {
			stopAnimation();
		} else {
			setAnimationDelay(Math.max(1, 1000 / fps));
		}
	}

	/**
	 * Sets the height of this program without changing its width.
	 * @param height the new height, in pixels (will be rounded down to nearest integer)
	 */
	public void setHeight(double height) {
		setSize(getWidth(), height);
	}

	/* Method: setInputModel(io) */
	/**
	 * Sets the input model associated with this program.
	 *
	 * @usage program.setInputModel(io);
	 * @param io The input model used for this program
	 */
	public void setInputModel(IOModel io) {
		inputModel = io;
	}

	/**
	 * Marks the given GObject as being 'invisible'.
	 * Not to be called by students.
	 */
	public void setInvisible(GObject obj, boolean invisible) {
		if (invisible) {
			invisibleObjects.add(obj);
		} else {
			invisibleObjects.remove(obj);
		}
	}

	/**
	 * Sets the Java Swing window frame that encloses this program.
	 * @param jframe
	 */
	public void setJFrame(JFrame jframe) {
		programFrame = jframe;
	}

	/**
	 * Sets the JMenuBar at the top of the program window to be the given menu bar.
	 * If the menu bar passed is null, removes the menu bar.
	 */
	public void setJMenuBar(JMenuBar bar) {
		super.setJMenuBar(bar);

		Window window = programFrame == null ? getWindow() : programFrame;
		if (window != null && window instanceof JFrame) {
			((JFrame) window).setJMenuBar(null);
		}

		if (myConsole != null && (bar == null || bar instanceof ProgramMenuBar)) {
			myConsole.setMenuBar((ProgramMenuBar) bar);
		}
	}

	/* Overridden method: setLayout(layout) */
	/**
	 * Sets the layout manager for the central region of the content pane.
	 *
	 * @usage setLayout(layout);
	 * @param layout The layout manager to use
	 * @noshow
	 */
	public void setLayout(LayoutManager layout) {
		if (isRootPaneCheckingEnabled()) {
			centerPanel.setLayout(layout);
		} else {
			super.setLayout(layout);
		}
	}

	/**
	 * Sets the (x, y) location of the top-left corner of this program.
	 * @param x the x location as a pixel offset (will be rounded down to nearest integer)
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setLocation(double x, double y) {
		getWindow().setLocation((int) x, (int) y);
	}

	/**
	 * Sets the (x, y) location of the top-left corner of this program.
	 * @param x the x location as a pixel offset (will be rounded down to nearest integer)
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setLocation(GPoint point) {
		getWindow().setLocation((int) point.getX(), (int) point.getY());
	}

	/**
	 * Sets the current output color used to display output on the console.
	 * If null is passed, uses a default of Color.BLACK.
	 */
	public void setOutputColor(Color color) {
		IOModel model = getOutputModel();
		if (model instanceof IOConsole) {
			ConsoleModel cmodel = ((IOConsole) model).getConsoleModel();
			if (cmodel instanceof StandardConsoleModel) {
				StandardConsoleModel smodel = (StandardConsoleModel) cmodel;
				smodel.setOutputColor(color);
			}
		}
	}

	/* Method: setOutputModel(io) */
	/**
	 * Sets the output model associated with this program.
	 *
	 * @usage program.setOutputModel(io);
	 * @param io The <code>IOModel</code> object used as the output model
	 */
	public void setOutputModel(IOModel io) {
		outputModel = io;
	}

	/* Method: setParameter(name, value) */
	/**
	 * Sets a new value for the named parameter.
	 *
	 * @usage setParameter(name, value);
	 * @param name The name of the parameter
	 * @param value The new value
	 * @noshow
	 */
	public void setParameter(String name, String value) {
		if (parameterTable == null) {
			parameterTable = new HashMap<String,String>();
		}
		parameterTable.put(name.toLowerCase(), value);
	}

	/* Protected method: setParameterTable(ht) */
	/**
	 * Sets the parameter table for this program.
	 *
	 * @usage setParameterTable(ht);
	 * @param ht The parameter table
	 */
	public void setParameterTable(Map<String, String> ht) {
		parameterTable = ht;
	}

	/**
	 * Applies a scaling factor to the duration of pause() calls.
	 * Not to be called by students.
	 */
	public void setPauseScaleFactor(double factor) {
		double oldFactor = pauseScaleFactor;
		pauseScaleFactor = factor;
		if (oldFactor == 0.0 && factor != 0.0) {
			// time is going from 'stopped' to 'not stopped' state;
			// wake up the program if needed
		}
	}

	/**
	 * Sets the title of this program.  The title appears in the title bar
	 * when the program is running as an application.
	 *
	 * @usage setTitle(title);
	 * @param title The title for this program
	 */
	public void setResizable(boolean resizable) {
		if (programFrame != null) {
			programFrame.setResizable(resizable);
		}
	}

	/**
	 * Sets the x location of the right edge of this program.
	 * @param x the x location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setRightX(double x) {
		setLocation(x - getWidth(), getY());
	}
	
	/**
	 * Sets whether the program is currently running.
	 * Not to be called by clients.
	 */
	public void setRunning(boolean running) {
		this.runFinished = !running;
	}

	/**
	 * Sets whether to display a grid of horizontal and vertical lines on this canvas
	 * for debugging, if this program contains a graphical canvas.
	 */
	public void setShowPixelGrid(boolean show) {
		for (GCanvas gc : GuiUtils.getDescendents(this, GCanvas.class)) {
			gc.setShowPixelGrid(show);
		}
	}

	/**
	 * Sets whether to display information on this canvas about the current pixel where
	 * the mouse pointer is resting in this program's graphical canvas, if this program
	 * contains a graphical canvas.
	 * Displays the current pixel's (x, y) coordinate and color.
	 */
	public void setShowPixelInfo(boolean show) {
		for (GCanvas gc : GuiUtils.getDescendents(this, GCanvas.class)) {
			gc.setShowPixelInfo(show);
		}
	}

	/**
	 * Sets the width and height of this program.
	 * @param width the new width, in pixels (will be rounded down to nearest integer)
	 * @param height the new height, in pixels (will be rounded down to nearest integer)
	 */
	public void setSize(double width, double height) {
		super.setSize((int) width, (int) height);
	}

	/* Protected method: setStartupObject(obj) */
	/**
	 * Sets the object that is created when the program is started so that
	 * it can be retrieved later by <code>getStartupObject</code>.
	 *
	 * @usage setStartupObject(obj);
	 * @param obj The startup object
	 */
	public void setStartupObject(Object obj) {
		startupObject = obj;
	}

	/* Method: setTitle(title) */
	/**
	 * Sets the title of this program.  The title appears in the title bar
	 * when the program is running as an application.
	 *
	 * @usage setTitle(title);
	 * @param title The title for this program
	 */
	public void setTitle(String title) {
		myTitle = title;
		if (programFrame != null) {
			programFrame.setTitle(title);
			if (Platform.isMac()) {
				// System.setProperty("com.apple.mrj.application.apple.menu.about.name", title);
			}
		}
	}

	/**
	 * Sets the width of this program without changing its height.
	 * @param width the new width, in pixels (will be rounded down to nearest integer)
	 */
	public void setWidth(double width) {
		setSize(width, getHeight());
	}

	/**
	 * Sets the x location of the top-left corner of this program.
	 * @param x the x location as a pixel offset (will be rounded down to nearest integer)
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setX(double x) {
		setLocation(x, getY());
	}

	/**
	 * Sets the y location of the top-left corner of this program.
	 * @param y the y location as a pixel offset (will be rounded down to nearest integer)
	 */
	public void setY(double y) {
		setLocation(getX(), y);
	}

	/* Method: showErrorMessage(msg) */
	/**
	 * Displays the error message in the standard output model.
	 *
	 * @usage showErrorMessage(msg);
	 * @param msg The error msg to be displayed
	 */
	public void showErrorMessage(String msg) {
		getOutputModel().showErrorMessage(msg);
	}

	/* Overridden method: start() */
	/**
	 * Starts the program when it is running in application mode.  Note that this
	 * overloads the <code>start</code> method in <code>Applet</code> and therefore
	 * will be called as part of applet startup.
	 *
	 * @usage program.start();
	 * @noshow
	 */
	public final void start() {
		appletMode = getParent() != null;
		if (appletMode) {
			if (!started) {
				started = true;
				validate();
				setVisible(true);
				appletStarter = new AppletStarter(this);
				appletStarter.start();
			}
		} else {
			start(null);
		}
	}

	/* Method: start(args) */
	/**
	 * Starts the program using the specified argument list.
	 * Note: Java back-end C++ lib programs do not call start(), so any mandatory initialization
	 * code that affects the back-end and C++ probably should not go here.
	 * Java back-end C++ programs instead use the JavaBackEnd class's main() / run() methods
	 * to construct and initialize Program objects.
	 *
	 * @usage program.start(args);
	 * @param args An array of strings passed to the program
	 */
	public void start(String[] args) {
		if (parameterTable == null) {
			parameterTable = createParameterTable(args);
		}
		if (getParent() == null) {
			initApplicationFrame();
		}
		validate();
		checkCompilerFlags();
		setVisible(true);
		if (programFrame != null) {
			programFrame.validate();
			int nComponents = centerPanel.getComponentCount();
			nComponents += northPanel.getComponentCount();
			nComponents += southPanel.getComponentCount();
			nComponents += westPanel.getComponentCount();
			nComponents += eastPanel.getComponentCount();
			if (nComponents > 0) {
				programFrame.setVisible(true);
				shown = true;
			}
			circumventFrameSizeBug(programFrame, programBounds.getSize());
		}
		started = true;
		if (!initFinished) {
			// try {
			init();
			//			} catch (Throwable t) {
			//				if (t instanceof RuntimeException) {
			//					throw (RuntimeException) t;
			//				}
			//				throw new RuntimeException(t);
			//			}
		}
		initFinished = true;
		if (programFrame != null && myMenuBar != null) {
			myMenuBar.install(programFrame);
		}
		validate();
		if (!isAppletMode()) {
			try {
				if (!StringUtils.isFalsey(System.getProperty(CONFIG_PROPERTY))
						&& !StringUtils.isFalsey(System.getenv(CONFIG_PROPERTY))) {
					loadConfiguration();
				}
			} catch (SecurityException sex) {
				// empty
			}
		}
		startRun();
	}

	/**
	 * Sets the program into animated mode.
	 * The animation() method will be called repeatedly in the background.
	 */
	public void startAnimation() {
		setAnimated(true);
	}

	/* Protected method: startHook() */
	/**
	 * Performs class-specific initialization for the program just before
	 * it starts.
	 * @noshow
	 */
	protected void startHook() {
		// empty
	}

	/* Protected method: startRun() */
	/**
	 * Initializes and runs the run method.
	 */
	protected void startRun() {
		ProgramStartupListener listener = new ProgramStartupListener();
		Component root = getRootPane();
		if (root.isShowing()) {
			root.addComponentListener(listener);
			root.validate();
			listener.waitForStartup(this);
			root.update(root.getGraphics());
		}
		root.setCursor(Cursor.getDefaultCursor());
		initFinished = true;
		runFinished = false;
		startHook();
		runHook();
		runFinished = true;
		endHook();
		if (!root.isShowing() && !getContentPane().isShowing()) {
			exit();
		}
	}
	
	/**
	 * Required method of ChangeListener interface.
	 */
	public void stateChanged(ChangeEvent event) {
		// empty
	}

	/**
	 * Sets the program into non-animated (default) mode.
	 * The animation() method will no longer be called repeatedly in the background.
	 */
	public void stopAnimation() {
		setAnimated(false);
	}
	
	/**
	 * Required method of TextListener interface.
	 * Invoked when the value of the text has changed.
	 */
	public void textValueChanged(TextEvent e) {
		// empty
	}
	
	/* Overridden method: validate() */
	/**
	 * Forwards validate to the content pane.
	 *
	 * @usage validate();
	 * @noshow
	 */
	public void validate() {
		if (isRootPaneCheckingEnabled()) getContentPane().validate();
		super.validate();
	}
	
	/** Required empty method of WindowListener interface. */
	public void windowActivated(WindowEvent event) {
		// empty
	}

	/** Required empty method of WindowListener interface. */
	public void windowClosed(WindowEvent event) {
		// empty
	}

	/** Required empty method of WindowListener interface. */
	public void windowClosing(WindowEvent event) {
		// empty
	}

	/** Required empty method of WindowListener interface. */
	public void windowDeactivated(WindowEvent event) {
		// empty
	}

	/** Required empty method of WindowListener interface. */
	public void windowDeiconified(WindowEvent event) {
		// empty
	}
	
	/**
	 * Required empty method of WindowFocusListener interface.
	 * Invoked when the Window is set to be the focused Window, which means that the Window, or one of its subcomponents, will receive keyboard events.
	 */
	public void	windowGainedFocus(WindowEvent e) {
		// empty
	}
	
	/** Required empty method of WindowListener interface. */
	public void windowIconified(WindowEvent event) {
		// empty
	}

	/**
	 * Required empty method of WindowFocusListener interface.
	 * Invoked when the Window is no longer the focused Window, which means that keyboard events will no longer be delivered to the Window or any of its subcomponents.
	 */
	public void windowLostFocus(WindowEvent e) {
		// empty
	}

	/** Required empty method of WindowListener interface. */
	public void windowOpened(WindowEvent event) {
		// empty
	}
	
	/**
	 * Required empty method of WindowStateListener interface.
	 * Invoked when window state is changed.
	 */
	public void windowStateChanged(WindowEvent e) {
		// empty
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	/* Static method: main(args) */
	/**
	 * Every application must either contain a "Main-Class" entry in its
	 * manifest file or include a main method that looks like this, where
	 * <code>MyClass</code> is the name of the program class:
	 *
	 * <p><pre><code>
	 * &nbsp;    public static void main(String[] args) {
	 * &nbsp;       new MyClass().start();
	 * &nbsp;    }
	 * </code></pre>
	 *
	 * <p>If the program needs the command line arguments, the <code>args</code>
	 * array can be passed to the <code>start</code> method and then retrieved
	 * using the <code>getArgumentArray</code> method.
	 *
	 * @param args An array of string arguments
	 */
	public static void main(String[] args) {
		if (!CommandLineProgram.isHeadless()) {
			GuiUtils.setSystemLookAndFeel();
		}
		Map<String,String> ht = createParameterTable(args);
		JTFTools.setDebugOptions(ht.get("debug"));
		String className = ht.get("code");
		if (className == null) {
			className = JTFTools.getMainClass();
		}
		Class<?> mainClass = null;
		ProgramInterface program = null;
		if (className != null) {
			if (className.endsWith(".class")) {
				className = className.substring(0, className.length() - 6);
			}
			className = className.replace('/', '.');
			CommandLineProgram.checkIfHeadless(className);
			try {
				mainClass = Class.forName(className);
			} catch (ClassNotFoundException ex) {
				// empty
			}
		}

		if (mainClass != null) {
			try {
				Object obj = mainClass.newInstance();
				if (obj instanceof ProgramInterface) {
					program = (ProgramInterface) obj;
					program.setStartupObject(null);
				} else {
					className = ht.get("program");
					if (className == null) {
						throw new ErrorException("Main class does not specify a program");
					}
					program = (Program) Class.forName(className).newInstance();
					program.setStartupObject(obj);
				}
			} catch (IllegalAccessException ex) {
				// empty
			} catch (InstantiationException ex) {
				// empty
			} catch (ClassNotFoundException ex) {
				// empty
			}
		}
		if (program == null) {
			throw new ErrorException("Cannot determine the main class.");
		}
		program.setParameterTable(ht);
		program.start();
	}
}
// end class Program
