package stanford.cs106.program;
import java.io.*;
import acm.program.*;

public class CommandLineProgramMarty extends CommandLineProgram {
	protected Boolean isApplet = null;        // null = unchecked, false = no, true = yes
	
	protected boolean isApplet() {
		if (isApplet == null) {
			try {
				isApplet = false;
				if (java.lang.System.getSecurityManager() != null) {
					// applets run with security managers enabled
					isApplet = true;
				} else {
					File currentDir = new File(java.lang.System.getProperty("user.dir"));
					currentDir.exists();  // calling this will trigger security exception if applet
				}
			} catch (SecurityException sex) {
				isApplet = true;
			}
		}
		return isApplet;
	}
	
	protected boolean fileExists(String filename) throws IOException {
		return fileExistsOnDisk(filename) || fileExistsInsideJAR(filename);
	}
	
	protected boolean fileExistsInsideJAR(String filename) throws IOException {
		// fallback to using internal class stream (JAR or applet)
		InputStream stream = getClass().getResourceAsStream(filename);
		if (stream == null) {
			return false;
		} else {
			stream.close();
			return true;
		}
	}
	
	protected boolean fileExistsOnDisk(String filename) throws IOException {
		if (isApplet()) {
			return false;
		} else {
			// try reading a real file
			File file = new File(filename);
			try {
				if (file.exists()) {
					return true;
				} else {
					file = new File("../" + filename);  // "../simple.txt"
					if (file.exists()) {
						return true;
					}
				}
			} catch (SecurityException sex) {
				// running as an applet
			}
			return false;
		}
	}
	
	protected InputStream openFile(String filename) throws IOException {
		if (!isApplet() && fileExistsOnDisk(filename)) {
			return openFileFromDisk(filename);
		} else if (fileExistsInsideJAR(filename)) {
			return openFileFromJAR(filename);
		} else {
			throw new FileNotFoundException(filename);
		}
	}
	
	protected InputStream openFileFromDisk(String filename) throws IOException {
		// try reading a real file first
		File file = new File(filename);
		try {
			if (file.exists()) {
				return new BufferedInputStream(new FileInputStream(file));
			} else {
				file = new File("../" + filename);  // "../simple.txt"
				if (file.exists()) {
					return new BufferedInputStream(new FileInputStream(file));
				}
			}
		} catch (SecurityException sex) {
			// running as an applet
		}
		throw new FileNotFoundException(filename);
	}
	
	protected InputStream openFileFromJAR(String filename) throws IOException {
		// fallback to using internal class stream (JAR or applet)
		InputStream stream = getClass().getResourceAsStream(filename);
		if (stream == null) {
			throw new FileNotFoundException(filename);
		} else {
			return new BufferedInputStream(stream);
		}
	}
	
	protected String readEntireFile(String filename) throws IOException {
		return readEntireStream(openFile(filename));
	}
	
	protected String readEntireStream(InputStream stream) throws IOException {
		StringBuilder sb = new StringBuilder();
		int ch;
		while ((ch = stream.read()) != -1) {
			sb.append((char) ch);
		}
		return sb.toString();
	}
}
