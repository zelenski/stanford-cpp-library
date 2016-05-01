/*
 * @author Marty Stepp
 * @version 2015/05/28
 * - fixed assertNull, assertNotNull, assertSame, assertNotSame
 */

package stanford.cs106.junit;

import stanford.cs106.diff.*;

public class Assert {
	private Assert() {
		// empty
	}
	
	public static void assertDiff(String message, String expected, String actual) {
		assertDiff(message, expected, actual, Diff.FLAGS_DEFAULT_LENIENT);
	}
	
	public static void assertDiff(String message, String expected, String actual, int flags) {
		String diffs = Diff.diff(expected, actual, flags);
		if (!diffs.equals(Diff.NO_DIFFS_MESSAGE)) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_DIFF,
					"diff",
					message,
					expected,
					actual
			);
			failure.setDetails(diffs);
			throw failure;
		}
	}
	
	public static void assertEquals(String message, String expected, String actual) {
		if (((expected == null) != (actual == null)) || 
				(expected != null && actual != null && !expected.equals(actual))) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_EQUALS,
					"string",
					message,
					expected,
					actual
			);
			throw failure;
		}
	}
	
	public static void assertEquals(String message, Object expected, Object actual) {
		if (((expected == null) != (actual == null)) || 
				(expected != null && actual != null && !expected.equals(actual))) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_EQUALS,
					"object",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertEquals(String message, int expected, int actual) {
		if (expected != actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_EQUALS,
					"int",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertEquals(String message, boolean expected, boolean actual) {
		if (expected != actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_EQUALS,
					"boolean",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertEquals(String message, char expected, char actual) {
		if (expected != actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_EQUALS,
					"char",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertEquals(String message, double expected, double actual, double delta) {
		if (Math.abs(expected - actual) > delta) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_EQUALS,
					"double",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	// TODO: double with slop range, char, etc., other types
	
	public static void assertTrue(String message, boolean b) {
		if (!b) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_TRUE,
					"boolean",
					message,
					"true",
					"false"
			);
			throw failure;
		}
	}
	
	public static void assertFalse(String message, boolean b) {
		if (b) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_FALSE,
					"boolean",
					message,
					"false",
					"true"
			);
			throw failure;
		}
	}
	
	public static void assertNull(String message, Object o) {
		if (o != null) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NULL,
					"Object",
					message,
					"null",
					o.toString()
			);
			throw failure;
		}
	}
	
	public static void assertNotNull(String message, Object o) {
		if (o == null) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_NULL,
					"Object",
					message,
					"non-null",
					"null"
			);
			throw failure;
		}
	}
	
	public static void assertNotSame(String message, int expected, int actual) {
		if (expected == actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"int",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertNotSame(String message, double expected, double actual) {
		if (expected == actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"double",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertNotSame(String message, double expected, double actual, double delta) {
		if (Math.abs(expected - actual) <= delta) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"double",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertNotSame(String message, boolean expected, boolean actual) {
		if (expected == actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"boolean",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertNotSame(String message, char expected, char actual) {
		if (expected == actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"char",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertSame(String message, Object expected, Object actual) {
		if (expected != actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"object",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void assertSame(String message, String expected, String actual) {
		if (expected != actual) {
			ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
					UnitTestType.ASSERT_NOT_EQUALS,
					"string",
					message,
					String.valueOf(expected),
					String.valueOf(actual)
			);
			throw failure;
		}
	}
	
	public static void fail(String message) {
		// TODO: this may not be the best way to represent a test failure
		ComparisonFailureEnhanced failure = new ComparisonFailureEnhanced(
				UnitTestType.FAIL,
				"string",
				message,
				"pass",
				"fail"
		);
		throw failure;
	}
	
	public static void pass(String message) {
		// TODO: this may not be the best way to represent a test passing
	}
}
