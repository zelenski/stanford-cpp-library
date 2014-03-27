/*
 * @(#)IOConsole.java   1.99.1 08/12/08
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
// Bug fix 18-Feb-07 (ESR, JTFBug 2007-003, reported by Dale Skrien)
//   1. Fixed bug in ConsoleModel.clear.
//
// Bug fix 08-May-07 (ESR, JTFBug 2007-008)
//   1. Completely rewrote PrintConsole code which no longer worked in JDK 1.6.
//
// Code cleanup 21-May-08 (ESR)
//   1. Reimplemented menu bar code to correspond to overall redesign of
//      the ProgramMenuBar class.

package acm.io;

import acm.program.*;
import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;
import javax.swing.text.*;

/* Class: IOConsole */
/**
 * The <code>IOConsole</code> class makes it easier to interact with
 * the user using text-based input and output in the style of a
 * traditional console.  Given a <code>IOConsole</code> object, you
 * can write output to that console using the
 * <a href="#print(String)"><code>print</code></a> and
 * <a href="#println(String)"><code>println</code></a>
 * methods, just as you would for the standard output stream.
 * To request input from the user, the most common methods are
 *
 * <ul>
 * <li><a href="#readInt()"><code>readInt</code></a>
 * <li><a href="#readDouble()"><code>readDouble</code></a>
 * <li><a href="#readBoolean()"><code>readBoolean</code></a>
 * <li><a href="#readLine()"><code>readLine</code></a>
 * </ul>
 *
 * <p>A <code>IOConsole</code> object is a lightweight component and must be
 * added to an installed <code>Frame</code> or <code>JFrame</code> before it becomes
 * visible on the screen.  The usual strategy for including a console in a frame is
 * to use the <a href="../program/ConsoleProgram.html"><code>ConsoleProgram</code></a>
 * mechanism in the <code>acm.program</code> package.
 *
 * The operation of the <code>IOConsole</code> class is illustrated by
 * the following <code>test</code> method, which generates the session shown on
 * the right.  The user input appears in <font color=blue>blue</font>,
 * just as it does in the console window.
 *
 * <p><table><tr><td><pre><code>
 * &nbsp;    public void test(IOConsole console) {
 * &nbsp;       console.println("IOConsole class test");
 * &nbsp;       int n = console.readInt("Enter an integer: ");
 * &nbsp;       console.println("That integer was " + n);
 * &nbsp;       double d = console.readDouble("Enter a real number: ");
 * &nbsp;       console.println("That number was " + d);
 * &nbsp;       boolean b = console.readBoolean("Enter a boolean value: ");
 * &nbsp;       console.println("That value was " + b);
 * &nbsp;       String line = console.readLine("Enter a line: ");
 * &nbsp;       console.println("That line was \"" + line + "\"");
 * &nbsp;    }
 * </code></pre></td>
 * <td width=260 align=right valign=top>
 * <img src="../../../../images/ConsoleTest.gif">
 * </td></tr></table>
 */
public class IOConsole extends Container implements IOModel {

/* Constant: SYSTEM_CONSOLE */
/**
 * This constant is an object that offers the functionality of the
 * <code>IOConsole</code> class, but which does so using the standard
 * I/O streams <code>System.in</code> and <code>System.out</code>.
 */
	public static final IOConsole SYSTEM_CONSOLE = new SystemConsole();

/* Constructor: IOConsole() */
/**
 * Creates a new <code>IOConsole</code> object.
 *
 * @usage IOConsole console = new IOConsole();
 */
	public IOConsole() {
		consoleModel = createConsoleModel();
		consoleModel.setConsole(this);
		setBackground(Color.WHITE);
		setInputColor(Color.BLUE);
		setInputStyle(Font.BOLD);
		setErrorColor(Color.RED);
		setErrorStyle(Font.BOLD);
		setFont(JTFTools.getStandardFont(DEFAULT_FONT));
		Component consolePane = consoleModel.getConsolePane();
		if (consolePane != null) {
			setLayout(new BorderLayout());
			add(consolePane, BorderLayout.CENTER);
		}
		reader = null;
		writer = null;
		exceptionOnError = false;
	}

/* Method: clear() */
/**
 * Clears the console display.
 *
 * @usage console.clear();
 */
	public void clear() {
		consoleModel.clear();
	}

/* Method: print(value) */
/**
 * Displays the argument value on the console, leaving the cursor at the end of
 * the output.  The <code>print</code> method is overloaded so that
 * <code>value</code> can be of any type.
 *
 * @usage console.print(value);
 * @param value The value to be displayed
 */
	public void print(String value) {
		getWriter().print(value);
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
 * @usage console.println();
 */
	public void println() {
		getWriter().println();
	}

/* Method: println(value) */
/**
 * Displays the argument value on the console and then advances the cursor
 * to the beginning of the next line.  The <code>println</code> method is
 * overloaded so that <code>value</code> can be of any type.
 *
 * @usage console.println(value);
 * @param value The value to be displayed
 */
	public void println(String value) {
		getWriter().println(value);
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
 * Displays the error message on the console.
 *
 * @usage console.showErrorMessage(msg);
 * @param msg The error msg to be displayed
 */
	public void showErrorMessage(String msg) {
		consoleModel.print(msg, ConsoleModel.ERROR_STYLE);
		consoleModel.print("\n", ConsoleModel.ERROR_STYLE);
	}

/* Method: readLine() */
/**
 * Reads and returns a line of input from the console, without
 * including the end-of-line characters that terminate the input.
 *
 * @usage String str = console.readLine();
 * @return The next line of input as a <code>String</code>
 */
	public final String readLine() {
		return readLine(null);
	}

/* Method: readLine(prompt) */
/**
 * Prompts the user to enter a line of text, which is then returned
 * as the value of this method.  The end-of-line characters that terminate
 * the input are not included in the returned string.
 *
 * @usage String str = console.readLine(prompt);
 * @param prompt The prompt string to display to the user
 * @return The next line of input as a <code>String</code>
 */
	public String readLine(String prompt) {
		if (prompt != null) print(prompt);
		consoleModel.requestFocus();
		try {
			String str = getReader().readLine();
			return str;
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/* Method: readInt() */
/**
 * Reads and returns an integer value from the user.  If the user types
 * a value that is not a legal integer, the method ordinarily offers the
 * user a chance to reenter the data, although this behavior can be
 * changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage int n = console.readInt();
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
 * @usage int n = console.readInt(low, high);
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
 * @usage int n = console.readInt(prompt);
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
		String msg = null;
		while (true) {
			String line = readLine(prompt);
			try {
				int n = Integer.parseInt(line);
				if (n >= low && n <= high) return n;
				msg = "Value is outside the range [" + low + ":" + high + "]";
			} catch (NumberFormatException ex) {
				msg = "Illegal numeric format";
			}
			showErrorMessage(msg);
			if (prompt == null) prompt = "Retry: ";
		}
	}

/* Method: readDouble() */
/**
 * Reads and returns a double-precision value from the user.  If the user
 * types a value that is not a legal number, the method ordinarily offers
 * the user a chance to reenter the data, although this behavior can be
 * changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage double d = console.readDouble();
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
 * @usage double d = console.readDouble(low, high);
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
 * @usage double d = console.readDouble(prompt);
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
 * @usage d = console.readDouble(prompt, low, high);
 * @param prompt The prompt string to display to the user
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a <code>double</code>
 */
	public double readDouble(String prompt, double low, double high) {
		String msg = null;
		while (true) {
			String line = readLine(prompt);
			try {
				double d = Double.valueOf(line).doubleValue();
				if (d >= low && d <= high) return d;
				msg = "Value is outside the range [" + low + ":" + high + "]";
			} catch (NumberFormatException ex) {
				msg = "Illegal numeric format";
			}
			showErrorMessage(msg);
			if (prompt == null) prompt = "Retry: ";
		}
	}

/* Method: readBoolean() */
/**
 * Reads and returns a boolean value from the user, which must match
 * either <code>true</code> or <code>false</code>, ignoring case.
 * If the user types a value that is not one of these possibilities,
 * the method ordinarily offers the user a chance to reenter the data,
 * although this behavior can be changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage boolean flag = console.readBoolean();
 * @return The value of the input interpreted as a boolean value
 */
	public final boolean readBoolean() {
		return readBoolean(null);
	}

/* Method: readBoolean(prompt) */
/**
 * Prompts the user to enter a boolean value, which is then returned as
 * the value of this method.  If the user types a value that is not a
 * legal boolean value, the method ordinarily offers the user a chance
 * to reenter the data, although this behavior can be changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage boolean flag = console.readBoolean(prompt);
 * @param prompt The prompt string to display to the user
 * @return The value of the input interpreted as a boolean value
 */
	public final boolean readBoolean(String prompt) {
		return readBoolean(prompt, "true", "false");
	}

/* Method: readBoolean(prompt, trueLabel, falseLabel) */
/**
 * Prompts the user to enter a value, which is interpreted as a boolean value
 * by matching it against the <code>trueLabel</code> and <code>falseLabel</code>
 * parameters.  If the user types a value that is not one of the two choices,
 * <code>readBoolean</code> ordinarily offers the user a chance
 * to reenter the data, although this behavior can be changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage boolean flag = console.readBoolean(prompt);
 * @param prompt The prompt string to display to the user
 * @param trueLabel The string used to indicate <code>true</code>
 * @param falseLabel The string used to indicate <code>false</code>
 * @return The value of the input interpreted as a boolean value
 */
	public boolean readBoolean(String prompt, String trueLabel, String falseLabel) {
		while (true) {
			String line = readLine(prompt);
			if (line ==  null) {
				throw new ErrorException("End of file encountered");
			} else if (line.equalsIgnoreCase(trueLabel)) {
				return true;
			} else if (line.equalsIgnoreCase(falseLabel)) {
				return false;
			} else {
				if (exceptionOnError) {
					throw new ErrorException("Illegal boolean format");
				}
				showErrorMessage("Illegal boolean format");
				if (prompt == null) prompt = "Retry: ";
			}
		}
	}

/* Method: getReader() */
/**
 * Returns a <code>BufferedReader</code> object that can be used to read
 * from the console.
 *
 * @usage BufferedReader rd = console.getReader();
 * @return A <code>BufferedReader</code> that reads from this console
 */
	public BufferedReader getReader() {
		if (reader == null) {
			reader = new BufferedReader(new ConsoleReader(consoleModel));
		}
		return reader;
	}

/* Method: getWriter() */
/**
 * Returns a <code>PrintWriter</code> object that can be used to send
 * output to the console.
 *
 * @usage PrintWriter wr = console.getWriter();
 * @return A <code>PrintWriter</code> that writes to this console
 */
	public PrintWriter getWriter() {
		if (writer == null) {
			writer = new PrintWriter(new ConsoleWriter(consoleModel));
		}
		return writer;
	}

/* Method: setExceptionOnError(flag) */
/**
 * Sets the error-handling mode of the console as specified by the <code>flag</code>
 * parameter.  If <code>flag</code> is <code>false</code> (which is the default), the
 * input methods give the user a chance to retry after erroneous input.  If this
 * value is set to <code>true</code>, illegal input raises an
 * <a href="../util/ErrorException.html"><code>ErrorException</code></a> instead.
 *
 * @usage console.setExceptionOnError(flag);
 * @param flag <code>false</code> to retry on errors; <code>true</code> to raise an exception
 */
	public void setExceptionOnError(boolean flag) {
		exceptionOnError = flag;
	}

/* Method: getExceptionOnError() */
/**
 * Returns the state of the error-handling flag.
 *
 * @usage boolean flag = console.getExceptionOnError();
 * @return The current setting of the error-handling mode (<code>false</code> to retry
 *         on errors; <code>true</code> to raise an exception)
 */
	public boolean getExceptionOnError() {
		return exceptionOnError;
	}

/* Method: setInputStyle(style) */
/**
 * Sets the style parameters for console input.  The style parameter
 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
 *
 * @usage console.setInputStyle(style);
 * @param style The style attributes to be used for console input
 * @noshow
 */
	public void setInputStyle(int style) {
		inputStyle = style;
		consoleModel.setInputStyle(style);
	}

/* Method: getInputStyle() */
/**
 * Returns the current style parameters for console input.
 *
 * @usage int style = console.getInputStyle();
 * @return The current input style
 * @noshow
 */
	public int getInputStyle() {
		return inputStyle;
	}

/* Method: setInputColor(color) */
/**
 * Sets the color used for console input.
 *
 * @usage console.setInputColor(color);
 * @param color The color used for console input
 * @noshow
 */
	public void setInputColor(Color color) {
		inputColor = color;
		consoleModel.setInputColor(color);
	}

/* Method: getInputColor() */
/**
 * Returns the color currently in use for console input.
 *
 * @usage Color color = console.getInputColor();
 * @return The current input color
 * @noshow
 */
	public Color getInputColor() {
		return inputColor;
	}

/* Method: setErrorStyle(style) */
/**
 * Sets the style parameters for console error messages.  The style parameter
 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
 *
 * @usage console.setErrorStyle(style);
 * @param style The style attributes to be used for console error messages
 * @noshow
 */
	public void setErrorStyle(int style) {
		errorStyle = style;
		consoleModel.setErrorStyle(style);
	}

/* Method: getErrorStyle() */
/**
 * Returns the current style parameters for console error messages.
 *
 * @usage int style = console.getErrorStyle();
 * @return The current error message style
 * @noshow
 */
	public int getErrorStyle() {
		return errorStyle;
	}

/* Method: setErrorColor(color) */
/**
 * Sets the color used for console error messages.
 *
 * @usage console.setErrorColor(color);
 * @param color The color used for console error messages
 * @noshow
 */
	public void setErrorColor(Color color) {
		errorColor = color;
		consoleModel.setErrorColor(color);
	}

/* Method: getErrorColor() */
/**
 * Returns the color currently in use for console error messages.
 *
 * @usage Color color = console.getErrorColor();
 * @return The current error message color
 * @noshow
 */
	public Color getErrorColor() {
		return errorColor;
	}

/* Method: setFont(str) */
/**
 * Sets the font used for the console as specified by
 * the string <code>str</code>, which is interpreted in the style of
 * <code>Font.decode</code>.  The usual format of the font string is
 *
 * <p>   <i>family</i><code>-</code><i>style</i><code>-</code><i>size</i><p>
 *
 * where both <i>style</i> and <i>size</i> are optional.  If any of these parts
 * are specified as an asterisk, the existing value is retained.
 *
 * @usage console.setFont(str);
 * @param str A <code>String</code> specifying the new font
 */
	public void setFont(String str) {
		setFont(JTFTools.decodeFont(str, getFont()));
	}

/* Method: setInputScript(rd) */
/**
 * Sets a new input script for the console, which will subsequently
 * take input from the specified reader.  When the input from the reader
 * has been exhausted, the console returns to its normal operation.  This
 * method is primarily useful for demonstrations and test suites, and is
 * not ordinarily invoked by students.
 *
 * @usage console.setInputScript(rd);
 * @param rd The reader from which console input is taken
 * @noshow
 */
	public void setInputScript(BufferedReader rd) {
		consoleModel.setInputScript(rd);
	}

/* Method: getInputScript() */
/**
 * Retrieves the input script.  After the end of the input script has been
 * reached, this method will return <code>null</code>.
 *
 * @usage BufferedReader rd = console.getInputScript();
 * @return The reader representing the current input script
 */
	public BufferedReader getInputScript() {
		return consoleModel.getInputScript();
	}

/* Method: cut() */
/**
 * Implements the "Cut" menu operation.
 *
 * @usage console.cut();
 * @noshow
 */
	public void cut() {
		consoleModel.cut();
	}

/* Method: copy() */
/**
 * Implements the "Copy" menu operation.
 *
 * @usage console.copy();
 * @noshow
 */
	public void copy() {
		consoleModel.copy();
	}

/* Method: paste() */
/**
 * Implements the "Paste" menu operation.
 *
 * @usage console.paste();
 * @noshow
 */
	public void paste() {
		consoleModel.paste();
	}

/* Method: selectAll() */
/**
 * Implements the "Select All" menu operation.
 *
 * @usage console.selectAll();
 * @noshow
 */
	public void selectAll() {
		consoleModel.selectAll();
	}

/* Method: save() */
/**
 * Implements the "Save" menu operation.
 *
 * @usage console.save();
 * @noshow
 */
	public void save() {
		Writer wr = null;
		while (wr == null) {
			try {
				if (file == null) {
					Frame frame = JTFTools.getEnclosingFrame(this);
					if (frame == null) return;
					String dir = System.getProperty("user.dir");
					FileDialog dialog = new FileDialog(frame, "Save Console As", FileDialog.SAVE);
					dialog.setDirectory(dir);
					dialog.setVisible(true);
					String filename = dialog.getFile();
					if (filename == null) return;
					file = new File(dialog.getDirectory(), filename);
				}
				wr = new FileWriter(file);
				save(wr);
				wr.close();
				Platform.setFileTypeAndCreator(file, "TEXT", "ttxt");
			} catch (IOException ex) {
				IODialog dialog = new IODialog(this);
				dialog.showErrorMessage(ex.getMessage());
			}
		}
	}

/* Method: saveAs() */
/**
 * Implements the "Save As" menu operation.
 *
 * @usage console.saveAs();
 * @noshow
 */
	public void saveAs() {
		file = null;
		save();
	}

/* Method: save(wr) */
/**
 * Copies the console output to the specified writer.
 *
 * @usage console.save(wr);
 * @param wr A <code>Writer</code> to which the console output is sent
 * @noshow
 */
	public void save(Writer wr) {
		try {
			wr.write(consoleModel.getText());
		} catch (IOException ex) {
			throw new ErrorException(ex);
		}
	}

/* Method: printConsole() */
/**
 * Implements the "Print Console" menu operation.
 *
 * @usage console.printConsole();
 * @noshow
 */
	public void printConsole() {
		Frame frame = JTFTools.getEnclosingFrame(this);
		if (frame == null) return;
		PrintJob pj = getToolkit().getPrintJob(frame, "Console", null);
		if (pj == null) return;
		printConsole(pj);
		pj.end();
	}

/* Method: printConsole(pj) */
/**
 * Prints the console output using the specified <code>PrintJob</code> object.
 * This method is usually invoked from the <code>Print</code> menu item in
 * the <code>ProgramMenuBar</code> class and is not ordinarily called by clients.
 *
 * @usage console.printConsole(pj);
 * @param pj <code>PrintJob</code> object to which the output is sent
 * @noshow
 */
	public void printConsole(PrintJob pj) {
		consoleModel.print(pj);
	}

/* Method: script() */
/**
 * Implements the "Script" menu operation.
 *
 * @usage console.script();
 * @noshow
 */
	public void script() {
		Frame frame = JTFTools.getEnclosingFrame(this);
		FileDialog dialog = new FileDialog(frame, "Input Script", FileDialog.LOAD);
		dialog.setDirectory(System.getProperty("user.dir"));
		dialog.setVisible(true);
		String dirname = dialog.getDirectory();
		String filename = dialog.getFile();
		if (filename != null) {
			try {
				FileReader rd = new FileReader(new File(new File(dirname), filename));
				setInputScript(new BufferedReader(rd));
			} catch (IOException ex) {
				throw new ErrorException(ex);
			}
		}
	}

/* Method: setMenuBar(mbar) */
/**
 * Sets the menu bar that controls this console.
 *
 * @usage console.setMenuBar(mbar);
 * @param mbar The menu bar
 */
	public void setMenuBar(ProgramMenuBar mbar) {
		menuBar = mbar;
		consoleModel.setMenuBar(mbar);
	}

/* Method: getMenuBar() */
/**
 * Returns the menu bar that controls this console.
 *
 * @usage ProgramMenuBar mbar = console.getMenuBar();
 * @return The menu bar
 */
	public ProgramMenuBar getMenuBar() {
		return menuBar;
	}

/* Method: menuAction(e) */
/**
 * Called whenever a relevant action is detected in the menu bar.
 * Subclasses can override this method to extend the set of menu
 * commands recognized by the console.
 */
	public boolean menuAction(ActionEvent e) {
		String cmd = e.getActionCommand();
		if (cmd.equals("Cut")) {
			cut();
			return true;
		} else if (cmd.equals("Copy")) {
			copy();
			return true;
		} else if (cmd.equals("Paste")) {
			paste();
			return true;
		} else if (cmd.equals("Select All")) {
			selectAll();
			return true;
		} else if (cmd.equals("Save")) {
			save();
			return true;
		} else if (cmd.equals("Save As")) {
			saveAs();
			return true;
		} else if (cmd.equals("Script")) {
			script();
			return true;
		} else if (cmd.equals("Print Console")) {
			printConsole();
			return true;
		}
		return false;
	}

/* Method: isConsoleMenuItem(item) */
/**
 * Returns <code>true</code> if the item is one that the console recognizes.
 */
	public boolean isConsoleMenuItem(JMenuItem item) {
		String cmd = item.getActionCommand();
		if (cmd == null) return false;
		for (int i = 0; i < CONSOLE_MENU_ACTIONS.length; i++) {
			if (cmd.equals(CONSOLE_MENU_ACTIONS[i])) return true;
		}
		return false;
	}

/* Method: updateMenuBar(mbar) */
/**
 * Updates the menu bar to enable the appropriate menu items.
 */
	public void updateMenuBar(ProgramMenuBar mbar) {
		Iterator<JMenuItem> iterator = mbar.iterator();
		while (iterator.hasNext()) {
			JMenuItem item = iterator.next();
			if (isConsoleMenuItem(item)) {
				item.setEnabled(true);
			} else {
				item.setEnabled(!mbar.isFocusedItem(item));
			}
		}
	}

/* Override method: setBackground(color) */
/**
 * Sets the background color used for the console.
 *
 * @usage console.setBackground(color);
 * @param color The new background color
 */
	public void setBackground(Color color) {
		Component textPane = consoleModel.getTextPane();
		if (textPane != null) textPane.setBackground(color);
		super.setBackground(color);
	}

/* Override method: setForeground(color) */
/**
 * Sets the foreground color used for the output text.
 *
 * @usage console.setForeground(color);
 * @param color The color to use for the output text
 */
	public void setForeground(Color color) {
		Component textPane = consoleModel.getTextPane();
		if (textPane != null) textPane.setForeground(color);
		super.setForeground(color);
	}

/* Override method: setFont(font) */
/**
 * Sets the font for the console.
 *
 * @usage console.setFont(font);
 * @param font The font to use for the console
 */
	public void setFont(Font font) {
		font = JTFTools.getStandardFont(font);
		Component textPane = consoleModel.getTextPane();
		if (textPane != null) textPane.setFont(font);
		super.setFont(font);
	}

/* Override method: requestFocus() */
/**
 * Overrides the <code>requestFocus</code> method so that it forwards to the
 * console model.
 *
 * @usage console.requestFocus();
 * @noshow
 */
	public void requestFocus() {
		consoleModel.requestFocus();
	}

/* Override method: getPreferredSize() */
/**
 * Overrides the <code>getPreferredSize</code> method to ensure that an
 * <code>IOConsole</code> is not too large.
 *
 * @usage Dimension size = console.getPreferredSize();
 * @noshow
 */
	public Dimension getPreferredSize() {
		return getMinimumSize();
	}

/* Override method: getMinimumSize() */
/**
 * Overrides the <code>getMinimumSize</code> method to ensure that an
 * <code>IOConsole</code> is not too large.
 *
 * @usage Dimension size = console.getMinimumSize();
 * @noshow
 */
	public Dimension getMinimumSize() {
		return new Dimension(MINIMUM_CONSOLE_WIDTH, MINIMUM_CONSOLE_HEIGHT);
	}

/* Protected method: createConsoleModel() */
/**
 * Creates the console model used by this console.
 */
	protected ConsoleModel createConsoleModel() {
		return new StandardConsoleModel();
	}

/* Protected constant: DEFAULT_FONT */
/**
 * The default font used by a new <code>IOConsole</code>.
 */
	protected static final Font DEFAULT_FONT = new Font("Monospaced", Font.PLAIN, 12);

/* Protected constant: LINE_SEPARATOR */
/**
 * The end-of-line separator for this platform.
 */
	protected static final String LINE_SEPARATOR = System.getProperty("line.separator");

/* Protected constant: MINIMUM_CONSOLE_WIDTH */
/**
 * The minimum width for a console.
 */
	protected static final int MINIMUM_CONSOLE_WIDTH = 50;

/* Protected constant: MINIMUM_CONSOLE_HEIGHT */
/**
 * The minimum height for a console.
 */
	protected static final int MINIMUM_CONSOLE_HEIGHT = 40;

/* Private constants */
	private static final String[] CONSOLE_MENU_ACTIONS = {
		"Save", "Save As", "Print Console", "Script",
		"Cut", "Copy", "Paste", "Select All"
	};

/* Private instance variables */
	private ConsoleModel consoleModel;
	private boolean exceptionOnError;
	private Color inputColor;
	private int inputStyle;
	private Color errorColor;
	private int errorStyle;
	private BufferedReader reader;
	private PrintWriter writer;
	private File file;
	private ProgramMenuBar menuBar;
}

/* Package interface: ConsoleModel */
/**
 * This interface defines the operations that any console model must implement.
 */
interface ConsoleModel {

/* Constants for display types */
	public static final int OUTPUT_STYLE = 0;
	public static final int INPUT_STYLE = 1;
	public static final int ERROR_STYLE = 2;

/* Method signatures */
	public void setConsole(IOConsole owner);
	public IOConsole getConsole();
	public void print(String str, int style);
	public String readLine();
	public void setInputScript(BufferedReader rd);
	public BufferedReader getInputScript();
	public void clear();
	public String getText();
	public String getText(int start, int end);
	public int getLength();
	public Component getConsolePane();
	public Component getTextPane();
	public void cut();
	public void copy();
	public void paste();
	public void selectAll();
	public boolean isPointSelection();
	public void print(PrintJob pj);
	public void setInputStyle(int style);
	public void setInputColor(Color color);
	public void setErrorStyle(int style);
	public void setErrorColor(Color color);
	public void requestFocus();
	public void setMenuBar(ProgramMenuBar mbar);
}

/* Package class: StandardConsoleModel */
/**
 * This class implements the console model using Swing.
 */
class StandardConsoleModel
  implements KeyListener, FocusListener, ConsoleModel {

/* Constructor: StandardConsoleModel() */
/**
 * Creates the framework for the standard console models.
 */
	public StandardConsoleModel() {
		outputMonitor = new ConsoleOutputMonitor(this);
		inputMonitor = new ConsoleInputMonitor(this);
		scrollPane = new JScrollPane(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS, JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
		textPane = new JTextPane();
		textPane.addKeyListener(this);
		textPane.addFocusListener(this);
		scrollPane.setViewportView(textPane);
		document = textPane.getDocument();
		lineSeparator = System.getProperty("line.separator");
		outputAttributes = new SimpleAttributeSet();
		inputAttributes = new SimpleAttributeSet();
		errorAttributes = new SimpleAttributeSet();
		buffer = new CharacterQueue();
		base = 0;
	}

/* Method: setConsole(owner) */
/**
 * Sets the identity of the owning console.  This method must be called after
 * creating the model object.
 */
	public void setConsole(IOConsole owner) {
		console = owner;
	}

/* Method: getConsole(console) */
/**
 * Returns the associated console.
 */
	public IOConsole getConsole() {
		return console;
	}

/* Method: print(str, style) */
/**
 * Prints the string to the console.
 */
	public void print(String str, int style) {
		outputMonitor.print(str, style);
	}

/* Method: readLine() */
/**
 * Reads and returns the next line of text from the console.
 */
	public String readLine() {
		return inputMonitor.readLine();
	}

/* Method: setInputScript(rd) */
/**
 * Sets a new input script for the console, which will subsequently
 * take input from the specified reader.
 */
	public void setInputScript(BufferedReader rd) {
		inputScript = rd;
		if (buffer.isWaiting()) {
			try {
				String line = inputScript.readLine();
				buffer.enqueue(line + "\n");
			} catch (IOException ex) {
				throw new ErrorException(ex);
			}
		}
	}

/* Method: getInputScript() */
/**
 * Retrieves the input script.  After the end of the input script has been
 * reached, this method will return <code>null</code>.
 */
	public BufferedReader getInputScript() {
		return inputScript;
	}

/* Method: clear() */
/**
 * Clears the console pane.
 */
	public void clear() {
		textPane.setText("");
		base = 0;
		buffer.clear();
	}

/* Method: getText() */
/**
 * Returns the text stored in the console model.
 */
	public String getText() {
		return textPane.getText();
	}

/* Method: getText(start, end) */
/**
 * Returns a substring from the text using endpoints defined as in
 * <code>substring</code> for strings.
 */
	public String getText(int start, int end) {
		try {
			return document.getText(start, end - start);
		} catch (BadLocationException ex) {
			throw new ErrorException(ex);
		}
	}

/* Method: getLength() */
/**
 * Returns the length of the text stored in the console model.
 */
	public int getLength() {
		return document.getLength();
	}

/* Method: getConsolePane() */
/**
 * Returns the top-level component that represents the console.  This is the
 * pane that needs to be added to a parent.
 */
	public Component getConsolePane() {
		return scrollPane;
	}

/* Method: getTextPane() */
/**
 * Returns the component that holds the text.  This is the pane to which
 * messages like <code>setFont</code> and <code>requestFocus</code> should
 * be directed.
 */
	public Component getTextPane() {
		return textPane;
	}

/* Method: cut() */
/**
 * Implements the "cut" menu operation.
 */
	public void cut() {
		copy();
		deleteSelection();
	}

/* Method: copy() */
/**
 * Implements the "copy" menu operation.
 */
	public void copy() {
		textPane.copy();
	}

/* Method: paste() */
/**
 * Implements the "paste" menu operation.
 */
	public void paste() {
		if (textPane.getSelectionEnd() != document.getLength()) return;
		int start = deleteSelection();
		textPane.setSelectionStart(start);
		textPane.paste();
		textPane.select(document.getLength(), document.getLength());
		if (document instanceof DefaultStyledDocument) {
			DefaultStyledDocument doc = (DefaultStyledDocument) document;
			doc.setCharacterAttributes(start, textPane.getSelectionEnd() - start, inputAttributes, true);
		}
	}

/* Method: selectAll() */
/**
 * Implements the "select all" menu operation.
 */
	public void selectAll() {
		textPane.selectAll();
	}

/* Method: isPointSelection() */
	public boolean isPointSelection() {
		return textPane.getSelectionStart() == textPane.getSelectionEnd();
	}

/* Method: print(pj) */
/**
 * Prints the entire console using the specified <code>PrintJob</code> object.
 */
	public void print(PrintJob pj) {
		Graphics g = pj.getGraphics();
		Dimension pageSize = pj.getPageDimension();
		FontMetrics fm = textPane.getFontMetrics(textPane.getFont());
		int fontHeight = fm.getHeight();
		int fontAscent = fm.getAscent();
		int x = PRINT_MARGIN;
		int y = PRINT_MARGIN + fontAscent;
		int linesPerPage = (pageSize.height - 2 * PRINT_MARGIN) / fontHeight;
		int linesRemaining = linesPerPage;
		ElementIterator i = new ElementIterator(document);
		while (true) {
			Element e = i.next();
			if (e == null) break;
			if (e.isLeaf()) {
				try {
					int len = e.getEndOffset() - e.getStartOffset();
					setStyleFromAttributes(g, e.getAttributes());
					fm = g.getFontMetrics();
					String text = document.getText(e.getStartOffset(), len);
					int start = 0;
					while (true) {
						boolean last = false;
						int delta = 1;
						int eol = text.indexOf("\n", start);
						if (eol == -1) {
							eol = text.indexOf(lineSeparator, start);
							if (eol == -1) {
								eol = text.length();
								last = true;
							} else {
								delta = lineSeparator.length();
							}
						}
						if (start < eol) {
							String str = text.substring(start, eol);
							g.drawString(str, x, y);
							x += fm.stringWidth(str);
						}
						if (last) break;
						start = eol + delta;
						x = PRINT_MARGIN;
						y += fontHeight;
						linesRemaining--;
						if (linesRemaining <= 0) {
							g.dispose();
							g = pj.getGraphics();
							y = PRINT_MARGIN + fontAscent;
							linesRemaining = linesPerPage;
						}
					}
				} catch (BadLocationException ex) {
					throw new ErrorException(ex);
				}
			}
		}
		pj.end();
	}

/* Method: setInputStyle(style) */
/**
 * Sets the style parameters for console input.  The style parameter
 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
 */
	public void setInputStyle(int style) {
		if (getLength() != 0) {
			throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
		}
		inputAttributes.addAttribute(StyleConstants.Bold, new Boolean((style & Font.BOLD) != 0));
		inputAttributes.addAttribute(StyleConstants.Italic, new Boolean((style & Font.ITALIC) != 0));
	}

/* Method: setInputColor(color) */
/**
 * Sets the color used for console input.
 */
	public void setInputColor(Color color) {
		if (getLength() != 0) {
			throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
		}
		inputAttributes.addAttribute(StyleConstants.Foreground, color);
	}

/* Method: setErrorStyle(style) */
/**
 * Sets the style parameters for console error messages.  The style parameter
 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
 */
	public void setErrorStyle(int style) {
		if (getLength() != 0) {
			throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
		}
		errorAttributes.addAttribute(StyleConstants.Bold, new Boolean((style & Font.BOLD) != 0));
		errorAttributes.addAttribute(StyleConstants.Italic, new Boolean((style & Font.ITALIC) != 0));
	}

/* Method: setErrorColor(color) */
/**
 * Sets the color used for console error.
 */
	public void setErrorColor(Color color) {
		if (getLength() != 0) {
			throw new ErrorException("Console styles and colors cannot be changed after I/O has started.");
		}
		errorAttributes.addAttribute(StyleConstants.Foreground, color);
	}

/* Method: requestFocus() */
/**
 * Forwards the request focus to the text pane.
 * @noshow
 */
	public void requestFocus() {
		if (textPane != null) textPane.requestFocus();
	}

/* Method: setMenuBar(mbar) */
/**
 * Sets the menu bar that controls this console.
 * @noshow
 */
	public void setMenuBar(ProgramMenuBar mbar) {
		menuBar = mbar;
	}

/* Implementation of the FocusListener interface */

/* Method: focusGained */
/**
 * Called when focus is gained by the console.
 */
	public void focusGained(FocusEvent e) {
		hasFocus = true;
		if (menuBar != null) {
			if (actionListener == null) {
				actionListener = new ConsoleActionListener(console);
			}
			menuBar.setFocusedListener(actionListener);
			console.updateMenuBar(menuBar);
		}
	}

/* Method: focusLost */
/**
 * Called when focus is lost by the console.
 */
	public void focusLost(FocusEvent e) {
		hasFocus = false;
		if (menuBar != null) {
			menuBar.setFocusedListener(null);
		}
	}

/* Implementation of the KeyListener interface */

/* Method: keyTyped(e) */
/**
 * Responds to a key being typed in the console pane. Implements <code>KeyListener</code>.
 */
	public void keyTyped(KeyEvent e) {
		if (!e.isMetaDown() && !e.isControlDown()) {
			buffer.enqueue(e.getKeyChar());
			e.consume();
		}
	}

/* Method: keyPressed(e) */
/**
 * Responds to key presses that do not correspond to typed keys.
 * Implements <code>KeyListener</code>.
 */
	public void keyPressed(KeyEvent e) {
		javax.swing.JOptionPane.showMessageDialog(null, "It works key! " + e.getKeyCode());
		switch (e.getKeyCode()) {
		  case KeyEvent.VK_LEFT:
			buffer.enqueue('\002');
			break;
		  case KeyEvent.VK_RIGHT:
			buffer.enqueue('\006');
			break;
		}
		if (menuBar != null) menuBar.fireAccelerator(e);
		e.consume();
	}

/* Method: keyReleased(e) */
/**
 * Responds to the release of a key.  Implements <code>KeyListener</code>.
 * @noshow
 */
	public void keyReleased(KeyEvent e) {
		e.consume();
	}

/* Protected method: printCallback(str, style) */
/**
 * Prints the string to the console.  Synchronization is provided by
 * the <code>ConsoleOutputMonitor</code> class.
 */
	protected void printCallback(String str, int style) {
		insert(str, base, style);
		base += str.length();
		setCaretPosition(base);
	}

/* Protected method: readLineCallback() */
/**
 * Reads and returns the next line of text from the console.
 * Synchronization is provided by the <code>ConsoleInputMonitor</code> class.
 */
	protected String readLineCallback() {
		base = getLength();
		if (inputScript != null) {
			String line = null;
			try {
				line = inputScript.readLine();
			} catch (IOException ex) {
				throw new ErrorException(ex);
			}
			if (line != null) {
				insert(line, base, INPUT_STYLE);
				insert("\n", base + line.length(), OUTPUT_STYLE);
				base += line.length() + 1;
				return line;
			}
			try {
				inputScript.close();
			} catch (IOException ex) {
				/* Empty */
			}
			inputScript = null;
		}
		int dot;
		char ch;
		setCaretPosition(base);
		while ((ch = buffer.dequeue()) != '\n' && ch != '\r') {
			if (getCaretPosition() < base) {
				setCaretPosition(getLength());
			}
			dot = getSelectionStart();
			switch (ch) {
			  case '\b': case '\177':
				if (dot == getSelectionEnd()) {
					if (dot > base) {
						delete(dot - 1, dot);
						dot--;
					}
				} else {
					dot = deleteSelection();
				}
				break;
			  case 'A'-'@':
				selectAll();
				dot = -1;
				break;
			  case 'B'-'@':
				dot = Math.max(getSelectionStart() - 1, base);
				break;
			  case 'C'-'@':
				copy();
				dot = -1;
				break;
			  case 'F'-'@':
				dot = Math.min(getSelectionEnd() + 1, getLength());
				break;
			  case 'P'-'@':
				console.printConsole();
				dot = -1;
				break;
			  case 'S'-'@':
				console.save();
				dot = -1;
				break;
			  case 'V'-'@':
				paste();
				dot = -1;
				break;
			  case 'X'-'@':
				cut();
				dot = -1;
				break;
			  default:
				if (dot != getSelectionEnd()) {
					dot = deleteSelection();
				}
				insert("" + ch, dot, INPUT_STYLE);
				dot++;
			}
			if (dot != -1) {
				select(dot, dot);
				setCaretPosition(dot);
			}
		}
		int len = getLength() - base;
		String line = getText(base, base + len);
		insert("\n", base + len, OUTPUT_STYLE);
		base += len + 1;
		return line;
	}

/* Private method: isCommandEnabled(cmd) */
/**
 * Returns true if the command should be enabled in the menu bar.
 */
	public boolean isCommandEnabled(String cmd) {
		if (cmd == cmd) /* Avoid unused parameter warning */;
		return hasFocus;
	}

/* Private method: insert(str, dot, style) */
/**
 * Inserts a string into the text pane at the position specified by <code>dot</code>,
 * using the specified style.
 */
	private void insert(String str, int dot, int style) {
		try {
			SimpleAttributeSet attributes = outputAttributes;
			switch (style) {
			  case INPUT_STYLE: attributes = inputAttributes; break;
			  case ERROR_STYLE: attributes = errorAttributes; break;
			}
			document.insertString(dot, str, attributes);
		} catch (BadLocationException ex) {
			/* Empty */
		}
	}

/* Private method: delete(p1, p2) */
/**
 * Deletes text from the text pane beginning at position <code>p1</code> and
 * continuing up to but not including <code>p2</code>.
 */
	private void delete(int p1, int p2) {
		try {
			document.remove(p1, p2 - p1);
		} catch (BadLocationException ex) {
			throw new ErrorException(ex);
		}
	}

/* Private method: setCaretPosition(pos) */
/**
 * Sets the position of the input carat.
 */
	private void setCaretPosition(int pos) {
		textPane.setCaretPosition(pos);
	}

/* Private method: getCaretPosition() */
/**
 * Returns the position of the input carat.
 */
	private int getCaretPosition() {
		return textPane.getCaretPosition();
	}

/* Private method: select(p1, p2) */
/**
 * Selects the characters in the range from p1 up to but not including p2.
 */
	private void select(int p1, int p2) {
		textPane.select(p1, p2);
	}

/* Private method: getSelectionStart() */
/**
 * Returns the start of the selection.
 */
	private int getSelectionStart() {
		return textPane.getSelectionStart();
	}

/* Private method: getSelectionEnd() */
/**
 * Returns the end of the selection.
 */
	private int getSelectionEnd() {
		return textPane.getSelectionEnd();
	}

/* Private method: deleteSelection() */
/**
 * Deletes the current selection and returns the index of the deletion point.
 */
	private int deleteSelection() {
		int start = Math.max(base, getSelectionStart());
		int end = getSelectionEnd();
		if (end <= base) return getLength();
		delete(start, end);
		return start;
	}

/* Private method: setStyleFromAttributes(g, attributes) */
/**
 * Sets the relevant components of the graphics context from the attribute set.
 */
	private void setStyleFromAttributes(Graphics g, AttributeSet attributes) {
		Font oldFont = textPane.getFont();
		int style = 0;
		if (Boolean.TRUE.equals(attributes.getAttribute(StyleConstants.Bold))) {
			style |= Font.BOLD;
		}
		if (Boolean.TRUE.equals(attributes.getAttribute(StyleConstants.Italic))) {
			style |= Font.ITALIC;
		}
		g.setFont(new Font(oldFont.getName(), style, oldFont.getSize()));
		Color color = (Color) attributes.getAttribute(StyleConstants.Foreground);
		if (color == null) color = textPane.getForeground();
		g.setColor(color);
	}

/* Private constants */
	private static final int PRINT_MARGIN = 36;

/* Private instance variables */
	private ActionListener actionListener;
	private ConsoleOutputMonitor outputMonitor;
	private ConsoleInputMonitor inputMonitor;
	private BufferedReader inputScript;
	private CharacterQueue buffer;
	private SimpleAttributeSet outputAttributes;
	private SimpleAttributeSet inputAttributes;
	private SimpleAttributeSet errorAttributes;
	private JScrollPane scrollPane;
	private JTextPane textPane;
	private Document document;
	private String lineSeparator;
	private int base;
	private boolean hasFocus;
	private IOConsole console;
	private ProgramMenuBar menuBar;
}

/* Package class: ConsoleOutputMonitor */
/**
 * This class implements synchronization for the output side of the console
 * using method-level synchronization, thereby avoiding the compatibility
 * problems of the <code>synchronized</code> statement.
 */
class ConsoleOutputMonitor {

/* Constructor: new ConsoleOutputMonitor(model) */
/**
 * Creates the new console output monitor, which calls back to the specified
 * model.
 */
	public ConsoleOutputMonitor(StandardConsoleModel model) {
		consoleModel = model;
	}

/* Method: print(str, style) */
/**
 * Prints the string to the console.
 */
	public synchronized void print(String str, int style) {
		consoleModel.printCallback(str, style);
	}

/* Private instance variables */
	private StandardConsoleModel consoleModel;
}

/* Package class: ConsoleInputMonitor */
/**
 * This class implements synchronization for the input side of the console
 * using method-level synchronization, thereby avoiding the compatibility
 * problems of the <code>synchronized</code> statement.
 */
class ConsoleInputMonitor {

/* Constructor: new ConsoleInputMonitor(model) */
/**
 * Creates the new console input monitor, which calls back to the specified
 * model.
 */
	public ConsoleInputMonitor(StandardConsoleModel model) {
		consoleModel = model;
	}

/* Method: readLine() */
/**
 * Reads and returns the next line of text from the console.
 */
	public synchronized String readLine() {
		return consoleModel.readLineCallback();
	}

/* Private instance variables */
	private StandardConsoleModel consoleModel;
}

/* Package class: SystemConsole */
/**
 * This class implements a version of <code>IOConsole</code> that responds to
 * the methods of that class but that uses <code>System.in</code> and
 * <code>System.out</code> for its input and output.  This class is not
 * instantiated directly; clients gain access to this facility by using the
 * <a href="IOConsole.html#SYSTEM_CONSOLE"><code>SYSTEM_CONSOLE</code></a>
 * constant in the <a href="IOConsole.html"><code>IOConsole</code></a> class.
 */
class SystemConsole extends IOConsole {

/* Factory method: createConsoleModel() */
/**
 * Creates the console model object used to implement this console.
 */
	protected ConsoleModel createConsoleModel() {
		return new SystemConsoleModel();
	}
}

/* Package class: ConsoleWriter */
/**
 * This class defines the underlying writer for the console.
 * @noshow
 */
class ConsoleWriter extends Writer {

/* Constructor: ConsoleWriter(cp) */
/**
 * Creates the basic writer object for the console.
 */
	public ConsoleWriter(ConsoleModel cp) {
		consoleModel = cp;
	}

/* Method: close() */
/**
 * Closes the writer (does nothing in this case).
 */
	public void close() {
		/* Empty */
	}

/* Method: flush() */
/**
 * Flushes any remaining output to the writer.
 */
	public void flush() {
		/* Empty */
	}

/* Method: write(cbuf, off, len) */
/**
 * Writes characters from <code>cbuf</code> starting at position <code>off</code>
 * up to a maximum of <code>len</code>.
 */
	public void write(char[] cbuf, int off, int len) {
		String str = new String(cbuf, off, len);
		int start = 0;
		int eol;
		while ((eol = str.indexOf(IOConsole.LINE_SEPARATOR, start)) != -1) {
			consoleModel.print(str.substring(start, eol), ConsoleModel.OUTPUT_STYLE);
			consoleModel.print("\n", ConsoleModel.OUTPUT_STYLE);
			start = eol + IOConsole.LINE_SEPARATOR.length();
		}
		consoleModel.print(str.substring(start), ConsoleModel.OUTPUT_STYLE);
	}

/* Private instance variables */
	private ConsoleModel consoleModel;
}

/* Package class: ConsoleReader */
/**
 * This class defines the underlying reader for the console.
 */
class ConsoleReader extends Reader {

/* Constructor: ConsoleReader(cp) */
/**
 * Creates the basic reader object for the console.
 */
	public ConsoleReader(ConsoleModel cp) {
		consoleModel = cp;
		buffer = null;
	}

/* Method: close() */
/**
 * Closes the reader (does nothing in this case).
 */
	public void close() {
		/* Empty */
	}

/* Method: read(cbuf, off, len) */
/**
 * Reads characters into <code>cbuf</code> starting at position <code>off</code>
 * up to a maximum of <code>len</code>.
 */
	public int read(char[] cbuf, int off, int len) {
		if (len == 0) return 0;
		if (buffer == null) {
			buffer = consoleModel.readLine();
			if (buffer == null) return -1;
			buffer += "\n";
		}
		if (len < buffer.length()) {
			buffer.getChars(0, len, cbuf, off);
			buffer = buffer.substring(len);
		} else {
			len = buffer.length();
			buffer.getChars(0, len, cbuf, off);
			buffer = null;
		}
		return len;
	}

/* Private instance variables */
	private ConsoleModel consoleModel;
	private String buffer;
}

/* Package class: SystemConsoleModel */
/**
 * Implements the basic console operations for the system console.
 */
class SystemConsoleModel implements ConsoleModel {

/* Constructor: SystemConsoleModel() */
/**
 * Creates a new <code>SystemConsoleModel</code> object.
 */
	public SystemConsoleModel() {
		text = "";
	}

/* Method: setConsole(owner) */
/**
 * Sets the identity of the owning console.  This method must be called after
 * creating the model object.
 */
	public void setConsole(IOConsole owner) {
		console = owner;
	}

/* Method: getConsole(console) */
/**
 * Returns the associated console.
 */
	public IOConsole getConsole() {
		return console;
	}

/* Method: clear() */
/**
 * Clears the console pane.
 */
	public void clear() {
		/* Empty */
	}

/* Method: print(str, style) */
/**
 * Prints the string to the end of the console output.
 */
	public void print(String str, int style) {
		System.out.print(str);
		text += str;
	}

/* Method: readLine() */
/**
 * Reads and returns the next line of text from the console.
 */
	public String readLine() {
		System.out.flush();
		String line = "";
		int ch;
		try {
			while (true) {
				if (inputScript == null) {
					ch = System.in.read();
				} else {
					ch = inputScript.read();
				}
				if (ch == -1 && line.length() == 0) {
					try {
						if (inputScript != null) inputScript.close();
					} catch (IOException ex) {
						/* Empty */
					}
					inputScript = null;
				} else {
					if (ch == -1 || ch == '\n') break;
					line += (char) ch;
				}
			}
		} catch (IOException ex) {
			/* Empty */
		}
		if (inputScript != null) print(line + "\n", INPUT_STYLE);
		return line;
	}

/* Method: getText() */
/**
 * Returns the text stored in the console pane.
 */
	public String getText() {
		return text;
	}

/* Method: getText(start, end) */
/**
 * Returns a substring from the text using endpoints defined as in
 * <code>substring</code> for strings.
 */
	public String getText(int start, int end) {
		return text.substring(start, end);
	}

/* Method: getLength() */
/**
 * Returns the length of the text.
 */
	public int getLength() {
		return text.length();
	}

/* Method: getConsoleModel() */
/**
 * Returns the top-level component that represents the console.  This is the
 * pane that needs to be added to a parent.
 */
	public Component getConsoleModel() {
		return null;
	}

/* Method: getTextPane() */
/**
 * Returns the component that holds the text.  This is the model to which
 * messages like <code>setFont</code> and <code>requestFocus</code> should
 * be directed.
 */
	public Component getTextPane() {
		return null;
	}

/* Method: setFont(font) */
/**
 * Sets the font for the console.
 *
 * @usage consoleModel.setFont(font);
 * @param font The new font for the console
 */
	public void setFont(Font font) {
		if (font != font); /* Avoid unused parameter warning */;
		/* Empty */
	}

/* Method: setInputStyle(style) */
/**
 * Sets the style parameters for console input.  The style parameter
 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
 */
	public void setInputStyle(int style) {
		/* Empty */
	}

/* Method: setInputColor(color) */
/**
 * Sets the color used for console input.
 */
	public void setInputColor(Color color) {
		/* Empty */
	}

/* Method: setErrorStyle(style) */
/**
 * Sets the style parameters for console error messages.  The style parameter
 * is either <code>Font.PLAIN</code> or a sum of one or more of the attributes
 * <code>Font.BOLD</code> and <code>Font.ITALIC</code>.
 */
	public void setErrorStyle(int style) {
		/* Empty */
	}

/* Method: setErrorColor(color) */
/**
 * Sets the color used for console error messages.
 */
	public void setErrorColor(Color color) {
		/* Empty */
	}

/* Method: cut() */
/**
 * Implements the "cut" menu operation.
 */
	public void cut() {
		/* Empty */
	}

/* Method: copy() */
/**
 * Implements the "copy" menu operation.
 */
	public void copy() {
		/* Empty */
	}

/* Method: paste() */
/**
 * Implements the "paste" menu operation.
 */
	public void paste() {
		/* Empty */
	}

/* Method: selectAll() */
/**
 * Implements the "select all" menu operation.
 */
	public void selectAll() {
		/* Empty */
	}

/* Method: isPointSelection() */
	public boolean isPointSelection() {
		return true;
	}

/* Method: print(pj) */
/**
 * Prints the entire console using the specified <code>PrintJob</code> object.
 */
	public void print(PrintJob pj) {
		/* Empty */
	}

/* Method: setInputScript(rd) */
/**
 * Sets a new input script for the console, which will subsequently
 * take input from the specified reader.
 */
	public void setInputScript(BufferedReader rd) {
		inputScript = rd;
	}

/* Method: getInputScript() */
/**
 * Retrieves the input script.  After the end of the input script has been
 * reached, this method will return <code>null</code>.
 */
	public BufferedReader getInputScript() {
		return inputScript;
	}

/* Method: getConsolePane() */
/**
 * Returns the top-level component that represents the console.
 */
	public Component getConsolePane() {
		return null;
	}

/* Method: requestFocus() */
/**
 * Forwards the request focus to the text pane.
 * @noshow
 */
	public void requestFocus() {
		/* Empty */
	}

/* Method: setMenuBar(mbar) */
/**
 * Sets the menu bar that controls this console.
 * @noshow
 */
	public void setMenuBar(ProgramMenuBar mbar) {
		/* Empty */
	}

/* Private instance variables */
	private IOConsole console;
	private BufferedReader inputScript;
	private String text;
}

/* Package class: CharacterQueue */
/**
 * This class defines a simple character queue.
 */
class CharacterQueue {

/* Constructor: CharacterQueue() */
/**
 * Creates an empty character queue.
 */
	public CharacterQueue() {
		buffer = "";
	}

/* Method: enqueue(ch) */
/**
 * Adds a character to the end of the queue.
 */
	public synchronized void enqueue(char ch) {
		buffer += ch;
		notifyAll();
	}

/* Method: enqueue(str) */
/**
 * Adds a string to the end of the queue.
 */
	public synchronized void enqueue(String str) {
		buffer += str;
		notifyAll();
	}

/* Method: dequeue() */
/**
 * Removes and returns the first character in the queue.  If the queue
 * is empty, this method waits for data.
 */
	public synchronized char dequeue() {
		while (buffer.length() == 0) {
			try {
				isWaiting = true;
				wait();
				isWaiting = false;
			} catch (InterruptedException ex) {
				/* Empty */
			}
		}
		char ch = buffer.charAt(0);
		buffer = buffer.substring(1);
		return ch;
	}

/* Method: clear() */
/**
 * Clears the character queue.
 */
	public synchronized void clear() {
		buffer = "";
		notifyAll();
	}

/* Method: isWaiting() */
/**
 * Returns <code>true</code> if the buffer is in a waiting state.
 */
	public boolean isWaiting() {
		return isWaiting;
	}

/* Private instance variables */
	private String buffer;
	private boolean isWaiting;
}

/* Package class: ConsoleActionListener */
/**
 * This class listens for menu bar actions directed toward the console.
 */
class ConsoleActionListener implements ActionListener {

/* Constructor: ConsoleActionListener(owner) */
/**
 * Creates a new action listener for the specified console.
 */
	public ConsoleActionListener(IOConsole owner) {
		console = owner;
	}

/* Method: actionPerformed(e) */
/**
 * Responds to the specified action event.
 */
	public void actionPerformed(ActionEvent e) {
		console.menuAction(e);
	}

/* Private instance variables */
	private IOConsole console;

}
