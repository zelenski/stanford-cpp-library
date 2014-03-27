// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;


// Referenced classes of package acm.io:
//            StandardConsoleModel

class ConsoleOutputMonitor
{

    public ConsoleOutputMonitor(StandardConsoleModel standardconsolemodel)
    {
        consoleModel = standardconsolemodel;
    }

    public synchronized void print(String s, int i)
    {
        consoleModel.printCallback(s, i);
    }

    private StandardConsoleModel consoleModel;
}
