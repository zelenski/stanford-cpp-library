package stanford.spl;

import stanford.spl.TopCompound;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GCompound_add extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String str = nextString(scanner);
		GObject localGObject1 = jbe.getGObject(str);
		scanner.verifyToken(",");
		str = nextString(scanner);
		GObject localGObject2 = jbe.getGObject(str);
		scanner.verifyToken(")");
		if ((localGObject1 != null) && (localGObject2 != null)) {
			((TopCompound) localGObject1).add(localGObject2);
			SplPipeDecoder.writeOK();
		} else {
			SplPipeDecoder.writeError("GCompound_add: an object was null");
		}
	}
}
