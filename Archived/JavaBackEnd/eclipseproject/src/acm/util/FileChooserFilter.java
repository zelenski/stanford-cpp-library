package acm.util;

import java.io.File;
import java.util.StringTokenizer;
import javax.swing.filechooser.FileFilter;

public class FileChooserFilter extends FileFilter {
	private String filenamePattern;
	private String filenameDescription;
	private String dir;

	public FileChooserFilter(String path) {
		this(path, null);
	}

	public FileChooserFilter(String path, String desc) {
		if (path == null) {
			path = "";
		}
		if (desc == null) {
			desc = "All files";
		}
		int i = Math.max(path.lastIndexOf("/"), path.lastIndexOf('\\'));
		filenamePattern = path.substring(i + 1);
		dir = i != -1 ? path.substring(0, i) : "";
		if (dir.isEmpty()) {
			dir = System.getProperty("user.dir");
		} else if (!dir.startsWith("/")) {
			dir = System.getProperty("user.dir") + "/" + dir;
		}
		
		filenameDescription = desc;
	}

	public String getDirectory() {
		return dir;
	}

	public String getPattern() {
		return filenamePattern;
	}

	public String getDescription() {
		return filenameDescription;
	}

	public boolean accept(File file) {
		if (file.isDirectory() || filenamePattern.isEmpty())
			return true;
		for (StringTokenizer stringtokenizer = new StringTokenizer(filenamePattern, ";");
				stringtokenizer.hasMoreTokens(); ) {
			String s = stringtokenizer.nextToken();
			if (s.length() > 0 && JTFTools.matchFilenamePattern(file.getName(), s)) {
				return true;
			}
		}

		return false;
	}
}
