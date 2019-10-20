package stanford.spl;

import java.awt.*;
import acm.util.TokenScanner;

public class GTable_create extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		int numRows = nextInt(scanner);
		scanner.verifyToken(",");
		int numCols = nextInt(scanner);
		scanner.verifyToken(",");
		double x = nextDouble(scanner);
		scanner.verifyToken(",");
		double y = nextDouble(scanner);
		scanner.verifyToken(",");
		double w = nextDouble(scanner);
		scanner.verifyToken(",");
		double h = nextDouble(scanner);
		scanner.verifyToken(")");
		
		GTable table = new GTable(numRows, numCols);
		table.setJavaBackEnd(jbe);
		table.setLocation(x, y);
		if (w > 0 && h > 0) {
			table.getInteractor().setPreferredSize(new Dimension((int) w, (int) h));
		}
		jbe.defineGObject(id, table);
		jbe.defineSource(table.getInteractor(), id);
	}
}
