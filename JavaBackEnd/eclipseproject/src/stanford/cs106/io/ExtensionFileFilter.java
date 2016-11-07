/*
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

	@Override
	public String getDescription() {
		if (extensions == null || extensions.length == 0) {
			return "Files";
		} else {
			return "." + extensions[0] + " files";
		}
	}
}
