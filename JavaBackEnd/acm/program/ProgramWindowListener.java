// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import java.awt.Component;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;

// Referenced classes of package acm.program:
//            Program

class ProgramWindowListener
    implements WindowListener
{

    public ProgramWindowListener(Program program1)
    {
        program = program1;
    }

    public void windowClosing(WindowEvent windowevent)
    {
        ((Component)windowevent.getSource()).setVisible(false);
        program.exit();
    }

    public void windowOpened(WindowEvent windowevent)
    {
    }

    public void windowClosed(WindowEvent windowevent)
    {
    }

    public void windowIconified(WindowEvent windowevent)
    {
    }

    public void windowDeiconified(WindowEvent windowevent)
    {
    }

    public void windowActivated(WindowEvent windowevent)
    {
    }

    public void windowDeactivated(WindowEvent windowevent)
    {
    }

    private Program program;
}
