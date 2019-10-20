/*
 * @author Marty Stepp
 * @version 2018/01/23
 * - fixed bug in getInteger reprompt message (should be "Illegal integer format")
 * @version 2016/10/02
 * - added ability to set reprompt messages for readInt/Double/Boolean
 * @version 2015/05/10
 * - added a space after all readInt/Double/Line prompts
 */

package acm.io;

import acm.program.*;
import acm.util.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.util.*;
import javax.swing.*;

public class IOConsole extends Container implements IOModel {
	private static final long serialVersionUID = 0L;
	
	public static final IOConsole SYSTEM_CONSOLE = new SystemConsole();

	public IOConsole() {
		this.consoleModel = createConsoleModel();
		this.consoleModel.setConsole(this);
		setBackground(Color.WHITE);
		setInputColor(Color.BLUE);
		setInputStyle(1);
		setErrorColor(Color.RED);
		setErrorStyle(1);
		setFont(JTFTools.getStandardFont(DEFAULT_FONT));
		Component localComponent = this.consoleModel.getConsolePane();
		if (localComponent != null) {
			setLayout(new BorderLayout());
			add(localComponent, "Center");
		}
		this.reader = null;
		this.writer = null;
		this.exceptionOnError = false;
	}

	public void clear() {
		this.consoleModel.clear();
	}
	
	public ConsoleModel getConsoleModel() {
		return this.consoleModel;
	}

	public void print(String paramString) {
		getWriter().print(paramString);
		if (echo) {
			System.out.print(paramString);
			System.out.flush();
		}
	}

	public final void print(boolean paramBoolean) {
		print("" + paramBoolean);
	}

	public final void print(char paramChar) {
		print("" + paramChar);
	}

	public final void print(double paramDouble) {
		print("" + paramDouble);
	}

	public final void print(float paramFloat) {
		print("" + paramFloat);
	}

	public final void print(int paramInt) {
		print("" + paramInt);
	}

	public final void print(long paramLong) {
		print("" + paramLong);
	}

	public final void print(Object paramObject) {
		print("" + paramObject);
	}

	public void println() {
		getWriter().println();
		if (echo) {
			System.out.println();
			System.out.flush();
		}
	}

	public void println(String paramString) {
		getWriter().println(paramString);
		if (echo) {
			System.out.println(paramString);
			System.out.flush();
		}
	}

	public final void println(boolean paramBoolean) {
		println("" + paramBoolean);
	}

	public final void println(char paramChar) {
		println("" + paramChar);
	}

	public final void println(double paramDouble) {
		println("" + paramDouble);
	}

	public final void println(float paramFloat) {
		println("" + paramFloat);
	}

	public final void println(int paramInt) {
		println("" + paramInt);
	}

	public final void println(long paramLong) {
		println("" + paramLong);
	}

	public final void println(Object paramObject) {
		println("" + paramObject);
	}

	public void showErrorMessage(String paramString) {
		this.consoleModel.print(paramString, 2);
		this.consoleModel.print("\n", 2);
	}

	public final String readLine() {
		return readLine(null);
	}

	public String readLine(String paramString) {
		if (paramString != null) {
			paramString = appendSpace(paramString);   // added by Marty 2015/05/10
			print(paramString);
		}
		this.consoleModel.requestFocus();
		try {
			String line = getReader().readLine();
			if (echo) {
				System.out.println(line);
				System.out.flush();
			}
			return line;
		} catch (IOException localIOException) {
			throw new ErrorException(localIOException);
		}
	}

	public final int readInt() {
		return readInt(null, Integer.MIN_VALUE, Integer.MAX_VALUE);
	}

	public final int readInt(int paramInt1, int paramInt2) {
		return readInt(null, paramInt1, paramInt2);
	}

	public final int readInt(String paramString) {
		return readInt(paramString, Integer.MIN_VALUE, Integer.MAX_VALUE);
	}

	public int readInt(String paramString, int paramInt1, int paramInt2) {
		String str1 = null;
		for (;;) {
			String str2 = readLine(paramString);
			try {
				int i = Integer.parseInt(str2);
				if ((i >= paramInt1) && (i <= paramInt2)) {
					return i;
				}
				str1 = "Value is outside the range [" + paramInt1 + ":"
						+ paramInt2 + "]";
			} catch (NumberFormatException localNumberFormatException) {
				str1 = GETINTEGER_REPROMPT_MESSAGE;
			}
			showErrorMessage(str1);
			if (paramString == null) {
				paramString = GETINTEGER_DEFAULT_PROMPT;
			}
		}
	}
	
	protected void setReadIntDefaultPrompt(String prompt) {
		GETINTEGER_DEFAULT_PROMPT = prompt;
	}

	protected void setReadIntRepromptMessage(String message) {
		GETINTEGER_REPROMPT_MESSAGE = message;
	}

	public final double readDouble() {
		return readDouble(null, Double.NEGATIVE_INFINITY,
				Double.POSITIVE_INFINITY);
	}

	public final double readDouble(double paramDouble1, double paramDouble2) {
		return readDouble(null, paramDouble1, paramDouble2);
	}

	public final double readDouble(String paramString) {
		return readDouble(paramString, Double.NEGATIVE_INFINITY,
				Double.POSITIVE_INFINITY);
	}

	public double readDouble(String paramString, double paramDouble1,
			double paramDouble2) {
		String str1 = null;
		for (;;) {
			String str2 = readLine(paramString);
			try {
				double d = Double.valueOf(str2).doubleValue();
				if ((d >= paramDouble1) && (d <= paramDouble2)) {
					return d;
				}
				str1 = "Value is outside the range [" + paramDouble1 + ":"
						+ paramDouble2 + "]";
			} catch (NumberFormatException localNumberFormatException) {
				str1 = GETREAL_REPROMPT_MESSAGE;
			}
			showErrorMessage(str1);
			if (paramString == null) {
				paramString = GETREAL_DEFAULT_PROMPT;
			}
		}
	}
	
	public void setEcho(boolean echo) {
		this.echo = echo;
	}

	protected void setReadDoubleDefaultPrompt(String prompt) {
		GETREAL_DEFAULT_PROMPT = prompt;
	}

	protected void setReadDoubleRepromptMessage(String message) {
		GETREAL_REPROMPT_MESSAGE = message;
	}

	public final boolean readBoolean() {
		return readBoolean(null);
	}

	public final boolean readBoolean(String paramString) {
		return readBoolean(paramString, "true", "false");
	}

	public boolean readBoolean(String paramString1, String paramString2,
			String paramString3) {
		for (;;) {
			String str = readLine(paramString1);
			if (str == null) {
				throw new ErrorException("End of file encountered");
			}
			if (str.equalsIgnoreCase(paramString2)) {
				return true;
			}
			if (str.equalsIgnoreCase(paramString3)) {
				return false;
			}
			if (this.exceptionOnError) {
				throw new ErrorException(GETBOOL_REPROMPT_MESSAGE);
			}
			showErrorMessage(GETBOOL_REPROMPT_MESSAGE);
			if (paramString1 == null) {
				paramString1 = "Retry: ";
			}
		}
	}
	
	protected void setReadBooleanRepromptMessage(String message) {
		GETBOOL_REPROMPT_MESSAGE = message;
	}
	
	protected String appendSpace(String s) {
		s = String.valueOf(s);
		if (!s.endsWith(" ")) {
			s += " ";
		}
		return s;
	}

	public BufferedReader getReader() {
		if (this.reader == null) {
			this.reader = new BufferedReader(new ConsoleReader(
					this.consoleModel));
		}
		return this.reader;
	}

	public PrintWriter getWriter() {
		if (this.writer == null) {
			this.writer = new PrintWriter(new ConsoleWriter(this.consoleModel));
		}
		return this.writer;
	}

	public void setExceptionOnError(boolean paramBoolean) {
		this.exceptionOnError = paramBoolean;
	}

	public boolean getExceptionOnError() {
		return this.exceptionOnError;
	}

	public void setInputStyle(int paramInt) {
		this.inputStyle = paramInt;
		this.consoleModel.setInputStyle(paramInt);
	}

	public int getInputStyle() {
		return this.inputStyle;
	}

	public void setInputColor(Color paramColor) {
		this.inputColor = paramColor;
		this.consoleModel.setInputColor(paramColor);
	}

	public Color getInputColor() {
		return this.inputColor;
	}

	public void setErrorStyle(int paramInt) {
		this.errorStyle = paramInt;
		this.consoleModel.setErrorStyle(paramInt);
	}

	public int getErrorStyle() {
		return this.errorStyle;
	}

	public void setErrorColor(Color paramColor) {
		this.errorColor = paramColor;
		this.consoleModel.setErrorColor(paramColor);
	}

	public Color getErrorColor() {
		return this.errorColor;
	}

	public void setFont(String paramString) {
		setFont(JTFTools.decodeFont(paramString, getFont()));
	}

	public void setInputScript(BufferedReader paramBufferedReader) {
		this.consoleModel.setInputScript(paramBufferedReader);
	}

	public BufferedReader getInputScript() {
		return this.consoleModel.getInputScript();
	}

	public void cut() {
		this.consoleModel.cut();
	}

	public void copy() {
		this.consoleModel.copy();
	}

	public void paste() {
		this.consoleModel.paste();
	}

	public void selectAll() {
		this.consoleModel.selectAll();
	}

	public void save() {
		FileWriter localFileWriter = null;
		while (localFileWriter == null) {
			try {
				if (this.file == null) {
					Frame localFrame = JTFTools.getEnclosingFrame(this);
					if (localFrame == null) {
						return;
					}
					String localObject = System.getProperty("user.dir");
					FileDialog localFileDialog = new FileDialog(localFrame,
							"Save Console As", 1);
					localFileDialog.setDirectory(localObject);
					localFileDialog.setVisible(true);
					String str = localFileDialog.getFile();
					if (str == null) {
						return;
					}
					this.file = new File(localFileDialog.getDirectory(), str);
				}
				localFileWriter = new FileWriter(this.file);
				save(localFileWriter);
				localFileWriter.close();
				Platform.setFileTypeAndCreator(this.file, "TEXT", "ttxt");
			} catch (IOException localIOException) {
				Object localObject = new IODialog(this);
				((IODialog) localObject).showErrorMessage(localIOException
						.getMessage());
			}
		}
	}

	public void saveAs() {
		this.file = null;
		save();
	}

	public void save(Writer paramWriter) {
		try {
			paramWriter.write(this.consoleModel.getText());
		} catch (IOException localIOException) {
			throw new ErrorException(localIOException);
		}
	}

	public void printConsole() {
		Frame localFrame = JTFTools.getEnclosingFrame(this);
		if (localFrame == null) {
			return;
		}
		PrintJob localPrintJob = getToolkit().getPrintJob(localFrame,
				"Console", null);
		if (localPrintJob == null) {
			return;
		}
		printConsole(localPrintJob);
		localPrintJob.end();
	}

	public void printConsole(PrintJob paramPrintJob) {
		this.consoleModel.print(paramPrintJob);
	}

	public void script() {
		Frame localFrame = JTFTools.getEnclosingFrame(this);
		FileDialog localFileDialog = new FileDialog(localFrame, "Input Script",
				0);
		localFileDialog.setDirectory(System.getProperty("user.dir"));
		localFileDialog.setVisible(true);
		String str1 = localFileDialog.getDirectory();
		String str2 = localFileDialog.getFile();
		if (str2 != null) {
			try {
				FileReader localFileReader = new FileReader(new File(new File(
						str1), str2));
				setInputScript(new BufferedReader(localFileReader));
			} catch (IOException localIOException) {
				throw new ErrorException(localIOException);
			}
		}
	}

	public void setMenuBar(ProgramMenuBar paramProgramMenuBar) {
		this.menuBar = paramProgramMenuBar;
		this.consoleModel.setMenuBar(paramProgramMenuBar);
	}

	public ProgramMenuBar getMenuBar() {
		return this.menuBar;
	}

	public boolean menuAction(ActionEvent paramActionEvent) {
		String str = paramActionEvent.getActionCommand();
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_CUT)) {
			cut();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_COPY)) {
			copy();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_PASTE)) {
			paste();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_SELECT_ALL)) {
			selectAll();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_SAVE)) {
			save();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_SAVE_AS)) {
			saveAs();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_SCRIPT)) {
			script();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_PRINT_CONSOLE)) {
			printConsole();
			return true;
		}
		if (str.equals(ProgramMenuBar.MENU_ITEM_TEXT_CLEAR_CONSOLE)) {
			clear();
			return true;
		}
		return false;
	}

	public boolean isConsoleMenuItem(JMenuItem paramJMenuItem) {
		String str = paramJMenuItem.getActionCommand();
		if (str == null) {
			return false;
		}
		for (int i = 0; i < CONSOLE_MENU_ACTIONS.length; i++) {
			if (str.equals(CONSOLE_MENU_ACTIONS[i])) {
				return true;
			}
		}
		return false;
	}

	public void updateMenuBar(ProgramMenuBar paramProgramMenuBar) {
		Iterator<JMenuItem> localIterator = paramProgramMenuBar.iterator();
		while (localIterator.hasNext()) {
			JMenuItem localJMenuItem = localIterator.next();
			if (isConsoleMenuItem(localJMenuItem)) {
				localJMenuItem.setEnabled(true);
			} else {
				localJMenuItem.setEnabled(!paramProgramMenuBar
						.isFocusedItem(localJMenuItem));
			}
		}
	}

	public void setBackground(Color paramColor) {
		Component localComponent = this.consoleModel.getTextPane();
		if (localComponent != null) {
			localComponent.setBackground(paramColor);
		}
		super.setBackground(paramColor);
	}

	public void setForeground(Color paramColor) {
		Component localComponent = this.consoleModel.getTextPane();
		if (localComponent != null) {
			localComponent.setForeground(paramColor);
		}
		super.setForeground(paramColor);
	}

	public void setFont(Font paramFont) {
		paramFont = JTFTools.getStandardFont(paramFont);
		Component localComponent = this.consoleModel.getTextPane();
		if (localComponent != null) {
			localComponent.setFont(paramFont);
		}
		super.setFont(paramFont);
	}

	public void requestFocus() {
		this.consoleModel.requestFocus();
	}

	public Dimension getPreferredSize() {
		return getMinimumSize();
	}

	public Dimension getMinimumSize() {
		return new Dimension(50, 40);
	}

	protected ConsoleModel createConsoleModel() {
		return new StandardConsoleModel();
	}

	protected static final Font DEFAULT_FONT = new Font("Monospaced", 0, 14);
	protected static final String LINE_SEPARATOR = System
			.getProperty("line.separator");
	protected static final int MINIMUM_CONSOLE_WIDTH = 50;
	protected static final int MINIMUM_CONSOLE_HEIGHT = 40;
	private static final String[] CONSOLE_MENU_ACTIONS = { "Save", "Save As",
			"Print Console", "Script", "Cut", "Copy", "Paste", "Select All", "Clear Console" };
	private ConsoleModel consoleModel;
	private boolean exceptionOnError;
	private boolean echo;
	private Color inputColor;
	private int inputStyle;
	private Color errorColor;
	private int errorStyle;
	private BufferedReader reader;
	private PrintWriter writer;
	private File file;
	private ProgramMenuBar menuBar;
	
	private String GETINTEGER_REPROMPT_MESSAGE = "Illegal integer format";
	private String GETINTEGER_DEFAULT_PROMPT = "Retry: ";
	private String GETREAL_REPROMPT_MESSAGE = "Illegal numeric format";
	private String GETREAL_DEFAULT_PROMPT = "Retry: ";
	private String GETBOOL_REPROMPT_MESSAGE = "Illegal boolean format";
}