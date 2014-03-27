// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GraphicsProgram.java

package acm.program;

import acm.graphics.*;
import acm.util.ErrorException;
import java.awt.*;
import java.awt.event.*;
import java.util.EventListener;
import java.util.Iterator;

// Referenced classes of package acm.program:
//            Program, GProgramListener, GObjectProgram

public abstract class GraphicsProgram extends Program
{

    protected GraphicsProgram()
    {
        eventListener = new GProgramListener(this);
        setGCanvas(createGCanvas());
        add(gc, "Center");
        validate();
    }

    public void run()
    {
    }

    public void init()
    {
    }

    public GCanvas getGCanvas()
    {
        return gc;
    }

    public void add(GObject gobject)
    {
        gc.add(gobject);
    }

    public final void add(GObject gobject, double d, double d1)
    {
        gobject.setLocation(d, d1);
        add(gobject);
    }

    public final void add(GObject gobject, GPoint gpoint)
    {
        gobject.setLocation(gpoint);
        add(gobject);
    }

    public final void add(Component component, double d, double d1)
    {
        component.setLocation(GMath.round(d), GMath.round(d1));
        gc.add(component);
    }

    public final void add(Component component, GPoint gpoint)
    {
        add(component, gpoint.getX(), gpoint.getY());
    }

    public void remove(GObject gobject)
    {
        gc.remove(gobject);
    }

    public void removeAll()
    {
        gc.removeAll();
    }

    public int getElementCount()
    {
        return gc.getElementCount();
    }

    public GObject getElement(int i)
    {
        return gc.getElement(i);
    }

    public GObject getElementAt(double d, double d1)
    {
        return gc.getElementAt(d, d1);
    }

    public final GObject getElementAt(GPoint gpoint)
    {
        return getElementAt(gpoint.getX(), gpoint.getY());
    }

    public void setGCanvas(GCanvas gcanvas)
    {
        gc = gcanvas;
        gcanvas.addMouseListener(eventListener);
        if(eventListener.needsMouseMotionListeners())
            gcanvas.addMouseMotionListener(eventListener);
    }

    public Iterator iterator()
    {
        return gc.iterator();
    }

    public Iterator iterator(int i)
    {
        return gc.iterator(i);
    }

    public void addMouseListeners()
    {
        gc.addMouseListener(this);
        gc.addMouseMotionListener(this);
    }

    public void addMouseListeners(EventListener eventlistener)
    {
        boolean flag = false;
        if(eventlistener instanceof MouseListener)
        {
            gc.addMouseListener((MouseListener)eventlistener);
            flag = true;
        }
        if(eventlistener instanceof MouseMotionListener)
        {
            gc.addMouseMotionListener((MouseMotionListener)eventlistener);
            flag = true;
        }
        if(!flag)
            throw new ErrorException("addMouseListeners: Illegal listener");
        else
            return;
    }

    public void addKeyListeners()
    {
        gc.addKeyListener(this);
    }

    public void addKeyListeners(KeyListener keylistener)
    {
        gc.addKeyListener(keylistener);
    }

    public void waitForClick()
    {
        eventListener.waitForClick();
    }

    public void repaint()
    {
        gc.repaint();
        super.repaint();
    }

    public void removeAllComponents()
    {
        super.removeAll();
    }

    public Dimension getCentralRegionSize()
    {
        if(gc.isShowing())
            return gc.getSize();
        else
            return super.getCentralRegionSize();
    }

    public void setBackground(Color color)
    {
        super.setBackground(color);
        if(gc != null)
            gc.setBackground(color);
    }

    public static void startGraphicsProgram(GObject gobject, String as[])
    {
        GObjectProgram gobjectprogram = new GObjectProgram();
        gobjectprogram.setStartupObject(gobject);
        gobjectprogram.start(as);
    }

    protected GCanvas createGCanvas()
    {
        return new GCanvas();
    }

    protected void endHook()
    {
        gc.repaint();
    }

    protected boolean isStarted()
    {
        if(gc == null || !super.isStarted())
        {
            return false;
        } else
        {
            Dimension dimension = gc.getSize();
            return dimension != null && dimension.width != 0 && dimension.height != 0;
        }
    }

    private GCanvas gc;
    private GProgramListener eventListener;
}
