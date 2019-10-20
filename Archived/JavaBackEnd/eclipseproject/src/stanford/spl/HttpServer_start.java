package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.net.BackEndServer;

public class HttpServer_start extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		int port = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		BackEndServer server = BackEndServer.getInstance(port);
		server.setJavaBackEnd(paramJavaBackEnd);
		server.start();
	}
}
