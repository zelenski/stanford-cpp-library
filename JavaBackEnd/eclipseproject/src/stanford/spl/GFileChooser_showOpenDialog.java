package stanford.spl;

import acm.util.TokenScanner;
import java.io.*;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;

public class GFileChooser_showOpenDialog extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String currentDir = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String fileFilter = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (currentDir.isEmpty()) {
			try {
				currentDir = System.getProperty("user.dir");
			} catch (Exception e) {}
		}
		JFileChooser chooser = new JFileChooser(currentDir);
		if (!fileFilter.isEmpty()) {
			chooser.setFileFilter(new GFileChooserFileFilter(fileFilter));
		}
		int result = chooser.showOpenDialog(paramJavaBackEnd.getJBEConsoleFrame());
		String filename = "";
		if (result == JFileChooser.APPROVE_OPTION) {
			File file = chooser.getSelectedFile();
			if (file != null) {
				filename = file.getAbsolutePath();
			}
		}
		SplPipeDecoder.writeResult(filename);
	}
	
	public static class GFileChooserFileFilter extends FileFilter {
		private String fileFilter;
		
		public GFileChooserFileFilter(String fileFilter) {
			this.fileFilter = fileFilter;
		}
		
		@Override
		public boolean accept(File f) {
			String filterRegex = fileFilter.replace(".", "\\.");
			filterRegex = filterRegex.replace("*", ".*");
			// JOptionPane.showMessageDialog(null, "filterRegex = " + filterRegex);
			return f.isDirectory() || f.getName().matches(".*" + filterRegex + ".*");
		}

		@Override
		public String getDescription() {
			return fileFilter + " files";
		}
	}
}
