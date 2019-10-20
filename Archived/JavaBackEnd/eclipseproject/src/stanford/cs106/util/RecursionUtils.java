package stanford.cs106.util;

/**
 * A class with a few "helper" methods for recursion and backtracking.
 */
public class RecursionUtils {
	private static final String INDENT = "    ";
	
	/*
	 * Private constructor to prevent instantiation.
	 */
	private RecursionUtils() {
		// empty
	}
	
	/**
	 * Returns the number of recursive calls currently on top of the call stack.
	 */
	public static int countCalls() {
		Throwable t = new RuntimeException();
		StackTraceElement[] methods = t.getStackTrace();
		
		// skip any methods of the Recursion class
		int startIndex = 1;
		while (startIndex < methods.length &&
				methods[startIndex].getClassName().equals(methods[0].getClassName())) {
			startIndex++;
		}
		return countCalls(startIndex + 1);   // go up 1 because we make a call
	}
	
	/**
	 * Prints the given value, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(boolean b) {
		println(String.valueOf(b));
	}
	
	/**
	 * Prints the given value, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(char c) {
		println(String.valueOf(c));
	}
	
	/**
	 * Prints the given value, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(double d) {
		println(String.valueOf(d));
	}
	
	/**
	 * Prints the given value, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(int n) {
		println(String.valueOf(n));
	}
	
	/**
	 * Prints the given value, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(long l) {
		println(String.valueOf(l));
	}
	
	/**
	 * Prints the given value, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(Object o) {
		println(String.valueOf(o));
	}
	
	/**
	 * Prints the given string, with each line indented relative to how many
	 * recursive calls are currently on top of the call stack.
	 */
	public static void println(String s) {
		if (s != null && s.contains("\n")) {
			// multi-line string; print each line indented
			String[] lines = s.split("[\r]?\n");
			for (String line : lines) {
				println(line);
			}
		} else {
			// single line; indent it and print it
			int calls = countCalls();
			for (int i = 0; i < calls - 1; i++) {
				System.out.print(INDENT);
			}
			System.out.println(s);
		}
	}
	
	// Returns the number of recursive calls currently on top of the call stack,
	// starting from the given index in the call stack.  (Used internally.)
	private static int countCalls(int startIndex) {
		Throwable t = new RuntimeException();
		StackTraceElement[] methods = t.getStackTrace();
		if (methods.length < startIndex) {
			return 0;
		}
		String className = methods[startIndex].getClassName();
		String methodName = methods[startIndex].getMethodName();
		int count = 1;
		for (int i = startIndex + 1; i < methods.length; i++) {
			if (methods[i].getClassName().equals(className) &&
					methods[i].getMethodName().equals(methodName)) {
				count++;
			} else {
				break;
			}
		}
		return count;
	}
}
