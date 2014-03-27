// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GArc.java

package acm.graphics;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Arc2D;

// Referenced classes of package acm.graphics:
//            GObject, GRectangle, GPoint, GFillable, 
//            GMath

public class GArc extends GObject
    implements GFillable
{

    public GArc(double d, double d1, double d2, double d3)
    {
        this(0.0D, 0.0D, d, d1, d2, d3);
    }

    public GArc(double d, double d1, double d2, double d3, double d4, double d5)
    {
        frameWidth = d2;
        frameHeight = d3;
        arcStart = d4;
        arcSweep = d5;
        setLocation(d, d1);
    }

    public void setStartAngle(double d)
    {
        arcStart = d;
        repaint();
    }

    public double getStartAngle()
    {
        return arcStart;
    }

    public void setSweepAngle(double d)
    {
        arcSweep = d;
        repaint();
    }

    public double getSweepAngle()
    {
        return arcSweep;
    }

    public GPoint getStartPoint()
    {
        return getArcPoint(arcStart);
    }

    public GPoint getEndPoint()
    {
        return getArcPoint(arcStart + arcSweep);
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        java.awt.geom.Arc2D.Double double1 = new java.awt.geom.Arc2D.Double(0.0D, 0.0D, frameWidth, frameHeight, arcStart, arcSweep, isFilled() ? 2 : 0);
        if(isFilled())
        {
            graphics2d.setColor(getFillColor());
            graphics2d.fill(double1);
            graphics2d.setColor(getColor());
        }
        graphics2d.draw(double1);
    }

    public GRectangle getBounds()
    {
        double d = frameWidth / 2D;
        double d1 = frameHeight / 2D;
        double d2 = getX() + d;
        double d3 = getY() + d1;
        double d4 = d2 + GMath.cosDegrees(arcStart) * d;
        double d5 = d3 - GMath.sinDegrees(arcStart) * d1;
        double d6 = d2 + GMath.cosDegrees(arcStart + arcSweep) * d;
        double d7 = d3 - GMath.sinDegrees(arcStart + arcSweep) * d1;
        double d8 = Math.min(d4, d6);
        double d9 = Math.max(d4, d6);
        double d10 = Math.min(d5, d7);
        double d11 = Math.max(d5, d7);
        if(containsAngle(0.0D))
            d9 = d2 + d;
        if(containsAngle(90D))
            d10 = d3 - d1;
        if(containsAngle(180D))
            d8 = d2 - d;
        if(containsAngle(270D))
            d11 = d3 + d1;
        if(isFilled())
        {
            d8 = Math.min(d8, d2);
            d10 = Math.min(d10, d3);
            d9 = Math.max(d9, d2);
            d11 = Math.max(d11, d3);
        }
        return new GRectangle(d8, d10, d9 - d8, d11 - d10);
    }

    public boolean contains(double d, double d1)
    {
        double d2 = frameWidth / 2D;
        double d3 = frameHeight / 2D;
        if(d2 == 0.0D || d3 == 0.0D)
            return false;
        double d4 = d - (getX() + d2);
        double d5 = d1 - (getY() + d3);
        double d6 = (d4 * d4) / (d2 * d2) + (d5 * d5) / (d3 * d3);
        if(isFilled())
        {
            if(d6 > 1.0D)
                return false;
        } else
        {
            double d7 = 2.5D / ((d2 + d3) / 2D);
            if(Math.abs(1.0D - d6) > d7)
                return false;
        }
        return containsAngle(GMath.toDegrees(Math.atan2(-d5, d4)));
    }

    public void setFrameRectangle(double d, double d1, double d2, double d3)
    {
        frameWidth = d2;
        frameHeight = d3;
        setLocation(d, d1);
    }

    public final void setFrameRectangle(GRectangle grectangle)
    {
        setFrameRectangle(grectangle.getX(), grectangle.getY(), grectangle.getWidth(), grectangle.getHeight());
    }

    public GRectangle getFrameRectangle()
    {
        return new GRectangle(getX(), getY(), frameWidth, frameHeight);
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

    public String paramString()
    {
        String s = super.paramString();
        s = s.substring(s.indexOf(')') + 1);
        GRectangle grectangle = getFrameRectangle();
        String s1 = (new StringBuilder()).append("frame=(").append(grectangle.getX()).append(", ").append(grectangle.getY()).append(", ").append(grectangle.getWidth()).append(", ").append(grectangle.getHeight()).append(")").toString();
        s1 = (new StringBuilder()).append(s1).append(", start=").append(arcStart).append(", sweep=").append(arcSweep).toString();
        return (new StringBuilder()).append(s1).append(s).toString();
    }

    private GPoint getArcPoint(double d)
    {
        double d1 = frameWidth / 2D;
        double d2 = frameHeight / 2D;
        double d3 = getX() + d1;
        double d4 = getY() + d2;
        return new GPoint(d3 + d1 * GMath.cosDegrees(d), d4 - d2 * GMath.sinDegrees(d));
    }

    private boolean containsAngle(double d)
    {
        double d1 = Math.min(getStartAngle(), getStartAngle() + getSweepAngle());
        double d2 = Math.abs(getSweepAngle());
        if(d2 >= 360D)
            return true;
        d = d >= 0.0D ? d % 360D : 360D - -d % 360D;
        d1 = d1 >= 0.0D ? d1 % 360D : 360D - -d1 % 360D;
        if(d1 + d2 > 360D)
            return d >= d1 || d <= (d1 + d2) - 360D;
        else
            return d >= d1 && d <= d1 + d2;
    }

    public static final double ARC_TOLERANCE = 2.5D;
    private double frameWidth;
    private double frameHeight;
    private double arcStart;
    private double arcSweep;
    private Color fillColor;
    private boolean isFilled;
    static final long serialVersionUID = 21L;
}
