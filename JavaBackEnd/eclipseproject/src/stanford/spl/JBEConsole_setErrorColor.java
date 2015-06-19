package stanford.spl.pipecommand;

import java.awt.Color;

import stanford.spl.JBECommand;
import stanford.spl.JBEConsole;
import stanford.spl.JavaBackEnd;
import acm.io.ConsoleModel;
import acm.io.StandardConsoleModel;
import acm.util.JTFTools;
import acm.util.TokenScanner;

public class JBEConsole_setErrorColor extends JBECommand {
	public void execute(TokenScanner paramTokenScanner, JavaBackEnd jbe) {
		paramTokenScanner.verifyToken("(");
		String colorHex = nextString(paramTokenScanner);
		paramTokenScanner.verifyToken(")");
		Color color = JTFTools.decodeColor(colorHex);
		
		JBEConsole console = jbe.getJBEConsole();
		ConsoleModel model = console.getConsoleModel();
		if (model instanceof StandardConsoleModel) {
			StandardConsoleModel smodel = (StandardConsoleModel) model;
			smodel.setOutputColor(color);
		}
	}
}
