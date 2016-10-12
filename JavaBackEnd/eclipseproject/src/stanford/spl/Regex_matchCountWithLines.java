package stanford.spl;

import java.util.*;
import java.util.regex.*;

import acm.util.TokenScanner;

public class Regex_matchCountWithLines extends JBECommand {
	private static final boolean DEBUG = false;
	
	private Map<Integer, Integer> getLineNumberMap(String s) {
		Map<Integer, Integer> map = new HashMap<Integer, Integer>();
		int linenum = 1;
		for (int i = 0, len = s.length(); i < len; i++) {
			char ch = s.charAt(i);
			map.put(i, linenum);
			if (ch == '\n') {
				linenum++;
			}
		}
		return map;
	}
	
	// "Regex.matchCountWithLines(\"hello abcd hi abcccd how abccd are abd you? abccccccd ^_^\", \"ab[c]+d\")")"
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		// String s = nextString(paramTokenScanner);
		String s = SplPipeDecoder.readAndDecode(paramTokenScanner);
		if (DEBUG) System.err.println("s after pipeDecode: \"" + s + "\"");
		paramTokenScanner.verifyToken(",");
		String regexp = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		Pattern pattern = Pattern.compile(regexp);
		Matcher matcher = pattern.matcher(s);

		Map<Integer, Integer> lineNumberMap = getLineNumberMap(s);
		if (DEBUG) System.err.println("line number map: " + lineNumberMap);
		StringBuilder linesStr = new StringBuilder();
		int count = 0;
		while (matcher.find()) {
			count++;
			int startIndex = matcher.start();
			if (DEBUG) System.err.println("matcher start: " + startIndex);
			if (lineNumberMap.containsKey(startIndex)) {
				int lineNum = lineNumberMap.get(startIndex);
				if (linesStr.length() > 0) {
					linesStr.append(",");
				}
				linesStr.append(lineNum);
			}
		}

		SplPipeDecoder.writeResult(count + ":" + linesStr);
	}
}
