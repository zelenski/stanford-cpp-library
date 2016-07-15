/*
 * @version 2016/07/06
 */

package stanford.spl;

import acm.util.*;
import stanford.cs106.diff.DiffImage;
import java.awt.*;
import java.io.*;

public class DiffImage_show extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String file1 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String file2 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		Image image1 = MediaTools.loadImage(file1);
		Image image2 = MediaTools.loadImage(file2);
		try {
			DiffImage diff = new DiffImage(image1, image2);
			diff.setVisible(true);
		} catch (IOException ioe) {
			SplPipeDecoder.writeResult("error:" + ioe.getMessage());
		}
		
		// useless "ok" result for C++ lib to throw away, to make dialog modal
		SplPipeDecoder.writeResult("ok");
	}
}
