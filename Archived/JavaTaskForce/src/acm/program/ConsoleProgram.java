/*
 * @(#)ConsoleProgram.java   1.99.1 08/12/08
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

package acm.program;

import acm.io.*;
import acm.util.*;
import java.awt.*;

/* Class: ConsoleProgram() */
/**
 * This class is a standard subclass of <code><a href="Program.html">Program</a></code>
 * that installs a console in the window.
 */
public abstract class ConsoleProgram extends Program {

/* Constructor: ConsoleProgram() */
/**
 * Creates a new console program.
 *
 * @usage ConsoleProgram program = new ConsoleProgram();
 */
	public ConsoleProgram() {
		add(getConsole(), CENTER);
		validate();
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
 * @usage program.setFont(str);
 * @param str A <code>String</code> specifying the new font
 */
	public void setFont(String str) {
		IOConsole console = getConsole();
		if (console != null) {
			console.setFont(str);
			super.setFont(console.getFont());
		}
	}

/* Override method: setFont(font) */
/**
 * Sets the font for the console.
 *
 * @usage program.setFont(font);
 * @param font The new font
 */
	public void setFont(Font font) {
		IOConsole console = getConsole();
		font = JTFTools.getStandardFont(font);
		if (console != null) console.setFont(font);
		super.setFont(font);
	}

/* Factory method: createConsole() */
/**
 * Creates the console used by the <code>ConsoleProgram</code>.
 *
 * @usage IOConsole console = program.createConsole();
 * @return The console to be used by the program
 */
	protected IOConsole createConsole() {
		return new IOConsole();
	}

/* Inherited method: print(value) */
/**
 * @inherited Program#void print(String value)
 * Displays the argument value on the console, leaving the cursor at the end of
 * the output.
 */

/* Inherited method: println() */
/**
 * @inherited Program#void println()
 * Advances the console cursor to the beginning of the next line.
 */

/* Inherited method: println(value) */
/**
 * @inherited Program#void println(String value)
 * Displays the argument value on the console and then advances the cursor
 * to the next line.
 */

/* Inherited method: readLine() */
/**
 * @inherited Program#String readLine()
 * Reads and returns a line of input from the console.
 */

/* Inherited method: readLine(prompt) */
/**
 * @inherited Program#String readLine(String prompt)
 * Prompts the user for a line of input.
 */

/* Inherited method: readInt() */
/**
 * @inherited Program#int readInt()
 * Reads and returns an integer value from the user.
 */

/* Inherited method: readInt(prompt) */
/**
 * @inherited Program#int readInt(String prompt)
 * Prompts the user to enter an integer.
 */

/* Inherited method: readDouble() */
/**
 * @inherited Program#double readDouble()
 * Reads and returns a double-precision value from the user.
 */

/* Inherited method: readDouble(prompt) */
/**
 * @inherited Program#double readDouble(String prompt)
 * Prompts the user to enter a double-precision number.
 */

/* Inherited method: readBoolean() */
/**
 * @inherited Program#boolean readBoolean()
 * Reads and returns a boolean value (<code>true</code> or <code>false</code>).
 */

/* Inherited method: readBoolean(prompt) */
/**
 * @inherited Program#boolean readBoolean(String prompt)
 * Prompts the user to enter a boolean value.
 */

/* Inherited method: readBoolean(prompt, trueLabel, falseLabel) */
/**
 * @inherited Program#boolean readBoolean(String prompt, String trueLabel, String falseLabel)
 * Prompts the user to enter a boolean value, which is matched against the
 * labels provided.
 */

/* Inherited method: getConsole() */
/**
 * @inherited Program#IOConsole getConsole()
 * Returns the console associated with this program.
 */

/* Inherited method: getDialog() */
/**
 * @inherited Program#IODialog getDialog()
 * Returns the dialog used for user interaction.
 */

/* Inherited method: getReader() */
/**
 * @inherited Program#BufferedReader getReader()
 * Returns a <code>BufferedReader</code> whose input comes from the console.
 */

/* Inherited method: getWriter() */
/**
 * @inherited Program#PrintWriter getWriter()
 * Returns a <code>PrintWriter</code> whose output is directed to the console.
 */

/* Inherited method: setTitle(title) */
/**
 * @inherited Program#void setTitle(String title)
 * Sets the title of this program.
 */

/* Inherited method: getTitle() */
/**
 * @inherited Program#String getTitle()
 * Gets the title of this program.
 */

/* Inherited method: pause(milliseconds) */
/**
 * @inherited Program#void pause(double milliseconds)
 * Delays the calling thread for the specified time, which is expressed in
 * milliseconds.
 */
}
