/*
 * @(#)DialogProgram.java   1.99.1 08/12/08
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
// Code cleanup 28-May-07 (ESR)
//   1. Rewrote code to call setInputModel/setOutputModel

package acm.program;

import acm.io.*;

/* Class: DialogProgram() */
/**
 * This class is a standard subclass of <code><a href="Program.html">Program</a></code>
 * that takes its input from a <code>IODialog</code> object.
 */
public abstract class DialogProgram extends Program {

/* Constructor: DialogProgram() */
/**
 * Creates a new dialog program.
 *
 * @usage DialogProgram program = new DialogProgram();
 */
	public DialogProgram() {
		IODialog dialog = getDialog();
		setInputModel(dialog);
		setOutputModel(dialog);
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

/* Inherited method: print(value) */
/**
 * @inherited Program#void print(String value)
 * Displays the argument value, allowing for the possibility of more
 * output in the same dialog.
 */

/* Inherited method: println() */
/**
 * @inherited Program#void println()
 * Completes the output line and displays the dialog.
 */

/* Inherited method: println(value) */
/**
 * @inherited Program#void println(String value)
 * Adds the value to the current output line and then displays the dialog.
 */

/* Inherited method: readLine() */
/**
 * @inherited Program#String readLine()
 * Puts up a dialog box asking the user for a line of text.
 */

/* Inherited method: readLine(prompt) */
/**
 * @inherited Program#String readLine(String prompt)
 * Puts up a dialog box asking the user for a line of text.
 */

/* Inherited method: readInt() */
/**
 * @inherited Program#int readInt()
 * Puts up a dialog box asking the user for an integer.
 */

/* Inherited method: readInt(prompt) */
/**
 * @inherited Program#int readInt(String prompt)
 * Puts up a dialog box asking the user for an integer.
 */

/* Inherited method: readDouble() */
/**
 * @inherited Program#double readDouble()
 * Puts up a dialog box asking the user for a double-precision number.
 */

/* Inherited method: readDouble(prompt) */
/**
 * @inherited Program#double readDouble(String prompt)
 * Puts up a dialog box asking the user for a double-precision number.
 */

/* Inherited method: readBoolean() */
/**
 * @inherited Program#boolean readBoolean()
 * Puts up a dialog box asking the user for a <code>true</code>/<code>false</code> value.
 */

/* Inherited method: readBoolean(prompt) */
/**
 * @inherited Program#boolean readBoolean(String prompt)
 * Puts up a dialog box asking the user for a <code>true</code>/<code>false</code> value.
 */

/* Inherited method: readBoolean(prompt) */
/**
 * @inherited Program#boolean readBoolean(String prompt, trueLabel, falseLabel)
 * Puts up a dialog box asking the user for a boolean value chosen from
 * buttons with the specified labels.
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
 * Delays the calling thread for the specified time.
 */
}
