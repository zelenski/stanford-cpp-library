/*
 * @author Marty Stepp
 * @version 2015/05/28
 * - fixed assertNull, assertNotNull, assertSame, assertNotSame
 * - added support for stack traces in autograder test GUI
 */

package stanford.cs106.junit;

import java.lang.annotation.Annotation;
import java.lang.reflect.*;

import stanford.cs106.reflect.*;
import stanford.cs106.util.ExceptionUtils;
import stanford.cs106.util.StringUtils;

import java.util.*;

import org.junit.ComparisonFailure;

public class JUnitUtils {
	private static final int STACK_TRACE_LINES_TO_DISPLAY = 10;
	
	private JUnitUtils() {
		// empty
	}
	
	public static String getTestCategory(Class<?> clazz, Method method) {
		return getTestCategory(clazz, method, /* infer */ true);
	}
	
	public static String getTestCategory(Class<?> clazz, Method method, boolean infer) {
		Annotation anno = method.getAnnotation(TestCategory.class);
		if (anno == null) {
			if (infer) {
				// try to infer category from test method's name
				// format: test##_categoryName_testCaseName()
				// e.g. "test26_getAge_bothGenders" => test category is "getAge"
				String methodName = method.getName();
				if (methodName.matches("test[0-9]+_.*_.*")) {
					methodName = methodName.replaceAll("test[0-9]+_", "");
					methodName = methodName.replaceAll("_.*", "");
					return methodName;
				} else {
					return "";
				}
			}
		} else {
			Class<? extends Annotation> annoClass = anno.annotationType();
			Method valueMethod = ReflectionUtils.getMethod(annoClass, "value");
			if (valueMethod != null) {
				try {
					String categoryName = (String) valueMethod.invoke(anno);
					if (categoryName != null) {
						return categoryName;
					}
				} catch (IllegalAccessException e) {
					throw new ReflectionRuntimeException(e);
				} catch (InvocationTargetException e) {
					throw new ReflectionRuntimeException(e);
				}
			}
		}
		return "";
	}
	
	// e.g. "intro_test00_introOutputSpacing"
	public static String getTestFullName(Class<?> clazz, Method method) {
		String category = getTestCategory(clazz, method);
		String methodName = method.getName();
		if (category == null || category.isEmpty()) {
			return methodName;
		} else {
			return category + "_" + methodName;
		}
	}

	public static boolean isJUnitMethod(Class<?> clazz, Method method) {
		return method.getAnnotation(org.junit.Test.class) != null;
	}

	public static boolean isJUnitMethod(Class<?> clazz, String methodName) {
		return isJUnitMethod(clazz, ReflectionUtils.getMethod(clazz, methodName));
	}

	@SuppressWarnings("deprecation")
	public static void runJUnitMethod(final Class<?> junitClass, final Method method, final JUnitListener listener) {
		String testFullName = getTestFullName(junitClass, method);   // category + _ + testName
		Annotation annotation = method.getAnnotation(org.junit.Test.class);
		Class<? extends Annotation> annoClass = annotation.annotationType();
		
		// get timeout, if any
		long timeout = Integer.MAX_VALUE;
		Method timeoutMethod = ReflectionUtils.getMethod(annoClass, "timeout");
		if (timeoutMethod != null) {
			try {
				Long timeoutInteger = (Long) timeoutMethod.invoke(annotation);
				if (timeoutInteger != null) {
					timeout = timeoutInteger.longValue();
				}
			} catch (IllegalAccessException e) {
				throw new ReflectionRuntimeException(e);
			} catch (InvocationTargetException e) {
				throw new ReflectionRuntimeException(e);
			}
		}
		
		// get expected exception, if any
		Class<?> expectedExceptionClass = null;
		Method expectedMethod = ReflectionUtils.getMethod(annoClass, "expected");
		if (expectedMethod != null) {
			try {
				expectedExceptionClass = (Class<?>) expectedMethod.invoke(annotation);
				if (expectedExceptionClass != null && expectedExceptionClass.getName().endsWith("None")) {
					// special case: JUnit uses internal class "org.junit.Test$None" to indicate no exception;
					// we'll represent that as null instead
					expectedExceptionClass = null;
				}
			} catch (IllegalAccessException e) {
				throw new ReflectionRuntimeException(e);
			} catch (InvocationTargetException e) {
				throw new ReflectionRuntimeException(e);
			}
		}
		
		// create JUnit class instance and initialize it
		// TODO: support setup/teardown methods
		try {
			final Object junitObj = junitClass.newInstance();
			JUnitTestRunnable run = new JUnitTestRunnable();
			run.method = method;
			run.object = junitObj;
			Thread thread = new Thread(run);
			long startTime = System.currentTimeMillis();
			thread.start();
			thread.join(timeout);

			if (thread.isAlive()) {
				// timed out
				thread.stop();
				if (listener != null) {
					listener.setTestResult(testFullName, "fail");
					listener.setTestDetailsMessage(testFullName, "test timed out after " + timeout + "ms");
				}
			} else {
				if (listener != null && run.passed) {
					listener.setTestResult(testFullName, "pass");
				}
			}
			long endTime = System.currentTimeMillis();
			long runtimeMS = endTime - startTime;

			if (run.thrown != null) {
				listener.setTestResult(testFullName, "fail");
				Map<String, String> details = new TreeMap<String, String>();
				
				if (run.thrown instanceof ComparisonFailure) {
					ComparisonFailure failure = (ComparisonFailure) run.thrown;
					String expected = failure.getExpected();
					String actual = failure.getActual();
					String message = failure.getMessage();
					String detailsText = "";
					String testType = UnitTestType.ASSERT_EQUALS.toString();
					String valueType = "";
					
					if (run.thrown instanceof ComparisonFailureEnhanced) {
						ComparisonFailureEnhanced failure2 = (ComparisonFailureEnhanced) run.thrown;
						detailsText = failure2.getDetails();
						testType = failure2.getType().toString();
						valueType = failure2.getValueType();
						if (failure2.getType() == UnitTestType.ASSERT_DIFF) {
							message = detailsText;
						}
						
						// include a stack trace if possible
						String stackTrace = ExceptionUtils.stackTraceToString(run.thrown, STACK_TRACE_LINES_TO_DISPLAY);
						
						// filter out some packages that don't help the viewer
						// (sun internal packages; stanford internal library classes; etc.)
						stackTrace = ExceptionUtils.filterStackTrace(stackTrace,
								"sun.",
								"junit.",
								"org.",
								"java.lang.reflect.",
								"java.lang.Thread.",
								"stanford.");
						
						// remove first line which just restates the heading
						String[] stackTraceLines = stackTrace.split("\r?\n");
						stackTrace = StringUtils.join(stackTraceLines, "\n", /* start index */ 1, /* limit */ 0);
						
						// remove class names being shown twice
						stackTrace = stackTrace.replaceAll("at [a-zA-Z_0-9]+\\.", "at ");
						stackTrace = stackTrace.replaceAll("\\(", " (");
						details.put("stackTrace", stackTrace);
					}
					
					details.put("testType", testType);
					details.put("expected", expected);
					details.put("student", actual);
					details.put("actual", actual);
					details.put("message", message);
					details.put("valueType", valueType);
					details.put("passed", "false");
				} else if (run.thrown instanceof AssertionError) {
					AssertionError failure = (AssertionError) run.thrown;
					// System.out.println("FAILURE: " + failure);
					// System.out.println("CAUSE  : " + failure.getCause());
					String expected = "passed assertion";
					String actual = "failed assertion";
					String message = failure.getMessage();
					// String detailsText = "";
					String testType = UnitTestType.ASSERT_TRUE.toString();
					String valueType = "";
					
					details.put("testType", testType);
					details.put("expected", expected);
					details.put("student", actual);
					details.put("actual", actual);
					details.put("message", message);
					details.put("valueType", valueType);
					details.put("passed", "false");
				} else {
					// some other exception
					details.put("testType", UnitTestType.EXCEPTION.toString());
					// listener.setTestDetailsMessage(testName, "test threw: " + run.thrown);
					details.put("message", "test threw: " + run.thrown + "\n" + ExceptionUtils.stackTraceToString(run.thrown));
				}
				
				listener.setTestDetails(testFullName, details);
			}
			
			if (listener != null) {
				listener.setTestRuntime(testFullName, (int) runtimeMS);
			}
		} catch (InstantiationException e) {
			throw new ReflectionRuntimeException(e);
		} catch (IllegalAccessException e) {
			throw new ReflectionRuntimeException(e);
		} catch (InterruptedException e) {
			// timed out
		}
	}
	
	private static class JUnitTestRunnable implements Runnable {
		public Method method;
		public Object object;
		public Throwable thrown;
		public boolean passed = true;
		
		public void run() {
			try {
				method.invoke(object);
			} catch (Throwable t) {
				passed = false;
				thrown = ExceptionUtils.getUnderlyingCause(t);
			}
		}
	}
}
