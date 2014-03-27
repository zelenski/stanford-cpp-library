// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Graphics2D;
import java.awt.Polygon;

// Referenced classes of package acm.graphics:
//            PathElement, PathState

class EndRegionElement extends PathElement
{

    public EndRegionElement()
    {
    }

    public void paint(Graphics2D graphics2d, PathState pathstate)
    {
        java.awt.Color color = graphics2d.getColor();
        graphics2d.setColor(pathstate.fillColor);
        graphics2d.fillPolygon(pathstate.region.xpoints, pathstate.region.ypoints, pathstate.region.npoints);
        graphics2d.setColor(color);
        graphics2d.drawPolygon(pathstate.region.xpoints, pathstate.region.ypoints, pathstate.region.npoints);
        pathstate.region = null;
    }
}
