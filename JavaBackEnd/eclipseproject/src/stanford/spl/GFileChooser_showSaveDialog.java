package stanford.spl;

import acm.util.TokenScanner;
import java.io.*;
import javax.swing.*;

public class GFileChooser_showSaveDialog extends JBESwingCommand {
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
			chooser.setFileFilter(new GFileChooser_showOpenDialog.GFileChooserFileFilter(fileFilter));
		}
		int result = chooser.showSaveDialog(paramJavaBackEnd.getJBEConsoleFrame());
		String filename = "";
		if (result == JFileChooser.APPROVE_OPTION) {
			File file = chooser.getSelectedFile();
			if (file != null) {
				filename = file.getAbsolutePath();
			}
		}
		SplPipeDecoder.writeResult(filename);
	}
}
