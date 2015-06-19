package stanford.spl.pipecommand;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import acm.util.TokenScanner;

public class StanfordCppLib_setCppVersion extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String version = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		paramJavaBackEnd.setCppVersion(version);
	}
}
