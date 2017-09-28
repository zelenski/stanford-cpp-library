/*
 * @version 2017/09/28
 * - initial version; most of code came from GFileChooser_showOpenDialog.java
 */

package stanford.cs106.gui;

import java.awt.*;
import java.io.*;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;

public final class JFileChooserUtils {
	private static final int MAX_PREF_WIDTH = 700;
	
	private JFileChooserUtils() {
		// empty
	}
	
	/**
	 * Creates a new file chooser pointed to the given current directory,
	 * set to filter its display to show only the files in the given filter.
	 * @param currentDir directory to start in
	 * @param fileFilter e.g. "*.jpg,*.txt"
	 * @return the created file chooser object
	 */
	public static JFileChooser createChooser(String currentDir, String fileFilter) {
		JFileChooser chooser = new JFileChooser(currentDir);
		
		// constrain preferred size of chooser
		Dimension prefSize = chooser.getPreferredSize();
		if (prefSize.width > MAX_PREF_WIDTH) {
			prefSize.width = MAX_PREF_WIDTH;
			chooser.setPreferredSize(prefSize);
		}
		
		if (fileFilter != null && !fileFilter.isEmpty()) {
			chooser.setFileFilter(new GFileChooserFileFilter(fileFilter));
		}
		
		return chooser;
	}

	public static class GFileChooserFileFilter extends FileFilter {
		private String fileFilter;
		
		public GFileChooserFileFilter(String fileFilter) {
			this.fileFilter = fileFilter;
		}
		
		@Override
		public boolean accept(File f) {
			String[] filters = fileFilter.split("[ ]*,[ ]*");
			for (String filter : filters) {
				String filterRegex = ".*" + filter.replace(".", "\\.").replace("*", ".*") + ".*";
				if (f.isDirectory() || f.getName().matches(filterRegex)) {
					return true;
				}
			}
			return false;
		}

		@Override
		public String getDescription() {
			return fileFilter + " files";
		}
	}
}
