// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GCompound.java

package acm.graphics;

import acm.util.ErrorException;
import java.awt.Graphics2D;
import java.awt.Shape;
import java.awt.event.MouseEvent;
import java.awt.geom.*;
import java.io.PrintStream;
import java.util.Iterator;

// Referenced classes of package acm.graphics:
//            GObject, GObjectList, GIterator, GRectangle, 
//            GPoint, GContainer, GCanvas

public class GCompound extends GObject
    implements GContainer
{

    public GCompound()
    {
        contents = new GObjectList(this);
        complete = false;
    }

    public void add(GObject gobject)
    {
        if(complete)
        {
            throw new ErrorException("You can't add objects to a GCompound that has been marked as complete.");
        } else
        {
            contents.add(gobject);
            repaint();
            return;
        }
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
        if(complete)
        {
            throw new ErrorException("You can't remove objects from a GCompound that has been marked as complete.");
        } else
        {
            contents.remove(gobject);
            repaint();
            return;
        }
    }

    public void removeAll()
    {
        if(complete)
        {
            throw new ErrorException("You can't remove objects from a GCompound that has been marked as complete.");
        } else
        {
            contents.removeAll();
            repaint();
            return;
        }
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

    public Object clone()
    {
        GCompound gcompound;
        gcompound = (GCompound)super.clone();
        gcompound.contents = new GObjectList(gcompound, contents);
        for(int i = 0; i < contents.getElementCount(); i++)
        {
            System.err.println((new StringBuilder()).append("original = ").append(contents.getElement(i).hashCode()).toString());
            System.err.println((new StringBuilder()).append("clone    = ").append(gcompound.contents.getElement(i).hashCode()).toString());
        }

        return gcompound;
        Exception exception;
        exception;
        throw new ErrorException("Impossible exception");
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        contents.mapPaint(graphics2d);
    }

    public GRectangle getBounds()
    {
        GRectangle grectangle = contents.getBounds();
        Object obj = new java.awt.geom.Rectangle2D.Double(grectangle.getX(), grectangle.getY(), grectangle.getWidth(), grectangle.getHeight());
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        java.awt.Rectangle rectangle = ((Shape) (obj)).getBounds();
        return new GRectangle(getX() + rectangle.getX(), getY() + rectangle.getY(), rectangle.getWidth(), rectangle.getHeight());
    }

    public boolean contains(double d, double d1)
    {
        d -= getX();
        d1 -= getY();
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            try
            {
                affinetransform = affinetransform.createInverse();
                java.awt.geom.Point2D.Double double1 = new java.awt.geom.Point2D.Double(d, d1);
                affinetransform.transform(double1, double1);
                d = double1.getX();
                d1 = double1.getY();
            }
            catch(NoninvertibleTransformException noninvertibletransformexception)
            {
                throw new ErrorException(noninvertibletransformexception);
            }
        return contents.contains(d, d1);
    }

    public final GPoint getCanvasPoint(GPoint gpoint)
    {
        return getCanvasPoint(gpoint.getX(), gpoint.getY());
    }

    public GPoint getCanvasPoint(double d, double d1)
    {
        GCompound gcompound;
        for(Object obj = this; obj instanceof GCompound; obj = gcompound.getParent())
        {
            gcompound = (GCompound)obj;
            d += gcompound.getX();
            d1 += gcompound.getY();
        }

        return new GPoint(d, d1);
    }

    public final GPoint getLocalPoint(GPoint gpoint)
    {
        return getLocalPoint(gpoint.getX(), gpoint.getY());
    }

    public GPoint getLocalPoint(double d, double d1)
    {
        GCompound gcompound;
        for(Object obj = this; obj instanceof GCompound; obj = gcompound.getParent())
        {
            gcompound = (GCompound)obj;
            d -= gcompound.getX();
            d1 -= gcompound.getY();
        }

        return new GPoint(d, d1);
    }

    public void markAsComplete()
    {
        complete = true;
    }

    protected void sendToFront(GObject gobject)
    {
        contents.sendToFront(gobject);
        repaint();
    }

    protected void sendToBack(GObject gobject)
    {
        contents.sendToBack(gobject);
        repaint();
    }

    protected void sendForward(GObject gobject)
    {
        contents.sendForward(gobject);
        repaint();
    }

    protected void sendBackward(GObject gobject)
    {
        contents.sendBackward(gobject);
        repaint();
    }

    protected void fireMouseListeners(MouseEvent mouseevent)
    {
        if(super.areMouseListenersEnabled())
        {
            super.fireMouseListeners(mouseevent);
            return;
        }
        GPoint gpoint = new GPoint((double)mouseevent.getX() - getX(), (double)mouseevent.getY() - getY());
        GObject gobject = getElementAt(gpoint);
        MouseEvent mouseevent1 = null;
        if(gobject != lastObject)
        {
            if(lastObject != null)
            {
                mouseevent1 = GCanvas.createMouseEvent(lastObject, 505, mouseevent);
                lastObject.fireMouseListeners(mouseevent1);
            }
            if(gobject != null)
            {
                mouseevent1 = GCanvas.createMouseEvent(gobject, 504, mouseevent);
                gobject.fireMouseListeners(mouseevent1);
            }
        }
        lastObject = gobject;
        if(dragObject != null)
            gobject = dragObject;
        if(gobject != null)
        {
            int i = mouseevent.getID();
            if(i != 505 && i != 504)
            {
                if(i == 501)
                    dragObject = gobject;
                else
                if(i == 502)
                    dragObject = null;
                mouseevent1 = GCanvas.createMouseEvent(gobject, i, mouseevent);
                gobject.fireMouseListeners(mouseevent1);
            }
        }
        if(mouseevent1 != null && mouseevent1.isConsumed())
            mouseevent.consume();
    }

    protected boolean areMouseListenersEnabled()
    {
        if(super.areMouseListenersEnabled())
            return true;
        else
            return contents.areMouseListenersEnabled();
    }

    private boolean complete;
    private GObjectList contents;
    private transient GObject lastObject;
    private transient GObject dragObject;
    static final long serialVersionUID = 21L;
}
