package stanford.spl;

import acm.graphics.*;
import acm.util.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/10/21
 */
class GBufferedImage_save extends JBECommand {
	// gbufferedimage.save("foobar.png");
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = paramJavaBackEnd.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.save(filename);
		}
	}
}
