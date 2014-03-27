// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GContainer.java

package acm.graphics;


// Referenced classes of package acm.graphics:
//            GObject, GPoint

public interface GContainer
{

    public abstract void add(GObject gobject);

    public abstract void add(GObject gobject, double d, double d1);

    public abstract void add(GObject gobject, GPoint gpoint);

    public abstract void remove(GObject gobject);

    public abstract void removeAll();

    public abstract int getElementCount();

    public abstract GObject getElement(int i);

    public abstract GObject getElementAt(double d, double d1);

    public abstract GObject getElementAt(GPoint gpoint);

    public static final int BACK_TO_FRONT = 0;
    public static final int FRONT_TO_BACK = 1;
}
