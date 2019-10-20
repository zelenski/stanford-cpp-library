/*
 * @author Marty Stepp (current maintainer)
 * @version 2018/11/12
 * - disabled Ctrl/Command mouse wheel font scaling 
 * @version 2017/10/31
 * - added Ctrl-number hotkeys to automatically load input script and compare output
 * @version 2017/06/01
 * - added [completed] when program is done running
 * @version 2016/10/02
 * - added reprompt message options
 * - added getYesOrNo, some other console I/O methods
 * - alphabetized all methods
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
import java.awt.event.*;
import java.io.*;
import java.util.Properties;
import javax.swing.*;
import acm.gui.JFontChooser;
import acm.io.*;
import acm.util.JTFTools;
import stanford.cs106.diff.DiffGui;
import stanford.cs106.gui.GuiUtils;
import stanford.cs106.io.*;
import stanford.cs106.util.*;

public abstract class AbstractConsoleProgram extends Program {
	private static final int DEFAULT_LINE_HEIGHT = 16;
	private static final int FONT_MAX_SIZE = 255;
	private static final int FONT_MIN_SIZE = 4;
	private static final String DEFAULT_REPROMPT_MESSAGE = "Unable to open that file.  Try again.";
	public static final String PROGRAM_COMPLETED_TITLE_SUFFIX = " [completed]";
	
	private boolean backgroundHasBeenSet = false;
	private boolean clearEnabled = true;   // whether clearConsole(); is effectual
	private boolean fontHasBeenSet = false;
	private boolean foregroundHasBeenSet = false;
	private boolean mouseListenersAdded = false;
	
	/*
	 * looks for some settings that can be supplied in the project info.
	 */
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

	/**
	 * Erases any text from the main console.
	 */
	public void clearConsole() {
		if (clearEnabled) {
			getConsole().clear();
		}
	}
	
	/**
	 * Pops up a file chooser to compare output to some expected output.
	 */
	protected void compareOutput() {
		try {
			// pick working dir for loading expected output files
			File dir = IOUtils.getExpectedOutputDirectory();

			// let the user browse for a file for expected output
			JFileChooser chooser = new JFileChooser(dir);
			int result = chooser.showOpenDialog(getJFrame());
			if (result == JFileChooser.CANCEL_OPTION) {
				return;
			}
			File selectedFile = chooser.getSelectedFile();
			compareOutput(selectedFile);
		} catch (Exception e) {
			JOptionPane.showMessageDialog(getJFrame(), "Unable to compare output.\n" + e, "Error",
					JOptionPane.WARNING_MESSAGE);
		}
	}
	
	/**
	 * Compares current console output to contents of the given file.
	 */
	protected void compareOutput(File file) {
		if (file == null || !file.isFile()) {
			return;
		}
		String expectedOutput = IOUtils.readEntireFile(file);
		String studentOutput = getAllOutput();
		DiffGui diff = new DiffGui("expected output", expectedOutput, "your output", studentOutput,
				/* checkboxes */ false);
		diff.show();
	}
	
	/**
	 * Tries to find an expected output file ending with the given number and compare it.
	 * For example, if num is 2, tries to find a file like expected-output-2.txt.
	 */
	protected boolean compareOutput(int num) {
		try {
			File dir = IOUtils.getExpectedOutputDirectory();
			for (File file : dir.listFiles()) {
				String name = file.getName();
				if (name.contains("expected-output") && name.endsWith("-" + num + ".txt")) {
					compareOutput(file);
					return true;
				}
			}
		} catch (Exception e) {
			// empty
		}
		return false;
	}
	
	/**
	 * Called as the program is shutting down.
	 * Puts [terminated] on the console window title.
	 */
	protected void endHook() {
		super.endHook();
		setTitle(getTitle() + PROGRAM_COMPLETED_TITLE_SUFFIX);
	}

	protected void loadInputScript() {
		try {
			// pick working dir for loading expected output files
			File dir = IOUtils.getExpectedInputDirectory();

			// let the user browse for a file for expected output
			JFileChooser chooser = new JFileChooser(dir);
			chooser.setFileFilter(new ExtensionFileFilter("txt"));
			int result = chooser.showOpenDialog(getJFrame());
			if (result == JFileChooser.CANCEL_OPTION) {
				return;
			}
			File selectedFile = chooser.getSelectedFile();
			if (selectedFile == null || !selectedFile.isFile()) {
				return;
			}

			String inputScript = IOUtils.readEntireFile(selectedFile);
			this.getConsole().setInputScript(new BufferedReader(new StringReader(inputScript)));
			
		} catch (Exception e) {
			JOptionPane.showMessageDialog(getJFrame(), "Unable to load input script.\n" + e, "Error",
					JOptionPane.WARNING_MESSAGE);
		}
	}
	
	protected void loadInputScript(File file) {
		if (file == null || !file.isFile()) {
			return;
		}
		String inputScript = IOUtils.readEntireFile(file);
		this.getConsole().setInputScript(new BufferedReader(new StringReader(inputScript)));
	}
	
	protected boolean loadInputScript(int num) {
		File dir = IOUtils.getExpectedOutputDirectory();
		for (File file : dir.listFiles()) {
			String name = file.getName();
			if (name.contains("input") && name.endsWith("-" + num + ".txt")) {
				loadInputScript(file);
				return true;
			}
		}
		return false;
	}

	/*
	 * Changes the console's font toggle by the given amount in pt.
	 */
	private void fontChangeSize(int increment) {
		Font font = fontExtract();
		if (font != null) {
			float newSize = (float) font.getSize() + increment;
			if (newSize >= FONT_MIN_SIZE && font.getSize() <= FONT_MAX_SIZE) {
				font = font.deriveFont(newSize);
				this.setFont(font);
				fontHasBeenSet = true;
				saveConfiguration();
			}
		}
	}

	/**
	 * Makes the console's font larger by 2pt.
	 */
	public void fontEnlarge() {
		fontChangeSize(+2);
	}

	/*
	 * Returns the console's current font.
	 */
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

	/**
	 * Makes the console's font smaller by 2pt.
	 */
	public void fontShrink() {
		fontChangeSize(-2);
	}

	/**
	 * Makes the console's font toggle between bold and non-bold.
	 */
	public void fontToggleBold() {
		Font font = fontExtract();
		if (font != null) {
			if ((font.getStyle() & Font.BOLD) != 0) {
				font = font.deriveFont(font.getStyle() & ~Font.BOLD);
			} else {
				font = font.deriveFont(font.getStyle() | Font.BOLD);
			}
			this.setFont(font);
			fontHasBeenSet = true;
			saveConfiguration();
		}
	}

	/**
	 * Returns all text that has been displayed on this console so far.
	 */
	public String getAllOutput() {
		return this.getConsole().getConsoleModel().getText();
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

	/*
	 * Returns the scroll pane used for scrolling the console's output text.
	 */
	private JScrollPane getScrollPane() {
		IOConsole console = getConsole();
		if (console != null && console.getConsoleModel() instanceof StandardConsoleModel) {
			StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
			return model.getScrollPane();
		} else {
			return null;
		}
	}

	/**
	 * Retrieves the next command in the console input history.
	 */
	public void historyDown() {
		IOConsole console = getConsole();
		if (console != null && console.getConsoleModel() instanceof StandardConsoleModel) {
			StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
			model.historyDown();
		}
	}

	/**
	 * Retrieves the previous command in the console input history.
	 */
	public void historyUp() {
		IOConsole console = getConsole();
		if (console != null && console.getConsoleModel() instanceof StandardConsoleModel) {
			StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
			model.historyUp();
		}
	}
	
	/**
	 * Reads the console's configuration settings, if present.
	 */
	@Override
	protected void loadConfiguration(Properties props) {
		if (!mouseListenersAdded) {
			mouseListenersAdded = true;
			
			// try to listen to mouse wheel scroll events
			addMouseWheelListener(this);
			getConsole().addMouseWheelListener(this);
			IOConsole console = getConsole();
			if (console != null && console.getConsoleModel() instanceof StandardConsoleModel) {
				StandardConsoleModel model = (StandardConsoleModel) console.getConsoleModel();
				if (model.getTextPane() != null) {
					model.getTextPane().addMouseWheelListener(this);
				}
			}
		}
		
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
	
	/**
	 * Responds to menu clicks.
	 */
	@Override
	public boolean menuAction(ActionEvent event) {
		String cmd = event.getActionCommand().intern();
		if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_COMPARE_OUTPUT) {
			compareOutput();
			return true;
		} else if (cmd == ProgramMenuBar.MENU_ITEM_TEXT_LOAD_INPUT_SCRIPT) {
			loadInputScript();
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
	 * Implementation of MouseWheelListener interface.
	 * When you Ctrl-wheel or Command-wheel, the font will grow or shrink.
	 */
	public void mouseWheelMoved(MouseWheelEvent e) {
		// JOptionPane.showMessageDialog(this, "mouseWheelMoved(" + e + ")");
		if (e.isControlDown() || e.isAltDown() || e.isMetaDown() || e.isShiftDown()) {
			int notches = e.getWheelRotation();
			if (notches < 0) {
				// mouse wheel moved up
				// fontEnlarge();
			} else {
				// mouse wheel moved down
				// fontShrink();
			}
		} else {
			// re-dispatch the event so we do not block regular scrolling
			e.getComponent().getParent().dispatchEvent(e);
		}
	}

	/**
	 * Asks the user to type a file name, re-prompting until the user types a
	 * file that exists in the current directory.
	 * The message "Unable to open that file. Try again." is shown every time a reprompt is necessary.
	 * The file's full path is returned as a string.
	 * @param prompt the text to display to the user
	 * @param directory the working directory in which to look for files (e.g. "res/")
	 * @return the file name typed by the user, including any directory prefix, such as "res/input.txt" or "foo.dat"
	 */
	public String promptUserForFile(String prompt) {
		return promptUserForFile(prompt, /* directory */ "");
	}
	
	/**
	 * Asks the user to type a file name, re-prompting until the user types a
	 * file that exists in the given directory.
	 * The message "Unable to open that file. Try again." is shown every time a reprompt is necessary.
	 * The file's full path is returned as a string.
	 * @param prompt the text to display to the user
	 * @param directory the working directory in which to look for files (e.g. "res/")
	 * @return the file name typed by the user, including any directory prefix, such as "res/input.txt" or "foo.dat"
	 */
	public String promptUserForFile(String prompt, String directory) {
		return promptUserForFile(prompt, directory, DEFAULT_REPROMPT_MESSAGE);
	}
	
	/**
	 * Asks the user to type a file name, re-prompting until the user types a
	 * file that exists in the given directory.
	 * The given reprompt message is shown every time a reprompt is necessary.
	 * The file's full path is returned as a string.
	 * @param prompt the text to display to the user
	 * @param directory the working directory in which to look for files (e.g. "res/")
	 * @return the file name typed by the user, including any directory prefix, such as "res/input.txt" or "foo.dat"
	 */
	public String promptUserForFile(String prompt, String directory, String reprompt) {
		String filename = readLine(prompt);
		while (filename.isEmpty() || !fileExists(directory, filename)) {
			getOutputModel().showErrorMessage(reprompt);
			filename = readLine(prompt).trim();
		}
		if (!directory.equals("")) {
			// filename = new File(directory, filename).getAbsolutePath();
			directory = directory.replace("\\", "/");
			if (!directory.endsWith("/")) {
				directory += "/";
			}
		}
		return directory + filename;
	}
	
	/**
	 * Turns on/off the ability to clear the console using clearConsole(); (default true)
	 * @param enabled Whether to enable clearConsole();
	 */
	public void setClearConsoleEnabled(boolean enabled) {
		clearEnabled = enabled;
	}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/**
	 * Writes the console's configuration settings to disk.
	 */
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
	
	/*
	 * Tells the console's output area to scroll itself by the given number of vertical px.
	 * (Negative = up, Positive = down.)
	 */
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

	/*
	 * Returns the height of a line in the scroll area in px.
	 */
	private int scrollLineHeight() {
		Font programFont = getFont();
		if (programFont == null) {
			return DEFAULT_LINE_HEIGHT;
		} else {
			FontMetrics fm = getFontMetrics(programFont);
			return fm.getHeight();
		}
	}

	/**
	 * Tells the console's output area to scroll itself downward by one line.
	 */
	public void scrollLineDown() {
		scrollBy(scrollLineHeight());
	}

	/**
	 * Tells the console's output area to scroll itself upward by one line.
	 */
	public void scrollLineUp() {
		scrollBy(-scrollLineHeight());
	}

	/**
	 * Tells the console's output area to scroll itself downward by one page.
	 */
	public void scrollPageDown() {
		scrollBy(scrollPageHeight());
	}

	/*
	 * Returns the height in px of the scrollable console output area.
	 */
	private int scrollPageHeight() {
		JScrollPane scroll = getScrollPane();
		if (scroll != null && scroll.getVerticalScrollBar() != null) {
			return scroll.getHeight();
		} else {
			return 0;
		}
	}

	/**
	 * Tells the console's output area to scroll itself upward by one page.
	 */
	public void scrollPageUp() {
		scrollBy(-scrollPageHeight());
	}

	/**
	 * Tells the console's output area to scroll itself to the top of the output.
	 */
	public void scrollToTop() {
		scrollBy(Integer.MIN_VALUE);
	}

	/**
	 * Tells the console's output area to scroll itself to the bottom of the output.
	 */
	public void scrollToBottom() {
		scrollBy(Integer.MAX_VALUE);
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
	 * Sets the font for the console.
	 *
	 * @usage program.setFont(font);
	 * @param font
	 *            The new font
	 */
	@Override
	public void setFont(Font font) {
		IOConsole console = getConsole();
		font = JTFTools.getStandardFont(font);
		if (console != null)
			console.setFont(font);
		super.setFont(font);
	}

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
}
