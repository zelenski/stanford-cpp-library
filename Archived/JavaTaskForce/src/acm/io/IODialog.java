/*
 * @(#)IODialog.java   1.99.1 08/12/08
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

package acm.io;

import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.reflect.*;
import java.util.*;
import javax.swing.*;

/* Class: IODialog */
/**
 * The <code>IODialog</code> class provides a simple, dialog-based mechanism
 * for interacting with the user.  It is therefore similar to the
 * <code>JOptionPane</code> facility in Swing (which it uses in the implementation).
 * The differences between the models are
 *
 * <ol>
 * <li>The <code>IODialog</code> mechanism is considerably simpler, mostly
 *     because it does not try to be as general.
 * <li>The <code>IODialog</code> mechanism does not use static methods; clients
 *     instantiate an <code>IODialog</code> object and make calls on that object,
 *     thereby emphasizing the object-oriented idea.
 * <li>The <code>IODialog</code> class works even if Swing is not available.
 * </ol>
 *
 * The input methods available for <code>IODialog</code> are intentionally the
 * same as those for the <a href="IOConsole.html"><code>IOConsole</code></a> class so
 * that clients can substitute one model for another without making source changes.
 * The input methods are therefore
 *
 * <ul>
 * <li><a href="#readInt()"><code>readInt</code></a>
 * <li><a href="#readDouble()"><code>readDouble</code></a>
 * <li><a href="#readBoolean()"><code>readBoolean</code></a>
 * <li><a href="#readLine()"><code>readLine</code></a>
 * </ul>
 *
 * For example, the following code pops up a dialog box and
 * allows the user to enter an integer:
 *
 * <p><pre><code>
 * &nbsp;    IODialog dialog = new IODialog();
 * &nbsp;    int n = dialog.readInt("Enter an integer: ");
 * </code></pre>
 */
public class IODialog implements IOModel {

/* Constructor: IODialog() */
/**
 * Instantiates a new IODialog object that can then be used for dialog-based
 * input and output.
 *
 * @usage dialog = new IODialog();
 */
	public IODialog() {
		this(null);
	}

/* Constructor: IODialog(owner) */
/**
 * Instantiates a new IODialog object that can then be used for dialog-based
 * input and output.
 *
 * @usage dialog = new IODialog(owner);
 * @param owner A <code>Component</code> used as the owner of created dialogs
 */
	public IODialog(Component owner) {
		myComponent = owner;
		model = createModel();
		outputLine = "";
		exceptionOnError = false;
		allowCancel = false;
	}

/* Method: print(value) */
/**
 * Displays the argument value, allowing for the possibility of more
 * output in the same dialog.  The <code>print</code> method is overloaded
 * so that <code>value</code> can be of any type.
 *
 * @usage dialog.print(value);
 * @param value The value to be displayed
 */
	public void print(String value) {
		outputLine += value;
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
 * Completes the output line and displays the dialog.
 *
 * @usage dialog.println();
 */
	public void println() {
		model.popupMessage(outputLine);
		outputLine = "";
	}

/* Method: println(value) */
/**
 * Adds the value to the current output line and then displays the dialog.
 * The <code>println</code> method is overloaded so that <code>value</code>
 * can be of any type.
 *
 * @usage dialog.println(value);
 * @param value The value to be displayed
 */
	public void println(String value) {
		print(value);
		println();
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
 * Displays the error message in an error dialog.
 *
 * @usage dialog.showErrorMessage(msg);
 * @param msg The error msg to be displayed
 */
	public void showErrorMessage(String msg) {
		model.popupErrorMessage(msg);
	}

/* Method: readLine() */
/**
 * Reads and returns a line of input from the dialog, without
 * including the end-of-line characters that terminate the input.
 *
 * @usage String str = dialog.readLine();
 * @return The next line of input as a <code>String</code>
 */
	public final String readLine() {
		return readLine(null);
	}

/* Method: readLine(prompt) */
/**
 * Prompts the user to enter a single character, which is then returned
 * as the value of this method.  The end-of-line characters that terminate
 * the input are not included in the returned string.
 *
 * @usage String str = dialog.readLine(prompt);
 * @param prompt The prompt string to display to the user
 * @return The next line of input as a <code>String</code>
 */
	public String readLine(String prompt) {
		if (myConsole != null && myConsole.getInputScript() != null) {
			return myConsole.readLine(prompt);
		}
		prompt = (prompt == null) ? outputLine : outputLine + prompt;
		outputLine = "";
		String line;
		while ((line = model.popupLineInputDialog(prompt, allowCancel)) == null) {
			if (allowCancel) throw new CancelledException();
		}
		return line;
	}

/* Method: readInt() */
/**
 * Reads and returns an integer value from the user.  If the user types
 * a value that is not a legal integer, the method ordinarily offers the
 * user a chance to reenter the data, although this behavior can be
 * changed using the
 * <a href="#setExceptionOnError(boolean)"><code>setExceptionOnError</code></a> method.
 *
 * @usage int n = dialog.readInt();
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
 * @usage int n = dialog.readInt(low, high);
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
 * @usage int n = dialog.readInt(prompt);
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
 * @usage int n = dialog.readInt(prompt, low, high);
 * @param prompt The prompt string to display to the user
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a decimal integer
 */
	public int readInt(String prompt, int low, int high) {
		while (true) {
			String line = readLine(prompt);
			try {
				int n = Integer.parseInt(line);
				if (n < low || n > high) {
					signalError("Value is outside the range [" + low + ":" + high + "]");
				}
				return n;
			} catch (NumberFormatException ex) {
				signalError("Illegal integer format");
			}
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
 * @usage double d = dialog.readDouble();
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
 * @usage double d = dialog.readDouble(low, high);
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
 * @usage double d = dialog.readDouble(prompt);
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
 * @usage d = dialog.readDouble(prompt, low, high);
 * @param prompt The prompt string to display to the user
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a <code>double</code>
 */
	public double readDouble(String prompt, double low, double high) {
		while (true) {
			String line = readLine(prompt);
			try {
				double d = Double.valueOf(line).doubleValue();
				if (d < low || d > high) {
					signalError("Value is outside the range [" + low + ":" + high + "]");
				}
				return d;
			} catch (NumberFormatException ex) {
				signalError("Illegal numeric format");
			}
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
 * @usage boolean flag = dialog.readBoolean();
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
 * @usage boolean flag = dialog.readBoolean(prompt);
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
 * @usage boolean flag = dialog.readBoolean(prompt);
 * @param prompt The prompt string to display to the user
 * @param trueLabel The label for the button indicating <code>true</code>
 * @param falseLabel The label for the button indicating <code>false</code>
 * @return The value of the input interpreted as a boolean value
 */
	public boolean readBoolean(String prompt, String trueLabel, String falseLabel) {
		if (myConsole != null && myConsole.getInputScript() != null) {
			return myConsole.readBoolean(prompt, trueLabel, falseLabel);
		}
		prompt = (prompt == null) ? outputLine : outputLine + prompt;
		outputLine = "";
		Boolean choice;
		while ((choice = model.popupBooleanInputDialog(prompt, trueLabel, falseLabel, allowCancel)) == null) {
			if (allowCancel) throw new CancelledException();
		}
		return choice.booleanValue();
	}

/* Method: setExceptionOnError(flag) */
/**
 * Sets the error-handling mode of the dialog as specified by the <code>flag</code>
 * parameter.  If <code>flag</code> is <code>false</code> (which is the default), the
 * input methods give the user a chance to retry after erroneous input.  If this
 * value is set to <code>true</code>, illegal input raises an
 * <a href="../util/ErrorException.html"><code>ErrorException</code></a> instead.
 *
 * @usage dialog.setExceptionOnError(flag);
 * @param flag <code>false</code> to retry on errors; <code>true</code> to raise an exception
 */
	public void setExceptionOnError(boolean flag) {
		exceptionOnError = flag;
	}

/* Method: getExceptionOnError() */
/**
 * Returns the state of the error-handling flag.
 *
 * @usage boolean flag = dialog.getExceptionOnError();
 * @return The current setting of the error-handling mode (<code>false</code> to retry
 *         on errors; <code>true</code> to raise an exception)
 */
	public boolean getExceptionOnError() {
		return exceptionOnError;
	}

/* Method: setAllowCancel(flag) */
/**
 * Sets the cancellation mode of the dialog as specified by the <code>flag</code>
 * parameter.  If <code>flag</code> is <code>false</code> (which is the default),
 * the input methods do not include a "Cancel" button.  If this value is set to
 * to <code>true</code>, a "Cancel" button appears, which throws a
 * <a href="../util/CancelledException.html"><code>CancelledException</code></a>
 * if it is invoked.
 *
 * @usage dialog.setAllowCancel(flag);
 * @param flag <code>false</code> to remove "Cancel" button; <code>true</code> to enable it
 */
	public void setAllowCancel(boolean flag) {
		allowCancel = flag;
	}

/* Method: getAllowCancel() */
/**
 * Returns the state of the cancellation mode.
 *
 * @usage boolean flag = dialog.getAllowCancel();
 * @return The current setting of the error-handling mode (<code>false</code> to retry
 *         on errors; <code>true</code> to raise an exception)
 */
	public boolean getAllowCancel() {
		return allowCancel;
	}

/* Method: setAssociatedConsole(console) */
/**
 * Establishes an associated console for the dialog. If the associated console
 * is non-<code>null</code> and the console is reading from an input stream,
 * then the dialog methods use the console for input instead of the dialog.
 *
 * @usage dialog.setAssociatedConsole(console);
 * @param console The associated console
 * @noshow
 */
	public void setAssociatedConsole(IOConsole console) {
		myConsole = console;
	}

/* Method: getAssociatedConsole() */
/**
 * Returns the associated console for the dialog.
 *
 * @usage IOConsole console = dialog.getAssociatedConsole();
 * @return The associated console
 * @noshow
 */
	public IOConsole getAssociatedConsole() {
		return myConsole;
	}

/* Factory method: createModel() */
/**
 * Creates the input dialog model in Swing or AWT style, as appropriate.
 */
	protected DialogModel createModel() {
		String className = new JPanel().getClass().getName();
		if (className.startsWith("javax.swing.") && Platform.isSwingAvailable()) {
			try {
				Class<?> swingDialogModelClass = Class.forName("acm.io.SwingDialogModel");
				Class[] types = { Class.forName("java.awt.Component") };
				Object[] args = { myComponent };
				Constructor<?> con = swingDialogModelClass.getConstructor(types);
				return (DialogModel) con.newInstance(args);
			} catch (Exception ex) {
				return new AWTDialogModel(myComponent);
			}
		} else {
			return new AWTDialogModel(myComponent);
		}
	}

/* Private method: signalError(msg) */
/**
 * Signals that an error occurred.  If <code>exceptionOnError</code> is <code>true</code>,
 * this method raises an exception with the specified message.  If not, this method
 * displays an error dialog and returns.
 */
	private void signalError(String msg) {
		if (exceptionOnError) throw new ErrorException(msg);
		model.popupErrorMessage(msg);
	}

/* Private instance variables */
	private boolean exceptionOnError;
	private boolean allowCancel;
	private DialogModel model;
	private Component myComponent;
	private IOConsole myConsole;
	private String outputLine;
}

/* Package interface: DialogModel */
/**
 * This interface describes the operations that a dialog must implement
 * to be used in the IODialog package.
 */
interface DialogModel {

/* Method: popupMessage(msg) */
/**
 * Pops up an informational message.
 */
	public void popupMessage(String msg);

/* Method: popupErrorMessage(msg) */
/**
 * Pops up an error message.
 */
	public void popupErrorMessage(String msg);

/* Method: popupLineInputDialog(prompt, allowCancel) */
/**
 * Pops up an dialog box to read a line.  Clicking the Cancel box (if present)
 * returns <code>null</code>.
 */
	public String popupLineInputDialog(String prompt, boolean allowCancel);

/* Method: popupBooleanInputDialog(prompt, trueLabel, falseLabel, allowCancel) */
/**
 * Pops up an dialog box to read a binary choice.  Clicking the Cancel box (if present)
 * returns <code>null</code>.
 */
	public Boolean popupBooleanInputDialog(String prompt, String trueLabel, String falseLabel,
	                                       boolean allowCancel);
}

/* Package class: SwingDialogModel */
/**
 * This class implements the <code>DialogModel</code> interface using
 * the Swing <code>JOptionPane</code> mechanism.
 */
class SwingDialogModel implements DialogModel {

/* Constructor: SwingDialogModel */
/**
 * Initializes the class.
 */
	public SwingDialogModel(Component owner) {
		myComponent = owner;
		if (!iconTested) {
			String lf = UIManager.getLookAndFeel().toString();
			if (lf.indexOf("AquaLookAndFeel") == -1) {
				inputIcon = null;
			} else {
				inputIcon = new ImageIcon(MediaTools.createImage(AQUA_QUESTION_IMAGE));
			}
			iconTested = true;
		}
	}

/* Method: popupMessage(msg) */
/**
 * Pops up an informational message.
 */
	public void popupMessage(String msg) {
		JOptionPane.showMessageDialog(myComponent, msg);
	}

/* Method: popupErrorMessage(msg) */
/**
 * Pops up an error message.
 */
	public void popupErrorMessage(String msg) {
		JOptionPane.showMessageDialog(myComponent, msg, "Error", JOptionPane.ERROR_MESSAGE);
	}

/* Method: popupLineInputDialog(prompt, allowCancel) */
/**
 * Pops up an dialog box to read a line.  Clicking the Cancel box
 * returns <code>null</code>.
 */
	public String popupLineInputDialog(String prompt, boolean allowCancel) {
		JOptionPane pane;
		if (allowCancel) {
			pane = new JOptionPane(prompt, JOptionPane.QUESTION_MESSAGE, JOptionPane.OK_CANCEL_OPTION,
			                       inputIcon);
			pane.setWantsInput(true);
		} else {
			Object[] choices = { "OK" };
			pane = new JOptionPane(prompt, JOptionPane.QUESTION_MESSAGE, JOptionPane.OK_CANCEL_OPTION,
				                   inputIcon, choices, choices[0]);
			pane.setWantsInput(true);
			pane.setInputValue("");
		}
		JDialog dialog = pane.createDialog(myComponent, "Input");
		dialog.setVisible(true);
		Object value = pane.getInputValue();
		if (value == JOptionPane.UNINITIALIZED_VALUE) return null;
		return (String) value;
	}

/* Method: popupBooleanInputDialog(prompt, trueLabel, falseLabel, allowCancel) */
/**
 * Pops up an dialog box to read a binary choice.  Clicking the Cancel box (if present)
 * returns <code>null</code>.
 */
	public Boolean popupBooleanInputDialog(String prompt, String trueLabel, String falseLabel,
	                                       boolean allowCancel) {
		Object[] choices;
		if (allowCancel) {
			choices = new Object[3];
			choices[2] = "Cancel";
		} else {
			choices = new Object[2];
		}
		choices[0] = trueLabel;
		choices[1] = falseLabel;
		String label = trueLabel + "/" + falseLabel + " question";
		int choice = JOptionPane.showOptionDialog(myComponent, prompt, label,
		                                          JOptionPane.DEFAULT_OPTION,
		                                          JOptionPane.QUESTION_MESSAGE,
		                                          inputIcon, choices, choices[0]);
		switch (choice) {
		  case 0: return Boolean.TRUE;
		  case 1: return Boolean.FALSE;
		  default: return null;
		}
	}

/* Static image to repair question mark bug in Aqua look and feel */
	private static final String[] AQUA_QUESTION_IMAGE = {
		"4749463839613D003D00E60000FFFFFFE5E5E5FFCBFF8C8C8CA7A7A7ADADAD8D8D8D929292818181",
		"8585856F6F6F7373738888888B8B8B9A9A9A9D9D9D616161646464757575787878C2C2C295959597",
		"97976666666868687E7E7E808080565656575757E6E6E6E7E7E79F9F9FA0A0A05E5E5E5F5F5F6B6B",
		"6B6C6C6C6D6D6D6E6E6E7979797A7A7A7C7C7C7D7D7D5A5A5A5B5B5B5C5C5C5D5D5DE1E1E1E2E2E2",
		"E3E3E3E4E4E4AEAEAEAFAFAFB0B0B0B1B1B1E8E8E8E9E9E9EAEAEAEBEBEBECECECEDEDEDEEEEEEEF",
		"EFEFB2B2B2B3B3B3B4B4B4B5B5B5B6B6B6B7B7B7B8B8B8B9B9B9BABABABBBBBBBCBCBCBDBDBDBEBE",
		"BEBFBFBFC0C0C0C1C1C1F0F0F0F1F1F1F2F2F2F3F3F3F4F4F4F5F5F5F6F6F6F7F7F7F8F8F8F9F9F9",
		"FAFAFAFBFBFBFCFCFCFDFDFDFEFEFED1D1D1D2D2D2D3D3D3D4D4D4D5D5D5D6D6D6D7D7D7D8D8D8D9",
		"D9D9DADADADBDBDBDCDCDCDDDDDDDEDEDEDFDFDFE0E0E0C9C9C9CACACACBCBCBCCCCCCCDCDCDCECE",
		"CECFCFCFD0D0D0C5C5C5C6C6C6C7C7C7C8C8C8C3C3C3C4C4C4000000ED0000ED0000ED000021F904",
		"01000002002C000000003D003D004607FF800282838485868788898A8B863E1D6B53009293949596",
		"97925C1E6B8C853A3098A10825A1672DA72D6EA195566A863C655BB2B3B4B5B6B7B235A7273CB8BE",
		"B23B6D833D676256C7C8C9CACBC91A171ACCD1D25176378D1D5F446852DCDDDEDFDD0D21E3E303E0",
		"E7DE3D4B7B6B3B9D853C376D60784C4334457166393EFDFEFF007DBCA120E78C0C1D3EDE295C08CF",
		"8399396474309C28286180356B3AE0E011E50A972E5AA83CD18103869A36387A5044A4234D965597",
		"A2E80171E0C003253C6056D2C146118F345D820A1D4A746804542DD4142582CA41D1A742A3701A94",
		"234C96AB58B36ADDAAD5CB29025CC38AED41266145187AD85059CBB6ADDBB754FFF628E060620FDC",
		"BB709B8C91A828D89C2442D640194CB8B0E1C3880BEFB803C40E190F665726F2D183C70E1D9873E4",
		"B8C1A64E131B667888268343B2E9D3A853ABEEA4E3469A363E74AECAE2010D8C1CA86FA0A122BBF7",
		"2A193DDFE93073C537003C103298A1E466042A03BE63C448D40106D4EB68901A783A031583EB45B1",
		"94F154A6CAAF5F2F947CB97523C4A906E76FADB126E891D8FB628F9C4A3005FF7D2C60F0E5C11D57",
		"1468E081082688A0062D68A0E083105E71061A7CE910C31E6D54A1E1861C76E8E1872086E8A11365",
		"9446480E6CEC81C7142CB6E8E28B2F1A20C28C330E00E38D30DE3004181D28E21A1E407C11C59044",
		"1669E4901494B001FF094E1CE92491391881041831F0F50E0E318C71871133EC11C313608629E698",
		"6486D9C31840D4D0C41C6B7860256A385487461872DC410113491C7184117C1651001C3D042AA818",
		"55AE865A0E01988147113FB431460086462A000EB8496AE9A589E890830731BCC0C6A76CB4014300",
		"37BC8929213D04A0860C4F70E1DB1528B25169A439ACF14271C6ADB2031A32A0D6031B38E49AEB15",
		"6D52D4411A5808ABAC2439A8616A223BB864DC0D158880810A291C750A0A4DF896C30B8BE8B0466C",
		"BE7911820597DC60022A09F886067D8DA47105784551E182016028F5940AA87C40EF503298588819",
		"56FC4B540748B570465148B06BB0506DC05BDF0D5C546CF1FFC5185F0C042A1564CCC501A8D4E1F1",
		"C8158F37880E61C4A7B22D069CB280142BD312000C83ECA0861638E7ACF3CE3CF7AC4510A7480085",
		"CF44E73CC519837460867F4C5FE5850B2D80D0B458557CC1570E7360A1F5D65C77ED75D7206BF0F5",
		"D8646321C31966DD00870711B67D20830EBAEDB61B6DF0E5035AD2E4BD0C011710A0F7DFADD4F1A6",
		"07615020E2E18827CE610C4ED45D480E621CD1038E94B738060B34B63046E59517FE426484783086",
		"1160708EE304E48C7382E930F64004DDCF0EC2430C720C71C693B8CBC102392B7881BB933C30D1C4",
		"3EA023A25B1E41D8C1D1EFCC371F451B362CF145A10BC1F005053FE8D14199DC773FE6183F1CF1C6",
		"82BBC52BA443076AD4A14711F880814340F0C3FF850D3628E1C61830087C9A0FF1BC30861C7B6082",
		"118050001A0C01097BB0031EF6D0384139F00B6638884A3045120FC8A00D6A288318C0E0853A7810",
		"804220420744239A479D8A213C08C018ECD0981CEC8034279C486BD4400726FC600E908A21457C90",
		"0318C880073A0C222102010021FF0B4D414347436F6E2004031039000000015772697474656E2062",
		"7920474946436F6E76657274657220322E342E33206F66204D6F6E6461792C204D61792032352C20",
		"31393938003B"
	};

/* Static variables */
	private static boolean iconTested;
	private static Icon inputIcon;

/* Private instance variables */
	private Component myComponent;
}

/* Package class: AWTDialogModel */
/**
 * This class implements the <code>DialogModel</code> interface using
 * the AWT.
 */
class AWTDialogModel implements DialogModel {

/* Constructor: AWTDialogModel */
/**
 * Initializes the images for the class.
 */
	public AWTDialogModel(Component owner) {
		myComponent = owner;
		imagesReady = false;
	}

/* Method: popupMessage(msg) */
/**
 * Pops up an informational message.
 */
	public void popupMessage(String msg) {
		if (!imagesReady) createImages();
		Frame frame = JTFTools.getEnclosingFrame(myComponent);
		new AWTMessageDialog(frame, "Message", informImage, msg).setVisible(true);
	}

/* Method: popupErrorMessage(msg) */
/**
 * Pops up an error message.
 */
	public void popupErrorMessage(String msg) {
		if (!imagesReady) createImages();
		Frame frame = JTFTools.getEnclosingFrame(myComponent);
		new AWTMessageDialog(frame, "Error", errorImage, msg).setVisible(true);
	}

/* Method: popupLineInputDialog(prompt, allowCancel) */
/**
 * Pops up an dialog box to read a line.  Clicking the Cancel box (if present)
 * returns <code>null</code>.
 */
	public String popupLineInputDialog(String prompt, boolean allowCancel) {
		if (!imagesReady) createImages();
		Frame frame = JTFTools.getEnclosingFrame(myComponent);
		AWTLineInputDialog dialog = new AWTLineInputDialog(frame, prompt, questionImage,
		                                                   allowCancel);
		dialog.setVisible(true);
		return dialog.getInput();
	}

/* Method: popupBooleanInputDialog(prompt, trueLabel, falseLabel, allowCancel) */
/**
 * Pops up an dialog box to read a binary choice.  Clicking the Cancel box (if present)
 * returns <code>null</code>.
 */
	public Boolean popupBooleanInputDialog(String prompt, String trueLabel, String falseLabel,
	                                       boolean allowCancel) {
		if (!imagesReady) createImages();
		Frame frame = JTFTools.getEnclosingFrame(myComponent);
		AWTBooleanInputDialog dialog = new AWTBooleanInputDialog(frame, prompt, questionImage,
		                                                         trueLabel, falseLabel, allowCancel);
		dialog.setVisible(true);
		return dialog.getInput();
	}

/* Private method: createImages */
	private void createImages() {
		errorImage = MediaTools.createImage(ERROR_IMAGE);
		informImage = MediaTools.createImage(INFORM_IMAGE);
		questionImage = MediaTools.createImage(QUESTION_IMAGE);
		imagesReady = true;
	}

/* Private instance variables */
	private Image errorImage;
	private Image informImage;
	private Image questionImage;
	private Component myComponent;
	private boolean imagesReady;

/* Images for the AWT dialogs */
	private static final String[] ERROR_IMAGE = {
		"47494638396120002000F70000FFFFFF980098339999989800111111222222000054CBFFCB003298",
		"0033660033CC0033FE00323266330066660000659800989800CC9900FE99329800659800CC0099FE",
		"0098659898999999CC9900FE98009800329800659900CC9800FE3399CB3399FF9999339898659832",
		"0098650099339998659833CB9833FF9999CC0099FE00336699656698CC9898FF9999323200336600",
		"32003233006632009833339965009866339900663300983200666600986500CC3300FE3200CC6600",
		"FE65CCCC98CCFF99FFCC99FFFF993300CC3200FE6600CC6500FECC0033CC0066FE0032FE00653399",
		"33339966669933669865CC00CCCB00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966",
		"FF9933FF9865333333326532323265326565660033653232660066653265CC3300CC6600FE3200FE",
		"65000066CC0099CC0066FE0098FE00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB33",
		"98CC6699FF3399FF659866CC9965FF9898CC9899FF99CCCC00CCFE00FECB00FEFE00993333996633",
		"9933669865659833CB9966CC9933FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698",
		"FF65CC98CCCCCCCCCC99FFCBCBFFFF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533",
		"CB6666CC6633FF6565FFCB3333CB6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF33",
		"33CB6633FF6666CB3366FF3366CC6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF",
		"65FF66CCCC65FFCC65CCFF65FFFF98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC",
		"33FFFF33FFCC65FFFF65444444656532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000",
		"000098000066777777888888AAAAAABBBBBB5555556666660000100000224400005400000000CC00",
		"00DC0000EE0000FE00003200004400880000980000AA0000BA0000CC0000DC0000EE0000FE00CC00",
		"00DC0000EE0000FE0000004400005400006600007600220000320000AA0000BA0000002200003200",
		"7600008800000000AA0000BA00007600008800000021F90401000096002C0000000020002000C7FF",
		"FFFF980098339999989800111111222222000054CBFFCB0032980033660033CC0033FE0032326633",
		"0066660000659800989800CC9900FE99329800659800CC0099FE0098659898999999CC9900FE9800",
		"9800329800659900CC9800FE3399CB3399FF99993398986598320098650099339998659833CB9833",
		"FF9999CC0099FE00336699656698CC9898FF99993232003366003200323300663200983333996500",
		"9866339900663300983200666600986500CC3300FE3200CC6600FE65CCCC98CCFF99FFCC99FFFF99",
		"3300CC3200FE6600CC6500FECC0033CC0066FE0032FE0065339933339966669933669865CC00CCCB",
		"00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966FF9933FF98653333333265323232",
		"65326565660033653232660066653265CC3300CC6600FE3200FE65000066CC0099CC0066FE0098FE",
		"00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB3398CC6699FF3399FF659866CC9965",
		"FF9898CC9899FF99CCCC00CCFE00FECB00FEFE009933339966339933669865659833CB9966CC9933",
		"FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698FF65CC98CCCCCCCCCC99FFCBCBFF",
		"FF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533CB6666CC6633FF6565FFCB3333CB",
		"6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF3333CB6633FF6666CB3366FF3366CC",
		"6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF65FF66CCCC65FFCC65CCFF65FFFF",
		"98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC33FFFF33FFCC65FFFF6544444465",
		"6532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000000098000066777777888888AAAA",
		"AABBBBBB5555556666660000100000224400005400000000CC0000DC0000EE0000FE000032000044",
		"00880000980000AA0000BA0000CC0000DC0000EE0000FE00CC0000DC0000EE0000FE000000440000",
		"5400006600007600220000320000AA0000BA00000022000032007600008800000000AA0000BA0000",
		"7600008800000008E5002D091C28909DC1830813B22348D0202D85D5AA294C185121872EEC1E4E44",
		"18B1E34676EBD85DB48430E4C793074736CC88F22387861A377A3438F3A04991186352AC08916742",
		"950339B404F9B30BC3A30E87A63C5A90A55297057572F43955A2C59C2D3B529D78D1A94C9F350DDE",
		"EC2A95A6D58F5B71067DBA112853A11CE2CA9D4BD72853A41969E9DDCB77AFC8BB2BCBB67CC93429",
		"DB9F810F43F55A156D5AB236779E353B1921E4931EC35E657C53F260AC982B6A2ECAB2F341CDA30D",
		"5EF6FCB5324EC68A23ABB5243476D1A3B5893E5DE716B76CB16DED020EDAA5B8F1E3C8EF06040021",
		"FF0B4D414347436F6E2004031039000000015772697474656E20627920474946436F6E7665727465",
		"7220322E342E33206F66204D6F6E6461792C204D61792032352C2031393938003B"
	};

	private static final String[] INFORM_IMAGE = {
		"47494638396120002000F70000FFFFFF980098339999989800111111222222000054CBFFCB003298",
		"0033660033CC0033FE00323266330066660000659800989800CC9900FE99329800659800CC0099FE",
		"0098659898999999CC9900FE98009800329800659900CC9800FE3399CB3399FF9999339898659832",
		"0098650099339998659833CB9833FF9999CC0099FE00336699656698CC9898FF9999323200336600",
		"32003233006632009833339965009866339900663300983200666600986500CC3300FE3200CC6600",
		"FE65CCCC98CCFF99FFCC99FFFF993300CC3200FE6600CC6500FECC0033CC0066FE0032FE00653399",
		"33339966669933669865CC00CCCB00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966",
		"FF9933FF9865333333326532323265326565660033653232660066653265CC3300CC6600FE3200FE",
		"65000066CC0099CC0066FE0098FE00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB33",
		"98CC6699FF3399FF659866CC9965FF9898CC9899FF99CCCC00CCFE00FECB00FEFE00993333996633",
		"9933669865659833CB9966CC9933FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698",
		"FF65CC98CCCCCCCCCC99FFCBCBFFFF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533",
		"CB6666CC6633FF6565FFCB3333CB6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF33",
		"33CB6633FF6666CB3366FF3366CC6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF",
		"65FF66CCCC65FFCC65CCFF65FFFF98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC",
		"33FFFF33FFCC65FFFF65444444656532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000",
		"000098000066777777888888AAAAAABBBBBB5555556666660000100000224400005400000000CC00",
		"00DC0000EE0000FE00003200004400880000980000AA0000BA0000CC0000DC0000EE0000FE00CC00",
		"00DC0000EE0000FE0000004400005400006600007600220000320000AA0000BA0000002200003200",
		"7600008800000000AA0000BA00007600008800000021F90401000096002C0000000020002000C7FF",
		"FFFF980098339999989800111111222222000054CBFFCB0032980033660033CC0033FE0032326633",
		"0066660000659800989800CC9900FE99329800659800CC0099FE0098659898999999CC9900FE9800",
		"9800329800659900CC9800FE3399CB3399FF99993398986598320098650099339998659833CB9833",
		"FF9999CC0099FE00336699656698CC9898FF99993232003366003200323300663200983333996500",
		"9866339900663300983200666600986500CC3300FE3200CC6600FE65CCCC98CCFF99FFCC99FFFF99",
		"3300CC3200FE6600CC6500FECC0033CC0066FE0032FE0065339933339966669933669865CC00CCCB",
		"00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966FF9933FF98653333333265323232",
		"65326565660033653232660066653265CC3300CC6600FE3200FE65000066CC0099CC0066FE0098FE",
		"00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB3398CC6699FF3399FF659866CC9965",
		"FF9898CC9899FF99CCCC00CCFE00FECB00FEFE009933339966339933669865659833CB9966CC9933",
		"FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698FF65CC98CCCCCCCCCC99FFCBCBFF",
		"FF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533CB6666CC6633FF6565FFCB3333CB",
		"6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF3333CB6633FF6666CB3366FF3366CC",
		"6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF65FF66CCCC65FFCC65CCFF65FFFF",
		"98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC33FFFF33FFCC65FFFF6544444465",
		"6532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000000098000066777777888888AAAA",
		"AABBBBBB5555556666660000100000224400005400000000CC0000DC0000EE0000FE000032000044",
		"00880000980000AA0000BA0000CC0000DC0000EE0000FE00CC0000DC0000EE0000FE000000440000",
		"5400006600007600220000320000AA0000BA00000022000032007600008800000000AA0000BA0000",
		"7600008800000008A0002D091C48B0A0C1830809F659B8F09F43870C17268C4891E2C38A18FB4C8C",
		"F81062C484200536F418D260C691FF4E6A44A8F2E2C9920531C29CC9706648861D534AB46909634E",
		"992C55F671A932A650A22F830AADC8B3274A8F2B9B9ADC291564D4AA072962353A14EA568E1D3FF2",
		"ACF8536BC98C65992A450B3523D7966D933A3D1AD7AD42B83AE54EF559B7E659B27D9B8245FA15A5",
		"DFAD22C5228EC933200021FF0B4D414347436F6E2004031039000000015772697474656E20627920",
		"474946436F6E76657274657220322E342E33206F66204D6F6E6461792C204D61792032352C203139",
		"3938003B"
	};

	private static final String[] QUESTION_IMAGE = {
		"47494638396120002000F70000FFFFFF980098339999989800111111222222000054CBFFCB003298",
		"0033660033CC0033FE00323266330066660000659800989800CC9900FE99329800659800CC0099FE",
		"0098659898999999CC9900FE98009800329800659900CC9800FE3399CB3399FF9999339898659832",
		"0098650099339998659833CB9833FF9999CC0099FE00336699656698CC9898FF9999323200336600",
		"32003233006632009833339965009866339900663300983200666600986500CC3300FE3200CC6600",
		"FE65CCCC98CCFF99FFCC99FFFF993300CC3200FE6600CC6500FECC0033CC0066FE0032FE00653399",
		"33339966669933669865CC00CCCB00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966",
		"FF9933FF9865333333326532323265326565660033653232660066653265CC3300CC6600FE3200FE",
		"65000066CC0099CC0066FE0098FE00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB33",
		"98CC6699FF3399FF659866CC9965FF9898CC9899FF99CCCC00CCFE00FECB00FEFE00993333996633",
		"9933669865659833CB9966CC9933FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698",
		"FF65CC98CCCCCCCCCC99FFCBCBFFFF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533",
		"CB6666CC6633FF6565FFCB3333CB6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF33",
		"33CB6633FF6666CB3366FF3366CC6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF",
		"65FF66CCCC65FFCC65CCFF65FFFF98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC",
		"33FFFF33FFCC65FFFF65444444656532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000",
		"000098000066777777888888AAAAAABBBBBB5555556666660000100000224400005400000000CC00",
		"00DC0000EE0000FE00003200004400880000980000AA0000BA0000CC0000DC0000EE0000FE00CC00",
		"00DC0000EE0000FE0000004400005400006600007600220000320000AA0000BA0000002200003200",
		"7600008800000000AA0000BA00007600008800000021F90401000096002C0000000020002000C7FF",
		"FFFF980098339999989800111111222222000054CBFFCB0032980033660033CC0033FE0032326633",
		"0066660000659800989800CC9900FE99329800659800CC0099FE0098659898999999CC9900FE9800",
		"9800329800659900CC9800FE3399CB3399FF99993398986598320098650099339998659833CB9833",
		"FF9999CC0099FE00336699656698CC9898FF99993232003366003200323300663200983333996500",
		"9866339900663300983200666600986500CC3300FE3200CC6600FE65CCCC98CCFF99FFCC99FFFF99",
		"3300CC3200FE6600CC6500FECC0033CC0066FE0032FE0065339933339966669933669865CC00CCCB",
		"00FEFE00CBFE00FE6699CC6598FF9898CC9999FFCB9833CC9966FF9933FF98653333333265323232",
		"65326565660033653232660066653265CC3300CC6600FE3200FE65000066CC0099CC0066FE0098FE",
		"00CCCC00FECB00CCFE00FEFE33CC0033FE0066CC0066FE00CB3398CC6699FF3399FF659866CC9965",
		"FF9898CC9899FF99CCCC00CCFE00FECB00FEFE009933339966339933669865659833CB9966CC9933",
		"FF9865FF33CBCB33FFCC33CCFF33FFFF99CB3399FF3399CC6698FF65CC98CCCCCCCCCC99FFCBCBFF",
		"FF99CCFFCBCBFF99FFFFCBFF3333CB3366CB3333FF3366FF6533CB6666CC6633FF6565FFCB3333CB",
		"6533CB3365CC6666FF3333FF6633FF3366FF656533CB3333FF3333CB6633FF6666CB3366FF3366CC",
		"6665FF65CB33CBCC66CCCC33FFCC65FFFF33CCFF65CCFF33FFFF65FF66CCCC65FFCC65CCFF65FFFF",
		"98CCCC99FFCC99CCFF99FFFFCBCB33CCFF33CCCC66CCFF65FFCC33FFFF33FFCC65FFFF6544444465",
		"6532DDDDDDCBFFFFFFFFCBEEEEEE100000980000001000660000000098000066777777888888AAAA",
		"AABBBBBB5555556666660000100000224400005400000000CC0000DC0000EE0000FE000032000044",
		"00880000980000AA0000BA0000CC0000DC0000EE0000FE00CC0000DC0000EE0000FE000000440000",
		"5400006600007600220000320000AA0000BA00000022000032007600008800000000AA0000BA0000",
		"7600008800000008A9002D091C485060AE83080F165C383021C27F1021268C28D1612E831633E6A2",
		"A8D121C68E1B2B82BCB8502345911E0982E478F024484B235BA27C593263C48E0C6BC64C98F3E3C4",
		"934033AAB4E952E64DA13B1FB24CDAF1E84E98319D8E2CD874A9D09E0D9F62659874EB50A608B112",
		"053A5321558B568B5ACC8A56ADDA9426DD5AE50936A4D4BA3FEFE2555A762F5FA65047EAD5C8D66F",
		"CAC26D07F3E43AB6AF59B175BD325EBB35200021FF0B4D414347436F6E2004031039000000015772",
		"697474656E20627920474946436F6E76657274657220322E342E33206F66204D6F6E6461792C204D",
		"61792032352C2031393938003B"
	};

}

/* Package class: AWTDialog */
/**
 * This class acts as the common foundation for the various AWT dialog boxes
 * that are displayed as part of the <code>IODialog</code> operation.
 */
abstract class AWTDialog extends Dialog implements ActionListener {

/* Constant: WIDTH */
/**
 * The standard width of the AWT dialog box.
 */
	public static final int WIDTH = 260;

/* Constant: HEIGHT */
/**
 * The standard height of the AWT dialog box.
 */
	public static final int HEIGHT = 115;

/* Constructor: AWTDialog(frame, title, icon) */
/**
 * Creates the standard AWT dialog.  Subclasses change the behavior by
 * overriding the various abstract methods.
 */
	public AWTDialog(Frame frame, String title, Image icon, boolean allowCancel) {
		super(frame, title, true);
		setLayout(new BorderLayout());
		Panel topPanel = new Panel();
		Panel buttonPanel = new Panel();
		Panel dataPanel = new Panel();
		Panel marginPanel = new Panel();
		topPanel.setLayout(new BorderLayout());
		buttonPanel.setLayout(new FlowLayout());
		dataPanel.setLayout(new BorderLayout());
		marginPanel.setLayout(new BorderLayout());
		marginPanel.add(new Label(" "));
		messageArea = new AWTMessageCanvas();
		dataPanel.add(messageArea, BorderLayout.CENTER);
		initButtonPanel(buttonPanel, allowCancel);
		initDataPanel(dataPanel);
		topPanel.add(new AWTIconCanvas(icon), BorderLayout.WEST);
		topPanel.add(dataPanel, BorderLayout.CENTER);
		add(topPanel, BorderLayout.CENTER);
		add(buttonPanel, BorderLayout.SOUTH);
		add(marginPanel, BorderLayout.EAST);
		Rectangle bounds = frame.getBounds();
		int cx = bounds.x + bounds.width / 2;
		int cy = bounds.y + bounds.height / 2;
		setBounds(cx - WIDTH / 2, cy - HEIGHT / 2, WIDTH, HEIGHT);
		validate();
	}

/* Abstract method: initButtonPanel(buttonPanel, allowCancel) */
/**
 * Initializes the contents of the button panel.  Subclasses override this
 * method to initialize the buttons for that subclass.  The layout of the
 * button panel is <code>FlowLayout</code> with right alignment.
 */
	public abstract void initButtonPanel(Panel buttonPanel, boolean allowCancel);

/* Abstract method: initDataPanel(dataPanel) */
/**
 * Initializes the contents of the data panel.  Subclasses override this
 * method to initialize the format for that subclass.  The layout of the
 * data panel is <code>BorderLayout</code>.
 */
	public abstract void initDataPanel(Panel dataPanel);

/* Abstract method: actionPerformed(ActionEvent e) */
/**
 * Responds to actions in the button panel.  Subclasses override this
 * method to respond to these actions as appropriate to the subclass.
 */
	public abstract void actionPerformed(ActionEvent e);

/* Method: setMessage(msg) */
/**
 * Sets the text of the message.
 */
	public void setMessage(String msg) {
		messageArea.setMessage(msg);
	}

/* Private instance variables */
	private AWTMessageCanvas messageArea;

}

/* Package class: AWTMessageDialog */
/**
 * This class displays a message dialog in the AWT world.
 */
class AWTMessageDialog extends AWTDialog {

/* Constructor: AWTMessageDialog(frame, title, icon, msg) */
/**
 * Creates an <code>AWTMessageDialog</code>.
 */
	public AWTMessageDialog(Frame frame, String title, Image icon, String msg) {
		super(frame, title, icon, false);
		setMessage(msg);
	}

/* Method: initButtonPanel(buttonPanel, allowCancel) */
/**
 * Initializes the contents of the button panel.
 */
	public void initButtonPanel(Panel buttonPanel, boolean allowCancel) {
		okButton = new Button("OK");
		okButton.addActionListener(this);
		buttonPanel.add(okButton);
	}

/* Method: initDataPanel(dataPanel) */
/**
 * Initializes the contents of the data panel.
 */
	public void initDataPanel(Panel dataPanel) {
		/* Empty */
	}

/* Method: actionPerformed(ActionEvent e) */
/**
 * Responds to actions in the button panel.
 */
	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == okButton) {
			setVisible(false);
		}
	}

/* Private instance variables */
	private Button okButton;
}

/* Package class: AWTLineInputDialog */
/**
 * This class requests a line of input in the AWT world.
 */
class AWTLineInputDialog extends AWTDialog {

/* Constructor: AWTLineInputDialog(frame, msg, icon) */
/**
 * Creates an <code>AWTLineInputDialog</code>.
 */
	public AWTLineInputDialog(Frame frame, String msg, Image icon, boolean allowCancel) {
		super(frame, "Input", icon, allowCancel);
		setMessage(msg);
	}

/* Method: getInput() */
/**
 * Returns the input line after the dialog has been dismissed.  This value
 * is either the string entered by the user or <code>null</code> if the
 * cancel button is entered.
 */
	public String getInput() {
		return input;
	}

/* Method: setVisible(flag) */
/**
 * Overrides the standard dialog <code>setVisible</code> method to ensure that
 * the text line gets the focus.
 */
	public void setVisible(boolean flag) {
		super.setVisible(flag);
		if (flag) textLine.requestFocus();
	}

/* Method: initButtonPanel(buttonPanel, allowCancel) */
/**
 * Initializes the contents of the button panel.
 */
	public void initButtonPanel(Panel buttonPanel, boolean allowCancel) {
		okButton = new Button("OK");
		okButton.addActionListener(this);
		buttonPanel.add(okButton);
		if (allowCancel) {
			cancelButton = new Button("Cancel");
			cancelButton.addActionListener(this);
			buttonPanel.add(cancelButton);
		}
	}

/* Method: initDataPanel(dataPanel) */
/**
 * Initializes the contents of the data panel.
 */
	public void initDataPanel(Panel dataPanel) {
		textLine = new TextField();
		textLine.addActionListener(this);
		dataPanel.add(textLine, BorderLayout.SOUTH);
	}

/* Method: actionPerformed(ActionEvent e) */
/**
 * Responds to actions in the button panel.
 */
	public void actionPerformed(ActionEvent e) {
		Component source = (Component) e.getSource();
		if (source == okButton || source == textLine) {
			input = textLine.getText();
			setVisible(false);
		} else if (source == cancelButton) {
			input = null;
			setVisible(false);
		}
	}

/* Private instance variables */
	private Button cancelButton, okButton;
	private TextField textLine;
	private String input;
}

/* Package class: AWTBooleanInputDialog */
/**
 * This class requests boolean input in the AWT world.
 */
class AWTBooleanInputDialog extends AWTDialog {

/* Constructor: AWTBooleanInputDialog(frame, msg, icon, trueLabel, falseLabel) */
/**
 * Creates an <code>AWTLineInputDialog</code>.
 */
	public AWTBooleanInputDialog(Frame frame, String msg, Image icon,
	                             String trueLabel, String falseLabel, boolean allowCancel) {
		super(frame, "Input", icon, allowCancel);
		setMessage(msg);
		trueButton.setLabel(trueLabel);
		falseButton.setLabel(falseLabel);
	}

/* Method: getInput() */
/**
 * Returns the boolean input after the dialog has been dismissed.  This value
 * is either <code>Boolean.TRUE</code>, <code>Boolean.FALSE</code>,
 * or <code>null</code> if the cancel button is entered.
 */
	public Boolean getInput() {
		return input;
	}

/* Method: initButtonPanel(buttonPanel, allowCancel) */
/**
 * Initializes the contents of the button panel.
 */
	public void initButtonPanel(Panel buttonPanel, boolean allowCancel) {
		trueButton = new Button("True");
		trueButton.addActionListener(this);
		buttonPanel.add(trueButton);
		falseButton = new Button("False");
		falseButton.addActionListener(this);
		buttonPanel.add(falseButton);
		if (allowCancel) {
			cancelButton = new Button("Cancel");
			cancelButton.addActionListener(this);
			buttonPanel.add(cancelButton);
		}
	}

/* Method: initDataPanel(dataPanel) */
/**
 * Initializes the contents of the data panel.
 */
	public void initDataPanel(Panel dataPanel) {
		/* Empty */
	}

/* Method: actionPerformed(ActionEvent e) */
/**
 * Responds to actions in the button panel.
 */
	public void actionPerformed(ActionEvent e) {
		Component source = (Component) e.getSource();
		if (source == trueButton) {
			input = Boolean.TRUE;
			setVisible(false);
		} else if (source == falseButton) {
			input = Boolean.FALSE;
			setVisible(false);
		} else if (source == cancelButton) {
			input = null;
			setVisible(false);
		}
	}

/* Private instance variables */
	private Button trueButton, falseButton, cancelButton;
	private Boolean input;
}

/* Package class: AWTIconCanvas */
/**
 * This class displays an icon in the AWT world.
 */
class AWTIconCanvas extends Canvas {

/* Constructor: AWTIconCanvas(icon) */
/**
 * Creates an canvas for displaying an icon.
 */
	public AWTIconCanvas(Image icon) {
		myIcon = icon;
	}

/* Method: getMinimumSize() */
/**
 * Returns the minimum size of the icon.
 */
	public Dimension getMinimumSize() {
		return new Dimension(48, 48);
	}

/* Method: getPreferredSize() */
/**
 * Returns the preferred size of the icon.
 */
	public Dimension getPreferredSize() {
		return getMinimumSize();
	}

/* Method: paint(g) */
/**
 * Draws the icon on the specified <code>Graphics</code> context.
 */
	public void paint(Graphics g) {
		g.drawImage(myIcon, 8, 8, this);
	}

/* Private instance variables */
	private Image myIcon;
}

/* Package class: AWTMessageCanvas */
/**
 * This class manages the message area of a dialog in the AWT world.
 */
class AWTMessageCanvas extends Canvas {

/* Constant: MARGIN */
/**
 * The margin for text display inside the message area.
 */
	public static final int MARGIN = 8;

/* Constant: MESSAGE_FONT */
/**
 * The font to use in the message box.
 */
	public static final Font MESSAGE_FONT = new Font("Dialog", Font.PLAIN, 12);

/* Constructor: AWTMessageCanvas() */
/**
 * Creates an canvas for displaying a message in a <code>IODialog</code> box.
 */
	public AWTMessageCanvas() {
		setFont(MESSAGE_FONT);
	}

/* Method: setMessage(msg) */
/**
 * Sets the text of the message.
 */
	public void setMessage(String msg) {
		message = msg;
	}

/* Method: paint(g) */
/**
 * Draws the message on the specified <code>Graphics</code> context.
 */
	public void paint(Graphics g) {
		FontMetrics fm = g.getFontMetrics();
		int x = MARGIN;
		int y = MARGIN + fm.getAscent();
		int limit = getSize().width - MARGIN;
		StringTokenizer tokenizer = new StringTokenizer(message, " ", true);
		while (tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			int width = fm.stringWidth(token);
			if (x + width > limit) {
				x = MARGIN;
				y += fm.getHeight();
				if (token.equals(" ")) continue;
			}
			g.drawString(token, x, y);
			x += width;
		}
	}

/* Private instance variables */
	private String message;
}
