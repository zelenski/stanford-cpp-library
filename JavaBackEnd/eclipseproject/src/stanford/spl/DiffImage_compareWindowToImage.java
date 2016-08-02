/*
 * Given a graphical window and an image file name, pops up a DiffImage window to compare the
 * window (actual output) to the image (expected output) for differences.
 * @version 2016/07/30
 * - added descriptive text labels to the two sides of the diff
 * @version 2016/07/06
 * - initial version
 */

package stanford.spl;

import acm.util.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import stanford.cs106.diff.DiffImage;
import stanford.cs106.io.IORuntimeException;

public class DiffImage_compareWindowToImage extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String windowID = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(windowID);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (window != null) {
			// convert the window canvas contents into an Image
			JBECanvas canvas = window.getCanvas();
			BufferedImage img = new BufferedImage(canvas.getWidth(), canvas.getHeight(), BufferedImage.TYPE_INT_ARGB);
			canvas.paint(img.getGraphics());
			
			// diff it
			File file = new File(filename);
			try {
				
				Image image1 = MediaTools.loadImage(filename);
				Image image2 = img;
				
				DiffImage diff = new DiffImage(image1, image2);
				diff.setImage1Label(file.getName());
				diff.setImage2Label("window");
				diff.setVisible(true);
				
				// useless "ok" result for C++ lib to throw away, to make dialog modal
				SplPipeDecoder.writeResult("ok");
			} catch (IORuntimeException ioe) {
				SplPipeDecoder.writeResult("error:" + ioe.getMessage());
			}
		}
		
	}
}
