package stanford.spl;

import acm.util.TokenScanner;

class StanfordCppLib_getJbeVersion extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		System.out.println("result:\"" + paramJavaBackEnd.getJbeVersion() + "\"");
		System.out.flush();
	}
}
