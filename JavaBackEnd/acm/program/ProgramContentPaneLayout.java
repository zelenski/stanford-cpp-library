// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import java.awt.*;
import java.awt.event.ComponentEvent;

// Referenced classes of package acm.program:
//            Program

class ProgramContentPaneLayout extends BorderLayout
{

    public ProgramContentPaneLayout(Program program)
    {
        myProgram = program;
    }

    public void layoutContainer(Container container)
    {
        super.layoutContainer(container);
        if(!myProgram.isAncestorOf(container))
        {
            Dimension dimension = container.getSize();
            Insets insets = container.getInsets();
            int i = insets.left;
            int j = insets.top;
            int k = dimension.width - insets.left - insets.right;
            int l = dimension.height - insets.top - insets.bottom;
            myProgram.setBounds(i, j, k, l);
            ComponentEvent componentevent = new ComponentEvent(myProgram, 101);
            Toolkit.getDefaultToolkit().getSystemEventQueue().postEvent(componentevent);
        }
    }

    private Program myProgram;
}
