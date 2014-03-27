// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPolygon.java

package acm.graphics;

import java.awt.geom.AffineTransform;
import java.awt.geom.Point2D;
import java.io.Serializable;
import java.util.ArrayList;

// Referenced classes of package acm.graphics:
//            GPoint, GRectangle, GMath

class VertexList
    implements Serializable
{

    public VertexList()
    {
        vertices = new ArrayList();
        cx = 0.0D;
        cy = 0.0D;
    }

    public VertexList(VertexList vertexlist)
    {
        this();
        for(int i = 0; i < vertexlist.vertices.size(); i++)
            vertices.add(vertexlist.vertices.get(i));

    }

    public synchronized void addVertex(double d, double d1)
    {
        cx = d;
        cy = d1;
        vertices.add(new GPoint(cx, cy));
    }

    public synchronized void addEdge(double d, double d1)
    {
        cx += d;
        cy += d1;
        vertices.add(new GPoint(cx, cy));
    }

    public void addArc(double d, double d1, double d2, double d3)
    {
        double d4 = d1 / d;
        double d5 = d / 2D;
        double d6 = d1 / 2D;
        double d7 = cx - d5 * GMath.cosDegrees(d2);
        double d8 = cy + d6 * GMath.sinDegrees(d2);
        if(d3 > 359.99000000000001D)
            d3 = 360D;
        if(d3 < -359.99000000000001D)
            d3 = -360D;
        double d9 = Math.atan2(1.0D, Math.max(d, d1));
        int i = (int)(GMath.toRadians(Math.abs(d3)) / d9);
        d9 = GMath.toRadians(d3) / (double)i;
        double d10 = GMath.toRadians(d2);
        for(int j = 0; j < i; j++)
        {
            d10 += d9;
            double d11 = d7 + d5 * Math.cos(d10);
            double d12 = d8 - d5 * Math.sin(d10) * d4;
            addVertex(d11, d12);
        }

    }

    public synchronized void add(GPoint agpoint[])
    {
        for(int i = 0; i < agpoint.length; i++)
            vertices.add(new GPoint(agpoint[i].getX(), agpoint[i].getY()));

    }

    public synchronized void remove(GPoint gpoint)
    {
        vertices.remove(gpoint);
    }

    public synchronized void clear()
    {
        vertices.clear();
    }

    public int size()
    {
        return vertices.size();
    }

    GPoint get(int i)
    {
        return (GPoint)vertices.get(i);
    }

    public GPoint getCurrentPoint()
    {
        return vertices.size() != 0 ? new GPoint(cx, cy) : null;
    }

    public synchronized GRectangle getBounds(AffineTransform affinetransform)
    {
        int i = vertices.size();
        if(i == 0)
            return new GRectangle();
        double d = 0.0D;
        double d1 = 0.0D;
        double d2 = 0.0D;
        double d3 = 0.0D;
        boolean flag = true;
        for(int j = 0; j < vertices.size(); j++)
        {
            GPoint gpoint = (GPoint)vertices.get(j);
            java.awt.geom.Point2D.Double double1 = new java.awt.geom.Point2D.Double(gpoint.getX(), gpoint.getY());
            if(affinetransform != null)
                affinetransform.transform(double1, double1);
            double d4 = double1.getX();
            double d5 = double1.getY();
            if(flag)
            {
                d = d4;
                d1 = d4;
                d2 = d5;
                d3 = d5;
                flag = false;
            } else
            {
                d = Math.min(d, d4);
                d1 = Math.max(d1, d4);
                d2 = Math.min(d2, d5);
                d3 = Math.max(d3, d5);
            }
        }

        return new GRectangle(d, d2, d1 - d, d3 - d2);
    }

    public synchronized boolean contains(double d, double d1, AffineTransform affinetransform)
    {
        int i = vertices.size();
        boolean flag = false;
        for(int j = 0; j < i; j++)
        {
            GPoint gpoint = (GPoint)vertices.get(j);
            java.awt.geom.Point2D.Double double1 = new java.awt.geom.Point2D.Double(gpoint.getX(), gpoint.getY());
            if(affinetransform != null)
                affinetransform.transform(double1, double1);
            gpoint = (GPoint)vertices.get((j + 1) % i);
            java.awt.geom.Point2D.Double double2 = new java.awt.geom.Point2D.Double(gpoint.getX(), gpoint.getY());
            if(affinetransform != null)
                affinetransform.transform(double2, double2);
            if((double1.getY() < d1 && double2.getY() >= d1 || double2.getY() < d1 && double1.getY() >= d1) && double1.getX() + ((d1 - double1.getY()) / (double2.getY() - double1.getY())) * (double2.getX() - double1.getX()) < d)
                flag = !flag;
        }

        return flag;
    }

    public void recenter()
    {
        double d = 0.0D;
        double d1 = 0.0D;
        double d2 = 0.0D;
        double d3 = 0.0D;
        boolean flag = true;
        for(int i = 0; i < vertices.size(); i++)
        {
            GPoint gpoint = (GPoint)vertices.get(i);
            if(flag)
            {
                d = gpoint.getX();
                d1 = gpoint.getX();
                d2 = gpoint.getY();
                d3 = gpoint.getY();
                flag = false;
            } else
            {
                d = Math.min(d, gpoint.getX());
                d1 = Math.max(d1, gpoint.getX());
                d2 = Math.min(d2, gpoint.getY());
                d3 = Math.max(d3, gpoint.getY());
            }
        }

        double d4 = (d + d1) / 2D;
        double d5 = (d2 + d3) / 2D;
        for(int j = 0; j < vertices.size(); j++)
        {
            GPoint gpoint1 = (GPoint)vertices.get(j);
            gpoint1.translate(-d4, -d5);
        }

    }

    private ArrayList vertices;
    private double cx;
    private double cy;
}
