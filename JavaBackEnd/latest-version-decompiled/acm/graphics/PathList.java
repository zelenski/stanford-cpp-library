// Decompiled by Jad v1.5.8f. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.kpdus.com/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   GPen.java

package acm.graphics;

import java.awt.Graphics2D;
import java.io.Serializable;
import java.util.ArrayList;

// Referenced classes of package acm.graphics:
//            PathElement, GRectangle, FinalPathElement, PathState

class PathList
    implements Serializable
{

    public PathList()
    {
        path = new ArrayList();
    }

    public synchronized void add(PathElement pathelement)
    {
        path.add(pathelement);
    }

    public synchronized void add(PathElement pathelement, PathElement pathelement1)
    {
        path.add(pathelement);
        path.add(pathelement1);
    }

    public synchronized void remove(PathElement pathelement)
    {
        path.remove(pathelement);
    }

    public synchronized void clear()
    {
        path.clear();
    }

    public int getElementCount()
    {
        return path.size();
    }

    public PathElement getElement(int i)
    {
        return (PathElement)path.get(i);
    }

    public synchronized GRectangle getBounds(PathState pathstate)
    {
        GRectangle grectangle = new GRectangle(-1D, -1D, -1D, -1D);
        int i = path.size();
        for(int j = 0; j < i; j++)
        {
            PathElement pathelement = (PathElement)path.get(j);
            pathelement.updateBounds(grectangle, pathstate);
        }

        return grectangle;
    }

    public synchronized void mapPaint(Graphics2D graphics2d, PathState pathstate)
    {
        int i = path.size();
        for(int j = 0; j < i; j++)
        {
            PathElement pathelement = (PathElement)path.get(j);
            pathelement.paint(graphics2d, pathstate);
        }

        FINAL_PATH_ELEMENT.paint(graphics2d, pathstate);
    }

    private static final PathElement FINAL_PATH_ELEMENT = new FinalPathElement();
    private ArrayList path;

}
