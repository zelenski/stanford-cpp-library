package stanford.spl;

import acm.util.TokenScanner;

public class StanfordCppLib_getJbeVersion extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		SplPipeDecoder.writeResult(paramJavaBackEnd.getJbeVersion());
	}
}
