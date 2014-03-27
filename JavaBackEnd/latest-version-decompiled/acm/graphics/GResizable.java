// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GResizable.java

package acm.graphics;


// Referenced classes of package acm.graphics:
//            GDimension, GRectangle

public interface GResizable
{

    public abstract void setSize(double d, double d1);

    public abstract void setSize(GDimension gdimension);

    public abstract void setBounds(double d, double d1, double d2, double d3);

    public abstract void setBounds(GRectangle grectangle);
}
