package stanford.spl;

import acm.util.FileChooserFilter;

public class JBEFileFilter extends FileChooserFilter {
	private String dir;

	public JBEFileFilter(String path) {
		super(getPattern(path));
		int i = Math.max(path.lastIndexOf("/"),
				path.lastIndexOf('\\'));
		this.dir = (i == -1 ? "" : path.substring(0, i));
		String str = path.substring(i + 1);
		if (!isPattern(str)) {
			if (dir.isEmpty() && str.isEmpty()) {
				dir += "/";
			}
			this.dir += str;
		}
		if (this.dir.isEmpty()) {
			this.dir = System.getProperty("user.dir");
		} else if (!this.dir.startsWith("/")) {
			this.dir = (System.getProperty("user.dir") + "/" + this.dir);
		}
	}

	public String getDirectory() {
		return this.dir;
	}

	// JL: getPatternPart?
	
	private static String getPattern(String paramString) {
		int i = Math.max(paramString.lastIndexOf("/"),
				paramString.lastIndexOf('\\'));
		String str = paramString.substring(i + 1);
		return isPattern(str) ? str : "";   // JL BUGFIX: null -> ""
	}

	private static boolean isPattern(String paramString) {
		for (int i = 0; i < paramString.length(); i++) {
			switch (paramString.charAt(i)) {
			case '*':
			case ';':
			case '?':
			case '[':
			case ']':
				return true;
			}
		}
		return false;
	}
}
