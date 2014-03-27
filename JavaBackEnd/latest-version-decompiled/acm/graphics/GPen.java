// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import acm.util.*;
import java.awt.*;

// Referenced classes of package acm.graphics:
//            GObject, PathList, DrawLineElement, SetLocationElement, 
//            SetColorElement, StartRegionElement, EndRegionElement, PathState, 
//            PenImage, GRectangle

public class GPen extends GObject
{

    public GPen()
    {
        penVisible = false;
        path = new PathList();
        animator = new Animator();
        setSpeed(1.0D);
        erasePath();
    }

    public GPen(double d, double d1)
    {
        this();
        setLocation(d, d1);
    }

    public void erasePath()
    {
        path.clear();
        regionOpen = false;
        regionStarted = false;
        repaint();
    }

    public void setLocation(double d, double d1)
    {
        if(regionStarted)
        {
            throw new ErrorException("It is illegal to move the pen while you are defining a filled region.");
        } else
        {
            super.setLocation(d, d1);
            animator.delay();
            return;
        }
    }

    public void drawLine(double d, double d1)
    {
        double d2 = getX();
        double d3 = getY();
        if(regionStarted)
        {
            path.add(new DrawLineElement(d, d1));
        } else
        {
            path.add(new SetLocationElement(d2, d3), new DrawLineElement(d, d1));
            regionStarted = regionOpen;
        }
        super.setLocation(d2 + d, d3 + d1);
        animator.delay();
    }

    public final void drawPolarLine(double d, double d1)
    {
        double d2 = (d1 * 3.1415926535897931D) / 180D;
        drawLine(d * Math.cos(d2), -d * Math.sin(d2));
    }

    public void setColor(Color color)
    {
        if(regionStarted)
        {
            throw new ErrorException("It is illegal to change the color while you are defining a filled region.");
        } else
        {
            path.add(new SetColorElement(color));
            super.setColor(color);
            return;
        }
    }

    public void setFillColor(Color color)
    {
        if(regionStarted)
        {
            throw new ErrorException("It is illegal to change the fill color while you are defining a filled region.");
        } else
        {
            fillColor = color;
            return;
        }
    }

    public Color getFillColor()
    {
        return fillColor != null ? fillColor : getColor();
    }

    public void startFilledRegion()
    {
        if(regionOpen)
        {
            throw new ErrorException("You are already filling a region.");
        } else
        {
            regionOpen = true;
            regionStarted = false;
            path.add(new StartRegionElement(fillColor));
            return;
        }
    }

    public void endFilledRegion()
    {
        if(!regionOpen)
        {
            throw new ErrorException("You need to call startFilledRegion before you call endFilledRegion.");
        } else
        {
            regionOpen = false;
            regionStarted = false;
            path.add(new EndRegionElement());
            repaint();
            return;
        }
    }

    public void showPen()
    {
        penVisible = true;
        repaint();
        animator.delay();
    }

    public void hidePen()
    {
        penVisible = false;
        repaint();
        animator.delay();
    }

    public boolean isPenVisible()
    {
        return penVisible;
    }

    public void setSpeed(double d)
    {
        animator.setSpeed(d);
    }

    public double getSpeed()
    {
        return animator.getSpeed();
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        graphics2d = (Graphics2D)graphics2d.create();
        Color color = getObjectColor();
        if(color != null)
            graphics2d.setColor(color);
        PathState pathstate = new PathState();
        path.mapPaint(graphics2d, pathstate);
        if(penVisible)
            drawPen(graphics2d);
    }

    public GRectangle getBounds()
    {
        PathState pathstate = new PathState();
        return path.getBounds(pathstate);
    }

    public boolean contains(double d, double d1)
    {
        return false;
    }

    public void setPenImage(Image image)
    {
        penImage = MediaTools.loadImage(image);
    }

    public Image getPenImage()
    {
        if(penImage == null)
            penImage = PenImage.getImage();
        return penImage;
    }

    protected void drawPen(Graphics g)
    {
        java.awt.Component component = getComponent();
        if(component == null)
            return;
        if(penImage == null)
            penImage = PenImage.getImage();
        int i = penImage.getWidth(component);
        int j = penImage.getHeight(component);
        int k = (int)Math.round(getX());
        int l = (int)Math.round(getY());
        g.drawImage(penImage, k - i / 2, l - j / 2, component);
    }

    protected Rectangle getPenBounds()
    {
        java.awt.Component component = getComponent();
        if(component == null)
            return new Rectangle();
        if(penImage == null)
            penImage = PenImage.getImage();
        int i = penImage.getWidth(component);
        int j = penImage.getHeight(component);
        int k = (int)Math.round(getX());
        int l = (int)Math.round(getY());
        return new Rectangle(k - i / 2, l - j / 2, i, j);
    }

    private Animator animator;
    private boolean regionOpen;
    private boolean regionStarted;
    private boolean penVisible;
    private PathList path;
    private Image penImage;
    private Color fillColor;
    static final long serialVersionUID = 21L;
}
