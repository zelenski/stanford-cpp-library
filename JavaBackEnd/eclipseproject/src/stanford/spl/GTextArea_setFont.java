package stanford.spl;

import acm.util.TokenScanner;

public class GTextArea_setFont extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		final String id = nextString(scanner);
		scanner.verifyToken(",");
		final String font = nextString(scanner);
		scanner.verifyToken(")");
		GTextArea area = (GTextArea) jbe.getGObject(id);
		if (area != null) {
			area.setFont(font);
		} else {
			System.out.println("error:GTextArea_setFont: null text area");
		}
	}
}
