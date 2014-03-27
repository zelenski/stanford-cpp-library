// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPolygon.java

package acm.graphics;

import acm.util.ErrorException;
import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.geom.Path2D;

// Referenced classes of package acm.graphics:
//            GObject, VertexList, GRectangle, GFillable, 
//            GMath, GPoint

public class GPolygon extends GObject
    implements GFillable
{

    public GPolygon()
    {
        vertices = new VertexList();
        clear();
    }

    public GPolygon(double d, double d1)
    {
        this();
        setLocation(d, d1);
    }

    public GPolygon(GPoint agpoint[])
    {
        this();
        vertices.add(agpoint);
        markAsComplete();
    }

    public void addVertex(double d, double d1)
    {
        if(complete)
        {
            throw new ErrorException("You can't add vertices to a GPolygon that has been marked as complete.");
        } else
        {
            vertices.addVertex(d, d1);
            return;
        }
    }

    public void addEdge(double d, double d1)
    {
        if(complete)
        {
            throw new ErrorException("You can't add edges to a GPolygon that has been marked as complete.");
        } else
        {
            vertices.addEdge(d, d1);
            return;
        }
    }

    public final void addPolarEdge(double d, double d1)
    {
        if(complete)
        {
            throw new ErrorException("You can't add edges to a GPolygon that has been marked as complete.");
        } else
        {
            vertices.addEdge(d * GMath.cosDegrees(d1), -d * GMath.sinDegrees(d1));
            return;
        }
    }

    public void addArc(double d, double d1, double d2, double d3)
    {
        if(complete)
        {
            throw new ErrorException("You can't add edges to a GPolygon that has been marked as complete.");
        } else
        {
            vertices.addArc(d, d1, d2, d3);
            return;
        }
    }

    public GPoint getCurrentPoint()
    {
        return vertices.getCurrentPoint();
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

    public GRectangle getBounds()
    {
        GRectangle grectangle = vertices.getBounds(getMatrix());
        return new GRectangle(grectangle.getX() + getX(), grectangle.getY() + getY(), grectangle.getWidth(), grectangle.getHeight());
    }

    public boolean contains(double d, double d1)
    {
        return vertices.contains(d - getX(), d1 - getY(), getMatrix());
    }

    protected void paint2d(Graphics2D graphics2d)
    {
        int i = vertices.size();
        java.awt.geom.Path2D.Double double1 = new java.awt.geom.Path2D.Double(0);
        double1.moveTo(vertices.get(0).getX(), vertices.get(0).getY());
        for(int j = 0; j < i; j++)
            double1.lineTo(vertices.get(j).getX(), vertices.get(j).getY());

        double1.lineTo(vertices.get(0).getX(), vertices.get(0).getY());
        if(isFilled())
        {
            graphics2d.setColor(getFillColor());
            graphics2d.fill(double1);
            graphics2d.setColor(getColor());
        }
        graphics2d.draw(double1);
    }

    public void recenter()
    {
        vertices.recenter();
    }

    public Object clone()
    {
        GPolygon gpolygon;
        gpolygon = (GPolygon)super.clone();
        gpolygon.vertices = new VertexList(gpolygon.vertices);
        return gpolygon;
        Exception exception;
        exception;
        throw new ErrorException("Impossible exception");
    }

    protected void markAsComplete()
    {
        complete = true;
    }

    protected void clear()
    {
        if(complete)
        {
            throw new ErrorException("You can't clear a GPolygon that has been marked as complete.");
        } else
        {
            vertices.clear();
            return;
        }
    }

    private VertexList vertices;
    private boolean complete;
    private boolean isFilled;
    private Color fillColor;
    static final long serialVersionUID = 21L;
}
