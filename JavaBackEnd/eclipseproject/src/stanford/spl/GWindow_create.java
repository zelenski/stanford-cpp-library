package stanford.spl;

import stanford.spl.TopCompound;
import acm.util.TokenScanner;

public class GWindow_create extends JBESwingCommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String str1 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int width = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int height = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		String str2 = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		boolean visible = nextBoolean(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		// creates an object of type JBEWindow
		paramJavaBackEnd.createWindow(str1, width, height,
				(TopCompound) paramJavaBackEnd.getGObject(str2), visible);
		paramJavaBackEnd.println("result:ok");
	}
}