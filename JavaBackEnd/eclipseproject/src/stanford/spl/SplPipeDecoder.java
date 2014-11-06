package stanford.spl;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;

import acm.util.ErrorException;
import acm.util.TokenScanner;

public class SplPipeDecoder {
	// related: similar constant in C++ lib platform.cpp
	public static final int PIPE_MAX_COMMAND_LENGTH = 4096;
	
	public static String decode(String s) {
		try {
			s = URLDecoder.decode(s, "UTF-8");
		} catch (UnsupportedEncodingException uee) {
			System.err.println(uee);
		}
		return s;
	}

	public static String encode(String s) {
		try {
			s = URLEncoder.encode(s, "UTF-8");
		} catch (UnsupportedEncodingException uee) {
			System.err.println(uee);
		}
		return s;
	}
	
	public static void encodeAndWrite(String s) {
		writeResult(encode(s));
	}

	public static String readAndDecode(TokenScanner scanner) {
		return decode(readEncodedString(scanner));
	}
	
	private static void eatSpaces(TokenScanner scanner) {
		int ch = scanner.getChar();
		while (ch >= 0 && Character.isWhitespace(ch)) {
			ch = scanner.getChar();
		}
		if (ch >= 0) {
			scanner.ungetChar(ch);
		}
	}
	
	public static String readEncodedString(TokenScanner scanner) {
		String s = "";
		eatSpaces(scanner);
		int ch = scanner.getChar();
		if (ch != '"') {
			throw new ErrorException("quoted string must start with \" character, but saw '" + (char)ch + "' (" + (int)ch + ")");
		}
		while (true) {
			ch = scanner.getChar();
			if (ch == '"' || ch < 0) {
				break;
			} else {
				s += (char) ch;
			}
		}
		if (ch != '"') {
			throw new ErrorException("quoted string must end with \" character, but saw '" + (char)ch + "' (" + (int)ch + ")");
		}
		return s;
	}
	
	public static void writeResult(Object o) {
		writeResult(o == null ? "null" : o.toString());
	}
	
	public static void writeResult(String s) {
		if (s.length() > PIPE_MAX_COMMAND_LENGTH) {
			writeLongResult(s);
		} else {
			System.out.println("result:" + s);
			System.out.flush();
		}
	}
	
	public static void writeLongResult(String s) {
		System.out.println("result_long:begin");
		System.out.flush();
		for (int i = 0, len = s.length();
				i < len;
				i += PIPE_MAX_COMMAND_LENGTH) {
			String chunk = s.substring(i, Math.min(i + PIPE_MAX_COMMAND_LENGTH, len));
			System.out.println(chunk);
			System.out.flush();
		}
		System.out.println("result_long:end");
		System.out.flush();
	}
}
