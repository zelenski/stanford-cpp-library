package stanford.spl;

import java.awt.*;

import acm.util.TokenScanner;

public class GWindow_setRegionSize extends JBESwingCommand {
	public void execute(TokenScanner scanner, JavaBackEnd jbe) {
		scanner.verifyToken("(");
		String id = nextString(scanner);
		scanner.verifyToken(",");
		String region = nextString(scanner);
		if (!region.isEmpty()) {
			// BorderLayout requires regions in title case, e.g. "North"
			region = region.substring(0, 1).toUpperCase() + region.substring(1).toLowerCase();
		}
		scanner.verifyToken(",");
		int width = (int) nextDouble(scanner); 
		scanner.verifyToken(",");
		int height = (int) nextDouble(scanner); 
		scanner.verifyToken(")");
		
		Dimension dim = new Dimension(width, height);
		JBEWindow window = jbe.getWindow(id);
		if (window != null) {
			Container pane = window.getContentPane();
			LayoutManager layout = pane.getLayout();
			if (layout instanceof BorderLayout) {
				BorderLayout border = (BorderLayout) layout;
				Component comp = border.getLayoutComponent(region);
				if (comp != null) {
					if (comp.isShowing()) {
						comp.setSize(dim);
						window.validate();
					} else {
						comp.setPreferredSize(dim);
					}
				} else if (region.equalsIgnoreCase("Center")) {
					// resize the window until the central area is the given size
					
				}
			}
		}
		SplPipeDecoder.writeResult("GDimension(" + dim.width + ", " + dim.height + ")");
	}
}
