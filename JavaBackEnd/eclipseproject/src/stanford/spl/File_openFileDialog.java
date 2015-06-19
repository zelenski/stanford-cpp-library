package stanford.spl.pipecommand;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;
import acm.util.TokenScanner;

public class File_openFileDialog extends JBECommand {
	public void execute(TokenScanner paramTokenScanner,
			JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String title = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String mode = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String path = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		String result = paramJavaBackEnd.openFileDialog(title, mode, path);
		SplPipeDecoder.writeResult(result);
	}
}
