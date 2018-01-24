/*
 * @author Marty Stepp
 * @version 2015/05/28
 * - added filterStackTrace
 */

package stanford.cs106.util;

import java.io.*;

import java.util.*;

/**
 * This class contains utility code related to exceptions.
 * A lot of the methods are related to examining an exception's stack trace
 * to learn things about what methods/classes are on the call stack.
 * This helps Practice-It figure out what went wrong in a student's code and where.
 * 
 * @author Marty Stepp
 */

public class ExceptionUtils {
	/** So that an object of this class cannot be constructed. */
	private ExceptionUtils() {
		// empty
	}
	
	public static String filterStackTrace(String stackTrace, String... packagesToStrip) {
		String[] lines = stackTrace.split("\r?\n");
		List<String> outLines = new ArrayList<String>();
		for (String line : lines) {
			boolean tainted = false;
			for (String packageStr : packagesToStrip) {
				if (line.contains("at " + packageStr)) {
					tainted = true;
					break;
				}
			}
			if (!tainted) {
				outLines.add(line);
			}
		}
		return StringUtils.join(outLines, "\n");
	}
	
	/**
	 * The current stack trace as a multi-line String.
	 */
	public static String stackTraceToString() {
		return stackTraceToString(new RuntimeException());
	}
	
	/**
	 * The current stack trace as a multi-line String.
	 */
	public static String stackTraceToString(int lines) {
		return stackTraceToString(new RuntimeException(), lines);
	}
	
	/**
	 * The stack trace of the given exception as a multi-line String.
	 */
	public static String stackTraceToString(Throwable throwable) {
		return stackTraceToString(throwable, -1);
	}
	
	/**
	 * The stack trace of the given exception as a multi-line String.
	 */
	public static String stackTraceToString(Throwable throwable, int lines) {
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		throwable.printStackTrace(new PrintStream(bytes));
		String result = bytes.toString();
		
		Throwable cause = throwable.getCause();
		if (cause != null) {
			result += "\n\ncaused by: " + stackTraceToString(cause);
		}
		
		if (lines >= 0) {
			result = StringUtils.fitToHeight(result, lines);
		}
		
		return result;
	}
	
	/**
	 * The top-level line number within the given class name on which
	 * this exception occurred.  For example, if you have the exception:
	 * 
	 * <pre>
	 * RuntimeException
	 *     at Foo.java line 42
	 *     at Bar.java line 18
	 *     at Bar.java line 29
	 * </pre>
	 * 
	 * and you ask for the line number from Bar, returns 18.
	 * @return -1 if the exception doesn't contain a stack frame for this class
	 */
	public static int getLineNumber(Throwable throwable, String className) {
		return getLineNumber(throwable, className, null);
	}
	
	/**
	 * The top-level line number within the given class and method name
	 * on which this exception occurred in the given method.  For example, if
	 * you have the exception:
	 * 
	 * <pre>
	 * RuntimeException
	 *     at Foo.java:method0() line 42
	 *     at Bar.java:method1() line 96
	 *     at Bar.java:method2() line 18
	 *     at Bar.java:method2() line 29
	 * </pre>
	 * 
	 * and you ask for the line number from Bar, method2, returns 18.
	 * @return -1 if the exception doesn't contain a stack frame for this class/method
	 */
	public static int getLineNumber(Throwable throwable, String className, String methodName) {
		if (throwable != null) {
			for (StackTraceElement ste : throwable.getStackTrace()) {
				if (className == null || ste.getClassName().contains(className)) {
					if (methodName == null || ste.getMethodName().equals(methodName)) {
						return ste.getLineNumber() + 1;
					}
				}
			}
		}
		return -1;
	}

	/**
	 * The current class name on top of the current method call stack,
	 * ignoring any classes in the given list of classes to skip.
	 * @return the class name, or null if none can be determined (unlikely)
	 */
	public static String getCallingClassName(String... classNamesToSkip) {
		RuntimeException re = new RuntimeException();
		for (StackTraceElement ste : re.getStackTrace()) {
			for (String classNameToSkip : classNamesToSkip) {
				String className = ste.getClassName();
				if (className.contains("ExceptionUtils")) {
					continue;
				}
				
				if (className.contains(classNameToSkip)) {
					return className;
				}
			}
		}
		return null;
	}

	/**
	 * The current method name within the given class name that is
	 * highest on the current method call stack.
	 * @return the method name, or empty string if no method from that class is on the stack
	 */
	public static String getMethodName(Throwable throwable, String className) {
		if (throwable != null) {
			for (StackTraceElement ste : throwable.getStackTrace()) {
				if (className == null || ste.getClassName().contains(className)) {
					return ste.getMethodName();
				}
			}
		}
		return "";
	}

	/**
	 * The true cause of an exception that has occurred.
	 * Basically calls t.getCause() until it hits the top level.
	 * If passed null, returns null.
	 */
	public static Throwable getUnderlyingCause(Throwable t) {
		while (t != null && t.getCause() != null) {
			t = t.getCause();
		}
		return t;
	}
}
