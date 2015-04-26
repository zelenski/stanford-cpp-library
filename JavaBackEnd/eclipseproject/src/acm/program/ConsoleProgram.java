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
import java.io.*;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

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

	// METHODS ADDED BY MARTY
	
	/**
	 * Erases any text from the main console.
	 */
	public void clearConsole() {
		getConsole().clear();
	}
	
	/**
	 * Asks the user to type a file name, re-prompting until the user types a
	 * file that exists in the 'res' resources directory.
	 * The file's full path is returned as a string.
	 * @param prompt the text to display to the user
	 * @param directory the working directory in which to look for files (e.g. "res/")
	 * @return the file name typed by the user
	 */
	public String promptUserForFile(String prompt) {
		return promptUserForFile(prompt, "");
	}
	
	/**
	 * Asks the user to type a file name, re-prompting until the user types a
	 * file that exists in the current directory.
	 * The file's full path is returned as a string.
	 * @param prompt the text to display to the user
	 * @param directory the working directory in which to look for files (e.g. "res/")
	 * @return the file name typed by the user
	 */
	public String promptUserForFile(String prompt, String directory) {
		String filename = readLine(prompt);
		while (!(new File(directory, filename).exists())) {
			println("Unable to open that file. Try again.");
			filename = readLine(prompt);
		}
		if (!directory.equals("")) {
			// filename = new File(directory, filename).getAbsolutePath();
			directory = directory.replace("\\", "/");
			if (!directory.endsWith("/")) {
				directory += "/";
			}
		}
		return directory + filename;
	}


	// BEGIN SNEAKY AUTOGRADER CODE //
	
	/* Instance Variables */
	private InputFileReader inputReader = null;
	private boolean outputCapture = false;
	private boolean inputOverride = false;
	private List<String> echoedComments = null;
	private StringBuilder capturedOutput = new StringBuilder();
	
	/* Static Constants */
	private static final String absolveStudentStr = "[NOT THE STUDENT'S FAULT!]"; 

	/* Administrative Methods */
	private boolean shouldOverrideInput() {
		return inputOverride && (inputReader.peekInputLine() != null);			
	}

	/**
	 * Signals the ConsoleProgram to use the input profile read from the file
	 * with inputFilename, rather than prompt the user for input.
	 * @param inputFilename 	File to draw the input from.
	 */
	public void overrideInput(String inputFilename) {
		if (!inputOverride) {
			inputOverride = true;
			inputReader = new InputFileReader(inputFilename);
			echoedComments = new ArrayList<String>();
			String newTitle = getTitle() + " [Input from " + inputFilename + "]";
			setTitle(newTitle);
		}
	}
	
	public void captureOutput() {
		captureOutput(true);
	}
	
	public void captureOutput(boolean capture) {
		outputCapture = capture;
		if (!outputCapture && capturedOutput.length() > 0) {
			capturedOutput.delete(0, capturedOutput.length());
		}
	}
	
	public String getCapturedOutput() {
		return capturedOutput.toString();
	}
	
	/**
	 * Print all echoed comments from the input override file onscreen.  
	 */
	public void echoComments() {
		println(getComments());
	}
	
	/**
	 * Print all echoed comments from the input override file onscreen.  
	 */
	public String getComments() {
		StringBuilder builder = new StringBuilder();
		if (inputOverride) {		
			inputReader.flush();
			Iterator<String> iter = echoedComments.iterator();
			while (iter.hasNext()) {
				builder.append(iter.next().replace("#> ", ""));
				builder.append('\n');
			}
		}
		return builder.toString();
	}
	
	/* Overridden "Hijacked" Methods */
	public void print(String value) {
		// all other print()s call print(String)
		if (outputCapture) {
			capturedOutput.append(value);
		} else {
			super.print(value);
		}
	}
	
	public void println() {
		if (outputCapture) {
			capturedOutput.append('\n');
		} else {
			super.println();
		}
	}
	
	public void println(String value) {
		// all other println()s call println(String)
		if (outputCapture) {
			capturedOutput.append(value);
			capturedOutput.append('\n');
		} else {
			super.println(value);
		}
	}
	
	@Override
	public int readInt(String prompt, int min, int max) {	
		int result;
		
		if (outputCapture || shouldOverrideInput()) {
			result = getInputInt();	
			checkRange(result, min, max);
			// super.println(prompt + result + "\t<readInt>");
			println(prompt + result + "\t<readInt>");
		} else {
			result = super.readInt(prompt, min, max);
				
		}
		
		return result; 
	}

	@Override
	public double readDouble(String prompt, double min, double max) {	
		double result;
		
		if (outputCapture || shouldOverrideInput()) {
			result = getInputDouble();
			checkRange(result, min, max);
			// super.println(prompt + result + "\t<readDouble>");
			println(prompt + result + "\t<readDouble>");
		} else {
			result = super.readDouble(prompt, min, max); 
		}
		
		return result; 
	}

	// TODO
//	@Override
//	public boolean readBoolean(String prompt, String y, String n) {	
//		boolean result = false;
//		
//		if (shouldOverrideInput()) {
//			result = getInputDouble();
//			checkRange(result, min, max);
//			super.println(prompt + result + "\t<readDouble>");
//		} else {
//			result = super.readDouble(prompt, min, max); 
//		}
//		
//		return result; 
//	}

	/* Support Methods */
	private int getInputInt() {
		String line = null;
		try {
			line = inputReader.readInputLine();
			return Integer.parseInt(line);
		}
		catch (NumberFormatException e) {
			throw new RuntimeException(absolveStudentStr + " Poorly formatted integer input: " + line, e);
		}		
	}
	
	private double getInputDouble() {
		String line = null;
		try {
			line = inputReader.readInputLine();
			return Double.parseDouble(line);
		}
		catch (NumberFormatException e) {
			throw new RuntimeException(absolveStudentStr + " Poorly formatted double input: " + line, e);
		}		
	}
	
	private static void checkRange(int value, int min, int max) {
		if ((min > value) || (max < value))
			throw new RuntimeException(absolveStudentStr + " Out of range ["
					+ min + ", " +	max + "] integer input: " + value);		
	}	
	
	private static void checkRange(double value, double min, double max) {
		if ((min > value) || (max < value))
			throw new RuntimeException(absolveStudentStr + " Out of range ["
					+ min + ", " +	max + "] double input: " + value);		
	}
	
	/* Input File Reader class */
	private class InputFileReader {
		/* Instance Variables */
		private BufferedReader inputReader;
		private String cachedLine;
		
		/* Constructor */
		public InputFileReader(String inputFilename) {
			try {
				cachedLine = null;
				if (inputFilename != null)
					inputReader = new BufferedReader(new FileReader(inputFilename));
			}
			catch (IOException e) {
				throw new RuntimeException(absolveStudentStr + " Error while opening file: " + inputFilename, e);
			}
		}

		/* Input file reading operations */
		public String readInputLine() {
			String result = peekInputLine();
			cachedLine = null;
			return result;
		}
		
		public String peekInputLine() {
			if (cachedLine == null) advanceLine();
			return cachedLine;
		}
		
		/* Advances lines in the file until end of file, or a non-comment line appears.
		 * Stores said line into the cachedLine instance variable. */
		private void advanceLine() {
			if (inputReader == null) return;
			
			try {
				cachedLine = inputReader.readLine();				
				while (isComment(cachedLine)) {
					// should we echo this line?
					if (cachedLine.startsWith(echoCommentBeginStr))
						echoedComments.add(cachedLine);

					cachedLine = inputReader.readLine();				
				}
				
				if (cachedLine == null) {
					print("[INPUT FILE EXHAUSTED!] ");
					inputReader = null;
				}
			}
			catch (IOException e) {
				throw new RuntimeException(absolveStudentStr + " Error while reading file.", e);		
			}		
		}
		
		private void flush() {
			if (inputReader == null) return;
			
			try {
				cachedLine = inputReader.readLine();				
				while (cachedLine != null) {
					// should we echo this line?
					if (cachedLine.startsWith(echoCommentBeginStr))
						echoedComments.add(cachedLine);
					cachedLine = inputReader.readLine();				
				}
			}
			catch (IOException e) {
				throw new RuntimeException(absolveStudentStr + " Error while reading file.", e);		
			}		
		}
		
		/* A string is a comment iff it is non-null
		 * and has beginning string "#" */
		private boolean isComment(String str) {
			return (str != null) && (str.startsWith(commentBeginStr));
		}

		/* Static/String Constants */
		private static final String commentBeginStr = "#";
		private static final String echoCommentBeginStr = "#>";
		
	}
}
