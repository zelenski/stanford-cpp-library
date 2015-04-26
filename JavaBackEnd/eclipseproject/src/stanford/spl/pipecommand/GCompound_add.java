package stanford.spl.pipecommand;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.TopCompound;
import acm.graphics.GObject;
import acm.util.TokenScanner;

public class GCompound_add extends JBECommand {
	public void execute(TokenScanner paramTokenScanner,
			JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str = nextString(paramTokenScanner);
		GObject localGObject1 = paramJavaBackEnd.getGObject(str);
		paramTokenScanner.verifyToken(",");
		str = nextString(paramTokenScanner);
		GObject localGObject2 = paramJavaBackEnd.getGObject(str);
		paramTokenScanner.verifyToken(")");
		if ((localGObject1 != null) && (localGObject2 != null)) {
			((TopCompound) localGObject1).add(localGObject2);
		}
	}
}
