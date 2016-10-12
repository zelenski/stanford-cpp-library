package stanford.spl;

import acm.util.TokenScanner;

public class StanfordCppLib_setCppVersion extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String version = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		paramJavaBackEnd.setCppVersion(version);
	}
}
