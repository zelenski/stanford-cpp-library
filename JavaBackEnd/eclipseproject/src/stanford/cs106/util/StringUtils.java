/*
 * @author Marty Stepp
 * @version 2016/11/29
 * - added formatDoubleLikeCpp
 * @version 2016/10/21
 * - added stringIsInteger/Double/Real/Boolean
 * @version 2015/05/28
 * - added truncate() with suffix string parameter
 */

package stanford.cs106.util;

import java.awt.Point;
import java.io.*;
import java.net.*;	// for URLEn/Decoder
import java.util.*;

/**
 * This class contains utility code related to Strings and text processing.
 * @author Marty Stepp
 */
public class StringUtils {
	public static final int DEFAULT_TAB_WIDTH = 4;
	public static final String TAB_STRING = "	";
	
	/** So that an object of this class cannot be constructed. */
	private StringUtils() {}
	
	public static String escape(String s) {
		s = String.valueOf(s);
		s = s.replace("\\", "\\\\");
		s = s.replace("\"", "\\\"");
		s = s.replace("\n", "\\n");
		s = s.replace("\t", "\\t");
		return s;
	}

	public static String fitToWidth(String value, int width) {
		return fitToBox(value, width, (value == null ? 0 : value.length()));
	}

	public static String fitToHeight(String value, int height) {
		return fitToBox(value, (value == null ? 0 : value.length()), height);
	}

	public static String fitToBox(String value, int width, int height) {
		return fitToBox(value, width, height, false);
	}
	
	public static String fitToBox(String value, int width, int height, boolean html) {
		if (value == null) {
			value = "null";
		}
		String[] lines = value.split("\n");
		for (int i = 0; i < lines.length; i++) {
			if (lines[i].length() > width) {
				lines[i] = lines[i].substring(0, width) + "...";
			}
		}

		if (lines.length > height) {
			lines[height++] = "...";
		}

		return join(lines, html ? "<br/>\n" : "\n", height);
	}
	
	/**
	 * Tries to format double the C++ iostream way, with up to 6 digits shown after the decimal point.
	 */
	public static String formatDoubleLikeCpp(double value) {
		final int CPP_ROUND_DIGITS = 6;
		
		long mult = 1;
		if (Math.abs(value) >= 1.0) {
			String integerComponent = String.valueOf((long) Math.abs(value));
			int integerDigits = integerComponent.length();
			for (int i = 0; i < CPP_ROUND_DIGITS - integerDigits; i++) {
				mult *= 10;
			}
		} else {
			// C++ seems to grab the first six non-zero digits after the decimal point
			String unrounded = String.valueOf(value);
			unrounded = unrounded.replaceAll(".*\\.", "");   // "0.001234562" -> "001234562"
			int nonZeroDigitCount = 0;
			boolean doneWithZeroes = false;
			while (!unrounded.isEmpty()) {
				mult *= 10;
				char ch = unrounded.charAt(0);
				unrounded = unrounded.substring(1);
				if (ch != '0') {
					doneWithZeroes = true;
				}
				if (doneWithZeroes) {
					nonZeroDigitCount++;
					if (nonZeroDigitCount == CPP_ROUND_DIGITS) {
						break;
					}
				}
			}
		}
		
		// do the rounding
		value = value * mult;
		long rounded = Math.round(value);
		value = rounded / (double) mult;
		
		String result = String.valueOf(value);
		if (result.endsWith(".0")) {
			result = result.substring(0, result.length() - 2);
		}
		return result;
	}

	public static int getWidth(String value) {
		if (value == null) {
			return 0;
		}
		String[] lines = value.split("\n");
		int width = 0;
		for (int i = 0; i < lines.length; i++) {
			width = Math.max(width, untabify(lines[i]).length());
		}
		return width;
	}

	public static int getHeight(String value) {
		if (value == null || value.length() == 0) {
			return 0;
		}
		String[] lines = (value + " ").split("\n");
		return lines.length;  // - (value.endsWith("\n") ? 1 : 0);
	}

	public static String htmlDecode(String value) {
		return htmlDecode(value, false);
	}

	public static String htmlDecode(String value, boolean replaceSpecialChars) {
		if (value == null) {
			return null;
		}

		value = value.replace("&lt;", "<");
		value = value.replace("&gt;", ">");
		value = value.replace("&nbsp;", " ");
		value = value.replace("\u00A0", " ");   // nbsp (fucking IE)
		value = value.replace("&amp;", "&");
		
		if (replaceSpecialChars) {
			value = value.replace("\\n", "\n");
			value = value.replace("\\t", "\t");
			value = value.replace("\\\"", "\"");
			value = value.replace("\\\\", "\\");
		}
		return value;
	}

	public static String htmlEncode(String value) {
		return htmlEncode(value, false);
	}

	public static String htmlEncode(String value, boolean replaceSpecialChars) {
		return htmlEncode(value, replaceSpecialChars, false);
	}
	
	public static String htmlEncode(String value, boolean replaceSpecialChars, boolean nbsp) {
		if (value == null) {
			return "null";
		}
		value = value.replace("&", "&amp;");
		value = value.replace("<", "&lt;");
		value = value.replace(">", "&gt;");
		if (nbsp) {
			value = value.replace(" ", "&nbsp;");
		}
		value = value.replace("\"", "&quot;");
		// value = value.replace("\\", "\\\\");
		if (replaceSpecialChars) {
			value = value.replace("\n", "<br />\n");
			value = value.replace("\t", "		");  // *** TODO: probably remove this
		}
		return value;
	}

	public static String htmlEncode(int value) {
		return htmlEncode(String.valueOf(value));
	}

	public static String htmlEncode(double value) {
		return htmlEncode(String.valueOf(value));
	}

	public static String htmlEncode(char value) {
		return htmlEncode(String.valueOf(value));
	}

	public static String htmlEncode(boolean value) {
		return htmlEncode(String.valueOf(value));
	}

	public static String htmlEncode(long value) {
		return htmlEncode(String.valueOf(value));
	}

	public static String htmlEncode(Object value) {
		return htmlEncode(String.valueOf(value));
	}

	public static boolean isFalsey(String value) {
		if (value == null) {
			return false;
		}
		value = value.trim().toLowerCase().intern();
		return value == "false" || value == "0" || value == "no" || value == "n" || value == "off" || value == "disabled";
	}

	public static boolean isTruthy(String value) {
		if (value == null) {
			return false;
		}
		value = value.trim().toLowerCase().intern();
		return value == "true" || value == "1" || value == "yes" || value == "y" || value == "on" || value == "enabled";
	}

	public static String join(String[] tokens, String delimiter) {
		return join(tokens, delimiter, (tokens == null ? 0 : tokens.length));
	}

	public static String join(String[] tokens, String delimiter, int limit) {
		return join(tokens, delimiter, /* startIndex */ 0, limit);
	}
	
	public static String join(String[] tokens, String delimiter, int startIndex, int limit) {
		if (tokens == null || tokens.length == 0
				|| startIndex < 0 || startIndex >= tokens.length) {
			return "";
		}
		StringBuffer buffer = new StringBuffer();
		buffer.append(tokens[startIndex]);
		if (limit == 0) {
			limit = tokens.length;
		} else {
			limit = Math.min(limit, tokens.length);
		}
		for (int i = startIndex + 1; i < limit; i++) {
			buffer.append(delimiter);
			buffer.append(tokens[i]);
		}
		return buffer.toString();
	}

	public static String join(Iterable<String> tokens, String delimiter) {
		if (tokens == null) {
			return "";
		}
		StringBuffer buffer = new StringBuffer();
		Iterator<String> itr = tokens.iterator();
		if (!itr.hasNext()) {
			return "";
		}
		
		buffer.append(itr.next());
		while (itr.hasNext()) {
			buffer.append(delimiter);
			buffer.append(itr.next());
		}
		return buffer.toString();
	}
	
	/**
	 * Returns the Levenshtein edit distance between the two given strings;
	 * the number of characters that must be added, removed, or modified to
	 * turn the one string into the other.
	 * Case-insensitive.
	 * @see http://en.wikipedia.org/wiki/Levenshtein_distance
	 */
	public static int levenshtein(String s1, String s2) {
		s1 = s1.toUpperCase();
		s2 = s2.toUpperCase();
		
		int[][] matrix = new int[s1.length() + 1][s2.length() + 1];
		
		for (int i = 0; i <= s1.length(); i++) {
			matrix[i][0] = i;
		}
		for (int j = 0; j <= s2.length(); j++) {
			matrix[0][j] = j;
		}
		
		for (int j = 1; j <= s2.length(); j++) {
			for (int i = 1; i <= s1.length(); i++) {
				if (s1.charAt(i - 1) == s2.charAt(j - 1)) {
					matrix[i][j] = matrix[i - 1][j - 1]; 
				} else {
					matrix[i][j] = 1 + Math.min(matrix[i - 1][j],
							Math.min(matrix[i][j - 1], matrix[i - 1][j - 1]));
				}
			}
		}

		return matrix[matrix.length - 1][matrix[0].length - 1];
	}

	/**
	 * Returns a string that contains s repeated n times.
	 * @param s string to repeat.  if s is null, returns null.
	 * @param n number of repetitions.  if <= 0, returns "".
	 * @return the repeated string
	 */
	public static String nCopies(String s, int n) {
		if (s == null) {
			return null;
		}
		StringBuilder sb = new StringBuilder(s.length() * n + 8);
		for (int i = 1; i <= n; i++) {
			sb.append(s);
		}
		return sb.toString();
	}
	
	public static String numberLines(String s) {
		s = String.valueOf(s);
		String[] lines = s.split("\r?\n");
		int digits = String.valueOf(lines.length).length();
		if (digits == 0) {
			return "";
		}
		for (int i = 0; i < lines.length; i++) {
			lines[i] = String.format("%0" + digits + "d| %s", i+1, lines[i]);
		}
		return join(lines, "\n");
	}
	
	public static String padL(int s, int width) {
		return padL(String.valueOf(s), width);
	}

	public static String padL(String s, int width) {
		s = String.valueOf(s);
		while (s.length() < width) {
			s = " " + s;
		}
		return s;
	}

	public static String padR(int s, int width) {
		return padR(String.valueOf(s), width);
	}

	public static String padR(String s, int width) {
		s = String.valueOf(s);
		while (s.length() < width) {
			s += " ";
		}
		return s;
	}
	
	public static String padNumber(int n, int length) {
		return padNumber(n, length, false);
	}

	public static String padNumber(int n, int length, boolean html) {
		String s = "" + n;
		int len = s.length();
		while (len < length) {
			if (html) {
				s = "&nbsp;" + s;
			} else {
				s = " " + s;
			}
			len++;
		}
		return s;
	}
	
	// turns "{true, false, true}" into new boolean[] {true, false, true}
	public static boolean[] parseBooleanArray(String s) {
		s = String.valueOf(s);
		String[] tokens = parseStringArray(s);
		boolean[] result = new boolean[tokens.length];
		for (int i = 0; i < tokens.length; i++) {
			result[i] = Boolean.parseBoolean(tokens[i]);
		}
		return result;
	}
	
	// turns "{'a', 'b', 'c'}" into new char[] {'a', 'b', 'c'}
	public static char[] parseCharArray(String s) {
		s = String.valueOf(s);
		s = s.replace("'", "");
		String[] tokens = parseStringArray(s);
		char[] result = new char[tokens.length];
		for (int i = 0; i < tokens.length; i++) {
			result[i] = tokens[i].length() > 0 ? tokens[i].charAt(0) : '\0';
		}
		return result;
	}
	
	// turns "{1.1, 2.2, 3.3}" into new double[] {1.1, 2.2, 3.3}
	public static double[] parseDoubleArray(String s) {
		s = String.valueOf(s);
		String[] tokens = parseStringArray(s);
		double[] result = new double[tokens.length];
		for (int i = 0; i < tokens.length; i++) {
			result[i] = Double.parseDouble(tokens[i]);
		}
		return result;
	}
	
	// turns "{1, 2, 3}" into new int[] {1, 2, 3}
	public static int[] parseIntArray(String s) {
		s = String.valueOf(s);
		String[] tokens = parseStringArray(s);
		int[] result = new int[tokens.length];
		for (int i = 0; i < tokens.length; i++) {
			result[i] = Integer.parseInt(tokens[i]);
		}
		return result;
	}
	
	// turns "{{1, 2, 3}, {4, 4, 4}}" into new int[][] {{1, 2, 3}, {4, 4, 4}}
	public static int[][] parseIntArray2D(String s) {
		s = String.valueOf(s);
		s = s.trim();
		s = s.replaceAll("^\\{+", "").replaceAll("\\}+$", "");
		if (s.length() == 0) {
			return new int[0][0];
		}
		
		String[] rows = s.split("\\}, \\{");
		int[][] result = new int[rows.length][];
		for (int i = 0; i < rows.length; i++) {
			result[i] = parseIntArray(rows[i]);
		}
		return result;
	}
	
	// turns "{1, &quot;hello!&quot;, 3}" into new String[] {"1", "hello!", "3"}
	public static String[] parseStringArray(String s) {
		s = String.valueOf(s);
		s = htmlDecode(s).trim().replace("[", "").replace("]", "").replace("{", "").replace("}", "").replace("&quot;", "");
		if (s.startsWith("\"")) {
			s = s.substring(1);
		}
		if (s.endsWith("\"")) {
			s = s.substring(0, s.length() - 1);
		}
		if (s.length() == 0) {
			return new String[0];
		}
		s = s.replace("\\\"", "\"");
		return s.split("[\"]?,[ ]*[\"]?");
	}
	
	// value = "new Point(5, -2)";   // for example
	public static Point parsePoint(String value) {
		// String value2 = value.replaceAll("new\\s+[a-zA-Z0-9_.]+", "");
		// strip all but the x/y coords, then split and grab them
		value = String.valueOf(value);
		String value2 = value.replaceAll("[^0-9\\-+.,]", "");
		String[] nums = value2.split("\\s*,\\s*");
		if (nums.length < 2) {
			throw new IllegalArgumentException("value=" + value + ", value2=" + value2 + ", nums=" + Arrays.toString(nums));
		}
		int x = Integer.parseInt(nums[0]);
		int y = Integer.parseInt(nums[1]);
		return new Point(x, y);
	}
	
	public static String plural(int n) {
		return (n == 1) ? "" : "s";
	}
	
	public static boolean equalsIgnoreWhitespace(String s1, String s2) {
		return equalsIgnoreWhitespace(s1, s2, false);
	}

	public static boolean equalsIgnoreWhitespace(String s1, String s2, boolean ignoreCase) {
		if (s1 == null && s2 == null) {
			return true;
		} else if (s1 == null || s2 == null) {
			return false;
		} else {
			if (ignoreCase) {
				return removeWhitespace(s1).equalsIgnoreCase(removeWhitespace(s2));
			} else {
				return removeWhitespace(s1).equals(removeWhitespace(s2));
			}
		}
	}
	
	public static String removeWhitespace(String s) {
		if (s == null) {
			return s;
		} else {
			s = s.replaceAll("[ \r\n\t\f]+", "");
			return s;
		}
	}
	
	public static String sortChars(String s) {
		s = String.valueOf(s);
		char[] a = s.toCharArray();
		Arrays.sort(a);
		return new String(a).intern();
	}
	
	public static boolean stringIsBoolean(String text) {
		return text != null && (text.equals("true") || text.equals("false"));
	}
	
	public static boolean stringIsDouble(String text) {
		try {
			Double.parseDouble(text);
			return true;
		} catch (NumberFormatException nfe) {
			return false;
		}
	}
	
	public static boolean stringIsInteger(String text) {
		return stringIsInteger(text, /* radix */ 10);
	}
	
	public static boolean stringIsInteger(String text, int radix) {
		try {
			Integer.parseInt(text, radix);
			return true;
		} catch (NumberFormatException nfe) {
			return false;
		}
	}
	
	public static boolean stringIsReal(String text) {
		return stringIsDouble(text);
	}
	
	public static String toAsciiDump(String s) {
		s = String.valueOf(s);
		StringBuilder sb = new StringBuilder(s.length() * 30);
		for (char c : s.toCharArray()) {
			sb.append(String.format("%3s : %3d\n", toPrintableChar(c), (int) c));
		}
		return sb.toString();
	}
	
	public static String toPrintableChar(char c) {
		switch (c) {
		case '\n': return "\\n";
		case '\r': return "\\r";
		case '\f': return "\\f";
		case '\t': return "\\t";
		case '\\': return "\\\\";
		case '\0': return "\\0";
		case ' ': return "' '";
		default: return String.valueOf(c);
		}
	}
	
	public static String toString(Point p) {
	    return "(" + p.x + ", " + p.y + ")";
	}

	/**
	 * Removes any blank lines (just \n or spaces/tabs followed by \n) from the
	 * start/end of s.
	 */
	public static String trimBlankLines(String s) {
		if (s == null) {
			return null;
		}
		
		// break apart into list of lines
		List<String> lines = new ArrayList<String>(Arrays.asList(s.split("[\r]?\n")));   // REGEX OK
		
		// remove blank lines from front/end
		while (!lines.isEmpty()) {
			if (lines.get(0).trim().isEmpty()) {
				lines.remove(0);
			} else if (lines.get(lines.size() - 1).trim().isEmpty()) {
				lines.remove(lines.size() - 1);
			} else {
				break;
			}
		}
		
		return CollectionUtils.join(lines, "\n");
	}

	public static String trimEnd(String s) {
		return trimR(s);
	}
	
	public static String trimR(String s) {
		s = String.valueOf(s);
		int end = s.length() - 1;
		while (end >= 0 && Character.isWhitespace(s.charAt(end))) {
			end--;
		}
		return s.substring(0, end + 1);
	}
	
	/**
	 * Trims out any leading/trailing whitespace from all lines of string s.
	 * @return null if s == null
	 */
	public static String trimLines(String s) {
		s = String.valueOf(s);
		if (s == null) {
			return null;
		} else {
			return s.replaceAll("[ \t]{0,999}\r?\n[ \t]{0,999}", "\n").trim();   // REGEX OK
		}
	}
	
	public static String trimEndsOfLines(String s) {
		s = String.valueOf(s);
		return s.replaceAll("\\s+\r?\n", "\n");
	}
	
	/**
	 * Trims the given string to be at most the given number of characters in length.
	 * Similar to fitToWidth, but doesn't put ... at end.
	 */
	public static String truncate(String s, int length) {
		return truncate(s, length, /* suffix */ "");
	}
	
	/**
	 * Trims the given string to be at most the given number of characters in length.
	 * Similar to fitToWidth, but doesn't put ... at end.
	 */
	public static String truncate(String s, int length, String suffix) {
		if (s == null || s.length() <= length) {
			return s;
		} else {
			s = s.substring(0, length);
			if (suffix != null && !suffix.isEmpty()) {
				s += suffix;
			}
			return s;
		}
	}
	
	/**
	 * Looks at all lines of the given string, figuring out how 'indented' each
	 * line is; then removes the longest common indentation prefix string that
	 * occurs in all lines.
	 * For example, if some lines are indented 8 spaces, some 16, and some 12,
	 * then each line starts with at least 8 spaces; so those 8 are stripped from
	 * each line to yield a string where some lines are indented 0 spaces,
	 * some 8, and some 4.
	 * @return null if s == null
	 */
	public static String unindent(String s) {
		s = String.valueOf(s);
		String[] lines = s.split("[\r]?\n");
		int minIndent = Integer.MAX_VALUE;
		for (String line : lines) {
			if (line.trim().length() == 0) {   // ignore blank lines
				continue;
			}
			int indent = 0;
			while (indent < line.length() && 
					(line.charAt(indent) == ' ' || line.charAt(indent) == '\t')) {
				indent++;
			}
			minIndent = Math.min(minIndent, indent);
		}
		minIndent = (minIndent == Integer.MAX_VALUE) ? 0 : minIndent;
		for (int i = 0; i < lines.length; i++) {
			lines[i] = lines[i].substring(Math.min(minIndent, lines[i].length()));
		}
		return join(lines, "\n");
	}

	/** Replaces tabs with a string of spaces of a default width. */
	public static String untabify(String s) {
		return untabify(s, DEFAULT_TAB_WIDTH);
	}

	/** Replaces tabs with a string of spaces of the given width. */
	public static String untabify(String s, int tabWidth) {
		s = String.valueOf(s);
		String tab = "";
		for (int i = 0; i < tabWidth; i++) {
			tab += " ";
		}
		return s.replaceAll("\t", tab);
	}
	
	public static String urlEncode(String s) {
		s = String.valueOf(s);
		try {
			return URLEncoder.encode(s, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return s;
		}
	}
	
	public static String urlDecode(String s) {
		s = String.valueOf(s);
		try {
			return URLDecoder.decode(s, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return s;
		}
	}
	
	public static String wrapLines(String s, int length) {
		StringBuilder sb = new StringBuilder(s.length() + 256);
		String[] lines = s.split("\r?\n");
		for (String line : lines) {
			while (line.length() > length) {
				String sub = line.substring(0, length);
				sb.append(sub);
				sb.append('\n');
				line = line.substring(length);
			}
			sb.append(line);
			sb.append('\n');
		}
		return sb.toString();
	}
}
