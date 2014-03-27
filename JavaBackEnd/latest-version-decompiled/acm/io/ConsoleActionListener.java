// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IOConsole.java

package acm.io;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

// Referenced classes of package acm.io:
//            IOConsole

class ConsoleActionListener
    implements ActionListener
{

    public ConsoleActionListener(IOConsole ioconsole)
    {
        console = ioconsole;
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        console.menuAction(actionevent);
    }

    private IOConsole console;
}
