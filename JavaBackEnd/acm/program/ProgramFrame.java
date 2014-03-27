// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   Program.java

package acm.program;

import java.awt.Graphics;
import javax.swing.JFrame;

class ProgramFrame extends JFrame
{

    public ProgramFrame(String s)
    {
        super(s);
    }

    public void update(Graphics g)
    {
        paint(g);
    }
}
