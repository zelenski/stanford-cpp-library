package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.server.SimpleServer;

public class HttpServer_start extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		int port = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		SimpleServer server = SimpleServer.getInstance(port);
		server.setJavaBackEnd(paramJavaBackEnd);
		server.start();
	}
}
