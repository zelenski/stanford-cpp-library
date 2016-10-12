// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GOval.java

package acm.graphics;

import acm.util.ErrorException;
import java.awt.*;
import java.awt.geom.*;

// Referenced classes of package acm.graphics:
//            GObject, GRectangle, GDimension, GFillable, 
//            GResizable

public class GOval extends GObject
    implements GFillable, GResizable
{

    public GOval(double d, double d1)
    {
        this(0.0D, 0.0D, d, d1);
    }

    public GOval(double d, double d1, double d2, double d3)
    {
        frameWidth = d2;
        frameHeight = d3;
        setLocation(d, d1);
    }

    public GRectangle getBounds()
    {
        Object obj = new java.awt.geom.Ellipse2D.Double(0.0D, 0.0D, frameWidth, frameHeight);
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        java.awt.Rectangle rectangle = ((Shape) (obj)).getBounds();
        return new GRectangle(rectangle.getX() + getX(), rectangle.getY() + getY(), rectangle.getWidth(), rectangle.getHeight());
    }

    public boolean contains(double d, double d1)
    {
        Object obj = new java.awt.geom.Ellipse2D.Double(0.0D, 0.0D, frameWidth, frameHeight);
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
            obj = affinetransform.createTransformedShape(((Shape) (obj)));
        return ((Shape) (obj)).contains(d - getX(), d1 - getY());
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        java.awt.geom.Ellipse2D.Double double1 = new java.awt.geom.Ellipse2D.Double(0.0D, 0.0D, frameWidth, frameHeight);
        if(isFilled())
        {
            graphics2d.setColor(getFillColor());
            graphics2d.fill(double1);
            graphics2d.setColor(getColor());
        }
        graphics2d.draw(double1);
    }

    public void setFilled(boolean flag)
    {
        isFilled = flag;
        repaint();
    }

    public boolean isFilled()
    {
        return isFilled;
    }

    public void setFillColor(Color color)
    {
        fillColor = color;
        repaint();
    }

    public Color getFillColor()
    {
        return fillColor != null ? fillColor : getColor();
    }

    public void setSize(double d, double d1)
    {
        if(getMatrix() != null)
        {
            throw new ErrorException("setSize: Object has been transformed");
        } else
        {
            frameWidth = d;
            frameHeight = d1;
            repaint();
            return;
        }
    }

    public final void setSize(GDimension gdimension)
    {
        setSize(gdimension.getWidth(), gdimension.getHeight());
    }

    public GDimension getSize()
    {
        return new GDimension(frameWidth, frameHeight);
    }

    public void setBounds(double d, double d1, double d2, double d3)
    {
        if(getMatrix() != null)
        {
            throw new ErrorException("setBounds: Object has been transformed");
        } else
        {
            frameWidth = d2;
            frameHeight = d3;
            setLocation(d, d1);
            return;
        }
    }

    public final void setBounds(GRectangle grectangle)
    {
        if(getMatrix() != null)
        {
            throw new ErrorException("setBounds: Object has been transformed");
        } else
        {
            setBounds(grectangle.getX(), grectangle.getY(), grectangle.getWidth(), grectangle.getHeight());
            return;
        }
    }

    private double frameWidth;
    private double frameHeight;
    private boolean isFilled;
    private Color fillColor;
    static final long serialVersionUID = 21L;
}
