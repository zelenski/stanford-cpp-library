/*
 * @author Marty Stepp (current maintainer)
 * @version 2016/05/17
 * - moved options menu stuff here from ConsoleProgram
 * - added Set Font, Set Back/Foreground Color menu items
 * @version 2016/04/07
 * - added fontEnlarge, fontShrink, fontToggleBold
 * @version 2015/06/19
 * - dummy superclass between Program and ConsoleProgram/JBEDummyProgram
 */

package acm.program;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.io.File;
import java.util.Properties;
import javax.swing.*;

import acm.gui.JFontChooser;
import acm.io.*;
import acm.util.JTFTools;
import stanford.cs106.diff.DiffGui;
import stanford.cs106.gui.GuiUtils;
import stanford.cs106.io.IOUtils;
import stanford.cs106.util.SystemProperties;

public abstract class AbstractConsoleProgram extends Program {
	private static final int DEFAULT_LINE_HEIGHT = 16;
	
	private boolean fontHasBeenSet = false;
	private boolean backgroundHasBeenSet = false;
	private boolean foregroundHasBeenSet = false;

	/* Method: setFont(str) */
	/**
	 * Sets the font used for the console as specified by the string
	 * <code>str</code>, which is interpreted in the style of
	 * <code>Font.decode</code>. The usual format of the font string is
	 *
	 * <p>
	 * <i>family</i><code>-</code><i>style</i><code>-</code><i>size</i>
	 * <p>
	 *
	 * where both <i>style</i> and <i>size</i> are optional. If any of these
	 * parts are specified as an asterisk, the existing value is retained.
	 *
	 * @usage program.setFont(str);
	 * @param str
	 *            A <code>String</code> specifying the new font
	 */
	public void setFont(String str) {
		IOConsole console = getConsole();
		if (console != null) {
			console.setFont(str);
			super.setFont(console.getFont());
		}
	}

	/* Override method: setFont(font) */
	/**
	 * Sets the font for the console.
	 *
	 * @usage program.setFont(font);
	 * @param font
	 *            The new font
	 */
	public void setFont(Font font) {
		IOConsole console = getConsole();
		font = JTFTools.getStandardFont(font);
		if (console != null)
			console.setFont(font);
		super.setFont(font);
	}

	/**
	 * Returns the font currently used in the console.
	 */
	public Font getFont() {
		Font font = super.getFont();
		if (font == null) {
			font = this.getConsole().getFont();
		}
		if (font == null) {
			font = JTFTools.decodeFont(null);
		}
		return font;
	}

	// looks for some settings that can be supplied in the project info.
	protected void checkCompilerFlags() {
		super.checkCompilerFlags();
		if (SystemProperties.hasSystemProperty(ProgramStartupFlags.SPL_CONSOLE_FONTSIZE)) {
			int size = SystemProperties.getSystemPropertyInt(ProgramStartupFlags.SPL_CONSOLE_FONTSIZE);
			setFont("Monospaced-Bold-" + size);
		}

		if (SystemProperties.hasSystemProperty(ProgramStartupFlags.SPL_CONSOLE_WIDTH)
				&& SystemProperties.hasSystemProperty(ProgramStartupFlags.SPL_CONSOLE_HEIGHT)) {
			int w = SystemProperties.getSystemPropertyInt(ProgramStartupFlags.SPL_CONSOLE_WIDTH);
			int h = SystemProperties.getSystemPropertyInt(ProgramStartupFlags.SPL_CONSOLE_HEIGHT);
			setSize(w, h);
		}

		if (SystemProperties.hasSystemProperty(ProgramStartupFlags.SPL_CONSOLE_X)
				&& SystemProperties.hasSystemProperty(ProgramStartupFlags.SPL_CONSOLE_Y)) {
			int x = SystemProperties.getSystemPropertyInt(ProgramStartupFlags.SPL_CONSOLE_X);
			int y = SystemProperties.getSystemPropertyInt(ProgramStartupFlags.SPL_CONSOLE_Y);
			setLocation(x, y);
		}

		if (SystemProperties.hasSystemProperty(ProgramStartupFlags.SPL_CONSOLE_LOCATION_SAVED)) {
			if (SystemProperties.getSystemPropertyBoolean(ProgramStartupFlags.SPL_CONSOLE_LOCATION_SAVED)) {
				GuiUtils.rememberWindowLocation(getJFrame());
			}
		}
	}

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
	
	@Override
	protected void loadConfiguration(Properties props) {
		if (props.containsKey("background")) {
			Color background = JTFTools.decodeColor(String.valueOf(props.get("background")));
			setBackground(background);
			getConsole().setBackground(background);
			backgroundHasBeenSet = true;
		}
		if (props.containsKey("foreground")) {
			Color foreground = JTFTools.decodeColor(String.valueOf(props.get("foreground")));
			setForeground(foreground);
			getConsole().setForeground(foreground);
			foregroundHasBeenSet = true;
		}
		if (props.containsKey("font")) {
			Font font = JTFTools.decodeFont(String.valueOf(props.get("font")));
			setFont(font);
			fontHasBeenSet = true;
		}
	}
	
	@Override
	protected void saveConfiguration(Properties props) {
		if (fontHasBeenSet) {
			Font font = getFont();
			String fontString = font.getName();
			if (fontString == null) {
				fontString = font.getFamily();
			}
			fontString += "-";
			int style = font.getStyle();
			if (style == Font.PLAIN) {
				fontString += "plain";
			} else if (style == Font.BOLD) {
				fontString += "bold";
			} else if (style == Font.ITALIC) {
				fontString += "italic";
			} else if (style == Font.BOLD + Font.ITALIC) {
				fontString += "bolditalic";
			}
			fontString += "-" + font.getSize();
			
			props.put("font", fontString);
		}
		
		if (backgroundHasBeenSet) {
			String backgroundString = String.format("#%06x",
					getBackground().getRGB());
			props.put("background", backgroundString);
		}
		
		if (foregroundHasBeenSet) {
			String foregroundString = String.format("#%06x",
					getForeground().getRGB());
			props.put("foreground", foregroundString);
		}
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

	/**
	 * Returns all text that has been displayed on this console so far.
	 */
	public String getAllOutput() {
		return this.getConsole().getConsoleModel().getText();
	}

	@Override
	public boolean menuAction(ActionEvent event) {
		String cmd = event.getActionCommand().intern();
		if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_COMPARE_OUTPUT) {
			compareOutput();
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_FONT) {
			setFontFromChooser();
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_BACKGROUND_COLOR) {
			setColorFromChooser(/* background */ true);
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_FOREGROUND_COLOR) {
			setColorFromChooser(/* background */ false);
			return true;
		} else {
			return super.menuAction(event);
		}
	}

	/**
	 * Pops up a JColorChooser to let the user pick a color for the console.
	 */
	protected void setColorFromChooser(boolean background) {
		String title = "Choose " + (background ? "background" : "foreground") + " color";
		Color color = JColorChooser.showDialog(getWindow(), title, getBackground());
		if (color == null) {
			return;
		}
		if (background) {
			setBackground(color);
			getConsole().setBackground(color);
		} else {
			setForeground(color);
			getConsole().setForeground(color);
		}

		// prompt make this the 'default' color for future console windows
		int result = JOptionPane.showConfirmDialog(getWindow(), "Make this the default for future console windows?",
				"Make default?", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
		boolean ok = result == JOptionPane.YES_OPTION;
		if (background) {
			backgroundHasBeenSet = ok;
		} else {
			foregroundHasBeenSet = ok;
		}
		if (ok) {
			saveConfiguration();
		}
	}

	/**
	 * Pops up a JFontChooser to let the user pick a font for the console.
	 */
	protected void setFontFromChooser() {
		JFontChooser choose = new JFontChooser();
		choose.setSelectedFont(getFont());
		if (choose.showDialog(getWindow()) == JFontChooser.OK_OPTION) {
			Font font = choose.getSelectedFont();
			setFont(font);

			// prompt make this the 'default' font for future console windows
			int result = JOptionPane.showConfirmDialog(getWindow(), "Make this the default for future console windows?",
					"Make default?", JOptionPane.YES_NO_OPTION, JOptionPane.QUESTION_MESSAGE);
			boolean ok = result == JOptionPane.YES_OPTION;
			fontHasBeenSet = ok;
			if (ok) {
				saveConfiguration();
			}
		}
	}

	/**
	 * Pops up a file chooser to compare output to some expected output.
	 */
	protected void compareOutput() {
		try {
			// pick working dir for loading expected output files
			File dir = new File(System.getProperty("user.dir"));
			File[] dirsToTry = { new File(dir, "output"), new File(dir, "expected-output"), new File(dir, "res/output"),
					new File(dir, "res/expected-output"), };
			for (File dirToTry : dirsToTry) {
				if (dirToTry.exists()) {
					dir = dirToTry;
					break;
				}
			}

			// let the user browse for a file for expected output
			JFileChooser chooser = new JFileChooser(dir);
			int result = chooser.showOpenDialog(getJFrame());
			if (result == JFileChooser.CANCEL_OPTION) {
				return;
			}
			File selectedFile = chooser.getSelectedFile();
			if (selectedFile == null || !selectedFile.isFile()) {
				return;
			}

			String expectedOutput = IOUtils.readEntireFile(selectedFile);
			String studentOutput = getAllOutput();
			DiffGui diff = new DiffGui("expected output", expectedOutput, "your output", studentOutput,
					/* checkboxes */ false);
			diff.show();
		} catch (Exception e) {
			JOptionPane.showMessageDialog(getJFrame(), "Unable to compare output.", "Error",
					JOptionPane.WARNING_MESSAGE);
		}
	}
}
