package stanford.spl;

import acm.util.*;

/**
 * 
 * @author Marty Stepp
 * @version 2014/08/05
 */
public class File_getTempDirectory extends JBECommand {
	// gbufferedimage = new GBufferedImage(x, y, width, height);
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		paramTokenScanner.verifyToken(")");
		String tempDir = System.getProperty("java.io.tmpdir");
		SplPipeDecoder.encodeAndWrite(tempDir);
	}
}
