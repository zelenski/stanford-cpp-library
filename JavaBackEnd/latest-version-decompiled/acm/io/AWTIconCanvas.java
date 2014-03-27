// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   IODialog.java

package acm.io;

import java.awt.*;

class AWTIconCanvas extends Canvas
{

    public AWTIconCanvas(Image image)
    {
        myIcon = image;
    }

    public Dimension getMinimumSize()
    {
        return new Dimension(48, 48);
    }

    public Dimension getPreferredSize()
    {
        return getMinimumSize();
    }

    public void paint(Graphics g)
    {
        g.drawImage(myIcon, 8, 8, this);
    }

    private Image myIcon;
}
