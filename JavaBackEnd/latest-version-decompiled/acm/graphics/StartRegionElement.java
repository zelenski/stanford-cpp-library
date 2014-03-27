// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.*;

// Referenced classes of package acm.graphics:
//            PathElement, PathState

class StartRegionElement extends PathElement
{

    public StartRegionElement(Color color1)
    {
        color = color1;
    }

    public void paint(Graphics2D graphics2d, PathState pathstate)
    {
        pathstate.region = new Polygon();
        pathstate.fillColor = color;
    }

    private Color color;
}
