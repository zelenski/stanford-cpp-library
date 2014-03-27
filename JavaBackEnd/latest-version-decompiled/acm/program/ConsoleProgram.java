// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   ConsoleProgram.java

package acm.program;

import acm.io.IOConsole;
import acm.util.JTFTools;
import java.awt.Font;

// Referenced classes of package acm.program:
//            Program

public abstract class ConsoleProgram extends Program
{

    public ConsoleProgram()
    {
        add(getConsole(), "Center");
        validate();
    }

    public void run()
    {
    }

    public void init()
    {
    }

    public void setFont(String s)
    {
        IOConsole ioconsole = getConsole();
        if(ioconsole != null)
        {
            ioconsole.setFont(s);
            super.setFont(ioconsole.getFont());
        }
    }

    public void setFont(Font font)
    {
        IOConsole ioconsole = getConsole();
        font = JTFTools.getStandardFont(font);
        if(ioconsole != null)
            ioconsole.setFont(font);
        super.setFont(font);
    }

    protected IOConsole createConsole()
    {
        return new IOConsole();
    }
}
