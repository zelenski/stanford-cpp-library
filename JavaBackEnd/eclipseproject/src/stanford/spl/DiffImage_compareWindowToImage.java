/*
 * Given a graphical window and an image file name, pops up a DiffImage window to compare the
 * window (actual output) to the image (expected output) for differences.
 * @version 2016/10/16
 * - added ignoreWindowSize option
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

public class DiffImage_compareWindowToImage extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String windowID = nextString(paramTokenScanner);
		JBEWindow window = paramJavaBackEnd.getWindow(windowID);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean ignoreWindowSize = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		if (window != null) {
			// convert the window canvas contents into an Image
			JBECanvas canvas = window.getCanvas();
			BufferedImage windowImage = canvas.toImage();
			
			// diff it
			File file = new File(filename);
			try {
				
				Image fileImage = MediaTools.loadImage(filename);
				
				if (ignoreWindowSize) {
					// enlarge both to same (larger) size
					int w1 = windowImage.getWidth();
					int w2 = fileImage.getWidth(window);
					int wmax = Math.max(w1, w2);
					int h1 = windowImage.getHeight();
					int h2 = fileImage.getHeight(window);
					int hmax = Math.max(h1, h2);
					
					boolean opaque = canvas.isOpaque();
					Color background = canvas.getBackground();
					int backgroundColor = background.getRGB();
					
					BufferedImage bfileImage = new BufferedImage(wmax, hmax, BufferedImage.TYPE_INT_ARGB);
					if (opaque) {
						bfileImage.getGraphics().setColor(background);
						bfileImage.getGraphics().fillRect(0, 0, wmax, hmax);
					}
					bfileImage.getGraphics().drawImage(fileImage, 0, 0, window);
					
					if (w1 < wmax || h1 < hmax) {
						windowImage = window.getCanvas().toImage(wmax, hmax);
						
						for (int x = 0; x < wmax; x++) {
							for (int y = 0; y < hmax; y++) {
								if ((x >= w1 || y >= h1) && (x < w2 && y < h2)) {
									int rgb = bfileImage.getRGB(x, y);
									if (opaque && (rgb & 0xff000000) == 0) {
										rgb = backgroundColor;
									}
									windowImage.setRGB(x, y, rgb);
								}
							}
						}
					}

					if (w2 < wmax || h2 < hmax) {
						for (int x = 0; x < wmax; x++) {
							for (int y = 0; y < hmax; y++) {
								if ((x >= w2 || y >= h2) && (x < w1 && y < h1)) {
									int rgb = windowImage.getRGB(x, y);
									if (opaque && (rgb & 0xff000000) == 0) {
										rgb = backgroundColor;
									}
									bfileImage.setRGB(x, y, rgb);
								}
							}
						}
						fileImage = bfileImage;
					}
				}
				
				DiffImage diff = new DiffImage(fileImage, windowImage);
				diff.setImage1Label(file.getName());
				diff.setImage2Label("window");
				diff.setVisible(true);
				
				// useless "ok" result for C++ lib to throw away, to make dialog modal
				SplPipeDecoder.writeOK();
			} catch (IORuntimeException ioe) {
				SplPipeDecoder.writeError(ioe.getMessage());
			}
		}
		
	}
}
