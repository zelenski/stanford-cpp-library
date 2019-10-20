package acm.util;

public class ErrorException extends RuntimeException {
	public ErrorException(String message) {
		super(message);
	}

	public ErrorException(String message, Throwable cause) {
		super(message, cause);
	}

	public ErrorException(Exception cause) {
		super(cause);
	}

	public ErrorException(Throwable cause) {
		super(cause);
	}
}
