package stanford.spl;

import acm.util.TokenScanner;

public class GTextArea_getText extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		final String id = nextString(scanner);
		scanner.verifyToken(")");
		final GTextArea area = (GTextArea) jbe.getGObject(id);
		if (area != null) {
			String text = area.getText();
			jbe.println("result:" + text);
		} else {
			jbe.println("error:GTextArea_getText: null text area");
		}
	}
}
