package stanford.cs106.program;

import java.io.BufferedReader;
import java.io.StringReader;

import javax.swing.text.*;

import stanford.cs106.reflect.ReflectionUtils;
import acm.io.*;
import acm.program.*;

public class ProgramUtils {
	private ProgramUtils() {
		// empty
	}
	
	public static void injectConsoleInput(ConsoleProgram program, String input) {
		program.getConsole().setInputScript(new BufferedReader(new StringReader(input)));
	}
	
	public static String getConsoleOutput(ConsoleProgram program) {
		IOConsole console = program.getConsole();
		Object model = ReflectionUtils.getFieldValue(console, "consoleModel");
		Document document = (Document) ReflectionUtils.getFieldValue(model, "document");
		try {
			return document.getText(0, document.getLength());
		} catch (BadLocationException e) {
			throw new RuntimeException(e);
		}
	}
}
