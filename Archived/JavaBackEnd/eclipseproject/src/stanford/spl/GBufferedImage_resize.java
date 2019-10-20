package stanford.spl;

import acm.graphics.*;
import acm.util.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/10/08
 */
public class GBufferedImage_resize extends JBESwingCommand {
	// gbufferedimage.setRGB(x, y, rgb);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int w = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int h = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String retain = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = paramJavaBackEnd.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			img.resize(w, h, retain.equals("true"));
		}
	}
}
