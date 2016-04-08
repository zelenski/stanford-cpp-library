package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.server.SimpleServer;

public class HttpServer_stop extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		
		SimpleServer server = SimpleServer.getInstance();
		server.stop();
	}
}
