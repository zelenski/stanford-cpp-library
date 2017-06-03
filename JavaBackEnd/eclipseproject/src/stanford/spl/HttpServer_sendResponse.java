package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.net.BackEndServer;

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
		
		BackEndServer server = BackEndServer.getInstance();
		server.sendResponse(requestID, httpErrorCode, contentType, responseText);
	}
}
