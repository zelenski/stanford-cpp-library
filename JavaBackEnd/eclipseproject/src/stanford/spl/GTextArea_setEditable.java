package stanford.spl;

import acm.util.TokenScanner;

public class GTextArea_setEditable extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		final String id = nextString(scanner);
		scanner.verifyToken(",");
		final boolean flag = nextBoolean(scanner);
		scanner.verifyToken(")");
		GTextArea area = (GTextArea) jbe.getGObject(id);
		if (area != null) { 
			area.setEditable(flag);
		} else {
			System.out.println("error:GTextArea_setEditable: null text area");
		}
	}
}
