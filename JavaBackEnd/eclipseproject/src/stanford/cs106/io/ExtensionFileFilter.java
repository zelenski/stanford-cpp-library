package stanford.cs106.io;

import java.io.File;
import java.io.FileFilter;

/*
 * ...
 */
public class ExtensionFileFilter implements FileFilter {
	private String[] extensions;
	
	/*
	 * ...
	 */
	public ExtensionFileFilter(String... extensions) {
		this.extensions = extensions;
	}
	
	/*
	 * ...
	 */
	public boolean accept(File file) {
		if (file.isDirectory()) {
			return true;
		}
		String filename = file.getName().toLowerCase();
		for (String extension : extensions) {
			extension = "." + extension.toLowerCase();
			if (filename.endsWith(extension)) {
				return true;
			}
		}
		return false;
	}
}
