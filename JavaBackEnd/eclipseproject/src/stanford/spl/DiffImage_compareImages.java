/*
 * Given three image file names, diffs the first two and writes the diff pixel output
 * into the third. 
 * @version 2016/07/30
 * - initial version
 */

package stanford.spl;

import acm.util.*;
import java.io.IOException;
import stanford.cs106.diff.DiffImage;
import stanford.cs106.io.IORuntimeException;

public class DiffImage_compareImages extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String filename1 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String filename2 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String outfile = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		try {
			DiffImage diff = new DiffImage(filename1, filename2, /* display */ false);
			diff.save(outfile);
		} catch (IOException ioe) {
			SplPipeDecoder.writeResult("error:" + ioe.getMessage());
		} catch (IORuntimeException ioe) {
			SplPipeDecoder.writeResult("error:" + ioe.getMessage());
		}
	}
}
