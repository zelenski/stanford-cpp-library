// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import acm.program.ProgramMenuBar;
import java.awt.*;
import java.io.BufferedReader;

// Referenced classes of package acm.io:
//            IOConsole

interface ConsoleModel
{

    public abstract void setConsole(IOConsole ioconsole);

    public abstract IOConsole getConsole();

    public abstract void print(String s, int i);

    public abstract String readLine();

    public abstract void setInputScript(BufferedReader bufferedreader);

    public abstract BufferedReader getInputScript();

    public abstract void clear();

    public abstract String getText();

    public abstract String getText(int i, int j);

    public abstract int getLength();

    public abstract Component getConsolePane();

    public abstract Component getTextPane();

    public abstract void cut();

    public abstract void copy();

    public abstract void paste();

    public abstract void selectAll();

    public abstract boolean isPointSelection();

    public abstract void print(PrintJob printjob);

    public abstract void setInputStyle(int i);

    public abstract void setInputColor(Color color);

    public abstract void setErrorStyle(int i);

    public abstract void setErrorColor(Color color);

    public abstract void requestFocus();

    public abstract void setMenuBar(ProgramMenuBar programmenubar);

    public static final int OUTPUT_STYLE = 0;
    public static final int INPUT_STYLE = 1;
    public static final int ERROR_STYLE = 2;
}
