package acm.io;

import acm.program.*;
import java.awt.*;
import java.io.*;

public class SystemConsoleModel implements ConsoleModel {
	private IOConsole console;
	private BufferedReader inputScript;
	private String text;

	public SystemConsoleModel() {
		this.text = "";
	}

	public void setConsole(IOConsole paramIOConsole) {
		this.console = paramIOConsole;
	}

	public IOConsole getConsole() {
		return this.console;
	}

	public void clear() {
	}

	public void print(String paramString, int paramInt) {
		System.out.print(paramString);
		this.text += paramString;
	}

	public String readLine() {
		System.out.flush();
		String str = "";
		try {
			for (;;) {
				int i;
				if (this.inputScript == null) {
					i = System.in.read();
				} else {
					i = this.inputScript.read();
				}
				if ((i == -1) && (str.length() == 0)) {
					try {
						if (this.inputScript != null) {
							this.inputScript.close();
						}
					} catch (IOException localIOException1) {
					}
					this.inputScript = null;
				} else {
					if ((i == -1) || (i == 10)) {
						break;
					}
					str = str + (char) i;
				}
			}
		} catch (IOException localIOException2) {
		}
		if (this.inputScript != null) {
			print(str + "\n", 1);
		}
		return str;
	}

	public String getText() {
		return this.text;
	}

	public String getText(int paramInt1, int paramInt2) {
		return this.text.substring(paramInt1, paramInt2);
	}

	public int getLength() {
		return this.text.length();
	}

	public Component getConsoleModel() {
		return null;
	}

	public Component getTextPane() {
		return null;
	}

	public void setFont(Font paramFont) {
	}

	public void setInputStyle(int paramInt) {
	}

	public void setInputColor(Color paramColor) {
	}

	public void setErrorStyle(int paramInt) {
	}

	public void setErrorColor(Color paramColor) {
	}

	public void cut() {
	}

	public void copy() {
	}

	public void paste() {
	}

	public void selectAll() {
	}

	public boolean isPointSelection() {
		return true;
	}

	public void print(PrintJob paramPrintJob) {
	}

	public void setInputScript(BufferedReader paramBufferedReader) {
		this.inputScript = paramBufferedReader;
	}

	public BufferedReader getInputScript() {
		return this.inputScript;
	}

	public Component getConsolePane() {
		return null;
	}

	public void requestFocus() {
	}

	public void setMenuBar(ProgramMenuBar paramProgramMenuBar) {
	}
}
