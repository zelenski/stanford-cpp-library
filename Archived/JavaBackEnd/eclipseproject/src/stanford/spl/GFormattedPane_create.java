package stanford.spl;

import acm.util.TokenScanner;

public class GFormattedPane_create extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(")");
		
		GFormattedPane pane = new GFormattedPane();
		jbe.defineGObject(id, pane);
		pane.addHyperlinkListener(jbe.createHyperlinkListener(pane));
		
		// TODO: getInteractor here returns the JScrollPane; should it be instead linked to the JEditorPane?
		jbe.defineSource(pane.getInteractor(), id);
	}
}