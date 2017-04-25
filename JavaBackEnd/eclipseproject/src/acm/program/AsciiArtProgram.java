package acm.program;

import acm.io.*;
import java.awt.Color;
import java.util.*;

import javax.swing.JScrollPane;

/**
 * ...
 */
public class AsciiArtProgram extends ConsoleProgram {
	private String background = " ";
	private String fill = ".";
	private String paint = "*";
	private Map<String, String> matrix = new TreeMap<String, String>();
	private int maxX = 0;
	private int maxY = 0;
	private boolean redrawAutomatically = true;
	private StandardConsoleModel sConsole;
	
	public AsciiArtProgram() {
		super();
		
		IOConsole console = getConsole();
		ConsoleModel consoleModel = console.getConsoleModel();
		if (consoleModel instanceof StandardConsoleModel) {
			sConsole = (StandardConsoleModel) consoleModel;

			// disable scrollbars
			sConsole.getScrollPane().setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
			sConsole.getScrollPane().setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
			
			// hide blinking cursor
			sConsole.getTextPane().setCursor(null);
		}
	}
	
	private void checkBounds(int x, int y) {
//		if (y < 0 || y >= getRows() || x < 0 || x >= getColumns()) {
//			throw new IndexOutOfBoundsException("Illegal x/y value: ("
//					+ x + ", " + y + ")");
//		}
	}
	
	public void clear() {
		matrix.clear();
		maxX = 0;
		maxY = 0;
		conditionalRedraw();
	}
	
	private void conditionalRedraw() {
		if (redrawAutomatically) {
			redraw();
		}
	}
	
	public void drawCharacter(int x, int y) {
		drawCharacterHelper(x, y, paint, /* repaint */ true);
	}
	
	public void drawCharacter(int x, int y, String character) {
		if (character == null || character.isEmpty()) {
			character = paint;
		} else if (character.length() != 1) {
			character = character.substring(0, 1);
		}
		drawCharacterHelper(x, y, character, /* repaint */ true);
	}
	
	private void drawCharacterHelper(int x, int y) {
		drawCharacterHelper(x, y, paint, /* repaint */ false);
	}
	
//	private void drawCharacterHelper(int x, int y, String character) {
//		drawCharacterHelper(x, y, character, /* repaint */ false);
//	}
	
	private void drawCharacterHelper(int x, int y, String character, boolean repaint) {
		checkBounds(x, y);
		String key = getKey(x, y);
		matrix.put(key, character);
		if (x > maxX) {
			maxX = x;
		}
		if (y > maxY) {
			maxY = y;
		}
		if (repaint) {
			redraw();
		}
	}
	
	public void drawLine(double x1, double y1, double x2, double y2) {
		drawLine((int) x1, (int) y1, (int) x2, (int) y2);
	}
	
	public void drawLine(int x1, int y1, int x2, int y2) {
		if (y1 == y2) {
			// horizontal line
			for (int x = x1; x <= x2; x++) {
				drawCharacterHelper(x, y1);
			}
		} else if (x1 == x2) {
			// vertical line
			for (int y = y1; y <= y2; y++) {
				drawCharacterHelper(x1, y);
			}
		} else {
			// TODO
		}
		conditionalRedraw();
	}

	public void drawRect(double x, double y, double width, double height) {
		drawRect((int) x, (int) y, (int) width, (int) height);
	}
	
	public void drawRect(int x, int y, int width, int height) {
		drawLine(x, y, x + width, y);   // top
		drawLine(x, y, x, y + height);  // left
		drawLine(x + width, y, x + width, y + height);  // right
		drawLine(x, y + height, x + width, y + height);  // bottom
	}
	
	public void fillRect(double x, double y, double width, double height) {
		fillRect((int) x, (int) y, (int) width, (int) height);
	}
	
	public void fillRect(int x, int y, int width, int height) {
		for (int yy = y; yy < y + height; yy++) {
			for (int xx = x; xx < x + width; xx++) {
				drawCharacterHelper(xx, yy);
			}
		}
		conditionalRedraw();
	}
	
	public String getBackgroundCharacter() {
		return background;
	}
	
	public Color getBackgroundColor() {
		return getBackground();
	}
	
	public String getCharacter(double x, double y) {
		return getCharacter((int) x, (int) y);
	}
	
	public String getCharacter(int x, int y) {
		String key = getKey(x, y);
		if (matrix.containsKey(key)) {
			return matrix.get(key);
		} else {
			return background;
		}
	}
	
	public int getColumns() {
		int width = this.getConsole().getWidth();
		int charWidth = this.getFontMetrics(this.getFont()).stringWidth("M");
		int columns = width / charWidth;
		return columns;
	}
	
	public String getFillCharacter() {
		return fill;
	}
	
	public String getForegroundCharacter() {
		return paint;
	}
	
	public Color getForegroundColor() {
		return getForeground();
	}
	
	private String getKey(int x, int y) {
		return x + "," + y;
	}
	
	public int getRows() {
		int height = this.getConsole().getHeight();
		int charHeight = this.getFontMetrics(this.getFont()).getHeight();
		int rows = height / charHeight;
		return rows;
	}
	
	public void redraw() {
		sConsole.clear();
		// clearConsole();
		int maxRow = Math.min(maxY, getRows() - 1);
		int maxCol = Math.min(maxX, getColumns() - 1);
		StringBuilder sb = new StringBuilder((maxRow + 2) * (maxCol + 2));
		for (int r = 0; r <= maxRow; r++) {
			for (int c = 0; c <= maxCol; c++) {
				String ch = getCharacter(c, r);
				sb.append(ch);
			}
			if (r != maxRow) {
				sb.append('\n');
			}
		}
		String screen = sb.toString();
		// print(screen);
		sConsole.getTextPane().setText(screen);
	}
	
	public void setBackgroundCharacter(char character) {
		setBackgroundCharacter(String.valueOf(character));
	}
	
	public void setBackgroundCharacter(String character) {
		if (character != null && !character.isEmpty()) {
			background = character.substring(0, 1);
			repaint();
		}
	}
	
	public void setBackgroundColor(Color color) {
		setBackground(color);
		this.getConsole().setBackground(color);
	}
	
	public void setForegroundCharacter(char character) {
		setForegroundCharacter(String.valueOf(character));
	}
	
	public void setForegroundCharacter(String character) {
		if (character != null && !character.isEmpty()) {
			paint = character.substring(0, 1);
		}
	}

	public void setFillCharacter(char character) {
		setFillCharacter(String.valueOf(character));
	}
	
	public void setFillCharacter(String character) {
		if (character != null && !character.isEmpty()) {
			fill = character.substring(0, 1);
		}
	}

	public void setForegroundColor(Color color) {
		setForeground(color);
		this.getConsole().setForeground(color);
	}
	
	public void setRedrawAutomatically(boolean redraw) {
		this.redrawAutomatically = redraw;
	}
}
