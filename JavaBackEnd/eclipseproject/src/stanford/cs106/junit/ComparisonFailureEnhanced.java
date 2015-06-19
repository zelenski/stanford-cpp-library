package stanford.cs106.junit;

import org.junit.*;

public class ComparisonFailureEnhanced extends ComparisonFailure {
	private UnitTestType type;
	private String details = "";
	private String valueType = "";
	private String expected;
	private String actual;
	private String message;
	
	public ComparisonFailureEnhanced(UnitTestType type,
			String message, String expected, String actual) {
		this(type, /* valueType */ "", message, expected, actual);
	}
	
	public ComparisonFailureEnhanced(UnitTestType type, String valueType,
			String message, String expected, String actual) {
		super(message, expected, actual);
		this.expected = expected;
		this.actual = actual;
		this.message = message;
		this.type = type;
		this.valueType = valueType;
	}
	
	public String getMessage() {
		return message;
	}
	
	public String getExpected() {
		return expected;
	}
	
	public String getActual() {
		return actual;
	}
	
	public String getStudent() {
		return actual;
	}
	
	public UnitTestType getType() {
		return type;
	}
	
	public String getDetails() {
		return details;
	}
	
	public String getValueType() {
		return valueType;
	}
	
	public void setDetails(String details) {
		this.details = details;
	}
}
