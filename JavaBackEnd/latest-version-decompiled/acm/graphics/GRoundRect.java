// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GRoundRect.java

package acm.graphics;

import java.awt.Graphics2D;
import java.awt.geom.RoundRectangle2D;

// Referenced classes of package acm.graphics:
//            GRect

public class GRoundRect extends GRect
{

    public GRoundRect(double d, double d1)
    {
        this(0.0D, 0.0D, d, d1, 10D);
    }

    public GRoundRect(double d, double d1, double d2, double d3)
    {
        this(d, d1, d2, d3, 10D);
    }

    public GRoundRect(double d, double d1, double d2, double d3, double d4)
    {
        this(d, d1, d2, d3, d4, d4);
    }

    public GRoundRect(double d, double d1, double d2, double d3, double d4, double d5)
    {
        super(d, d1, d2, d3);
        aWidth = d4;
        aHeight = d5;
    }

    public double getArcWidth()
    {
        return aWidth;
    }

    public double getArcHeight()
    {
        return aHeight;
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        java.awt.geom.RoundRectangle2D.Double double1 = new java.awt.geom.RoundRectangle2D.Double(0.0D, 0.0D, getFrameWidth(), getFrameHeight(), aWidth, aHeight);
        if(isFilled())
        {
            graphics2d.setColor(getFillColor());
            graphics2d.fill(double1);
            graphics2d.setColor(getColor());
        }
        graphics2d.draw(double1);
    }

    public static final double DEFAULT_ARC = 10D;
    private double aWidth;
    private double aHeight;
    static final long serialVersionUID = 21L;
}
