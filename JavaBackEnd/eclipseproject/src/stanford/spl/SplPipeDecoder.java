/*
 * @version 2015/10/08
 */

package stanford.spl;

import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.net.URLEncoder;

import acm.util.ErrorException;
import acm.util.TokenScanner;

public class SplPipeDecoder {
	// related: similar constant in C++ lib platform.cpp
	public static final int PIPE_MAX_COMMAND_LENGTH = 2048;
	
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
	
	public static synchronized void print(Object o) {
		print(String.valueOf(o));
	}
	
	public static synchronized void print(String s) {
		System.out.print(s);
		System.out.flush();
	}
	
	public static synchronized void println(Object o) {
		println(String.valueOf(o));
	}
	
	public static synchronized void println(String s) {
		System.out.println(s);
		System.out.flush();
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
	
	public static synchronized void writeAck() {
		writeAck("");
	}
	
	public static synchronized void writeAck(String s) {
		writeResult("___jbe___ack___ " + s);
	}
	
	public static synchronized void writeError(String msg) {
		writeResult("error:" + msg);
	}
	
	public static synchronized void writeOK() {
		writeResult("ok");
	}
	
	public static synchronized void writeResult(Object o) {
		writeResult(String.valueOf(o));
	}
	
	public static synchronized void writeResult(String s) {
		if (s.length() > PIPE_MAX_COMMAND_LENGTH - 7) {
			writeLongResult(s);
		} else {
			println("result:" + s);
		}
	}
	
	public static synchronized void writeLongResult(String s) {
		println("result_long:begin");
		for (int i = 0, len = s.length();
				i < len;
				i += PIPE_MAX_COMMAND_LENGTH) {
			String chunk = s.substring(i, Math.min(i + PIPE_MAX_COMMAND_LENGTH, len));
			println(chunk);
		}
		println("result_long:end");
	}
}
