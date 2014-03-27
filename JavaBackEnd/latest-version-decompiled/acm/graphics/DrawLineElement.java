// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Graphics2D;
import java.awt.Polygon;

// Referenced classes of package acm.graphics:
//            PathElement, PathState, GMath, GRectangle

class DrawLineElement extends PathElement
{

    public DrawLineElement(double d, double d1)
    {
        deltaX = d;
        deltaY = d1;
    }

    public void paint(Graphics2D graphics2d, PathState pathstate)
    {
        int i = GMath.round(pathstate.sx * pathstate.cx);
        int j = GMath.round(pathstate.sy * pathstate.cy);
        pathstate.cx += deltaX;
        pathstate.cy += deltaY;
        int k = GMath.round(pathstate.sx * pathstate.cx);
        int l = GMath.round(pathstate.sy * pathstate.cy);
        if(pathstate.region == null)
            graphics2d.drawLine(i, j, k, l);
        else
            pathstate.region.addPoint(k, l);
    }

    public void updateBounds(GRectangle grectangle, PathState pathstate)
    {
        if(grectangle.getWidth() < 0.0D)
            grectangle.setBounds(pathstate.sx * pathstate.cx, pathstate.sy * pathstate.cy, 0.0D, 0.0D);
        else
            grectangle.add(pathstate.sx * pathstate.cx, pathstate.sy * pathstate.cy);
        pathstate.cx += deltaX;
        pathstate.cy += deltaY;
        grectangle.add(pathstate.sx * pathstate.cx, pathstate.sy * pathstate.cy);
    }

    private double deltaX;
    private double deltaY;
}
