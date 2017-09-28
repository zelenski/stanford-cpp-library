package stanford.spl;

import acm.graphics.*;
import acm.util.*;

/**
 * 
 * @author Marty Stepp
 * @version 2017/09/27
 * - bug fix for loading from bogus files (e.g. *.o); was causing NullPointerException
 */
public class GBufferedImage_load extends JBESwingCommand {
	// gbufferedimage.load("foobar.png");
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GObject gobj = paramJavaBackEnd.getGObject(id);
		if (gobj != null && gobj instanceof GBufferedImage) {
			GBufferedImage img = (GBufferedImage) gobj;
			try {
				img.load(filename);
				String b64 = img.toStringBase64();
				SplPipeDecoder.writeResult(b64);   // this is a LONG string
			} catch (Exception ex) {
				SplPipeDecoder.writeError(ex);
			}
		}
	}
}
