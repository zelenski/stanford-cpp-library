package stanford.spl;

import acm.util.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/08/05
 */
public class GBufferedImage_create extends JBESwingCommand {
	// gbufferedimage = new GBufferedImage(x, y, width, height);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int x = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int y = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int w = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int h = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GBufferedImage img = new GBufferedImage(w, h, rgb);
		img.setLocation(x, y);
		paramJavaBackEnd.defineGObject(id, img);
		paramJavaBackEnd.defineSource(img.getInteractor(), id);
	}
}
