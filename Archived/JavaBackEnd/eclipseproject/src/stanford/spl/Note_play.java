/*
 * @version 2017/04/25
 * - changed from writeAck to writeOK
 */

package stanford.spl;

import acm.util.TokenScanner;
import stanford.cs106.audio.*;

public class Note_play extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String noteStr = SplPipeDecoder.readAndDecode(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		Note note = new Note(noteStr);
		note.play();
		SplPipeDecoder.writeOK();   // tell C++ that note is done playing
	}
}
