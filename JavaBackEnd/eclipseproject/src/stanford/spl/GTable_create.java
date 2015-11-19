package stanford.spl;

import java.awt.*;
import acm.util.TokenScanner;

public class GTable_create extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd paramJavaBackEnd) {
		paramTokenScanner.verifyToken("(");
		String id = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int numRows = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		int numCols = nextInt(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		double x = nextDouble(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		double y = nextDouble(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		double w = nextDouble(paramTokenScanner);
		paramTokenScanner.verifyToken(",");
		double h = nextDouble(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		
		GTable table = new GTable(numRows, numCols);
		table.setJavaBackEnd(paramJavaBackEnd);
		table.setLocation(x, y);
		if (w > 0 && h > 0) {
			table.getInteractor().setPreferredSize(new Dimension((int) w, (int) h));
		}
		paramJavaBackEnd.defineGObject(id, table);
		paramJavaBackEnd.defineSource(table.getInteractor(), id);
	}
}
