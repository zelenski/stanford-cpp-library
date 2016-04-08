package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.server.SimpleServer;

public class HttpServer_sendResponse extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		int requestID = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int httpErrorCode = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String contentType = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String responseText = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		SimpleServer server = SimpleServer.getInstance();
		server.sendResponse(requestID, httpErrorCode, contentType, responseText);
	}
}
