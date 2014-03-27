// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import acm.program.ProgramMenuBar;
import java.awt.*;
import java.io.*;

// Referenced classes of package acm.io:
//            ConsoleModel, IOConsole

class SystemConsoleModel
    implements ConsoleModel
{

    public SystemConsoleModel()
    {
        text = "";
    }

    public void setConsole(IOConsole ioconsole)
    {
        console = ioconsole;
    }

    public IOConsole getConsole()
    {
        return console;
    }

    public void clear()
    {
    }

    public void print(String s, int i)
    {
        System.out.print(s);
        new StringBuilder();
        this;
        JVM INSTR dup_x1 ;
        text;
        append();
        s;
        append();
        toString();
        text;
    }

    public String readLine()
    {
        System.out.flush();
        String s = "";
        do
        {
            try
            {
                int i;
                if(inputScript == null)
                    i = System.in.read();
                else
                    i = inputScript.read();
                if(i == -1 && s.length() == 0)
                {
                    try
                    {
                        if(inputScript != null)
                            inputScript.close();
                    }
                    catch(IOException ioexception) { }
                    inputScript = null;
                    continue;
                }
                if(i == -1 || i == 10)
                    break;
                s = (new StringBuilder()).append(s).append((char)i).toString();
                continue;
            }
            catch(IOException ioexception1) { }
            break;
        } while(true);
        if(inputScript != null)
            print((new StringBuilder()).append(s).append("\n").toString(), 1);
        return s;
    }

    public String getText()
    {
        return text;
    }

    public String getText(int i, int j)
    {
        return text.substring(i, j);
    }

    public int getLength()
    {
        return text.length();
    }

    public Component getConsoleModel()
    {
        return null;
    }

    public Component getTextPane()
    {
        return null;
    }

    public void setFont(Font font)
    {
    }

    public void setInputStyle(int i)
    {
    }

    public void setInputColor(Color color)
    {
    }

    public void setErrorStyle(int i)
    {
    }

    public void setErrorColor(Color color)
    {
    }

    public void cut()
    {
    }

    public void copy()
    {
    }

    public void paste()
    {
    }

    public void selectAll()
    {
    }

    public boolean isPointSelection()
    {
        return true;
    }

    public void print(PrintJob printjob)
    {
    }

    public void setInputScript(BufferedReader bufferedreader)
    {
        inputScript = bufferedreader;
    }

    public BufferedReader getInputScript()
    {
        return inputScript;
    }

    public Component getConsolePane()
    {
        return null;
    }

    public void requestFocus()
    {
    }

    public void setMenuBar(ProgramMenuBar programmenubar)
    {
    }

    private IOConsole console;
    private BufferedReader inputScript;
    private String text;
}
