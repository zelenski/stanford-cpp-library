package stanford.spl;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import acm.util.TokenScanner;

public class Regex_matchCount extends JBECommand {
	// "Regex.matchCount(\"hello abcd hi abcccd how abccd are abd you? abccccccd ^_^\", \"ab[c]+d\")")"
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String s = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String regexp = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		Pattern pattern = Pattern.compile(regexp);
		Matcher matcher = pattern.matcher(s);
		int count = 0;
		while (matcher.find()) {
			count++;
		}

		SplPipeDecoder.writeResult(count);
	}
}
