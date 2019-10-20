/*
 * @version 2017/10/22
 * - normalize various formats like "txt", ".txt", "*.txt"
 * @version 2016/11/03
 * - made it work for both java.io.FileFilter and javax.swing.filechooser.FileFilter (stupid)
 */

package stanford.cs106.io;

import java.io.File;

/*
 * ...
 */
public class ExtensionFileFilter
		extends javax.swing.filechooser.FileFilter
		implements java.io.FileFilter {
	private String description;
	private String[] extensions;
	
	/**
	 * Filters to keep only the given extensions (don't include the dot in front of each).
	 */
	public ExtensionFileFilter(String... extensions) {
		this.extensions = extensions;
		
		// ensure that extensions are in correct format;
		// should be "txt", not "*.txt"
		for (int i = 0; i < this.extensions.length; i++) {
			String ext = String.valueOf(this.extensions[i]).trim();
			if (ext.startsWith("*")) {
				ext = ext.substring(1);
			}
			if (!ext.startsWith(".")) {
				ext = "." + ext;
			}
			this.extensions[i] = ext;
		}
		
		if (extensions == null || extensions.length == 0) {
			description = "Files";
		} else {
			description = extensions[0] + " files";
		}
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
			if (filename.endsWith(extension.toLowerCase())) {
				return true;
			}
		}
		return false;
	}

	@Override
	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}
}
