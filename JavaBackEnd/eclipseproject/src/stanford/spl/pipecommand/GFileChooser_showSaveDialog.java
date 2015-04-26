package stanford.spl.pipecommand;

import acm.util.TokenScanner;

import java.io.*;

import javax.swing.*;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

public class GFileChooser_showSaveDialog extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String currentDir = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		if (currentDir.isEmpty()) {
			try {
				currentDir = System.getProperty("user.dir");
			} catch (Exception e) {}
		}
		JFileChooser chooser = new JFileChooser(currentDir);
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
