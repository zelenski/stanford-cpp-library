// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import java.io.Writer;

// Referenced classes of package acm.io:
//            IOConsole, ConsoleModel

class ConsoleWriter extends Writer
{

    public ConsoleWriter(ConsoleModel consolemodel)
    {
        consoleModel = consolemodel;
    }

    public void close()
    {
    }

    public void flush()
    {
    }

    public void write(char ac[], int i, int j)
    {
        String s = new String(ac, i, j);
        int k;
        int l;
        for(k = 0; (l = s.indexOf(IOConsole.LINE_SEPARATOR, k)) != -1; k = l + IOConsole.LINE_SEPARATOR.length())
        {
            consoleModel.print(s.substring(k, l), 0);
            consoleModel.print("\n", 0);
        }

        consoleModel.print(s.substring(k), 0);
    }

    private ConsoleModel consoleModel;
}
