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

import acm.gui.*;
import acm.io.*;
import acm.util.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.lang.reflect.*;
import java.net.*;
import java.util.*;
import javax.swing.*;

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
public abstract class Program extends JApplet
  implements IOModel, Runnable, MouseListener, MouseMotionListener,
             KeyListener, ActionListener {

/** Constant specifying the north edge of the container */
	public static final String NORTH = BorderLayout.NORTH;

/** Constant specifying the south edge of the container */
	public static final String SOUTH = BorderLayout.SOUTH;

/** Constant specifying the east edge of the container */
	public static final String EAST = BorderLayout.EAST;

/** Constant specifying the west edge of the container */
	public static final String WEST = BorderLayout.WEST;

/** Constant specifying the center of the container */
	public static final String CENTER = BorderLayout.CENTER;

/* Default constructor: Program */
/**
 * This code initializes the program data structures.
 */
	protected Program() {
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
		/* Empty */
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
		/* Empty */
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
 * Makes sure that <code>print</code> can display a <code>boolean</code>.
 * @noshow
 */
	public final void print(boolean x) {
		print("" + x);
	}

/**
 * Makes sure that <code>print</code> can display a <code>char</code>.
 * @noshow
 */
	public final void print(char x) {
		print("" + x);
	}

/**
 * Makes sure that <code>print</code> can display a <code>double</code>.
 * @noshow
 */
	public final void print(double x) {
		print("" + x);
	}

/**
 * Makes sure that <code>print</code> can display a <code>float</code>.
 * @noshow
 */
	public final void print(float x) {
		print("" + x);
	}

/**
 * Makes sure that <code>print</code> can display an <code>int</code>.
 * @noshow
 */
	public final void print(int x) {
		print("" + x);
	}

/**
 * Makes sure that <code>print</code> can display a <code>long</code>.
 * @noshow
 */
	public final void print(long x) {
		print("" + x);
	}

/**
 * Makes sure that <code>print</code> can display an <code>Object</code>.
 * @noshow
 */
	public final void print(Object x) {
		print("" + x);
	}

/* Method: println() */
/**
 * Advances the console cursor to the beginning of the next line.
 *
 * @usage program.println();
 */
	public void println() {
		getOutputModel().println();
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
 * Makes sure that <code>println</code> can display a <code>boolean</code>.
 * @noshow
 */
	public final void println(boolean x) {
		println("" + x);
	}

/**
 * Makes sure that <code>println</code> can display a <code>char</code>.
 * @noshow
 */
	public final void println(char x) {
		println("" + x);
	}

/**
 * Makes sure that <code>println</code> can display a <code>double</code>.
 * @noshow
 */
	public final void println(double x) {
		println("" + x);
	}

/**
 * Makes sure that <code>println</code> can display a <code>float</code>.
 * @noshow
 */
	public final void println(float x) {
		println("" + x);
	}

/**
 * Makes sure that <code>println</code> can display an <code>int</code>.
 * @noshow
 */
	public final void println(int x) {
		println("" + x);
	}

/**
 * Makes sure that <code>println</code> can display a <code>long</code>.
 * @noshow
 */
	public final void println(long x) {
		println("" + x);
	}

/**
 * Makes sure that <code>println</code> can display an <code>Object</code>.
 * @noshow
 */
	public final void println(Object x) {
		println("" + x);
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
		return readLine(null);
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
		return readInt(null, Integer.MIN_VALUE, Integer.MAX_VALUE);
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
		return readInt(null, low, high);
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
		return readDouble(null, Double.NEGATIVE_INFINITY, Double.POSITIVE_INFINITY);
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
		return readDouble(null, low, high);
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
		return readBoolean(null);
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
		return readBoolean(prompt, "true", "false");
	}

/* Method: readBoolean(prompt, trueLabel, falseLabel) */
/**
 * Prompts the user to enter a boolean value, which is matched against the
 * labels provided.  If the user enters a value that is not one of the two
 * choices, <code>readBoolean</code> ordinarily offers the user a chance
 * to reenter the data, although this behavior can be changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage boolean flag = program.readBoolean(prompt);
 * @param prompt The prompt string to display to the user
 * @param trueLabel The string used to indicate <code>true</code>
 * @param falseLabel The string used to indicate <code>false</code>
 * @return The value of the input interpreted as a boolean value
 */
	public boolean readBoolean(String prompt, String trueLabel, String falseLabel) {
		return getInputModel().readBoolean(prompt, trueLabel, falseLabel);
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
		if (region.equals(NORTH)) {
			return northPanel;
		} else if (region.equals(SOUTH)) {
			return southPanel;
		} else if (region.equals(WEST)) {
			return westPanel;
		} else if (region.equals(EAST)) {
			return eastPanel;
		} else if (region.equals(CENTER)) {
			return centerPanel;
		} else {
			throw new ErrorException("getRegionPanel: Illegal region " + region);
		}
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
 */
	public void add(Component comp, String region, Object constraints) {
		if (region.equals(NORTH)) {
			northPanel.add(comp, constraints);
		} else if (region.equals(SOUTH)) {
			southPanel.add(comp, constraints);
		} else if (region.equals(WEST)) {
			westPanel.add(comp, constraints);
		} else if (region.equals(EAST)) {
			eastPanel.add(comp, constraints);
		} else if (region.equals(CENTER)) {
			centerPanel.add(comp, constraints);
		} else {
			throw new ErrorException("add: Illegal region " + region);
		}
	}

/* Method: addActionListeners() */
/**
 * Adds the program as an <code>ActionListener</code> to every button in
 * the structure that does not have a listener already.
 *
 * @usage addActionListeners();
 */
	public void addActionListeners() {
		addActionListeners(this);
	}

/* Method: addActionListeners(listener) */
/**
 * Adds the specified listener to every button in
 * the structure that does not have a listener already.
 *
 * @usage addActionListeners(listener);
 * @param listener The <code>ActionListener</code> to be added
 */
	public void addActionListeners(ActionListener listener) {
		addActionListeners(getContentPane(), listener);
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
		if (programFrame != null) programFrame.setTitle(title);
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

/* Method: start(args) */
/**
 * Starts the program using the specified argument list.
 *
 * @usage program.start(args);
 * @param args An array of strings passed to the program
 */
	public void start(String[] args) {
		if (parameterTable == null) parameterTable = createParameterTable(args);
		if (getParent() == null) initApplicationFrame();
		validate();
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
		init();
		if (programFrame != null && myMenuBar != null) {
			myMenuBar.install(programFrame);
		}
		validate();
		startRun();
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
		if (!appletMode) System.exit(0);
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
		JTFTools.pause(milliseconds);
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

/**********************************************************************/
/* Listener methods                                                   */
/**********************************************************************/

/* Method: mouseClicked (implements MouseListener) */
/**
 * Called when the mouse is clicked.  A call to <code>mouseClicked</code>
 * is always preceded by both a <code>mousePressed</code> and a
 * <code>mouseReleased</code> event for the same source.
 */
	public void mouseClicked(MouseEvent e) { }

/* Method: mousePressed (implements MouseListener) */
/**
 * Called when the mouse button is pressed.
 */
	public void mousePressed(MouseEvent e) { }

/* Method: mouseReleased (implements MouseListener) */
/**
 * Called when the mouse button is released.
 */
	public void mouseReleased(MouseEvent e) { }

/* Method: mouseEntered (implements MouseListener) */
/**
 * Called when the mouse enters the source (which may be
 * either a component or a <code>GObject</code>).
 */
	public void mouseEntered(MouseEvent e) { }

/* Method: mouseExited (implements MouseListener) */
/**
 * Called when the mouse exits the source (which may be
 * either a component or a <code>GObject</code>).
 */
	public void mouseExited(MouseEvent e) { }

/* Method: mouseMoved (implements MouseMotionListener) */
/**
 * Called when the mouse is moved.
 */
	public void mouseMoved(MouseEvent e) { }

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
	public void mouseDragged(MouseEvent e) { }

/* Method: keyTyped (implements KeyListener) */
/**
 * Called when a key is typed (i.e., pressed and released).
 */
	public void keyTyped(KeyEvent e) { }

/* Method: keyPressed (implements KeyListener) */
/**
 * Called when a key is pressed.
 */
	public void keyPressed(KeyEvent e) { }

/* Method: keyReleased (implements KeyListener) */
/**
 * Called when a key is released.
 */
	public void keyReleased(KeyEvent e) { }

/* Method: actionPerformed (implements ActionListener) */
/**
 * Called when a component (typically a button) is activated.
 */
	public void actionPerformed(ActionEvent e) { }

/**********************************************************************/
/* Factory methods                                                    */
/**********************************************************************/

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

/**********************************************************************/
/* Overrides of existing methods                                      */
/**********************************************************************/

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
			centerPanel.setBackground(color);
		}
		super.setBackground(color);
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
		HashMap<String,String> ht = createParameterTable(args);
		JTFTools.setDebugOptions(ht.get("debug"));
		String className = ht.get("code");
		if (className == null) {
			className = JTFTools.getMainClass();
		}
		Class<?> mainClass = null;
		Program program = null;
		if (className != null) {
			if (className.endsWith(".class")) {
				className = className.substring(0, className.length() - 6);
			}
			className = className.replace('/', '.');
			CommandLineProgram.checkIfHeadless(className);
			try {
				mainClass = Class.forName(className);
			} catch (ClassNotFoundException ex) {
				/* Empty */
			}
		}
		if (mainClass != null) {
			try {
				Object obj = mainClass.newInstance();
				if (obj instanceof Program) {
					program = (Program) obj;
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
				/* Empty */
			} catch (InstantiationException ex) {
				/* Empty */
			} catch (ClassNotFoundException ex) {
				/* Empty */
			}
		}
		if (program == null) throw new ErrorException("Cannot determine the main class.");
		program.setParameterTable(ht);
		program.start();
	}

/* Method: menuAction(e) */
/**
 * Called whenever a program action is detected in the menu bar.
 * Subclasses can override this method to extend the set of menu
 * commands recognized even in the absence of a component with
 * keyboard focus.
 */
	public boolean menuAction(ActionEvent e) {
		String cmd = e.getActionCommand();
		if (cmd.equals("Quit")) {
			exit();
		} else if (cmd.equals("Print")) {
			Frame frame = JTFTools.getEnclosingFrame(this);
			if (frame == null) return true;
			PrintJob pj = frame.getToolkit().getPrintJob(frame, myTitle, null);
			if (pj == null) return true;
			Graphics pg = pj.getGraphics();
			pg.translate(PRINT_MARGIN, PRINT_MARGIN);
			frame.printAll(pg);
			pj.end();
			return true;
		} else if (cmd.equals("Export Applet") || cmd.equals("Submit Project")) {
			JTFTools.executeExportAction(this, cmd);
			return true;
		}
		return getConsole().menuAction(e);
	}

/**********************************************************************/
/* Protected methods                                                  */
/**********************************************************************/

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

/* Protected method: isStarted() */
/**
 * Checks to see whether this program has started, usually by checking to see
 * whether some pane exists.  Subclasses can override this method to ensure
 * that their structures are visible before proceeding.
 * @noshow
 */
	protected boolean isStarted() {
		IOConsole console = getConsole();
		if (console == null) return false;
		if (console.getParent() == null) return true;
		Dimension size = console.getSize();
		return (console.isShowing()) && (size.width != 0) && (size.height != 0);
	}

/* Protected method: startHook() */
/**
 * Performs class-specific initialization for the program just before
 * it starts.
 * @noshow
 */
	protected void startHook() {
		/* Empty */
	}

/* Protected method: endHook() */
/**
 * Performs class-specific cleanup for the program just after
 * it finishes.
 * @noshow
 */
	protected void endHook() {
		/* Empty */
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

/* Protected method: setParameterTable(ht) */
/**
 * Sets the parameter table for this program.
 *
 * @usage setParameterTable(ht);
 * @param ht The parameter table
 */
	protected void setParameterTable(HashMap<String,String> ht) {
		parameterTable = ht;
	}

/* Protected method: getParameterTable() */
/**
 * Retrieves the parameter table.
 *
 * @usage ParameterTable ht = getParameterTable();
 * @return The parameter table
 */
	protected HashMap<String,String> getParameterTable() {
		return parameterTable;
	}

/* Protected method: setStartupObject(obj) */
/**
 * Sets the object that is created when the program is started so that
 * it can be retrieved later by <code>getStartupObject</code>.
 *
 * @usage setStartupObject(obj);
 * @param obj The startup object
 */
	protected void setStartupObject(Object obj) {
		startupObject = obj;
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
		startHook();
		runHook();
		endHook();
		if (!root.isShowing() && !getContentPane().isShowing()) exit();
	}

/* Protected method: runHook() */
/**
 * Calls the run method in the program.  Subclasses can override this
 * method to transfer control somewhere else.
 */
	protected void runHook() {
		run();
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
	protected static HashMap<String,String> createParameterTable(String[] args) {
		if (args == null) return null;
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

/**********************************************************************/
/* Private methods                                                    */
/**********************************************************************/

/* Private method: initContentPane(contentPane) */
/**
 * Initializes the content pane to contain its five regions.
 */
	private void initContentPane(Container contentPane) {
		contentPane.setLayout(new ProgramContentPaneLayout(this));
		northPanel = new JPanel();
		southPanel = new JPanel();
		eastPanel = new JPanel();
		westPanel = new JPanel();
		centerPanel = new JPanel();
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

/* Private method: addActionListeners(comp, listener) */
/**
 * Recursively adds the specified listener as an <code>ActionListener</code> to
 * every button in the hierarchy.  Reflection is used because there are many
 * possible classes of button-like objects.
 */
	private void addActionListeners(Component comp, ActionListener listener) {
		if (isButton(comp)) {
			if (!hasActionListener(comp)) {
				try {
					Class[] types = { Class.forName("java.awt.event.ActionListener") };
					Object[] args = { listener };
					Method addActionListener = comp.getClass().getMethod("addActionListener", types);
					addActionListener.invoke(comp, args);
				} catch (Exception ex) {
					throw new ErrorException(ex);
				}
			}
		} else if (comp instanceof Container) {
			Container container = (Container) comp;
			int nComponents = container.getComponentCount();
			for (int i = 0; i < nComponents; i++) {
				addActionListeners(container.getComponent(i), listener);
			}
		}
	}

/* Private method: isButton(comp) */
/**
 * Determines whether the component is a button.
 */
	private boolean isButton(Component comp) {
		return (comp instanceof Button || comp instanceof JButton);
	}

/* Private method: hasActionListener(comp) */
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

/* Private method: getMyCaller() */
/**
 * Returns the name of the caller of the method that invoked <code>getMyCaller</code>.
 */
	private String getMyCaller() {
		StackTraceElement[] stack = new Throwable().getStackTrace();
		return stack[2].getClassName() + "." + stack[2].getMethodName();
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

/* Private constants */
	private static final int DEFAULT_X = 16;
	private static final int DEFAULT_Y = 40;
	private static final int DEFAULT_WIDTH = 754;
	private static final int DEFAULT_HEIGHT = 492;
	private static final int PRINT_MARGIN = 36;

/* Private instance variables */
	private ArrayList<Object> finalizers;
	private HashMap<String,String> parameterTable;
	private JFrame programFrame;
	private AppletStub appletStub;
	private String myTitle;
	private ProgramMenuBar myMenuBar;
	private Component northBorder;
	private Component southBorder;
	private Component eastBorder;
	private Component westBorder;
	private JPanel northPanel;
	private JPanel southPanel;
	private JPanel eastPanel;
	private JPanel westPanel;
	private JPanel centerPanel;
	private IOConsole myConsole;
	private IODialog myDialog;
	private IOModel inputModel;
	private IOModel outputModel;
	private Object startupObject;
	private AppletStarter appletStarter;
	private Rectangle programBounds;
	private boolean started;
	private boolean shown;
	private boolean initFinished;
	private boolean appletMode;

}

/* Package class: AppletStarter */
/**
 * This class creates a new thread in which to execute the <code>run</code>
 * method in the context of an applet.
 */

class AppletStarter implements Runnable {

/* Constructor: AppletStarter(program) */
/**
 * Creates an object responsible for starting the main thread for the program.
 */
	public AppletStarter(Program program) {
		myProgram = program;
	}

/* Method: start() */
/**
 * Starts a new thread for the program that will execute
 * the <code>run</code> method.
 */
	public void start() {
		try {
			mainThread = new Thread(this);
			mainThread.start();
			if (JTFTools.testDebugOption("startup")) {
				System.out.println("Starting main thread using Thread package");
			}
		} catch (SecurityException ex) {
			if (JTFTools.testDebugOption("startup")) {
				System.out.println("Starting main thread using Executor because " + ex);
			}
			forkUsingExecutor();
		}
	}

/* Method: stop() */
/**
 * Stops the main thread, using whichever strategy is appropriate for
 * the implementation.
 */
	public void stop() {
		try {
			if (executor == null) {
				Class<?> threadClass = Class.forName("java.lang.Thread");
				Method stop = threadClass.getMethod("stop", new Class[0]);
				stop.invoke(mainThread, new Object[0]);
			} else {
				Method shutdownNow = executor.getClass().getMethod("shutdownNow", new Class[0]);
				shutdownNow.invoke(executor, new Object[0]);
			}
		} catch (Exception ex) {
			/* Empty */
		}
	}

/* Method: run() */
/**
 * Starts the program by calling its <code>startRun</code> method, which will
 * eventually call <code>run</code> in the program.
 */
	public void run() {
		myProgram.startRun();
	}

/* Method: getMainThread() */
/**
 * Returns the thread that is running the main program.
 */
	public Thread getMainThread() {
		return mainThread;
	}

/* Private method: forkUsingExecutor */
/**
 * The AppletRunner in Java 5.0 does not allow nontrusted packages to fork
 * new threads.  This class uses the <code>ScheduledExecutor</code> class in
 * <code>java.util.concurrent</code> to do the fork so that it comes from an
 * acceptable place.
 */
	private void forkUsingExecutor() {
		try {
			Class<?> scheduledExecutorClass = Class.forName("java.util.concurrent.ScheduledExecutor");
			Class[] types1 = { Integer.TYPE };
			Object[] args1 = { new Integer(1) };
			Constructor<?> constructor = scheduledExecutorClass.getConstructor(types1);
			executor = constructor.newInstance(args1);
			Class<?> timeUnitClass = Class.forName("java.util.concurrent.TimeUnit");
			Field secondsField = timeUnitClass.getField("SECONDS");
			Object seconds = secondsField.get(null);
			Class[] types2 = { Class.forName("java.lang.Runnable"),
			                  Long.TYPE,
			                  Class.forName("java.util.concurrent.TimeUnit") };
			Object[] args2 = { this, new Long(0), seconds };
			Method schedule = executor.getClass().getMethod("schedule", types2);
			schedule.invoke(executor, args2);
		} catch (Exception ex) {
			if (JTFTools.testDebugOption("startup")) {
				System.out.println("Executor failed because " + ex);
			}
			executor = null;
			mainThread = Thread.currentThread();
			myProgram.startRun();
		}
	}

/* Private instance variables */
	private Program myProgram;
	private Thread mainThread;
	private Object executor;
}

/* Package class: ProgramActionListener */
/**
 * This class listens for global action events in the menu bar.  These events are
 * passed back to the program through the protected <code>globalMenuAction<code>
 * method.
 */
class ProgramActionListener implements ActionListener {

	public ProgramActionListener(Program owner) {
		program = owner;
	}

/* ActionListener interface */
	public void actionPerformed(ActionEvent e) {
		program.menuAction(e);
	}

/* Private instance variables */
	private Program program;

}

/* Package class: ProgramWindowListener */
/**
 * This class implements a simple window listener for programs whose only
 * function is to exit if the window is closed.
 */
class ProgramWindowListener implements WindowListener {

	public ProgramWindowListener(Program owner) {
		program = owner;
	}

/* WindowListener interface */
	public void windowClosing(WindowEvent e) {
		((Component) e.getSource()).setVisible(false);
		program.exit();
	}

	public void windowOpened(WindowEvent e) { }
	public void windowClosed(WindowEvent e) { }
	public void windowIconified(WindowEvent e) { }
	public void windowDeiconified(WindowEvent e) { }
	public void windowActivated(WindowEvent e) { }
	public void windowDeactivated(WindowEvent e) { }

/* Private instance variables */
	private Program program;

}

/* Package class: ProgramStartupListener */
/**
 * This class implements a component listener that supports the program
 * startup logic.
 */
class ProgramStartupListener implements ComponentListener {

/* Method: waitForStartup(program) */
/**
 * Waits until the specified program has started.
 */
	public synchronized void waitForStartup(Program program) {
		JTFTools.pause(STARTUP_DELAY);
		while (!program.isStarted()) {
			try {
				wait(STARTUP_CYCLE);
			} catch (InterruptedException ex) {
				/* Empty */
			}
		}
	}

/* ComponentListener interface */
	public void componentHidden(ComponentEvent e) { }
	public void componentMoved(ComponentEvent e) { }

	public void componentResized(ComponentEvent e) {
		componentShown(e);
	}

	public synchronized void componentShown(ComponentEvent e) {
		notifyAll();
	}

/* Private constants */
	private static final int STARTUP_DELAY = 1000;
	private static final int STARTUP_CYCLE = 300;

}

/* Package class: ProgramFrame */
/**
 * This frame represents the visible component that encloses the program.
 */
class ProgramFrame extends JFrame {

	public ProgramFrame(String title) {
		super(title);
	}

	public void update(Graphics g) {
		paint(g);
	}
}

/* Package class: ProgramContentPaneLayout */
/**
 * This layout manager is identical to the BorderLayout manager except that
 * it also resizes the Program object itself to correspond to the available
 * space.  This class is required for JDK 1.6 systems, when it became
 * necessary to separate the content pane of the JApplet from the program's
 * JApplet itself.
 */
class ProgramContentPaneLayout extends BorderLayout {

	public ProgramContentPaneLayout(Program program) {
		myProgram = program;
	}

	public void layoutContainer(Container parent) {
		super.layoutContainer(parent);
		if (!myProgram.isAncestorOf(parent)) {
			Dimension psize = parent.getSize();
			Insets insets = parent.getInsets();
			int x = insets.left;
			int y = insets.top;
			int width = psize.width - insets.left - insets.right;
			int height = psize.height - insets.top - insets.bottom;
			myProgram.setBounds(x, y, width, height);
			ComponentEvent e = new ComponentEvent(myProgram, ComponentEvent.COMPONENT_RESIZED);
			Toolkit.getDefaultToolkit().getSystemEventQueue().postEvent(e);
		}
	}

/* Private instance variables */
	private Program myProgram;

}

/* Package class: ProgramAppletStub */
/**
 * This class implements both the AppletStub and AppletContext interfaces and allows
 * standard applications to run with the same capabilities as an applet.
 */
class ProgramAppletStub implements AppletContext, AppletStub {
	public ProgramAppletStub(Program program) {
		applet = program;
	}

	public void setFrame(Frame frame) {
		enclosure = frame;
	}

	public boolean isActive() {
		return true;
	}

	public URL getDocumentBase() {
		return getCodeBase();
	}

	public URL getCodeBase() {
		try {
			return new URL("file:" + getCanonicalPath("."));
		} catch (MalformedURLException ex) {
			throw new ErrorException("Error: Illegal document base URL");
		}
	}

	public String getParameter(String name) {
		return null;
	}

	public AppletContext getAppletContext() {
		return this;
	}

	public void appletResize(int width, int height) {
		if (enclosure == null) {
			if (!recursiveResizeCheck) {
				recursiveResizeCheck = true;
				applet.resize(width, height);
				applet.validate();
				recursiveResizeCheck = false;
			}
		} else {
			enclosure.setSize(width, height);
			enclosure.validate();
		}
	}

	public AudioClip getAudioClip(URL url) {
		AudioClip clip = null;
		if (clip == null) clip = getNewAudioClip(url);
		return clip;
	}

	public Image getImage(URL url) {
		try {
			Object content = url.getContent();
			if (content instanceof ImageProducer) {
				return applet.createImage((ImageProducer) content);
			}
		} catch (IOException ex) {
			/* Ignore the exception and fail */
		}
		return null;
	}

	public Applet getApplet(String name) {
		return null;
	}

	public Enumeration<Applet> getApplets() {
		return new Vector<Applet>().elements();
	}

	public void showDocument(URL url) {
		if (applet != null) applet.getAppletContext().showDocument(url);
	}

	public void showDocument(URL url, String target) {
		if (applet != null) applet.getAppletContext().showDocument(url, target);
	}

	public void showStatus(String status) {
		if (applet == null) {
			System.out.println(status);
		} else {
			applet.showStatus(status);
		}
	}

	public void setStream(String key, InputStream stream) {
		throw new ErrorException("setStream: unimplemented operation");
	}

	public InputStream getStream(String key) {
		throw new ErrorException("getStream: unimplemented operation");
	}

	public java.util.Iterator<String> getStreamKeys() {
		throw new ErrorException("getStreamKeys: unimplemented operation");
	}

	private String getCanonicalPath(String start) {
		String path = new File(start).getAbsolutePath();
		int sp;
		while ((sp = path.indexOf(' ')) != -1) {
			path = path.substring(0, sp) + "%20" + path.substring(sp + 1);
		}
		return path;
	}

	private synchronized AudioClip getNewAudioClip(URL url) {
		try {
			Class<?> type = Class.forName("java.applet.Applet");
			Class[] types = { Class.forName("java.net.URL") };
			Object[] args = { url };
			Method fn = type.getMethod("newAudioClip", types);
			return (AudioClip) fn.invoke(null, args);
		} catch (Exception ex) {
			return null;
		}
	}

/* Private instance variables */
	private Applet applet;
	private Frame enclosure;
	private boolean recursiveResizeCheck;

}

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
				Class[] types = { Class.forName("java.awt.event.ActionListener") };
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
