// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import acm.util.JTFTools;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

// Referenced classes of package acm.program:
//            Program

class ProgramStartupListener
    implements ComponentListener
{

    ProgramStartupListener()
    {
    }

    public synchronized void waitForStartup(Program program)
    {
        JTFTools.pause(1000D);
        while(!program.isStarted()) 
            try
            {
                wait(300L);
            }
            catch(InterruptedException interruptedexception) { }
    }

    public void componentHidden(ComponentEvent componentevent)
    {
    }

    public void componentMoved(ComponentEvent componentevent)
    {
    }

    public void componentResized(ComponentEvent componentevent)
    {
        componentShown(componentevent);
    }

    public synchronized void componentShown(ComponentEvent componentevent)
    {
        notifyAll();
    }

    private static final int STARTUP_DELAY = 1000;
    private static final int STARTUP_CYCLE = 300;
}
