package stanford.cs106.program;
import java.io.*;
import acm.program.*;

public class ConsoleProgramMarty extends ConsoleProgram {
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
