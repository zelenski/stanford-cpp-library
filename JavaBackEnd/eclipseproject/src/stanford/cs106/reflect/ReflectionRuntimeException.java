package stanford.cs106.reflect;

public class ReflectionRuntimeException extends RuntimeException {
	private static final long serialVersionUID = 1L;
	public ReflectionRuntimeException(String s) {
		super(s);
	}
	public ReflectionRuntimeException(String s, Throwable t) {
		super(s, t);
	}
	public ReflectionRuntimeException(Throwable t) {
		super(t);
	}
}
