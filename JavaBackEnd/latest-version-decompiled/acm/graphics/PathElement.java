// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Graphics2D;
import java.io.Serializable;

// Referenced classes of package acm.graphics:
//            PathState, GRectangle

abstract class PathElement
    implements Serializable
{

    public PathElement()
    {
    }

    public void paint(Graphics2D graphics2d, PathState pathstate)
    {
    }

    public void updateBounds(GRectangle grectangle, PathState pathstate)
    {
    }
}
