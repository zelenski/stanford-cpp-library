/*
 * @version 2018/10/02
 * - setEcho for accessibility
 * @version 2017/07/14
 * - bug fix for readInt/Double/Boolean/Line with null prompt
 * @version 2017/04/26
 * - bug fix for captured output with printf() method
 * @version 2016/10/02
 * - moved some I/O code to AbstractConsoleProgram superclass
 * @version 2016/05/17
 * - moved some menuAction code to AbstractConsoleProgram superclass
 * - added getFont() method to fix null-font-before-set issue
 * @version 2016/04/18
 * - modified readBoolean, readLine to work with override input feature (e.g. HW2 autograder)
 * @version 2015/05/12
 * - added scroll__() methods for scrolling around in the console
 *   (these are called by ProgramMenuBar but are made public in case clients want them)
 */

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
import java.awt.*;
import java.io.*;
import java.util.*;
import java.util.List;

/* Class: ConsoleProgram() */
/**
 * This class is a standard subclass of <code><a href="Program.html">Program</a></code>
 * that installs a console in the window.
 */
public abstract class ConsoleProgram extends AbstractConsoleProgram {
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

	// METHODS ADDED BY MARTY
	

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
		return inputOverride && (inputReader != null && inputReader.peekInputLine() != null);			
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
	
	/**
	 * Signals the ConsoleProgram to use the input profile read from the given stream,
	 * rather than prompt the user for input.
	 * @param inputFilename 	File to draw the input from.
	 */
	public void overrideInput(InputStream stream) {
		if (!inputOverride) {
			inputOverride = true;
			inputReader = new InputFileReader(stream);
			echoedComments = new ArrayList<String>();
			String newTitle = getTitle() + " [Input from file]";
			setTitle(newTitle);
		}
	}
	
	/**
	 * Signals the ConsoleProgram to use the input profile read from the given reader,
	 * rather than prompt the user for input.
	 * @param inputFilename 	File to draw the input from.
	 */
	public void overrideInput(Reader reader) {
		if (!inputOverride) {
			inputOverride = true;
			inputReader = new InputFileReader(reader);
			echoedComments = new ArrayList<String>();
			String newTitle = getTitle() + " [Input from file]";
			setTitle(newTitle);
		}
	}
	
	public void captureOutput() {
		captureOutput(true);
	}
	
	public void captureOutput(boolean capture) {
		outputCapture = capture;
		if (capturedOutput == null) {
			capturedOutput = new StringBuilder();
		}
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
		}
		super.print(value);
	}
	
	public void print(String value, Color color) {
		// all other print()s call print(String)
		if (outputCapture) {
			capturedOutput.append(value);
		}
		super.print(value, color);
	}
	
	public void printf(String format, Object... args) {
		// BUGFIX: used to append captured output here, but implementation
		// of super.printf() calls super.print(), which made it print twice
		super.printf(format, args);
	}
	
	public void println() {
		if (outputCapture) {
			capturedOutput.append('\n');
		}
		super.println();
	}
	
	public void println(String value) {
		// all other println()s call println(String)
		if (outputCapture) {
			capturedOutput.append(value);
			capturedOutput.append('\n');
		}
		super.println(value);
	}
	
	public void println(String value, Color color) {
		// all other println()s call println(String)
		if (outputCapture) {
			capturedOutput.append(value);
			capturedOutput.append('\n');
		}
		super.println(value, color);
	}
	
	@Override
	public int readInt(String prompt, int min, int max) {	
		int result;
		
		if (shouldOverrideInput()) {
			if (prompt != null && !prompt.endsWith(" ")) {
				prompt += " ";
			}
			result = getInputInt();	
			checkRange(result, min, max);
			// super.println(prompt + result + "\t<readInt>");
			if (prompt != null) {
				print(prompt);
			}
			print(result, Color.BLUE);
			println("\t<readInt>");
		} else {
			result = super.readInt(prompt, min, max);
			if (outputCapture) {
				capturedOutput.append(result);
				capturedOutput.append("\n");
			}
		}
		
		return result; 
	}

	@Override
	public double readDouble(String prompt, double min, double max) {	
		double result;
		
		if (shouldOverrideInput()) {
			if (prompt != null && !prompt.endsWith(" ")) {
				prompt += " ";
			}
			result = getInputDouble();
			checkRange(result, min, max);
			// super.println(prompt + result + "\t<readDouble>");
			if (prompt != null) {
				print(prompt);
			}
			print(result, Color.BLUE);
			println("\t<readDouble>");
		} else {
			result = super.readDouble(prompt, min, max); 
			if (outputCapture) {
				capturedOutput.append(result);
				capturedOutput.append("\n");
			}
		}
		
		return result; 
	}

	@Override
	public boolean readBoolean(String prompt, String y, String n) {	
		Boolean result = false;
		
		if (shouldOverrideInput()) {
			if (prompt != null && !prompt.endsWith(" ")) {
				prompt += " ";
			}
			if (prompt != null) {
				print(prompt);
			}
			result = getInputBoolean(y, n);
			println("\t<readBoolean>");
		} else {
			result = super.readBoolean(prompt, y, n); 
		}
		
		return result == null ? false : (boolean) result; 
	}

	@Override
	public String readLine(String prompt) {	
		String line = "";
		
		if (shouldOverrideInput()) {
			if (prompt != null && !prompt.endsWith(" ")) {
				prompt += " ";
			}
			if (prompt != null) {
				print(prompt);
			}
			line = inputReader.readInputLine();
			println(line + "\t<readLine>");
		} else {
			line = super.readLine(prompt);
		}
		
		return line; 
	}
	
	public void setEcho() {
		setEcho(true);
	}

	public void setEcho(boolean echo) {
		if (getOutputModel() instanceof IOConsole) {
			((IOConsole) getOutputModel()).setEcho(echo);
		}
	}

	/* Support Methods */
	private int getInputInt() {
		String line = null;
		try {
			line = inputReader.readInputLine();
			return Integer.parseInt(line);
		}
		catch (NumberFormatException e) {
			throw new RuntimeException(absolveStudentStr + " Poorly formatted integer input: \"" + line + "\"", e);
		}		
	}
	
	private double getInputDouble() {
		String line = null;
		try {
			line = inputReader.readInputLine();
			return Double.parseDouble(line);
		}
		catch (NumberFormatException e) {
			throw new RuntimeException(absolveStudentStr + " Poorly formatted double input: \"" + line + "\"", e);
		}		
	}
	
	private Boolean getInputBoolean(String t, String f) {
		String line = null;
		if (inputReader != null) {
			line = inputReader.readInputLine();
		}
		print(line, Color.BLUE);
		return (t != null && t.equalsIgnoreCase(line)) ? Boolean.TRUE
				: (f != null && f.equalsIgnoreCase(line)) ? Boolean.FALSE
				: null;
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
	class InputFileReader {
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

		/* Constructor */
		public InputFileReader(InputStream stream) {
			cachedLine = null;
			inputReader = new BufferedReader(new InputStreamReader(stream));
		}

		/* Constructor */
		public InputFileReader(Reader reader) {
			cachedLine = null;
			inputReader = new BufferedReader(reader);
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
