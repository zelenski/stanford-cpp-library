/*
 * @version 2017/05/22
 * - bug fix for readEntireStream (was stopping prematurely)
 * @version 2016/07/13
 * - added readEntireReader, readEntireScanner, readEntireStream
 */

package stanford.cs106.io;

import java.io.*;
import java.util.*;

/**
 * Utility methods related to file processing and I/O.
 * @author Marty Stepp
 */
public class IOUtils {
	/** A copy of the original System.in, which Practice-It replaces while student code is running. */
	private static final InputStream SYSTEM_IN_ORIGINAL = System.in;

	/** A copy of the original System.out, which Practice-It replaces while student code is running. */
	private static final PrintStream SYSTEM_OUT_ORIGINAL = System.out;

	/** A copy of the original System.err, which Practice-It replaces while student code is running. */
	private static final PrintStream SYSTEM_ERR_ORIGINAL = System.err;

	/** So that an object of this class cannot be constructed. */
	private IOUtils() {}

	/**
	 * Copies the entire contents of the given in file into the given out file.
	 */
	public static void copy(File in, File out) {
		try {
			FileInputStream fis  = new FileInputStream(in);
			FileOutputStream fos = new FileOutputStream(out);
			byte[] buf = new byte[4096];
			int len;
			while ((len = fis.read(buf)) >= 0) {
				fos.write(buf, 0, len);
			}
			fis.close();
			fos.close();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
	
	/**
	 * The directory in which the current app is running.
	 */
	public static String getCurrentDirectory() {
		String currentDir = ".";
		try {
			currentDir = System.getProperty("user.dir");
		} catch (Exception e) {
			// empty
		}
		return currentDir;
	}
	
	/**
	 * Returns a directory that is likely to contain expected input files for
	 * a running Stanford lib project.
	 * Checks in subfolders of the current directory such as input/, expected-output/, etc.
	 */
	public static File getExpectedInputDirectory() {
		File dir = new File(System.getProperty("user.dir"));
		File[] dirsToTry = {
				new File(dir, "input"),
				new File(dir, "res/input"),
				new File(dir, "output"),
				new File(dir, "expected-output"),
				new File(dir, "res/output"),
				new File(dir, "res/expected-output")
		};
		for (File dirToTry : dirsToTry) {
			if (dirToTry.isDirectory()) {
				dir = dirToTry;
				break;
			}
		}
		return dir;
	}
	
	/**
	 * Returns a directory that is likely to contain expected output files for
	 * a running Stanford lib project.
	 * Checks in subfolders of the current directory such as output/, expected-output/, etc.
	 */
	public static File getExpectedOutputDirectory() {
		File dir = new File(System.getProperty("user.dir"));
		File[] dirsToTry = {
				new File(dir, "output"),
				new File(dir, "expected-output"),
				new File(dir, "res/output"),
				new File(dir, "res/expected-output")
		};
		for (File dirToTry : dirsToTry) {
			if (dirToTry.isDirectory()) {
				dir = dirToTry;
				break;
			}
		}
		return dir;
	}
	
	/**
	 * The file name extension (such as mp3, doc, txt) of the given file's name.
	 * @return null if the file is null, or "" if the file has no extension
	 */
	public static String getExtension(File file) {
		if (file == null) {
			return null;
		}
		return getExtension(file.getName());
	}

	/**
	 * The file name extension (such as mp3, doc, txt) of the given file's name.
	 * @return null if the file is null, or "" if the file has no extension
	 */
	public static String getExtension(String fileName) {
		if (fileName == null) {
			return null;
		}
		int dot = fileName.lastIndexOf(".");
		if (dot >= 0) {
			return fileName.substring(dot + 1);
		} else {
			return "";
		}
	}
	
	/**
	 * Strips folders from file's name.  Like basename in Perl/PHP.
	 * e.g. "foo/bar/Baz.java" returns "Baz.java"
	 */
	public static String getFileName(String filePath) {
		return new File(filePath).getName();
	}

	// 
	/**
	 * The folder name of the given file path.
	 * e.g. "foo/bar/Baz.java" returns "foo/bar/"
	 * PRE: no folders in fileName (no "foo/bar/Baz.java")
	 */
	public static String getFolder(String fileName) {
		int slash = fileName.lastIndexOf(File.separatorChar);
		if (slash < 0) {
			slash = fileName.lastIndexOf("/");  // fallback
		}
		
		if (slash >= 0) {
			return fileName.substring(0, slash + 1);
		} else {
			return "./";
		}
	}

	/**
	 * The full path to the server's temp directory, such as "/tmp/foobar/".
	 */
	public static String getTempFolder() {
		String tempFolder = System.getProperty("java.io.tmpdir");
		if (!tempFolder.endsWith(File.separator)) {
			tempFolder += File.separator;
		}
		return tempFolder;
	}

	/**
	 * Reads the given file's text fully and returns it as a String.
	 * @throws FileNotFoundException if file cannot be read
	 */
	public static String readEntireFile(File file) {
		try {
			FileReader fr = new FileReader(file);
			return readEntireReader(fr);
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
	
	/**
	 * Reads the given file's text fully and returns it as a String.
	 * @throws FileNotFoundException if file cannot be read
	 */
	public static String readEntireFile(String fileName) {
		return readEntireFile(new File(fileName));
	}
	
	/**
	 * Reads the given reader's text fully and returns it as a String.
	 * @throws FileNotFoundException if file cannot be read
	 */
	public static String readEntireReader(Reader reader) {
		try {
			BufferedReader breader = new BufferedReader(reader);
			StringBuilder text = new StringBuilder();
			while (breader.ready()) {
				text.append((char) breader.read());
			}
			breader.close();
			reader.close();
			return text.toString();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
	
	/**
	 * Reads the given scanner's text fully and returns it as a String.
	 */
	public static String readEntireScanner(Scanner input) {
		StringBuilder text = new StringBuilder();
		while (input.hasNextLine()) {
			text.append(input.hasNextLine());
			text.append('\n');
		}
		input.close();
		return text.toString();
	}
	
	/**
	 * Reads the given stream's text fully and returns it as a String.
	 * @throws FileNotFoundException if file cannot be read
	 */
	public static String readEntireStream(InputStream stream) {
		try {
			BufferedReader breader = new BufferedReader(new InputStreamReader(stream));
			StringBuilder text = new StringBuilder();
			while (true) {
				int b = breader.read();
				if (b < 0) {
					break;
				}
				text.append((char) b);
			}
			breader.close();
			stream.close();
			return text.toString();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
	
	/**
	 * Reads the given stream's text fully and returns it as an array of bytes.
	 * @throws IOException if stream cannot be read
	 */
	public static byte[] readEntireStreamBytes(InputStream stream) {
		return readEntireReaderBytes(new InputStreamReader(stream));
	}

	/**
	 * Reads the given reader's text fully and returns it as an array of bytes.
	 * @throws IOException if stream cannot be read
	 */
	public static byte[] readEntireReaderBytes(Reader reader) {
		try {
			ByteArrayOutputStream out = new ByteArrayOutputStream();
			int count = 0;
			while (reader.ready()) {
				int b = reader.read();
				out.write(b);
				System.out.printf("%4d: %3d (%s)\n", count, b, (char) b);
				count++;
			}
			return out.toByteArray();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}

	/**
	 * Reads the full text from the file with the given name,
	 * and returns it as an array of bytes.
	 * @throws IOException if stream cannot be read
	 */
	public static byte[] readEntireFileBytes(String fileName) {
		return readEntireFileBytes(new File(fileName));
	}
	
	/**
	 * Reads the full text from the given file,
	 * and returns it as an array of bytes.
	 * @throws IOException if stream cannot be read
	 */
	public static byte[] readEntireFileBytes(File file) {
		try {
			InputStream stream = new FileInputStream(file);
			byte[] bytes = new byte[(int) file.length()];
			int total = 0;
			while (total < bytes.length) {
				int bytesRead = stream.read(bytes, total, bytes.length - total);
				if (bytesRead < 0) {
					break;
				}
			}
			stream.close();
			return bytes;
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
	
	/**
	 * Creates and returns an input stream to read the characters of the given
	 * string as bytes, one character at a time.
	 */
	public static InputStream toInputStream(String data) {
		if (data == null) {
			data = "";
		}
		ByteArrayOutputStream stream = new ByteArrayOutputStream(data.length());
		PrintStream out = new PrintStream(stream);
		out.print(data);
		out.flush();
		out.close();
		ByteArrayInputStream in = new ByteArrayInputStream(stream.toByteArray());
		return in;
	}

	/**
	 * Redirects System.in to pull input data from the text in the given string.
	 */
	public static void redirectInput(String data) {
		synchronized (IOUtils.class) {
			System.setIn(toInputStream(data));
		}
	}
	
	/**
	 * Redirects System.out to a byte array output stream, which is returned.
	 * Also sets a limit on the amount of output that can be produced before an
	 * ExcessiveOutputException will occur (so students can't infinitely println).
	 */
	public static ByteArrayOutputStream redirectOutput() {
		return redirectOutput(true);
	}

	/**
	 * Redirects System.out to a byte array output stream, which is returned.
	 * @param limit if true, sets a limit on the amount of output that can be
	 *              produced before an ExcessiveOutputException will occur
	 *              (so students can't infinitely println).
	 */
	public static ByteArrayOutputStream redirectOutput(boolean limit) {
		ByteArrayOutputStream out = null;
		System.out.flush();
		System.err.flush();
		out = new ByteArrayOutputStream(2048);
		PrintStream outErr = null;
		if (limit) {
			outErr = new LimitedPrintStream(out);
		} else {
			outErr = new PrintStream(out);
		}
		synchronized (IOUtils.class) {
			System.setOut(outErr);
			System.setErr(outErr);
		}
		return out;
	}

	/**
	 * Removes the file's extension; e.g. "foobar.mp3" returns "foobar"
	 */
	public static String removeExtension(String fileName) {
		int dot = fileName.lastIndexOf(".");
		if (dot >= 0) {
			fileName = fileName.substring(0, dot);
		}
		return fileName;
	}

	/**
	 * Removes any leading folders from the given file name;
	 * e.g. "foo/bar/Baz.java" returns "Baz.java"
	 */
	public static String removeFolder(String fileName) {
		int slash = fileName.lastIndexOf(File.separatorChar);
		if (slash < 0) {
			slash = fileName.lastIndexOf("/");  // fallback
		}

		if (slash >= 0) {
			fileName = fileName.substring(slash + 1);
		}
		return fileName;
	}

	/**
	 * Sets System.in back to its original state.
	 */
	public static void stopRedirectingInput() {
		synchronized (IOUtils.class) {
			System.setIn(SYSTEM_IN_ORIGINAL);
		}
	}

	/**
	 * Sets System.out/err back to their original state.
	 */
	public static void stopRedirectingOutput() {
		System.out.flush();
		System.err.flush();
		
		synchronized (IOUtils.class) {
			System.setOut(SYSTEM_OUT_ORIGINAL);
			System.setErr(SYSTEM_ERR_ORIGINAL);
		}
	}
	
	/**
	 * Writes the given text into the file with the given name, replacing any existing text.
	 */
	public static void writeEntireFile(String text, String fileName) {
		writeEntireFile(text, new File(fileName));
	}
	
	/**
	 * Writes the given text into the given file, replacing any existing text.
	 */
	public static void writeEntireFile(String text, File file) {
		try {
			java.io.PrintStream output = new java.io.PrintStream(file);
			output.print(text);
			output.close();
		} catch (FileNotFoundException fnfe) {
			fnfe.printStackTrace();  // this will never happen; we're writing the file!
		}
	}

	/**
	 * Writes the given bytes into the given file, replacing any existing content.
	 */
	public static void writeBytes(byte[] bytes, String fileName) {
		try {
			FileOutputStream output = new FileOutputStream(fileName);
			output.write(bytes);
			output.close();
		} catch (IOException ioe) {
			throw new IORuntimeException(ioe);
		}
	}
}
