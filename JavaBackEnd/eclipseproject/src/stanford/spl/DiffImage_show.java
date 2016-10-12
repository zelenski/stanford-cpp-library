/*
 * Given two file names, pops up a DiffImage window to compare them for differences.
 * @version 2016/07/30
 * - added descriptive text labels to the two images
 * @version 2016/07/06
 * - initial version
 */

package stanford.spl;

import acm.util.*;
import stanford.cs106.diff.DiffImage;
import stanford.cs106.io.IORuntimeException;

import java.awt.*;
import java.io.*;

public class DiffImage_show extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String filename1 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename2 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		Image image1 = MediaTools.loadImage(filename1);
		Image image2 = MediaTools.loadImage(filename2);
		File file1 = new File(filename1);
		File file2 = new File(filename2);
		try {
			DiffImage diff = new DiffImage(image1, image2);
			diff.setImage1Label(file1.getName());
			diff.setImage2Label(file2.getName());
			diff.setVisible(true);
			
			// useless "ok" result for C++ lib to throw away, to make dialog modal
			SplPipeDecoder.writeResult("ok");
		} catch (IORuntimeException ioe) {
			SplPipeDecoder.writeResult("error:" + ioe.getMessage());
		}
	}
}
