package stanford.spl;

import acm.util.TokenScanner;

public class Regex_match extends JBECommand {
	// "Regex.match(\"hello abcd hi abcccd how abccd are abd you? abccccccd ^_^\", \"ab[c]+d\")")"
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String s = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String regexp = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		boolean result = s.matches(regexp);
		SplPipeDecoder.writeResult(result);
	}
}
