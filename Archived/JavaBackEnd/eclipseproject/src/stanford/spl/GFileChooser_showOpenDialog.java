/*
 * @version 2017/09/28
 * - made file filter work with comma-separated lists of filters
 * - moved heavy lifting code out to JFileChooserUtils
 */

package stanford.spl;

import acm.util.TokenScanner;
import java.io.*;
import javax.swing.*;
import stanford.cs106.gui.*;

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
		
		JFileChooser chooser = JFileChooserUtils.createChooser(currentDir, fileFilter);
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
	
}
