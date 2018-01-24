/*
 * @version 2016/10/07
 * - oops, actually IGNORE_BLANK_LINES flag was still totally broken; NOW it works!
 * @version 2016/04/28
 * - fixed IGNORE_BLANK_LINES flag to actually work
 */

package stanford.cs106.diff;

import java.util.*;
import stanford.cs106.util.*;

public class Diff {
	public static final String NO_DIFFS_MESSAGE = "No differences found";

	public static final int IGNORE_LEADING      = 0x1;
	public static final int IGNORE_TRAILING     = 0x2;
	public static final int IGNORE_WHITESPACE   = 0x4;
	public static final int IGNORE_BLANK_LINES  = 0x8;
	public static final int IGNORE_CASE         = 0x10;
	public static final int IGNORE_NUMBERS      = 0x20;
	public static final int IGNORE_NONNUMBERS   = 0x40;
	public static final int IGNORE_PUNCTUATION  = 0x80;
	public static final int IGNORE_AFTERDECIMAL = 0x100;
	public static final int IGNORE_CHARORDER    = 0x200;
	public static final int IGNORE_LINEORDER    = 0x400;
	public static final int IGNORE_EVERYTHING   = 0x100000;
	
	public static final int FLAGS_DEFAULT = 0;
	public static final int FLAGS_DEFAULT_LENIENT = IGNORE_TRAILING | IGNORE_WHITESPACE | IGNORE_BLANK_LINES | IGNORE_CASE | IGNORE_PUNCTUATION;
	public static final int FLAGS_DEFAULT_STRICT = IGNORE_TRAILING | IGNORE_BLANK_LINES;
	
	
	public static List<Difference> diffAsList(String s1, String s2) {
		return diffAsList(s1, s2, FLAGS_DEFAULT);
	}

	public static List<Difference> diffAsList(String s1, String s2, int flags) {
		s1 = String.valueOf(s1);
		s2 = String.valueOf(s2);
		String[] lines1 = s1.split("\r?\n");
		String[] lines2 = s2.split("\r?\n");
//		String[] lines1Original = Arrays.copyOf(lines1, lines1.length);
//		String[] lines2Original = Arrays.copyOf(lines2, lines2.length);

		if ((flags & IGNORE_NUMBERS) != 0) {
			s1 = s1.replaceAll("[0-9]+", "###");
			s2 = s2.replaceAll("[0-9]+", "###");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_NONNUMBERS) != 0) {
			s1 = s1.replaceAll("[^0-9\n]+", " ");
			s2 = s2.replaceAll("[^0-9\n]+", " ");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_PUNCTUATION) != 0) {
			String punct = "[.,?!'\"()\\/#$%@^&*_\\[\\]{}|<>:;-]+";
			s1 = s1.replaceAll(punct, "");
			s2 = s2.replaceAll(punct, "");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_AFTERDECIMAL) != 0) {
			s1 = s1.replaceAll("\\.[0-9]+", ".#");
			s2 = s2.replaceAll("\\.[0-9]+", ".#");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_CASE) != 0) {
			s1 = s1.toLowerCase();
			s2 = s2.toLowerCase();
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_CHARORDER) != 0) {
			ArrayList<String> lines1Sorted = new ArrayList<String>();
			for (String line : lines1) {
				line = StringUtils.sortChars(line);
				lines1Sorted.add(line);
			}
			lines1 = lines1Sorted.toArray(new String[0]);
			s1 = StringUtils.join(lines1, "\n");

			ArrayList<String> lines2Sorted = new ArrayList<String>();
			for (String line : lines2) {
				line = StringUtils.sortChars(line);
				lines2Sorted.add(line);
			}
			lines2 = lines2Sorted.toArray(new String[0]);
			s2 = StringUtils.join(lines2, "\n");
		}
		if ((flags & IGNORE_LINEORDER) != 0) {
			Arrays.sort(lines1);
			Arrays.sort(lines2);
			s1 = StringUtils.join(lines1, "\n");
			s2 = StringUtils.join(lines2, "\n");
		}
		if ((flags & IGNORE_WHITESPACE) != 0) {
			for (int i = 0; i < lines1.length; i++) {
				lines1[i] = StringUtils.removeWhitespace(lines1[i]);
			}
			for (int i = 0; i < lines2.length; i++) {
				lines2[i] = StringUtils.removeWhitespace(lines2[i]);
			}
		}

		List<Difference> out = new ArrayList<Difference>();
		if (StringUtils.trimR(s1).equals(StringUtils.trimR(s2))) {
			return out;
		}

		// build a reverse-index array using the line as key and line number as value
		// don't store blank lines, so they won't be targets of the shortest distance
		// search
		Map<String, Set<Integer>> reverse1 = new TreeMap<String, Set<Integer>>();
		Map<String, Set<Integer>> reverse2 = new TreeMap<String, Set<Integer>>();
		for (int i = 0; i < lines1.length; i++) {
			String line = lines1[i];
			if (line.length() > 0) {
				if (!reverse1.containsKey(line)) {
					reverse1.put(line, new TreeSet<Integer>());
				}
				reverse1.get(line).add(i);
			}
		}
		for (int i = 0; i < lines2.length; i++) {
			String line = lines2[i];
			if (line.length() > 0) {
				if (!reverse2.containsKey(line)) {
					reverse2.put(line, new TreeSet<Integer>());
				}
				reverse2.get(line).add(i);
			}
		}

		// indexes of current lines within each file
		// (start at beginning of each list)
		int index1 = 0;
		int index2 = 0;
		List<Integer> actions = new ArrayList<Integer>();

		// walk this loop until we reach the end of one of the lists of lines
		while (index1 < (int) lines1.length && index2 < (int) lines2.length) {
			// if we have a common line, save it and go to the next
			if (lines1[index1].equals(lines2[index2])) {
				actions.add(4);
				index1++;
				index2++;
				continue;
			}
			// otherwise, find the shortest move (Manhattan-distance) from the
			// current location
			int best1 = lines1.length;
			int best2 = lines2.length;
			int sub1 = index1;
			int sub2 = index2;
			while ((sub1 + sub2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
				int d = -1;
				if (lines2.length > sub2 && reverse1.containsKey(lines2[sub2])) {
					for (int lineNumber : reverse1.get(lines2[sub2])) {
						if (lineNumber >= sub1) {
							d = lineNumber;
							break;
						}
					}
				}
				if (d >= sub1 && (d + sub2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
					best1 = d;
					best2 = sub2;
				}

				d = -1;
				if (lines1.length > sub1 && reverse2.containsKey(lines1[sub1])) {
					for (int lineNumber : reverse2.get(lines1[sub1])) {
						if (lineNumber >= sub2) {
							d = lineNumber;
							break;
						}
					}
				}
				if (d >= sub2 && (d + sub1 - index1 - index2) < (best1 + best2 - index1 - index2)) {
					best1 = sub1;
					best2 = d;
				}

				sub1++;
				sub2++;
			}

			while (index1 < best1) {
				actions.add(1);
				index1++;
			}  // deleted elements

			while (index2 < best2) {
				actions.add(2);
				index2++;
			}  // added elements
		}

		// we've reached the end of one list, now walk to the end of the other
		while (index1 < (int) lines1.length) {
			actions.add(1);
			index1++;
		}  // deleted elements

		if ((flags & IGNORE_TRAILING) != 0) {
			while (index2 < (int) lines2.length) {
				actions.add(2);
				index2++;
			}  // added elements
		}

		// and this marks our ending point
		actions.add(8);

		int op = 0;
		int x0 = 0;
		int x1 = 0;
		int y0 = 0;
		int y1 = 0;

		for (int action : actions) {
			if (action == 1) {
				op |= action;
				x1++;
				continue;
			} else if (action == 2) {
				op |= action;
				y1++;
				continue;
			}

			if (op > 0) {
//				boolean multipleLines = (x1 != x0 + 1);
//				String xstr = "" + (multipleLines ? ((x0 + 1) + "-" + x1) : x1);
//				String ystr = "" + ((y1 != y0 + 1) ? ((y0 + 1) + "-" + y1) : y1);
//				String linesStr = "\nLine" + (multipleLines ? "s " : " ");
//				String doStr = "do" + (multipleLines ? "" : "es");
				if (op == 1) {
					out.add(new Difference(Difference.Type.DELETE, x0, x1-1, y0, y1-1));
				} else if (op == 3) {
					out.add(new Difference(Difference.Type.MODIFY, x0, x1-1, y0, y1-1));
				}

				while (x0 < x1) {
					out.add(new Difference(Difference.Type.DELETE, x0, x1-1, y0, y1-1));
					x0++;
				}   // deleted elems

				if (op == 2) {
					if ((flags & IGNORE_LEADING) == 0 || x1 > 0) {
						out.add(new Difference(Difference.Type.ADD, x0, x1-1, y0, y1-1));
					}
				} else if (op == 3) {
					// out += "---";
				}

				while (y0 < y1) {
					if ((flags & IGNORE_LEADING) == 0 || op != 2 || x1 > 0) {
						out.add(new Difference(Difference.Type.ADD, x0, x1-1, y0, y1-1));
					}
					y0++;
				}   // added elems
			}
			x1++;
			x0 = x1;
			y1++;
			y0 = y1;
			op = 0;
		}

		return out;
	}
	
	public static class Difference {
		public static enum Type { DELETE, ADD, MODIFY }
		public Type type;
		public int expectedStart;
		public int expectedEnd;
		public int actualStart;
		public int actualEnd;
		
		public Difference(Type type, int expectedStart, int expectedEnd, int actualStart, int actualEnd) {
			this.type = type;
			this.expectedStart = expectedStart;
			this.expectedEnd = expectedEnd;
			this.actualStart = actualStart;
			this.actualEnd = actualEnd;
		}
		
		private String rangeHelper(int start, int end) {
			String result = "";
			if (start >= 0) {
				result += start;
			}
			if (end > start) {
				result += (result.isEmpty() ? "" : "-") + end;
			}
			return result;
		}
		
		public String expectedToString() {
			return rangeHelper(expectedStart, expectedEnd);
		}
		
		public String actualToString() {
			return rangeHelper(actualStart, actualEnd);
		}
		
		public String toString() {
			return "type=" + type + " exp=" + expectedToString() + " act=" + actualToString();
		}
	}

	
	
	public static String diff(String s1, String s2) {
		return diff(s1, s2, FLAGS_DEFAULT);
	}

	public static String diff(String s1, String s2, int flags) {
		s1 = String.valueOf(s1);
		s2 = String.valueOf(s2);
		String[] lines1 = s1.split("\r?\n");
		String[] lines2 = s2.split("\r?\n");
		String[] lines1Original = Arrays.copyOf(lines1, lines1.length);
		String[] lines2Original = Arrays.copyOf(lines2, lines2.length);

		if ((flags & IGNORE_NUMBERS) != 0) {
			s1 = s1.replaceAll("[0-9]+", "###");
			s2 = s2.replaceAll("[0-9]+", "###");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_NONNUMBERS) != 0) {
			s1 = s1.replaceAll("[^0-9\n]+", " ");
			s2 = s2.replaceAll("[^0-9\n]+", " ");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_PUNCTUATION) != 0) {
			String punct = "[.,?!'\"()\\/#$%@^&*_\\[\\]{}|<>:;-]+";
			s1 = s1.replaceAll(punct, "");
			s2 = s2.replaceAll(punct, "");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_AFTERDECIMAL) != 0) {
			s1 = s1.replaceAll("\\.[0-9]+", ".#");
			s2 = s2.replaceAll("\\.[0-9]+", ".#");
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
		if ((flags & IGNORE_CASE) != 0) {
			s1 = s1.toLowerCase();
			s2 = s2.toLowerCase();
			lines1 = s1.split("\r?\n");
			lines2 = s2.split("\r?\n");
		}
//		if ((flags & IGNORE_BLANK_LINES) != 0) {
//			while (s1.startsWith("\r") || s1.startsWith("\n")) {
//				s1 = s1.substring(1);
//			}
//			while (s2.startsWith("\r") || s2.startsWith("\n")) {
//				s2 = s2.substring(1);
//			}
//			while (s1.endsWith("\r") || s1.endsWith("\n")) {
//				s1 = s1.substring(0, s1.length() - 1);
//			}
//			while (s2.endsWith("\r") || s2.endsWith("\n")) {
//				s2 = s2.substring(0, s2.length() - 1);
//			}
//			s1 = s1.replaceAll("\r?\n[ \t]{0,999}\r?\n", "\n");
//			s2 = s2.replaceAll("\r?\n[ \t]{0,999}\r?\n", "\n");
//			lines1 = s1.split("\r?\n");
//			lines2 = s2.split("\r?\n");
//		}
		if ((flags & IGNORE_CHARORDER) != 0) {
			ArrayList<String> lines1Sorted = new ArrayList<String>();
			for (String line : lines1) {
				line = StringUtils.sortChars(line);
				lines1Sorted.add(line);
			}
			lines1 = lines1Sorted.toArray(new String[0]);
			s1 = StringUtils.join(lines1, "\n");

			ArrayList<String> lines2Sorted = new ArrayList<String>();
			for (String line : lines2) {
				line = StringUtils.sortChars(line);
				lines2Sorted.add(line);
			}
			lines2 = lines2Sorted.toArray(new String[0]);
			s2 = StringUtils.join(lines2, "\n");
		}
		if ((flags & IGNORE_LINEORDER) != 0) {
			Arrays.sort(lines1);
			Arrays.sort(lines2);
			s1 = StringUtils.join(lines1, "\n");
			s2 = StringUtils.join(lines2, "\n");
		}
		if ((flags & IGNORE_WHITESPACE) != 0) {
			for (int i = 0; i < lines1.length; i++) {
				lines1[i] = StringUtils.removeWhitespace(lines1[i]);
			}
			for (int i = 0; i < lines2.length; i++) {
				lines2[i] = StringUtils.removeWhitespace(lines2[i]);
			}
		}

		if (StringUtils.trimR(s1).equals(StringUtils.trimR(s2))) {
			return NO_DIFFS_MESSAGE;
		}

		// build a reverse-index array using the line as key and line number as value
		// don't store blank lines, so they won't be targets of the shortest distance
		// search
		Map<String, Set<Integer>> reverse1 = new TreeMap<String, Set<Integer>>();
		Map<String, Set<Integer>> reverse2 = new TreeMap<String, Set<Integer>>();
		for (int i = 0; i < lines1.length; i++) {
			String line = lines1[i];
			if (line.length() > 0) {
				if (!reverse1.containsKey(line)) {
					reverse1.put(line, new TreeSet<Integer>());
				}
				reverse1.get(line).add(i);
			}
		}
		for (int i = 0; i < lines2.length; i++) {
			String line = lines2[i];
			if (line.length() > 0) {
				if (!reverse2.containsKey(line)) {
					reverse2.put(line, new TreeSet<Integer>());
				}
				reverse2.get(line).add(i);
			}
		}

		// indexes of current lines within each file
		// (start at beginning of each list)
		int index1 = 0;
		int index2 = 0;
		List<Integer> actions = new ArrayList<Integer>();

		// walk this loop until we reach the end of one of the lists of lines
		while (index1 < (int) lines1.length && index2 < (int) lines2.length) {
			// if we have a common line, save it and go to the next
			if (lines1[index1].equals(lines2[index2])) {
				actions.add(4);
				index1++;
				index2++;
				continue;
			}
			// otherwise, find the shortest move (Manhattan-distance) from the
			// current location
			int best1 = lines1.length;
			int best2 = lines2.length;
			int sub1 = index1;
			int sub2 = index2;
			while ((sub1 + sub2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
				int d = -1;
				if (lines2.length > sub2 && reverse1.containsKey(lines2[sub2])) {
					for (int lineNumber : reverse1.get(lines2[sub2])) {
						if (lineNumber >= sub1) {
							d = lineNumber;
							break;
						}
					}
				}
				if (d >= sub1 && (d + sub2 - index1 - index2) < (best1 + best2 - index1 - index2)) {
					best1 = d;
					best2 = sub2;
				}

				d = -1;
				if (lines1.length > sub1 && reverse2.containsKey(lines1[sub1])) {
					for (int lineNumber : reverse2.get(lines1[sub1])) {
						if (lineNumber >= sub2) {
							d = lineNumber;
							break;
						}
					}
				}
				if (d >= sub2 && (d + sub1 - index1 - index2) < (best1 + best2 - index1 - index2)) {
					best1 = sub1;
					best2 = d;
				}

				sub1++;
				sub2++;
			}

			while (index1 < best1) {
				actions.add(1);
				index1++;
			}  // deleted elements

			while (index2 < best2) {
				actions.add(2);
				index2++;
			}  // added elements
		}

		// we've reached the end of one list, now walk to the end of the other
		while (index1 < (int) lines1.length) {
			actions.add(1);
			index1++;
		}  // deleted elements

		if ((flags & IGNORE_TRAILING) != 0) {
			while (index2 < (int) lines2.length) {
				actions.add(2);
				index2++;
			}  // added elements
		}

		// and this marks our ending point
		actions.add(8);

		int op = 0;
		int x0 = 0;
		int x1 = 0;
		int y0 = 0;
		int y1 = 0;
		List<String> out = new ArrayList<String>();

		for (int action : actions) {
			if (action == 1) {
				op |= action;
				x1++;
				continue;
			} else if (action == 2) {
				op |= action;
				y1++;
				continue;
			}

			List<String> outTemp = new ArrayList<String>();
			List<String> outLinesTemp = new ArrayList<String>();
			if (op > 0) {
				boolean multipleLines = (x1 != x0 + 1);
				String xstr = "" + (multipleLines ? ((x0 + 1) + "-" + x1) : x1);
				String ystr = "" + ((y1 != y0 + 1) ? ((y0 + 1) + "-" + y1) : y1);
				String linesStr = "\nLine" + (multipleLines ? "s " : " ");
				String doStr = "do" + (multipleLines ? "" : "es");
				if (op == 1) {
					outTemp.add(linesStr + xstr + " deleted near student line " + y1);
				} else if (op == 3) {
					if (xstr.equals(ystr)) {
						outTemp.add(linesStr + xstr + " " + doStr + " not match");
					} else {
						outTemp.add(linesStr + xstr + " changed to student line " + ystr);
					}
				}

				while (x0 < x1) {
					if ((flags & IGNORE_BLANK_LINES) == 0 || !lines1Original[x0].trim().isEmpty()) {
						outLinesTemp.add("EXPECTED < " + lines1Original[x0]);
					}
					x0++;
				}   // deleted elems

				if (op == 2) {
					if ((flags & IGNORE_LEADING) == 0 || x1 > 0) {
						// out.add(linesStr + x1 + " added at student line " + ystr);
						// Marty 2015/04/21
						outTemp.add(linesStr + " added near student line " + ystr);
					}
				} else if (op == 3) {
					// out += "---";
				}

				while (y0 < y1) {
					if ((flags & IGNORE_LEADING) == 0 || op != 2 || x1 > 0) {
						if ((flags & IGNORE_BLANK_LINES) == 0 || !lines2Original[y0].trim().isEmpty()) {
							outLinesTemp.add("STUDENT  > " + lines2Original[y0]);
						}
					}
					y0++;
				}   // added elems
			}
			
			// decide whether we should show this diff, based on flags
			// (Basically, outTemp almost always has stuff in it, but flags might strip outLinesTemp,
			//  and if the lines got stripped, we shouldn't show any of it.)
			if (!outTemp.isEmpty() && !outLinesTemp.isEmpty()) {
				out.addAll(outTemp);
				out.addAll(outLinesTemp);
			}
			
			x1++;
			x0 = x1;
			y1++;
			y0 = y1;
			op = 0;
		}

		if (out.size() > 0) {
			out.add("");
			return StringUtils.join(out, "\n").trim();
		} else {
			return NO_DIFFS_MESSAGE;
		}
	}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/**
	 * Returns a side-by-side comparision using the diff tool.
	 *
	 * @param str1 the first string. Output appears on the left.
	 * @param str2 the string to compare to str2. Output is on the right.
	 * @param a side-by-side diff of str1 and str2
	 */
	public static String diffSideBySide(String str1, String str2) {
		return diffSideBySide(str1, str2, StringUtils.getWidth(str1));
	}
	
	/**
	 * Returns a side-by-side comparision using the diff tool.
	 *
	 * @param str1 the first string. Output appears on the left.
	 * @param str2 the string to compare to str2. Output is on the right.
	 * @param a side-by-side diff of str1 and str2
	 */
	// MARTY NOTE 2015/04/21: this function seems to not work properly? fix? TODO
	public static String diffSideBySide(String str1, String str2, int width) {
		str1 = String.valueOf(str1);
		str2 = String.valueOf(str2);
		StringBuilder output = new StringBuilder();
		try {
			String[] cparts = str1.split("\r?\n");
			String[] sparts = str2.split("\r?\n");
			DiffCollection differ = new DiffCollection(cparts, sparts);
			for (DiffCollection.Difference diff : differ.diff()) {
				int delStart = diff.getDeletedStart();
				int delEnd = diff.getDeletedEnd();
				int addStart = diff.getAddedStart();
				int addEnd = diff.getAddedEnd();
				String from = diff.deletedIndexToString();
				String to = diff.addedIndexToString();
				String type = delEnd != DiffCollection.Difference.NONE
						&& addEnd != DiffCollection.Difference.NONE ? " changed "
						: (delEnd == DiffCollection.Difference.NONE ? " added " : " deleted ");

				output.append(from + type + to + '\n');
				int deli = delStart;
				int addi = addStart;
				while(deli <= delEnd || addi <= addEnd) {
					output.append(String.format("%-" + width + "s	|	%s\n",
							deli <= delEnd ? cparts[deli] : "",
							addi <= addEnd ? sparts[addi] : ""));
					deli++;
					addi++;
				}
			}
			return output.toString();
		} catch(Exception e) {
			return "Exception thrown: " + ExceptionUtils.stackTraceToString(e);
		} catch(Error e2) {
			return "Error thrown: " + e2;
		}
	}
}
