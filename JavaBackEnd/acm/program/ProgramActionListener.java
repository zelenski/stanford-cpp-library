// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

// Referenced classes of package acm.program:
//            Program

class ProgramActionListener
    implements ActionListener
{

    public ProgramActionListener(Program program1)
    {
        program = program1;
    }

    public void actionPerformed(ActionEvent actionevent)
    {
        program.menuAction(actionevent);
    }

    private Program program;
}
