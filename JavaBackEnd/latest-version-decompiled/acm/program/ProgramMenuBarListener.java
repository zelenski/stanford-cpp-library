// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   ProgramMenuBar.java

package acm.program;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

// Referenced classes of package acm.program:
//            ProgramMenuBar

class ProgramMenuBarListener
    implements ActionListener
{

    public ProgramMenuBarListener(ProgramMenuBar programmenubar)
    {
        menuBar = programmenubar;
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        menuBar.fireActionListeners(actionevent);
    }

    private ProgramMenuBar menuBar;
}
