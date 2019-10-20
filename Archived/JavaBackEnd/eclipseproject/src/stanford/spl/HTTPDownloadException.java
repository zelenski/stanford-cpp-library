package stanford.spl;

/* Exception type thrown when something goes wrong with an HTTP download. */
public class HTTPDownloadException extends Exception {
	final int statusCode;
	
	public HTTPDownloadException(int statusCode) {
		this.statusCode = statusCode;
	}
	
	public int getStatusCode() {
		return statusCode;
	}

	@Override public String getMessage() {
		return "HTTP Error: " + statusCode;
	}
}
