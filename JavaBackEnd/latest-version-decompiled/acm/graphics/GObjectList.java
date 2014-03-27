// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GContainer.java

package acm.graphics;

import java.awt.Graphics;
import java.io.Serializable;
import java.util.ArrayList;

// Referenced classes of package acm.graphics:
//            GCanvas, GObject, GRectangle, GContainer

class GObjectList
    implements Serializable
{

    public GObjectList(GContainer gcontainer)
    {
        parent = gcontainer;
        contents = new ArrayList();
        if(parent instanceof GCanvas)
            enabledList = new ArrayList();
    }

    public GObjectList(GContainer gcontainer, GObjectList gobjectlist)
    {
        parent = gcontainer;
        contents = new ArrayList();
        enabledList = new ArrayList();
        int i = gobjectlist.contents.size();
        for(int j = 0; j < i; j++)
            contents.add((GObject)((GObject)gobjectlist.contents.get(j)).clone());

    }

    public synchronized void add(GObject gobject)
    {
        if(gobject.getParent() != null)
            gobject.getParent().remove(gobject);
        gobject.setParent(parent);
        contents.add(gobject);
        if(enabledList != null && gobject.areMouseListenersEnabled())
            enabledList.add(gobject);
    }

    public synchronized void remove(GObject gobject)
    {
        contents.remove(gobject);
        gobject.setParent(null);
        if(enabledList != null)
            enabledList.remove(gobject);
    }

    public synchronized void removeAll()
    {
        contents.clear();
        if(enabledList != null)
            enabledList.clear();
    }

    public int getElementCount()
    {
        return contents.size();
    }

    public GObject getElement(int i)
    {
        return (GObject)contents.get(i);
    }

    public synchronized GObject getElementAt(double d, double d1, boolean flag)
    {
        ArrayList arraylist = flag ? enabledList : contents;
        for(int i = arraylist.size() - 1; i >= 0; i--)
        {
            GObject gobject = (GObject)arraylist.get(i);
            if(gobject.contains(d, d1))
                return gobject;
        }

        return null;
    }

    public synchronized void sendToFront(GObject gobject)
    {
        int i = contents.indexOf(gobject);
        if(i >= 0)
        {
            contents.remove(i);
            contents.add(gobject);
        }
    }

    public synchronized void sendToBack(GObject gobject)
    {
        int i = contents.indexOf(gobject);
        if(i >= 0)
        {
            contents.remove(i);
            contents.add(0, gobject);
        }
    }

    public synchronized void sendForward(GObject gobject)
    {
        int i = contents.indexOf(gobject);
        if(i >= 0)
        {
            contents.remove(i);
            contents.add(Math.min(contents.size(), i + 1), gobject);
        }
    }

    public synchronized void sendBackward(GObject gobject)
    {
        int i = contents.indexOf(gobject);
        if(i >= 0)
        {
            contents.remove(i);
            contents.add(Math.max(0, i - 1), gobject);
        }
    }

    public synchronized GRectangle getBounds()
    {
        GRectangle grectangle = new GRectangle();
        int i = contents.size();
        for(int j = 0; j < i; j++)
            if(j == 0)
                grectangle = new GRectangle(((GObject)contents.get(j)).getBounds());
            else
                grectangle.add(((GObject)contents.get(j)).getBounds());

        return grectangle;
    }

    public synchronized boolean contains(double d, double d1)
    {
        int i = contents.size();
        for(int j = 0; j < i; j++)
            if(((GObject)contents.get(j)).contains(d, d1))
                return true;

        return false;
    }

    public synchronized void mapPaint(Graphics g)
    {
        int i = contents.size();
        for(int j = 0; j < i; j++)
            ((GObject)contents.get(j)).paint(g);

    }

    public synchronized boolean areMouseListenersEnabled()
    {
        int i = contents.size();
        for(int j = 0; j < i; j++)
        {
            GObject gobject = (GObject)contents.get(j);
            if(gobject.areMouseListenersEnabled())
                return true;
        }

        return false;
    }

    public synchronized void updateEnabledList()
    {
        enabledList.clear();
        int i = contents.size();
        for(int j = 0; j < i; j++)
        {
            GObject gobject = (GObject)contents.get(j);
            if(gobject.areMouseListenersEnabled())
                enabledList.add(gobject);
        }

    }

    private transient GContainer parent;
    private ArrayList contents;
    private ArrayList enabledList;
}
