// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   SwingTimer.java

package acm.util;

import java.awt.event.ActionListener;
import javax.swing.Timer;

public class SwingTimer extends Timer
{

    public SwingTimer(int i, ActionListener actionlistener)
    {
        super(i, actionlistener);
    }

    static final long serialVersionUID = 1L;
}
