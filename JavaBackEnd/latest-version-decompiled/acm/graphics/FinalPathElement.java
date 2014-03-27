// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Graphics2D;
import java.awt.Polygon;

// Referenced classes of package acm.graphics:
//            PathElement, PathState

class FinalPathElement extends PathElement
{

    public FinalPathElement()
    {
    }

    public void paint(Graphics2D graphics2d, PathState pathstate)
    {
        if(pathstate.region != null)
            graphics2d.drawPolyline(pathstate.region.xpoints, pathstate.region.ypoints, pathstate.region.npoints);
    }
}
