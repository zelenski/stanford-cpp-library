package stanford.karel;

import java.awt.*;
import javax.swing.*;

class ResizeCanvas extends JPanel {
	private static final int SIZE = 102;
	private static final Font FONT = new Font("Helvetica", Font.PLAIN, 12);

	private int rows;
	private int cols;
	private int sqSize;

	public void setDimension(int width, int height) {
		this.cols = width;
		this.rows = height;
	}

	public Dimension getPreferredSize() {
		return new Dimension(SIZE, SIZE);
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.setColor(Color.BLACK);
		g.drawRect(0, 0, SIZE - 1, SIZE - 1);
		int sqSize = (SIZE - 2) / Math.max(rows, cols);
		int x = (SIZE - sqSize * (cols - 1)) / 2;
		for (int ix = 1; ix <= cols; ix++) {
			int y = (SIZE + sqSize * (rows - 1)) / 2;
			for (int iy = 1; iy <= rows; iy++) {
				drawCornerMarker(g, x, y);
				y -= sqSize;
			}
			x += sqSize;
		}
		String str = cols + "x" + rows;
		g.setFont(FONT);
		FontMetrics fm = g.getFontMetrics();
		int width = fm.stringWidth(str) + 6;
		int height = fm.getHeight() + 2;
		g.setColor(Color.WHITE);
		g.fillRect((SIZE - width) / 2, (SIZE - height) / 2, width, height);
		g.setColor(Color.BLACK);
		g.drawString(str, (SIZE - fm.stringWidth(str)) / 2,
				(SIZE + fm.getAscent()) / 2);
	}

	private void drawCornerMarker(Graphics g, int x, int y) {
		if (sqSize < KarelWorld.CROSS_THRESHOLD) {
			g.drawLine(x, y, x, y);
		} else {
			g.drawLine(x - 1, y, x + 1, y);
			g.drawLine(x, y - 1, x, y + 1);
		}
	}
}
