// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GCanvas.java

package acm.graphics;

import java.awt.*;
import java.awt.event.MouseEvent;
import java.util.Iterator;
import javax.swing.JComponent;

// Referenced classes of package acm.graphics:
//            GObjectList, GCanvasListener, GIterator, GMouseEvent, 
//            GContainer, GObject, GPoint, GMath

public class GCanvas extends JComponent
    implements GContainer
{

    public GCanvas()
    {
        contents = new GObjectList(this);
        setBackground(Color.WHITE);
        setForeground(Color.BLACK);
        setOpaque(true);
        setAutoRepaintFlag(true);
        setNativeArcFlag(false);
        setLayout(null);
        gCanvasListener = new GCanvasListener(this);
        addMouseListener(gCanvasListener);
        addMouseMotionListener(gCanvasListener);
    }

    public void add(GObject gobject)
    {
        contents.add(gobject);
        conditionalRepaint();
    }

    public final void add(GObject gobject, double d, double d1)
    {
        gobject.setLocation(d, d1);
        add(gobject);
    }

    public final void add(GObject gobject, GPoint gpoint)
    {
        add(gobject, gpoint.getX(), gpoint.getY());
    }

    public void remove(GObject gobject)
    {
        contents.remove(gobject);
        conditionalRepaint();
    }

    public void removeAll()
    {
        contents.removeAll();
        super.removeAll();
        repaint();
    }

    public Component add(Component component)
    {
        super.add(component);
        Dimension dimension = component.getSize();
        if(dimension.width == 0 || dimension.height == 0)
        {
            Dimension dimension1 = component.getPreferredSize();
            if(dimension.width == 0)
                dimension.width = dimension1.width;
            if(dimension.height == 0)
                dimension.height = dimension1.height;
            component.setSize(dimension);
        }
        return component;
    }

    public final void add(Component component, double d, double d1)
    {
        component.setLocation(GMath.round(d), GMath.round(d1));
        add(component);
    }

    public final void add(Component component, GPoint gpoint)
    {
        add(component, gpoint.getX(), gpoint.getY());
    }

    public void remove(Component component)
    {
        super.remove(component);
        conditionalRepaint();
    }

    public int getElementCount()
    {
        return contents.getElementCount();
    }

    public GObject getElement(int i)
    {
        return contents.getElement(i);
    }

    public GObject getElementAt(double d, double d1)
    {
        return contents.getElementAt(d, d1, false);
    }

    public final GObject getElementAt(GPoint gpoint)
    {
        return getElementAt(gpoint.getX(), gpoint.getY());
    }

    public Iterator iterator()
    {
        return new GIterator(this, 0);
    }

    public Iterator iterator(int i)
    {
        return new GIterator(this, i);
    }

    public void paint(Graphics g)
    {
        if(isOpaque())
        {
            g.setColor(getBackground());
            g.fillRect(0, 0, getWidth(), getHeight());
            g.setColor(getForeground());
        }
        super.paint(g);
    }

    public void paintComponent(Graphics g)
    {
        contents.mapPaint(g);
        super.paintComponent(g);
    }

    public void setAutoRepaintFlag(boolean flag)
    {
        autoRepaint = flag;
    }

    public boolean getAutoRepaintFlag()
    {
        return autoRepaint;
    }

    public void setNativeArcFlag(boolean flag)
    {
        nativeArcFlag = flag;
    }

    public boolean getNativeArcFlag()
    {
        return nativeArcFlag;
    }

    protected void sendToFront(GObject gobject)
    {
        contents.sendToFront(gobject);
        conditionalRepaint();
    }

    protected void sendToBack(GObject gobject)
    {
        contents.sendToBack(gobject);
        conditionalRepaint();
    }

    protected void sendForward(GObject gobject)
    {
        contents.sendForward(gobject);
        conditionalRepaint();
    }

    protected void sendBackward(GObject gobject)
    {
        contents.sendBackward(gobject);
        conditionalRepaint();
    }

    protected void dispatchMouseEvent(MouseEvent mouseevent)
    {
        GObject gobject = contents.getElementAt(mouseevent.getX(), mouseevent.getY(), true);
        GMouseEvent gmouseevent = null;
        if(gobject != lastObject)
        {
            if(lastObject != null)
            {
                gmouseevent = new GMouseEvent(lastObject, 505, mouseevent);
                lastObject.fireMouseListeners(gmouseevent);
            }
            if(gobject != null)
            {
                gmouseevent = new GMouseEvent(gobject, 504, mouseevent);
                gobject.fireMouseListeners(gmouseevent);
            }
        }
        lastObject = gobject;
        if(dragObject != null)
            gobject = dragObject;
        if(gobject != null)
        {
            int i = mouseevent.getID();
            if(i != 505 && i != 504 && (i != 506 || dragObject != null))
            {
                if(i == 501)
                    dragObject = gobject;
                else
                if(i == 502)
                    dragObject = null;
                gmouseevent = new GMouseEvent(gobject, i, mouseevent);
                gobject.fireMouseListeners(gmouseevent);
            }
        }
        if(gmouseevent != null && gmouseevent.isConsumed())
            mouseevent.consume();
    }

    protected void conditionalRepaint()
    {
        if(autoRepaint)
            repaint();
    }

    protected void updateEnabledList()
    {
        contents.updateEnabledList();
    }

    static MouseEvent createMouseEvent(Object obj, int i, MouseEvent mouseevent)
    {
        return new GMouseEvent(obj, i, mouseevent);
    }

    private GCanvasListener gCanvasListener;
    private GObject lastObject;
    private GObject dragObject;
    private GObjectList contents;
    private boolean autoRepaint;
    private boolean nativeArcFlag;
}
