package stanford.spl;

import java.awt.*;

import acm.util.TokenScanner;

public class GWindow_getRegionSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		String region = nextString(scanner);
		if (!region.isEmpty()) {
			// BorderLayout requires regions in title case, e.g. "North"
			region = region.substring(0, 1).toUpperCase() + region.substring(1).toLowerCase();
		}
		scanner.verifyToken(")");
		
		Dimension dim = new Dimension(0, 0);
		JBEWindow window = jbe.getWindow(id);
		if (window != null) {
			Container pane = window.getContentPane();
			LayoutManager layout = pane.getLayout();
			if (layout instanceof BorderLayout) {
				BorderLayout border = (BorderLayout) layout;
				Component comp = border.getLayoutComponent(region);
				if (comp != null) {
					dim = comp.isShowing() ? comp.getSize() : comp.getPreferredSize();
				}
			}
		}
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
