// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPoint.java

package acm.graphics;

import java.awt.Point;
import java.io.Serializable;

public class GPoint
    implements Serializable
{

    public GPoint()
    {
        this(0.0D, 0.0D);
    }

    public GPoint(double d, double d1)
    {
        xc = d;
        yc = d1;
    }

    public GPoint(GPoint gpoint)
    {
        this(gpoint.xc, gpoint.yc);
    }

    public GPoint(Point point)
    {
        this(point.x, point.y);
    }

    public double getX()
    {
        return xc;
    }

    public double getY()
    {
        return yc;
    }

    public void setLocation(double d, double d1)
    {
        xc = d;
        yc = d1;
    }

    public void setLocation(GPoint gpoint)
    {
        setLocation(gpoint.xc, gpoint.yc);
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

    public Point toPoint()
    {
        return new Point((int)Math.round(xc), (int)Math.round(yc));
    }

    public int hashCode()
    {
        return (new Float((float)xc)).hashCode() ^ 37 * (new Float((float)yc)).hashCode();
    }

    public boolean equals(Object obj)
    {
        if(!(obj instanceof GPoint))
        {
            return false;
        } else
        {
            GPoint gpoint = (GPoint)obj;
            return (float)xc == (float)gpoint.xc && (float)yc == (float)gpoint.yc;
        }
    }

    public String toString()
    {
        return (new StringBuilder()).append("(").append((float)xc).append(", ").append((float)yc).append(")").toString();
    }

    private double xc;
    private double yc;
    static final long serialVersionUID = 21L;
}
