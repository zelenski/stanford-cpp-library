// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GLine.java

package acm.graphics;

import java.awt.Graphics2D;
import java.awt.geom.*;

// Referenced classes of package acm.graphics:
//            GObject, GRectangle, GPoint, GMath

public class GLine extends GObject
{

    public GLine(double d, double d1, double d2, double d3)
    {
        setLocation(d, d1);
        dx = d2 - d;
        dy = d3 - d1;
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        java.awt.geom.Line2D.Double double1 = new java.awt.geom.Line2D.Double(0.0D, 0.0D, GMath.round(dx), GMath.round(dy));
        graphics2d.draw(double1);
    }

    public GRectangle getBounds()
    {
        double d = getX();
        double d1 = getY();
        double d2 = d + dx;
        double d3 = d1 + dy;
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
        {
            java.awt.geom.Point2D.Double double1 = new java.awt.geom.Point2D.Double(d, d1);
            affinetransform.transform(double1, double1);
            d = double1.getX();
            d1 = double1.getY();
            double1 = new java.awt.geom.Point2D.Double(d2, d3);
            affinetransform.transform(double1, double1);
            d2 = double1.getX();
            d3 = double1.getY();
        }
        double d4 = Math.min(d, d2);
        double d5 = Math.min(d1, d3);
        return new GRectangle(d4, d5, Math.abs(d2 - d) + 1.0D, Math.abs(d3 - d1) + 1.0D);
    }

    public void setStartPoint(double d, double d1)
    {
        dx += getX() - d;
        dy += getY() - d1;
        setLocation(d, d1);
    }

    public GPoint getStartPoint()
    {
        return getLocation();
    }

    public void setEndPoint(double d, double d1)
    {
        dx = d - getX();
        dy = d1 - getY();
        repaint();
    }

    public GPoint getEndPoint()
    {
        return new GPoint(getX() + dx, getY() + dy);
    }

    public boolean contains(double d, double d1)
    {
        double d2 = getX();
        double d3 = getY();
        double d4 = d2 + dx;
        double d5 = d3 + dy;
        AffineTransform affinetransform = getMatrix();
        if(affinetransform != null)
        {
            java.awt.geom.Point2D.Double double1 = new java.awt.geom.Point2D.Double(d2, d3);
            affinetransform.transform(double1, double1);
            d2 = double1.getX();
            d3 = double1.getY();
            double1 = new java.awt.geom.Point2D.Double(d4, d5);
            affinetransform.transform(double1, double1);
            d4 = double1.getX();
            d5 = double1.getY();
        }
        double d6 = 2.25D;
        if(distanceSquared(d, d1, d2, d3) < d6)
            return true;
        if(distanceSquared(d, d1, d4, d5) < d6)
            return true;
        if(d < Math.min(d2, d4) - 1.5D)
            return false;
        if(d > Math.max(d2, d4) + 1.5D)
            return false;
        if(d1 < Math.min(d3, d5) - 1.5D)
            return false;
        if(d1 > Math.max(d3, d5) + 1.5D)
            return false;
        if((float)d2 - (float)d4 == 0.0F && (float)d3 - (float)d5 == 0.0F)
        {
            return false;
        } else
        {
            double d7 = ((d - d2) * (d4 - d2) + (d1 - d3) * (d5 - d3)) / distanceSquared(d2, d3, d4, d5);
            return distanceSquared(d, d1, d2 + d7 * (d4 - d2), d3 + d7 * (d5 - d3)) < d6;
        }
    }

    public String paramString()
    {
        String s = super.paramString();
        s = s.substring(s.indexOf(')') + 1);
        GPoint gpoint = getStartPoint();
        String s1 = (new StringBuilder()).append("start=(").append(gpoint.getX()).append(", ").append(gpoint.getY()).append(")").toString();
        gpoint = getEndPoint();
        s1 = (new StringBuilder()).append(s1).append(", end=(").append(gpoint.getX()).append(", ").append(gpoint.getY()).append(")").toString();
        return (new StringBuilder()).append(s1).append(s).toString();
    }

    private double distanceSquared(double d, double d1, double d2, double d3)
    {
        return (d2 - d) * (d2 - d) + (d3 - d1) * (d3 - d1);
    }

    public static final double LINE_TOLERANCE = 1.5D;
    private double dx;
    private double dy;
    static final long serialVersionUID = 21L;
}
