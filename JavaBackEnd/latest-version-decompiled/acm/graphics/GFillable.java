// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GFillable.java

package acm.graphics;

import java.awt.Color;

public interface GFillable
{

    public abstract void setFilled(boolean flag);

    public abstract boolean isFilled();

    public abstract void setFillColor(Color color);

    public abstract Color getFillColor();
}
