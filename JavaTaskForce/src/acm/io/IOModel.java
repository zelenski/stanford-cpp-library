/*
 * @(#)IOModel.java   1.99.1 08/12/08
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

/* Interface: IOModel */
/**
 * The <code>IOModel</code> interface defines the input and output methods
 * supported by both the <a href="IOConsole.html"><code>IOConsole</code></a>
 * and <a href="IODialog.html"><code>IODialog</code></a> classes.
 */
public interface IOModel {

/* Method: print(value) */
/**
 * Displays the argument value, allowing for the possibility of more
 * output on the same line.  The <code>print</code> method is overloaded
 * so that <code>value</code> can be of any type.
 *
 * @usage model.print(value);
 * @param value The value to be displayed
 */
	public void print(String value);

/**
 * Makes sure that <code>print</code> can display a <code>boolean</code>.
 * @noshow
 */
	public void print(boolean x);

/**
 * Makes sure that <code>print</code> can display a <code>char</code>.
 * @noshow
 */
	public void print(char x);

/**
 * Makes sure that <code>print</code> can display a <code>double</code>.
 * @noshow
 */
	public void print(double x);

/**
 * Makes sure that <code>print</code> can display a <code>float</code>.
 * @noshow
 */
	public void print(float x);

/**
 * Makes sure that <code>print</code> can display an <code>int</code>.
 * @noshow
 */
	public void print(int x);

/**
 * Makes sure that <code>print</code> can display a <code>long</code>.
 * @noshow
 */
	public void print(long x);

/**
 * Makes sure that <code>print</code> can display an <code>Object</code>.
 * @noshow
 */
	public void print(Object x);

/* Method: println() */
/**
 * Completes the output line.
 *
 * @usage io.println();
 */
	public void println();

/* Method: println(value) */
/**
 * Displays the value and then completes the line.  The <code>println</code>
 * method is overloaded so that <code>value</code> can be of any type.
 *
 * @usage io.println(value);
 * @param value The value to be displayed
 */
	public void println(String value);

/**
 * Makes sure that <code>println</code> can display a <code>boolean</code>.
 * @noshow
 */
	public void println(boolean x);

/**
 * Makes sure that <code>println</code> can display a <code>char</code>.
 * @noshow
 */
	public void println(char x);

/**
 * Makes sure that <code>println</code> can display a <code>double</code>.
 * @noshow
 */
	public void println(double x);

/**
 * Makes sure that <code>println</code> can display a <code>float</code>.
 * @noshow
 */
	public void println(float x);

/**
 * Makes sure that <code>println</code> can display an <code>int</code>.
 * @noshow
 */
	public void println(int x);

/**
 * Makes sure that <code>println</code> can display a <code>long</code>.
 * @noshow
 */
	public void println(long x);

/**
 * Makes sure that <code>println</code> can display an <code>Object</code>.
 * @noshow
 */
	public void println(Object x);

/* Method: showErrorMessage(msg) */
/**
 * Displays the error message.
 *
 * @usage io.showErrorMessage(msg);
 * @param msg The error msg to be displayed
 */
	public void showErrorMessage(String msg);

/* Method: readLine() */
/**
 * Reads and returns a line of input, without including the end-of-line
 * characters that terminate the input.
 *
 * @usage String str = io.readLine();
 * @return The next line of input as a <code>String</code>
 */
	public String readLine();

/* Method: readLine(prompt) */
/**
 * Prompts the user to enter a line of text, which is then returned
 * as the value of this method.  The end-of-line characters that terminate
 * the input are not included in the returned string.
 *
 * @usage String str = io.readLine(prompt);
 * @param prompt The prompt string to display to the user
 * @return The next line of input as a <code>String</code>
 */
	public String readLine(String prompt);

/* Method: readInt() */
/**
 * Reads and returns an integer value from the user.
 *
 * @usage int n = io.readInt();
 * @return The value of the input interpreted as a decimal integer
 */
	public int readInt();

/* Method: readInt(low, high) */
/**
 * Reads and returns an integer value from the user, which is constrained to
 * be within the specified inclusive range.
 *
 * @usage int n = io.readInt(low, high);
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a decimal integer
 */
	public int readInt(int low, int high);

/* Method: readInt(prompt) */
/**
 * Prompts the user to enter an integer, which is then returned as the value
 * of this method.
 *
 * @usage int n = io.readInt(prompt);
 * @param prompt The prompt string to display to the user
 * @return The value of the input interpreted as a decimal integer
 */
	public int readInt(String prompt);

/* Method: readInt(prompt, low, high) */
/**
 * Prompts the user to enter an integer, which is then returned as the value
 * of this method.  The value must be within the inclusive range between
 * <code>low</code> and <code>high</code>.
 *
 * @usage int n = io.readInt(prompt, low, high);
 * @param prompt The prompt string to display to the user
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a decimal integer
 */
	public int readInt(String prompt, int low, int high);

/* Method: readDouble() */
/**
 * Reads and returns a double-precision value from the user.
 *
 * @usage double d = io.readDouble();
 * @return The value of the input interpreted as a <code>double</code>
 */
	public double readDouble();

/* Method: readDouble(low, high) */
/**
 * Reads and returns a double-precision value from the user, which is
 * constrained to be within the specified inclusive range.
 *
 * @usage double d = io.readDouble(low, high);
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a <code>double</code>
 */
	public double readDouble(double low, double high);

/* Method: readDouble(prompt) */
/**
 * Prompts the user to enter an double-precision number, which is then
 * returned as the value of this method.
 *
 * @usage double d = io.readDouble(prompt);
 * @param prompt The prompt string to display to the user
 * @return The value of the input interpreted as a <code>double</code>
 */
	public double readDouble(String prompt);

/* Method: readDouble(prompt, low, high) */
/**
 * Prompts the user to enter an double-precision number, which is then returned
 * as the value of this method.  The value must be within the inclusive range
 * between <code>low</code> and <code>high</code>.
 *
 * @usage d = io.readDouble(prompt, low, high);
 * @param prompt The prompt string to display to the user
 * @param low The lowest value in the permitted range
 * @param high The highest value in the permitted range
 * @return The value of the input interpreted as a <code>double</code>
 */
	public double readDouble(String prompt, double low, double high);

/* Method: readBoolean() */
/**
 * Reads and returns a boolean value from the user, which must match
 * either <code>true</code> or <code>false</code>, ignoring case.
 *
 * @usage boolean flag = io.readBoolean();
 * @return The value of the input interpreted as a boolean value
 */
	public boolean readBoolean();

/* Method: readBoolean(prompt) */
/**
 * Prompts the user to enter a boolean value, which is then returned as
 * the value of this method.
 *
 * @usage boolean flag = io.readBoolean(prompt);
 * @param prompt The prompt string to display to the user
 * @return The value of the input interpreted as a boolean value
 */
	public boolean readBoolean(String prompt);

/* Method: readBoolean(prompt, trueLabel, falseLabel) */
/**
 * Prompts the user to enter a value, which is interpreted as a boolean value
 * by matching it against the <code>trueLabel</code> and <code>falseLabel</code>
 * parameters.
 *
 * @usage boolean flag = dialog.readBoolean(prompt);
 * @param prompt The prompt string to display to the user
 * @param trueLabel The string used to indicate <code>true</code>
 * @param falseLabel The string used to indicate <code>false</code>
 * @return The value of the input interpreted as a boolean value
 */
	public boolean readBoolean(String prompt, String trueLabel, String falseLabel);
}
