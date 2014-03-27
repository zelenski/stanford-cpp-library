// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GObject.java

package acm.graphics;

import acm.util.ErrorException;
import acm.util.JTFTools;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.io.Serializable;
import java.lang.reflect.Method;

// Referenced classes of package acm.graphics:
//            GPoint, GDimension, GCanvas, GCompound, 
//            GResizable, GFillable, GScalable, GRectangle, 
//            GMath, GContainer

public abstract class GObject
    implements Cloneable, Serializable, GScalable
{

    protected GObject()
    {
        matrix = null;
        lineWidth = 1.0D;
        isVisible = true;
        mouseListenersEnabled = false;
    }

    public void paint(Graphics g)
    {
        if(isVisible)
        {
            Graphics2D graphics2d = createTransformedGraphics(g);
            paint2d(graphics2d);
            graphics2d.dispose();
        }
    }

    protected abstract void paint2d(Graphics2D graphics2d);

    public abstract GRectangle getBounds();

    public void setLocation(double d, double d1)
    {
        xc = d;
        yc = d1;
        repaint();
    }

    public final void setLocation(GPoint gpoint)
    {
        setLocation(gpoint.getX(), gpoint.getY());
    }

    public GPoint getLocation()
    {
        return new GPoint(xc, yc);
    }

    public double getX()
    {
        return xc;
    }

    public double getY()
    {
        return yc;
    }

    public void move(double d, double d1)
    {
        setLocation(xc + d, yc + d1);
    }

    public final void movePolar(double d, double d1)
    {
        double d2 = (d1 * 3.1415926535897931D) / 180D;
        move(d * Math.cos(d2), -d * Math.sin(d2));
    }

    public GDimension getSize()
    {
        GRectangle grectangle = getBounds();
        return new GDimension(grectangle.getWidth(), grectangle.getHeight());
    }

    public double getWidth()
    {
        return getBounds().getWidth();
    }

    public double getHeight()
    {
        return getBounds().getHeight();
    }

    public boolean contains(double d, double d1)
    {
        return false;
    }

    public final boolean contains(GPoint gpoint)
    {
        return contains(gpoint.getX(), gpoint.getY());
    }

    public void sendToFront()
    {
        if(compoundParent != null)
            compoundParent.sendToFront(this);
        else
        if(transientParent instanceof GCanvas)
            ((GCanvas)transientParent).sendToFront(this);
        else
        if(transientParent != null)
            try
            {
                Class class1 = transientParent.getClass();
                Class aclass[] = {
                    Class.forName("acm.graphics.GObject")
                };
                Object aobj[] = {
                    this
                };
                Method method = class1.getMethod("sendToFront", aclass);
                if(method != null)
                    method.invoke(transientParent, aobj);
            }
            catch(Exception exception) { }
        if(mouseListenersEnabled)
            updateEnabledList();
    }

    public void sendToBack()
    {
        if(compoundParent != null)
            compoundParent.sendToBack(this);
        else
        if(transientParent instanceof GCanvas)
            ((GCanvas)transientParent).sendToBack(this);
        else
        if(transientParent != null)
            try
            {
                Class class1 = transientParent.getClass();
                Class aclass[] = {
                    Class.forName("acm.graphics.GObject")
                };
                Object aobj[] = {
                    this
                };
                Method method = class1.getMethod("sendToBack", aclass);
                if(method != null)
                    method.invoke(transientParent, aobj);
            }
            catch(Exception exception) { }
        if(mouseListenersEnabled)
            updateEnabledList();
    }

    public void sendForward()
    {
        if(compoundParent != null)
            compoundParent.sendForward(this);
        else
        if(transientParent instanceof GCanvas)
            ((GCanvas)transientParent).sendForward(this);
        else
        if(transientParent != null)
            try
            {
                Class class1 = transientParent.getClass();
                Class aclass[] = {
                    Class.forName("acm.graphics.GObject")
                };
                Object aobj[] = {
                    this
                };
                Method method = class1.getMethod("sendForward", aclass);
                if(method != null)
                    method.invoke(transientParent, aobj);
            }
            catch(Exception exception) { }
        if(mouseListenersEnabled)
            updateEnabledList();
    }

    public void sendBackward()
    {
        if(compoundParent != null)
            compoundParent.sendBackward(this);
        else
        if(transientParent instanceof GCanvas)
            ((GCanvas)transientParent).sendBackward(this);
        else
        if(transientParent != null)
            try
            {
                Class class1 = transientParent.getClass();
                Class aclass[] = {
                    Class.forName("acm.graphics.GObject")
                };
                Object aobj[] = {
                    this
                };
                Method method = class1.getMethod("sendBackward", aclass);
                if(method != null)
                    method.invoke(transientParent, aobj);
            }
            catch(Exception exception) { }
        if(mouseListenersEnabled)
            updateEnabledList();
    }

    public void setColor(Color color)
    {
        objectColor = color;
        repaint();
    }

    public Color getColor()
    {
        GObject gobject;
        for(gobject = this; gobject.objectColor == null;)
        {
            GContainer gcontainer = gobject.getParent();
            if(gcontainer instanceof GObject)
                gobject = (GObject)gcontainer;
            else
            if(gcontainer instanceof Component)
                return ((Component)gcontainer).getForeground();
            else
                return Color.BLACK;
        }

        return gobject.objectColor;
    }

    public void setLineWidth(double d)
    {
        lineWidth = d;
        repaint();
    }

    public double getLineWidth()
    {
        return lineWidth;
    }

    public void rotate(double d)
    {
        if(matrix == null)
            matrix = new AffineTransform();
        matrix.rotate(-GMath.toRadians(d));
        repaint();
    }

    public void scale(double d, double d1)
    {
        if(matrix == null)
            matrix = new AffineTransform();
        matrix.scale(d, d1);
        repaint();
    }

    public final void scale(double d)
    {
        scale(d, d);
    }

    public void setVisible(boolean flag)
    {
        isVisible = flag;
        repaint();
    }

    public boolean isVisible()
    {
        return isVisible;
    }

    public String toString()
    {
        String s = getClass().getName();
        if(s.startsWith("acm.graphics."))
            s = s.substring("acm.graphics.".length());
        return (new StringBuilder()).append(s).append("[").append(paramString()).append("]").toString();
    }

    public GContainer getParent()
    {
        return ((GContainer) (compoundParent == null ? transientParent : compoundParent));
    }

    public void pause(double d)
    {
        JTFTools.pause(d);
    }

    public void addMouseListener(MouseListener mouselistener)
    {
        mouseListener = AWTEventMulticaster.add(mouseListener, mouselistener);
        mouseListenersEnabled = true;
        updateEnabledList();
    }

    public void removeMouseListener(MouseListener mouselistener)
    {
        mouseListener = AWTEventMulticaster.remove(mouseListener, mouselistener);
    }

    public void addMouseMotionListener(MouseMotionListener mousemotionlistener)
    {
        mouseMotionListener = AWTEventMulticaster.add(mouseMotionListener, mousemotionlistener);
        mouseListenersEnabled = true;
        updateEnabledList();
    }

    public void removeMouseMotionListener(MouseMotionListener mousemotionlistener)
    {
        mouseMotionListener = AWTEventMulticaster.remove(mouseMotionListener, mousemotionlistener);
    }

    public void addActionListener(ActionListener actionlistener)
    {
        actionListener = AWTEventMulticaster.add(actionListener, actionlistener);
    }

    public void removeActionListener(ActionListener actionlistener)
    {
        actionListener = AWTEventMulticaster.remove(actionListener, actionlistener);
    }

    public void fireActionEvent(String s)
    {
        fireActionEvent(new ActionEvent(this, 1001, s));
    }

    public void fireActionEvent(ActionEvent actionevent)
    {
        if(actionListener != null)
            actionListener.actionPerformed(actionevent);
    }

    public Object clone()
    {
        GObject gobject;
        gobject = (GObject)super.clone();
        gobject.compoundParent = null;
        gobject.transientParent = null;
        gobject.mouseListener = null;
        gobject.mouseMotionListener = null;
        gobject.actionListener = null;
        if(gobject.matrix != null)
            gobject.matrix = (AffineTransform)matrix.clone();
        return gobject;
        Exception exception;
        exception;
        throw new ErrorException("Impossible exception");
    }

    public void setParent(GContainer gcontainer)
    {
        if(gcontainer instanceof GCompound)
            compoundParent = (GCompound)gcontainer;
        else
            transientParent = gcontainer;
    }

    protected void fireMouseListeners(MouseEvent mouseevent)
    {
        switch(mouseevent.getID())
        {
        default:
            break;

        case 501: 
            if(mouseListener != null)
                mouseListener.mousePressed(mouseevent);
            break;

        case 502: 
            if(mouseListener != null)
                mouseListener.mouseReleased(mouseevent);
            break;

        case 500: 
            if(mouseListener != null)
                mouseListener.mouseClicked(mouseevent);
            break;

        case 505: 
            if(mouseListener != null)
                mouseListener.mouseExited(mouseevent);
            break;

        case 504: 
            if(mouseListener != null)
                mouseListener.mouseEntered(mouseevent);
            break;

        case 503: 
            if(mouseMotionListener != null)
                mouseMotionListener.mouseMoved(mouseevent);
            break;

        case 506: 
            if(mouseMotionListener != null)
                mouseMotionListener.mouseDragged(mouseevent);
            break;
        }
    }

    protected boolean areMouseListenersEnabled()
    {
        return mouseListenersEnabled;
    }

    protected void start()
    {
        start(null);
    }

    protected void start(String as[])
    {
        try
        {
            Class class1 = Class.forName("acm.program.GraphicsProgram");
            Class class2 = Class.forName("acm.graphics.GObject");
            Class aclass[] = {
                class2, as.getClass()
            };
            Object aobj[] = {
                this, as
            };
            Method method = class1.getMethod("startGraphicsProgram", aclass);
            method.invoke(null, aobj);
        }
        catch(Exception exception)
        {
            throw new ErrorException(exception);
        }
    }

    protected Color getObjectColor()
    {
        return objectColor;
    }

    protected AffineTransform getMatrix()
    {
        return matrix;
    }

    protected Graphics2D createTransformedGraphics(Graphics g)
    {
        Graphics2D graphics2d = (Graphics2D)g.create();
        Color color = getObjectColor();
        if(color != null)
            graphics2d.setColor(color);
        graphics2d.translate(getX(), getY());
        graphics2d.setStroke(new BasicStroke((float)lineWidth));
        if(matrix != null)
            graphics2d.transform(matrix);
        graphics2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        return graphics2d;
    }

    protected String paramString()
    {
        String s = "";
        if(this instanceof GResizable)
        {
            GRectangle grectangle = getBounds();
            s = (new StringBuilder()).append(s).append("bounds=(").append(grectangle.getX()).append(", ").append(grectangle.getY()).append(", ").append(grectangle.getWidth()).append(", ").append(grectangle.getHeight()).append(")").toString();
        } else
        {
            GPoint gpoint = getLocation();
            s = (new StringBuilder()).append(s).append("location=(").append(gpoint.getX()).append(", ").append(gpoint.getY()).append(")").toString();
        }
        if(objectColor != null)
            s = (new StringBuilder()).append(s).append(", color=").append(colorName(objectColor)).toString();
        if(this instanceof GFillable)
        {
            s = (new StringBuilder()).append(s).append(", filled=").append(((GFillable)this).isFilled()).toString();
            Color color = ((GFillable)this).getFillColor();
            if(color != null && color != objectColor)
                s = (new StringBuilder()).append(s).append(", fillColor=").append(colorName(color)).toString();
        }
        return s;
    }

    protected static String colorName(Color color)
    {
        if(color.equals(Color.BLACK))
            return "BLACK";
        if(color.equals(Color.BLUE))
            return "BLUE";
        if(color.equals(Color.CYAN))
            return "CYAN";
        if(color.equals(Color.DARK_GRAY))
            return "DARK_GRAY";
        if(color.equals(Color.GRAY))
            return "GRAY";
        if(color.equals(Color.GREEN))
            return "GREEN";
        if(color.equals(Color.LIGHT_GRAY))
            return "LIGHT_GRAY";
        if(color.equals(Color.MAGENTA))
            return "MAGENTA";
        if(color.equals(Color.ORANGE))
            return "ORANGE";
        if(color.equals(Color.PINK))
            return "PINK";
        if(color.equals(Color.RED))
            return "RED";
        if(color.equals(Color.WHITE))
            return "WHITE";
        if(color.equals(Color.YELLOW))
        {
            return "YELLOW";
        } else
        {
            int i = color.getRGB() & 0xffffff;
            return (new StringBuilder()).append("0x").append(Integer.toString(i, 16).toUpperCase()).toString();
        }
    }

    protected Component getComponent()
    {
        GContainer gcontainer;
        for(gcontainer = getParent(); gcontainer instanceof GObject; gcontainer = ((GObject)gcontainer).getParent());
        return (gcontainer instanceof Component) ? (Component)gcontainer : null;
    }

    protected void updateEnabledList()
    {
        Component component = getComponent();
        if(component instanceof GCanvas)
            ((GCanvas)component).updateEnabledList();
    }

    protected void repaint()
    {
        GContainer gcontainer;
        for(gcontainer = getParent(); gcontainer instanceof GObject; gcontainer = ((GObject)gcontainer).getParent());
        if(gcontainer instanceof GCanvas)
            ((GCanvas)gcontainer).conditionalRepaint();
    }

    private GCompound compoundParent;
    private AffineTransform matrix;
    private Color objectColor;
    private double lineWidth;
    private double xc;
    private double yc;
    private boolean isVisible;
    private boolean mouseListenersEnabled;
    private transient MouseListener mouseListener;
    private transient MouseMotionListener mouseMotionListener;
    private transient ActionListener actionListener;
    private transient GContainer transientParent;
    static final long serialVersionUID = 21L;
}
