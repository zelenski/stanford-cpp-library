// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Graphics2D;
import java.awt.Polygon;

// Referenced classes of package acm.graphics:
//            PathElement, PathState, GMath, GRectangle

class SetLocationElement extends PathElement
{

    public SetLocationElement(double d, double d1)
    {
        cx = d;
        cy = d1;
    }

    public void paint(Graphics2D graphics2d, PathState pathstate)
    {
        pathstate.cx = cx;
        pathstate.cy = cy;
        if(pathstate.region != null)
            pathstate.region.addPoint(GMath.round(pathstate.sx * cx), GMath.round(pathstate.sy * cy));
    }

    public void updateBounds(GRectangle grectangle, PathState pathstate)
    {
        pathstate.cx = cx;
        pathstate.cy = cy;
    }

    private double cx;
    private double cy;
}
