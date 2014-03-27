package stanford.spl;

import acm.util.TokenScanner;

class Regex_replace extends JBECommand {
	private String pipeDecode(String s) {
		s = s.replace("\\n", "\n");
		s = s.replace("\\t", "\t");
		s = s.replace("\\\"", "\"");
		return s;
	}

	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String s = Regex_match.readEncodedString(paramTokenScanner);
		s = pipeDecode(s);
		paramTokenScanner.verifyToken(",");
		String regexp = Regex_match.readEncodedString(paramTokenScanner);
		regexp = pipeDecode(regexp);
		paramTokenScanner.verifyToken(",");
		String replacement = Regex_match.readEncodedString(paramTokenScanner);
		replacement = pipeDecode(replacement);
		paramTokenScanner.verifyToken(")");
		String result = s.replaceAll(regexp, replacement);
		result = Regex_match.pipeEncode(result);
		System.out.println("result:" + result);
		// System.out.println("result:\"" + result.replace("\"", "\\\"") + "\"");
		// System.out.println("result:\"hello\"");
		System.out.flush();
	}
}
