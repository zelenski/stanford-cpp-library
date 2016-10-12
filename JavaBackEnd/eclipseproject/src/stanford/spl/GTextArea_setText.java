package stanford.spl;

import acm.util.TokenScanner;

public class GTextArea_setText extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		final String id = nextString(scanner);
		scanner.verifyToken(",");
		final String text = nextString(scanner);
		scanner.verifyToken(")");
		GTextArea area = (GTextArea) jbe.getGObject(id);
		if (area != null) { 
			area.setText(text);
		} else {
			System.out.println("error:GTextArea_setText: null text area");
		}
	}
}
