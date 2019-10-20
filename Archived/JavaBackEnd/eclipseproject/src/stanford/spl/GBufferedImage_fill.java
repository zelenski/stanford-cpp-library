package stanford.spl;

import acm.graphics.*;
import acm.util.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/08/05
 */
public class GBufferedImage_fill extends JBESwingCommand {
	// gbufferedimage.setRGB(x, y, rgb);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int rgb = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = paramJavaBackEnd.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.fill(rgb);
		}
	}
}
