/*
 * @author Marty Stepp (current maintainer)
 * @version 2016/04/07
 * - added fontEnlarge, fontShrink, fontToggleBold
 * @version 2015/06/19
 * - dummy superclass between Program and ConsoleProgram/JBEDummyProgram
 */

package acm.program;

import java.awt.*;
import javax.swing.*;
import acm.io.*;
import acm.util.JTFTools;

public class AbstractConsoleProgram extends Program {
	private static final int DEFAULT_LINE_HEIGHT = 16;
	
	public void fontEnlarge() {
		fontChangeSize(+2);
	}
	
	public void fontShrink() {
		fontChangeSize(-2);
	}
	
	public void fontToggleBold() {
		Font font = fontExtract();
		if (font != null) {
			if ((font.getStyle() & Font.BOLD) != 0) {
				font = font.deriveFont(font.getStyle() & ~Font.BOLD);
			} else {
				font = font.deriveFont(font.getStyle() | Font.BOLD);
			}
			this.setFont(font);
		}
	}
	
	private void fontChangeSize(int increment) {
		Font font = fontExtract();
		if (font != null) {
			font = font.deriveFont((float) font.getSize() + increment);
			this.setFont(font);
		}
	}
	
	private Font fontExtract() {
		Font font = this.getFont();
		if (font == null) {
			font = this.getConsole().getFont();
		}
		if (font == null) {
			font = JTFTools.decodeFont(null);
		}
		return font;
	}
	
	private JScrollPane getScrollPane() {
		IOConsole console = getConsole();
		StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
		return model.getScrollPane();
	}
	
	public void historyDown() {
		IOConsole console = getConsole();
		StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
		model.historyDown();
	}
	
	public void historyUp() {
		IOConsole console = getConsole();
		StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
		model.historyUp();
	}
	
	private int scrollPageHeight() {
		JScrollPane scroll = getScrollPane();
		if (scroll != null && scroll.getVerticalScrollBar() != null) {
			return scroll.getHeight();
		} else {
			return 0;
		}
	}
	
	private int scrollLineHeight() {
		Font programFont = getFont();
		if (programFont == null) {
			return DEFAULT_LINE_HEIGHT;
		} else {
			FontMetrics fm = getFontMetrics(programFont);
			return fm.getHeight();
		}
	}
	
	private void scrollBy(int dy) {
		JScrollPane scroll = getScrollPane();
		if (scroll == null) {
			return;
		}
		JScrollBar bar = scroll.getVerticalScrollBar();
		if (bar == null) {
			return;
		}
		
		int y;
		int min = scroll.getVerticalScrollBar().getMinimum();
		int max = scroll.getVerticalScrollBar().getMaximum();
		if (dy == Integer.MIN_VALUE) {
			y = min;
		} else if (dy == Integer.MAX_VALUE) {
			y = max;
		} else {
			// shift y by dy, bounded between [min..max] inclusive
			y = bar.getValue() + dy;
			y = Math.max(min, y);
			y = Math.min(max, y);
		}
		bar.setValue(y);
	}
	
	public void scrollToTop() {
		scrollBy(Integer.MIN_VALUE);
	}
	
	public void scrollToBottom() {
		scrollBy(Integer.MAX_VALUE);
	}
	
	public void scrollPageUp() {
		scrollBy(-scrollPageHeight());
	}
	
	public void scrollPageDown() {
		scrollBy(scrollPageHeight());
	}
	
	public void scrollLineUp() {
		scrollBy(-scrollLineHeight());
	}
	
	public void scrollLineDown() {
		scrollBy(scrollLineHeight());
	}
}
