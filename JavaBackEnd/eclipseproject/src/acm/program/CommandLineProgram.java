/*
 * @version 2017/07/21
 * - added add() overloads
 * @version 2017/04/27
 * - added support for HeadlessGraphicsProgram
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
// Class introduced in V1.1

package acm.program;

import acm.io.*;
import acm.util.*;
import stanford.cs106.util.ExceptionUtils;
import stanford.karel.*;
import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;

/**
 * This class simulates the functionality of a <code>ConsoleProgram</code>
 * in an environment that lacks a graphics context. As of JDK 1.4, it is
 * illegal even to instantiate an applet in a non-graphics environment
 * (called "headless" in the Java terminology), which means that the program
 * can no longer extend <code>Applet</code> or <code>JApplet</code>.  This
 * class creates a stripped-down program class that duplicates the operation
 * of a <code>ConsoleProgram</code> using the standard I/O streams.
 */
public class CommandLineProgram
		implements ProgramInterface, IOModel, MouseListener, MouseMotionListener,
		KeyListener, ActionListener {

	/* Private instance variables */
	private HashMap<String,String> parameterTable;
	private ArrayList<Object> finalizers;
	@SuppressWarnings("deprecation")
	private AppletStub appletStub;
	private String myTitle;
	private IOConsole myConsole;

	/* Default constructor: CommandLineProgram */
	/**
	 * This code initializes the program data structures.
	 */
	protected CommandLineProgram() {
		parameterTable = null;
		finalizers = new ArrayList<Object>();
		myTitle = getClass().getName();
		myTitle = myTitle.substring(myTitle.lastIndexOf(".") + 1);
		setConsole(createConsole());
	}

	/**
	 * Returns true if the program is running in non-graphical "headless" mode.
	 */
	public static boolean isHeadless() {
		if (JTFTools.testDebugOption("headless")) {
			return true;
		}

		try {
			Class<?> gEnvClass = java.awt.GraphicsEnvironment.class;
			Method isHeadless = gEnvClass.getMethod("isHeadless", new Class[0]);
			if (Boolean.TRUE.equals(isHeadless.invoke(null))) {
				return true;
			}
		} catch (Exception ex) {
			// empty
		}

		return false;
	}

	/* Static method: checkIfHeadless(className) */
	/**
	 * Checks to see if the program is running in a headless environment and, if so,
	 * runs it in that form.  If the environment is indeed headless, this call never
	 * returns.
	 *
	 * @usage CommandLineProgram.checkIfHeadless(className);
	 * @param className The name of the main class
	 */
	public static void checkIfHeadless(String className) {
		if (!isHeadless()) {
			return;
		}

		// see if this is a ConsoleProgram, CommandLineProgram, Karel, GraphicsProgram, ...
		boolean isConsole  = false;
		boolean isGraphics = false;
		boolean isKarel    = false;

		try {
			ClassLoader loader = ClassLoader.getSystemClassLoader();
			Class<?> mainClass = loader.loadClass(className);
			isConsole = CommandLineProgram.class.isAssignableFrom(mainClass)
					|| ConsoleProgram.class.isAssignableFrom(mainClass);
			isKarel = KarelInterface.class.isAssignableFrom(mainClass)
					|| KarelProgram.class.isAssignableFrom(mainClass);
			isGraphics = GraphicsProgramInterface.class.isAssignableFrom(mainClass)
					|| GraphicsProgram.class.isAssignableFrom(mainClass);
			if (!isConsole && !isKarel && !isGraphics) {
				return;   // not a proper program
			}
		} catch (Exception ex) {
			// empty
		}

		// now reload the main class again but using a custom ClassLoader,
		// which will patch it to be a subclass of CommandLineProgram so that
		// it will run correctly in a headless mode
		try {
			ClassLoader loader = null;
			ProgramInterface program = null;

			if (isConsole) {
				loader = new PatchingClassLoader(className,
						/* new superclass */ acm.program.CommandLineProgram.class);
			} else if (isKarel) {
				loader = new PatchingClassLoader(className,
						/* new superclass */ stanford.karel.HeadlessSuperKarel.class);
			} else if (isGraphics) {
				loader = new PatchingClassLoader(className,
						/* new superclass */ acm.program.HeadlessGraphicsProgram.class);
			}

			if (loader == null) {
				return;
			}

			Class<?> mainClass = loader.loadClass(className);
			program = (ProgramInterface) mainClass.getDeclaredConstructor().newInstance();
			program.init();
			program.run();
			program.exit();
		} catch (Exception ex) {
			throw new ErrorException(ExceptionUtils.getUnderlyingCause(ex));
		}
	}

	/* Method: run() */
	/**
	 * Contains the code to be executed for each specific program subclass.  If
	 * you are defining your own program, you need to override the definition of
	 * <code>run</code> so that it contains the code for your application.
	 */
	public void run() {
		/* Empty */
	}

	/* Method: init() */
	/**
	 * The init method is called at startup time before the run method is
	 * called.  Subclasses can override this method to perform any
	 * initialization code that would ordinarily be included in an applet
	 * <code>init</code> method.  This method is used only for certain styles
	 * of application development that have their roots in the applet world;
	 * other styles will not ordinarily use or override this method.
	 *
	 * @usage program.init();
	 */
	public void init() {
		/* Empty */
	}

	/**
	 * Causes the program to delay for the given number of milliseconds.
	 */
	public void pause(double milliseconds) {
		try {
			Thread.sleep((long) milliseconds);
		} catch (InterruptedException ie) {
			// empty
		}
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
		return false;
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

	/* Method: getDialog() */
	/**
	 * Returns the dialog used for user interaction.
	 *
	 * @usage IODialog dialog = program.getDialog();
	 * @return The <code>IODialog</code> object used for this program
	 */
	public IODialog getDialog() {
		return null;
	}

	/* Method: getInputModel() */
	/**
	 * Returns the <code>IOModel</code> used for program input, which will
	 * typically be either the default <code>IOConsole</code> or <code>IODialog</code> object.
	 *
	 * @usage IOModel io = program.getInputModel();
	 * @return The <code>IOModel</code> used for program input
	 */
	public IOModel getInputModel() {
		return getConsole();
	}

	/* Method: getOutputModel() */
	/**
	 * Returns the <code>IOModel</code> used for program output, which will
	 * typically be either the default <code>IOConsole</code> or <code>IODialog</code> object.
	 *
	 * @usage IOModel io = program.getOutputModel();
	 * @return The <code>IOModel</code> used for program output
	 */
	public IOModel getOutputModel() {
		return getConsole();
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

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public JPanel getRegionPanel(String region) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public Component add(Component comp) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void add(Component comp, Object constraints) {
		throw new ErrorException("No graphics environment");
	}
	
	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void add(Component comp, String region, Object constraints) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void addActionListeners() {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void addActionListeners(ActionListener listener) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void addMouseListeners() {
		throw new ErrorException("No graphics environment");
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

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void setFont(Font font) {
		throw new ErrorException("No graphics environment");
	}
	
	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void setForeground(Color color) {
		throw new ErrorException("No graphics environment");
	}
	
	public void setParameterTable(Map<String, String> table) {
		// empty
	}

	public void setStartupObject(Object obj) {
		// empty
	}

	public void start() {
		start(new String[0]);
	}

	/* Method: start(args) */
	/**
	 * Starts the program using the specified argument list.
	 *
	 * @usage program.start(args);
	 * @param args An array of strings passed to the program
	 */
	public void start(String[] args) {
		if (parameterTable == null && args != null) {
			parameterTable = createParameterTable(args);
		}
		init();
		try {
			run();
		} catch (Throwable t) {
			if (t instanceof RuntimeException) {
				throw (RuntimeException) t;
			} else {
				throw new RuntimeException(t);
			}
		}
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

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	protected IODialog createDialogIO() {
		throw new ErrorException("No graphics environment");
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
		return new Dimension(0, 0);
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
		return 0;
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
		return 0;
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
		return value;
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void setLayout(LayoutManager layout) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public LayoutManager getLayout() {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void setBackground(Color color) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	protected void addImpl(Component comp, Object constraints, int index) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void remove(int index) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void remove(Component comp) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void removeAll() {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void validate() {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void repaint() {
		throw new ErrorException("No graphics environment");
	}

	/* Overridden method: destroy() */
	/**
	 * Called when the program has been told to destroy itself.
	 *
	 * @usage program.destroy();
	 * @noshow
	 */
	public void destroy() {
		/* Empty */
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
		CommandLineProgram program = null;
		if (className != null) {
			if (className.endsWith(".class")) {
				className = className.substring(0, className.length() - 6);
			}
			className = className.replace('/', '.');
			try {
				mainClass = Class.forName(className);
			} catch (ClassNotFoundException ex) {
				/* Empty */
			}
		}
		if (mainClass != null) {
			try {
				Object obj = mainClass.getDeclaredConstructor().newInstance();
				if (obj instanceof CommandLineProgram) {
					program = (CommandLineProgram) obj;
				} else {
					throw new ErrorException("Main class does not specify a program");
				}
			} catch (IllegalAccessException ex) {
				/* Empty */
			} catch (InstantiationException ex) {
				/* Empty */
			} catch (InvocationTargetException ex) {
				/* Empty */
			} catch (NoSuchMethodException ex) {
				/* Empty */
			}
		}
		if (program == null) throw new ErrorException("Cannot determine the main class.");
		program.setParameterTable(ht);
		program.start(null);
	}

	/**********************************************************************/
	/* Menu handling methods                                              */
	/**********************************************************************/

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	public void menuAction(String cmd) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	protected void setMacMenuBarFlag(boolean flag) {
		throw new ErrorException("No graphics environment");
	}

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	protected boolean getMacMenuBarFlag() {
		throw new ErrorException("No graphics environment");
	}

	/**********************************************************************/
	/* Protected methods                                                  */
	/**********************************************************************/

	/**
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	protected Component getBorder(String side) {
		throw new ErrorException("No graphics environment");
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
	 * Throws an ErrorException because a CommandLineProgram has no graphical environment.
	 */
	protected boolean isStarted() {
		throw new ErrorException("No graphics environment");
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
	@SuppressWarnings("deprecation")
	protected void setAppletStub(AppletStub stub) {
		appletStub = stub;
	}

	/* Protected method: getAppletStub() */
	/**
	 * Retrieves the applet stub.
	 *
	 * @usage AppletStub stub = getAppletStub();
	 * @return The applet stub
	 */
	@SuppressWarnings("deprecation")
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
}

