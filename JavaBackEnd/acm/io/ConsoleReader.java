// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import java.io.Reader;

// Referenced classes of package acm.io:
//            ConsoleModel

class ConsoleReader extends Reader {

	public ConsoleReader(ConsoleModel consolemodel) {
		consoleModel = consolemodel;
		buffer = null;
	}

	public void close() {
	}

	public int read(char ac[], int i, int j) {
		if (j == 0)
			return 0;
		if (buffer == null) {
			buffer = consoleModel.readLine();
			if (buffer == null)
				return -1;
			buffer += "\n";
		} else {
			if (j < buffer.length()) {
				buffer.getChars(0, j, ac, i);
				buffer = buffer.substring(j);
			} else {
				j = buffer.length();
				buffer.getChars(0, j, ac, i);
				buffer = null;
			}
		}
		return j;
	}

	private ConsoleModel consoleModel;
	private String buffer;
}
