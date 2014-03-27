package stanford.spl;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;

import acm.util.ErrorException;
import acm.util.TokenScanner;

class Regex_match extends JBECommand {
	public static String pipeDecode(String s) {
//		s = s.replace("\\n", "\n");
//		s = s.replace("\\t", "\t");
//		s = s.replace("&quot;", "\"");
		try {
			s = URLDecoder.decode(s, "UTF-8");
		} catch (UnsupportedEncodingException uee) {
			System.err.println(uee);
		}
		return s;
	}

	public static String pipeEncode(String s) {
//		s = s.replace("\n", "\\n");
//		s = s.replace("\t", "\\t");
//		s = s.replace("\"", "\\\"");
		try {
			s = URLEncoder.encode(s, "UTF-8");
		} catch (UnsupportedEncodingException uee) {
			System.err.println(uee);
		}
		return s;
	}

	public static String readEncodedString(TokenScanner paramTokenScanner) {
		String s = "";
		if ((char) paramTokenScanner.getChar() != '"') {
			throw new ErrorException("quoted string must start with \" character");
		}
		while (true) {
			char ch = (char) paramTokenScanner.getChar();
			if (ch == '"' || ch < 0) {
				break;
			} else {
				s += ch;
			}
		}
		return s;
	}


	// "Regex.match(\"hello abcd hi abcccd how abccd are abd you? abccccccd ^_^\", \"ab[c]+d\")")"
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String s = Regex_match.readEncodedString(paramTokenScanner);
		s = pipeDecode(s);
		paramTokenScanner.verifyToken(",");
		String regexp = Regex_match.readEncodedString(paramTokenScanner);
		regexp = pipeDecode(regexp);
		paramTokenScanner.verifyToken(")");
		boolean result = s.matches(regexp);
		System.out.println("result:" + result);
		System.out.flush();
	}
}
