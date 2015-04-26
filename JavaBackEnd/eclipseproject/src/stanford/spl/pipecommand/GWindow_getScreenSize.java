package stanford.spl.pipecommand;

import java.awt.*;

import stanford.spl.JBECommand;
import stanford.spl.JavaBackEnd;
import stanford.spl.SplPipeDecoder;

import acm.util.TokenScanner;

public class GWindow_getScreenSize extends JBECommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		scanner.verifyToken(")");
		Dimension dim = Toolkit.getDefaultToolkit().getScreenSize();
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
