// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   JTFTools.java

package acm.util;

import java.awt.Container;
import java.awt.Graphics;

class EmptyContainer extends Container
{

    EmptyContainer()
    {
    }

    public void update(Graphics g)
    {
        paint(g);
    }
}
