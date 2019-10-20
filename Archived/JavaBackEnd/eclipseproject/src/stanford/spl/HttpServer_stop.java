package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.net.BackEndServer;

public class HttpServer_stop extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		
		BackEndServer server = BackEndServer.getInstance();
		server.stop();
	}
}
