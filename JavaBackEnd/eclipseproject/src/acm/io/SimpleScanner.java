/*
 * @version 2016/04/24
 * - initial version
 */

package acm.io;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.PrintWriter;
import java.io.PushbackReader;
import java.io.Reader;
import java.io.StringReader;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.Locale;
import java.util.NoSuchElementException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import stanford.cs106.io.IORuntimeException;

/**
 A simple text scanner which can parse primitive types and strings 
 using regular expressions.

 <p>
 A Scanner breaks its input into tokens using a delimiter pattern,
 which by default matches whitespace. The resulting tokens may then be
 converted into values of different types using the various next methods.

 <p>
 This class is based on a subset of the functionality of Sun's
 java.util.Scanner class from J2SE v1.5.0, with some code borrowed 
 from the TextReader class written by Stuart Reges of the 
 University of Washington.  It should work with 'lazy input' from the
 keyboard as needed.

 <p>
 Some notable differences from java.util.Scanner are the following:

 <ul><li>

 All Java 5-specific features, such as implementing Iterator &lt; String &gt;
 and the public MatchResult match() method, are missing.

 </li><li>

 The useLocale, findInLine, and findInHorizon methods are not implemented
 and throw an UnsupportedOperationException when called.

 </li><li>

 This implementation is not guaranteed to exactly match the parsing behavior
 of java.util.Scanner for complex cases.  The java.util.Scanner uses 
 sophisticated delimiting between tokens of input; this Scanner simply 
 tokenizes on whitespace.  Also, for example, this implementation returns
 true on a call to hasNextFloat() if the upcoming input token is "2.1F", 
 because this implementation's parsing is based on the primitive wrappers;
 java.util.Scanner would return false in this same case.

 </li><li>

 The toString text, exception text and exception-throwing behavior 
 is not guaranteed to match that of java.util.Scanner.  In particular,
 this implementation includes the offending input line number in its
 NoSuchElementException text to aid debugging.

 </li><li>

 This implementation is slower than java.util.Scanner, partly because it 
 does not directly cache input or regular expression patterns for simplicity.
 Since this class is a holdover implementation until widespread adoption of
 Java 5, this was deemed acceptable.

 </li></ul>

 <p>
 Recent changes:

 <ul><li>

 2011-12-05: Fixed bug with scanner.reuse property and System.setIn.
 </li><li>
 2008-02-27: Fixed bugs: next() w/ trailing spaces; multiple System.in Scanners
 </li><li>
 2007-08-13: Fixed hasNextLine bug where last line doesn't have a trailing \n .
 </li><li>
 2007-07-27: Added -Dscanner.workingdir=DIR option for Grade-It.
 </li><li>
 2007-07-18: More hanging input bug fixes for Grade-It system.
 </li><li>
 2007-07-15: Fixed various input bugs (used to hang after waiting for input).

 </li><li>

 <ul><li>

 2006-09-18: Added support for multiple Scanners reading the same underlying
 input stream without problems (useful for CSE 14x grading scripts).  Example:

 <pre>

 % java -Dscanner.reuse=1 MyProgramThatUsesScanner

 </pre>

 </li><li>

 2005-07-30: Added support for regular expressions, delimiter, hasNext(Pattern),
 next(Pattern), useDelimiter, and skip(Pattern).

 </li><li>

 2005-07-12: Added better error messages when wrong token type is read.
 The goal is for CS1 students to be able to more easily find their mistakes.

 Added support for command-line flag -Dscanner.echo=1 flag so that the
 Scanner can be made to echo stdin when it is being redirected in from
 a file.  Example:

 <pre>

 % java -Dscanner.echo=1 MyProgramThatUsesScanner < input.txt

 </pre>

 </li><li>

 2005-01-22: Fixed a bug where nextLine returned an empty string rather than
 throwing an exception when no more input was available.

 </li><li>

 2005-01-01: Renamed all private methods with underscores because students
 were discovering the private hasNextChar method and trying to use it in 
 their programs.

 </li></ul>

 @author Marty Stepp (stepp AT cs washington edu),
 Lecturer, University of Washington CS&E

 @version 2011-12-05
 */
//@SuppressWarnings("unused")
public final class SimpleScanner {
	// class constants
	public static final String SCANNER_APPLET_MODE_PROPERTY_NAME = "scanner.appletmode";
	public static final String SCANNER_ECHO_PROPERTY_NAME = "scanner.echo";
	public static final String SCANNER_REUSE_PROPERTY_NAME = "scanner.reuse";
	public static final String SCANNER_WORKING_DIR_PROPERTY_NAME = "scanner.workingdir";

	private static final int EOF = -1;           // used to denote end-of-input
	private static final int PUSHBACK_BUFFER_SIZE = 4096;  // buffer to unread
	private static final Pattern DEFAULT_DELIMITER = 
			Pattern.compile("\\s+");   // pattern that matches whitespace chars
	private static final String TRUE = "true";
	private static final String FALSE = "false";
	private static final String REMOVE_EXCEPTION_MESSAGE = 
			"Remove is not supported by this Scanner.";
	private static final String NEAR_LINE_MESSAGE = "  Near input line ";
	private static final String NO_TOKEN_MESSAGE = "No tokens remain in input.";
	private static boolean DEBUG = false;

	// static fields set by static 'set' method calls or System properties
	private static boolean s_appletMode = false;
	private static boolean s_echo = false;
	private static boolean s_reuse = false;
	private static String s_workingDir = null;

	// shared readers to implement scanner.reuse option
	private static PushbackReader s_reader = null;
	private static LineNumberReader s_lnReader = null;
	private static InputStream s_SystemIn = null;    // added to catch cases where they change System.in on me (used to break scanner.reuse mode)

	static {
		if (DEBUG) System.out.println("Scanner static initializer");
		try {
			String debugProp = System.getProperty("scanner.debug");
			DEBUG = DEBUG || "true".equalsIgnoreCase(debugProp) ||
					"on".equalsIgnoreCase(debugProp) ||
					"yes".equalsIgnoreCase(debugProp) ||
					"1".equals(debugProp);
		} catch (Throwable t) {}
	}

	// Wipes the saved System.in readers that were added to 
	// (generally doesn't need to be called except by JUnit scripts etc)
	public static void clearCache() {
		s_reader = null;
		s_lnReader = null;
		s_SystemIn = null;
	}

	// Returns whether to echo every token read from System.in
	public static boolean getEcho() {
		try {
			s_echo = isTruthy(System.getProperty(SCANNER_ECHO_PROPERTY_NAME));
		} catch (SecurityException e) {}
		return s_echo;
	}

	// Returns whether Scanner is running inside an applet
	// (if so, converts all File Scanners to read from ClassLoader resources instead)
	public static boolean getAppletMode() {
		try {
			s_appletMode = isTruthy(System.getProperty(SCANNER_APPLET_MODE_PROPERTY_NAME));
		} catch (SecurityException e) {}
		return s_appletMode;
	}

	// Returns whether to reuse the underlying input stream for multiple
	// Scanners on the same source
	public static boolean getReuse() {
		try {
			s_reuse = isTruthy(System.getProperty(SCANNER_REUSE_PROPERTY_NAME));
		} catch (SecurityException e) {}
		return s_reuse;
	}

	// Returns whether to use a certain working directory for every file opened
	// (used by Grade-it)
	public static String getWorkingDir() {
		try {
			s_workingDir = System.getProperty(SCANNER_WORKING_DIR_PROPERTY_NAME);
		} catch (SecurityException e) {}
		return s_workingDir;
	}

	// Sets whether to echo every token read from System.in
	public static void setAppletMode(boolean value) {
		try {
			System.setProperty(SCANNER_APPLET_MODE_PROPERTY_NAME, value ? "true" : null);
		} catch (SecurityException e) {}
		s_appletMode = value;
	}

	// Sets whether to echo every token read from System.in
	public static void setEcho(boolean value) {
		try {
			System.setProperty(SCANNER_ECHO_PROPERTY_NAME, value ? "true" : null);
		} catch (SecurityException e) {}
		s_echo = value;
	}

	// Sets whether to reuse the underlying input stream for multiple
	// Scanners on the same source (helpful for students who construct
	// multiple scanners to read from System.in)
	public static void setReuse(boolean value) {
		try {
			System.setProperty(SCANNER_REUSE_PROPERTY_NAME, value ? "true" : null);
		} catch (SecurityException e) {}
		s_reuse = value;
	}

	// Sets whether to use a certain working directory for every file opened
	// (used by Grade-it)
	public static void setWorkingDir(String value) {
		try {
			System.setProperty(SCANNER_WORKING_DIR_PROPERTY_NAME, value);
		} catch (SecurityException e) {}
		s_workingDir = value;
	}

	// Returns the given exception's stack trace as a String.
	private static String _getStackTraceAsString(Throwable e) {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		PrintWriter writer = new PrintWriter(bytes, true);
		e.printStackTrace(writer);
		String str = bytes.toString();

		try {
			writer.close();
			bytes.close();
		}
		catch (IOException ex) {}

		return str;
	}

	// Returns true if one of Scanner's "hasNext" methods is on the stack.
	// Used so that we don't echo certain things when simply testing for hasNext.
	private static boolean _inHasNextMethod() {
		String stack = _getStackTraceAsString(new RuntimeException());
		return stack.indexOf("Scanner.hasNext") >= 0;
	}

	private static boolean isTruthy(String s) {
		return s != null && (s.equals("1") || s.equalsIgnoreCase("true") || s.equalsIgnoreCase("on") || s.equalsIgnoreCase("enabled"));
	}

	// fields
	private Object m_source;                   // place where input came from in ctor
	private PushbackReader m_reader;           // underlying input source
	private LineNumberReader m_lnReader;       // keeps track of line numbers
	private IOException m_ioException = null;  // last IO exception
	private StringBuffer m_previousNextBuffer = new StringBuffer();  // last token
	private Pattern m_delimiter = DEFAULT_DELIMITER;  // delimiter between tokens
	private Locale m_locale = Locale.getDefault();  // Locale (not used)
	private int m_radix = 10;                  // default integer base
	private boolean m_closed = false;          // set true on close or exception
	private boolean m_showLineNumbers = true;  // set to false when reading System.in
	private boolean m_echoExceptions = true;   // when true, will print any exceptions thrown
	private boolean m_echoInput = false;       // set to false when reading files

	private java.io.PrintStream m_echoStream = System.out;   // where to print echoed input

	/**
	Constructs a new Scanner that produces values scanned from the specified 
	file. Bytes from the file are converted into characters using the 
	underlying platform's default charset.
	
	@param source A file to be scanned
	@throws IORuntimeException if source is not found or cannot be read
	 */
	public SimpleScanner(File source) {
		this(source, false);
	}

	/**
	Constructs a new Scanner that produces values scanned from the specified 
	file. Bytes from the file are converted into characters using the 
	underlying platform's default charset.
	
	@param source A file to be scanned
	@throws IORuntimeException if source is not found or cannot be read
	 */
	private SimpleScanner(File source, boolean echo) {
		// possibly redirect working directories based on VM argument
		m_source = "File: " + source;
		String workingDir = getWorkingDir();
		if (workingDir != null) {
			source = new File(workingDir, source.getName());
		}

		if (getAppletMode()) {
			// applets can't read files; use ClassLoader instead
			InputStream stream = getClass().getResourceAsStream(source.toString());
			if (stream == null) {
				throw new NullPointerException("\nApplet resource missing: " + source);
			}
			_init(new InputStreamReader(stream), echo);
		} else {
			try {
				_init(new FileReader(source), echo);
			} catch (IOException ioe) {
				throw new IORuntimeException("unable to read file " + source, ioe);
			}
		}
		m_showLineNumbers = true;
	}

	/**
     Constructs a new Scanner that produces values scanned from the specified 
     input stream. Bytes from the stream are converted into characters using 
     the underlying platform's default charset.

     @param source An input stream to be scanned
	 */
	public SimpleScanner(InputStream source) {
		this(source, false);
	}

	/**
    Constructs a new Scanner that produces values scanned from the specified 
    input stream. Bytes from the stream are converted into characters using 
    the underlying platform's default charset.

    @param source An input stream to be scanned
    @param echo Whether to echo (repeat) input to System.out as it is read
	 */
	private SimpleScanner(InputStream source, boolean echo) {
		// special case for console input, to accommodate multiple Scanners
		if (DEBUG) System.out.println("    Scanner(InputStream, echo=" + echo + ")");

		m_source = "InputStream: " + source;

		// don't show line numbers on exceptions unless reading from files
		m_showLineNumbers = (source != System.in);

		if (source == System.in && getReuse()) {
			if (s_reader == null || s_lnReader == null || s_SystemIn != System.in) {
				if (DEBUG) System.out.println("    Scanner() System.in starting anew");
				_init(new InputStreamReader(source), echo);
				s_reader = m_reader;
				s_lnReader = m_lnReader;
				s_SystemIn = System.in;
			} else {
				// re-use existing readers
				m_reader = s_reader;
				m_lnReader = s_lnReader;

				// very special case: If a student makes multiple System.in scanners,
				// suddenly the next() / nextLine() pattern works.
				// I should match this by chomping the leading \n if any.
				m_echoExceptions = false;
				try {
					if (hasNextLine()) {
						char peekChar = (char) _peek();
						if (peekChar == '\r' || peekChar == '\n') {
							if (DEBUG) System.out.println("    Scanner() multiple System.in Scanners; throwing away blank line");
							nextLine();   // throw away blank line
						}
					}
				} catch (IOException e) {}
				m_echoExceptions = true;
			}
		} else {
			// normal initialization
			_init(new InputStreamReader(source), echo);
		}

		// only echo from System.in, not from file input
		m_echoInput = echo || (getEcho() && source == System.in);
	}

	/**
     Constructs a new Scanner that produces values scanned from the specified
     source.

     @param source A character source to scan
	 */
	public SimpleScanner(Reader source) {
		this(source, false);
	}

	private SimpleScanner(Reader source, boolean echo) {
		m_source = "Reader: " + source;
		_init(source, echo);
	}

	/**
     Constructs a new Scanner that produces values scanned from the specified
     string.

     @param source A string to scan
	 */
	public SimpleScanner(String source) {
		this(source, false);
	}

	private SimpleScanner(String source, boolean echo) {
		m_source = "String: " + source;
		_init(new StringReader(source), echo);
	}

	// Does the real work to initialize the Scanner.
	private void _init(Reader source, boolean echo) {
		m_lnReader = new LineNumberReader(source);
		m_lnReader.setLineNumber(1);  // it would be 0-based otherwise
		m_reader = new PushbackReader(m_lnReader, PUSHBACK_BUFFER_SIZE);
		m_echoInput = echo;
	}

	/**
     Closes this scanner.

     <p>
     If this scanner is already closed then invoking 
     this method will have no effect.
	 */
	public void close() {
		try {
			m_reader.close();
			m_closed = true;
		} catch (IOException ioe) {
			_setIoException(ioe);
		}
	}

	/**
     Returns the Pattern this Scanner is currently using to match delimiters.

     @return this scanner's delimiting pattern.
	 */
	public Pattern delimiter() {
		return this.m_delimiter;
	}

	/**
     Attempts to find the next occurrence of the specified pattern.

     This method searches through the input up to the specified search 
     horizon, ignoring delimiters. If the pattern is found the scanner 
     advances past the input that matched and returns the string that 
     matched the pattern. If no such pattern is detected then the null is
     returned and the scanner's position remains unchanged. This method may
     block waiting for input that matches the pattern.

     A scanner will never search more than horizon code points beyond its
     current position. Note that a match may be clipped by the horizon;
     that is, an arbitrary match result may have been different if the 
     horizon had been larger. The scanner treats the horizon as a 
     transparent, non-anchoring bound.

     If horizon is 0, then the horizon is ignored and this method continues
     to search through the input looking for the specified pattern without
     bound. In this case it may buffer all of the input searching for the 
     pattern.

     If horizon is negative, then an IllegalArgumentException is thrown.

     @param pattern the pattern to scan for 
     @return the text that matched the specified pattern 
     @throws IllegalStateException if this scanner is closed 
     @throws IllegalArgumentException if horizon is negative
	 */
	public String findWithinHorizon(Pattern pattern, int horizon) {
		throw new UnsupportedOperationException();
	}

	/**
     Attempts to find the next occurrence of a pattern constructed from the 
     specified string, ignoring delimiters.

     An invocation of this method of the form findWithinHorizon(pattern) 
     behaves in exactly the same way as the invocation 
     findWithinHorizon(Pattern.compile(pattern, horizon)).

     @param pattern a string specifying the pattern to search for 
     @return the text that matched the specified pattern 
     @throws IllegalStateException if this scanner is closed 
     @throws IllegalArgumentException if horizon is negative
	 */
	public String findWithinHorizon(String pattern, int horizon) {
		return findWithinHorizon(Pattern.compile(pattern), horizon);
	}

	/**
     Attempts to find the next occurrence of the specified pattern ignoring 
     delimiters. If the pattern is found before the next line separator, the
     scanner advances past the input that matched and returns the string that
     matched the pattern. If no such pattern is detected in the input up to 
     the next line separator, then null is returned and the scanner's position
     is unchanged. This method may block waiting for input that matches the 
     pattern.

     Since this method continues to search through the input looking for the 
     specified pattern, it may buffer all of the input searching for the 
     desired token if no line separators are present.

     @param pattern the pattern to scan for 
     @return the text that matched the specified pattern 
     @throws IllegalStateException - if this scanner is closed
	 */
	public String findInLine(Pattern pattern) {
		throw new UnsupportedOperationException();
	}

	/**
     Attempts to find the next occurrence of a pattern constructed from the 
     specified string, ignoring delimiters.

     An invocation of this method of the form findInLine(pattern) behaves in 
     exactly the same way as the invocation findInLine(Pattern.compile(pattern)).

     @param pattern a string specifying the pattern to search for 
     @return the text that matched the specified pattern 
     @throws IllegalStateException if this scanner is closed
	 */
	public String findInLine(String pattern) {
		return findInLine(Pattern.compile(pattern));
	}

	/**
     Returns true if this scanner has another token in its input. This method 
     may block while waiting for input to scan. The scanner does not advance 
     past any input.

     @return true if and only if this scanner has another token
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNext() {
		if (DEBUG) System.out.println("    Scanner.hasNext()");
		try {
			next();
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next complete token matches the specified pattern.
     A complete token is prefixed and postfixed by input that matches
     the delimiter pattern. This method may block while waiting for input.
     The scanner does not advance past any input.

     @param pattern the pattern to scan for
     @return true if and only if this scanner has another token matching
             the specified pattern
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNext(Pattern pattern) {
		// temporarily use this pattern as delimiter, search, then put it back
		Pattern oldDelimiter = this.delimiter();
		useDelimiter(pattern);
		boolean result = hasNext();
		useDelimiter(oldDelimiter);
		return result;
	}

	/**
     Returns the next token if it matches the pattern constructed from the 
     specified string.  If the match is successful, the scanner advances 
     past the input that matched the pattern.

     An invocation of this method of the form next(pattern)
     behaves in exactly the same way as the invocation 
     next(Pattern.compile(pattern)).

     @param pattern a string specifying the pattern to scan
     @return the next token
     @throws NoSuchElementException if no such tokens are available
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNext(String pattern) {
		return hasNext(Pattern.compile(pattern));
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted
     as a BigDecimal using the nextBigDecimal() method. The scanner does not 
     advance past any input.

     @return true if and only if this scanner's next token is a valid BigDecimal
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextBigDecimal() {
		try {
			nextBigDecimal();
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted
     as a BigInteger in the default radix using the nextBigInteger() method. 
     The scanner does not advance past any input.

     @return true if and only if this scanner's next token is a valid BigInteger 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextBigInteger() {
		return hasNextBigInteger(m_radix);
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a BigInteger in the specified radix using the nextBigInteger() method. The 
     scanner does not advance past any input.

     @param radix the radix used to interpret the token as an integer 
     @return true if and only if this scanner's next token is a valid BigInteger 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextBigInteger(int radix) {
		try {
			nextBigInteger(radix);
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a boolean value. The scanner does not advance past the input that matched.

     @return true if and only if this scanner's next token is a valid boolean value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextBoolean() {
		try {
			nextBoolean();
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as 
     a byte value in the default radix using the nextByte() method. The scanner 
     does not advance past any input.

     @return true if and only if this scanner's next token is a valid byte value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextByte() {
		return hasNextByte(m_radix);
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a byte value in the specified radix using the nextByte() method. The scanner 
     does not advance past any input.

     @param radix the radix used to interpret the token as a byte value 
     @return true if and only if this scanner's next token is a valid byte value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextByte(int radix) {
		try {
			nextByte(radix);
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a double value using the nextDouble() method. The scanner does not advance 
     past any input.

     @return true if and only if this scanner's next token is a valid double value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextDouble() {
		if (DEBUG) System.out.println("    Scanner.hasNextDouble()");
		try {
			nextDouble();
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a float value using the nextFloat() method. The scanner does not advance past 
     any input.

     @return true if and only if this scanner's next token is a valid float value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextFloat() {
		try {
			nextFloat();
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     an int value in the default radix using the nextInt() method. The scanner
     does not advance past any input.

     @return true if and only if this scanner's next token is a valid int value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextInt() {
		if (DEBUG) System.out.println("    Scanner.hasNextInt()");
		return hasNextInt(m_radix);
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as 
     an int value in the specified radix using the nextInt() method. The scanner 
     does not advance past any input.

     @param radix the radix used to interpret the token as an int value 
     @return true if and only if this scanner's next token is a valid int value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextInt(int radix) {
		try {
			nextInt(radix);
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if there is another line in the input of this scanner. This
     method may block while waiting for input. The scanner does not advance past 
     any input.

     @return true if and only if this scanner has another line of input 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextLine() {
		if (DEBUG) System.out.println("    Scanner.hasNextLine()");
		return _hasNextChar();
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a long value in the default radix using the nextLong() method. The scanner
     does not advance past any input.

     @return true if and only if this scanner's next token is a valid long value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextLong() {
		return hasNextLong(m_radix);
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a long value in the specified radix using the nextLong() method. The scanner
     does not advance past any input.

     @return true if and only if this scanner's next token is a valid long value 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextLong(int radix) {
		try {
			nextLong(radix);
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as 
     a short value in the default radix using the nextShort() method. The scanner 
     does not advance past any input.

     @return true if and only if this scanner's next token is a valid short value
     in the default radix 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextShort() {
		return hasNextShort(m_radix);
	}

	/**
     Returns true if the next token in this scanner's input can be interpreted as
     a short value in the specified radix using the nextShort() method. The scanner
     does not advance past any input.

     @param radix the radix used to interpret the token as a short value 
     @return true if and only if this scanner's next token is a valid short value
     in the specified radix 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean hasNextShort(int radix) {
		try {
			nextShort(radix);
			return true;
		} catch (NoSuchElementException nsee) {
			return false;
		} finally {
			_undoNext();
		}
	}

	/**
     Returns the IOException last thrown by this Scanner. This method returns
     null if no such exception exists.

     @return the last exception thrown by this scanner's readable
	 */
	public IOException ioException() {
		return m_ioException;
	}

	/**
     Returns this scanner's locale.

     A scanner's locale affects many elements of its default primitive matching regular expressions.

     @return this scanner's locale
	 */
	public Locale locale() {
		return this.m_locale;
	}

	/**
     Returns the match result of the last scanning operation performed by this
     scanner. This method throws IllegalStateException if no match has been 
     performed, or if the last match was not successful.

     The various next methods of Scanner make a match result available if they
     complete without throwing an exception. For instance, after an invocation
     of the nextInt() method that returned an int, this method returns a
     MatchResult for the search of the Integer regular expression defined
     above. Similarly the findInLine(java.lang.String),
     findWithinHorizon(java.lang.String, int), and skip(java.util.regex.Pattern)
     methods will make a match available if they succeed.

     @return a match result for the last match operation 
     @throws IllegalStateException If no match result is available
	 */
	/*
     // commented out because MatchResult does not exist in Java 1.4
     public MatchResult match() {
         throw new UnsupportedOperationException();
     }
	 */

	/**
     Finds and returns the next complete token from this scanner. A complete 
     token is preceded and followed by input that matches the delimiter pattern.
     This method may block while waiting for input to scan, even if a previous 
     invocation of hasNext() returned true.

     @return the next token
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public String next() {
		if (DEBUG) System.out.println("    Scanner.next()");

		// wipe buffer of characters read by previous call to next()
		m_previousNextBuffer.setLength(0);

		try {
			// we will match either DELIM TOKEN DELIM
			// or                   TOKEN DELIM
			String returnValue = null;
			String peekStr = null;
			while (true) {
				boolean endOfInput = !_hasNextChar();
				peekStr = m_previousNextBuffer.toString();
				if (!endOfInput) {
					peekStr += (char) _peek();
				}

				Matcher matcher = m_delimiter.matcher(peekStr);
				if (matcher.find()) {
					// determine whether we have a leading delimiter
					boolean leadingDelim = (matcher.start() == 0);
					if (leadingDelim) {
						// must also have a trailing delimiter or end-of-input to stop reading
						int leadingEnd = matcher.end();
						if (matcher.find()) {
							if (endOfInput || matcher.end() <= peekStr.length()) {
								// we have a leading AND trailing delimiter; we can stop now
								returnValue = peekStr.substring(leadingEnd, matcher.start());
								break;
							}
						} else if (endOfInput) {
							// returnValue = peekStr.substring(leadingEnd);
							if (m_delimiter.matcher(peekStr).matches()) {
								// empty
							} else {
								returnValue = peekStr.substring(leadingEnd);
							}
						}
					} else {
						// no leading delimiter
						if (endOfInput || matcher.end() <= peekStr.length()) {
							// we have no leading delim, but we (greedily) found a trailing one; we can stop
							returnValue = peekStr.substring(0, matcher.start());
							break;
						}
					}
				} else if (endOfInput && peekStr.length() > 0) {
					// No delimiter remains, but we've reached the end of the input
					// and have a token to process.
					returnValue = peekStr;
				}

				if (endOfInput) {
					break;
				}

				char chr = (char) _nextChar();
				m_previousNextBuffer.append(chr);
			}



			if (returnValue == null) {
				throw _getNoSuchElementException(NO_TOKEN_MESSAGE);
			}

			// possibly echo the value typed by the user;
			// also peeks and pre-echos a trailing space or newline
			// (this doesn't quite work if the user types multiple inputs on one line
			// separated by more than one space, I think... oh well)
			if (m_echoInput && !_inHasNextMethod()) {
				String toPrint = returnValue;
				char peek = peekStr.charAt(peekStr.length() - 1);
				if (m_delimiter == DEFAULT_DELIMITER && Character.isWhitespace(peek)) {
					toPrint += peek;
				}
				System.out.print(toPrint);
			}

			return returnValue;

		} catch (IOException ioe) {
			_setIoException(ioe);
			return null;
		}
	}

	/**
     Returns the next token if it matches the specified pattern. This 
     method may block while waiting for input to scan, even if a previous
     invocation of hasNext(Pattern) returned true. 
     If the match is successful, the scanner advances past the input that 
     matched the pattern.

     @param pattern the pattern to scan for
     @return the next token
     @throws NoSuchElementException if no more tokens are available
     @throws IllegalStateException if this scanner is closed
	 */
	public String next(Pattern pattern) {
		Pattern oldDelimiter = this.delimiter();
		useDelimiter(pattern);
		String token = next();
		useDelimiter(oldDelimiter);
		return token;
	}

	/**
     Returns the next token if it matches the pattern constructed from the 
     specified string.  If the match is successful, the scanner advances 
     past the input that matched the pattern.

     An invocation of this method of the form next(pattern)
     behaves in exactly the same way as the invocation 
     next(Pattern.compile(pattern)).

     @param pattern a string specifying the pattern to scan
     @return the next token
     @throws NoSuchElementException if no such tokens are available
     @throws IllegalStateException if this scanner is closed
	 */
	public String next(String pattern) {
		return next(Pattern.compile(pattern));
	}

	/**
     Scans the next token of the input as a BigDecimal.

     @return the BigDecimal scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public BigDecimal nextBigDecimal() {
		try {
			return new BigDecimal(next());
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(BigDecimal.class);
		}
	}

	/**
     Scans the next token of the input as a BigInteger.

     @return the BigInteger scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public BigInteger nextBigInteger() {
		return nextBigInteger(m_radix);
	}

	/**
     Scans the next token of the input as a BigInteger.

     @param radix the radix used to interpret the token
     @return the BigInteger scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public BigInteger nextBigInteger(int radix) {
		try {
			return new BigInteger(next(), radix);
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(BigInteger.class);
		}
	}

	/**
     Scans the next token of the input into a boolean value and returns that
     value. This method will throw FormatException if the next token cannot 
     be translated into a valid boolean value. If the match is successful, 
     the scanner advances past the input that matched.

     @return the boolean scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public boolean nextBoolean() {
		// Boolean.parseBoolean doesn't do what we want here; it assumes all 
		// strings other than "true" are false rather than invalid
		String token = next();
		if (token.equalsIgnoreCase(TRUE))
			return true;
		else if (token.equalsIgnoreCase(FALSE))
			return false;
		else
			throw _getNoSuchElementException(Boolean.TYPE);
	}

	/**
     Scans the next token of the input as a byte.

     <p>
     An invocation of this method of the form nextByte() behaves in exactly the
     same way as the invocation nextByte(radix), where radix is the default radix
     of this scanner.

     @return the byte scanned from the input 
     @throws NoSuchElementException if input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public byte nextByte() {
		return nextByte(m_radix);
	}

	/**
     Scans the next token of the input as a byte.

     @param radix the radix used to interpret the token as a byte value 
     @return the byte scanned from the input 
     @throws InputMismatchException if the next token does not match the 
     Integer regular expression, or is out of range 
     @throws NoSuchElementException if input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public byte nextByte(int radix) {
		try {
			return Byte.parseByte(next());
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(Byte.TYPE);
		}
	}

	/**
     Scans the next token of the input as a double. This method will throw
     NumberFormatException if the next token cannot be translated into a valid
     double value. If the translation is successful, the scanner advances past
     the input that matched.

     @return the double scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public double nextDouble() {
		if (DEBUG) System.out.println("    Scanner.nextDouble()");
		try {
			return Double.parseDouble(next());
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(Double.TYPE);
		}
	}

	/**
     Scans the next token of the input as a float. This method will throw
     NumberFormatException if the next token cannot be translated into a valid
     float value. If the translation is successful, the scanner advances past
     the input that matched.

     @return the float scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public float nextFloat() {
		try {
			return Float.parseFloat(next());
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(Float.TYPE);
		}
	}

	/**
     Scans the next token of the input as an int. This method will throw
     NumberFormatException if the next token cannot be translated into a valid
     int value. If the translation is successful, the scanner advances past the
     input that matched.

     @return the int scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public int nextInt() {
		if (DEBUG) System.out.println("    Scanner.nextInt()");
		return nextInt(m_radix);
	}

	/**
     Scans the next token of the input as an int. This method will throw
     FormatException if the next token cannot be translated into a valid 
     int value. If the translation is successful, the scanner advances past the
     input that matched.

     @param radix the radix used to interpret the token as an int value
     @return the int scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public int nextInt(int radix) {
		try {
			return Integer.parseInt(next(), radix);
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(Integer.TYPE);
		}
	}

	/**
     Advances this scanner past the current line and returns the input that was
     skipped. This method returns the rest of the current line, excluding any
     line separator at the end. The position is set to the beginning of the 
     next line.

     Since this method continues to search through the input looking for a line
     separator, it may buffer all of the input searching for the line to skip
     if no line separators are present. 

     @return the line that was skipped
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public String nextLine() {
		if (DEBUG) System.out.println("    Scanner.nextLine()");
		if (!hasNextLine())
			throw _getNoSuchElementException(NO_TOKEN_MESSAGE);

		StringBuffer result = new StringBuffer();
		while (_hasNextChar()) {
			char next = (char)_nextChar();

			// don't put the newline separator into the result
			if (next == '\n')
				break;
			else if (next == '\r')
				continue;

			result.append(next);
		}

		String returnValue = result.toString();
		if (m_echoInput) {
			m_echoStream.println(returnValue);
		}
		return returnValue;
	}

	/**
     Scans the next token of the input as a long. This method will throw
     NumberFormatException if the next token cannot be translated into a valid
     long value. If the translation is successful, the scanner advances past the
     input that matched.

     @return the long scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public long nextLong() {
		return nextLong(m_radix);
	}

	/**
     Scans the next token of the input as a long. This method will throw
     FormatException if the next token cannot be translated into a valid 
     long value. If the translation is successful, the scanner advances past the
     input that matched.

     @param radix the radix used to interpret the token as a long value
     @return the long scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public long nextLong(int radix) {
		try {
			return Long.parseLong(next(), radix);
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(Long.TYPE);
		}
	}

	/**
     Scans the next token of the input as a short. This method will throw
     NumberFormatException if the next token cannot be translated into a valid
     short value. If the translation is successful, the scanner advances past the
     input that matched.

     @return the short scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public short nextShort() {
		return nextShort(m_radix);
	}

	/**
     Scans the next token of the input as a short. This method will throw
     FormatException if the next token cannot be translated into a valid 
     short value. If the translation is successful, the scanner advances past the
     input that matched.

     @param radix the radix used to interpret the token as a short value
     @return the long scanned from the input
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public short nextShort(int radix) {
		try {
			return Short.parseShort(next(), radix);
		} catch (NumberFormatException nfe) {
			throw _getNoSuchElementException(Short.TYPE);
		}
	}

	/**
     Returns this scanner's default radix.

     <p>
     A scanner's radix affects elements of its default number matching 
     regular expressions.

     @return the default radix of this scanner
     @throws NoSuchElementException if the input is exhausted 
     @throws IllegalStateException if this scanner is closed
	 */
	public int radix() {
		return m_radix;
	}

	/**
     The remove operation is not supported by this implementation of Iterator.

     @throws UnsupportedOperationException if this method is invoked.
	 */
	public void remove() {
		throw new UnsupportedOperationException(REMOVE_EXCEPTION_MESSAGE);
	}

	public void setEchoForce(boolean value) {
		m_echoInput = value;
	}

	public void setEchoStream(java.io.PrintStream stream) {
		m_echoStream = stream;
	}

	/**
     Skips input that matches the specified pattern, ignoring delimiters. This
     method will skip input if an anchored match of the specified pattern 
     succeeds.

     If a match to the specified pattern is not found at the current position,
     then no input is skipped and a NoSuchElementException is thrown.

     Since this method seeks to match the specified pattern starting at the
     scanner's current position, patterns that can match a lot of input (".*",
     for example) may cause the scanner to buffer a large amount of input.

     Note that it is possible to skip something without risking a 
     NoSuchElementException by using a pattern that can match nothing, e.g.,
     sc.skip("[ \t]*").

     @param pattern a string specifying the pattern to skip over 
     @return this scanner 
     @throws NoSuchElementException if the specified pattern is not found 
     @throws IllegalStateException if this scanner is closed
	 */
	public SimpleScanner skip(Pattern pattern) {
		m_previousNextBuffer.setLength(0);
		try {
			while (_hasNextChar()) {
				String peekStr = m_previousNextBuffer.toString() + (char)_peek();
				Matcher matcher = pattern.matcher(peekStr);
				if (matcher.find() && matcher.start() == 0 && matcher.end() != peekStr.length()) {
					// we have seen the end of the pattern; we can stop now
					return this;
				}

				char chr = (char)_nextChar();
				m_previousNextBuffer.append(chr);
			}

			throw _getNoSuchElementException(NO_TOKEN_MESSAGE);

		} catch (IOException ioe) {
			_setIoException(ioe);
			return null;
		}
	}

	/**
     Skips input that matches a pattern constructed from the specified string.

     An invocation of this method of the form skip(pattern) behaves in exactly
     the same way as the invocation skip(Pattern.compile(pattern)).

     @param pattern a string specifying the pattern to skip over 
     @return this scanner 
     @throws IllegalStateException if this scanner is closed
	 */
	public SimpleScanner skip(String pattern) {
		return skip(Pattern.compile(pattern));
	}

	/**
     Returns the string representation of this Scanner. The string 
     representation of a Scanner contains information that may be useful 
     for debugging. The exact format is unspecified.

     @return The string representation of this scanner
	 */
	public String toString() {
		return this.getClass().getName();
	}

	/**
     Sets this scanner's delimiting pattern to the specified pattern.

     @param pattern A delimiting pattern
     @return this scanner
	 */
	public SimpleScanner useDelimiter(Pattern pattern) {
		if (DEBUG) System.out.println("    Scanner.useDelimeter(" + pattern + ")");
		if (pattern == null) {
			throw new NullPointerException();
		}

		this.m_delimiter = pattern;
		return this;
	}

	/**
     Sets this scanner's delimiting pattern to a pattern constructed from the specified String.

     An invocation of this method of the form useDelimiter(pattern) behaves in exactly the same way as the invocation hasDelimiter(Pattern.compile(pattern)). 

     @param pattern A string specifying a delimiting pattern
     @return this scanner
	 */
	public SimpleScanner useDelimiter(String pattern) {
		return this.useDelimiter(Pattern.compile(pattern));
	}

	/**
     Sets this scanner's locale to the specified locale.

     A scanner's locale affects many elements of its default primitive matching regular expressions; see localized numbers above.

     @param locale A string specifying the locale to use 
     @return this scanner
	 */
	public SimpleScanner useLocale(Locale locale) {
		this.m_locale = locale;
		return this;
	}

	/**
     Sets this scanner's default radix to the specified radix.

     <p>
     A scanner's radix affects elements of its default number matching 
     regular expressions.

     <p>
     If the radix is less than Character.MIN_RADIX or greater than 
     Character.MAX_RADIX, then an IllegalArgumentException is thrown. 

     @throws IllegalArgumentException if radix is out of range
	 */
	public SimpleScanner useRadix(int radix) {
		if (radix < Character.MIN_RADIX || radix > Character.MAX_RADIX)
			throw new IllegalArgumentException();

		this.m_radix = radix;
		return this;
	}

	// common function to make sure input has not been exhausted
	private boolean _hasNextChar() {
		try {
			int chr = _nextChar();
			if (chr == EOF)
				return false;

			_unread(chr);
			return true;
		} catch (IOException ioe) {
			_setIoException(ioe);
			return false;
		} catch (NoSuchElementException nsee) {
			return false;
		}
	}

	// advances one character in the input
	private int _nextChar() {
		if (m_closed)
			throw new IllegalStateException();

		try {
			return m_reader.read();
		} catch (IOException ioe) {
			_setIoException(ioe);
			return EOF;
		}
	}

	// returns next character in the input without advancing the reader
	private int _peek() throws IOException {
		int peekChar = _nextChar();
		_unread(peekChar);
		return peekChar;
	}

	private NoSuchElementException _getNoSuchElementException(String message) {
		if (m_showLineNumbers) {
			int lineNum = m_lnReader.getLineNumber();
			message += NEAR_LINE_MESSAGE + lineNum;
		}
		return new NoSuchElementException(message);
	}

	private NoSuchElementException _getNoSuchElementException(Class<?> clazz) {
		String message = "";
		if (m_showLineNumbers) {
			int lineNum = m_lnReader.getLineNumber();
			message += NEAR_LINE_MESSAGE + lineNum;
		}

		message += "token '" + m_previousNextBuffer + 
				"' cannot be interpreted as type " + clazz.getName();
		return new NoSuchElementException(message);
	}

	// sets internal io exception
	private void _setIoException(IOException ioe) {
		m_ioException = ioe;
		if (m_echoExceptions) {
			System.err.println("Exception thrown by Scanner reading from " + m_source);
			ioe.printStackTrace();
		}

		// BUGFIX: if there's a failure reading from the source, throw out any cached readers
		clearCache();
	}

	// unreads buffer of characters that were consumed by
	// the last call to next()
	private void _undoNext() {
		if (m_previousNextBuffer.length() > 0) {
			try {
				m_reader.unread(m_previousNextBuffer.toString().toCharArray());
			} catch (IOException ioe) {
				_setIoException(ioe);
			}
		}
	}

	// put given value back into the input stream
	private void _unread(int chr) throws IOException {
		m_reader.unread(chr);
	}
}
