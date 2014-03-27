// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GDimension.java

package acm.graphics;

import java.awt.Dimension;
import java.io.Serializable;

public class GDimension
    implements Serializable
{

    public GDimension()
    {
        this(0.0D, 0.0D);
    }

    public GDimension(double d, double d1)
    {
        width = d;
        height = d1;
    }

    public GDimension(GDimension gdimension)
    {
        this(gdimension.width, gdimension.height);
    }

    public GDimension(Dimension dimension)
    {
        this(dimension.width, dimension.height);
    }

    public double getWidth()
    {
        return width;
    }

    public double getHeight()
    {
        return height;
    }

    public void setSize(double d, double d1)
    {
        width = d;
        height = d1;
    }

    public void setSize(GDimension gdimension)
    {
        setSize(gdimension.width, gdimension.height);
    }

    public GDimension getSize()
    {
        return new GDimension(width, height);
    }

    public Dimension toDimension()
    {
        return new Dimension((int)Math.round(width), (int)Math.round(height));
    }

    public int hashCode()
    {
        return (new Float((float)width)).hashCode() ^ 37 * (new Float((float)height)).hashCode();
    }

    public boolean equals(Object obj)
    {
        if(!(obj instanceof GDimension))
        {
            return false;
        } else
        {
            GDimension gdimension = (GDimension)obj;
            return (float)width == (float)gdimension.width && (float)height == (float)gdimension.height;
        }
    }

    public String toString()
    {
        return (new StringBuilder()).append("(").append((float)width).append("x").append((float)height).append(")").toString();
    }

    private double width;
    private double height;
    static final long serialVersionUID = 21L;
}
