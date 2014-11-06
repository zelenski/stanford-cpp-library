package acm.io;

import acm.program.ProgramMenuBar;
import acm.util.ErrorException;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.PrintJob;
import java.awt.event.ActionListener;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.BufferedReader;
import java.io.IOException;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.Document;
import javax.swing.text.Element;
import javax.swing.text.ElementIterator;
import javax.swing.text.SimpleAttributeSet;
import javax.swing.text.StyleConstants;

class StandardConsoleModel implements KeyListener, FocusListener, ConsoleModel {
	private static final int PRINT_MARGIN = 36;
	private ActionListener actionListener;
	private ConsoleOutputMonitor outputMonitor;
	private ConsoleInputMonitor inputMonitor;
	private BufferedReader inputScript;
	private CharacterQueue buffer;
	private SimpleAttributeSet outputAttributes;
	private SimpleAttributeSet inputAttributes;
	private SimpleAttributeSet errorAttributes;
	private JScrollPane scrollPane;
	private JTextPane textPane;
	private Document document;
	private String lineSeparator;
	private int base;
	private boolean hasFocus;
	private IOConsole console;
	private ProgramMenuBar menuBar;

	public StandardConsoleModel() {
		this.outputMonitor = new ConsoleOutputMonitor(this);
		this.inputMonitor = new ConsoleInputMonitor(this);
		this.scrollPane = new JScrollPane(22, 32);
		this.textPane = new JTextPane();
		this.textPane.addKeyListener(this);
		this.textPane.addFocusListener(this);
		this.scrollPane.setViewportView(this.textPane);
		this.document = this.textPane.getDocument();
		this.lineSeparator = System.getProperty("line.separator");
		this.outputAttributes = new SimpleAttributeSet();
		this.inputAttributes = new SimpleAttributeSet();
		this.errorAttributes = new SimpleAttributeSet();
		this.buffer = new CharacterQueue();
		this.base = 0;
	}

	public void setConsole(IOConsole paramIOConsole) {
		this.console = paramIOConsole;
	}

	public IOConsole getConsole() {
		return this.console;
	}

	public void print(String paramString, int paramInt) {
		this.outputMonitor.print(paramString, paramInt);
	}

	public String readLine() {
		return this.inputMonitor.readLine();
	}

	public void setInputScript(BufferedReader paramBufferedReader) {
		this.inputScript = paramBufferedReader;
		if (this.buffer.isWaiting()) {
			try {
				String str = this.inputScript.readLine();
				this.buffer.enqueue(str + "\n");
			} catch (IOException localIOException) {
				throw new ErrorException(localIOException);
			}
		}
	}

	public BufferedReader getInputScript() {
		return this.inputScript;
	}

	public void clear() {
		this.textPane.setText("");
		this.base = 0;
		this.buffer.clear();
	}

	public String getText() {
		return this.textPane.getText();
	}

	public String getText(int paramInt1, int paramInt2) {
		try {
			return this.document.getText(paramInt1, paramInt2 - paramInt1);
		} catch (BadLocationException localBadLocationException) {
			throw new ErrorException(localBadLocationException);
		}
	}

	public int getLength() {
		return this.document.getLength();
	}

	public Component getConsolePane() {
		return this.scrollPane;
	}

	public Component getTextPane() {
		return this.textPane;
	}

	public void cut() {
		copy();
		deleteSelection();
	}

	public void copy() {
		this.textPane.copy();
	}

	public void paste() {
		if (this.textPane.getSelectionEnd() != this.document.getLength()) {
			return;
		}
		int i = deleteSelection();
		this.textPane.setSelectionStart(i);
		this.textPane.paste();
		this.textPane.select(this.document.getLength(),
				this.document.getLength());
		if ((this.document instanceof DefaultStyledDocument)) {
			DefaultStyledDocument localDefaultStyledDocument = (DefaultStyledDocument) this.document;
			localDefaultStyledDocument.setCharacterAttributes(i,
					this.textPane.getSelectionEnd() - i, this.inputAttributes,
					true);
		}
	}

	public void selectAll() {
		this.textPane.selectAll();
	}

	public boolean isPointSelection() {
		return this.textPane.getSelectionStart() == this.textPane
				.getSelectionEnd();
	}

	public void print(PrintJob paramPrintJob) {
		Graphics localGraphics = paramPrintJob.getGraphics();
		Dimension localDimension = paramPrintJob.getPageDimension();
		FontMetrics localFontMetrics = this.textPane
				.getFontMetrics(this.textPane.getFont());
		int i = localFontMetrics.getHeight();
		int j = localFontMetrics.getAscent();
		int k = PRINT_MARGIN;
		int m = PRINT_MARGIN + j;
		int n = (localDimension.height - 72) / i;
		int i1 = n;
		ElementIterator localElementIterator = new ElementIterator(
				this.document);
		for (;;) {
			Element localElement = localElementIterator.next();
			if (localElement == null) {
				break;
			}
			if (localElement.isLeaf()) {
				try {
					int i2 = localElement.getEndOffset()
							- localElement.getStartOffset();
					setStyleFromAttributes(localGraphics,
							localElement.getAttributes());
					localFontMetrics = localGraphics.getFontMetrics();
					String str1 = this.document.getText(
							localElement.getStartOffset(), i2);
					int i3 = 0;
					for (;;) {
						int i4 = 0;
						int i5 = 1;
						int i6 = str1.indexOf("\n", i3);
						if (i6 == -1) {
							i6 = str1.indexOf(this.lineSeparator, i3);
							if (i6 == -1) {
								i6 = str1.length();
								i4 = 1;
							} else {
								i5 = this.lineSeparator.length();
							}
						}
						if (i3 < i6) {
							String str2 = str1.substring(i3, i6);
							localGraphics.drawString(str2, k, m);
							k += localFontMetrics.stringWidth(str2);
						}
						if (i4 != 0) {
							break;
						}
						i3 = i6 + i5;
						k = PRINT_MARGIN;
						m += i;
						i1--;
						if (i1 <= 0) {
							localGraphics.dispose();
							localGraphics = paramPrintJob.getGraphics();
							m = PRINT_MARGIN + j;
							i1 = n;
						}
					}
				} catch (BadLocationException localBadLocationException) {
					throw new ErrorException(localBadLocationException);
				}
			}
		}
		paramPrintJob.end();
	}

	public void setInputStyle(int paramInt) {
		if (getLength() != 0) {
			throw new ErrorException(
					"Console styles and colors cannot be changed after I/O has started.");
		}
		this.inputAttributes.addAttribute(StyleConstants.Bold, new Boolean(
				(paramInt & 0x1) != 0));
		this.inputAttributes.addAttribute(StyleConstants.Italic, new Boolean(
				(paramInt & 0x2) != 0));
	}

	public void setInputColor(Color paramColor) {
		if (getLength() != 0) {
			throw new ErrorException(
					"Console styles and colors cannot be changed after I/O has started.");
		}
		this.inputAttributes
				.addAttribute(StyleConstants.Foreground, paramColor);
	}

	public void setErrorStyle(int paramInt) {
		if (getLength() != 0) {
			throw new ErrorException(
					"Console styles and colors cannot be changed after I/O has started.");
		}
		this.errorAttributes.addAttribute(StyleConstants.Bold, new Boolean(
				(paramInt & 0x1) != 0));
		this.errorAttributes.addAttribute(StyleConstants.Italic, new Boolean(
				(paramInt & 0x2) != 0));
	}

	public void setErrorColor(Color paramColor) {
		if (getLength() != 0) {
			throw new ErrorException(
					"Console styles and colors cannot be changed after I/O has started.");
		}
		this.errorAttributes
				.addAttribute(StyleConstants.Foreground, paramColor);
	}

	public void requestFocus() {
		if (this.textPane != null) {
			this.textPane.requestFocus();
		}
	}

	public void setMenuBar(ProgramMenuBar paramProgramMenuBar) {
		this.menuBar = paramProgramMenuBar;
	}

	public void focusGained(FocusEvent paramFocusEvent) {
		this.hasFocus = true;
		if (this.menuBar != null) {
			if (this.actionListener == null) {
				this.actionListener = new ConsoleActionListener(this.console);
			}
			this.menuBar.setFocusedListener(this.actionListener);
			this.console.updateMenuBar(this.menuBar);
		}
	}

	public void focusLost(FocusEvent paramFocusEvent) {
		this.hasFocus = false;
		if (this.menuBar != null) {
			this.menuBar.setFocusedListener(null);
		}
	}

	public void keyTyped(KeyEvent paramKeyEvent) {
		if ((!paramKeyEvent.isMetaDown()) && (!paramKeyEvent.isControlDown())) {
			this.buffer.enqueue(paramKeyEvent.getKeyChar());
			paramKeyEvent.consume();
		}
	}

	public void keyPressed(KeyEvent paramKeyEvent) {
		switch (paramKeyEvent.getKeyCode()) {
		case 37:
			this.buffer.enqueue('\002');
			break;
		case 39:
			this.buffer.enqueue('\006');
		}
		if (this.menuBar != null) {
			this.menuBar.fireAccelerator(paramKeyEvent);
		}
		paramKeyEvent.consume();
	}

	public void keyReleased(KeyEvent paramKeyEvent) {
		paramKeyEvent.consume();
	}

	protected void printCallback(String paramString, int paramInt) {
		insert(paramString, this.base, paramInt);
		this.base += paramString.length();
		setCaretPosition(this.base);
	}

	protected String readLineCallback() {
		this.base = getLength();
		if (this.inputScript != null) {
			String str1 = null;
			try {
				str1 = this.inputScript.readLine();
			} catch (IOException localIOException1) {
				throw new ErrorException(localIOException1);
			}
			if (str1 != null) {
				insert(str1, this.base, 1);
				insert("\n", this.base + str1.length(), 0);
				this.base += str1.length() + 1;
				return str1;
			}
			try {
				this.inputScript.close();
			} catch (IOException localIOException2) {
			}
			this.inputScript = null;
		}
		setCaretPosition(this.base);
		char c;
		while (((c = this.buffer.dequeue()) != '\n') && (c != '\r')) {
			if (getCaretPosition() < this.base) {
				setCaretPosition(getLength());
			}
			int i = getSelectionStart();
			switch (c) {
			case '\b':
			case '':
				if (i == getSelectionEnd()) {
					if (i > this.base) {
						delete(i - 1, i);
						i--;
					}
				} else {
					i = deleteSelection();
				}
				break;
			case '\001':
				selectAll();
				i = -1;
				break;
			case '\002':
				i = Math.max(getSelectionStart() - 1, this.base);
				break;
			case '\003':
				copy();
				i = -1;
				break;
			case '\006':
				i = Math.min(getSelectionEnd() + 1, getLength());
				break;
			case '\020':
				this.console.printConsole();
				i = -1;
				break;
			case '\023':
				this.console.save();
				i = -1;
				break;
			case '\026':
				paste();
				i = -1;
				break;
			case '\030':
				cut();
				i = -1;
				break;
			default:
				if (i != getSelectionEnd()) {
					i = deleteSelection();
				}
				insert("" + c, i, 1);
				i++;
			}
			if (i != -1) {
				select(i, i);
				setCaretPosition(i);
			}
		}
		int j = getLength() - this.base;
		String str2 = getText(this.base, this.base + j);
		insert("\n", this.base + j, 0);
		this.base += j + 1;
		return str2;
	}

	public boolean isCommandEnabled(String paramString) {
		return this.hasFocus;
	}

	private void insert(String paramString, int paramInt1, int paramInt2) {
		try {
			SimpleAttributeSet localSimpleAttributeSet = this.outputAttributes;
			switch (paramInt2) {
			case 1:
				localSimpleAttributeSet = this.inputAttributes;
				break;
			case 2:
				localSimpleAttributeSet = this.errorAttributes;
			}
			this.document.insertString(paramInt1, paramString,
					localSimpleAttributeSet);
		} catch (BadLocationException localBadLocationException) {
		}
	}

	private void delete(int paramInt1, int paramInt2) {
		try {
			this.document.remove(paramInt1, paramInt2 - paramInt1);
		} catch (BadLocationException localBadLocationException) {
			throw new ErrorException(localBadLocationException);
		}
	}

	private void setCaretPosition(int paramInt) {
		this.textPane.setCaretPosition(paramInt);
	}

	private int getCaretPosition() {
		return this.textPane.getCaretPosition();
	}

	private void select(int paramInt1, int paramInt2) {
		this.textPane.select(paramInt1, paramInt2);
	}

	private int getSelectionStart() {
		return this.textPane.getSelectionStart();
	}

	private int getSelectionEnd() {
		return this.textPane.getSelectionEnd();
	}

	private int deleteSelection() {
		int i = Math.max(this.base, getSelectionStart());
		int j = getSelectionEnd();
		if (j <= this.base) {
			return getLength();
		}
		delete(i, j);
		return i;
	}

	private void setStyleFromAttributes(Graphics paramGraphics,
			AttributeSet paramAttributeSet) {
		Font localFont = this.textPane.getFont();
		int i = 0;
		if (Boolean.TRUE.equals(paramAttributeSet
				.getAttribute(StyleConstants.Bold))) {
			i |= 0x1;
		}
		if (Boolean.TRUE.equals(paramAttributeSet
				.getAttribute(StyleConstants.Italic))) {
			i |= 0x2;
		}
		paramGraphics.setFont(new Font(localFont.getName(), i, localFont
				.getSize()));
		Color localColor = (Color) paramAttributeSet
				.getAttribute(StyleConstants.Foreground);
		if (localColor == null) {
			localColor = this.textPane.getForeground();
		}
		paramGraphics.setColor(localColor);
	}
}
