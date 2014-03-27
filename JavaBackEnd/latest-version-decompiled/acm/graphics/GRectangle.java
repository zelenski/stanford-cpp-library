// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GRectangle.java

package acm.graphics;

import java.awt.Rectangle;
import java.io.Serializable;

// Referenced classes of package acm.graphics:
//            GPoint, GDimension, GMath

public class GRectangle
    implements Serializable
{

    public GRectangle()
    {
        this(0.0D, 0.0D, 0.0D, 0.0D);
    }

    public GRectangle(double d, double d1, double d2, double d3)
    {
        xc = d;
        yc = d1;
        width = d2;
        height = d3;
    }

    public GRectangle(double d, double d1)
    {
        this(0.0D, 0.0D, d, d1);
    }

    public GRectangle(GPoint gpoint, GDimension gdimension)
    {
        this(gpoint.getX(), gpoint.getY(), gdimension.getWidth(), gdimension.getHeight());
    }

    public GRectangle(GPoint gpoint)
    {
        this(gpoint.getX(), gpoint.getY(), 0.0D, 0.0D);
    }

    public GRectangle(GDimension gdimension)
    {
        this(0.0D, 0.0D, gdimension.getWidth(), gdimension.getHeight());
    }

    public GRectangle(GRectangle grectangle)
    {
        this(grectangle.xc, grectangle.yc, grectangle.width, grectangle.height);
    }

    public double getX()
    {
        return xc;
    }

    public double getY()
    {
        return yc;
    }

    public double getWidth()
    {
        return width;
    }

    public double getHeight()
    {
        return height;
    }

    public void setBounds(double d, double d1, double d2, double d3)
    {
        xc = d;
        yc = d1;
        width = d2;
        height = d3;
    }

    public void setBounds(GPoint gpoint, GDimension gdimension)
    {
        setBounds(gpoint.getX(), gpoint.getY(), gdimension.getWidth(), gdimension.getHeight());
    }

    public void setBounds(GRectangle grectangle)
    {
        setBounds(grectangle.xc, grectangle.yc, grectangle.width, grectangle.height);
    }

    public GRectangle getBounds()
    {
        return new GRectangle(this);
    }

    public void setLocation(double d, double d1)
    {
        xc = d;
        yc = d1;
    }

    public void setLocation(GPoint gpoint)
    {
        setLocation(gpoint.getX(), gpoint.getY());
    }

    public GPoint getLocation()
    {
        return new GPoint(xc, yc);
    }

    public void translate(double d, double d1)
    {
        xc += d;
        yc += d1;
    }

    public void setSize(double d, double d1)
    {
        width = d;
        height = d1;
    }

    public void setSize(GDimension gdimension)
    {
        setSize(gdimension.getWidth(), gdimension.getHeight());
    }

    public GDimension getSize()
    {
        return new GDimension(width, height);
    }

    public void grow(double d, double d1)
    {
        xc -= d;
        yc -= d1;
        width += 2D * d;
        height += 2D * d1;
    }

    public boolean isEmpty()
    {
        return width <= 0.0D || height <= 0.0D;
    }

    public boolean contains(double d, double d1)
    {
        return d >= xc && d1 >= yc && d < xc + width && d1 < yc + height;
    }

    public boolean contains(GPoint gpoint)
    {
        return contains(gpoint.getX(), gpoint.getY());
    }

    public boolean intersects(GRectangle grectangle)
    {
        GRectangle grectangle1 = this;
        if(grectangle1.xc > grectangle.xc + grectangle.width)
            return false;
        if(grectangle1.yc > grectangle.yc + grectangle.height)
            return false;
        if(grectangle.xc > grectangle1.xc + grectangle1.width)
            return false;
        return grectangle.yc <= grectangle1.yc + grectangle1.height;
    }

    public GRectangle intersection(GRectangle grectangle)
    {
        GRectangle grectangle1 = this;
        double d = Math.max(grectangle1.xc, grectangle.xc);
        double d1 = Math.max(grectangle1.yc, grectangle.yc);
        double d2 = Math.min(grectangle1.xc + grectangle1.width, grectangle.xc + grectangle.width);
        double d3 = Math.min(grectangle1.yc + grectangle1.height, grectangle.yc + grectangle.height);
        return new GRectangle(d, d1, d2 - d, d3 - d1);
    }

    public GRectangle union(GRectangle grectangle)
    {
        if(isEmpty())
            return new GRectangle(grectangle);
        if(grectangle.isEmpty())
        {
            return new GRectangle(this);
        } else
        {
            GRectangle grectangle1 = this;
            double d = Math.min(grectangle1.xc, grectangle.xc);
            double d1 = Math.min(grectangle1.yc, grectangle.yc);
            double d2 = Math.max(grectangle1.xc + grectangle1.width, grectangle.xc + grectangle.width);
            double d3 = Math.max(grectangle1.yc + grectangle1.height, grectangle.yc + grectangle.height);
            return new GRectangle(d, d1, d2 - d, d3 - d1);
        }
    }

    public void add(GRectangle grectangle)
    {
        if(grectangle.isEmpty())
            return;
        if(isEmpty())
        {
            setBounds(grectangle);
            return;
        } else
        {
            double d = Math.max(xc + width, grectangle.xc + grectangle.width);
            double d1 = Math.max(yc + height, grectangle.yc + grectangle.height);
            xc = Math.min(grectangle.xc, xc);
            yc = Math.min(grectangle.yc, yc);
            width = d - xc;
            height = d1 - yc;
            return;
        }
    }

    public void add(double d, double d1)
    {
        if(isEmpty())
        {
            setBounds(d, d1, 0.0D, 0.0D);
            return;
        } else
        {
            double d2 = Math.max(d + width, d);
            double d3 = Math.max(d1 + height, d1);
            xc = Math.min(d, xc);
            yc = Math.min(d1, yc);
            width = d2 - xc;
            height = d3 - yc;
            return;
        }
    }

    public Rectangle toRectangle()
    {
        return new Rectangle(GMath.round(xc), GMath.round(yc), GMath.round(width), GMath.round(height));
    }

    public int hashCode()
    {
        int i = (new Float((float)xc)).hashCode();
        i = 37 * i ^ (new Float((float)yc)).hashCode();
        i = 37 * i ^ (new Float((float)width)).hashCode();
        i = 37 * i ^ (new Float((float)height)).hashCode();
        return i;
    }

    public boolean equals(Object obj)
    {
        if(!(obj instanceof GRectangle))
            return false;
        GRectangle grectangle = (GRectangle)obj;
        if((float)xc != (float)grectangle.xc)
            return false;
        if((float)yc != (float)grectangle.yc)
            return false;
        if((float)width != (float)grectangle.width)
            return false;
        return (float)height == (float)grectangle.height;
    }

    public String toString()
    {
        return (new StringBuilder()).append("[").append((float)xc).append(", ").append((float)yc).append(", ").append((float)width).append("x").append((float)height).append("]").toString();
    }

    private double xc;
    private double yc;
    private double width;
    private double height;
    static final long serialVersionUID = 21L;
}
