/*
 * @version 2016/07/06
 */

package stanford.spl;

import acm.util.*;
import stanford.cs106.diff.DiffImage;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;

public class DiffImage_compareWindowToImage extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String windowID = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(windowID);
		paramTokenScanner.verifyToken(",");
		String file = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (window != null) {
			// convert the window canvas contents into an Image
			JBECanvas canvas = window.getCanvas();
			BufferedImage img = new BufferedImage(canvas.getWidth(), canvas.getHeight(), BufferedImage.TYPE_INT_ARGB);
			canvas.paint(img.getGraphics());
			
			// diff it
			Image image1 = img;
			Image image2 = MediaTools.loadImage(file);
			try {
				DiffImage diff = new DiffImage(image1, image2);
				diff.setVisible(true);
			} catch (IOException ioe) {
				SplPipeDecoder.writeResult("error:" + ioe.getMessage());
			}
		}
		
		// useless "ok" result for C++ lib to throw away, to make dialog modal
		SplPipeDecoder.writeResult("ok");
	}
}
